#ifndef REPLACE_HEXA_BY_TETRA_H
#define REPLACE_HEXA_BY_TETRA_H

#include "mesh.h"
#include "Tetra.h"
#include "Hexa.h"

namespace LMT {

/*!
    L'opérateur \a Replace_Hexa_by_Tetra est conçu pour la fonction \a replace_Hexa_by_Tetra .
*/
template<class TM>
struct Replace_Hexa_by_Tetra {
    Replace_Hexa_by_Tetra( TM &m ) : ptr_m( &m ) {} 
    
    template< class NameBehavior, class TNode_, class TData, unsigned num_in_elem_list_ >
    bool operator() ( Element< Hexa, NameBehavior, TNode_, TData, num_in_elem_list_ > &e ) {
        typedef typename Element< Hexa, NameBehavior, TNode_, TData, num_in_elem_list_ >::TNode TNode;
        Vec<TNode*,4> vn;
        /// Tetra 0
        vn[ 0 ] = e.node( 0 );
        vn[ 1 ] = e.node( 1 );
        vn[ 2 ] = e.node( 3 );
        vn[ 3 ] = e.node( 4 );
        permutation_if_jac_neg ( Tetra(), vn.ptr() );
        ptr_m->add_element( Tetra(), DefaultBehavior(), vn.ptr() );
        /// Tetra 1
        vn[ 0 ] = e.node( 1 );
        vn[ 1 ] = e.node( 2 );
        vn[ 2 ] = e.node( 3 );
        vn[ 3 ] = e.node( 5 );
        permutation_if_jac_neg ( Tetra(), vn.ptr() );
        ptr_m->add_element( Tetra(), DefaultBehavior(), vn.ptr() );
        /// Tetra 2
        vn[ 0 ] = e.node( 1 );
        vn[ 1 ] = e.node( 3 );
        vn[ 2 ] = e.node( 4 );
        vn[ 3 ] = e.node( 5 );
        permutation_if_jac_neg ( Tetra(), vn.ptr() );
        ptr_m->add_element( Tetra(), DefaultBehavior(), vn.ptr() );
        /// Tetra 3
        vn[ 0 ] = e.node( 5 );
        vn[ 1 ] = e.node( 3 );
        vn[ 2 ] = e.node( 7 );
        vn[ 3 ] = e.node( 4 );
        permutation_if_jac_neg ( Tetra(), vn.ptr() );
        ptr_m->add_element( Tetra(), DefaultBehavior(), vn.ptr() );
        /// Tetra 4
        vn[ 0 ] = e.node( 5 );
        vn[ 1 ] = e.node( 2 );
        vn[ 2 ] = e.node( 3 );
        vn[ 3 ] = e.node( 7 );
        permutation_if_jac_neg ( Tetra(), vn.ptr() );
        ptr_m->add_element( Tetra(), DefaultBehavior(), vn.ptr() );
        /// Tetra 5
        vn[ 0 ] = e.node( 5 );
        vn[ 1 ] = e.node( 2 );
        vn[ 2 ] = e.node( 6 );
        vn[ 3 ] = e.node( 7 );
        permutation_if_jac_neg ( Tetra(), vn.ptr() );
        ptr_m->add_element( Tetra(), DefaultBehavior(), vn.ptr() );
        return true;    
    }
    
    /// pour tous les autres éléments autres que \a Hexa , on renvoie faux
    template<class TE> 
    bool operator() ( TE &e ) { 
        return false;
    }

    TM* ptr_m;
};

/*!
    Objectif :
        La fonction \a replace_Hexa_by_Tetra enlève tous les \a Hexa du maillage m et les remplace par des \a Tetra . Le maillage m est donc __modifié__ .

    Paramètre :
        * m : est un maillage de la plateforme qui sera modifié.

    Exemple de code pour remplacer les \a Hexa par des \a Tetra dans un maillage :
    \code C/C++

        #include "mesh/make_rect.h"
        #include "mesh/displayparaview.h"

        // inclusion du code de notre MeshCarac
        #include "MonMeshCarac.h"

        int main( int argc, char **argv ) {
            typedef Mesh< Mesh_carac_MonMeshCarac<double,3> > TM;

            TM m;
            make_rect( m, Hexa(), Pvec( 0, 0, 0 ), Pvec( 1, 1, 1 ), Pvec( 2, 2, 2 ) );

            display_mesh( m );

            replace_Hexa_by_Tetra( m );

            display_mesh( m );

            return 0;
        }

    \relates replace_Hexa_by_Tetra
    \relates replace_Wedge_by_Tetra
    \relates replace_Quad_by_Triangle
    \keyword Maillage/Opération
    \friend lecler@lmt.ens-cachan.fr
*/
template <class TM>
void replace_Hexa_by_Tetra( TM &m ) {

    Replace_Hexa_by_Tetra<TM> rh( m );
    
    m.remove_elements_if( rh );
}
 
}
#endif  /// REPLACE_HEXA_BY_TETRA
 
