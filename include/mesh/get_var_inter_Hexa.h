#ifndef GET_VAR_INTER__Hexa_H
#define GET_VAR_INTER__Hexa_H

#include "../containers/system_Poly.h"
#include "../containers/reduction_resultant_for_element.h"
#include "../containers/newton_for_element.h"


namespace LMT {

template<class PosNodes, class Pvec, class TVI >
bool get_var_inter_algebric( const Hexa &elem, const PosNodes &pos_nodes, const Pvec &pos, TVI &var_inter, typename Pvec::template SubType<0>::T tolerance = 1e-7 ) {
    typedef typename Pvec::template SubType<0>::T T;
    typedef Poly<T> P;
    Vec< Vec<T, 20 >, 3 > coeff_poly;
    Vec< Vec<T, 15 >, 2 > poly_2d;
    Vec< P, 3 > syst_p;
    Vec< Vec< T, 2 > > uv;
    Vec< T > w;
    bool isSolution;

    /// résolution par la méthode du résultant

    T R_0 = pos_nodes[6][2]; T R_1 = pos_nodes[7][2]; T R_2 = (-1.0)*R_1; R_2 = R_0+R_2;
    R_0 = pos_nodes[5][2]; T R_3 = (-1.0)*R_0; R_3 = R_2+R_3; R_2 = pos_nodes[2][2];
    T R_4 = (-1.0)*R_2; T R_5 = pos_nodes[4][2]; R_3 = R_5+R_3; R_3 = R_4+R_3;
    R_4 = (-1.0)*R_5; R_1 = R_1+R_4; R_4 = R_0+R_4; R_0 = pos_nodes[3][2];
    R_3 = R_0+R_3; T R_6 = (-1.0)*R_0; R_1 = R_1+R_6; R_6 = R_2+R_6;
    R_2 = pos_nodes[1][2]; R_3 = R_2+R_3; T R_7 = (-1.0)*R_2; R_4 = R_7+R_4;
    R_6 = R_7+R_6; R_7 = pos[2]; R_7 = (-1.0)*R_7; T R_8 = pos_nodes[0][2];
    R_1 = R_8+R_1; coeff_poly[2][8] = R_1; R_4 = R_8+R_4; coeff_poly[2][6] = R_4;
    R_6 = R_8+R_6; coeff_poly[2][5] = R_6; R_6 = (-1.0)*R_8; R_3 = R_3+R_6;
    coeff_poly[2][14] = R_3; R_5 = R_5+R_6; coeff_poly[2][3] = R_5; R_0 = R_0+R_6;
    coeff_poly[2][2] = R_0; R_6 = R_2+R_6; coeff_poly[2][1] = R_6; R_7 = R_8+R_7;
    coeff_poly[2][0] = R_7; R_7 = pos_nodes[6][1]; R_8 = pos_nodes[7][1]; R_6 = (-1.0)*R_8;
    R_6 = R_7+R_6; R_7 = pos_nodes[5][1]; R_2 = (-1.0)*R_7; R_6 = R_2+R_6;
    R_2 = pos_nodes[2][1]; R_0 = (-1.0)*R_2; R_5 = pos_nodes[4][1]; R_6 = R_5+R_6;
    R_0 = R_6+R_0; R_6 = (-1.0)*R_5; R_8 = R_8+R_6; R_6 = R_7+R_6;
    R_7 = pos_nodes[3][1]; R_0 = R_7+R_0; R_3 = (-1.0)*R_7; R_8 = R_8+R_3;
    R_3 = R_2+R_3; R_2 = pos_nodes[1][1]; R_0 = R_2+R_0; R_4 = (-1.0)*R_2;
    R_6 = R_4+R_6; R_4 = R_3+R_4; R_3 = pos[1]; R_3 = (-1.0)*R_3;
    R_1 = pos_nodes[0][1]; R_8 = R_1+R_8; coeff_poly[1][8] = R_8; R_6 = R_1+R_6;
    coeff_poly[1][6] = R_6; R_4 = R_1+R_4; coeff_poly[1][5] = R_4; R_4 = (-1.0)*R_1;
    R_0 = R_4+R_0; coeff_poly[1][14] = R_0; R_5 = R_5+R_4; coeff_poly[1][3] = R_5;
    R_7 = R_7+R_4; coeff_poly[1][2] = R_7; R_4 = R_2+R_4; coeff_poly[1][1] = R_4;
    R_3 = R_1+R_3; coeff_poly[1][0] = R_3; R_3 = pos_nodes[6][0]; R_1 = pos_nodes[7][0];
    R_4 = (-1.0)*R_1; R_4 = R_3+R_4; R_3 = pos_nodes[5][0]; R_2 = (-1.0)*R_3;
    R_4 = R_2+R_4; R_2 = pos_nodes[2][0]; R_7 = (-1.0)*R_2; R_5 = pos_nodes[4][0];
    R_4 = R_5+R_4; R_7 = R_4+R_7; R_4 = (-1.0)*R_5; R_1 = R_1+R_4;
    R_4 = R_3+R_4; R_3 = pos_nodes[3][0]; R_7 = R_3+R_7; R_0 = (-1.0)*R_3;
    R_1 = R_1+R_0; R_0 = R_2+R_0; R_2 = pos_nodes[1][0]; R_7 = R_2+R_7;
    R_6 = (-1.0)*R_2; R_4 = R_4+R_6; R_6 = R_0+R_6; R_0 = pos[0];
    R_0 = (-1.0)*R_0; R_8 = pos_nodes[0][0]; R_1 = R_8+R_1; coeff_poly[0][8] = R_1;
    R_4 = R_8+R_4; coeff_poly[0][6] = R_4; R_6 = R_8+R_6; coeff_poly[0][5] = R_6;
    R_6 = (-1.0)*R_8; R_7 = R_7+R_6; coeff_poly[0][14] = R_7; R_5 = R_5+R_6;
    coeff_poly[0][3] = R_5; R_3 = R_3+R_6; coeff_poly[0][2] = R_3; R_6 = R_2+R_6;
    coeff_poly[0][1] = R_6; R_0 = R_8+R_0; coeff_poly[0][0] = R_0; 
    coeff_poly[0][4] = 0.0; coeff_poly[0][7] = 0.0; coeff_poly[0][9] = 0.0; coeff_poly[0][10] = 0.0;
    coeff_poly[0][11] = 0.0; coeff_poly[0][12] = 0.0; coeff_poly[0][13] = 0.0; coeff_poly[0][15] = 0.0;
    coeff_poly[0][16] = 0.0; coeff_poly[0][17] = 0.0; coeff_poly[0][18] = 0.0; coeff_poly[0][19] = 0.0;
    coeff_poly[1][4] = 0.0; coeff_poly[1][7] = 0.0; coeff_poly[1][9] = 0.0; coeff_poly[1][10] = 0.0;
    coeff_poly[1][11] = 0.0; coeff_poly[1][12] = 0.0; coeff_poly[1][13] = 0.0; coeff_poly[1][15] = 0.0;
    coeff_poly[1][16] = 0.0; coeff_poly[1][17] = 0.0; coeff_poly[1][18] = 0.0; coeff_poly[1][19] = 0.0;
    coeff_poly[2][4] = 0.0; coeff_poly[2][7] = 0.0; coeff_poly[2][9] = 0.0; coeff_poly[2][10] = 0.0;
    coeff_poly[2][11] = 0.0; coeff_poly[2][12] = 0.0; coeff_poly[2][13] = 0.0; coeff_poly[2][15] = 0.0;
    coeff_poly[2][16] = 0.0; coeff_poly[2][17] = 0.0; coeff_poly[2][18] = 0.0; coeff_poly[2][19] = 0.0;
    


    //PRINT( coeff_poly[0] ); 
    //PRINT( coeff_poly[1] ); 
    //PRINT( coeff_poly[2] ); 

    poly_2d[ 0 ] = reduction_resultant_for_element( Hexa(), coeff_poly[ 0 ], coeff_poly[ 2 ] );
    poly_2d[ 1 ] = reduction_resultant_for_element( Hexa(), coeff_poly[ 1 ], coeff_poly[ 2 ] );
    isSolution = solve_polynomial_system_resultant_4_par( uv, poly_2d[ 0 ], poly_2d[ 1 ] ); 

    if ( not( isSolution ) ) { 
        poly_2d[ 0 ] = reduction_resultant_for_element( Hexa(), coeff_poly[ 0 ], coeff_poly[ 1 ] );
        poly_2d[ 1 ] = reduction_resultant_for_element( Hexa(), coeff_poly[ 2 ], coeff_poly[ 1 ] );
        isSolution = solve_polynomial_system_resultant_4_par( uv, poly_2d[ 0 ], poly_2d[ 1 ] ); 
    } 

    if ( not( isSolution ) ) { 
        poly_2d[ 0 ] = reduction_resultant_for_element( Hexa(), coeff_poly[ 1 ], coeff_poly[ 0 ] );
        poly_2d[ 1 ] = reduction_resultant_for_element( Hexa(), coeff_poly[ 2 ], coeff_poly[ 0 ] );
        isSolution = solve_polynomial_system_resultant_4_par( uv, poly_2d[ 0 ], poly_2d[ 1 ] ); 
    } 

    //PRINT( isSolution ); 
    //PRINT( uv ); 
    if ( isSolution ) {
        for( unsigned i = 0; i < uv.size(); ++i ) {
            Vec<T, 2 > cp;
            cp[ 0 ] = coeff_poly[0][0]  + coeff_poly[0][1] * uv[i][ 0 ] + coeff_poly[0][2] * uv[i][ 1 ] + coeff_poly[0][5] * uv[i][ 0 ]* uv[i][ 1 ]; 
            cp[ 1 ] = coeff_poly[0][3]  + coeff_poly[0][6] * uv[i][ 0 ] + coeff_poly[0][8] * uv[i][ 1 ] + coeff_poly[0][14] * uv[i][ 0 ]* uv[i][ 1 ]; 

            isSolution = solve_polynomial_system( w, cp ); 
            if ( not( isSolution ) ) { 
                Vec<T, 2 > cp;
                cp[ 0 ] = coeff_poly[1][0]  + coeff_poly[1][1] * uv[i][ 0 ] + coeff_poly[1][2] * uv[i][ 1 ] + coeff_poly[1][5] * uv[i][ 0 ]* uv[i][ 1 ]; 
                cp[ 1 ] = coeff_poly[1][3]  + coeff_poly[1][6] * uv[i][ 0 ] + coeff_poly[1][8] * uv[i][ 1 ] + coeff_poly[1][14] * uv[i][ 0 ]* uv[i][ 1 ]; 

                isSolution = solve_polynomial_system( w, cp ); 
            } 
            if ( not( isSolution ) ) { 
                Vec<T, 2 > cp;
                cp[ 0 ] = coeff_poly[2][0]  + coeff_poly[2][1] * uv[i][ 0 ] + coeff_poly[2][2] * uv[i][ 1 ] + coeff_poly[2][5] * uv[i][ 0 ]* uv[i][ 1 ]; 
                cp[ 1 ] = coeff_poly[2][3]  + coeff_poly[2][6] * uv[i][ 0 ] + coeff_poly[2][8] * uv[i][ 1 ] + coeff_poly[2][14] * uv[i][ 0 ]* uv[i][ 1 ]; 

                isSolution = solve_polynomial_system( w, cp ); 
            } 

            if ( isSolution ) { 
                for( unsigned j = 0; j < w.size(); ++j ) { 
                    var_inter = TVI( uv[ i ][ 0 ], uv[ i ][ 1 ], w[ j ] ); 
                    if ( var_inter_is_inside( Hexa(), var_inter, tolerance ) ) {
                        Vec<T,3> residu( polyres( coeff_poly[0], var_inter ), polyres( coeff_poly[1], var_inter ), polyres( coeff_poly[2], var_inter ) );
                        if ( newton_for_element( Hexa(), coeff_poly, var_inter, residu ) )
                            return true;
                    }
                }
            }
        }
    }

    syst_p[ 0 ].init( coeff_poly[ 0 ], 3 );
    syst_p[ 1 ].init( coeff_poly[ 1 ], 3 );
    syst_p[ 2 ].init( coeff_poly[ 2 ], 3 );

    //PRINT( syst_p[ 0 ] ); 
    //PRINT( syst_p[ 1 ] ); 
    //PRINT( syst_p[ 2 ] ); 

    Vec< Vec<T,3> > sol_poly;
    if ( solve_basic_polynomial_system( sol_poly, syst_p ) )
        for( unsigned i = 0; i < sol_poly.size(); ++i ) 
            if ( var_inter_is_inside( Hexa(), sol_poly[i], tolerance ) ) {
                var_inter = sol_poly[i];
                return true;
            }

    var_inter = 0.5;
    return false;
}

}
#endif  /// GET_VAR_INTER__Hexa

