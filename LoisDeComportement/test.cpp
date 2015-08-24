#include "../include/containers/gnuplot.h"
#include "../include/containers/mat.h"
#include "plast_impl_umat.h"
using namespace LMT;

int main() {
    Vec<double,6> stress;
    Vec<double,6> statev( 0 );
    Vec<double,3> props( 210e9, 0.3, 200e6 );
    Mat<double,Gen<6> > ddsdde;
    double time = 0, dtime = 1;

    Vec<double,6> stran( 0 );
    Vec<double,6> tstran( 0.1, -0.03, -0.03, 0, 0, 0 );
    
    Vec<double> s_0;
    for( double inc = 0, step = 0.001; inc < 0.1; inc += step ) {
        Vec<double,6> dstran = step * tstran;
        umat_( stress.ptr(), statev.ptr(), ddsdde.data.ptr(), 0, 0,
                0, 0, 0,
                0, 0, stran.ptr(), dstran.ptr(),
                &time, &dtime, 0, 0, 0,
                0, 0, 0, 0, 0, 0,
                props.ptr(), 0, 0, 0,
                0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 
                0 );

        
        PRINT( stran );
        PRINT( dstran );
        PRINT( stress );
        PRINT( statev );
        s_0 << stress[ 0 ];
        
        stran += dstran;
    }
    
    plot( s_0 );
}