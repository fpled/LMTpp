# -*- coding: utf-8 -*-
import string
from vecgenhelp import *

print """
#ifndef VEC_SORTED_LIST_H
#define VEC_SORTED_LIST_H

#include "vec.h"

namespace LMT {


/*!
*/
template<class TT,unsigned atomic_size=32> struct SortedList {};

template<class TT,unsigned atomic_size> struct IsVecOp<SortedList<TT,atomic_size> > { typedef int T; };

/*!
    TT doit contenir 'TT *next' et 'TT *prev' comme attributs
    
    Attention : les éléments ne sont pas stockés dans ce vecteur. Il faut un autre vecteur (avec stabilité des pointeurs) pour stocker les données (de type TT)
    
    Exemple d'utilisation :
    \code
        #include <containers/VecSortedList.h>
        
        using namespace LMT;
        
        struct A {
            A( double r = 0 ) : b( r ) {}
            bool operator<(const A &a) const { return b < a.b; }
            double b;
            A *next, *prev;
        };
        std::ostream &operator<<(std::ostream &os,const A &a) {
            os << a.b;
            return os;
        }
        
        int main() {
            Vec<SortedList<A> > v;
            Vec<A> t( 1, 2, 3, 4 );
            
            for(unsigned i=0;i<t.size();++i)
                v.push_sup( &t[i] ); // ok parce t est trié
            
            t[2].b = 0.5;
            v.signal_decreasing_item( &t[2] );
            PRINT( v );
            
            v.rm_item( &t[2] );
            PRINT( v );
            
            // parcourt dans le sens croissant
            for( A *i=v.beg; i; i=i->next )
                std::cout << *i << " ";
            std::cout << std::lastl;
            
            // parcourt dans le sens decroissant
            for( A *i=v.last; i; i=i->prev )
                std::cout << *i << " ";
            std::cout << std::lastl;
        }
    
*/
template<class TT,unsigned atomic_size>
class Vec<SortedList<TT,atomic_size>,-1,int> {
public:
    template<unsigned n> struct SubType { typedef TT T; };
    static const unsigned nb_sub_type = 1;
    
    typedef TT T;
    static const int sparsity_rate = 0;
    
    /// 
    Vec() : beg(NULL), last(NULL) {}
    
    ///
    unsigned size() const {
        unsigned res = 0;
        for(const T *i=beg;i;i=i->next)
            ++res;
        return res;
    }
    
    ///
    bool empty() const {
        return beg == NULL;
    }
    
    ///
    T &back() { return *last; }
    ///
    const T &back() const { return *last; }
    
    ///
    T &front() { return *beg; }
    ///
    const T &front() const { return *beg; }
   
    /// assuming that v > all element in this
    void push_sup( T *v ) {
        if ( not beg )
            beg = v;
        v->next = NULL;
        if ( last )
            last->next = v;
        v->prev = last;
        last = v;
    }
   
    ///
    void rm_item( T *i ) {
        if ( i->prev )
            i->prev->next = i->next;
        else {
            beg = i->next;
            if ( beg )
                beg->prev = NULL;
        }
        //
        if ( i->next )
            i->next->prev = i->prev;
        else {
            last = i->prev;
            if ( last )
                last->next = NULL;
        }
    }
    
    /// value of i has decreased and i must be reordered.
    void signal_decreasing_item( T *i ) {
        if ( not i->prev ) 
            return;
        if ( *i > *i->prev )
            return;
        //
        rm_item( i );
        T *n = i->prev;
        while ( n and *n > *i )
            n = n->prev;
        if ( n ) {
            i->prev = n;
            i->next = n->next;
            if ( n->next )
                n->next->prev = i;
            n->next = i;
            i->prev = n;
        } else {
            if ( beg )
                beg->prev = i;
            i->next = beg;
            i->prev = NULL;
            beg = i;
        }
    }
   
    T &operator[](int i) {
        T *res = beg;
        while (i--)
            res = res->next;
        return *res;
    }
    const T &operator[](int i) const {
        const T *res = beg;
        while (i--)
            res = res->next;
        return *res;
    }
    
    T *beg;
    T *last;
};"""


