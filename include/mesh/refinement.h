//
// C++ Interface: refinement
//
// Description:
//
//
// Author: Hugo LECLERC <leclerc@lmt.ens-cachan.fr>, (C) 2004
//         Florent PLED <florent.pled@univ-paris-est.fr>, (C) 2015
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMT_refinement_HEADER
#define LMT_refinement_HEADER

#include "mesh.h"
#include "Bar.h"
#include "Triangle.h"
#include "Quad.h"
#include "Tetra.h"
#include "Hexa.h"
#include "../util/rectilinear_iterator.h"

#include <list>

namespace LMT {

namespace LMTPRIVATE {
    /*!
        Objectif : 
            La fonction \a append_node_cut_at_bar sert à ajouter un noeud à une barre pour la découpe d'un maillage.
            
        Paramètres :
            * m est le maillage de barres que l'on souhaite couper.
            * m_parent est le maillage globale.  m est l'avant-dernier sous-maillage de m_parent.
            * cut est la liste des noeuds où l'on coupe le maillage.
            * *e est une barre du maillage m ( de type \a Element ou \a ElementAncestor ).
            * p0 et p1 sont les poids barycentriques de chaque sommet de l'arête. 
    */
    template<class TM,class TMParent, unsigned nb_elem_type, class TE, class T>
    void append_node_cut_at_bar( TM &m, 
                                 TMParent &m_parent, 
                                 DynamicData<typename TM::TNode*, nb_elem_type > &cut, 
                                 TE *e, 
                                 T p0, 
                                 T p1 ) {

        typedef typename TMParent::TNode TN;
        TN *nn = m_parent.add_node();
        m.elem_list.get_data( cut, *e ) = nn;

        std::pair< T,const TN *> pond_list[] = {
            std::pair< T,const TN *>( p0, e->node_virtual( 0 ) ),
            std::pair< T,const TN *>( p1, e->node_virtual( 1 ) )
        };
        DM::get_ponderation( pond_list, 2, *nn );
    }

    template<class TM,class TMParent,unsigned num_sub_mesh,unsigned max_num_sub_mesh>
    struct Refinement {
        typedef typename TM::TNode TNode;
        typedef Refinement<typename TM::TNext,TMParent,num_sub_mesh+1,max_num_sub_mesh> RNext;
        typedef typename TM::Tpos T;
        typedef DynamicData<TNode *,TM::TElemList::nb_elem_type> TDN;

        ///
        Refinement(TMParent *mp):m_parent(mp),cut("cut"),next(mp) {}
        
        ///
        template<class Op> struct RefineBars {
            template<class TE> void operator()(const TE &e) const {}
            template<class NB,class TN,class TD,unsigned nl> void operator()( Element<Bar,NB,TN,TD,nl> &e ) const {
                T val = (*op)(e);
                if ( val == 1 )
                    val = 0.5;
                //
                if ( val > 0 and val < 1 ) {
                    typename TMParent::TNode *nn = m_parent->add_node();
                    m->elem_list.get_data(*cut,e) = nn;

                    std::pair<typename TM::Tpos,const TN *> pond_list[] = {
                        std::pair<typename TM::Tpos,const TN *>( 1-val, e.node(0) ),
                        std::pair<typename TM::Tpos,const TN *>( val  , e.node(1) )
                    };
                    DM::get_ponderation( pond_list, 2, *nn );
                    //append_node_cut_at_bar( *m, *m_parent, *cut, &e, 1 - val, val );
                }
                else
                    m->elem_list.get_data(*cut,e) = (TNode *)NULL;
            }
            Op *op;
            TDN *cut;
            TM *m;
            TMParent *m_parent;
        };
        ///
        template<class Op> 
        void update_cut( TM &m, Op &op ) {
            // create nodes on center of Bar elements which are considered to be too long
            next.update_cut( m.next, op );

            RefineBars<Op> rb;
            rb.op = &op;
            rb.cut = &cut;
            rb.m = &m;
            rb.m_parent = m_parent;

            m.elem_list.synchronize_dyn( &cut );
            apply( m.elem_list, rb );
        }
        ///
        template<class TE>
        bool div(TE &e,const Number<0> &nnn) { return false; }

        template<class TE>
        bool div(TE &e,const Number<1> &nnn) {
            TNode *nn[] = { m_parent->elem_list.get_data(cut,e) };
            return divide_element( e, *m_parent, nn );
        }

        template<class TE>
        bool div(TE &e,const Number<2> &nnn) {
            static const unsigned nb_children = NbChildrenElement<typename TE::NE,1>::res;
            TNode *nn[ nb_children ];
            for(unsigned i = 0; i < nb_children; ++i )
                nn[i] = m_parent->template sub_mesh<1>().elem_list.get_data(next.cut, *m_parent->get_children_of(e,Number<1>())[i] );
            return divide_element( e, *m_parent, nn );
        }

        template<class TE>
        bool div(TE &e,const Number<3> &nnn) {
            static const unsigned nb_children = NbChildrenElement<typename TE::NE,2>::res;
            TNode *nn[ nb_children ];
            for(unsigned i = 0; i < nb_children; ++i )
                nn[i] = m_parent->template sub_mesh<2>().elem_list.get_data(next.next.cut, *m_parent->get_children_of(e,Number<2>())[i] );
            return divide_element( e, *m_parent, nn );
        }

        /// premier foncteur
        template<class TE>
        bool operator()( TE &e ) {
            return div(e,Number<TE::nb_var_inter>());
        }
                
        /// A tous les éléments 2D qui ont au moins deux arêtes coupées, on coupe toutes les autres arêtes.
        /// Number<dim> représente la dimension du maillage initial.
        template<class TE>
        void append_constrained_cut( TE &e, const Number<0> &nnn ) { }

        template<class TE>
        void append_constrained_cut( TE &e, const Number<1> &nnn ) { }

        template<class TE>
        void append_constrained_cut( TE &e, const Number<2> &nnn ) {
            static const unsigned nb_children = NbChildrenElement<typename TE::NE,1>::res;
            TNode *nn[ nb_children ];
            unsigned cpt_cut = 0;
            for( unsigned i = 0; i < nb_children; ++i ) {
                nn[ i ] = m_parent->template sub_mesh<1>().elem_list.get_data( next.cut, *m_parent->get_children_of( e, Number<1>() )[ i ] );
                if ( nn[ i ] )
                    cpt_cut++;
            }
            
            if ( cpt_cut >= 2 ) {
                for( unsigned i = 0; i < nb_children; ++i )
                    if ( nn[ i ] == NULL )
                        append_node_cut_at_bar( m_parent->template sub_mesh<1>(), 
                                                *m_parent, 
                                                next.cut, 
                                                m_parent->get_children_of( e, Number<1>() )[ i ], 
                                                .5, 
                                                .5 );
            }
        }
        /// pour la 3D mais le foncteur qui utilisera cette méthode parcourera des \a Triangle .
        template<class TE> 
        void append_constrained_cut( TE &e, const Number<3> &nnn ) {
            static const unsigned nb_bar = 3;
            unsigned i_n[ nb_bar ][ 2 ] = { { 0, 1 }, { 1, 2 }, { 0, 2 } }; 
            TNode *nn[ 2 ];
            TNode *middle[ nb_bar ];
            unsigned cpt_cut = 0;
        
            for( unsigned i = 0 ; i < nb_bar; ++i ) {
                nn[ 0 ] = e.node( i_n[ i ][ 0 ] );
                nn[ 1 ] = e.node( i_n[ i ][ 1 ] );            
                middle[ i ] = m_parent->template sub_mesh<2>().elem_list.get_data( next.next.cut, *m_parent->template sub_mesh<2>().elem_list.find( Bar(), DefaultBehavior(), *m_parent, nn ) );
                if ( middle[ i ] )
                    cpt_cut++;
            }
            
            if ( cpt_cut == 2 ) {
                for( unsigned i = 0; i < nb_bar; ++i )
                    if ( middle[ i ] == NULL ) {
                        nn[ 0 ] = e.node( i_n[ i ][ 0 ] );
                        nn[ 1 ] = e.node( i_n[ i ][ 1 ] );                      
                        append_node_cut_at_bar( m_parent->template sub_mesh<2>(), 
                                                *m_parent, 
                                                next.next.cut, 
                                                m_parent->template sub_mesh<2>().elem_list.find( Bar(), DefaultBehavior(), *m_parent, nn ), 
                                                .5, 
                                                .5 );
                        break;
                    }
            }
        }

        /// deuxième foncteur
        template<class TE, unsigned num>
        void operator()( TE &e, const Number<num> &nnn ) {
            append_constrained_cut( e, nnn ); 
        }
        
        ///
        struct Control_two_cuts {
            Control_two_cuts() : has_two_cuts( false ) {}
            
            bool has_two_cuts;
        };
        ///
        template<class TE> 
        void has_two_cuts( TE &e, const Number<0> &nnn, Control_two_cuts &ctrl ) { }

        template<class TE> 
        void has_two_cuts( TE &e, const Number<1> &nnn, Control_two_cuts &ctrl ) { }

        template<class TE> 
        void has_two_cuts( TE &e, const Number<2> &nnn, Control_two_cuts &ctrl ) { }
        /// pour la 3D mais le foncteur qui utilisera cette méthode parcourera des \a Triangle .
        template<class TE> 
        void has_two_cuts( TE &e, const Number<3> &nnn, Control_two_cuts &ctrl ) {
            static const unsigned nb_bar = 3;
            unsigned i_n[ nb_bar ][ 2 ] = { { 0, 1 }, { 1, 2 }, { 0, 2 } }; 
            TNode *nn[ 2 ];
            TNode *middle[ nb_bar ];
            unsigned cpt_cut = 0;
        
            for( unsigned i = 0 ; i < nb_bar; ++i ) {
                nn[ 0 ] = e.node( i_n[ i ][ 0 ] );
                nn[ 1 ] = e.node( i_n[ i ][ 1 ] );            
                middle[ i ] = m_parent->template sub_mesh<2>().elem_list.get_data( next.next.cut, *m_parent->template sub_mesh<2>().elem_list.find( Bar(), DefaultBehavior(), *m_parent, nn ) );
                if ( middle[ i ] )
                    cpt_cut++;
            }
            
            if ( cpt_cut == 2 )
                ctrl.has_two_cuts = true;
        }
        
        /// troisième foncteur
        template<class TE, unsigned num>
        void operator() ( TE &e, const Number<num> &nnn, Control_two_cuts &ctrl ) {
            has_two_cuts( e, nnn, ctrl );
        }

        /// On étend la coupe si l'arête coupée n'est pas la plus longue de l'élément.
        /// Number<dim> représente la dimension du maillage initial.
        template<class TN> 
        void spread_cut( ElementAncestor<TN> *ea, const Number<0> &nnn ) { }

        template<class TN> 
        void spread_cut( ElementAncestor<TN> *ea, const Number<1> &nnn ) { }
        
        template<class TN> 
        void spread_cut( ElementAncestor<TN> *ea, const Number<2> &nnn ) {
            typedef typename TN::T T;
            typedef ElementAncestor<TN> EA;
            
            unsigned nb_children = ea->nb_children_virtual( 1 );
            Vec<TNode *> nn; nn.resize( nb_children );
            Vec<T> ll; ll.resize( nb_children );

            T max_l_cut = 0;
            
            /// détermination des arêtes coupées et calcul de la longueur des arêtes
            for( unsigned i = 0; i < nb_children; ++i ) {
                nn[ i ] = m_parent->template sub_mesh<1>().elem_list.get_data( next.cut, *m_parent->get_children_of_EA( ea, Number<1>() )[ i ] );
                ll[ i ] = length( m_parent->get_children_of_EA( ea, Number<1>() )[ i ]->node_virtual( 1 )->pos - m_parent->get_children_of_EA( ea, Number<1>() )[ i ]->node_virtual( 0 )->pos );
                if ( ( nn[ i ] ) and ( ll[ i ] > max_l_cut ) )
                    max_l_cut = ll[ i ];
            }
            
            if ( max_l_cut > 0 ) { /// au moins une barre est coupée
            
                SimpleConstIterator< EA* > itea = m_parent->get_elem_neighbours_EA( ea );
            
                for( unsigned i = 0; i < nb_children; ++i ) {
                    EA *child = NULL;
                    /// on coupe toutes les arêtes pas déjà coupées et qui ont une longueur supérieure ou égale à la plus longue arête coupée
                    if ( ( nn[ i ] == NULL ) and ( ll[ i ] >= max_l_cut ) ) {
                        child = m_parent->get_children_of_EA( ea, Number<1>() )[ i ];
                        
                        append_node_cut_at_bar( m_parent->template sub_mesh<1>(), *m_parent, next.cut, child, .5, .5 );
                    }
                    
                    if ( child ) { /// extension aux voisins
                        /// détermination du voisin qui a aussi child comme enfant
                        bool loop = true;
                        for( SimpleConstIterator< EA*> it( itea ) ; ( bool )it and loop; ++it ) {
                            for( unsigned j = 0; j < (*it)->nb_children_virtual( 1 ); ++j ) {
                                if ( child == m_parent->get_children_of_EA( *it, Number<1>() )[ j ] ) {
                                    spread_cut( *it, nnn );
                                    loop = false;
                                    break;
                                }
                            }
                        }           
                    }
                }
            }
        }
        
