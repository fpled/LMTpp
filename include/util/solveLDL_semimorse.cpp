#include "../containers/simd.h"
#include "beep.h"
extern "C" double _ddot( const double &n, const double *x, const double &step_x , const double *y, const double &step_y );

typedef double T;
using namespace LMT;

///
void semi_morse_lsolve_( double *Y, unsigned nb_beep_lines, const LineOfBeeps *beep_lines ) {
    static const unsigned al = SimdSize<T>::res;
    typedef SimdVec<T,al> TS;
    
    for(unsigned line=0;line<nb_beep_lines;++line) {
        for(unsigned nbs=0;nbs<beep_lines[line].small_size_beeps.size();++nbs) {
            const Beep &b = beep_lines[line].small_size_beeps[nbs];
            for(unsigned i=0;i<b.size;++i)
                Y[ b.col + i ] -= b.data[ i ] * Y[ line ];
        }
        for(unsigned nbs=0;nbs<beep_lines[line].large_size_beeps.size();++nbs) {
            const Beep &b = beep_lines[line].large_size_beeps[nbs];
            
            int i = 0;
            T *bp = b.data, *yp = &Y[b.col], yline = Y[line];
            TS yline_ts = yline;
            if ( b.col % al ) { // assuming al == 2
                yp[0] -= bp[0] * yline;
                ++i;
            }
            for( ; i<(int)b.size-4*int(al)+1; i+=4*al ) {
                __builtin_prefetch( bp + i + 128 );
                __builtin_prefetch( yp + i + 128, 1 );
                reinterpret_cast<TS &>( yp[ i + 0*al ] ) -= reinterpret_cast<TS &>( bp[ i + 0*al ] ) * yline_ts;
                reinterpret_cast<TS &>( yp[ i + 1*al ] ) -= reinterpret_cast<TS &>( bp[ i + 1*al ] ) * yline_ts;
                reinterpret_cast<TS &>( yp[ i + 2*al ] ) -= reinterpret_cast<TS &>( bp[ i + 2*al ] ) * yline_ts;
                reinterpret_cast<TS &>( yp[ i + 3*al ] ) -= reinterpret_cast<TS &>( bp[ i + 3*al ] ) * yline_ts;
            }
            for( ; i<(int)b.size-int(al)+1; i+=al )
                reinterpret_cast<TS &>( yp[ i + 0*al ] ) -= reinterpret_cast<TS &>( bp[ i + 0*al ] ) * yline_ts;
            for( ; i<(int)b.size; ++i )
                yp[i] -= bp[i] * yline;
        }
    }
}

///
void semi_morse_ltsolve_( double *Y, unsigned nb_beep_lines, const LineOfBeeps *beep_lines ) {
    static const unsigned al = SimdSize<T>::res;
    typedef SimdVec<T,al> TS;
    
    for(int line=nb_beep_lines-1;line>=0;--line) {
        for(unsigned nbs=0;nbs<beep_lines[line].small_size_beeps.size();++nbs) {
            const Beep &b = beep_lines[line].small_size_beeps[nbs];
            // Y[ line ] -= _ddot( b.size, b.data, 1, Y + b.col, 1 );
            T r = Y[ line ];
            for(unsigned i=0;i<b.size;++i)
                r -= b.data[ i ] * Y[ b.col + i ];
            Y[line] = r;
        }
        for(unsigned nbs=0;nbs<beep_lines[line].large_size_beeps.size();++nbs) {
            const Beep &b = beep_lines[line].large_size_beeps[nbs];
            //
            T r = Y[ line ];
            
            int i = 0;
            T *bp = b.data, *yp = &Y[b.col];
            if ( b.col % al ) { // assuming al == 2
                r -= bp[0] * yp[0];
                ++i;
            }
            TS r0 = 0.0, r1 = 0.0, r2 = 0.0, r3 = 0.0;
            for( ; i<(int)b.size-4*int(al)+1; i+=4*al ) {
                __builtin_prefetch( bp + i - 128 );
                // __builtin_prefetch( yp + i - 128 );
                r0 -= reinterpret_cast<TS &>( bp[ i + 0*al ] ) * reinterpret_cast<TS &>( yp[ i + 0*al ] );
                r1 -= reinterpret_cast<TS &>( bp[ i + 1*al ] ) * reinterpret_cast<TS &>( yp[ i + 1*al ] );
                r2 -= reinterpret_cast<TS &>( bp[ i + 2*al ] ) * reinterpret_cast<TS &>( yp[ i + 2*al ] );
                r3 -= reinterpret_cast<TS &>( bp[ i + 3*al ] ) * reinterpret_cast<TS &>( yp[ i + 3*al ] );
            }
            r2 += r3;
            for( ; i<(int)b.size-int(al)+1; i+=al )
                r0 -= *reinterpret_cast<TS *>( bp + i ) * *reinterpret_cast<TS *>( yp + i );
            r1 += r2;
            for( ; i<(int)b.size; ++i )
                r -= bp[i] * yp[i];
            r += r0[0] + r0[1] + r1[0] + r1[1];
            
            Y[line] = r;
        }
    }
}
