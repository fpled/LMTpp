#ifndef LMT_gauss_point_for_simplex_Hexa_H
#define LMT_gauss_point_for_simplex_Hexa_H

#include "Hexa.h"
#include "Bar.h"
#include "gauss_point_for_Bar.h"

namespace LMT {
/*!
    Si want_product == false, order = ordre total -> TODO
    Si want_product == true, order = ordre max partiel (pour chacune des var_inter prise separement) * dim
*/
inline void gauss_points( const Hexa &elem, unsigned order, Vec<double> &poids, Vec< Vec<double, Hexa::nb_var_inter > > &valeurs, bool want_product = true ) { 
    typedef Vec<double, Hexa::nb_var_inter > TPOS;
    
    if ( want_product == false ) {
        std::cerr << "TODO " << std::endl; assert(0);
    }
    else {
        Vec<double> pond;
        Vec< Vec<double, Bar::nb_var_inter > > val;
        unsigned order_Bar = order; // ceil(order,Hexa::nb_var_inter)/Hexa::nb_var_inter;
        gauss_points( Bar(), order_Bar, pond, val );
        unsigned nb_pts_gauss = pow( pond.size(), Hexa::nb_var_inter );
        poids.resize( nb_pts_gauss );
        valeurs.resize( nb_pts_gauss );
        for(unsigned i=0; i<pond.size(); ++i) {;
            for(unsigned j=0; j<pond.size(); ++j) {
                for(unsigned k=0; k<pond.size(); ++k) {
                    poids[ ( i * pond.size() + j ) * pond.size() + k ] = pond[ i ] * pond[ j ] * pond[ k ];
                    valeurs[ ( i * pond.size() + j ) * pond.size() + k ] = TPOS( val[ i ][ 0 ], val[ j ][ 0 ], val[ k ][ 0 ] );
                }
            }
        }
        val.free();
        pond.free();
        return;
    }
}

} /// LMT
#endif /// LMT_gauss_point_for_simplex_Hexa