        template<class TN>
        void spread_cut( ElementAncestor<TN> *ea, const Number<3> &nnn ) {
            /// le code qui suit est un malheureux copier-coller de la version du dessus ( i.e. Number<2> ) : que St Isidore me pardonne ;-)
            typedef typename TN::T T;
            typedef ElementAncestor<TN> EA;
            
            unsigned nb_children = ea->nb_children_virtual( 2 ); /// nombre d'arêtes
            Vec<TNode *> nn; nn.resize( nb_children );
            Vec<T> ll; ll.resize( nb_children );

            T max_l_cut = 0;
            unsigned nb_cut = 0;
            
            /// détermination des arêtes coupées et calcule de la longueur des arêtes
            EA* const * ppea = m_parent->get_children_of_EA( ea, Number<2>() );
            for( unsigned i = 0; i < nb_children; ++i ) {
                nn[ i ] = m_parent->template sub_mesh<2>().elem_list.get_data( next.next.cut, *( ppea[ i ] ) );
                ll[ i ] = length( m_parent->get_children_of_EA( ea, Number<2>() )[ i ]->node_virtual( 1 )->pos - ppea[ i ]->node_virtual( 0 )->pos );
                if ( ( nn[ i ] ) and ( ll[ i ] > max_l_cut ) )
                    max_l_cut = ll[ i ];
                if ( nn[ i ] )
                    nb_cut++;
            }
            
            if ( nb_cut ) { /// au moins une barre est coupée
            
                SimpleConstIterator< EA* > itea = m_parent->get_elem_neighbours_EA( ea );
            
                for( unsigned i = 0; i < nb_children; ++i ) {
                    EA *child = NULL;
                    /// on coupe toutes les arêtes pas déjà coupées et qui ont une longueur supérieure ou égale à la plus longue arête coupée
                    if ( ( nn[ i ] == NULL ) and ( ll[ i ] >= max_l_cut ) ) {
                        child = m_parent->get_children_of_EA( ea, Number<2>() )[ i ];
                        
                        append_node_cut_at_bar( m_parent->template sub_mesh<2>(), *m_parent, next.next.cut, child, .5, .5 );
                    }
                    
                    if ( child ) { /// extension aux voisins
                        /// détermination du voisin qui a aussi child comme enfant
                        bool loop = true;
                        for( SimpleConstIterator< EA*> it( itea ) ; ( bool )it and loop; ++it ) {
                            for( unsigned j = 0; j < (*it)->nb_children_virtual( 1 ); ++j ) {
                                if ( child == m_parent->get_children_of_EA( *it, Number<2>() )[ j ] ) {
                                    spread_cut( *it, nnn );
                                    loop = false;
                                    break;
                                }
                            }
                        }           
                    }
                }
            }
        }

        TMParent *m_parent;
        TDN cut;
        RNext next;
        bool appended_cut;
    };
    template<class TM,class TMParent,unsigned max_num_sub_mesh>
    struct Refinement<TM,TMParent,max_num_sub_mesh,max_num_sub_mesh> {
        Refinement(TMParent *mp) {}
        template<class T> void update_cut(const TM &m, T length_max) const {}
    };
}

/*!
    Objectif :
        La fonction \a refinement permet de raffiner un maillage. Elle divise les barres (segments) d'un maillage 1D, 2D ou 3D suivant l'opérateur op. Plus précisément, elle divise les éléments Tetra, Hexa, ... qui contiennent ces barres.

    Paramètres :
        * <strong> m </strong> est un maillage qui sera modifié si nécessaire.
        * <strong> op </strong> est un opérateur qui peut soit renvoyer un booléen soit un double. Il prend aussi forcément un élément de type barre en paramètre.
            Si c'est 0 (=false) -> on ne coupe pas.
            Si c'est 1 (=true) -> on coupe au milieu.
            Si c'est dans ] 0, 1 [ -> on coupe de façon proportionnelle (0 -> vers le noeud 0, 1 -> vers le noeud 1).
            C'est-à-dire qu'il est au moins de la forme :
            \code C/C++
                struct MyOp {
                    template<class TE>
                    RET operator()( TE &e ) const { // e un 1 var_inter (Bar, ...)
                        /// votre code
                    }
                };
            où RET est soit un type flottant, soit un booléen.
            S'il renvoie un booléen, il y a opération de division au milieu de la barre s'il renvoie vrai.
            S'il renvoie un double r, il divise la barre lorsque r est compris entre 0 et 1. La position de la division dépend de r suivant les points barycentriques ( pos(0); 1 - r ) ( pos(1); r ). Lorsque r=1, il divise au milieu. Lorsque r=0, il ne fait rien.

    Retour :
        Cette fonction renvoie vrai si elle divise au moins une barre et faux sinon.

    \relates refinement
    \keyword Maillage/Opération
    \friend lecler@lmt.ens-cachan.fr

    Subdivide each element bar e contained in mesh or sub_meshes such as op(e)==true (true means subdivision). Subdivide parents.
*/
template<class TM,class Op>
bool refinement( TM &m, Op &op, bool spread_cut = false ) {
    //m.update_elem_children();
    m.update_elem_children( Number<TM::nvi-1>() );
    if ( TM::dim == 3 )
        m.update_elem_children( Number<TM::nvi-2>() );
    LMTPRIVATE::Refinement<TM,TM,0,TM::dim+1> r( &m );
    r.update_cut( m, op );
    
    typename LMTPRIVATE::Refinement<TM,TM,0,TM::dim+1>::Control_two_cuts ctrl;
    
    do {
        ctrl.has_two_cuts = false;
        /// on raffine s'il y a au moins deux arêtes coupées par élément
        switch( TM::dim ) {
            case 2 : apply( m.elem_list, r, Number<TM::dim>() ); break;
            case 3 : apply( m.sub_mesh( Number<1>() ).elem_list, r, Number<TM::dim>() ); break; /// application sur les triangles
            default:
                assert( 0 );
        }
        
        /// on propage le raffinement au reste du maillage
        if ( spread_cut ) {
            m.update_elem_neighbours();
            for( unsigned i = 0; i < m.elem_list.size(); ++i )
                r.spread_cut( m.elem_list[ i ], Number<TM::dim>() );
        }
        
        /// contrôle s'il y a deux coupes pour la 3D ( j'espère provisoire )
        if ( TM::dim == 3 ) {
            apply( m.sub_mesh( Number<1>() ).elem_list, r, Number<TM::dim>(), ctrl ); /// application sur les triangles
        }

    } while( ctrl.has_two_cuts );

    m.elem_list.reg_dyn( &r.cut );
    bool res = m.remove_elements_if( r );
    m.elem_list.unreg_dyn( &r.cut );
    m.remove_unused_nodes();

    m.signal_connectivity_change();
    return res;
}

/*!
    L'opérateur \a LevelSetRefinement est conçu pour les fonctions \a level_set_refinement et \a level_set_cut .
*/
template<class ExtractPhi>
struct LevelSetRefinement {
    LevelSetRefinement( const ExtractPhi &_p ) : p( _p ) {}

    template<class TE>
    typename TE::T operator()( TE &e ) const {
        typename TE::T p0 = p( *e.node(0) );
        typename TE::T p1 = p( *e.node(1) );
        if ( ( p0 * p1 ) >= 0 )
            return 0;
        typename TE::T o = p0 / ( p0 - p1 );
//         if ( o < 0.1 ) {
//             e.node( 0 )->pos = e.node( 0 )->pos + ( e.node( 1 )->pos - e.node( 0 )->pos ) * o;
//             return 0;
//         }
//         if ( o > 0.9 ) {
//             e.node( 1 )->pos = e.node( 1 )->pos + ( e.node( 0 )->pos - e.node( 1 )->pos ) * ( 1 - o );
//             return 0;
//         }
        return o;
    }

    const ExtractPhi &p;
};

/*!
    Objectif :
        La fonction \a level_set_refinement permet de raffiner un maillage en fonction de la valeur d'un attribut nodal et scalaire (level-set). Plus précisément, considérons un attribut nodal scalaire ( i.e. double en général ), qu'on nommera phi. La fonction divise toutes les barres (segments) du maillage pour lesquelles les valeurs de phi aux deux noeuds sont de signe opposé ( i.e. les barres intersectées par le level-set)

    Paramètres :
        * <strong> m </strong> est un maillage qui sera modifié si nécessaire.
        * <strong> ExtractPhi </strong> est une classe qui permet d'accéder à la valeur d'un attribut du maillage m. Par exemple ce sera la classe \a ExtractDM < phi_DM > où <strong> phi </strong> est le nom de l'attribut. Remarque : il faut que le MeshCarac du maillage contienne une classe phi_DM.

    Retour :
        Cette fonction renvoie vrai si elle divise au moins une barre et faux sinon.

    Exemple de code pour raffiner un maillage avec un level-set :
    Remarque : il faut adapter le MeshCarac ( cf le MeshCarac venant juste après le code C++ ).
    \code C/C++

        #include "mesh/make_rect.h"
        #include "mesh/refinement.h"
        #include "mesh/displayparaview.h"

        // inclusion du code de notre MeshCarac
        #include "MonMeshCarac.h"

        int main( int argc, char **argv ) {
            typedef Mesh< Mesh_carac_MonMeshCarac<double,2> > TM;
            typedef TM::Pvec Pvec;
            typedef TM::TNode::T T;

            TM m;
            make_rect( m, Triangle(), Pvec( 0., 0. ), Pvec( 1., 1. ), Pvec( 20, 20 ) );

            for( unsigned i = 0 ; i < m.node_list.size(); ++i )
                m.node_list[i].phi = sin( std::sqrt( i ) * 5. );

            display_mesh( m );

            level_set_refinement( m, ExtractDM< phi_DM >() );

            display_mesh( m );

            return 0;
        }

        Commençons par le MeshCarac contenu dans le fichier Python formulation_test_sep_mesh.py ( que vous créerez ) :
        \code Python
            # -*- coding: utf-8 -*-
            from LMT.formal_lf import *

            write_pb(
                name = 'test_cut_mesh',
                formulations = ['test_cut_mesh'],
                elements = ['Triangle', 'Tetra' ]
            )

        Et sa formulation contenue dans le fichier MeshCarac_test_cut_mesh.h.py ( que vous créerez ) :
        \code Python
            # -*- coding: utf-8 -*-

            phi  = Variable( default_value='1e10', unit='' )

            #
            def formulation():
                return 0

    \relates level_set_refinement
    \relates level_set_cut
    \relates refinement
    \keyword Maillage/Opération
    \friend lecler@lmt.ens-cachan.fr
*/
template<class TM,class ExtractPhi>
bool level_set_refinement( TM &m, const ExtractPhi &p, bool spread_cut = false ) {
    LevelSetRefinement<ExtractPhi> lr( p );
    return refinement( m, lr, spread_cut );
}

/*!
    L'opérateur \a LevelSetRemoveNeg est conçu pour la fonction \a level_set_cut .
*/
template<class ExtractPhi>
struct LevelSetRemoveNeg {
    LevelSetRemoveNeg( const ExtractPhi &_p ) : p( _p ) {}

    template<class TE>
    typename TE::T operator()( const TE &e ) const {
        typename TE::T d = 0;
        for(unsigned i=0;i<TE::nb_nodes;++i)
            d += p( *e.node(i) );
        d /= TE::nb_nodes;
        return d <= 0;
    }

    const ExtractPhi &p;
};

/*!
    Objectif :
        La fonction \a level_set_cut permet de couper un maillage en fonction de la valeur d'un attribut nodal et scalaire (level-set). Plus précisément, considérons un attribut nodal scalaire ( i.e. double en général ), qu'on nommera phi. Après l'appel de la fonction, le maillage n'aura que des éléments pour lesquels la valeur de phi aux noeuds est positive.

    Paramètres :
        * <strong> m </strong> est un maillage qui sera modifié si nécessaire.
        * <strong> ExtractPhi </strong> est une classe qui permet d'accéder à la valeur d'un attribut du maillage m. Par exemple ce sera la classe \a ExtractDM < phi_DM > où <strong> phi </strong> est le nom de l'attribut. Remarque : il faut que le MeshCarac du maillage contienne une classe phi_DM.

    Retour :
        Cette fonction renvoie vrai s'il y a eu des changements et faux sinon.

    Exemple de code pour couper un maillage avec un level-set :
    Remarque : il faut adapter le MeshCarac.
    \code C/C++

        #include "mesh/make_rect.h"
        #include "mesh/refinement.h"
        #include "mesh/displayparaview.h"

        // inclusion du code de notre MeshCarac
        #include "MonMeshCarac.h"

        int main( int argc, char **argv ) {
            typedef Mesh< Mesh_carac_MonMeshCarac<double,2> > TM;
            typedef TM::Pvec Pvec;
            typedef TM::TNode::T T;

            TM m;
            make_rect( m, Triangle(), Pvec( 0., 0. ), Pvec( 1., 1. ), Pvec( 20, 20 ) );

            for( unsigned i = 0 ; i < m.node_list.size(); ++i )
                m.node_list[i].phi = sin( std::sqrt( i ) * 5. );

            display_mesh( m );

            level_set_cut( m, ExtractDM< phi_DM >() );

            display_mesh( m );

            return 0;
        }

    \relates level_set_cut
    \relates level_set_refinement
    \relates refinement
    \keyword Maillage/Opération
    \friend lecler@lmt.ens-cachan.fr
*/
template<class TM,class ExtractPhi>
bool level_set_cut( TM &m, const ExtractPhi &p, bool spread_cut = false ) {
    LevelSetRefinement<ExtractPhi> lr( p );
    refinement( m, lr, spread_cut );
    LevelSetRemoveNeg<ExtractPhi> ln( p );
    return m.remove_elements_if( ln );
}

/*!
    L'opérateur \a LevelSetImageRefinement est conçu pour la fonction \a level_set_image_refinement .
*/
template<class TIMG1,class TIMG2=TIMG1>
struct LevelSetImageRefinement {
    LevelSetImageRefinement( const TIMG1 &ls_crack, const TIMG2 &ls_front, double eps = 1e-1 ) : ls_crack( ls_crack ), ls_front( ls_front ), eps( eps ) {}

