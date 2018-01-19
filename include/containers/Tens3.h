#ifndef TENS3_H
#define TENS3_H

#include "vec.h"

namespace LMT {

template<class T>
struct Tens3 {

    T &operator[](unsigned index) { return data[ index ]; }
    const T &operator[](unsigned index) const { return data[ index ]; }

    T &operator[](int index) { return data[ index ]; }
    const T &operator[](int index) const { return data[ index ]; }

    void clear() { data.set((T)0); } /// set all values to 0

    T trace() const
    {
        T tmp = 0;
        for (unsigned k=0; k<size[ 0 ]; ++k)
            tmp += operator()(k,k,k);
        return tmp;
    }

    T &operator()( long x, long y, long z ) {
        return data[ z * size[ 0 ] * size[ 1 ] + y * size[ 0 ] + x ];
    }

    T *ptr() {
        return data.ptr();
    }

    Tens3() {}
    explicit Tens3(unsigned n) { resize(n); }
    Tens3(unsigned n0,unsigned n1,unsigned n2) { resize(n0,n1,n2); }
    Tens3(unsigned n0,unsigned n1,unsigned n2,const T &val) { resize(n0,n1,n2); set(val); }
    template<class T2,int s2> Tens3(unsigned n0,unsigned n1,unsigned n2,const Vec<T2,s2> &v):data(v) { resize(n0,n1,n2); }

    void resize(unsigned n) { resize( n, n, n ); }
    void resize(unsigned n0,unsigned n1,unsigned n2) { data.resize(n0*n1*n2); this->size = Vec<int>(n0,n1,n2); }
    template<class TV>
    void resize( const TV &size ) {
        data.resize( size[ 0 ] * size[ 1 ] * size[ 2 ] );
        this->size = size;
    }

    template<class T2> void set(const T2 &v) { data.set(v); }

    //template<class T2> Tens3(const T2 &val) { data = val; }

    //template<class T2> Tens3 &operator=(const T2 &val) { data.set((T)val); return *this; }
    Tens3 &operator=(int val) { data.set(val); return *this; }

    template<class T2> Tens3 &operator+=(const T2 &val) { data += val; return *this; }
    template<class T2> Tens3 &operator-=(const T2 &val) { data -= val; return *this; }
    template<class T2> Tens3 &operator*=(const T2 &val) { data *= val; return *this; }
    template<class T2> Tens3 &operator/=(const T2 &val) { data /= val; return *this; }

    template<class T2> Tens3 &operator+=(const Tens3<T2> &val) { data += val.data; return *this; }
    template<class T2> Tens3 &operator-=(const Tens3<T2> &val) { data -= val.data; return *this; }
    template<class T2> Tens3 &operator*=(const Tens3<T2> &val) { *this = *this * val; return *this; }
    template<class T2> Tens3 &operator/=(const Tens3<T2> &val) { *this = *this / val; return *this; }

    void free() { resize(0); data.free(); } /// free data

    Vec<int> size;
    Vec<T>   data;
};

template<class T>
Tens3<T> operator+(const Tens3<T> &t1,const Tens3<T> &t2) {
    Tens3<T> res;
    res.data = t1.data + t2.data;
    return res;
}

template<class T>
Tens3<T> operator-(const Tens3<T> &t1,const Tens3<T> &t2) {
    Tens3<T> res;
    res.data = t1.data - t2.data;
    return res;
}

}

#endif // TENS3_H
