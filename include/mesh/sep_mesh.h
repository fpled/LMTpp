#ifndef SEP_MESH_H
#define SEP_MESH_H

#include "../containers/vec.h"

namespace LMT {

template<class TM>
struct SepMesh {
    SepMesh( TM &m ) {
        created_nodes.resize( m.node_list.size(), 0 );
    }

    template<class TE0,class TL1,class TL2>
    void operator()( TE0 &e0, TM &m, const TL1 &ls_crack, const TL2 &ls_front ) {
        if ( ls_crack( center( e0 ) ) >= 0 )
            return;
        SimpleConstIterator<typename TM::EA *> iter = m.get_elem_neighbours( e0 );
        for( unsigned num_nei = 0; num_nei < iter.size(); ++num_nei ) {
            typename TM::EA &e1 = *iter[ num_nei ];
            if ( ls_front( center( e1 ) ) >= 0 or ls_crack( center( e1 ) ) < 0 )
                continue;
            for(unsigned num_n0=0;num_n0<TE0::nb_nodes;++num_n0) {
                for(unsigned num_n1=0;num_n1<e1.nb_nodes_virtual();++num_n1) {
                    if ( e0.node( num_n0 ) == e1.node_virtual( num_n1 ) ) {
                        unsigned n = e0.node( num_n0 )->number;
                        if ( not created_nodes[ n ] )
                            created_nodes[ n ] = m.add_node( e0.node( num_n0 )->pos );
                        e0.nodes[ num_n0 ] = created_nodes[ n ];
                        break;
                    }
                }
            }
        }
    }

    template<class TE0,class TL1 >
    void operator()( TE0 &e0, TM &m, const TL1 &ls_crack ) {
        if ( ls_crack( center( e0 ) ) >= 0 )
            return;
        SimpleConstIterator<typename TM::EA *> iter = m.get_elem_neighbours( e0 );
        for( unsigned num_nei = 0; num_nei < iter.size(); ++num_nei ) {
            typename TM::EA &e1 = *iter[ num_nei ];
            if ( ls_crack( center( e1 ) ) < 0 )
                continue;
            for(unsigned num_n0=0;num_n0<TE0::nb_nodes;++num_n0) {
                for(unsigned num_n1=0;num_n1<e1.nb_nodes_virtual();++num_n1) {
                    if ( e0.node( num_n0 ) == e1.node_virtual( num_n1 ) ) {
                        unsigned n = e0.node( num_n0 )->number;
                        if ( not created_nodes[ n ] )
                            created_nodes[ n ] = m.add_node( e0.node( num_n0 )->pos );
                        e0.nodes[ num_n0 ] = created_nodes[ n ];
                        break;
                    }
                }
            }
        }
    }

