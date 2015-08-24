//
// C++ Interface: combinatorialiterator
//
// Description: 
//
//
// Author: Hugo LECLERC <leclerc@lmt.ens-cachan.fr>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMT_combinatorialiterator_HEADER
#define LMT_combinatorialiterator_HEADER

#include "../containers/vec.h"

namespace LMT {

/** it is assumed that nb_sub <= nb_tot
    Exemple : <pre>
    for(CombinatorialIterator<4,2> ci;ci;++ci)
        PRINT( *ci );</pre>

    produces
    <pre>
  *ci -> 0 1
  *ci -> 0 2
  *ci -> 0 3
  *ci -> 1 2
  *ci -> 1 3
  *ci -> 2 3
    </pre>
*/
template<unsigned nb_tot,unsigned nb_sub>
class CombinatorialIterator {
public:
    CombinatorialIterator() { pos = nb_sub-1; }
    void operator++() {
        if ( ++pos >= nb_tot ) {
            ++next;
            pos = next.pos+1;
        }
    }
    void get_iter(unsigned *res) const { res[nb_sub-1] = pos; next.get_iter(res); }
    Vec<unsigned,nb_sub> operator*() const { Vec<unsigned,nb_sub> res; get_iter(res.ptr()); return res; }
    
    operator bool() const { return ( pos < nb_tot ); }

    unsigned pos;
    CombinatorialIterator<nb_tot-1,nb_sub-1> next;
};
/**
*/
template<unsigned nb_tot>
class CombinatorialIterator<nb_tot,1> {
public:
    CombinatorialIterator() { pos = 0; }
    void operator++() { ++pos; }
    void get_iter(unsigned *res) const { res[0] = pos; }
    Vec<unsigned,1> operator*() const { return Vec<unsigned,1>(pos); }
    
    operator bool() const { return ( pos < nb_tot ); }

    unsigned pos;
};

/*!
    itérateur sur les n-uplet d'entiers positifs ou nuls ( a_0, a_1, ..., a_n ) tels que a_0 + a_1 + ... + a_n <= s où n = dim - 1
    voici un exemple de code qui affiche tous les résultats :
    \code C/C++
        EFiterator ef( 3, 4 );
        unsigned c = 0;
        while( true ) {
            PRINT( ef.pos );
            ++c;
            if ( not( ef ) ) break;
            ++ef;
        }
        
        PRINT( c );
    
    Le résultat est :
    \verbatim
        ef.pos -> 0 0 0 
        ef.pos -> 1 0 0 
        ef.pos -> 2 0 0 
        ef.pos -> 3 0 0 
        ef.pos -> 4 0 0 
        ef.pos -> 0 1 0 
        ef.pos -> 1 1 0 
        ef.pos -> 2 1 0 
        ef.pos -> 3 1 0 
        ef.pos -> 0 2 0 
        ef.pos -> 1 2 0 
        ef.pos -> 2 2 0 
        ef.pos -> 0 3 0 
        ef.pos -> 1 3 0 
        ef.pos -> 0 4 0 
        ef.pos -> 0 0 1 
        ef.pos -> 1 0 1 
        ef.pos -> 2 0 1 
        ef.pos -> 3 0 1 
        ef.pos -> 0 1 1 
        ef.pos -> 1 1 1 
        ef.pos -> 2 1 1 
        ef.pos -> 0 2 1 
        ef.pos -> 1 2 1 
        ef.pos -> 0 3 1 
        ef.pos -> 0 0 2 
        ef.pos -> 1 0 2 
        ef.pos -> 2 0 2 
        ef.pos -> 0 1 2 
        ef.pos -> 1 1 2 
        ef.pos -> 0 2 2 
        ef.pos -> 0 0 3 
        ef.pos -> 1 0 3 
        ef.pos -> 0 1 3 
        ef.pos -> 0 0 4 
        c -> 35

       
    Rem : EF pour élément fini
*/
struct EFiterator {

    void init( unsigned dim_ ) {
        pos.resize( dim_, 0 );
    }

    EFiterator( unsigned _dim, unsigned s ) : s( s ), dim( _dim ) { 
        assert( dim > 0 );  
        init( _dim );
    }
    
    operator bool() const { 
        return pos.back() != s; 
    }
    