    template<class TE>
    double operator()( TE &e ) const {
        double step = 1.0 / measure( e );
        for(double x = 0; x <= 1; x += step ) {
            typename TE::Pvec P0 = e.pos(0) + ( e.pos(1) - e.pos(0) ) * ( x - step );
            if ( ls_front( P0 ) > 0 )
                return 0;
            typename TE::Pvec P1 = e.pos(0) + ( e.pos(1) - e.pos(0) ) * ( x        );
            bool s1 = ls_crack( P0 ) > 0;
            bool s2 = ls_crack( P1 ) > 0;
            if ( s1 xor s2 ) {
                if ( x < eps ) {
                    e.node( 0 )->pos = P1;
                    return 0;
                }
                if ( x > 1 - eps ) {
                    e.node( 1 )->pos = P1;
                    return 0;
                }
                return x;
            }
        }
        return 0;
    }

    const TIMG1 &ls_crack;
    const TIMG2 &ls_front;
    double eps;
};

/*!
    Objectif :
        La fonction \a level_set_image_refinement permet de raffiner un maillage à partir d'une image.

    Paramètres :
        * <strong> m </strong> est un maillage qui sera modifié si nécessaire.
        * <strong> ls_crack </strong>
        * <strong> ls_front </strong>

    Paramètres optionels :
        * <strong> eps </strong>

    Retour :
        Cette fonction renvoie vrai si elle divise au moins une barre et faux sinon.

    Exemple de code pour raffiner un maillage à partir d'une image :
    \code C/C++

        #include <mesh/mesh.h>
        #include <mesh/meshcaracstd.h>
        #include <mesh/refinement.h>
        #include <mesh/make_rect.h>
        #include <mesh/displayparaview.h>
        #include <correlation/ImgInterp.h>
        using namespace LMT;

        int main( int argc, char **argv ) {
            typedef Mesh<MeshCaracStd<2,2> > TM;
            typedef ImgInterp<double,2> TI;

            TM m;
            make_rect( m, Triangle(), 0, 1024, 40 );

            display_mesh( m );

            TI bin( "/home/leclerc/Data/Croix/masque_0.png" );
            TI cut = img_dist_from_front( bin, 100, 128.0 );
            TI stp; stp.resize( cut.sizes, -1 );

            cut.display( true );

            level_set_image_refinement( m, cut, stp );

            display_mesh( m );

            return 0;
        }

    \relates level_set_refinement
    \relates refinement
    \keyword Maillage/Opération
    \friend lecler@lmt.ens-cachan.fr
*/
template<class TM,class TIMG1,class TIMG2>
bool level_set_image_refinement( TM &m, const TIMG1 &ls_crack, const TIMG2 &ls_front, double eps = 1e-1, bool spread_cut = false ) {
    LevelSetImageRefinement<TIMG1,TIMG2> lr( ls_crack, ls_front, eps );
    return refinement( m, lr, spread_cut );
}

/*!
    L'opérateur \a RefinementBasedOnLength est conçu pour la fonction \a refinement_if_length_sup .
*/
template<class T>
struct RefinementBasedOnLength {
    RefinementBasedOnLength( T length_max ) : l_max( length_max ) {}

    template<class TE>
    bool operator()( TE &e ) const {
        return length( e.node(1)->pos - e.node(0)->pos ) > l_max;
    }

    T l_max;
};

/*!
    Objectif :
        La fonction \a refinement_if_length_sup permet de raffiner un maillage suivant un critère géométrique (longueur des barres). Elle divise toutes les barres (segments) du maillage en deux pour lesquelles la longueur est supérieure à l_max.

    Paramètres :
        * <strong> m </strong> est un maillage qui sera modifié si nécessaire.
        * <strong> l_max </strong> est un double qui correspond à la longueur maximale des barres qui ne seront pas divisées.

    Retour :
        Cette fonction renvoie vrai si elle divise au moins une barre et faux sinon. Ainsi si on souhaite que toutes les barres soient inférieures à l_max, on relancera la fonction autant de fois que nécessaire.

    Exemple de code pour raffiner un maillage jusqu'à obtenir une taille max donnée de 0.05 :
    \code C/C++

        #include "mesh/make_rect.h"
        #include "mesh/refinement.h"
        #include "mesh/displayparaview.h"

        // inclusion du code de notre MeshCarac
        #include "MonMeshCarac.h"

        int main( int argc, char **argv ) {
            typedef Mesh< Mesh_carac_MonMeshCarac<double,2> > TM;
            typedef TM::Pvec Pvec;
            typedef TM::TNode::T T;

            TM m;
            make_rect( m, Triangle(), Pvec( 0., 0. ), Pvec( 1., 1. ), Pvec( 20, 20 ) );

            display_mesh( m );

            while ( refinement_if_length_sup( m, 0.05 ) );

            display_mesh( m );

            return 0;
        }

    \relates refinement_if_length_sup
    \relates refinement_if_constraints
    \relates refinement_if_nodal_field_sup
    \relates refinement_if_constraints_or_nodal_field_sup
    \relates refinement_if_elem_field_sup
    \relates refinement_if_constraints_or_elem_field_sup
    \relates refinement
    \keyword Maillage/Opération
    \friend lecler@lmt.ens-cachan.fr
    \friend florent.pled@univ-paris-est.fr

    Subdivide each element bar e such as length(e)>l_max (true means subdivision).
*/
template<class TM,class T>
bool refinement_if_length_sup( TM &m, T length_max, bool spread_cut = false ) {
    RefinementBasedOnLength<T> rl( length_max );
    return refinement( m, rl, spread_cut );
}

/*!
    L'opérateur \a RefinementBasedOnConstraints est conçu pour la fonction \a refinement_if_constraints .
*/
template<class TF>
struct RefinementBasedOnConstraints {
    RefinementBasedOnConstraints( const TF &f ) : ptr_f( &f ) {}

    template<class TE>
    bool operator()( TE &e ) const {
        return ( ptr_f->constrained_nodes()[ e.node(0)->number ] ) and ( ptr_f->constrained_nodes()[ e.node(1)->number ] );
    }

    const TF* ptr_f;
};

/*!
    Objectif :
        La fonction \a refinement_if_constraints permet de raffiner un maillage sur les bords de Dirichlet. Elle divise toutes les barres (segments) du maillage en deux appartenant à un bord de Dirchlet (bord soumis à des contraintes cinématiques).

    Paramètres :
        * <strong> f </strong> est une formulation de la plateforme.

    Retour :
        Cette fonction renvoie vrai si elle divise au moins une barre et faux sinon.

    Exemple de code pour raffiner un maillage autour des noeuds contraints (appartenant a un bord de Dirichlet) :
    \code C/C++

        #include "mesh/make_rect.h"
        #include "mesh/refinement.h"
        #include "mesh/displayparaview.h"

        // inclusion du code de notre MeshCarac
        #include "MonMeshCarac.h"
        // inclusion du code de notre Formulation
        #include "MaFormulation.h"

        int main( int argc, char **argv ) {
            typedef Mesh< Mesh_carac_MonMeshCarac<double,2> > TM;
            typedef Formulation<TM,MaFormulation,DefaultBehavior,double,wont_add_nz> TF;
            typedef TM::Pvec Pvec;
            typedef TM::TNode::T T;

            TM m;
            T lx = 10., ly = 1.;
            make_rect( m, Triangle(), Pvec( 0., 0. ), Pvec( lx, ly ), Pvec( 21, 5 ) );

            TF f( m );

            for(unsigned i = 0; i < m.node_list.size(); ++i ) {
                if ( ( m.node_list[ i ].pos[ 0 ] < 1e-6 ) or ( m.node_list[ i ].pos[ 0 ] > lx - 1e-6 ) ) {
                    for( int d = 0; d < dim; ++d )
                        f.add_constraint( "sin( node[" + to_string( i ) + "].dep[" + to_string( d ) + "] ) - " + to_string( 0.1 * m.node_list[ i ].pos[ 0 ] * ( d == 0 ) ), 1e5 );
                }
            }

            f.solve();

            display_mesh( m );

            refinement_if_constraints( m, f );

            display_mesh( m );

            return 0;
        }

    \relates refinement_if_constraints
    \relates refinement_if_length_sup
    \relates refinement_if_nodal_field_sup
    \relates refinement_if_constraints_or_nodal_field_sup
    \relates refinement_if_elem_field_sup
    \relates refinement_if_constraints_or_elem_field_sup
    \relates refinement
    \keyword Maillage/Opération
    \friend lecler@lmt.ens-cachan.fr
    \friend florent.pled@univ-paris-est.fr
*/
template<class TM,class TF>
bool refinement_if_constraints( TM &m, const TF &f, bool spread_cut = false ) {
    RefinementBasedOnConstraints<TF> r( f );
    return refinement( m, r, spread_cut );
}

/*!
    L'opérateur \a RefinementBasedOnNodalField est conçu pour la fonction \a refinement_if_nodal_field_sup .
*/
template<class ExtractPhi,class TM,class T>
struct RefinementBasedOnNodalField {
    RefinementBasedOnNodalField( const ExtractPhi &_p, TM &m, T _k, T _pmax ) : p( _p ), ptr_m( &m ), k( _k  ), pmax( _pmax ) {}

    template<class TE>
    bool operator()( TE &e ) const {
        T p0 = p( *e.node(0) );
        T p1 = p( *e.node(1) );
        return ( p0 > k * pmax ) or ( p1 > k * pmax );
    }

    const ExtractPhi &p;
    TM* ptr_m;
    T k, pmax;
};

/*!
    Objectif :
        La fonction \a refinement_if_nodal_field_sup permet de raffiner un maillage suivant la valeur d'un attribut nodal d'un maillage. Plus précisément, considérons un attribut nodal scalaire ( i.e. double en général ), qu'on nommera phi. La fonction divise toutes les barres (segments) du maillage en deux pour lesquelles la valeur de phi à un des noeuds est supérieure à k fois la valeur maximale de phi sur tout le maillage.

    Paramètres :
        * <strong> m </strong> est un maillage qui sera modifié si nécessaire.
        * <strong> ExtractPhi </strong> est une classe qui permet d'accéder à la valeur d'un attribut du maillage m. Par exemple ce sera la classe \a ExtractDM < phi_DM > où <strong> phi </strong> est le nom de l'attribut. Remarque : il faut que le MeshCarac du maillage contienne une classe phi_DM.
        * <strong> k </strong> est un double qui correspond au rapport maximal (entre la valeur de l'attribut sur un des noeuds d'une barre et sa valeur maximale sur tout le maillage) des barres qui ne seront pas divisées.

    Retour :
        Cette fonction renvoie vrai si elle divise au moins une barre et faux sinon. Ainsi si on souhaite atteindre une précision donnée, on relancera la fonction après la mise à jour de l'attribut nodal autant de fois que nécessaire.

    Exemple de code pour raffiner un maillage autour des noeuds pour lesquels la valeur d'un attribut nodal est supérieure à 0.75 fois la valeur maximale sur tout le maillage :
    \code C/C++

        #include "mesh/make_rect.h"
        #include "mesh/refinement.h"
        #include "mesh/displayparaview.h"

        // inclusion du code de notre MeshCarac
        #include "MonMeshCarac.h"

        int main( int argc, char **argv ) {
            typedef Mesh< Mesh_carac_MonMeshCarac<double,2> > TM;
            typedef TM::Pvec Pvec;
            typedef TM::TNode::T T;

            TM m;
            make_rect( m, Triangle(), Pvec( 0., 0. ), Pvec( 1., 1. ), Pvec( 20, 20 ) );

            for( unsigned i = 0 ; i < m.node_list.size(); ++i )
                m.node_list[i].phi = sin( std::sqrt( i ) * 5. );

            display_mesh( m );

            refinement_if_nodal_field_sup( m, ExtractDM< phi_DM >(), 0.75 );

            display_mesh( m );

            return 0;
        }

    \relates refinement_if_nodal_field_sup
    \relates refinement_if_constraints_or_nodal_field_sup
    \relates refinement_if_elem_field_sup
    \relates refinement_if_constraints_or_elem_field_sup
    \relates refinement_if_length_sup
    \relates refinement_if_constraints
    \relates refinement
    \keyword Maillage/Opération
    \friend lecler@lmt.ens-cachan.fr
    \friend florent.pled@univ-paris-est.fr
*/
template<class TM,class ExtractPhi,class T>
bool refinement_if_nodal_field_sup( TM &m, const ExtractPhi &p, T k, bool spread_cut = false ) {
    T pmin, pmax;
    get_min_max( m.node_list, p, pmin, pmax );
    RefinementBasedOnNodalField<ExtractPhi,TM,T> r( p, m, k, pmax );
    return refinement( m, r, spread_cut );
}

/*!
    L'opérateur \a RefinementBasedOnConstraintsAndNodalField est conçu pour la fonction \a refinement_if_constraints_or_nodal_field_sup .
*/
template<class ExtractPhi,class TF,class TM,class T>
struct RefinementBasedOnConstraintsAndNodalField {
    RefinementBasedOnConstraintsAndNodalField( const ExtractPhi &_p, const TF &f, TM &m, T _k, T _pmax ) : p( _p ), ptr_f( &f ), ptr_m( &m ), k( _k  ), pmax( _pmax ) {}

