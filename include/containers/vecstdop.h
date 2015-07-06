#include "../io/lineoutput.h"
#include "veccst.h"

namespace LMT {

// -------------------------------------------------------------- INPUT/OUTPUT --------------------------------------------------------------

/*!
 \relates Vec
 */
template<class TT,int stat_dim>
std::istream &operator>>( std::istream &s, Vec<TT,stat_dim> &v ) {
    apply(v, GetFromStream(), s );
    return s;
}
/*!
 \relates Vec
 */
template<class TT,int stat_dim>
std::istream &lineinput( std::istream &s, Vec<TT,stat_dim> &v ) {
    apply(v, LineInput(), s );
    return s;
}

/*!
 \relates Vec
 */
template<class T>
std::istream &operator>>( std::istream &s, Vec<T,-1> &v ) {
    T val;
    while( s >> val ) v.push_back( val );
    s.clear();
    return s;
}
/*!
 \relates Vec
 */
template<class T>
std::istream &line_input( std::istream &s, Vec<T,-1> &v ) {
    T val;
    while( line_input( s, val ) ) v.push_back( val );
    s.clear();
    return s;
}

/*!
 \relates Vec
 */
template<class T,int stat_dim>
std::ostream &operator<<( std::ostream &s, const Vec<T,stat_dim> &v ) {
    apply( v, Display(), s , " " );
    return s;
}
/*!
 \relates Vec
 */
template<class T,int stat_dim>
std::ostream &line_output( std::ostream &s, const Vec<T,stat_dim> &v ) {
    apply( v, LineOutput(), s , " " );
    return s;
}


// -------------------------------------------------------------- UTILITARY OP -----------------------------------------------------------------
struct PushBack {
    template<class T,class TV> void operator()( const T &val, TV &vec ) const {
        vec.push_back( val );
    }
};

struct PushBackPtr {
    template<class T,class TV> void operator()( T &val, TV &vec ) const {
        vec.push_back( &val );
    }
};
struct PushBackConstPtr {
    template<class T,class TV> void operator()( const T &val, TV &vec ) const {
        vec.push_back( &val );
    }
};

// -------------------------------------------------------------- REDUCTION OP -----------------------------------------------------------------
/*!
  \relates Vec
*/
template<class Op,class T,unsigned ne,bool ssi> struct VecReductionOp {
    typedef SimdVec<T,ne> COT;
    VecReductionOp() {
        op->init_reduction(res1);
        op->init_reduction(res2);
        op->init_reduction(res3);
        op->init_reduction(res4);
        op->init_reduction(res);
    }
    inline void operator()(COT val1,COT val2,COT val3,COT val4) {
        res1 = (*op)(res1,val1); res2 = (*op)(res2,val2); res3 = (*op)(res3,val3); res4 = (*op)(res4,val4);
    }
    inline void operator()(const COT &val1) { res1 = (*op)(res1,val1); }
    inline void operator()(const T &val1) { res = (*op)(res,val1); }
    T get_res() {
        COT rs = (*op)( (*op)(res1, res2), (*op)(res3, res4) );
        for(unsigned i=0;i<COT::nb_elem;++i)
            res = (*op)( res, rs[i] );
        return res;
    }
    COT res1, res2, res3, res4;
    T res;
    const Op *op;
};

template<class Op,class T> struct VecReductionOp<Op,T,1,false> {
    VecReductionOp() { op->init_reduction(res1); op->init_reduction(res2); op->init_reduction(res3); op->init_reduction(res4); }
    inline void operator()(const T &val1) { res1 = (*op)(res1,val1); }
    inline void operator()(const T &val1,const T &val2,const T &val3,const T &val4) { res1 = (*op)(res1,val1); res2 = (*op)(res2,val2); res3 = (*op)(res3,val3); res4 = (*op)(res4,val4); }
    T get_res() { return (*op)( (*op)(res1, res2), (*op)(res3, res4) ); }
    T res1, res2, res3, res4;
    const Op *op;
};

/*!
  \relates Vec
  \keyword Vecteur
  \todo apply_nz if reduction of type...
*/
template<class TT,int s,class Op>
inline typename TypeReduction<Op,Vec<TT,s> >::T reduction_w_wo_simd(const Vec<TT,s> &v,const Op &op,const Number<0> &nn) {
    typedef typename TypeReduction<Op,Vec<TT,s> >::T T;
    VecReductionOp<Op,T,1,false> vo; vo.op = &op;
    apply( v, vo );
    //v.apply_nz( vo );
    return vo.get_res();
}
/*!
  \relates Vec
*/
template<class TT,int s,class Op>
inline typename TypeReduction<Op,Vec<TT,s> >::T reduction_w_wo_simd(const Vec<TT,s> &v,const Op &op,const Number<1> &nn) {
    typedef typename TypeReduction<Op,Vec<TT,s> >::T T;
    VecReductionOp<Op,T,SimdSizeForVec<Vec<TT,s> >::res,true> vo; vo.op = &op;
    apply_simd( v, vo );
    //v.apply_nz( vo );
    return vo.get_res();
}

/*! Reduction of all elements
  \relates Vec
  \keyword Vecteur
  typename Op::template ReturnType<typename Vec<TT,s>::T,typename Vec<TT,s>::T>::T
*/
template<class TT,int s,class Op>
inline typename TypeReduction<Op,Vec<TT,s> >::T reduction(const Vec<TT,s> &v,const Op &op) {
    return reduction_w_wo_simd(v,op,Number< (s==-1 || s>16) && (SimdSize<typename TypeReduction<Op,Vec<TT,s> >::T>::res>1) >());
}

/*! Sum of all elements
  \relates Vec
  \keyword Vecteur
*/
template<class TT,int s>
inline typename TypeReduction<Plus,Vec<TT,s> >::T sum(const Vec<TT,s> &v) {
    return reduction<TT,s,Plus>(v,Plus());
}

/*! Product of all elements
  \relates Vec
  \keyword Vecteur
*/
template<class TT,int s>
inline typename TypeReduction<Multiplies,Vec<TT,s> >::T product(const Vec<TT,s> &v) {
    return reduction<TT,s,Multiplies>(v,Multiplies());
}

/*! Mean of all elements
  \relates Vec
  \keyword Vecteur
*/
template<class TT,int s>
inline typename TypeReduction<Plus,Vec<TT,s> >::T mean(const Vec<TT,s> &v) {
    return sum( v ) / v.size();
}

/*! root mean square
  \relates Vec
  \keyword Vecteur
*/
template<class TT,int s>
inline typename TypeReduction<Plus,Vec<TT,s> >::T rms( const Vec<TT,s> &v ) {
    return sqrt( sum( v * v ) / v.size() );
}

/*!
  \keyword Vecteur
*/
template<class TT,int s>
inline typename TypeReduction<Plus,Vec<TT,s> >::T variance( const Vec<TT,s> &v ) {
    return mean( generate( v, cst_vec( mean( v ), v.size() ), ( _1 - _2 ) * ( _1 - _2 ) ) );
}

/*!
  \keyword Vecteur
*/
template<class TT,int s>
inline typename TypeReduction<Plus,Vec<TT,s> >::T standard_deviation(const Vec<TT,s> &v) {
    return sqrt( variance( v ) );
}


/*! Scalar product. Generic form
  \relates Vec
  \keyword Vecteur
 */
template<class T1,int s1,class IO1,class T2,int s2,class IO2>
inline typename TypePromote<Multiplies,typename Vec<T1,s1,IO1>::template SubType<0>::T,typename Vec<T2,s2,IO2>::template SubType<0>::T>::T
dot(const Vec<T1,s1,IO1> &p1,const Vec<T2,s2,IO2> &p2) {
//     DEBUGASSERT(( Vec<T2,s1,IO1>::nb_sub_type == 1 ));
//     DEBUGASSERT(( Vec<T2,s2,IO2>::nb_sub_type == 1 ));
    return sum(p1 * p2);
}
/*! Scalar product of two tiny vectors with different sizes. Uses min(d1,d2)
  \relates Vec
  \keyword Vecteur
 */
template<class T1,int s1,class IO1,class T2,int s2,class IO2>
inline typename TypePromote<Multiplies,typename Vec<T1,s1,IO1>::template SubType<0>::T,typename Vec<T2,s2,IO2>::template SubType<0>::T>::T
dot_md(const Vec<T1,s1,IO1> &p1,const Vec<T2,s2,IO2> &p2) {
    typename TypePromote<Multiplies,typename Vec<T1,s1,IO1>::template SubType<0>::T,typename Vec<T2,s2,IO2>::template SubType<0>::T>::T res = typename TypePromote<Multiplies,typename Vec<T1,s1,IO1>::template SubType<0>::T,typename Vec<T2,s2,IO2>::template SubType<0>::T>::T(0);
    for(unsigned i=0;i<min(p1.size(),p2.size());++i)
        res += p1[i]*p2[i];
    return res;
}


/*! min_[x in c} x
  \relates Vec
  \keyword Vecteur
 */
template<class T,int s>
inline typename TypeReduction<Min,Vec<T,s> >::T min(const Vec<T,s> &c) {
    DEBUGASSERT( c.size() );
    return reduction(c,Min());
}
/*! max_[x in c} x
 \relates Vec
  \keyword Vecteur
 */
template<class T,int s>
inline typename TypeReduction<Max,Vec<T,s> >::T max(const Vec<T,s> &c) {
    DEBUGASSERT( c.size() );
    return reduction(c,Max());
}

/** index_of_max
 \relates Vec
  \keyword Vecteur
 */
template<class TV,class Op>
inline unsigned index_of_max( const TV &c, const Op &op ) {
    DEBUGASSERT( c.size() );
    unsigned index = 0;
    for(unsigned i=1;i<c.size();++i) 
        if( op( c[index], c[i] ) )
            index = i;
    return index;
}

/** index_of_max
 \relates Vec
 */
template<class TV>
inline unsigned index_of_max( const TV &c ) {
    return index_of_max( c, Less() );
}

/** index_of_min
 \relates Vec
*/
template<class TV,class Op>
inline unsigned index_of_min( const TV &c, const Op &op ) {
    DEBUGASSERT( c.size() );
    unsigned index = 0;
    for(unsigned i=1;i<c.size();++i)
        if( op( c[i], c[index] ) )
            index = i;
    return index;
}

/** index_of_min
 \relates Vec
  \keyword Vecteur
 */
template<class TV>
inline unsigned index_of_min( const TV &c ) {
    return index_of_min( c, Less() );
}

/*! true if all v verify v==true
 \relates Vec
  \keyword Vecteur
 */
template<class T,int s,class TO>
inline bool all(const Vec<T,s,TO> &c) {
    return ! find( c, cast_bool(_1) == 0 );
}
/*! true if at least one v verify v==true
 \relates Vec
  \keyword Vecteur
 */
template<class T,int s>
inline bool at_least_one(const Vec<T,s> &c) {
    return find( c, _1 );
}


// ----------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __ICC
inline Vec<double,3> vect_prod(const Vec<double,3> &p1,const Vec<double,3> &p2) {
    return Vec<double,3>(
        p1[1]*p2[2] - p1[2]*p2[1],
        p1[2]*p2[0] - p1[0]*p2[2],
        p1[0]*p2[1] - p1[1]*p2[0]
    );
}

inline double vect_prod(const Vec<double,2> &p1,const Vec<double,2> &p2) {
    return p1[0]*p2[1] - p1[1]*p2[0];
}
#else
/*! vectorial product 3D case (return a vector).
  \keyword Vecteur
 \relates Vec
 */
template<class T1,typename V1,class T2,typename V2>
inline Vec<typename TypePromote<Multiplies,typename Vec<T1,3,V1>::template SubType<0>::T,typename Vec<T2,3,V2>::template SubType<0>::T>::T,3> vect_prod(const Vec<T1,3,V1> &p1,const Vec<T2,3,V2> &p2) {
    DEBUGASSERT(( Vec<T1,3>::nb_sub_type == 1 ));
    DEBUGASSERT(( Vec<T2,3>::nb_sub_type == 1 ));
    return Vec<typename TypePromote<Multiplies,typename Vec<T1,3,V1>::template SubType<0>::T,typename Vec<T2,3,V2>::template SubType<0>::T>::T,3>(
        p1[1]*p2[2] - p1[2]*p2[1],
        p1[2]*p2[0] - p1[0]*p2[2],
        p1[0]*p2[1] - p1[1]*p2[0]
    );
}

/*! vectorial product 2D case (return a scalar). \warning in the 3D case, vect_prod return a vector
  \keyword Vecteur
 \relates Vec
 */
template<class T1,typename V1,class T2,typename V2>
inline typename TypePromote<Multiplies,typename Vec<T1,2,V1>::template SubType<0>::T,typename Vec<T2,2,V2>::template SubType<0>::T>::T vect_prod(const Vec<T1,2,V1> &p1,const Vec<T2,2,V2> &p2) {
    return p1[0]*p2[1] - p1[1]*p2[0];
}
#endif

/*! 
    \latex
        $ \sqrt{ \sum_i c_i^2 } $
    \relates Vec
  \keyword Vecteur
 */
template<class T,int s>
inline typename FloatType<typename TypeReduction<Multiplies,Vec<T,s> >::T>::T length(const Vec<T,s> &c) {
    return sqrt(dot(c,c));
}

/*! 
    \latex 
        $ \sqrt{ \sum_i (c_i-d_i)^2 } $
    \relates Vec
  \keyword Vecteur
 */
template<class T,int s>
inline typename FloatType<typename TypeReduction<Multiplies,Vec<T,s> >::T>::T distance(const Vec<T,s> &c, const Vec<T,s> &d) {
    Vec<T,s> tmp = c - d;
    return length( tmp );
}

/*!    
    \latex 
        $ \sum_i c_i^2 $
    \relates Vec
  \keyword Vecteur
 */
template<class T,int s>
inline typename TypeReduction<Multiplies,Vec<T,s> >::T norm_2_p2(const Vec<T,s> &c) {
    return dot(c,c);
}

/*!
    \latex 
        $ \sqrt{ \sum_i c_i^2 } $
    \relates Vec
 */
template<class T,int s>
inline typename FloatType<typename TypeReduction<Multiplies,Vec<T,s> >::T>::T norm_2(const Vec<T,s> &c) {
    return sqrt(norm_2_p2(c));
}

/*! 
    \latex 
        $ \sqrt{ \sum_i c_i^2 + ad } $
    \relates Vec
  \keyword Vecteur
 */
template<class T,int s>
inline typename FloatType<typename TypeReduction<Multiplies,Vec<T,s> >::T>::T norm_2( const Vec<T,s> &c, T ad ) {
    return sqrt( norm_2_p2(c) + ad );
}

/*! 
        \relates Vec
  \keyword Vecteur
 */
template<class T,int s>
inline typename TypePromote<Abs,typename TypeReduction<Multiplies,Vec<T,s> >::T>::T norm_inf(const Vec<T,s> &c) {
    return max(abs(c));
}

/*! 
        \relates Vec
  \keyword Vecteur
 */
template<class T,int s>
inline typename TypePromote<Abs,typename TypeReduction<Multiplies,Vec<T,s> >::T>::T norm_1(const Vec<T,s> &c) {
    return sum(abs(c));
}

/*!
*/
struct LengthOp {
    template<class P1> struct ReturnType { typedef typename TypeReduction<Multiplies,P1>::T T; };
    template<class P1> typename ReturnType<P1>::T operator()(const P1 &p1) const { return length(p1); }
};
/*! functor to get abs(dot(vector/length(vector),val)) + add_if_0 * length(vector)==0. Can be used with min_element, as example.
 */
template<int add_if_0> struct AbsDotNormalizedOp {
    template<class P,class V> struct ReturnType {
        typedef typename TypePromote<Multiplies,typename P::template SubType<0>::T,typename V::template SubType<0>::T>::T T;
    };
    template<class P,class V> typename ReturnType<P,V>::T operator()(const P &p,const V &v) const {
        typename P::template SubType<0>::T l = length(p);
        return abs(dot(p/(l+(l==0)),v)) + add_if_0 * (l==0);
    }
};
/*! functor to get distance between two points. Can be used with max_element, as example. */
struct DistBetweenOp {
    template<class P1,class P2,class T3=void,class T4=void> struct ReturnType { typedef typename TypePromote<Plus,typename P1::template SubType<0>::T,typename P2::template SubType<0>::T>::T T; };
    template<class P1,class P2> typename TypePromote<Plus,typename P1::template SubType<0>::T,typename P2::template SubType<0>::T>::T operator()(const P1 &p1,const P2 &p2) const {
        return length(p2-p1);
    }
};

/*! functor to get distance between two points. Can be used with max_element, as example. */
// template<class TD> struct DistBetweenOpInfEq {
//     template<class TV> struct ReturnType { typedef bool T; };
//     template<class P1,class P2> typename bool operator()(const P1 &p1,const P2 &p2) const {
//         return length(p2-p1)<;
//     }
//     TD dist;
// };

/*! */
template<class Pvec,class TD> struct DistBetweenVecInfEq {
    template<class TV> struct ReturnType { typedef bool T; };
    template<class P1> bool operator()(const P1 &p1) const {
        return length(p2-p1)<dist;
    }
    Pvec p2;
    TD dist;
};

template<class Pvec,class TD> DistBetweenVecInfEq<Pvec,TD> dist_between_vec_inf_eq( const Pvec &vec, const TD &d ) {
    DistBetweenVecInfEq<Pvec,TD> res;
    res.p2 = vec;
    res.dist = d;
    return res;
}

/*! 
    \latex 
        $ \sqrt{ \sum_i c_i^2 + ad } $
    \relates Vec
  \keyword Vecteur
 */
template<class T,int s>
inline typename TypeReduction<Multiplies,Vec<T,s> >::T length(const Vec<T,s> &c,T ad ) {
    return sqrt(dot(c,c)+ad);
}

/*!
 \relates Vec
  \keyword Vecteur
 */
template<class T,int s>
inline Vec<typename TypeReduction<Multiplies,Vec<T,s> >::T,s> normalized(const Vec<T,s> &c) {
    return c / ( length(c) + (length(c)==0));
}

/*! Only for 2D vec. get p rotated by 
    \latex
        $ \frac{\Pi}{2} $
Rotaion de 90 ° dans le sens direct.
 \relates Vec
  \keyword Vecteur
 */
template<class T>
inline Vec<typename Vec<T,2>::template SubType<0>::T,2> ortho(const Vec<T,2> &p) {
  return Vec<T,2>( -p[1], p[0] );
}

/*! find a vector orthonormal to p. In 2D, it's trivial but in 3D, we start from random vectors
 \relates Vec
  \keyword Vecteur
 */
template<class T>
inline Vec<typename Vec<T,2>::template SubType<0>::T,2> find_orthonormal(const Vec<T,2> &p) { return ortho(p) / length(p); }

/*! find a vector orthonormal to p. In 2D, it's trivial but in 3D, we start from random vectors
 \relates Vec
  \keyword Vecteur
 */
template<class TO,int s>
inline Vec<typename Vec<TO,s>::template SubType<0>::T,s> find_orthonormal(Vec<TO,s> p) {
    typedef typename Vec<TO,s>::template SubType<0>::T T;
    if ( length(p)==0.0 ) return T(0);
    p /= length(p);
    for(unsigned i=0;i<p.size();++i) {
        if ( abs(p[i]) > 1e-6 ) {
            Vec<T,s> res = dirac_vec(1.0,(i+1)%p.size(),p.size());
            res -= dot(res,p) * p;
            return res / length(res);
        }
    }
    while ( true ) {
        Vec<T,s> res = Vec<T,s>::random();
        if ( abs(dot(res,p)) > 1e-3 ) {
            res -= dot(res,p) * p;
            return res / length(res);
        }
    }
    return T(0);
}

/*!
 \relates Vec
  \keyword Vecteur
 */
template<class T1,class T2,class T3,int s>
Vec<typename Vec<T1,s>::template SubType<0>::T,s> getCenterOfCircumCircle(const Vec<T1,s> &P0,const Vec<T2,s> &P1,const Vec<T3,s> &P2) {
    typedef typename Vec<T1,s>::template SubType<0>::T T;
    Vec<T,s> P01(P1-P0), P02(P2-P0);
    T x1 = length(P01);
    P01 /= x1;
    T x2 = dot( P02, P01 );
    P02 -= x2 * P01;

    T y2 = length( P02 );
    P02 /= y2;

    T C0 = x1 * 0.5;
    T C1 = ( x2*x2+y2*y2-x2*x1 ) / ( 2.0 * y2 );

    return P0 + C0*P01 + C1*P02;
}
/*!
 \relates Vec
 */
template<class T1,class T2,class T3,int s>
Vec<typename Vec<T1,s>::template SubType<0>::T,s> getCenterOfInCircle(const Vec<T1,s> &P0,const Vec<T2,s> &P1,const Vec<T3,s> &P2) {
    typedef typename Vec<T1,s>::template SubType<0>::T T;
    Vec<T,s> P01(P1-P0), P02(P2-P0), P12(P2-P1);
    T x1 = length(P01);
    T x2 = length(P02);
    T x3 = length(P12);

    T C01 = x2 / ( x1+x2+x3 );
    T C02 = x1 / ( x1+x2+x3 );

    return P0 + C01*P01 + C02*P02;
}
/*!
 \relates Vec
 */
template<class T1,class T2,class T3,class T4,int s>
Vec<typename Vec<T1,s>::template SubType<0>::T,s> getCenterOfCircumSphere(const Vec<T1,s> &P0,const Vec<T2,s> &P1,const Vec<T3,s> &P2,const Vec<T4,s> &P3) {
    typedef typename Vec<T1,s>::template SubType<0>::T T;
    Vec<T,s> C0 = getCenterOfCircumCircle(P0,P1,P2);
    Vec<T,s> C1 = getCenterOfCircumCircle(P1,P2,P3);
    Vec<T,s> N0 = vect_prod(P1-P0,P2-P0);
    Vec<T,s> N1 = vect_prod(P2-P1,P3-P1);
    Vec<T,s> den = vect_prod(N0,N1);
    return C0 + N0 * dot(vect_prod(N1,C0-C1),den) / dot(den,den);
}
/*!
 \relates Vec
 */
template<class T1,class T2,class T3,class T4,int s>
Vec<typename Vec<T1,s>::template SubType<0>::T,s> getCenterOfInSphere(const Vec<T1,s> &P0,const Vec<T2,s> &P1,const Vec<T3,s> &P2,const Vec<T4,s> &P3) {
    typedef typename Vec<T1,s>::template SubType<0>::T T;
    Vec<T,s> P01(P1-P0), P02(P2-P0), P03(P3-P0), P12(P2-P1), P13(P3-P1);

    T x1 = length(P01);
    T x2 = length(P02);
    T x12 = length(P12);

    Vec<T,s> P01_N = P01 / x1;
    Vec<T,s> P02_N = P02 / x2;
    Vec<T,s> P12_N = P12 / x12;

    Vec<T,s> H1 = P02 - dot( P02, P01_N ) * P01_N;
    T y1 = length( H1 );
    T S1 = x1 * y1 / 2;

    Vec<T,s> H2 = P03 - dot( P03, P01_N ) * P01_N;
    T y2 = length( H2 );
    T S2 = x1 * y2 / 2;

    Vec<T,s> H3 = P03 - dot( P03, P02_N ) * P02_N;
    T y3 = length( H3 );
    T S3 = x2 * y3 / 2;

    Vec<T,s> H4 = P13 - dot( P13, P12_N ) * P12_N;
    T y4 = length( H4 );
    T S4 = x12 * y4 / 2;

    T C01 = S3 / ( S1 + S2 + S3 + S4 );
    T C02 = S2 / ( S1 + S2 + S3 + S4 );
    T C03 = S1 / ( S1 + S2 + S3 + S4 );
    return P0 + C01 * P01 + C02 * P02 + C03 * P03;
}


}