#TP = ['class TT','unsigned atomic_size']
#TV = 'Vec<SortedList<TT,atomic_size>,s,int>'
#for ptr,e in zip(['','_ptr'],['','*']):
    #print_apply_ext('apply'+ptr,TP,TV,"""
            #unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            #for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op("""+e+"""v.atoms[i]->data[j],PARALIST);
            #for(unsigned j=0;j<sl;++j) op("""+e+"""v.last_atom->data[j],PARALIST);
    #""")
    #print_apply_ext('apply_wi'+ptr,TP,TV,"""
            #unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            #for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op("""+e+"""v.atoms[i]->data[j],cpt++,PARALIST);
            #for(unsigned j=0;j<sl;++j) op("""+e+"""v.last_atom->data[j],cpt++,PARALIST);
    #""")
    #print_apply_ext('find'+ptr,TP,TV,"""
            #unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            #for(unsigned i=0;i<nf;++i)
                #for(unsigned j=0;j<atomic_size;++j)
                    #if (op("""+e+"""v.atoms[i]->data[j],PARALIST)) return true;
            #for(unsigned j=0;j<sl;++j)
                #if (op("""+e+"""v.last_atom->data[j],PARALIST)) return true;
            #return false;
    #""",ret='bool')
    #print_apply_ext('remove_if'+ptr,TP,TV,"""
            #unsigned new_in_last = 0, to_be_filtered = v.size();
            #if ( to_be_filtered == 0 ) return;
            #unsigned j=atomic_size;
            #for(unsigned i=0;i<v.atoms.size();++i) {
                #for(j=(j==0);j<atomic_size;++j) {
                    #unsigned os = v.size();
                    #bool res = op("""+e+"""v.atoms[i]->data[j],PARALIST);
                    #new_in_last += v.size() - os;
                    #if ( res ) {
                        #v.atoms[i]->data[j] = v.back();
                        #v.pop_back();
                        #if ( new_in_last ) --new_in_last;
                        #else              --j;
                    #}
                    #if ( --to_be_filtered == 0 ) return;
                #}
            #}
        #""")
    #print_apply_ext('apply_range'+ptr,TP,TV,"""
            #DEBUGASSERT( to <= v.size() );
            #unsigned ie = to / atomic_size;
            #unsigned je = to % atomic_size;
            #unsigned ib = from / atomic_size;
            #unsigned jb = from % atomic_size;
            #if ( ib == ie ) {
                #for(;jb<je;++jb)
                    #op("""+e+""" v.atoms[ib]->data[jb],PARALIST);
            #}
            #else {
                #for(;ib<ie;++ib) {
                    #for(;jb<atomic_size;++jb)
                        #op("""+e+""" v.atoms[ib]->data[jb],PARALIST);
                    #jb=0;
                #}
                #for(;jb<je;++jb)
                    #op("""+e+""" v.atoms[ie]->data[jb],PARALIST);
            #}
    #""",suppar=['int from','int to'])
    #print_apply_ext('apply_nz'+ptr,TP,TV,"""
            #unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            #for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op("""+e+"""v.atoms[i]->data[j],cpt,PARALIST);
            #for(unsigned j=0;j<sl;++j,++cpt) op("""+e+"""v.last_atom->data[j],cpt,PARALIST);
    #""")
    #print_apply_ext('apply_nz_unique'+ptr,TP,TV,'apply_nz'+ptr+'(op,PARALIST);')

#for const in [False,True]:
    #print_apply_ext('find_elem',TP,TV,"""
            #unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            #for(unsigned i=0;i<nf;++i)
                #for(unsigned j=0;j<atomic_size;++j)
                    #if (op(v.atoms[i]->data[j],PARALIST)) return &v.atoms[i]->data[j];
            #for(unsigned j=0;j<sl;++j)
                #if (op(v.last_atom->data[j],PARALIST)) return &v.last_atom->data[j];
            #return NULL;
    #""",ret='const '*const+'TT *',onlyforconstvec=const, onlyfornonconstvec=not const)


print """
#lastif // VEC_SORTED_LIST_H
}
"""