    template<class TE>
    bool operator()( TE &e ) const {
        if ( ( ptr_f->constrained_nodes()[ e.node(0)->number ] ) and ( ptr_f->constrained_nodes()[ e.node(1)->number ] ) )
            return 1;
        T p0 = p( *e.node(0) );
        T p1 = p( *e.node(1) );
        return ( p0 > k * pmax ) or ( p1 > k * pmax );
    }

    const ExtractPhi &p;
    const TF* ptr_f;
    TM* ptr_m;
    T k, pmax;
};

/*!
    Objectif :
        La fonction \a refinement_if_constraints_or_nodal_field_sup permet de raffiner un maillage sur les bords de Dirichlet et suivant la valeur d'un attribut nodal sur le reste du maillage. Plus précisément, considérons un attribut nodal scalaire ( i.e. double en général ), qu'on nommera phi. La fonction divise toutes les barres (segments) du maillage en deux appartenant à un bord de Dirchlet (bord soumis à des contraintes cinématiques) ou pour lesquelles la valeur de phi à un des noeuds est supérieure à k fois la valeur maximale de phi sur tout le maillage.

    Paramètres :
        * <strong> m </strong> est un maillage qui sera modifié si nécessaire.
        * <strong> f </strong> est une formulation de la plateforme.
        * <strong> ExtractPhi </strong> est une classe qui permet d'accéder à la valeur d'un attribut du maillage m. Par exemple ce sera la classe \a ExtractDM < phi_DM > où <strong> phi </strong> est le nom de l'attribut. Remarque : il faut que le MeshCarac du maillage contienne une classe phi_DM.
        * <strong> k </strong> est un double qui correspond au rapport maximal (entre la valeur de l'attribut sur un des noeuds d'une barre et sa valeur maximale sur tout le maillage) des barres qui ne seront pas divisées.

    Retour :
        Cette fonction renvoie vrai si elle divise au moins une barre et faux sinon. Ainsi si on souhaite atteindre une précision donnée, on relancera la fonction après la mise à jour de l'attribut nodal autant de fois que nécessaire.

    Exemple de code pour raffiner un maillage autour des noeuds contraints (appartenant a un bord de Dirichlet) ou pour lesquels la valeur d'un attribut nodal est supérieure à 0.75 fois la valeur maximale sur tout le maillage :
    \code C/C++

        #include "mesh/make_rect.h"
        #include "mesh/refinement.h"
        #include "mesh/displayparaview.h"

        // inclusion du code de notre MeshCarac
        #include "MonMeshCarac.h"

        int main( int argc, char **argv ) {
            typedef Mesh< Mesh_carac_MonMeshCarac<double,2> > TM;
            typedef TM::Pvec Pvec;
            typedef TM::TNode::T T;

            TM m;
            T lx = 10., ly = 1.;
            make_rect( m, Triangle(), Pvec( 0., 0. ), Pvec( lx, ly ), Pvec( 21, 5 ) );

            for( unsigned i = 0 ; i < m.node_list.size(); ++i )
                m.node_list[i].phi = sin( std::sqrt( i ) * 5. );

            TF f( m );

            for(unsigned i = 0; i < m.node_list.size(); ++i ) {
                if ( ( m.node_list[ i ].pos[ 0 ] < 1e-6 ) or ( m.node_list[ i ].pos[ 0 ] > lx - 1e-6 ) ) {
                    for( int d = 0; d < dim; ++d )
                        f.add_constraint( "sin( node[" + to_string( i ) + "].dep[" + to_string( d ) + "] ) - " + to_string( 0.1 * m.node_list[ i ].pos[ 0 ] * ( d == 0 ) ), 1e5 );
                }
            }

            f.solve();

            display_mesh( m );

            refinement_if_constraints_or_nodal_field_sup( m, f, ExtractDM< phi_DM >(), 0.75 );

            display_mesh( m );

            return 0;
        }

    \relates refinement_if_constraints_or_nodal_field_sup
    \relates refinement_if_constraints_or_elem_field_sup
    \relates refinement_if_nodal_field_sup
    \relates refinement_if_elem_field_sup
    \relates refinement_if_length_sup
    \relates refinement_if_constraints
    \relates refinement
    \keyword Maillage/Opération
    \friend lecler@lmt.ens-cachan.fr
    \friend florent.pled@univ-paris-est.fr
*/
template<class TM,class TF,class ExtractPhi,class T>
bool refinement_if_constraints_or_nodal_field_sup( TM &m, const TF &f, const ExtractPhi &p, T k, bool spread_cut = false ) {
    T pmin, pmax;
    get_min_max( m.node_list, p, pmin, pmax );
    RefinementBasedOnConstraintsAndNodalField<ExtractPhi,TF,TM,T> r( p, f, m, k, pmax );
    return refinement( m, r, spread_cut );
}

/*!
    L'opérateur \a RefinementBasedOnElemField est conçu pour la fonction \a refinement_if_elem_field_sup .
*/
template<class DM,class TM,class T>
struct RefinementBasedOnElemField {
    RefinementBasedOnElemField( const DM &_dm, TM &m, T _k, T _dmax ) : dm( _dm ), ptr_m( &m ), k( _k  ), dmax( _dmax ) {}

    template<class TE>
    bool operator()( TE &e ) const {
        for(unsigned n=0;n<ptr_m->sub_mesh(Number<TM::nvi-1>()).get_parents_of( e ).size() ;++n)
            if ( ptr_m->sub_mesh(Number<TM::nvi-1>()).get_parents_of( e )[n]->get_field( dm.name(), StructForType<T>() ) > k * dmax )
                return 1;
        return 0;
    }

    const DM &dm;
    TM* ptr_m;
    T k, dmax;
};

/*!
    Objectif :
        La fonction \a refinement_if_elem_field_sup permet de raffiner un maillage suivant la valeur d'un attribut élémentaire d'un maillage. Plus précisément, considérons un attribut élémentaire scalaire ( i.e. double en général ), qu'on nommera phi. La fonction divise toutes les barres (segments) du maillage en deux pour lesquelles la valeur de phi sur un des éléments est supérieure à k fois la valeur maximale de phi sur tout le maillage.

    Paramètres :
        * <strong> m </strong> est un maillage qui sera modifié si nécessaire.
        * <strong> DM </strong> est une classe qui permet d'accéder au nom d'un attribut du maillage m. Par exemple ce sera la classe \a phi_DM où <strong> phi </strong> est le nom de l'attribut. Remarque : il faut que le MeshCarac du maillage contienne une classe phi_DM.
        * <strong> k </strong> est un double qui correspond au rapport maximal (entre la valeur de l'attribut sur un des éléments d'une barre et sa valeur maximale sur tout le maillage) des barres qui ne seront pas divisées.

    Retour :
        Cette fonction renvoie vrai si elle divise au moins une barre et faux sinon. Ainsi si on souhaite atteindre une précision donnée, on relancera la fonction après la mise à jour de l'attribut élémentaire autant de fois que nécessaire.

    Exemple de code pour raffiner un maillage autour des éléments pour lesquels la valeur d'un attribut élémentaire est supérieure à 0.75 fois la valeur maximale sur tout le maillage :
    \code C/C++

        #include "mesh/make_rect.h"
        #include "mesh/refinement.h"
        #include "mesh/displayparaview.h"

        // inclusion du code de notre MeshCarac
        #include "MonMeshCarac.h"

        int main( int argc, char **argv ) {
            typedef Mesh< Mesh_carac_MonMeshCarac<double,2> > TM;
            typedef TM::Pvec Pvec;
            typedef TM::TNode::T T;

            TM m;
            make_rect( m, Triangle(), Pvec( 0., 0. ), Pvec( 1., 1. ), Pvec( 20, 20 ) );

            for( unsigned n = 0 ; n < m.elem_list.size(); ++n )
                m.elem_list[n]->set_field( "phi", sin( std::sqrt( n ) * 5. );

            display_mesh( m );

            refinement_if_elem_field_sup( m, phi_DM, 0.75 );

            display_mesh( m );

            return 0;
        }

    \relates refinement_if_elem_field_sup
    \relates refinement_if_constraints_or_elem_field_sup
    \relates refinement_if_nodal_field_sup
    \relates refinement_if_constraints_or_nodal_field_sup
    \relates refinement_if_length_sup
    \relates refinement_if_constraints
    \relates refinement
    \keyword Maillage/Opération
    \friend lecler@lmt.ens-cachan.fr
    \friend florent.pled@univ-paris-est.fr
*/
template<class TM,class DM,class T>
bool refinement_if_elem_field_sup( TM &m, const DM &dm, T k, bool spread_cut = false ) {
//    m.update_skin();
    m.update_elem_parents( Number<TM::nvi-1>() );
    if ( TM::dim == 3 )
        m.update_elem_parents( Number<TM::nvi-2>() );
    T dmin, dmax;
    ExtractDM<DM> ed;
    get_min_max( m.elem_list, ed, dmin, dmax );
    RefinementBasedOnElemField<DM,TM,T> r( dm, m, k, dmax );
    return refinement( m, r, spread_cut );
}

/*!
    L'opérateur \a RefinementBasedConstraintsAndOnElemField est conçu pour la fonction \a refinement_if_constraints_or_elem_field_sup .
*/
template<class DM,class TF,class TM,class T>
struct RefinementBasedOnConstraintsAndElemField {
    RefinementBasedOnConstraintsAndElemField( const DM &_dm, const TF &f, TM &m, T _k, T _dmax ) : dm( _dm ), ptr_f( &f ), ptr_m( &m ), k( _k  ), dmax( _dmax ) {}

    template<class TE>
    bool operator()( TE &e ) const {
        if ( ( ptr_f->constrained_nodes()[ e.node(0)->number ] ) and ( ptr_f->constrained_nodes()[ e.node(1)->number ] ) )
            return 1;
        for(unsigned n=0;n<ptr_m->sub_mesh(Number<TM::nvi-1>()).get_parents_of( e ).size() ;++n)
            if ( ptr_m->sub_mesh(Number<TM::nvi-1>()).get_parents_of( e )[n]->get_field( dm.name(), StructForType<T>() ) > k * dmax )
                return 1;
        return 0;
    }

