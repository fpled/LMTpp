#include <containers/Tens3.h>
#include <util/Hdf.h>
#include <fstream>
using namespace std;
using namespace LMT;


int main() {
    Hdf hdf( "/home/leclerc/Documents/Publis/ReconstructionFreeDvc/22_151/res.h5" );

    Vec<int> s;
    hdf.read_size( "/diff_0", s );
    PRINT( s );
    s << 1; // hum (/diff_0 has size 512 512)

    Tens3<double> u;
    u.resize( s );
    hdf.read_data( "/diff_0", u.ptr(), s, s );

    PRINT( u( 0, 0, 0 ) );
}



