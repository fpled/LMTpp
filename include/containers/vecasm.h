//
// C++ Interface: vecasm
//
// Description: 
//
//
// Author: Hugo LECLERC <leclerc@lmt.ens-cachan.fr>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMT_vecasm_HEADER
#define LMT_vecasm_HEADER

namespace LMT {

typedef AlternativeOnType<sizeof(long long)==sizeof(int *),int,long long>::T SizeType;

/*!
    Objectif: 
        calculer le produit scalaire 4 par 4 
*/
template<class T> inline T dot_aligned(const T *a,const T *b,unsigned size) {
    T res1 = T(0);
    switch ( size & 3 ) {
        case 3: res1  = a[ size-3 ] * b[ size-3 ];
        case 2: res1 += a[ size-2 ] * b[ size-2 ];
        case 1: res1 += a[ size-1 ] * b[ size-1 ];
    }
    size -= size & 3;
    if ( size==0 ) return res1;
    T res2 = T(0), res3 = T(0), res4 = T(0);
    for(unsigned i=0;i<size;i+=4) {
        res1 += a[i+0] * b[i+0];
        res2 += a[i+1] * b[i+1];
        res3 += a[i+2] * b[i+2];
        res4 += a[i+3] * b[i+3];
    }
    return res1+res2+res3+res4;
}

/*! sum a[i]*b[i] assuming that it is possible to find a+x and b+x that are aligned adresses */
template<class T> inline T dot_aligned_with_offset(const T *a,const T *b,unsigned size) {
    T res = (T)0;
    while ( size && ( (SizeType)a & (SimdSize<T>::res*sizeof(T)-1) ) ) {
        res += a[0] * b[0];
        ++a; ++b; --size;
    }
    return res + dot_aligned( a, b, size );
}

/*! sum_i a_i^2 */
template<class T> inline T norm_2_p2(const T *a,unsigned size) {
    return dot_aligned_with_offset(a,a,size);
}

/// produit scalaire hermitien pour les vecteurs de complexes
template<class T,class size_type>
inline T dot_conj(const T *a,const T *b,size_type nb_val) {
  T r1 = 0;
  T r2 = 0;
  T r3 = 0;
  T r4 = 0;
  while (nb_val&3) {
    --nb_val;
    r1 += a[nb_val] * LMT::conj(b[nb_val]);
  }
  register size_type dep = 4;
  while (nb_val) {
    nb_val -= dep;
    r1 += a[nb_val+0] * LMT::conj(b[nb_val+0]);
    r2 += a[nb_val+1] * LMT::conj(b[nb_val+1]);
    r3 += a[nb_val+2] * LMT::conj(b[nb_val+2]);
    r4 += a[nb_val+3] * LMT::conj(b[nb_val+3]);
  }
  return r1+r2+r3+r4;
}
/** sum_i a_i^2 */
template<class T> inline T norm_2_p2_conj(const T *a,unsigned size) {
    return dot_conj(a,a,size);
}

///
template<class T> inline T dot_aligned(const T *a,const T *b,const T *c,unsigned size) {
    T res1 = T(0);
    switch ( size & 3 ) {
        case 3: res1  = a[ size-3 ] * b[ size-3 ] * c[ size-3 ];
        case 2: res1 += a[ size-2 ] * b[ size-2 ] * c[ size-2 ];
        case 1: res1 += a[ size-1 ] * b[ size-1 ] * c[ size-1 ];
    }
    size -= size & 3;
    if ( size==0 ) return res1;
    T res2 = T(0), res3 = T(0), res4 = T(0);
    for(unsigned i=0;i<size;i+=4) {
        res1 += a[i+0] * b[i+0] * c[i+0];
        res2 += a[i+1] * b[i+1] * c[i+1];
        res3 += a[i+2] * b[i+2] * c[i+2];
        res4 += a[i+3] * b[i+3] * c[i+3];
    }
    return res1+res2+res3+res4;
}

/** sum a[i]*b[i] assuming that it is possible to find a+x and b+x that are aligned adresses */
template<class T> inline T dot_aligned_with_offset(const T *a,const T *b, const T *c, unsigned size) {
    T res = (T)0;
    while ( size && ( (SizeType)a & (SimdSize<T>::res*sizeof(T)-1) ) ) {
        res += a[0] * b[0] * c[0];
        ++a; ++b; ++c; --size;
    }
    return res + dot_aligned( a, b, c, size );
}

#ifdef __SSE__
    /*! sum a[i]*b[i] assuming that a and b are aligned adresses */
    inline float dot_aligned(const float *a,const float *b,unsigned size) {
        float res = .0f;
        switch ( size & 3 ) {
            case 3: res  = a[ size-3 ] * b[ size-3 ];
            case 2: res += a[ size-2 ] * b[ size-2 ];
            case 1: res += a[ size-1 ] * b[ size-1 ];
            default: break;
        }
        size /= 4;
        if ( size==0 ) return res;

        __v4sf resv1 = _mm_set1_ps(.0f);
        switch ( size & 3 ) {
            case 3: resv1  = _mm_add_ps(resv1, _mm_mul_ps( *(const __v4sf *)(a+4*size-12), *(const __v4sf *)(b+4*size-12) ) );
            case 2: resv1  = _mm_add_ps(resv1, _mm_mul_ps( *(const __v4sf *)(a+4*size-8 ), *(const __v4sf *)(b+4*size-8 ) ) );
            case 1: resv1  = _mm_add_ps(resv1, _mm_mul_ps( *(const __v4sf *)(a+4*size-4 ), *(const __v4sf *)(b+4*size-4 ) ) );
            default: break;
        }
        size /= 4;
        if ( size==0 ) return res + ((const float *)&resv1)[0] + ((const float *)&resv1)[1] + ((const float *)&resv1)[2] + ((const float *)&resv1)[3];

        __v4sf resv2 = _mm_set1_ps(.0f);
        __v4sf resv3 = _mm_set1_ps(.0f);
        __v4sf resv4 = _mm_set1_ps(.0f);
        size *= 16;
        for(unsigned i=0;i<size;i+=16) {
            resv1 = _mm_add_ps(resv1, _mm_mul_ps( *(const __v4sf *)(a+i+0 ), *(const __v4sf *)(b+i+0 ) ) );
            resv2 = _mm_add_ps(resv2, _mm_mul_ps( *(const __v4sf *)(a+i+4 ), *(const __v4sf *)(b+i+4 ) ) );
            resv3 = _mm_add_ps(resv3, _mm_mul_ps( *(const __v4sf *)(a+i+8 ), *(const __v4sf *)(b+i+8 ) ) );
            resv4 = _mm_add_ps(resv4, _mm_mul_ps( *(const __v4sf *)(a+i+12), *(const __v4sf *)(b+i+12) ) );
        }
        resv1 = _mm_add_ps(resv1, resv2 );
        resv1 = _mm_add_ps(resv1, resv3 );
        resv1 = _mm_add_ps(resv1, resv4 );
        return res + ((const float *)&resv1)[0] + ((const float *)&resv1)[1] + ((const float *)&resv1)[2] + ((const float *)&resv1)[3];
    }
    /*! Specialization (because of disappointment about compilers) \relates Vec */
    inline float dot(const Vec<float,-1> &p1,const Vec<float,-1> &p2) { VECASSERTSAMESIZE(p1,p2); return dot_aligned(p1.ptr(),p2.ptr(),p1.size()); }
    /*! Specialization (because of disappointment about compilers) \relates Vec */
    inline float dot(const Vec<Ref<float>,-1> &p1,const Vec<float,-1> &p2) { VECASSERTSAMESIZE(p1,p2); return dot_aligned(p1.ptr(),p2.ptr(),p1.size()); }
#endif
#ifdef __SSE2__
    /*! sum a[i]*b[i] assuming that a and b are aligned adresses */
    inline double dot_aligned(const double *a,const double *b,int size) {
        __v2df resv1 = _mm_set1_pd(.0), resv2 = _mm_set1_pd(.0), resv3 = resv1, resv4 = resv1;
        int i;
        for(i=0;i<size-2*4+1;i += 2*4) {
            resv1 = _mm_add_pd(resv1, _mm_mul_pd( *(const __v2df *)(a+i+0), *(const __v2df *)(b+i+0) ) );
            resv2 = _mm_add_pd(resv2, _mm_mul_pd( *(const __v2df *)(a+i+2), *(const __v2df *)(b+i+2) ) );
            resv3 = _mm_add_pd(resv3, _mm_mul_pd( *(const __v2df *)(a+i+4), *(const __v2df *)(b+i+4) ) );
            resv4 = _mm_add_pd(resv4, _mm_mul_pd( *(const __v2df *)(a+i+6), *(const __v2df *)(b+i+6) ) );
        }
        for(;i<size-2+1;i += 2)
            resv1 = _mm_add_pd(resv1, _mm_mul_pd( *(const __v2df *)(a+i+0), *(const __v2df *)(b+i+0) ) );
        resv1 = _mm_add_pd(resv1, resv2 );
        resv1 = _mm_add_pd(resv1, resv3 );
        resv1 = _mm_add_pd(resv1, resv4 );
        double res = ((const double *)&resv1)[0] + ((const double *)&resv1)[1];
        if ( size & 1 )
            return a[ size-1 ] * b[ size-1 ] + res;
        return res;
    }
    /*! Specialization (because of disappointment about compilers) \relates Vec */
    //inline double dot(const Vec<double,-1> &p1,const Vec<double,-1> &p2) { VECASSERTSAMESIZE(p1,p2); return dot_aligned(p1.ptr(),p2.ptr(),p1.size()); }
#endif

/// specialized for float, double, ...
template<class T>
inline void copy_vec(const T *from,T *to,unsigned size) {
    for(unsigned i=0;i<size;++i) to[i] = from[i];
}
/// assumes that from and to are aligned
template<class T>
inline void copy_vec_aligned(const T *from,T *to,unsigned size) {
    for(unsigned i=0;i<size;++i) to[i] = from[i];
}

template<class T>
inline void copy_vec_value(const T &from,T *to,unsigned size) {
    for(unsigned i=0;i<size;++i) to[i] = from;
}
template<class T>
inline void copy_vec_value_aligned(const T &from,T *to,unsigned size) {
    for(unsigned i=0;i<size;++i) to[i] = from;
}


}

#endif // LMT_vecasm_HEADER