    const DM &dm;
    const TF* ptr_f;
    TM* ptr_m;
    T k, dmax;
};

/*!
    Objectif :
        La fonction \a refinement_if_constraints_or_elem_field_sup permet de raffiner un maillage sur les bords de Dirichlet et suivant la valeur d'un attribut élémentaire sur le reste du maillage. Plus précisément, considérons un attribut élémentaire scalaire ( i.e. double en général ), qu'on nommera phi. La fonction divise toutes les barres (segments) du maillage en deux appartenant à un bord de Dirchlet (bord soumis à des contraintes cinématiques) ou pour lesquelles la valeur de phi sur un des éléments est supérieure à k fois la valeur maximale de phi sur tout le maillage.

    Paramètres :
        * <strong> m </strong> est un maillage qui sera modifié si nécessaire.
        * <strong> f </strong> est une formulation de la plateforme.
        * <strong> DM </strong> est une classe qui permet d'accéder au nom d'un attribut du maillage m. Par exemple ce sera la classe \a phi_DM où <strong> phi </strong> est le nom de l'attribut. Remarque : il faut que le MeshCarac du maillage contienne une classe phi_DM.
        * <strong> k </strong> est un double qui correspond au rapport maximal (entre la valeur de l'attribut sur un des éléments d'une barre et sa valeur maximale sur tout le maillage) des barres qui ne seront pas divisées.

    Retour :
        Cette fonction renvoie vrai si elle divise au moins une barre et faux sinon. Ainsi si on souhaite atteindre une précision donnée, on relancera la fonction après la mise à jour de l'attribut élémentaire autant de fois que nécessaire.

    Exemple de code pour raffiner un maillage autour des noeuds contraints (appartenant a un bord de Dirichlet) et autour des éléments pour lesquels la valeur d'un attribut élémentaire est supérieure à 0.75 fois la valeur maximale sur tout le maillage :
    \code C/C++

        #include "mesh/make_rect.h"
        #include "mesh/refinement.h"
        #include "mesh/displayparaview.h"

        // inclusion du code de notre MeshCarac
        #include "MonMeshCarac.h"

        int main( int argc, char **argv ) {
            typedef Mesh< Mesh_carac_MonMeshCarac<double,2> > TM;
            typedef TM::Pvec Pvec;
            typedef TM::TNode::T T;

            TM m;
            T lx = 10., ly = 1.;
            make_rect( m, Triangle(), Pvec( 0., 0. ), Pvec( lx, ly ), Pvec( 21, 5 ) );

            for( unsigned n = 0 ; n < m.elem_list.size(); ++n )
                m.elem_list[n]->set_field( "phi", sin( std::sqrt( n ) * 5. );

            for(unsigned i = 0; i < m.node_list.size(); ++i ) {
                if ( ( m.node_list[ i ].pos[ 0 ] < 1e-6 ) or ( m.node_list[ i ].pos[ 0 ] > lx - 1e-6 ) ) {
                    for( int d = 0; d < dim; ++d )
                        f.add_constraint( "sin( node[" + to_string( i ) + "].dep[" + to_string( d ) + "] ) - " + to_string( 0.1 * m.node_list[ i ].pos[ 0 ] * ( d == 0 ) ), 1e5 );
                }
            }

            f.solve();

            display_mesh( m );

            refinement_if_constraints_or_elem_field_sup( m, f, phi_DM(), 0.75 );

            display_mesh( m );

            return 0;
        }

    \relates refinement_if_constraints_or_elem_field_sup
    \relates refinement_if_constraints_or_nodal_field_sup
    \relates refinement_if_elem_field_sup
    \relates refinement_if_nodal_field_sup
    \relates refinement_if_length_sup
    \relates refinement_if_constraints
    \relates refinement
    \keyword Maillage/Opération
    \friend lecler@lmt.ens-cachan.fr
    \friend florent.pled@univ-paris-est.fr
*/
template<class TM,class TF,class DM,class T>
bool refinement_if_constraints_or_elem_field_sup( TM &m, const TF &f, const DM &dm, T k, bool spread_cut = false ) {
//    m.update_skin();
    m.update_elem_parents( Number<TM::nvi-1>() );
    if ( TM::dim == 3 )
        m.update_elem_parents( Number<TM::nvi-2>() );
    T dmin, dmax;
    ExtractDM<DM> ed;
    get_min_max( m.elem_list, ed, dmin, dmax );
    RefinementBasedOnConstraintsAndElemField<DM,TF,TM,T> r( dm, f, m, k, dmax );
    return refinement( m, r, spread_cut );
}

/*!
    L'opérateur \a Smoothing est conçu pour la fonction \a smoothing .
*/
template<class ExtractPhiNodal,class TM>
struct Smoothing {
    Smoothing( const ExtractPhiNodal &_pn, TM &m ) : pn( _pn ), ptr_m( &m ) {}

    template<class TE,class ExtractPhiElem>
    void operator()( TE &e, const ExtractPhiElem &pe ) const {
        for(unsigned i=0;i<TE::nb_nodes;++i)
            pn( *e.node(i) ) += pe( e ) / ptr_m->get_node_parents( e.node(i)->number ).size();
    }

    const ExtractPhiNodal &pn;
    TM* ptr_m;
};

/*!
    Objectif :
        La fonction \a smoothing permet de lisser un attribut élémentaire aux noeuds d'un maillage et de compléter un attribut nodal.

    Paramètres :
        * <strong> m </strong> est un maillage qui ne sera pas modifié.
        * <strong> ExtractPhiNodal </strong> est une classe qui permet d'accéder à la valeur d'un attribut nodal du maillage m. Par exemple ce sera la classe \a ExtractDM < phi_nodal_DM > où <strong> phi_nodal </strong> est le nom de l'attribut nodal. Remarque : il faut que le MeshCarac du maillage contienne une classe phi_nodal_DM.
        * <strong> ExtractPhiElem </strong> est une classe qui permet d'accéder à la valeur d'un attribut élémentaire du maillage m. Par exemple ce sera la classe \a ExtractDM < phi_elem_DM > où <strong> phi_elem </strong> est le nom de l'attribut élémentaire. Remarque : il faut que le MeshCarac du maillage contienne une classe phi_elem_DM.

    Exemple de code pour lisser un attribut élémtaire aux noeuds d'un maillage et compléter un attribut nodal :
    \code C/C++

        #include "mesh/make_rect.h"
        #include "mesh/refinement.h"
        #include "mesh/displayparaview.h"

        // inclusion du code de notre MeshCarac
        #include "MonMeshCarac.h"

        int main( int argc, char **argv ) {
            typedef Mesh< Mesh_carac_MonMeshCarac<double,2> > TM;
            typedef TM::Pvec Pvec;
            typedef TM::TNode::T T;

            TM m;
            make_rect( m, Triangle(), Pvec( 0., 0. ), Pvec( 1., 1. ), Pvec( 20, 20 ) );

            for( unsigned n = 0 ; n < m.elem_list.size(); ++n )
                m.elem_list[n]->set_field( "phi_elem", sin( std::sqrt( n ) * 5. ) );

            smoothing( m, ExtractDM< phi_nodal_DM >(), ExtractDM< phi_elem_DM >() );

            display_mesh( m );

            return 0;
        }

    \relates smoothing
    \keyword Maillage/Opération
    \friend lecler@lmt.ens-cachan.fr
    \friend florent.pled@univ-paris-est.fr
*/
template<class TM,class ExtractPhiNodal,class ExtractPhiElem>
void smoothing( TM &m, const ExtractPhiNodal &pn, const ExtractPhiElem &pe ) {
    for( unsigned i = 0 ; i < m.node_list.size(); ++i )
        pn( m.node_list[i] ) = 0.;
    m.update_elem_neighbours();
    Smoothing<ExtractPhiNodal,TM> re( pn, m );
    apply( m.elem_list, re, pe );
}

/*!
    Le foncteur \a RefinementPoint est conçu pour la fonction \a refinement_point .
*/
template<class T,class Pvec>
struct RefinementPoint {
    RefinementPoint( T length_min, T _k, Pvec _c ) : l_min( length_min ), k( _k ), c( _c ) {}

    template<class TE> 
    bool operator()( TE &e ) const {
        return length( e.node( 1 )->pos - e.node( 0 )->pos ) > length( center( e ) - c ) * k + l_min;
    }

    T l_min, k;
    Pvec c; /// centre
};

/*!
    Objectif :
        La fonction \a refinement_point permet de raffiner localement un maillage autour d'un point.

    Attributs :
        * <strong> c </strong> le centre de la zone que l'on veut raffiner. c n'est pas forcément un point dans le maillage.
        * <strong> l_min </strong> la longueur minimale des côtés des éléments du maillage.
        * <strong> k </strong> le coefficient d'augmentation de la longueur maximale des côtés des éléments en fonction de la distance au point c.

    Description :
        On décide de couper le côté d'un élément ( i.e. une \a Bar ) si sa longueur est supérieure à d * k + l_min où d est la distance entre le milieu du côté et le centre c.

    Retour :
        Cette fonction renvoie vrai si elle divise au moins une barre et faux sinon.

    Exemple de code pour raffiner un maillage autour du point c de coordonnées ( 0.2, 0.5 ) avec une longueur minimale de 0.01 et une augmentation de 0.2 :
    \code C/C++

        #include "mesh/make_rect.h"
        #include "mesh/refinement.h"
        #include "mesh/displayparaview.h"

        // inclusion du code de notre MeshCarac
        #include "MonMeshCarac.h"

        int main( int argc, char **argv ) {
            typedef Mesh< Mesh_carac_MonMeshCarac<double,2> > TM;
            typedef TM::Pvec Pvec;
            typedef TM::TNode::T T;

            TM m;
            make_rect( m, Triangle(), Pvec( 0., 0. ), Pvec( 1., 1. ), Pvec( 20, 20 ) );

            display_mesh( m );

            refinement_point( m, 0.01, 0.2, Pvec( 0.2, 0.5 ) );

            display_mesh( m );

            return 0;
        }

    \relates refinement_point
    \relates refinement_circle
    \relates refinement
    \keyword Maillage/Opération
    \friend lecler@lmt.ens-cachan.fr
    \friend florent.pled@univ-paris-est.fr
*/
template<class TM,class T,class Pvec>
bool refinement_point( TM &m, T length_min, T k, Pvec c, bool spread_cut = false ) {
    RefinementPoint<T,Pvec> r( length_min, k, c );
    return refinement( m, r, spread_cut );
}

/*!
    Le foncteur \a RefinementCircle est conçu pour la fonction \a refinement_circle .
*/
template<class T,class Pvec>
struct RefinementCircle {
    RefinementCircle( T length_min, T _k, Pvec _c, T _R ) : l_min( length_min ), k( _k ), c( _c ), R( _R ) {}

    template<class TE>
    bool operator()( TE &e ) const {
        return length( e.node( 1 )->pos - e.node( 0 )->pos ) > fabs( R - length( center( e ) - c ) ) * k + l_min;
    }

    T l_min, k;
    T R; /// rayon du cercle
    Pvec c; /// centre du cercle
};

/*!
    Objectif :
        La fonction \a refinement_circle permet de raffiner localement un maillage autour d'un cercle.

    Attributs :
        * <strong> c </strong> le centre de la zone (cercle) autour duquel on veut raffiner. c n'est pas forcément un point dans le maillage.
        * <strong> R </strong> le rayon de la zone (cercle) autour duquel on veut raffiner.
        * <strong> l_min </strong> la longueur minimale des côtés des éléments du maillage.
        * <strong> k </strong> le coefficient d'augmentation de la longueur maximale des côtés des éléments en fonction de la distance au cercle.

    Description :
        On décide de couper le côté d'un élément ( i.e. une \a Bar ) si sa longueur est supérieure à d * k + l_min où d est la distance entre le milieu du côté et le cercle de centre c et de rayon R.

    Retour :
        Cette fonction renvoie vrai si elle divise au moins une barre et faux sinon.

    Exemple de code pour raffiner un maillage autour du cercle de centre c de coordonnées ( 0.3, 0.5 ) et de rayon 0.1 avec une longueur minimale de 0.01 et une augmentation de 0.2 :
    \code C/C++

        #include "mesh/make_rect.h"
        #include "mesh/refinement.h"
        #include "mesh/displayparaview.h"

        // inclusion du code de notre MeshCarac
        #include "MonMeshCarac.h"

        int main( int argc, char **argv ) {
            typedef Mesh< Mesh_carac_MonMeshCarac<double,2> > TM;
            typedef TM::Pvec Pvec;
            typedef TM::TNode::T T;

            TM m;
            make_rect( m, Triangle(), Pvec( 0., 0. ), Pvec( 1., 1. ), Pvec( 20, 20 ) );

            display_mesh( m );

            refinement_circle( m, 0.01, 0.2, Pvec( 0.3, 0.5 ), 0.1 );

            display_mesh( m );

            return 0;
        }

    \relates refinement_circle
    \relates refinement_point
    \relates refinement
    \keyword Maillage/Opération
    \friend lecler@lmt.ens-cachan.fr
    \friend florent.pled@univ-paris-est.fr
*/
template<class TM,class T,class Pvec>
bool refinement_circle( TM &m, T length_min, T k, Pvec c, T R, bool spread_cut = false ) {
    RefinementCircle<T,Pvec> rc( length_min, k, c, R );
    return refinement( m, rc, spread_cut );
}

/*!
    L'opérateur \a MaskCutter est conçu pour la fonction \a mask_cut .
*/
namespace LMTPRIVATE {
    template<class TM,class MA,class I>
    struct MaskCutter {
        typedef typename TM::Tpos T;
        MaskCutter( const MA &mask, I lim_inf, I lim_sup ) : mask( mask ), lim_inf( lim_inf ), lim_sup( lim_sup ) {}
        //
        struct Cut {
            Cut( MaskCutter &mc ) : mc( mc ) {}
            template<class TE> T operator()( TE &e ) const {
                if ( mc.node_is_blocked[ e.node( 0 )->number ] and mc.node_is_blocked[ e.node( 1 )->number ] )
                    return 0;
                double step = 1.0 / measure( e );
                for( double x=0; x <= 1; x += step ) {
                    typename TE::Pvec P0 = e.pos( 0 ) + ( e.pos( 1 ) - e.pos( 0 ) ) * ( x - step );
                    typename TE::Pvec P1 = e.pos( 0 ) + ( e.pos( 1 ) - e.pos( 0 ) ) * ( x        );
                    bool c_01 = mc.mask( P0 ) < mc.lim_inf and mc.mask( P1 ) >= mc.lim_sup;
                    bool c_10 = mc.mask( P1 ) < mc.lim_inf and mc.mask( P0 ) >= mc.lim_sup;
                    if ( c_01 or c_10 )
                        return x;
                }
                return 0;
            }
            MaskCutter &mc;
        };
        //
        struct Rem {
            Rem( MaskCutter &mc ) : mc( mc ) {}
            template<class TE> bool operator()( TE &e ) const { return mc.mask( center( e ) ) < mc.lim_inf; }
            MaskCutter &mc;
        };

