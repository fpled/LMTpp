# -*- coding: utf-8 -*-
import string
from vecgenhelp import *

print """
namespace LMT {
template<class T=int> struct VecDirac {};

template<class TT> struct IsVecOp<VecDirac<TT> > { typedef int T; };

/// Dirac vector
template<class TT,int static_size_>
class Vec<VecDirac<TT>,static_size_,int> {
public:
    template<unsigned n> struct SubType { typedef TT T; };
    static const unsigned nb_sub_type = 1;
    
    static const int static_size = static_size_;
    static const bool fixed_size = (static_size_>=0);
    static const int sparsity_rate = 3;

    struct inc_index_access_const {
        inline const TT &operator[](unsigned i) const { return (i==nn) * *val; }
        const TT *val;
        unsigned nn;
    };
    inc_index_access_const begin_inc_index_access_const() const { inc_index_access_const res; res.val=&val; res.nn=nn; return res; }
    
    Vec(const TT &cst_val,unsigned nnn,unsigned s=static_size_):val(cst_val),nn(nnn) { si.set(s); }
    
    TT operator[](unsigned i) const { return (i==nn)*val; } /// access to element i
    TT get(unsigned i) const { return (i==nn)*val; } ///
    unsigned size() const { return si.val; }
    void clear() { si.set(0); }
    
    template<class T2> Vec &operator*=(const T2 &v) { val *= v; return *this; }
    template<class T2> Vec &operator/=(const T2 &v) { val /= v; return *this; }
    template<class T2> Vec &operator%=(const T2 &v) { val %= v; return *this; }
    template<class T2> Vec &operator&=(const T2 &v) { val &= v; return *this; }

    template<class T2,int s2> Vec &operator*=(const Vec<T2,s2> &v) { val *= v[nn]; return *this; }
    template<class T2,int s2> Vec &operator/=(const Vec<T2,s2> &v) { val /= v[nn]; return *this; }
    template<class T2,int s2> Vec &operator%=(const Vec<T2,s2> &v) { val %= v[nn]; return *this; }
    template<class T2,int s2> Vec &operator&=(const Vec<T2,s2> &v) { val &= v[nn]; return *this; }

"""+GETRANGE+"""

    TT val;
    unsigned nn;
private:
    StaticValIf<unsigned,fixed_size,static_size_> si;
};

/*!
 Cette fonction retourne un vecteur \a Vec<VecDirac<T>,-1> , spécialisation de \a Vec , qui est nul partout sauf à l'indice nn avec la valeur val.
 \\relates static_dirac_vec
 \\relates Vec 
 \\friend samir.amrouche@lmt.ens-cachan.fr
 \\friend hugo.leclerc@lmt.ens-cachan.fr
*/
template<class T> Vec<VecDirac<T>,-1> dirac_vec(const T &val,unsigned nn,unsigned size) { return Vec<VecDirac<T>,-1>(val,nn,size); }
/*! 
  Cette fonction retourne un vecteur static \\a Vec<VecDirac<T>,int> , spécialisation de \\a Vec , qui est nul partout sauf à l'indice nn avec la valeur val.
  \\relates dirac_vec 
  \\relates Vec 
 \\friend samir.amrouche@lmt.ens-cachan.fr
 \\friend hugo.leclerc@lmt.ens-cachan.fr
*/
template<int s,class T> Vec<VecDirac<T>,s> static_dirac_vec(const T &val,unsigned nn) { return Vec<VecDirac<T>,s>(val,nn); }

"""


TT = ['class TT']
TV = 'Vec<VecDirac<TT>,s,int>'
print_apply_ext('apply'          ,TT,TV,'for(unsigned i=0;i<v.nn;op(0,PARALIST),++i); op(v.val,PARALIST); for(unsigned i=v.nn+1;i<v.size();op(0,PARALIST),++i);',onlyforconstvec=True)

print_apply_ext('apply_nz'       ,TT,TV,'op(v.val,PARALIST);')
print_apply_ext('apply_range'    ,TT,TV,'DEBUGASSERT(to<=v.size()); for(unsigned i=from;i<v.nn;op(0,PARALIST),++i); if (v.nn>=to && v.nn<from) op(v.val,PARALIST); for(unsigned i=v.nn+1;i<to;op(0,PARALIST), ++i);',suppar=['int from','int to'],onlyforconstvec=True)

print_apply_ext('apply_wi'       ,TT,TV,'for(unsigned i=0;i<v.nn;op(0,i,PARALIST),++i); op(v.val,v.nn,PARALIST); for(unsigned i=v.nn+1;i<v.size();op(0,i,PARALIST),++i);',onlyforconstvec=True)
print_apply_ext('apply_nz_wi'    ,TT,TV,'op(v.val,v.nn,PARALIST);')
print_apply_ext('apply_range_wi' ,TT,TV,'DEBUGASSERT(to<=v.size()); for(unsigned i=from;i<v.nn;op(0,i,PARALIST),++i); if (v.nn>=to && v.nn<from) op(v.val,v.nn,PARALIST); for(unsigned i=v.nn+1;i<to;op(0,i,PARALIST), ++i);',suppar=['int from','int to'],onlyforconstvec=True)

print_apply_ext('find'           ,TT,TV,'return (v.size()>1 && op(0,PARALIST)) || op(0,PARALIST);',ret='bool',onlyforconstvec=True)

print """} // namespace LMT"""
