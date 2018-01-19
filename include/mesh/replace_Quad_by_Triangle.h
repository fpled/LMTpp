#ifndef REPLACE_QUAD_BY_TRIANGLE_H
#define REPLACE_QUAD_BY_TRIANGLE_H

#include "mesh.h"
#include "Triangle.h"
#include "Quad.h"

namespace LMT {

/*!
    L'opérateur \a Replace_Quad_by_Triangle est conçu pour la fonction \a replace_Quad_by_Triangle .
*/
template<class TM>
struct Replace_Quad_by_Triangle {
    Replace_Quad_by_Triangle( TM &m ) : ptr_m( &m ) {}
    
    template< class NameBehavior, class TNode_, class TData, unsigned num_in_elem_list_ >
    bool operator() ( Element< Quad, NameBehavior, TNode_, TData, num_in_elem_list_ > &e ) {
        typedef typename Element< Quad, NameBehavior, TNode_, TData, num_in_elem_list_ >::TNode TNode;
        Vec<TNode*,3> vn;
        /// Triangle 0
        vn[ 0 ] = e.node( 0 );
        vn[ 1 ] = e.node( 1 );
        vn[ 2 ] = e.node( 3 );
        permutation_if_jac_neg ( Triangle(), vn.ptr() );
        ptr_m->add_element( Triangle(), DefaultBehavior(), vn.ptr() );
        /// Triangle 1
        vn[ 0 ] = e.node( 1 );
        vn[ 1 ] = e.node( 2 );
        vn[ 2 ] = e.node( 3 );
        permutation_if_jac_neg ( Triangle(), vn.ptr() );
        ptr_m->add_element( Triangle(), DefaultBehavior(), vn.ptr() );
        return true;    
    }
    
    /// pour tous les autres éléments autres que \a Quad , on renvoie faux
    template<class TE> 
    bool operator() ( TE &e ) { 
        return false;
    }

    TM* ptr_m;
};

/*!
    Objectif :
        La fonction \a replace_Quad_by_Triangle enlève tous les \a Quad du maillage m et les remplace par des \a Triangle . Le maillage m est donc __modifié__ .

    Paramètre :
        * m : est un maillage de la plateforme qui sera modifié.

    Exemple de code pour remplacer les \a Quad par des \a Triangle dans un maillage :
    \code C/C++

        #include "mesh/make_rect.h"
        #include "mesh/replace_Quad_by_Triangle.h"
        #include "mesh/displayparaview.h"

        // inclusion du code de notre MeshCarac
        #include "MonMeshCarac.h"

        int main( int argc, char **argv ) {
            typedef Mesh< Mesh_carac_MonMeshCarac<double,2> > TM;

            TM m;
            make_rect( m, Quad(), Pvec( 0., 0. ), Pvec( 1., 1. ), Pvec( 2, 2 ) );

            display_mesh( m );

            replace_Quad_by_Triangle( m );

            display_mesh( m );

            return 0;
        }

    \relates replace_Quad_by_Triangle
    \relates replace_Hexa_by_Tetra
    \relates replace_Wedge_by_Triangle
    \keyword Maillage/Opération
    \friend lecler@lmt.ens-cachan.fr
*/
template <class TM>
void replace_Quad_by_Triangle( TM &m ) {

    Replace_Quad_by_Triangle<TM> rq( m );
    
    m.remove_elements_if( rq );
}
 
}
#endif  /// REPLACE_QUAD_BY_TRIANGLE
 