        const MA &mask;
        I lim_inf;
        I lim_sup;
        Vec<bool> node_is_blocked;
    };
}

/*!
    Cette fonction découpe un maillage par un masque. Un élément est coupé quand un bord a mask < lim_inf et l'autre a mask >= lim_sup.
    \keyword Maillage/Opération
*/
template<class TM,class MA,class I>
bool mask_cut( TM &m, const MA &mask, I lim_inf, I lim_sup, I dist_disp, bool remove_inf_elem = false, bool spread_cut = false ) {
    typedef LMTPRIVATE::MaskCutter<TM,MA,I> MC;
    typedef typename TM::Pvec Pvec;
    typedef typename TM::Tpos T;
    //
    MC mc( mask, lim_inf, lim_sup );
    mc.node_is_blocked.resize( m.node_list.size(), false );
    // nodal displacement
    m.update_skin();
    for( int n = 0; n < m.node_list.size(); ++n ) {
        Pvec &P = m.node_list[ n ].pos, N = P;
        T o = dist_disp * ( 1 + 1e-6 ), d = o;
        if ( mask( P ) < lim_inf ) {
            for( Rectilinear_iterator<T,TM::dim> iter( Pvec( P - dist_disp ), Pvec( P + dist_disp + 1 ) ); iter; ++iter ) {
                T n = length( P - iter.pos );
                if ( d > n and mask( iter.pos ) >= lim_sup ) {
                    N = iter.pos;
                    d = n;
                }
            }
        } else if ( mask( P ) >= lim_sup ) {
            for( Rectilinear_iterator<T,TM::dim> iter( Pvec( P - dist_disp ), Pvec( P + dist_disp + 1 ) ); iter; ++iter ) {
                T n = length( P - iter.pos );
                if ( d > n and mask( iter.pos ) < lim_inf ) {
                    N = iter.pos;
                    d = n;
                }
            }
        }
        if ( d != o ) {
            mc.node_is_blocked[ m.node_list[ n ].number ] = true;
            P = N;
        }
    }
    // cut
    typename MC::Cut cut( mc );
    bool res = refinement( m, cut, spread_cut );
    // rem
    if ( remove_inf_elem ) {
        typename MC::Rem rem( mc );
        m.remove_elements_if( rem );
    }
    return res;
}

/*!
    L'opérateur \a DivideElement est conçu pour la fonction \a divide_element .
*/
template<class TM, class TV>
struct DivideElement {
    DivideElement( TM &m, const TV &elem_list ) : ptr_m( &m ), ptr_elem_list( &elem_list ) {}

    template< class NameBehavior, class TNode_, class TData, unsigned num_in_elem_list_ >
    bool operator() ( Element< Triangle, NameBehavior, TNode_, TData, num_in_elem_list_ > &e ) {
       if ( find( *ptr_elem_list, _1 == e.number ) ) {
            typedef typename Element< Triangle, NameBehavior, TNode_, TData, num_in_elem_list_ >::TNode TNode;
            Vec<TNode*,3> vn;
            ptr_m->update_elem_children();
            BestialNodeAdder<TM> ban; ban.m = ptr_m; ban.prec = 1e-6;
            Vec<TNode*,3> node_center_Bar;
            for (unsigned i=0;i<3;++i) {
                node_center_Bar[i] = ban.get_node( center( *(ptr_m->get_children_of(e,Number<1>())[i]) ) );
            }
            /// Triangle 0
            vn[ 0 ] = e.node( 0 );
            vn[ 1 ] = node_center_Bar[ 0 ];
            vn[ 2 ] = node_center_Bar[ 2 ];
            permutation_if_jac_neg ( Triangle(), vn.ptr() );
            DM::copy( e, *ptr_m->add_element( Triangle(), NameBehavior(), vn.ptr() ) );
            /// Triangle 1
            vn[ 0 ] = node_center_Bar[ 0 ];
            vn[ 1 ] = e.node( 1 );
            vn[ 2 ] = node_center_Bar[ 1 ];
            permutation_if_jac_neg ( Triangle(), vn.ptr() );
            DM::copy( e, *ptr_m->add_element( Triangle(), NameBehavior(), vn.ptr() ) );
            /// Triangle 2
            vn[ 0 ] = node_center_Bar[ 2 ];
            vn[ 1 ] = node_center_Bar[ 1 ];
            vn[ 2 ] = e.node( 2 );
            permutation_if_jac_neg ( Triangle(), vn.ptr() );
            DM::copy( e, *ptr_m->add_element( Triangle(), NameBehavior(), vn.ptr() ) );
            /// Triangle 3
            vn[ 0 ] = node_center_Bar[ 0 ];
            vn[ 1 ] = node_center_Bar[ 1 ];
            vn[ 2 ] = node_center_Bar[ 2 ];
            permutation_if_jac_neg ( Triangle(), vn.ptr() );
            DM::copy( e, *ptr_m->add_element( Triangle(), NameBehavior(), vn.ptr() ) );
            return true;
       }
       else
           return false;
    }

    template< class NameBehavior, class TNode_, class TData, unsigned num_in_elem_list_ >
    bool operator() ( Element< Quad, NameBehavior, TNode_, TData, num_in_elem_list_ > &e ) {
        if ( find( *ptr_elem_list, _1 == e.number ) ) {
            typedef typename Element< Quad, NameBehavior, TNode_, TData, num_in_elem_list_ >::TNode TNode;
            Vec<TNode*,4> vn;
            ptr_m->update_elem_children();
            BestialNodeAdder<TM> ban; ban.m = ptr_m; ban.prec = 1e-6;
            TNode* node_center_Quad = ban.get_node( center(e) );
            Vec<TNode*,4> node_center_Bar;
            for (unsigned i=0;i<4;++i) {
                node_center_Bar[i] = ban.get_node( center( *(ptr_m->get_children_of(e,Number<1>())[i]) ) );
            }
            /// Quad 0
            vn[ 0 ] = e.node( 0 );
            vn[ 1 ] = node_center_Bar[ 0 ];
            vn[ 2 ] = node_center_Quad;
            vn[ 3 ] = node_center_Bar[ 3 ];
            permutation_if_jac_neg ( Quad(), vn.ptr() );
            DM::copy( e, *ptr_m->add_element( Quad(), NameBehavior(), vn.ptr() ) );
            /// Quad 1
            vn[ 0 ] = node_center_Bar[ 0 ];
            vn[ 1 ] = e.node( 1 );
            vn[ 2 ] = node_center_Bar[ 1 ];
            vn[ 3 ] = node_center_Quad;
            permutation_if_jac_neg ( Quad(), vn.ptr() );
            DM::copy( e, *ptr_m->add_element( Quad(), NameBehavior(), vn.ptr() ) );
            /// Quad 2
            vn[ 0 ] = node_center_Quad;
            vn[ 1 ] = node_center_Bar[ 1 ];
            vn[ 2 ] = e.node( 2 );
            vn[ 3 ] = node_center_Bar[ 2 ];
            permutation_if_jac_neg ( Quad(), vn.ptr() );
            DM::copy( e, *ptr_m->add_element( Quad(), NameBehavior(), vn.ptr() ) );
            /// Quad 3
            vn[ 0 ] = node_center_Bar[ 3 ];
            vn[ 1 ] = node_center_Quad;
            vn[ 2 ] = node_center_Bar[ 2 ];
            vn[ 3 ] = e.node( 3 );
            permutation_if_jac_neg ( Quad(), vn.ptr() );
            DM::copy( e, *ptr_m->add_element( Quad(), NameBehavior(), vn.ptr() ) );
            return true;
       }
       else
           return false;
    }

    template< class NameBehavior, class TNode_, class TData, unsigned num_in_elem_list_ >
    bool operator() ( Element< Tetra, NameBehavior, TNode_, TData, num_in_elem_list_ > &e ) {
        if ( find( *ptr_elem_list, _1 == e.number ) ) {
            typedef typename Element< Tetra, NameBehavior, TNode_, TData, num_in_elem_list_ >::TNode TNode;
            Vec<TNode*,4> vn;
            ptr_m->update_elem_children();
            ptr_m->update_elem_children( Number<2>() );
            BestialNodeAdder<TM> ban; ban.m = ptr_m; ban.prec = 1e-6;
            Vec<TNode*,6> node_center_Bar;
            for (unsigned i=0;i<6;++i) {
                node_center_Bar[i] = ban.get_node( center( *(ptr_m->get_children_of(e,Number<2>())[i]) ) );
            }
            /// Tetra 0
            vn[ 0 ] = node_center_Bar[ 2 ];
            vn[ 1 ] = node_center_Bar[ 4 ];
            vn[ 2 ] = node_center_Bar[ 5 ];
            vn[ 3 ] = e.node( 3 );
            permutation_if_jac_neg ( Tetra(), vn.ptr() );
            DM::copy( e, *ptr_m->add_element( Tetra(), NameBehavior(), vn.ptr() ) );
            /// Tetra 1
            vn[ 0 ] = e.node( 0 );
            vn[ 1 ] = node_center_Bar[ 0 ];
            vn[ 2 ] = node_center_Bar[ 1 ];
            vn[ 3 ] = node_center_Bar[ 2 ];
            permutation_if_jac_neg ( Tetra(), vn.ptr() );
            DM::copy( e, *ptr_m->add_element( Tetra(), NameBehavior(), vn.ptr() ) );
            /// Tetra 2
            vn[ 0 ] = node_center_Bar[ 1 ];
            vn[ 1 ] = node_center_Bar[ 3 ];
            vn[ 2 ] = e.node( 2 );
            vn[ 3 ] = node_center_Bar[ 5 ];
            permutation_if_jac_neg ( Tetra(), vn.ptr() );
            DM::copy( e, *ptr_m->add_element( Tetra(), NameBehavior(), vn.ptr() ) );
            /// Tetra 3
            vn[ 0 ] = node_center_Bar[ 0 ];
            vn[ 1 ] = e.node( 1 );
            vn[ 2 ] = node_center_Bar[ 3 ];
            vn[ 3 ] = node_center_Bar[ 4 ];
            permutation_if_jac_neg ( Tetra(), vn.ptr() );
            DM::copy( e, *ptr_m->add_element( Tetra(), NameBehavior(), vn.ptr() ) );
            /// Tetra 4
            vn[ 0 ] = node_center_Bar[ 2 ];
            vn[ 1 ] = node_center_Bar[ 0 ];
            vn[ 2 ] = node_center_Bar[ 1 ];
            vn[ 3 ] = node_center_Bar[ 4 ];
            permutation_if_jac_neg ( Tetra(), vn.ptr() );
            DM::copy( e, *ptr_m->add_element( Tetra(), NameBehavior(), vn.ptr() ) );
            /// Tetra 5
            vn[ 0 ] = node_center_Bar[ 1 ];
            vn[ 1 ] = node_center_Bar[ 3 ];
            vn[ 2 ] = node_center_Bar[ 5 ];
            vn[ 3 ] = node_center_Bar[ 4 ];
            permutation_if_jac_neg ( Tetra(), vn.ptr() );
            DM::copy( e, *ptr_m->add_element( Tetra(), NameBehavior(), vn.ptr() ) );
            /// Tetra 6
            vn[ 0 ] = node_center_Bar[ 2 ];
            vn[ 1 ] = node_center_Bar[ 1 ];
            vn[ 2 ] = node_center_Bar[ 5 ];
            vn[ 3 ] = node_center_Bar[ 4 ];
            permutation_if_jac_neg ( Tetra(), vn.ptr() );
            DM::copy( e, *ptr_m->add_element( Tetra(), NameBehavior(), vn.ptr() ) );
            /// Tetra 7
            vn[ 0 ] = node_center_Bar[ 0 ];
            vn[ 1 ] = node_center_Bar[ 3 ];
            vn[ 2 ] = node_center_Bar[ 1 ];
            vn[ 3 ] = node_center_Bar[ 4 ];
            permutation_if_jac_neg ( Tetra(), vn.ptr() );
            DM::copy( e, *ptr_m->add_element( Tetra(), NameBehavior(), vn.ptr() ) );
            return true;
       }
       else
           return false;
    }

