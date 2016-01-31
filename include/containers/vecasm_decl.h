#ifndef VECASM_DECL_H
#define VECASM_DECL_H

namespace LMT {

/*!
    Objectif:
        calculer le produit scalaire 4 par 4
*/
template<class T> inline T dot_aligned(const T *a,const T *b,unsigned size);

/*! sum a[i]*b[i] assuming that it is possible to find a+x and b+x that are aligned adresses */
template<class T> inline T dot_aligned_with_offset(const T *a,const T *b,unsigned size);

/*! sum_i a_i^2 */
template<class T> inline T norm_2_p2(const T *a,unsigned size);

/// produit scalaire hermitien pour les vecteurs de complexes
template<class T,class size_type>
inline T dot_conj(const T *a,const T *b,size_type nb_val);

/** sum_i a_i^2 */
template<class T> inline T norm_2_p2_conj(const T *a,unsigned size);

///
template<class T> inline T dot_aligned(const T *a,const T *b,const T *c,unsigned size);

/** sum a[i]*b[i] assuming that it is possible to find a+x and b+x that are aligned adresses */
template<class T> inline T dot_aligned_with_offset(const T *a,const T *b, const T *c, unsigned size);

#ifdef __SSE__
    /*! sum a[i]*b[i] assuming that a and b are aligned adresses */
    inline float dot_aligned(const float *a,const float *b,unsigned size);
    /*! Specialization (because of disappointment about compilers) \relates Vec */
    //inline float dot(const Vec<float,-1> &p1,const Vec<float,-1> &p2);
    /*! Specialization (because of disappointment about compilers) \relates Vec */
    // inline float dot(const Vec<Ref<float>,-1> &p1,const Vec<float,-1> &p2);
#endif
#ifdef __SSE2__
    /*! sum a[i]*b[i] assuming that a and b are aligned adresses */
    inline double dot_aligned(const double *a,const double *b,int size);
    /*! Specialization (because of disappointment about compilers) \relates Vec */
    ////inline double dot(const Vec<double,-1> &p1,const Vec<double,-1> &p2) { VECASSERTSAMESIZE(p1,p2); return dot_aligned(p1.ptr(),p2.ptr(),p1.size()); }
#endif

/// specialized for float, double, ...
template<class T>
inline void copy_vec(const T *from,T *to,unsigned size);

/// assumes that from and to are aligned
template<class T>
inline void copy_vec_aligned(const T *from,T *to,unsigned size);

template<class T>
inline void copy_vec_value(const T &from,T *to,unsigned size);

template<class T>
inline void copy_vec_value_aligned(const T &from,T *to,unsigned size);


}

#endif // VECASM_DECL_H
