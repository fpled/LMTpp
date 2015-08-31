//
// C++ Interface: refinement
//
// Description:
//
//
// Author: Hugo LECLERC <leclerc@lmt.ens-cachan.fr>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMT_refinement_HEADER
#define LMT_refinement_HEADER

#include "mesh.h"
#include "bar.h"
#include "triangle.h"
#include "quad.h"
#include "tetra.h"
#include "../util/rectilinear_iterator.h"

#include <list>

namespace LMT {

namespace LMTPRIVATE {
    /*!
        Objectif : 
            Cette fonction sert à ajouter un noeud à une barre pour la découpe d'un maillage.
            
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
        /// pour la 3D mais le foncteur qui utilisera cette méthode parcourera des \a Triangle.
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
        /// pour la 3D mais le foncteur qui utilisera cette méthode parcourera des \a Triangle.
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
            
            /// détermination des arêtes coupées et calcule de la longueur des arêtes
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
                    /// on coupe toutes les arêtes pas déjà coupées et qui ont une longueur supérieure ou égal à la plus longue arête coupée
                    if ( ( nn[ i ] == NULL ) and ( ll[ i ] >= max_l_cut ) ) {
                        child = m_parent->get_children_of_EA( ea, Number<1>() )[ i ];
                        
                        append_node_cut_at_bar( m_parent->template sub_mesh<1>(), *m_parent, next.cut, child, .5, .5 );
                    }
                    
                    if ( child ) { /// extension aux voisins
                        /// Détermination du voisin qui a aussi child comme enfant
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
                    /// on coupe toutes les arêtes pas déjà coupées et qui ont une longueur supérieure ou égal à la plus longue arête coupée
                    if ( ( nn[ i ] == NULL ) and ( ll[ i ] >= max_l_cut ) ) {
                        child = m_parent->get_children_of_EA( ea, Number<2>() )[ i ];
                        
                        append_node_cut_at_bar( m_parent->template sub_mesh<2>(), *m_parent, next.next.cut, child, .5, .5 );
                    }
                    
                    if ( child ) { /// extension aux voisins
                        /// Détermination du voisin qui a aussi child comme enfant
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
        template<class T> void update_cut(const TM &m,T max_length) const {}
    };
};

/*!
    Cette fonction divise les barres (segments) d'un maillage 1, 2 ou 3D suivant l'opérateur op.

    Ça veut dire, que ça divise les éléments Tetra, Hexa, etc... qui contiennent ces barres.

    L'opérateur op peut soit renvoyer un booléen soit un double. Il prend aussi forcément un élément de type barre en paramètre.
    
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
    Enfin refinement() renvoie vrai si elle divise au moins une barre et faux sinon.

    \keyword Maillage/Elément/Opération

    subdivide each element bar e contained in mesh or sub_meshes such as op(e)==true (true means subdivision). Subdivide parents


    Voici un exemple pour diviser un maillage jusqu'à obtenir une taille max donnée

    \code C/C++
        read_msh_2( ... );
        while ( refinement_if_length_sup( m, max_length ) );



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
            case 3 : apply( m.sub_mesh( Number<1>() ).elem_list, r, Number<TM::dim>() ); break;/// application sur les triangles
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

    m.signal_connectivity_change();
    return res;
}

/*!
    Peut être utilisé pour couper un maillage avec un level-set
*/
template<class PhiExtract>
struct LevelSetRefinement {
    LevelSetRefinement( const PhiExtract &p ) : ed( p ) {}
    template<class TE> typename TE::T operator()( TE &e ) const {
        typename TE::T d0 = ed( *e.node(0) );
        typename TE::T d1 = ed( *e.node(1) );
        if ( ( d0 * d1 ) >= 0 )
            return 0;
        typename TE::T o = d0 / ( d0 - d1 );
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
    const PhiExtract &ed;
};

template<class PhiExtract>
struct LevelSetRemoveNeg {
    LevelSetRemoveNeg( const PhiExtract &p ) : ed( p ) {}
    template<class TE> typename TE::T operator()( const TE &e ) const {
        typename TE::T d = 0;
        for(unsigned i=0;i<TE::nb_nodes;++i)
            d += ed( *e.node(i) );
        d /= TE::nb_nodes;
        return d <= 0;
    }
    const PhiExtract &ed;
};

/*!
    Objectif :
        Cette fonction permet de couper un maillage en fonction de la valeur d'un attribut nodal et scalaire. Plus précisément considérons un attribut nodal scalaire ( i.e. double en général ), qu'on nommera phi. Après l'appel de la fonction, le maillage n'aura que des éléments pour lesquelles la valeur de phi aux noeuds est positive.
        
    Paramètres :
        * <strong> m </strong> est un maillage qui sera modifié si nécessaire.
        * <strong> PhiExtract </strong> est une classe qui permet d'accéder à la valeur d'un attribut du maillage m. Par exemple ce sera la classe \a ExtractDM < phi_DM > où <strong> phi </strong> est le nom de l'attribut. Remarque : il faut que le MeshCarac du maillage contienne une classe phi_DM.
           
    Retour :
        Cette fonction renvoie vrai s'il y a eu des changements et faux sinon.
        
    Voici un exemple de code. Il faudra adapter le MeshCarac ( cf le MeshCarac venant juste après le code C++ ).
    \code C/C++
    
        #include "mesh/make_rect.h"
        #include "mesh/refinement.h"
        #include "mesh/displayparaview.h"
        
        // inclusion du code de notre MeshCarac  
        #include "MonMeshCarac.h"
    
        int main(int argc,char **argv) {
        
            typedef Mesh< Mesh_carac_MonMeshCarac< double,2> > TM;
            typedef TM::Pvec Pvec;
            typedef TM::TNode::T T;
            
            TM m;
            make_rect( m, Triangle(), Pvec( 0, 0 ), Pvec( 1., 1. ), Pvec( 20, 20 ) );
        
            for( unsigned i = 0 ; i < m.node_list.size(); ++i )
                m.node_list[i].phi = sin( std::sqrt( i ) * 5. ); 
        
            display_mesh( m );
        
            PRINT( level_set_cut( m, ExtractDM< phi_DM >() ) );

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
                
    \relates level_set_cut
    \relates refinement
    \keyword Maillage/Opération
    \friend lecler@lmt.ens-cachan.fr
*/
template<class TM,class PhiExtract>
bool level_set_cut( TM &m, const PhiExtract &p, bool spread_cut = false ) {
    LevelSetRefinement<PhiExtract> lr( p );
    refinement( m, lr, spread_cut );
    LevelSetRemoveNeg<PhiExtract> ln( p );
    return m.remove_elements_if( ln );
}

/*!
    opérateur créé pour la fonction \a refinement_if_length_sup .
*/
template<class T>
struct RefinementOpBasedOnLength {
    template<class TE> bool operator()(const TE &e) const { return length(e.node(1)->pos-e.node(0)->pos) > max_length; }
    T max_length;
};

/*!
    Objectif :
        Cette fonction divise toutes les barres (segments) du maillage en deux pour lesquelles la longueur est supérieure à max_length.
    Retour :
        Cette fonction renvoie vrai si elle divise au moins une barre et faux sinon. Ainsi si on souhaite que toutes les barres soient inférieures à max_length, on relancera la fonction autant de fois que nécessaire.

    \keyword Maillage/Opération
    subdivide each element bar e such as length(e)>max_length (true means subdivision).
*/
template<class TM,class T>
bool refinement_if_length_sup( TM &m, T max_length, bool spread_cut = false ) {
    RefinementOpBasedOnLength<T> rl;
    rl.max_length = max_length;
    return refinement( m, rl, spread_cut );
}

/*!
  Exemple :

  \code
    #include <mesh/mesh.h>
    #include <mesh/meshcaracstd.h>
    #include <mesh/refinement.h>
    #include <mesh/make_rect.h>
    #include <mesh/displayparaview.h>
    #include <correlation/ImgInterp.h>
    using namespace LMT;


    int main() {
        typedef Mesh<MeshCaracStd<2,2> > TM;
        typedef ImgInterp<double,2> TI;

        TM m;
        make_rect( m, Triangle(), 0, 1024, 40 );

        TI bin( "/home/leclerc/Data/Croix/masque_0.png" );
        TI cut = img_dist_from_front( bin, 100, 128.0 );
        TI stp; stp.resize( cut.sizes, -1 );

        cut.display( true );

        LevelSetImageRefinement<TI> lr( cut, stp );
        refinement( m, lr );

        display( m );
    }

    \keyword Maillage/Opération
*/
template<class TIMG1,class TIMG2=TIMG1>
struct LevelSetImageRefinement {
    LevelSetImageRefinement( const TIMG1 &ls_crack, const TIMG2 &ls_front, double eps = 1e-1 ) : ls_crack( ls_crack ), ls_front( ls_front ), eps( eps ) {}
    template<class TE> double operator()( TE &e ) const {
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
        Ce foncteur est conçu pour la fonction \a refinement(). Il permet de raffiner localement un maillage autour d'un point. Pour plus de renseignement, voir l'explication à la fin.
        
    Attributs :
        * <strong> c </strong> le centre de la zone que l'on veut raffiner. c n'est pas forcément un point dans le maillage.
        * <strong> l_min </strong> la longueur minimale des côtés des éléments du maillage.
        * <strong> k </strong> le coefficient d'augmentation de la longueur maximale des côtés des éléments en fonction de la distance au point c.
        
    Description :
        On décide de couper le côté d'un élément ( i.e. une \a Bar ) si sa longueur est supérieure à d * k + l_min où d est la distance entre le milieu du côté et le centre c.
         
    Exemple de code :
    \code C/C++
        typedef Mesh< Mesh_carac_MonMeshCarac< double,2> > TM;
        typedef TM::Pvec Pvec;
        typedef TM::TNode::T T;
    
        refinement( m, Local_refinement<T, Pvec >( 0.01, 0.2, Pvec( 0.2, 0.5 ) ) );
    
    On raffinera au point de coordonnées ( 0.2, 0.5 ) avec une longueur minimale de 0.01 et une augmentation de 0.2. 
    
    
    \keyword Maillage/Opération
*/
template < class T, class Pvec>
struct Local_refinement {
    Local_refinement( T length_min, T _k, Pvec _c ) : l_min( length_min ), k( _k ), c( _c ) {}

    template<class TE> 
    bool operator()( TE &e ) const {
        T l = length( e.node( 1 )->pos - e.node( 0 )->pos );
        T v = length( center( e ) - c ) * k + l_min;
        if ( l > v ) 
            return true;
        else
            return false;
    }

    T l_min, k;
    Pvec c; /// centre
};

/*!
    Objectif :
        Ce foncteur est conçu pour la fonction \a refinement(). Il permet de raffiner localement un maillage autour d'un cercle. Pour plus de renseignement, voir l'explication à la fin.
        
    Attributs :
        * <strong> c </strong> le centre du cercle autour duquel on veut raffiner. c n'est pas forcément un point dans le maillage.
        * <strong> R </strong> le rayon du cercle autour duquel on veut raffiner.
        * <strong> l_min </strong> la longueur minimale des côtés des éléments du maillage.
        * <strong> k </strong> le coefficient d'augmentation de la longueur maximale des côtés des éléments en fonction de la distance au point c.
        
    Description :
        On décide de couper le côté d'un élément ( i.e. une \a Bar ) si sa longueur est supérieure à d * k + l_min où d est la distance entre le milieu du côté et le cercle de centre c et de rayon R.
         
    Exemple de code :
    \code C/C++
        typedef Mesh< Mesh_carac_MonMeshCarac< double,2> > TM;
        typedef TM::Pvec Pvec;
        typedef TM::TNode::T T;
    
        refinement( m, Local_refinement<T, Pvec >( 0.01, 0.2, Pvec( 0.2, 0.5 ), 0.2 ) );
    
    On raffinera autour du cercle de centre ( 0.2, 0.5 ) et de rayon 0.2 avec une longueur minimale de 0.01 et une augmentation de 0.2. 
    \keyword Maillage/Opération
*/
template < class T, class Pvec>
struct Local_refinement_circle {
    Local_refinement_circle( T length_min, T _k, Pvec _c, T _R ) : l_min( length_min ), k( _k ), c( _c ), R( _R ) {}

    template<class TE> 
    bool operator()( TE &e ) const {
        T l = length( e.node( 1 )->pos - e.node( 0 )->pos );
        T v = fabs( R - length( center( e ) - c ) ) * k + l_min;
        if ( l > v ) 
            return true;
        else
            return false;
    }

    T l_min, k;
    T R; /// rayon du cercle
    Pvec c; /// centre du cercle
};

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



}

#endif // LMT_refinement_HEADER
