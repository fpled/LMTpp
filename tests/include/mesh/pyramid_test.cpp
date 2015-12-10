#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "../../../include/containers/vec.h"
#include "../../../include/mesh/Pyramid.h"

using namespace LMT;

#ifdef METIL_COMP_DIRECTIVE
#pragma lib_name xml2
#endif

int var_inter_is_inside_Pyramid_0() {
    return var_inter_is_inside( Pyramid(), Vec<double,3>( 0, 0, 0 ), 1e-20 );
};

int var_inter_is_inside_Pyramid_1() {
    return var_inter_is_inside( Pyramid(), Vec<double,3>( 0.5, 0.5, 0.5 ), 1e-20 );
};

int var_inter_is_inside_Pyramid_2() {
    return not( var_inter_is_inside( Pyramid(), Vec<double,3>( 1, 1, 1 ), 1e-20 ) );
};

BOOST_AUTO_TEST_SUITE()
   BOOST_AUTO_TEST_CASE( var_inter_is_inside_Pyramid_0_case ){
   	BOOST_REQUIRE_EQUAL( var_inter_is_inside_Pyramid_0(), 1 );
   }

   BOOST_AUTO_TEST_CASE( var_inter_is_inside_Pyramid_1_case ){
   	BOOST_REQUIRE_EQUAL( var_inter_is_inside_Pyramid_1() , 1 );
   }

   /*BOOST_AUTO_TEST_CASE( var_inter_is_inside_Pyramid_2_case ){
   	BOOST_REQUIRE_EQUAL( var_inter_is_inside_Pyramid_2(), 1 );
   }*/
BOOST_AUTO_TEST_SUITE_END()
   



