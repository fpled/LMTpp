#ifndef GET_VAR_INTER__Triangle_6_H
#define GET_VAR_INTER__Triangle_6_H

#include "../containers/system_Poly.h"


namespace LMT {

template<class PosNodes, class Pvec, class TVI >
bool get_var_inter_algebric( const Triangle_6 &elem, const PosNodes &pos_nodes, const Pvec &pos, TVI &var_inter, typename Pvec::template SubType<0>::T tolerance = 1e-7 ) {
    typedef typename Pvec::template SubType<0>::T T;
    typedef Poly<T> P;
    Vec< Vec<T, 6 >, 2 > coeff_poly;
    Vec< P, 2 > syst_p;
    Vec< Vec< T, 2 > > sol_poly;
    bool isSolution;

    /// résolution par la méthode du résultant

    T R_0 = pos_nodes[4][1]; R_0 = 4.0*R_0; T R_1 = pos_nodes[5][1]; T R_2 = (-4.0)*R_1;
    R_1 = 4.0*R_1; T R_3 = pos_nodes[2][1]; T R_4 = 2.0*R_3; R_3 = (-1.0)*R_3;
    T R_5 = pos_nodes[1][1]; T R_6 = 2.0*R_5; R_5 = (-1.0)*R_5; T R_7 = pos_nodes[3][1];
    T R_8 = (-4.0)*R_7; R_0 = R_0+R_8; R_7 = 4.0*R_7; T R_9 = pos[1];
    R_9 = (-1.0)*R_9; T R_10 = pos_nodes[0][1]; T R_11 = 4.0*R_10; R_11 = R_0+R_11;
    R_11 = R_11+R_2; coeff_poly[1][4] = R_11; R_11 = 2.0*R_10; R_4 = R_11+R_4;
    R_2 = R_4+R_2; coeff_poly[1][5] = R_2; R_8 = R_11+R_8; R_6 = R_8+R_6;
    coeff_poly[1][3] = R_6; R_6 = (-3.0)*R_10; R_3 = R_3+R_6; R_1 = R_3+R_1;
    coeff_poly[1][2] = R_1; R_6 = R_7+R_6; R_5 = R_6+R_5; coeff_poly[1][1] = R_5;
    R_9 = R_10+R_9; coeff_poly[1][0] = R_9; R_9 = pos_nodes[4][0]; R_9 = 4.0*R_9;
    R_10 = pos_nodes[5][0]; R_5 = (-4.0)*R_10; R_10 = 4.0*R_10; R_6 = pos_nodes[2][0];
    R_7 = 2.0*R_6; R_6 = (-1.0)*R_6; R_1 = pos_nodes[1][0]; R_3 = 2.0*R_1;
    R_1 = (-1.0)*R_1; R_8 = pos_nodes[3][0]; R_11 = (-4.0)*R_8; R_9 = R_9+R_11;
    R_8 = 4.0*R_8; R_2 = pos[0]; R_2 = (-1.0)*R_2; R_4 = pos_nodes[0][0];
    R_0 = 4.0*R_4; R_9 = R_0+R_9; R_9 = R_5+R_9; coeff_poly[0][4] = R_9;
    R_9 = 2.0*R_4; R_7 = R_7+R_9; R_7 = R_5+R_7; coeff_poly[0][5] = R_7;
    R_11 = R_9+R_11; R_11 = R_3+R_11; coeff_poly[0][3] = R_11; R_11 = (-3.0)*R_4;
    R_6 = R_6+R_11; R_10 = R_6+R_10; coeff_poly[0][2] = R_10; R_11 = R_8+R_11;
    R_1 = R_11+R_1; coeff_poly[0][1] = R_1; R_2 = R_4+R_2; coeff_poly[0][0] = R_2;

    //std::cout << setprecision( 17 );
    //PRINT( coeff_poly[0] ); 
    //PRINT( coeff_poly[1] ); 

    isSolution = solve_polynomial_system_resultant_2( sol_poly, coeff_poly[ 0 ], coeff_poly[ 1 ] ); 
    //PRINT( isSolution ); 
    //PRINT( sol_poly ); 
    if ( isSolution ) {
        for( unsigned i = 0; i < sol_poly.size(); ++i ) 
            if ( var_inter_is_inside( Triangle_6(), sol_poly[i], tolerance ) ) {
                var_inter = sol_poly[i];
                return true;
            }
    }

    syst_p[ 0 ].init( coeff_poly[ 0 ], 2 );
    syst_p[ 1 ].init( coeff_poly[ 1 ], 2 );

    //PRINT( syst_p[ 0 ] ); 
    //PRINT( syst_p[ 1 ] ); 

    if ( solve_basic_polynomial_system( sol_poly, syst_p ) )
        for( unsigned i = 0; i < sol_poly.size(); ++i ) 
            if ( var_inter_is_inside( Triangle_6(), sol_poly[i], tolerance ) ) {
                var_inter = sol_poly[i];
                return true;
            }

    var_inter = 0.5;
    return false;
}

}
#endif  /// GET_VAR_INTER__Triangle_6