    template< class NameBehavior, class TNode_, class TData, unsigned num_in_elem_list_ >
    bool operator() ( Element< Hexa, NameBehavior, TNode_, TData, num_in_elem_list_ > &e ) {
        if ( find( *ptr_elem_list, _1 == e.number ) ) {
            typedef typename Element< Hexa, NameBehavior, TNode_, TData, num_in_elem_list_ >::TNode TNode;
            Vec<TNode*,8> vn;
            ptr_m->update_elem_children();
            ptr_m->update_elem_children( Number<2>() );
            BestialNodeAdder<TM> ban; ban.m = ptr_m; ban.prec = 1e-6;
            TNode* node_center_Hexa = ban.get_node( center(e) );
            Vec<TNode*,6> node_center_Quad;
            for (unsigned i=0;i<6;++i) {
                node_center_Quad[i] = ban.get_node( center( *(ptr_m->get_children_of(e,Number<1>())[i]) ) );
            }
            Vec<TNode*,12> node_center_Bar;
            for (unsigned i=0;i<12;++i) {
                node_center_Bar[i] = ban.get_node( center( *(ptr_m->get_children_of(e,Number<2>())[i]) ) );
            }
            /// Hexa 0
            vn[ 0 ] = e.node( 0 );
            vn[ 1 ] = node_center_Bar[ 0 ];
            vn[ 2 ] = node_center_Quad[ 0 ];
            vn[ 3 ] = node_center_Bar[ 3 ];
            vn[ 4 ] = node_center_Bar[ 8 ];
            vn[ 5 ] = node_center_Quad[ 2 ];
            vn[ 6 ] = node_center_Hexa;
            vn[ 7 ] = node_center_Quad[ 4 ];
            permutation_if_jac_neg ( Hexa(), vn.ptr() );
            DM::copy( e, *ptr_m->add_element( Hexa(), NameBehavior(), vn.ptr() ) );
            /// Hexa 1
            vn[ 0 ] = node_center_Bar[ 0 ];
            vn[ 1 ] = e.node( 1 );
            vn[ 2 ] = node_center_Bar[ 1 ];
            vn[ 3 ] = node_center_Quad[ 0 ];
            vn[ 4 ] = node_center_Quad[ 2 ];
            vn[ 5 ] = node_center_Bar[ 9 ];
            vn[ 6 ] = node_center_Quad[ 5 ];
            vn[ 7 ] = node_center_Hexa;
            permutation_if_jac_neg ( Hexa(), vn.ptr() );
            DM::copy( e, *ptr_m->add_element( Hexa(), NameBehavior(), vn.ptr() ) );
            /// Hexa 2
            vn[ 0 ] = node_center_Bar[ 3 ];
            vn[ 1 ] = node_center_Quad[ 0 ];
            vn[ 2 ] = node_center_Bar[ 2 ];
            vn[ 3 ] = e.node( 3 );
            vn[ 4 ] = node_center_Quad[ 4 ];
            vn[ 5 ] = node_center_Hexa;
            vn[ 6 ] = node_center_Quad[ 3 ];
            vn[ 7 ] = node_center_Bar[ 11 ];
            permutation_if_jac_neg ( Hexa(), vn.ptr() );
            DM::copy( e, *ptr_m->add_element( Hexa(), NameBehavior(), vn.ptr() ) );
            /// Hexa 3
            vn[ 0 ] = node_center_Quad[ 0 ];
            vn[ 1 ] = node_center_Bar[ 1 ];
            vn[ 2 ] = e.node( 2 );
            vn[ 3 ] = node_center_Bar[ 2 ];
            vn[ 4 ] = node_center_Hexa;
            vn[ 5 ] = node_center_Quad[ 5 ];
            vn[ 6 ] = node_center_Bar[ 10 ];
            vn[ 7 ] = node_center_Quad[ 3 ];
            permutation_if_jac_neg ( Hexa(), vn.ptr() );
            DM::copy( e, *ptr_m->add_element( Hexa(), NameBehavior(), vn.ptr() ) );
            /// Hexa 4
            vn[ 0 ] = node_center_Bar[ 8 ];
            vn[ 1 ] = node_center_Quad[ 2 ];
            vn[ 2 ] = node_center_Hexa;
            vn[ 3 ] = node_center_Quad[ 4 ];
            vn[ 4 ] = e.node( 4 );
            vn[ 5 ] = node_center_Bar[ 4 ];
            vn[ 6 ] = node_center_Quad[ 1 ];
            vn[ 7 ] = node_center_Bar[ 7 ];
            permutation_if_jac_neg ( Hexa(), vn.ptr() );
            DM::copy( e, *ptr_m->add_element( Hexa(), NameBehavior(), vn.ptr() ) );
            /// Hexa 5
            vn[ 0 ] = node_center_Quad[ 2 ];
            vn[ 1 ] = node_center_Bar[ 9 ];
            vn[ 2 ] = node_center_Quad[ 5 ];
            vn[ 3 ] = node_center_Hexa;
            vn[ 4 ] = node_center_Bar[ 4 ];
            vn[ 5 ] = e.node( 5 );
            vn[ 6 ] = node_center_Bar[ 5 ];
            vn[ 7 ] = node_center_Quad[ 1 ];
            permutation_if_jac_neg ( Hexa(), vn.ptr() );
            DM::copy( e, *ptr_m->add_element( Hexa(), NameBehavior(), vn.ptr() ) );
            /// Hexa 6
            vn[ 0 ] = node_center_Quad[ 4 ];
            vn[ 1 ] = node_center_Hexa;
            vn[ 2 ] = node_center_Quad[ 3 ];
            vn[ 3 ] = node_center_Bar[ 11 ];
            vn[ 4 ] = node_center_Bar[ 7 ];
            vn[ 5 ] = node_center_Quad[ 1 ];
            vn[ 6 ] = node_center_Bar[ 6 ];
            vn[ 7 ] = e.node( 7 );
            permutation_if_jac_neg ( Hexa(), vn.ptr() );
            DM::copy( e, *ptr_m->add_element( Hexa(), NameBehavior(), vn.ptr() ) );
            /// Hexa 7
            vn[ 0 ] = node_center_Hexa;
            vn[ 1 ] = node_center_Quad[ 5 ];
            vn[ 2 ] = node_center_Bar[ 10 ];
            vn[ 3 ] = node_center_Quad[ 3 ];
            vn[ 4 ] = node_center_Quad[ 1 ];
            vn[ 5 ] = node_center_Bar[ 5 ];
            vn[ 6 ] = e.node( 6 );
            vn[ 7 ] = node_center_Bar[ 6 ];
            permutation_if_jac_neg ( Hexa(), vn.ptr() );
            DM::copy( e, *ptr_m->add_element( Hexa(), NameBehavior(), vn.ptr() ) );
            return true;
       }
       else
           return false;
    }

    /// pour tous les autres éléments autres que \a Triangle, \a Quad, \a Tetra, \a Hexa, on renvoie faux
    template<class TE>
    bool operator() ( TE &e ) {
        return false;
    }

    TM* ptr_m;
    const TV* ptr_elem_list;
};

/*!
    Objectif :
        La fonction \a divide_element permet de diviser une liste d'éléments d'un maillage. Elle divise en 4 les \a Triangle et \a Quad , et en 8 les \a Tetra et \a Hexa .

    Paramètres :
        * <strong> m </strong> est un maillage qui sera modifié.
        * <strong> elem_list </strong> est une liste numéros d'éléments.

    Retour :
        Cette fonction renvoie vrai s'il y a eu des changements et faux sinon.

    Exemple de code pour diviser les éléments 5, 50, 80, 100, 200 d'un maillage :
    Remarque : il faut adapter le MeshCarac.
    \code C/C++

        #include "mesh/make_rect.h"
        #include "mesh/refinement.h"
        #include "mesh/displayparaview.h"

        // inclusion du code de notre MeshCarac
        #include "MonMeshCarac.h"

        int main( int argc, char **argv ) {
            typedef Mesh< Mesh_carac_MonMeshCarac<double,2> > TM;
            typedef TM::Pvec Pvec;
            typedef TM::TNode::T T;

            TM m;
            make_rect( m, Triangle(), Pvec( 0., 0. ), Pvec( 1., 1. ), Pvec( 20, 20 ) );

            display_mesh( m );

            divide_element( m, Vec<unsigned>( 5, 50, 80, 100, 200 ) );

            display_mesh( m );

            return 0;
        }

    \relates divide_element
    \relates divide
    \keyword Maillage/Opération
    \friend lecler@lmt.ens-cachan.fr
*/
template <class TM, class TV>
bool divide_element( TM &m, const TV &elem_list ) {
    DivideElement<TM,TV> de( m, elem_list );
    return m.remove_elements_if( de );
}

/*!
    L'opérateur \a Divide est conçu pour la fonction \a divide .
*/
template<class TM>
struct Divide {
    Divide( TM &m ) : ptr_m( &m ) {}

    template< class NameBehavior, class TNode_, class TData, unsigned num_in_elem_list_ >
    bool operator() ( Element< Triangle, NameBehavior, TNode_, TData, num_in_elem_list_ > &e ) {
        typedef typename Element< Triangle, NameBehavior, TNode_, TData, num_in_elem_list_ >::TNode TNode;
        Vec<TNode*,3> vn;
        ptr_m->update_elem_children();
        BestialNodeAdder<TM> ban; ban.m = ptr_m; ban.prec = 1e-6;
        Vec<TNode*,3> node_center_Bar;
        for (unsigned i=0;i<3;++i) {
            node_center_Bar[i] = ban.get_node( center( *(ptr_m->get_children_of(e,Number<1>())[i]) ) );
        }
        /// Triangle 0
        vn[ 0 ] = e.node( 0 );
        vn[ 1 ] = node_center_Bar[ 0 ];
        vn[ 2 ] = node_center_Bar[ 2 ];
        permutation_if_jac_neg ( Triangle(), vn.ptr() );
        DM::copy( e, *ptr_m->add_element( Triangle(), NameBehavior(), vn.ptr() ) );
        /// Triangle 1
        vn[ 0 ] = node_center_Bar[ 0 ];
        vn[ 1 ] = e.node( 1 );
        vn[ 2 ] = node_center_Bar[ 1 ];
        permutation_if_jac_neg ( Triangle(), vn.ptr() );
        DM::copy( e, *ptr_m->add_element( Triangle(), NameBehavior(), vn.ptr() ) );
        /// Triangle 2
        vn[ 0 ] = node_center_Bar[ 2 ];
        vn[ 1 ] = node_center_Bar[ 1 ];
        vn[ 2 ] = e.node( 2 );
        permutation_if_jac_neg ( Triangle(), vn.ptr() );
        DM::copy( e, *ptr_m->add_element( Triangle(), NameBehavior(), vn.ptr() ) );
        /// Triangle 3
        vn[ 0 ] = node_center_Bar[ 0 ];
        vn[ 1 ] = node_center_Bar[ 1 ];
        vn[ 2 ] = node_center_Bar[ 2 ];
        permutation_if_jac_neg ( Triangle(), vn.ptr() );
        DM::copy( e, *ptr_m->add_element( Triangle(), NameBehavior(), vn.ptr() ) );
        return true;
    }

    template< class NameBehavior, class TNode_, class TData, unsigned num_in_elem_list_ >
    bool operator() ( Element< Quad, NameBehavior, TNode_, TData, num_in_elem_list_ > &e ) {
        typedef typename Element< Quad, NameBehavior, TNode_, TData, num_in_elem_list_ >::TNode TNode;
        Vec<TNode*,4> vn;
        ptr_m->update_elem_children();
        BestialNodeAdder<TM> ban; ban.m = ptr_m; ban.prec = 1e-6;
        TNode* node_center_Quad = ban.get_node( center(e) );
        Vec<TNode*,4> node_center_Bar;
        for (unsigned i=0;i<4;++i) {
            node_center_Bar[i] = ban.get_node( center( *(ptr_m->get_children_of(e,Number<1>())[i]) ) );
        }
        /// Quad 0
        vn[ 0 ] = e.node( 0 );
        vn[ 1 ] = node_center_Bar[ 0 ];
        vn[ 2 ] = node_center_Quad;
        vn[ 3 ] = node_center_Bar[ 3 ];
        permutation_if_jac_neg ( Quad(), vn.ptr() );
        DM::copy( e, *ptr_m->add_element( Quad(), NameBehavior(), vn.ptr() ) );
        /// Quad 1
        vn[ 0 ] = node_center_Bar[ 0 ];
        vn[ 1 ] = e.node( 1 );
        vn[ 2 ] = node_center_Bar[ 1 ];
        vn[ 3 ] = node_center_Quad;
        permutation_if_jac_neg ( Quad(), vn.ptr() );
        DM::copy( e, *ptr_m->add_element( Quad(), NameBehavior(), vn.ptr() ) );
        /// Quad 2
        vn[ 0 ] = node_center_Quad;
        vn[ 1 ] = node_center_Bar[ 1 ];
        vn[ 2 ] = e.node( 2 );
        vn[ 3 ] = node_center_Bar[ 2 ];
        permutation_if_jac_neg ( Quad(), vn.ptr() );
        DM::copy( e, *ptr_m->add_element( Quad(), NameBehavior(), vn.ptr() ) );
        /// Quad 3
        vn[ 0 ] = node_center_Bar[ 3 ];
        vn[ 1 ] = node_center_Quad;
        vn[ 2 ] = node_center_Bar[ 2 ];
        vn[ 3 ] = e.node( 3 );
        permutation_if_jac_neg ( Quad(), vn.ptr() );
        DM::copy( e, *ptr_m->add_element( Quad(), NameBehavior(), vn.ptr() ) );
        return true;
    }

