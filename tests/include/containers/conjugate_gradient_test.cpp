#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#define ERROR_UNIT_TEST( a , b ) std::abs( (a) - (b) ) / ( 1e-20 + std::abs( a ) + std::abs( b ) )
#include "../../../include/containers/matinvsparse.h"
#include "../../../include/containers/conjugate_gradient.h"
#include <iostream>

using namespace LMT;

#ifdef METIL_COMP_DIRECTIVE
#pragma lib_name pthread
#endif

int conjugate_gradient_0() {
    Mat<double, Sym<>, SparseLine<> > m(3,3);
    m(0,0) = 1; m(0,1) = 2; m(0,2) = -3;
                m(1,1) = 5; m(1,2) = -7;
                            m(2,2) = 13;
    PRINTN( m );
    Vec<double, 3> s;
    Vec<double, 3> b(3,4,5);
    Mat<double, Sym<>, SparseLine<> > K( m );
    incomplete_chol_factorize( K );
    PRINTN( K );
    
    unsigned cpt_iter = conjugate_gradient( new_SolveUsingCholFactorize( K ), m, b, s, ConvergenceCriteriumNormInfDelta<double>( 1e-5 ) );
    PRINT( cpt_iter );
    PRINT( s );
    
    bool r0 = ERROR_UNIT_TEST( s[0], 11.0 ) < 1e-12;
    bool r1 = ERROR_UNIT_TEST( s[1], 2.0  ) < 1e-12;
    bool r2 = ERROR_UNIT_TEST( s[2], 4.0  ) < 1e-12;
    return r0 and r1 and r2;
};

int conjugate_gradient_1() {
    Mat<double, Sym<>, SparseLine<> > M(  10, 10 );

    M( 0, 0 ) = 348; M( 0, 1 ) = 295; M( 0, 2 ) = 221; M( 0, 3 ) = 165; M( 0, 4 ) = 221; M( 0, 5 ) = 311; M( 0, 6 ) = 311; M( 0, 7 ) = 279; M( 0, 8 ) = 216; M( 0, 9 ) = 200; 
    M( 1, 0 ) = 295; M( 1, 1 ) = 333; M( 1, 2 ) = 113; M( 1, 3 ) = 152; M( 1, 4 ) = 174; M( 1, 5 ) = 271; M( 1, 6 ) = 244; M( 1, 7 ) = 250; M( 1, 8 ) = 169; M( 1, 9 ) = 179; 
    M( 2, 0 ) = 221; M( 2, 1 ) = 113; M( 2, 2 ) = 277; M( 2, 3 ) = 125; M( 2, 4 ) = 166; M( 2, 5 ) = 215; M( 2, 6 ) = 272; M( 2, 7 ) = 159; M( 2, 8 ) = 207; M( 2, 9 ) = 134; 
    M( 3, 0 ) = 165; M( 3, 1 ) = 152; M( 3, 2 ) = 125; M( 3, 3 ) = 152; M( 3, 4 ) = 155; M( 3, 5 ) = 135; M( 3, 6 ) = 206; M( 3, 7 ) = 107; M( 3, 8 ) = 143; M( 3, 9 ) = 143; 
    M( 4, 0 ) = 221; M( 4, 1 ) = 174; M( 4, 2 ) = 166; M( 4, 3 ) = 155; M( 4, 4 ) = 270; M( 4, 5 ) = 250; M( 4, 6 ) = 280; M( 4, 7 ) = 170; M( 4, 8 ) = 219; M( 4, 9 ) = 151; 
    M( 5, 0 ) = 311; M( 5, 1 ) = 271; M( 5, 2 ) = 215; M( 5, 3 ) = 135; M( 5, 4 ) = 250; M( 5, 5 ) = 412; M( 5, 6 ) = 389; M( 5, 7 ) = 287; M( 5, 8 ) = 247; M( 5, 9 ) = 167; 
    M( 6, 0 ) = 311; M( 6, 1 ) = 244; M( 6, 2 ) = 272; M( 6, 3 ) = 206; M( 6, 4 ) = 280; M( 6, 5 ) = 389; M( 6, 6 ) = 474; M( 6, 7 ) = 259; M( 6, 8 ) = 259; M( 6, 9 ) = 219; 
    M( 7, 0 ) = 279; M( 7, 1 ) = 250; M( 7, 2 ) = 159; M( 7, 3 ) = 107; M( 7, 4 ) = 170; M( 7, 5 ) = 287; M( 7, 6 ) = 259; M( 7, 7 ) = 274; M( 7, 8 ) = 140; M( 7, 9 ) = 159; 
    M( 8, 0 ) = 216; M( 8, 1 ) = 169; M( 8, 2 ) = 207; M( 8, 3 ) = 143; M( 8, 4 ) = 219; M( 8, 5 ) = 247; M( 8, 6 ) = 259; M( 8, 7 ) = 140; M( 8, 8 ) = 297; M( 8, 9 ) = 141; 
    M( 9, 0 ) = 200; M( 9, 1 ) = 179; M( 9, 2 ) = 134; M( 9, 3 ) = 143; M( 9, 4 ) = 151; M( 9, 5 ) = 167; M( 9, 6 ) = 219; M( 9, 7 ) = 159; M( 9, 8 ) = 141; M( 9, 9 ) = 165;
    
    PRINTN( M );
    Vec<double, 10> s;
    double s_Maple[10] = {-1.4176810081375044408 , -3.0067729851774937789 , 3.5389214608316368121 , 7.5278767154067853482 , -.95330870992906846260 , 16.004952790217101221 , -11.900092892559983095 , -7.5660154416662888416 , -6.0846051557343736576 , 8.5770626957989029109 };
    Vec<double, 10> b( 0 , 8 , 1 , 4 , 6 , 6 , 0 , 9 , 0 , 6  );
    Mat<double, Sym<>, SparseLine<> > K( M );
    incomplete_chol_factorize( K );
    
    unsigned cpt_iter = conjugate_gradient( new_SolveUsingCholFactorize( K ), M, b, s, ConvergenceCriteriumNormInfDelta<double>( 1e-5 ) );
    PRINT( cpt_iter );
    PRINT( s );
    for( unsigned i =0; i<10 ; ++i )
        PRINT( s_Maple[i] );
    
    bool res[10];
    for( unsigned i =0; i<10 ; ++i )
        res[i] = ERROR_UNIT_TEST( s[i], s_Maple[i] ) < 1e-12;
    bool final = true;
    for( unsigned i =0; i<10 ; ++i )
        final &= res[i];
    return final;
};


BOOST_AUTO_TEST_SUITE()
    BOOST_AUTO_TEST_CASE( conjugate_gradient_0_case ){
        
        BOOST_CHECK_EQUAL( 1, 1 );
    }

   BOOST_AUTO_TEST_CASE( conjugate_gradient_1_case ){
        
        BOOST_CHECK_EQUAL( conjugate_gradient_0() , 1 );
    }
BOOST_AUTO_TEST_SUITE_END()


