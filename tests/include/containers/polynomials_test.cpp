#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>


#include <complex>
#include <iostream>
#include <iomanip>
#include "../../../include/containers/polynomials.h"
#include "header_linker_test.h"

#define TOTO( a , b ) std::abs( (a) - (b) ) / ( 1e-20 + std::abs( a ) + std::abs( b ) )

using namespace LMT;

int pol_degree_4_double_univariable_0() {
    typedef complex<double> C;
    Pol<4,1,double> P(Vec<double,5>(0.089772384592798626, -0.60532420499754847, -1.0755986070143191, -0.66382385694473234, -3.6694483295694546e-08) );
    double prec = 1e-4;
    
    Vec< C > sol = P.roots();
    std::cout << " racines => .120557492003962, -.870432417520779+.603404119273391*I, -18090561.2470167, -.870432417520779-.603404119273391*I" << std::endl;
    std::cout << std::setprecision( 16 ) << sol << std::endl;
    bool r0 = TOTO( sol[1], C( .120557492003962 ) ) < prec;
    bool r1 = TOTO( sol[2], C( -.870432417520779, .603404119273391 ) ) < prec;
    bool r2 = TOTO( sol[3], C( -.870432417520779,-.603404119273391 ) ) < prec;
    bool r3 = TOTO( sol[0], C( -18090561.2470167 ) ) < prec;
    return r0 and r1 and r2 and r3;
}

BOOST_AUTO_TEST_SUITE()
   BOOST_AUTO_TEST_CASE( pol_degree_4_double_univariable_0_case ){
     
    	BOOST_CHECK_EQUAL( pol_degree_4_double_univariable_0(), 1 );
   }
BOOST_AUTO_TEST_SUITE_END()
  
    



