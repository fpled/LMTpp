# -*- coding: utf-8 -*-
import string
from vecgenhelp import *

print """
#include "vecasm_decl.h"
#include "matlabhelp.h"

namespace LMT {
    template<class T,int s> struct VecSupportSIMD<Vec<T,s,void> > { static const unsigned res = true; };
}
"""
for static_size in [1,0]:
    size = ['s','static_size_'][static_size]
    constructors = ''
    if static_size:
        constructors  = '    Vec() {}\n'
        constructors += '    template<class T2> Vec(const T2 &v) { for(unsigned i=0;i<(unsigned)static_size;val[i++]=v); } /// copy val in all elements of *this\n'
        constructors += '    void get_data_from_ptr(const TT *v) { for(unsigned i=0;i<(unsigned)static_size;++i) val[i]=v[i]; } /// copy val in all elements of *this\n'
        for i in range(1,38):
            args = string.join( [ 'const TT &v%i'%k for k in range(i) ], ',')
            sets = string.join( [ 'val[%i]=v%i;'%(k,k) for k in range(i) ] , ' ')
            if i>1:
                constructors += '    Vec('+args+') { DEBUGASSERT(static_size_==%i); %s } /// static_size_ must == %i\n' % (i,sets,i)
            constructors += '    void assign_and_complete_with_last('+args+') { '+sets+' for(unsigned i='+str(i)+';i<size();++i) val[i]=v'+str(i-1)+'; }\n'
    else:
        constructors = '    Vec() { init(); }\n'
        for i in range(1,38):
            args = string.join( [ 'const TT &v%i'%k for k in range(i) ] ,',')
            sets = string.join( [ 'val[%i]=v%i;'%(k,k) for k in range(i) ] ,' ')
            constructors += '    explicit Vec('+args+') { init(); resize(%i); %s } /// declare a vector containing args\n' % (i,sets)

    print """
namespace LMT {

/**
    Full vector. Elements are contiguous.
    \warning during resize, pop_back, ... and so on, location of data in memory may be moved. Use SplittedVector if you are pointer stability.
*/
/*!
    \\brief vecteur a element contigus. Si la taille change , on preferera le type SplittedVector qui assure que le vecteur ne sera pas deplace en memoire apres trop d'ajouts.
    Full vector. Elements are contiguous.
    <strong> IMORTANT : during resize, pop_back, ... and so on, location of data in memory may be moved. Use SplittedVector if you are pointer stability. </strong>
    \\friend samir.amrouche@lmt.ens-cachan.fr
    \\friend hugo.leclerc@lmt.ens-cachan.fr
*/

template<class TT"""+',int static_size_'*static_size+""">
class Vec<TT,"""+['-1','static_size_'][static_size]+""",void> {
public:
    template<unsigned n,unsigned inner=0> struct SubType { typedef TT T; };
    template<unsigned inner> struct SubType<1,inner> { typedef void T; };
    static const unsigned nb_sub_type = 1;
    typedef int ST;

    static const int static_size = """+['-1','static_size_'][static_size]+""";
    static const bool fixed_size = """+['false','true'][static_size]+""";
    static const int sparsity_rate = 0;
    """+'typedef typename PreferredAllocator<TT>::T Allocator;'*(1-static_size)+"""

""" + constructors + """

    TT &front() { return val[0]; } /// first element
    const TT &front() const { return val[0]; } /// first element
    TT &back() { return val[size()-1]; } /// last element
    const TT &back() const { return val[size()-1]; } /// last element
    const TT *ptr() const { return val; }
    TT *ptr() { return val; }
    const TT *begin() const { return val; }
    TT *begin() { return val; }
    const TT *end() const { return val+size(); }
    TT *end() { return val+size(); }

    struct SetVal {
        template<class T2> void operator()(const T2 &v,unsigned i,TT *val) const { val[i] = (TT)v; }
        template<class T2> void operator()(const T2 &v1,const T2 &v2,const T2 &v3,const T2 &v4,unsigned i,TT *val) const { val[i+3] = (TT)v1; val[i+2] = (TT)v2; val[i+1] = (TT)v3; val[i+0] = (TT)v4; }
        template<class T2,unsigned ne> void operator()(const SimdVec<T2,ne> &v,unsigned i,TT *val) const { reinterpret_cast<SimdVec<TT,ne> &>(val[i]) = v; }
        template<class T2,unsigned ne> void operator()(const SimdVec<T2,ne> &v1,const SimdVec<T2,ne> &v2,const SimdVec<T2,ne> &v3,const SimdVec<T2,ne> &v4,unsigned i,TT *val) const {
            reinterpret_cast<SimdVec<TT,ne> &>(val[i+3*ne]) = v1;
            reinterpret_cast<SimdVec<TT,ne> &>(val[i+2*ne]) = v2;
            reinterpret_cast<SimdVec<TT,ne> &>(val[i+1*ne]) = v3;
            reinterpret_cast<SimdVec<TT,ne> &>(val[i+0*ne]) = v4;
        }
    };

    template<class T2,int s1,class IO1> Vec(const Vec<T2,s1,IO1> &vec) { """+'DEBUGASSERT(s1<0 or s1==static_size); DEBUGASSERT((unsigned)static_size==vec.size());'*static_size+'init(); resize(vec.size());'*(1-static_size)+"""
         if ( static_size>=0 ) {
             for(unsigned i=0;i<min((unsigned)static_size,vec.size());++i)
                 val[i] = (TT)vec[i];
         }
         else
            apply_simd_wi(vec,SetVal(),val);
    } /// copy constructor
    Vec(const Vec &vec) { init(); resize(vec.size()); for(unsigned i=0;i<size();++i) val[i] = vec[i]; } /// copy constructor
    template<class T2,int s1> Vec &operator=(const Vec<T2,s1> &vec) { """+'DEBUGASSERT(s1<0 or s1==static_size); DEBUGASSERT((unsigned)static_size==vec.size());'*static_size+'resize(vec.size());'*(1-static_size)+"""
        if ( static_size>=0 ) {
            for(unsigned i=0;i<min((unsigned)static_size,vec.size());++i)
                val[i] = (TT)vec[i];
        }
        else
            apply_simd_wi(vec,SetVal(),val);
        return *this;
    }
    Vec &operator=(const Vec &vec) { """+'DEBUGASSERT((unsigned)static_size==vec.size());'*static_size*0+'resize(vec.size());'*(1-static_size)+"""
        if ( static_size>=0 ) {
            for(unsigned i=0;i<min((unsigned)static_size,vec.size());++i)
                val[i] = (TT)vec[i];
        }
        else
            apply_simd_wi(vec,SetVal(),val);
        return *this;
    }
    Vec(std::istream &is) { init(); is >> *this; } /// get data from stream
    // Vec(const std::string &s) { init(); std::istringstream is(s); is >> *this; } /// get data from string

    template<class T2>
    void set( const T2 &v ) { for(unsigned i=0;i<size();++i) operator[]( i ) = v; }

    #ifdef MATLAB_MEX_FILE
        explicit Vec(const mxArray *variable) throw(std::runtime_error) {
            if ( mxIsEmpty(variable) ) {
                printf( "Error : LMT::Vec(mxArray *) need a Matrix\\n" );
                throw std::runtime_error("LMT::Vec(mxArray *) need a Matrix");
            }
            if ( ! mxArrayGoodtype<TT>::res(variable) ) {
                printf( "Error : constructor LMT::Vec<%s>(mxArray *) needs a %s Matrix\\n", TypeInformation<TT>::type().c_str(), TypeInformation<typename mxArrayGoodtype<TT>::needed>::type().c_str() );
                throw std::runtime_error("constructor LMT::Vec<"+TypeInformation<TT>::type()+">(mxArray *) needs a "+TypeInformation<typename mxArrayGoodtype<TT>::needed>::type()+" Matrix");
            }
            if ( fixed_size && static_size!=mxGetN(variable) ) {
                PRINTM( "Error : Vec<...,"+to_string<static_size>()+">(const mxArray *variable) must take a vector of size "+to_string<static_size>() );
                throw std::runtime_error("size error");
            }

            init();
            resize( mxGetN(variable) * mxGetM(variable) );

            double *pr;
            pr = mxGetPr(variable);
            for(unsigned i=0;i<size();++i)
                val[i] = (TT)pr[i];
            if ( mxIsComplex(variable) ) {
                pr = mxGetPi(variable);
                for(unsigned i=0;i<size();++i)
                    set_imag(val[i],pr[i]);
            }
        } /// matlab -> LMT::Vec
        Vec &operator=(const mxArray *variable) throw(std::runtime_error) {
            if ( mxIsEmpty(variable) ) {
                printf( "Error : LMT::Vec(mxArray *) need a Matrix\\n" );
                throw std::runtime_error("LMT::Vec(mxArray *) need a Matrix");
            }
            if ( ! mxArrayGoodtype<TT>::res(variable) ) {
                printf( "Error : constructor LMT::Vec<%s>(mxArray *) needs a %s Matrix\\n", TypeInformation<TT>::type().c_str(), TypeInformation<typename mxArrayGoodtype<TT>::needed>::type().c_str() );
                throw std::runtime_error("constructor LMT::Vec<"+TypeInformation<TT>::type()+">(mxArray *) needs a "+TypeInformation<typename mxArrayGoodtype<TT>::needed>::type()+" Matrix");
            }
            if ( fixed_size && static_size!=mxGetN(variable) ) {
                PRINTM( "Error : Vec<...,"+to_string<static_size>()+">(const mxArray *variable) must take a vector of size "+to_string<static_size>() );
                throw std::runtime_error("size error");
            }

            resize( mxGetN(variable) * mxGetM(variable) );

            double *pr;
            pr = mxGetPr(variable);
            for(unsigned i=0;i<size();++i)
                val[i] = (TT)pr[i];
            if ( mxIsComplex(variable) ) {
                pr = mxGetPi(variable);
                for(unsigned i=0;i<size();++i)
                    set_imag(val[i],pr[i]);
            }
	    return *this;
        } /// matlab -> LMT::Vec
    #endif

    """ + """~Vec() { allocator.free_mem(val); }
    Vec(const SetSize &ss,unsigned s) { init(); resize(s); } ///
    Vec(const SetSize &ss,unsigned s,const TT &val) { init(); resize(s,val); } ///

    unsigned reserved() const { return r; }

    template<class T2,int s1> Vec &append(const Vec<T2,s1> &vec) { unsigned os=size(); resize(size()+vec.size()); for(unsigned i=0;i<vec.size();++i,++os) val[os] = (TT)vec[i]; return *this; }
    TT *new_elem() { if ( r==0 ) reserve(1); else if ( s==r ) reserve(2*r); return &val[s++]; } /// resize *this if necessary and return address to last element
    Vec &operator<<( const TT &val ) { push_back( val ); return *this; }
    TT &push_back(const TT &val) { TT &res = *new_elem(); res = val; return res; } /// Add a copy of val as a new element.
    TT &push_back_in_reserved(const TT &v) { val[s] = v; return val[s++]; } /// Add a copy of val as a new element. Assumes that room for val is already reserved. Only for dynamic vectors.
    TT &push_back_unique(const TT &v) { for(unsigned i=0;i<size();++i) if ( val[i] == v ) return val[i]; return push_back( v ); } /// Add a copy of val as a new element. Assumes that room for val is already reserved. Only for dynamic vectors.
    TT& pop_back() { --s; return val[s]; } /// remove the last element.
    void pop_back(unsigned nb_val) { s -= nb_val; } /// remove the nb_val elements at end.
    void fit_memory() { reserve(size()); } /// if size() is lesser than reserved, reallocate memory with 'right' size.
    void erase_elem_nb(unsigned i) { for(unsigned j=i;j<s-1;++j) val[j]=val[j+1]; pop_back(); } /// erase element number i. This procedure maintains the order of elements
    void erase_elem_strip(unsigned i,unsigned j) {
        if ( j > i ) {
            unsigned d = j - i;
            s -= d;
            for(unsigned k=i;k<s;++k)
                val[k]=val[k+d];
        }
    } /// erase elements from number i to number j (excluded). This procedure maintains the order of elements
    template<class T2> void erase_first(const T2 &v) { for(unsigned i=0;i<size();++i) if (val[i]==v) { val[i]=back(); pop_back(); } } /// erase first element==v. This procedure DOES NOT maintain the order of elements
    void erase_elem_nb_unordered(unsigned i) { val[i] = val[--s]; } /// erase element number i. This procedure does not maintain the order of elements

    void resize(unsigned ns) { if ( ns > r ) reserve(ns); s = ns; }
    void resize(unsigned ns,const TT &v) { unsigned os=s; resize(ns); if (ns>os) copy_vec_value_aligned(v,val+os,ns-os); }

    void reserve(unsigned ns) {
        Allocator nallocator;
        TT *nvec = nallocator.allocate(ns);
        s = min( s, ns );
        copy_vec_aligned(val,nvec,s);
        allocator.free_mem(val);
        val = nvec;
        allocator = nallocator;
        r = ns;
    } /// Reserve room for new elements if ns > size. If ns is lesser than size, resize *this.

    void free() { allocator.free_mem(val); init(); }

    TT *make_room_to_insert( ST from, ST n ) {
        ST os = s;
        resize( s + n );
        for( ST i = os - 1; i >= from; --i )
            val[ i + n ] = val[ i ];
        return val + from;
    }

    void insert_before( ST i, const TT &v ) {
        *make_room_to_insert( i, 1 ) = v;
    }

    TT *insert_ordered( const TT &v ) {
        ST i=0;
        for( ; i < size() and val[ i ] < v; ++i );
        TT *res = make_room_to_insert( i, 1 );
        *res = v;
        return res;
    }

    // res will contain data of this and this is cleared.
    void throw_ref_and_clear( Vec &res ) {
        res.free();
        res.val = val;
        res.s = s;
        res.r = r;
        res.allocator = allocator;
        //
        val = NULL;
        s = 0;
        r = 0;
        allocator.clear();
    }

    """*(1-static_size) + """
    inline void resize(unsigned s) const { DEBUGASSERT( static_size_==s ); }
    inline void resize(unsigned s,const TT &val) { DEBUGASSERT( static_size_==s ); *this = val; }
    """*static_size + """

    unsigned size_in_bytes() const { return sizeof(TT) * size(); }

    TT &operator[](unsigned i) { DEBUGASSERT(i<size()); return val[i]; } /// access to element i
    const TT &operator[](unsigned i) const { DEBUGASSERT(i<size()); return val[i]; } /// access to element i
    const TT &get(unsigned i) const { DEBUGASSERT(i<size()); return val[i]; } /// calling get ensure that the const version is called (useful for SparseVec)
    void set(unsigned i,const TT &v) { DEBUGASSERT(i<size()); val[i] = v; } /// set element i

    template<class ST> SimdVec<TT,2> operator[]( const SimdVec<ST,2> &ind ) const { return SimdVec<TT,2>(val[ind[0]],val[ind[1]]); } /// access to element ind[0], ind[1]...


    template<unsigned nne> SimdVec<TT,nne> &simd(unsigned i,const Number<nne> &) { return reinterpret_cast<SimdVec<TT,nne> &>(val[i]); }
    template<unsigned nne> const SimdVec<TT,nne> &simd(unsigned i,const Number<nne> &) const { return reinterpret_cast<const SimdVec<TT,nne> &>(val[i]); }

    unsigned size() const { return """+size+"""; }

    Vec operator-() const { Vec res = generate(*this,Negate()); return res; }

    /*
    template<class T2> Vec &operator*=(const T2 &va) { apply_nz(*this,AssignSelfOp(),Multiplies(),va); return *this; }
    template<class T2> Vec &operator/=(const T2 &va) { apply_nz(*this,AssignSelfOp(),Divides   (),va); return *this; }
    template<class T2> Vec &operator%=(const T2 &va) { apply_nz(*this,AssignSelfOp(),Modulus   (),va); return *this; }
    template<class T2> Vec &operator&=(const T2 &va) { apply_nz(*this,AssignSelfOp(),And       (),va); return *this; }

    template<class T2,int s2> Vec &operator*=(const Vec<T2,s2> &va) { apply_nz_wi(*this,AssignSelfOp(),Multiplies(),va); return *this; }
    template<class T2,int s2> Vec &operator/=(const Vec<T2,s2> &va) { apply_nz_wi(*this,AssignSelfOp(),Divides   (),va); return *this; }
    template<class T2,int s2> Vec &operator%=(const Vec<T2,s2> &va) { apply_nz_wi(*this,AssignSelfOp(),Modulus   (),va); return *this; }
    template<class T2,int s2> Vec &operator&=(const Vec<T2,s2> &va) { apply_nz_wi(*this,AssignSelfOp(),And       (),va); return *this; }

    template<class T2> Vec &operator+=(const T2 &val) { apply_simd( *this, AssignSelfOp(), Plus() , val ); return *this; }
    template<class T2> Vec &operator-=(const T2 &val) { apply_simd( *this, AssignSelfOp(), Minus(), val ); return *this; }
    template<class T2> Vec &operator^=(const T2 &val) { apply_simd( *this, AssignSelfOp(), Xor()  , val ); return *this; }
    template<class T2> Vec &operator|=(const T2 &val) { apply_simd( *this, AssignSelfOp(), Or()   , val ); return *this; }

    template<class T2,int s2> Vec &operator+=(const Vec<T2,s2> &val) { apply_simd_wi( *this, AssignSelfOp(), Plus() , val ); return *this; }
    template<class T2,int s2> Vec &operator-=(const Vec<T2,s2> &val) { apply_simd_wi( *this, AssignSelfOp(), Minus(), val ); return *this; }
    template<class T2,int s2> Vec &operator^=(const Vec<T2,s2> &val) { apply_simd_wi( *this, AssignSelfOp(), Xor()  , val ); return *this; }
    template<class T2,int s2> Vec &operator|=(const Vec<T2,s2> &val) { apply_simd_wi( *this, AssignSelfOp(), Or()   , val ); return *this; }
    */
"""+SELFOP+"""
"""+GETRANGE+"""

    template<class T2> bool contains( const T2 &v ) const {
        for( unsigned i=0; i<size(); ++i )
            if ( val[i] == v )
                return true;
        return false;
    }
    
    template<class T2 > 
    unsigned find( const T2 &v ) const {
        for( unsigned i=0; i<size(); ++i )
            if ( val[i] == v )
                return i;
        return size();
    }

    template<class T2, class Cmp > 
    unsigned find( const T2 &v, Cmp cmp ) const {
        for( unsigned i=0; i<size(); ++i )
            if ( cmp( val[i], v ) )
                return i;
        return size();
    }

    Vec &set(const TT &v) { for(unsigned i=0;i<size();++i) val[i]=v; return *this; }

    /// return a Vec with random values in [0,1]. if s_dim==-1, user must specify size, else size must be = s_dim
    static Vec random("""+'unsigned size_vec'*(1-static_size)+""") { Vec res; """+'res.resize(size_vec);'*(1-static_size)+""" for(unsigned  i = 0; i < res.size(); res[i++] = TT(rand()/(double)RAND_MAX) ); return res; }

    /*!
    return
    \latex
        $$ [ \\frac{ v \& 2^i }{ 2^i } ]_i $$

    if s_dim==-1, user must specify size, else size must be = s_dim
    */
    static Vec binary_decomp(unsigned v"""+',unsigned size_vec'*(1-static_size)+""") {
        Vec res; """+'res.resize(size_vec);'*(1-static_size)+"""
        for(unsigned i=0,p2i=1;i<res.size();++i,p2i*=2) res[i] = bool( v & p2i );
        return res;
    }
private:
    """ + """
    TT val[static_size_];
    inline void init() const {}
    """ * static_size + """
    inline void init() { val=(TT *)0; s=0; r=0; }

    TT *val;
    unsigned s,r;
    Allocator allocator;

    template<class U> friend void swap(Vec<U> &v1,Vec<U> &v2);

    """ * (1-static_size) + """
};
}
"""

print """
namespace LMT {
template<class T> void swap(Vec<T> &v1,Vec<T> &v2) {
    swap( v1.val      , v2.val       );
    swap( v1.s        , v2.s         );
    swap( v1.r        , v2.r         );
    swap( v1.allocator, v2.allocator );
}
}

"""
