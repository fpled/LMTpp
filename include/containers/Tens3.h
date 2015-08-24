#ifndef TENS3_H
#define TENS3_H

#include "vec.h"

namespace LMT {

template<class T>
struct Tens3 {

    template<class TV>
    void resize( const TV &size ) {
        data.resize( size[ 0 ] * size[ 1 ] * size[ 2 ] );
        this->size = size;
    }

    T &operator()( long x, long y, long z ) {
        return data[ z * size[ 0 ] * size[ 1 ] + y * size[ 0 ] + x ];
    }

    T *ptr() {
        return data.ptr();
    }

    template<class T2> void set(const T2 &v) { data.set(v); }

    Vec<int> size;
    Vec<T>   data;
};

}

#endif // TENS3_H
