#ifndef GET_VAR_INTER__Quad_9_H
#define GET_VAR_INTER__Quad_9_H

#include "../containers/system_Poly.h"


namespace LMT {

template<class PosNodes, class Pvec, class TVI >
bool get_var_inter_algebric( const Quad_9 &elem, const PosNodes &pos_nodes, const Pvec &pos, TVI &var_inter, typename Pvec::template SubType<0>::T tolerance = 1e-7 ) {
    typedef typename Pvec::template SubType<0>::T T;
    typedef Poly<T> P;
    Vec< Vec<T, 15 >, 2 > coeff_poly;
    Vec< P, 2 > syst_p;
    Vec< Vec< T, 2 > > sol_poly;
    bool isSolution;

    /// résolution par la méthode du résultant

    T R_0 = pos_nodes[6][1]; T R_1 = (-8.0)*R_0; T R_2 = 8.0*R_0; T R_3 = 4.0*R_0;
    R_0 = (-4.0)*R_0; T R_4 = pos_nodes[5][1]; T R_5 = (-8.0)*R_4; T R_6 = 4.0*R_4;
    T R_7 = 8.0*R_4; R_4 = (-4.0)*R_4; T R_8 = pos_nodes[8][1]; T R_9 = (-16.0)*R_8;
    R_8 = 16.0*R_8; T R_10 = pos_nodes[2][1]; T R_11 = 4.0*R_10; T R_12 = (-2.0)*R_10;
    T R_13 = pos_nodes[7][1]; T R_14 = (-8.0)*R_13; T R_15 = 12.0*R_13; T R_16 = 8.0*R_13;
    T R_17 = (-4.0)*R_13; T R_18 = (-12.0)*R_13; R_13 = 4.0*R_13; T R_19 = pos_nodes[3][1];
    T R_20 = 4.0*R_19; T R_21 = (-6.0)*R_19; T R_22 = (-2.0)*R_19; T R_23 = 2.0*R_19;
    T R_24 = 3.0*R_19; R_19 = (-1.0)*R_19; T R_25 = pos_nodes[1][1]; T R_26 = 4.0*R_25;
    R_5 = R_26+R_5; R_26 = (-2.0)*R_25; R_26 = R_6+R_26; R_6 = (-6.0)*R_25;
    R_6 = R_7+R_6; R_7 = 3.0*R_25; R_4 = R_7+R_4; R_7 = 2.0*R_25;
    R_25 = (-1.0)*R_25; T R_27 = pos_nodes[4][1]; T R_28 = (-8.0)*R_27; T R_29 = 8.0*R_27;
    T R_30 = 12.0*R_27; T R_31 = (-12.0)*R_27; T R_32 = (-4.0)*R_27; R_27 = 4.0*R_27;
    T R_33 = pos[1]; R_33 = (-1.0)*R_33; T R_34 = pos_nodes[0][1]; T R_35 = 4.0*R_34;
    R_35 = R_5+R_35; R_35 = R_28+R_35; R_14 = R_35+R_14; R_14 = R_8+R_14;
    R_20 = R_14+R_20; R_20 = R_1+R_20; R_11 = R_20+R_11; coeff_poly[1][12] = R_11;
    R_11 = (-6.0)*R_34; R_26 = R_11+R_26; R_26 = R_29+R_26; R_15 = R_26+R_15;
    R_15 = R_9+R_15; R_21 = R_15+R_21; R_21 = R_2+R_21; R_21 = R_12+R_21;
    coeff_poly[1][8] = R_21; R_11 = R_6+R_11; R_11 = R_30+R_11; R_16 = R_11+R_16;
    R_16 = R_9+R_16; R_16 = R_22+R_16; R_3 = R_16+R_3; R_3 = R_12+R_3;
    coeff_poly[1][7] = R_3; R_3 = 9.0*R_34; R_4 = R_3+R_4; R_4 = R_31+R_4;
    R_4 = R_18+R_4; R_4 = R_8+R_4; R_24 = R_4+R_24; R_0 = R_24+R_0;
    R_0 = R_10+R_0; coeff_poly[1][4] = R_0; R_0 = 2.0*R_34; R_17 = R_17+R_0;
    R_17 = R_23+R_17; coeff_poly[1][5] = R_17; R_7 = R_0+R_7; R_7 = R_32+R_7;
    coeff_poly[1][3] = R_7; R_7 = (-3.0)*R_34; R_13 = R_13+R_7; R_13 = R_19+R_13;
    coeff_poly[1][2] = R_13; R_7 = R_25+R_7; R_7 = R_27+R_7; coeff_poly[1][1] = R_7;
    R_33 = R_34+R_33; coeff_poly[1][0] = R_33; R_33 = pos_nodes[8][0]; R_34 = (-16.0)*R_33;
    R_33 = 16.0*R_33; R_7 = pos_nodes[5][0]; R_27 = (-8.0)*R_7; R_25 = 4.0*R_7;
    R_13 = 8.0*R_7; R_7 = (-4.0)*R_7; R_19 = pos_nodes[6][0]; R_32 = (-8.0)*R_19;
    R_0 = 8.0*R_19; R_17 = 4.0*R_19; R_19 = (-4.0)*R_19; R_23 = pos_nodes[2][0];
    R_10 = 4.0*R_23; R_24 = (-2.0)*R_23; R_4 = pos_nodes[7][0]; R_8 = (-8.0)*R_4;
    R_18 = 12.0*R_4; R_31 = 8.0*R_4; R_3 = (-4.0)*R_4; R_12 = (-12.0)*R_4;
    R_4 = 4.0*R_4; R_16 = pos_nodes[3][0]; R_22 = 4.0*R_16; R_9 = (-6.0)*R_16;
    R_11 = (-2.0)*R_16; R_30 = 2.0*R_16; R_6 = 3.0*R_16; R_16 = (-1.0)*R_16;
    R_21 = pos_nodes[1][0]; R_2 = 4.0*R_21; R_27 = R_2+R_27; R_2 = (-2.0)*R_21;
    R_2 = R_25+R_2; R_25 = (-6.0)*R_21; R_13 = R_25+R_13; R_25 = 3.0*R_21;
    R_25 = R_7+R_25; R_7 = 2.0*R_21; R_21 = (-1.0)*R_21; R_15 = pos_nodes[4][0];
    R_26 = (-8.0)*R_15; R_29 = 8.0*R_15; R_20 = 12.0*R_15; R_1 = (-12.0)*R_15;
    R_14 = (-4.0)*R_15; R_15 = 4.0*R_15; R_35 = pos[0]; R_35 = (-1.0)*R_35;
    R_28 = pos_nodes[0][0]; R_5 = 4.0*R_28; R_27 = R_5+R_27; R_27 = R_26+R_27;
    R_27 = R_8+R_27; R_27 = R_27+R_33; R_27 = R_22+R_27; R_32 = R_27+R_32;
    R_10 = R_32+R_10; coeff_poly[0][12] = R_10; R_10 = (-6.0)*R_28; R_2 = R_10+R_2;
    R_2 = R_29+R_2; R_2 = R_18+R_2; R_2 = R_2+R_34; R_2 = R_9+R_2;
    R_2 = R_0+R_2; R_2 = R_2+R_24; coeff_poly[0][8] = R_2; R_13 = R_10+R_13;
    R_13 = R_20+R_13; R_13 = R_31+R_13; R_13 = R_34+R_13; R_11 = R_13+R_11;
    R_11 = R_17+R_11; R_24 = R_11+R_24; coeff_poly[0][7] = R_24; R_24 = 9.0*R_28;
    R_25 = R_24+R_25; R_25 = R_1+R_25; R_25 = R_12+R_25; R_33 = R_25+R_33;
    R_33 = R_6+R_33; R_33 = R_19+R_33; R_33 = R_23+R_33; coeff_poly[0][4] = R_33;
    R_33 = 2.0*R_28; R_3 = R_3+R_33; R_3 = R_30+R_3; coeff_poly[0][5] = R_3;
    R_33 = R_7+R_33; R_33 = R_14+R_33; coeff_poly[0][3] = R_33; R_33 = (-3.0)*R_28;
    R_4 = R_4+R_33; R_4 = R_16+R_4; coeff_poly[0][2] = R_4; R_33 = R_21+R_33;
    R_33 = R_15+R_33; coeff_poly[0][1] = R_33; R_35 = R_28+R_35; coeff_poly[0][0] = R_35;
    coeff_poly[0][6] = 0.0; coeff_poly[0][9] = 0.0; coeff_poly[0][10] = 0.0; coeff_poly[0][11] = 0.0;
    coeff_poly[0][13] = 0.0; coeff_poly[0][14] = 0.0; coeff_poly[1][6] = 0.0; coeff_poly[1][9] = 0.0;
    coeff_poly[1][10] = 0.0; coeff_poly[1][11] = 0.0; coeff_poly[1][13] = 0.0; coeff_poly[1][14] = 0.0;
    


    //PRINT( coeff_poly[0] ); 
    //PRINT( coeff_poly[1] ); 

    isSolution = solve_polynomial_system_resultant_4_par( sol_poly, coeff_poly[ 0 ], coeff_poly[ 1 ] ); 
    //PRINT( isSolution ); 
    //PRINT( sol_poly ); 
    if ( isSolution ) {
        for( unsigned i = 0; i < sol_poly.size(); ++i ) 
            if ( var_inter_is_inside( Quad_9(), sol_poly[i], tolerance ) ) {
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
            if ( var_inter_is_inside( Quad_9(), sol_poly[i], tolerance ) ) {
                var_inter = sol_poly[i];
                return true;
            }

    var_inter = 0.5;
    return false;
}

}
#endif  /// GET_VAR_INTER__Quad_9