    template< class NameBehavior, class TNode_, class TData, unsigned num_in_elem_list_ >
    bool operator() ( Element< Tetra, NameBehavior, TNode_, TData, num_in_elem_list_ > &e ) {
        typedef typename Element< Tetra, NameBehavior, TNode_, TData, num_in_elem_list_ >::TNode TNode;
        Vec<TNode*,4> vn;
        ptr_m->update_elem_children();
        ptr_m->update_elem_children( Number<2>() );
        BestialNodeAdder<TM> ban; ban.m = ptr_m; ban.prec = 1e-6;
        Vec<TNode*,6> node_center_Bar;
        for (unsigned i=0;i<6;++i) {
            node_center_Bar[i] = ban.get_node( center( *(ptr_m->get_children_of(e,Number<2>())[i]) ) );
        }
        /// Tetra 0
        vn[ 0 ] = node_center_Bar[ 2 ];
        vn[ 1 ] = node_center_Bar[ 4 ];
        vn[ 2 ] = node_center_Bar[ 5 ];
        vn[ 3 ] = e.node( 3 );
        permutation_if_jac_neg ( Tetra(), vn.ptr() );
        DM::copy( e, *ptr_m->add_element( Tetra(), NameBehavior(), vn.ptr() ) );
        /// Tetra 1
        vn[ 0 ] = e.node( 0 );
        vn[ 1 ] = node_center_Bar[ 0 ];
        vn[ 2 ] = node_center_Bar[ 1 ];
        vn[ 3 ] = node_center_Bar[ 2 ];
        permutation_if_jac_neg ( Tetra(), vn.ptr() );
        DM::copy( e, *ptr_m->add_element( Tetra(), NameBehavior(), vn.ptr() ) );
        /// Tetra 2
        vn[ 0 ] = node_center_Bar[ 1 ];
        vn[ 1 ] = node_center_Bar[ 3 ];
        vn[ 2 ] = e.node( 2 );
        vn[ 3 ] = node_center_Bar[ 5 ];
        permutation_if_jac_neg ( Tetra(), vn.ptr() );
        DM::copy( e, *ptr_m->add_element( Tetra(), NameBehavior(), vn.ptr() ) );
        /// Tetra 3
        vn[ 0 ] = node_center_Bar[ 0 ];
        vn[ 1 ] = e.node( 1 );
        vn[ 2 ] = node_center_Bar[ 3 ];
        vn[ 3 ] = node_center_Bar[ 4 ];
        permutation_if_jac_neg ( Tetra(), vn.ptr() );
        DM::copy( e, *ptr_m->add_element( Tetra(), NameBehavior(), vn.ptr() ) );
        /// Tetra 4
        vn[ 0 ] = node_center_Bar[ 2 ];
        vn[ 1 ] = node_center_Bar[ 0 ];
        vn[ 2 ] = node_center_Bar[ 1 ];
        vn[ 3 ] = node_center_Bar[ 4 ];
        permutation_if_jac_neg ( Tetra(), vn.ptr() );
        DM::copy( e, *ptr_m->add_element( Tetra(), NameBehavior(), vn.ptr() ) );
        /// Tetra 5
        vn[ 0 ] = node_center_Bar[ 1 ];
        vn[ 1 ] = node_center_Bar[ 3 ];
        vn[ 2 ] = node_center_Bar[ 5 ];
        vn[ 3 ] = node_center_Bar[ 4 ];
        permutation_if_jac_neg ( Tetra(), vn.ptr() );
        DM::copy( e, *ptr_m->add_element( Tetra(), NameBehavior(), vn.ptr() ) );
        /// Tetra 6
        vn[ 0 ] = node_center_Bar[ 2 ];
        vn[ 1 ] = node_center_Bar[ 1 ];
        vn[ 2 ] = node_center_Bar[ 5 ];
        vn[ 3 ] = node_center_Bar[ 4 ];
        permutation_if_jac_neg ( Tetra(), vn.ptr() );
        DM::copy( e, *ptr_m->add_element( Tetra(), NameBehavior(), vn.ptr() ) );
        /// Tetra 7
        vn[ 0 ] = node_center_Bar[ 0 ];
        vn[ 1 ] = node_center_Bar[ 3 ];
        vn[ 2 ] = node_center_Bar[ 1 ];
        vn[ 3 ] = node_center_Bar[ 4 ];
        permutation_if_jac_neg ( Tetra(), vn.ptr() );
        DM::copy( e, *ptr_m->add_element( Tetra(), NameBehavior(), vn.ptr() ) );
        return true;
    }

    template< class NameBehavior, class TNode_, class TData, unsigned num_in_elem_list_ >
    bool operator() ( Element< Hexa, NameBehavior, TNode_, TData, num_in_elem_list_ > &e ) {
        typedef typename Element< Hexa, NameBehavior, TNode_, TData, num_in_elem_list_ >::TNode TNode;
        Vec<TNode*,8> vn;
        ptr_m->update_elem_children();
        ptr_m->update_elem_children( Number<2>() );
        BestialNodeAdder<TM> ban; ban.m = ptr_m; ban.prec = 1e-6;
        TNode* node_center_Hexa = ban.get_node( center(e) );
        Vec<TNode*,6> node_center_Quad;
        for (unsigned i=0;i<6;++i) {
            node_center_Quad[i] = ban.get_node( center( *(ptr_m->get_children_of(e,Number<1>())[i]) ) );
        }
        Vec<TNode*,12> node_center_Bar;
        for (unsigned i=0;i<12;++i) {
            node_center_Bar[i] = ban.get_node( center( *(ptr_m->get_children_of(e,Number<2>())[i]) ) );
        }
        /// Hexa 0
        vn[ 0 ] = e.node( 0 );
        vn[ 1 ] = node_center_Bar[ 0 ];
        vn[ 2 ] = node_center_Quad[ 0 ];
        vn[ 3 ] = node_center_Bar[ 3 ];
        vn[ 4 ] = node_center_Bar[ 8 ];
        vn[ 5 ] = node_center_Quad[ 2 ];
        vn[ 6 ] = node_center_Hexa;
        vn[ 7 ] = node_center_Quad[ 4 ];
        permutation_if_jac_neg ( Hexa(), vn.ptr() );
        DM::copy( e, *ptr_m->add_element( Hexa(), NameBehavior(), vn.ptr() ) );
        /// Hexa 1
        vn[ 0 ] = node_center_Bar[ 0 ];
        vn[ 1 ] = e.node( 1 );
        vn[ 2 ] = node_center_Bar[ 1 ];
        vn[ 3 ] = node_center_Quad[ 0 ];
        vn[ 4 ] = node_center_Quad[ 2 ];
        vn[ 5 ] = node_center_Bar[ 9 ];
        vn[ 6 ] = node_center_Quad[ 5 ];
        vn[ 7 ] = node_center_Hexa;
        permutation_if_jac_neg ( Hexa(), vn.ptr() );
        DM::copy( e, *ptr_m->add_element( Hexa(), NameBehavior(), vn.ptr() ) );
        /// Hexa 2
        vn[ 0 ] = node_center_Bar[ 3 ];
        vn[ 1 ] = node_center_Quad[ 0 ];
        vn[ 2 ] = node_center_Bar[ 2 ];
        vn[ 3 ] = e.node( 3 );
        vn[ 4 ] = node_center_Quad[ 4 ];
        vn[ 5 ] = node_center_Hexa;
        vn[ 6 ] = node_center_Quad[ 3 ];
        vn[ 7 ] = node_center_Bar[ 11 ];
        permutation_if_jac_neg ( Hexa(), vn.ptr() );
        DM::copy( e, *ptr_m->add_element( Hexa(), NameBehavior(), vn.ptr() ) );
        /// Hexa 3
        vn[ 0 ] = node_center_Quad[ 0 ];
        vn[ 1 ] = node_center_Bar[ 1 ];
        vn[ 2 ] = e.node( 2 );
        vn[ 3 ] = node_center_Bar[ 2 ];
        vn[ 4 ] = node_center_Hexa;
        vn[ 5 ] = node_center_Quad[ 5 ];
        vn[ 6 ] = node_center_Bar[ 10 ];
        vn[ 7 ] = node_center_Quad[ 3 ];
        permutation_if_jac_neg ( Hexa(), vn.ptr() );
        DM::copy( e, *ptr_m->add_element( Hexa(), NameBehavior(), vn.ptr() ) );
        /// Hexa 4
        vn[ 0 ] = node_center_Bar[ 8 ];
        vn[ 1 ] = node_center_Quad[ 2 ];
        vn[ 2 ] = node_center_Hexa;
        vn[ 3 ] = node_center_Quad[ 4 ];
        vn[ 4 ] = e.node( 4 );
        vn[ 5 ] = node_center_Bar[ 4 ];
        vn[ 6 ] = node_center_Quad[ 1 ];
        vn[ 7 ] = node_center_Bar[ 7 ];
        permutation_if_jac_neg ( Hexa(), vn.ptr() );
        DM::copy( e, *ptr_m->add_element( Hexa(), NameBehavior(), vn.ptr() ) );
        /// Hexa 5
        vn[ 0 ] = node_center_Quad[ 2 ];
        vn[ 1 ] = node_center_Bar[ 9 ];
        vn[ 2 ] = node_center_Quad[ 5 ];
        vn[ 3 ] = node_center_Hexa;
        vn[ 4 ] = node_center_Bar[ 4 ];
        vn[ 5 ] = e.node( 5 );
        vn[ 6 ] = node_center_Bar[ 5 ];
        vn[ 7 ] = node_center_Quad[ 1 ];
        permutation_if_jac_neg ( Hexa(), vn.ptr() );
        DM::copy( e, *ptr_m->add_element( Hexa(), NameBehavior(), vn.ptr() ) );
        /// Hexa 6
        vn[ 0 ] = node_center_Quad[ 4 ];
        vn[ 1 ] = node_center_Hexa;
        vn[ 2 ] = node_center_Quad[ 3 ];
        vn[ 3 ] = node_center_Bar[ 11 ];
        vn[ 4 ] = node_center_Bar[ 7 ];
        vn[ 5 ] = node_center_Quad[ 1 ];
        vn[ 6 ] = node_center_Bar[ 6 ];
        vn[ 7 ] = e.node( 7 );
        permutation_if_jac_neg ( Hexa(), vn.ptr() );
        DM::copy( e, *ptr_m->add_element( Hexa(), NameBehavior(), vn.ptr() ) );
        /// Hexa 7
        vn[ 0 ] = node_center_Hexa;
        vn[ 1 ] = node_center_Quad[ 5 ];
        vn[ 2 ] = node_center_Bar[ 10 ];
        vn[ 3 ] = node_center_Quad[ 3 ];
        vn[ 4 ] = node_center_Quad[ 1 ];
        vn[ 5 ] = node_center_Bar[ 5 ];
        vn[ 6 ] = e.node( 6 );
        vn[ 7 ] = node_center_Bar[ 6 ];
        permutation_if_jac_neg ( Hexa(), vn.ptr() );
        DM::copy( e, *ptr_m->add_element( Hexa(), NameBehavior(), vn.ptr() ) );
        return true;
    }

    /// pour tous les autres éléments autres que \a Triangle, \a Quad, \a Tetra, \a Hexa, on renvoie faux
    template<class TE>
    bool operator() ( TE &e ) {
        return false;
    }

    TM* ptr_m;
};

/*!
    Objectif :
        La fonction \a divide permet de diviser tous les éléments d'un maillage.  Elle divise en 4 les \a Triangle et \a Quad , et en 8 les \a Tetra et \a Hexa .

    Paramètres :
        * <strong> m </strong> est un maillage qui sera modifié.

    Retour :
        Cette fonction renvoie vrai s'il y a eu des changements et faux sinon.

    Exemple de code pour diviser tous les éléments d'un maillage :
    Remarque : il faut adapter le MeshCarac.
    \code C/C++

        #include "mesh/make_rect.h"
        #include "mesh/refinement.h"
        #include "mesh/displayparaview.h"

        // inclusion du code de notre MeshCarac
        #include "MonMeshCarac.h"

        int main( int argc, char **argv ) {
            typedef Mesh< Mesh_carac_MonMeshCarac<double,2> > TM;
            typedef TM::Pvec Pvec;
            typedef TM::TNode::T T;

            TM m;
            make_rect( m, Triangle(), Pvec( 0., 0. ), Pvec( 1., 1. ), Pvec( 20, 20 ) );

            display_mesh( m );

            divide( m );

            display_mesh( m );

            return 0;
        }

    \relates divide
    \relates divide_element
    \keyword Maillage/Opération
    \friend lecler@lmt.ens-cachan.fr
*/
template <class TM>
bool divide( TM &m ) {
    Divide<TM> d( m );
    return m.remove_elements_if( d );
}

}

#endif // LMT_refinement_HEADER
