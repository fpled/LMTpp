
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
};

#lastif // VEC_SORTED_LIST_H
}

