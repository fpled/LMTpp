#include "../codegen/write_code.h"
#include "../codegen/exvector.h"
#include "../containers/vec.h"
using namespace Codegen;
using namespace std;
using namespace LMT;

void test() {
    float a = 10;
    float _S0 = 0;
    float _S1 = 0;
    while ( true ) {
        float reg0=_S0+_S1; reg0=reg0-1; float reg1=_S0-a; float delta_0=reg1; reg0=reg1-reg0;
        float delta_1=-reg0;
        float max_delta = 0;
        _S0 -= delta_0; max_delta = max( max_delta, abs( delta_0 ) );
        _S1 -= delta_1; max_delta = max( max_delta, abs( delta_1 ) );
        if ( max_delta < 1e-6 ) break;
    }

    //    float _S0 = 0;
    //    float _S1 = 0;
    //    while ( true ) {
    //        float reg0=_S0+_S1; reg0=reg0-1; float reg1=_S0-2; float delta_0=reg1; reg0=reg1-reg0;
    //        float delta_1=-reg0;
    //        float max_delta = 0;
    //        _S0 -= delta_0; max_delta = max( max_delta, abs( delta_0 ) );
    //        _S1 -= delta_1; max_delta = max( max_delta, abs( delta_1 ) );
    //        if ( max_delta < 1e-6 ) break;
    //    }
    //    // s_0+=_S0; s_1+=_S1;

    std::cout << _S0 << " " << _S1 << std::endl;
}

int main() {
    Ex x( "x" ), y( "y" ), a( "a" );
    ExVector eqs( x + y - 1, x - a );
    ExVector unk( x, y );
    ExVector beg( 0, 0 );
    ExVector sol = solve( eqs, unk, beg, 1e-6 );
    PRINT( sol );


    Write_code wc( "float" );
    wc.add( sol[ 0 ], "s_0", Write_code::Add );
    wc.add( sol[ 1 ], "s_1", Write_code::Add );
    // wc.add( sol[ 1 ].diff( a ), "s_1", Write_code::Add );
    std::cout << wc.to_string() << std::endl;

    test();
}

