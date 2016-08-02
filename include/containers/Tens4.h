#ifndef TENS4_H
#define TENS4_H

#include "vec.h"

namespace LMT {

template<class T>
struct Tens4 {

    template<class TV>
    void resize( const TV &size ) {
        data.resize( size[ 0 ] * size[ 1 ] * size[ 2 ] * size[ 3 ] );
        this->size = size;
    }

    T &operator()( long x, long y, long z, long t ) {
        return data[ t * size[ 0 ] * size[ 1 ] * size[ 2 ] + z * size[ 0 ] * size[ 1 ] + y * size[ 0 ] + x ];
    }

    T *ptr() {
        return data.ptr();
    }

    template<class T2> void set(const T2 &v) { data.set(v); }

    Vec<int> size;
    Vec<T>   data;
};

}

#endif // TENS4_H