    Vec<typename TM::TNode *> created_nodes;
};

/**
    front < 0 to cut
*/
template<class TM,class TL1,class TL2>
void sep_mesh( TM &m, const TL1 &ls_crack, const TL2 &ls_front ) {
    m.update_elem_neighbours();
    SepMesh<TM> sm( m );
    apply( m.elem_list, sm, m, ls_crack, ls_front );
    m.signal_connectivity_change();
}

/*!
    Objectif :
        sep_mesh() sert à redéfinir la peau d' un maillage 2D ou 3D, noté m, en fonction d'une fonctionnelle définie dans le plan ou l'espace suivant la dimension du maillage. Le résultat est stocké dans m sous la forme d'un seul maillage mais avec ajout de noeuds et d'éléments à la peau aux endroits proche de la coupe.
    Remarque :
        ne pas oublier de faire m.update_skin() après l'appel de sep_mesh() pour recalculer la peau du maillage.
         
    Paramètre :
        * <strong> m </strong> est le maillage
        * <strong> ls_crack </strong> est un foncteur qui doit définir un opérateur () qui prend en paramètre un élément Bar de la forme <strong> Element< Bar, NB, TN, TD, nl> </strong> et qui renvoie un scalaire r. La barre est coupée ssi Si ce scalaire r est entre 0 et 1. Le point de coupe, disons M, sera alors le barycentre des points (pos(0);r) (pos(1);1-r) où pos(0) et pos(1) sont les extrémités de la barre. Le plus simple est d'étudier l'exemple ci-dessous.
         
         
    Exemple:
        On souhaite couper un maillage 2D par une droite.
        On définit un MeshCarac pour caractériser un maillage avec un attribut nommé <strong> flag </strong> qui nous servira à distinguer les différents parties du maillage.
        On définit une classe <strong> HyperPlan </strong> qui représente une droite du plan ( ou un plan de l'espace ). Cette classe servira aussi de foncteur pour couper un maillage grâce à l'opérateur parenthèse <strong> operator()( const Element<Bar,...> &e ) </strong> 
        
        Commençons par le MeshCarac contenu dans le fichier Python formulation_test_sep_mesh.py ( que vous créerez ) :
        \code Python
            # -*- coding: utf-8 -*-
            from LMT.formal_lf import *
            
            write_pb(
                name = 'test_sep_mesh',
                formulations = ['test_sep_mesh'],
                elements = ['Triangle', 'Quad', 'Tetra', 'Wedge' ],
                #elements = ['Triangle','Triangle_6','Quad','Tetra','Hexa','Wedge'],
            )
        
        Et sa formulation contenue dans le fichier MeshCarac_test_sep_mesh.h.py ( que vous créerez ) :
        \code Python
            # -*- coding: utf-8 -*-
            
            flag  = Variable( default_value='1e10', unit='' )
            
            #
            def formulation():
                return 0
        
        Ensuite le code exemple avec la classe HyperPlan.
        Un point important est l'usage de la fonction \a refinement () pour couper le maillage par le plan.
        \code C/C++
            #include "mesh/make_rect.h"
            #include "mesh/mesh.h"
            #include "mesh/displayparaview.h"
            #include "mesh/sep_mesh.h"
            
            /// MeshCarac pour le test
            #include "MeshCarac_test_sep_mesh.h"
            
            using namespace LMT;
            using namespace std;
            
            
            //    Classe qui permet de :
            //        * diviser les arêtes d'un élément en fonction du plan d'équation a x + b y + c z + d = 0  i.e.   vec(n) . vec(OM) + d = 0 où //          vec(n) = ( a, b, c ) via le premier opérateur operator()
            //
            template< class T = double, unsigned _dim = 2>
            struct HyperPlan {
                static const unsigned dim = _dim;
                typedef Vec< T, _dim> Pvec;
            
                HyperPlan() : n( 0. ), d( 0. ) {}
                HyperPlan( const Pvec &normal, const Pvec &p ) { 
                    n = normal / length( normal );
                    d = - dot( n, p );
                }
            
                ///
                /// TODO
                ///
                HyperPlan( const Vec<Pvec> & pt_list ) {
            
                }
            
                template<class NB,class TN,class TD,unsigned nl> 
                T operator()( const Element< Bar, NB, TN, TD, nl> &e ) const {
                    
                    T h_a = ( *this )( e.pos( 0 ) );
                    T h_b = ( *this )( e.pos( 1 ) );
                    if ( ( h_a == 0 ) and ( h_b == 0 ) )
                        return 0; /// on ne coupe pas car la barre est dans l'hyperplan. 
                        
                    if ( ( h_a * h_b ) <= 0 )
                        return h_a / ( h_a - h_b );
                    else
                        return 0;  /// on ne coupe pas car la barre
                }
                
                /// renvoie ax+by+cz+d dans le cas de la 3D
                T operator() ( const Pvec p ) const {
                    return dot( p, n ) + d;
                }
                
                Pvec n; /// normale à l'hyperplan
                T d;    /// partie "affine"
            };
            
            int main( int argc, char **argv ) {
                static const unsigned dim = 2; 
                typedef double T;
                typedef Mesh< Mesh_carac_test_sep_mesh<T,dim> > TM; /// maillage 2D
                typedef TM::TNode TNode;
                typedef TM::Pvec Pvec;
                
            
                TM m;
                /// chargement du maillage
                Pvec mini = 0, maxi = 1;
                make_rect( m, Triangle(), mini, maxi, 11 ); 
            
                display_mesh( m );
            
                /// création d'un plan qui passe par le point P et dont la normale est nor
                //Pvec diag = maxi - mini; 
                Pvec nor( 0.15, -0.35 ); 
                Pvec P( 0.35, 0.45 );
                HyperPlan<T,dim> plan( nor, P );D 
            
                refinement( m, plan );
                sep_mesh( m, plan );
                
                for( unsigned i = 0; i < m.node_list.size(); ++i )
                    m.node_list[ i ].flag = sgn( plan( m.node_list[ i ].pos ) );
                
                display_mesh( m );
                
                return 0;
            }

    \relates level_set_cut
    \relates refinement
    \keyword Maillage/Opération
    \friend lecler@lmt.ens-cachan.fr
*/
template<class TM,class TL1 >
void sep_mesh( TM &m, const TL1 &ls_crack ) {
    m.update_elem_neighbours();
    SepMesh<TM> sm( m );
    apply( m.elem_list, sm, m, ls_crack );
    m.signal_connectivity_change();
}

} 

#endif // SEP_MESH_H
