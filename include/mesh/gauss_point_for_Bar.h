#ifndef LMT_gauss_point_for_simplex_Bar_H
#define LMT_gauss_point_for_simplex_Bar_H

#include "Bar.h"

namespace LMT {

inline void gauss_points( const Bar &elem, unsigned order, Vec<double> &poids, Vec< Vec<double, Bar::nb_var_inter > > &valeurs ) { 
    typedef Vec<double, Bar::nb_var_inter > TPOS;
    
    if ( order <= 1 ) {
        poids.resize( 1 );
        valeurs.resize( 1 );
        poids[0] = 1.0; /// 1
        valeurs[0] = TPOS( 0.5 ); ///  1/2
        return;
    }
    if ( order <= 3 ) {
        poids.resize( 2 );
        valeurs.resize( 2 );
        poids[0] = 0.5; /// 1/2
        poids[1] = 0.5; /// 1/2
        valeurs[0] = TPOS( 0.211324865405187 ); ///  (1-sqrt(1/3))/2 
        valeurs[1] = TPOS( 0.788675134594813 ); ///  (1+sqrt(1/3))/2 
        return;
    }
    if ( order <= 5 ) {
        poids.resize( 3 );
        valeurs.resize( 3 );
        poids[0] = 0.277777777777778; /// (5/9)/2
        poids[1] = 0.444444444444444; /// 8/9
        poids[2] = 0.277777777777778; /// (5/9)/2
        valeurs[0] = TPOS( 0.112701665379258 ); ///  (1-sqrt(3/5))/2 
        valeurs[1] = TPOS( 0.5 ); ///  1/2 
        valeurs[2] = TPOS( 0.887298334620742 ); ///  (1+sqrt(3/5))/2 
        return;
    }
    if ( order <= 7 ) {
        poids.resize( 4 );
        valeurs.resize( 4 );
        poids[0] = 0.173927422568727; /// ((18-sqrt(30))/36)/2
        poids[1] = 0.326072577431273; /// ((18+sqrt(30))/36)/2
        poids[2] = 0.326072577431273; /// ((18+sqrt(30))/36)/2
        poids[3] = 0.173927422568727; /// ((18+sqrt(30))/36)/2
        valeurs[0] = TPOS( 0.069431844202974 ); ///  (1-sqrt(3+2*sqrt(6/5)/7)/2 
        valeurs[1] = TPOS( 0.330009478207572 ); ///  (1-sqrt(3-2*sqrt(6/5)/7)/2 
        valeurs[2] = TPOS( 0.669990521792428 ); ///  (1+sqrt(3-2*sqrt(6/5)/7)/2 
        valeurs[3] = TPOS( 0.930568155797026 ); ///  (1+sqrt(3+2*sqrt(6/5)/7)/2 
        return;
    }
    if ( order <= 9 ) {
        poids.resize( 5 );
        valeurs.resize( 5 );
        poids[0] = 0.118463442528095; /// ((322-13*sqrt(70))/900)/2
        poids[1] = 0.239314335249683; /// ((322+13*sqrt(70))/900)/2
        poids[2] = 0.284444444444444; /// (128/225)/2
        poids[3] = 0.239314335249683; /// ((322+13*sqrt(70))/900)/2
        poids[4] = 0.118463442528095; /// ((322-13*sqrt(70))/900)/2
        valeurs[0] = TPOS( 0.046910077030668 ); ///  (1-1/3*sqrt(5+2*sqrt(10/7)))/2 
        valeurs[1] = TPOS( 0.230765344947159 ); ///  (1-1/3*sqrt(5-2*sqrt(10/7)))/2 
        valeurs[2] = TPOS( 0.5 ); ///  1/2 
        valeurs[3] = TPOS( 0.769234655052841 ); ///  (1+1/3*sqrt(5-2*sqrt(10/7)))/2 
        valeurs[4] = TPOS( 0.953089922969332 ); ///  (1+1/3*sqrt(5+2*sqrt(10/7)))/2 
        return;
    }
    if ( order <= 11 ) {
        poids.resize( 6 );
        valeurs.resize( 6 );
        poids[0] = 0.085662246189585;
        poids[1] = 0.180380786524069;
        poids[2] = 0.233956967286345;
        poids[3] = 0.233956967286345;
        poids[4] = 0.180380786524069;
        poids[5] = 0.085662246189585;
        valeurs[0] = TPOS( 0.033765242898424 );
        valeurs[1] = TPOS( 0.169395306766867 );
        valeurs[2] = TPOS( 0.380690406958402 );
        valeurs[3] = TPOS( 0.619309593041598 );
        valeurs[4] = TPOS( 0.830604693233133 );
        valeurs[5] = TPOS( 0.966234757101576 );
        return;
    }
    /// order == 13
//     if ( order <= 13 ) {
        poids.resize( 7 );
        valeurs.resize( 7 );
        poids[0] = 0.064742483084435;
        poids[1] = 0.139852695744638;
        poids[2] = 0.190915025252559;
        poids[3] = 0.208979591836735;
        poids[4] = 0.190915025252559;
        poids[5] = 0.139852695744638;
        poids[6] = 0.064742483084435;
        valeurs[0] = TPOS( 0.025446043828620 );
        valeurs[1] = TPOS( 0.129234407200303 );
        valeurs[2] = TPOS( 0.297077424311301 );
        valeurs[3] = TPOS( 0.5 );
        valeurs[4] = TPOS( 0.702922575688699 );
        valeurs[5] = TPOS( 0.870765592799697 );
        valeurs[6] = TPOS( 0.974553956171380 );
        return;
//     }
//     std::cerr << "TODO : order > 13" << std::endl; assert(0);
}

} /// LMT
#endif /// LMT_gauss_point_for_simplex_Bar