    EFiterator &operator++() {
        unsigned i = 0, somme;
        
        while( true ) {
            somme = 1;
            for( unsigned j = i; j < dim; ++j )
                somme += pos[ j ];

            if ( somme <= s ) {
                pos[ i ]++;
                for( unsigned j = 0; j < i; ++j )
                    pos[ j ] = 0;
                break;            
            }

            i++;
            if ( i > dim )
                break; /// finish : it's the last sequence
        }
    
        return *this;
    } 
   
    unsigned s, dim;
    Vec<unsigned> pos;
};

/*!
    itérateur sur les n-uplet d'entiers positifs ou nuls ( a_0, a_1, ..., a_n ) tels que a_0 + a_1 + ... + a_n = s où n = dim - 1
    voici un exemple de code qui affiche tous les résultats :
    \code C/C++
        unsigned c = 0;
        Hiterator<2> hi( 3 );
        while( true ) {
            PRINT( hi.pos );
            ++c;
            if ( not( hi ) ) break;
            ++hi;
        }
        
        PRINT( c );
    
    Le résultat est :
    \verbatim
        hi.pos -> 3 0 
        hi.pos -> 2 1 
        hi.pos -> 1 2 
        hi.pos -> 0 3 
        c -> 4
    
    
    Rem : H pour hyperplan
*/
template< unsigned _dim >
struct Hiterator {
    static const unsigned dim = _dim;
    
    void init( unsigned s ) {
        pos[ 0 ] = s;
        for( unsigned i = 1; i < dim; ++i ) pos[ i ] = 0;    
    }
    
    Hiterator( unsigned s ) : s( s ) { 
        assert( dim > 0 );  
        init( s );
    }
    
    operator bool() const { 
        return pos[ dim - 1 ] != s; 
    }
    
    Hiterator &operator++() {
        if ( pos[ 0 ] ) {
            pos[ 0 ]--;
            pos[ 1 ]++;
        } else {
            unsigned j = 1;
            while( ( j < dim ) and ( not( pos[ j ] ) ) ) j++;
            if ( j >= dim - 1 )
                return *this; /// finish : it's the last sequence
            pos[ j + 1 ]++;
            pos[ 0 ] = pos[ j ] - 1;
            pos[ j ] = 0;
        }
        return *this;
    } 
   
    unsigned s;
    Vec<unsigned, dim> pos;
};

/*!
    itérateur sur les permutations des nombres { 0, 1, ..., _dim - 1 }
    voici un exemple de code qui affiche tous les résultats :
    \code C/C++
        unsigned c = 0;
        Piterator<3> pi;
        while( true ) {
            PRINT( pi.pos );
            ++c;
            if ( not( pi ) ) break;
            ++pi;
        }
        
        PRINT( c );
    
    Le résultat est :
    \verbatim    
        pi.pos -> 0 1 2 
        pi.pos -> 0 2 1 
        pi.pos -> 1 0 2 
        pi.pos -> 1 2 0 
        pi.pos -> 2 0 1 
        pi.pos -> 2 1 0 
        c -> 6


*/
template< unsigned _dim >
struct Piterator {
    static const unsigned dim = _dim;
    
    void init() {
        for( unsigned i = 0; i < dim; ++i ) pos[ i ] = i;
    }
    
    Piterator( ) { 
        assert(dim > 1);  
        init();
    }
    
    operator bool() const { 
        unsigned k = dim - 2;
        while( ( k < dim - 1 ) and ( pos[ k ] >= pos[ k + 1 ] ) ) --k;
        if ( k > dim - 1 ) 
            return false;
        else 
            return true;
    }
    
    Piterator &operator++() {
        unsigned k = dim - 2;
        while( ( k < dim - 1 ) and ( pos[ k ] >= pos[ k + 1 ] ) ) --k;
        if ( k > dim - 1 ) return *this; /// it's the last permutation
        unsigned l = dim - 1;
        while( ( l > k ) and ( pos[ k ] >= pos[ l ] ) ) --l;
        unsigned t;
        /// swap pos[l] and pos[k]
        t        = pos[ k ];
        pos[ k ] = pos[ l ];
        pos[ l ] = t;
        /// reverse pos[ k+1 ] up to pos[ dim-1]
        unsigned a, b;
        for( a = k + 1, b = dim - 1; a < b; ++a, --b ) {
            t = pos[ a ];
            pos[ a ] = pos[ b ];
            pos[ b ] = t;
        }
        return *this;
    } 
   
    unsigned s;
    Vec<unsigned, dim> pos;
};



};

#endif // LMT_combinatorialiterator_HEADER
