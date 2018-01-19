#include "../include/containers/Tens3.h"
#include "../include/containers/Tens4.h"
#include "../include/util/Hdf.h"
#include <fstream>

using namespace std;
using namespace LMT;

void create() {
    Hdf hdf( "res.h5" );

    Vec<int> v;
    v << 10;
    Vec<int,1> s( v.size() );
    hdf.write_data( "/g/v", v.ptr(), s );

    hdf.add_tag( "/g", "tag", "tata" );
}

void read() {
    Hdf hdf( "res.h5" );

    Vec<int> s;
    hdf.read_size( "/g/v", s );
    PRINT( s );

    Vec<int> v;
    v.resize( s[ 0 ] );
    hdf.read_data( "/g/v", v.ptr(), s, s );
    PRINT( v );

    string tag;
    hdf.read_tag( "/g", "tag", tag );
    PRINT( tag );
}

int main() {
    system( "rm res.h5" );
    create();
    read();

//    Hdf hdf( "/home/leclerc/Documents/Publis/ReconstructionFreeDvc/22_151/res.h5" );

//    Vec<int> s;
//    hdf.read_size( "/diff_0", s );
//    PRINT( s );
//    s << 1;

//    Tens3<double> u;
//    u.resize( s );
//    hdf.read_data( "/diff_0", u.ptr(), s, s );

//    PRINT( u( 0, 0, 0 ) );
}



