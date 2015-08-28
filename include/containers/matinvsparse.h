//
// C++ Interface: matinvsparse
//
// Description:
//
//
// Author: Hugo LECLERC <hugo.leclerc@lmt.ens-cachan.fr>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMT_matinvsparse_HEADER
#define LMT_matinvsparse_HEADER

#include <string.h>

#include "boolean.h"
#include "vec.h"
#include "mat.h"

namespace LMT {

template<class T,int s> inline T dot_chol_factorize( const Vec<Sparse<T>,s> &v1, const Vec<Sparse<T>,s> &v2 ) {
    T res = T( 0 );
    if ( v2.indices.size() == 0 ) return T( 0 );
    for(unsigned i=0,j=0;i<v1.indices.size()-1;++i) {
        while ( v2.indices[j] < v1.indices[i] ) {
            ++j;
            if ( j==v2.indices.size() ) return res;
        }
        if ( v2.indices[j] == v1.indices[i] ) res += v2.data[j] * v1.data[i];
    }
    return res;
}

template<unsigned n>
struct HashCH {
    HashCH() {
        for(unsigned i=0;i<n;++i) v[i] = 0;
    }
    inline void add(unsigned col) {
        unsigned i = col / 32;
        v[ i % n ] |= ( 1 << (col%32) );
    }
    inline static bool cor(const HashCH &h1,const HashCH &h2) {
        for(unsigned i=0;i<n;++i)
            if ( h1.v[i] & h2.v[i] )
                return true;
        return false;
    }
    unsigned v[n];
};

/*!
    Cette fonction effectue la factorisation de Cholesky. La matrice m, de format spécial \a Mat<T,TS,SparseLine<>> , contient le résultat.
    Pour un exemple d'utilisation, consulter l'exemple \a [Décomposition d'une matrice] .

    \friend samir.amrouche@lmt.ens-cachan.fr
    \friend hugo.leclerc@lmt.ens-cachan.fr
    \keyword Mathématiques/Algèbre linéaire/Décomposition de matrices
    \author Hugo Leclerc
*/
template<class T,class TS> void chol_factorize( Mat<T,TS,SparseLine<> > &m ) {
    static const unsigned NN = 16;
    Vec<HashCH<NN> > hash; hash.resize( m.nb_rows() );
    for(unsigned line=0;line<m.nb_rows();++line) {
        for(int ind=0;ind<(int)m.data[line].indices.size()-1;++ind) {
            // m_ij != 0
            unsigned col = m.data[line].indices[ind];
            if ( m.data[col].data.size() == 0 )
                continue;
            m.data[line].data[ind] = ( m.data[line].data[ind] - dot_chol_factorize( m.data[col], m.data[line] ) ) * m.data[col].data.back();
            hash[ line ].add( col );
            // m_ij == 0
            unsigned ie = m.data[line].indices[ind+1];
            while ( ++col < ie ) {
                if ( not HashCH<NN>::cor( hash[col], hash[ line ] ) ) continue;

                T v = dot_chol_factorize( m.data[col], m.data[line] );
                if ( boolean_( v ) ) {
                    unsigned os = m.data[line].indices.size();
                    m.data[line].indices.resize( os+1 );
                    m.data[line].data.resize( os+1 );
                    ++ind;
                    for(int k=os;k>ind;--k) {
                        m.data[line].indices[k] = m.data[line].indices[k-1];
                        m.data[line].data[k] = m.data[line].data[k-1];
                    }
                    m.data[line].data[ind] = -v * m.data[col].data.back();
                    m.data[line].indices[ind] = col;
                    hash[ line ].add( col );
                }
            }
        }
        // on diag
        if ( m.data[line].data.size() )
            m.data[line].data.back() = T(1.0) / sqrt( m.data[line].data.back() - norm_2_p2( m.data[line].data.begin(), m.data[line].data.size()-1 ) );
    }
    //PRINT( hash );
}


/*!
 Cette fonction retourne la matrice ret, triangulaire inférieure réelle de la décomposition de Cholesky d'une matrice symétrique réelle m.

 On utilise l'algorithme de Cholesky-Banachiewicz qui fonctionne par lignes (cf l'article http://en.wikipedia.org/wiki/Cholesky_decomposition )

    \friend samir.amrouche@lmt.ens-cachan.fr
*/
template<class T,class TS> void get_Cholesky( Mat<T,TS,SparseLine<> > &m, Mat<T,TriLower<>,SparseLine<> > & ret ) {
    static const unsigned NN = 16;
    Vec<HashCH<NN> > hash; hash.resize( m.nb_rows() );
    ret.clear();
    ret.resize( m.nb_rows() );
    for(unsigned line=0;line<m.nb_rows();++line) {
        for(int ind=0;ind<(int)m.data[line].indices.size()-1;++ind) {
            /// m_ij != 0
            unsigned col = m.data[line].indices[ind];
            ret.data[line].indices.push_back(col);
            ret.data[line].data.push_back( ( m.data[line].data[ind] - dot_chol_factorize( ret.data[col], ret.data[line] ) ) / ret.data[col].data.back() );
            hash[ line ].add( col );
            /// m_ij == 0
            unsigned ie = m.data[line].indices[ind+1];
            while ( ++col < ie ) {
                if ( not HashCH<NN>::cor( hash[col], hash[ line ] ) ) continue;

                T v = dot_chol_factorize( ret.data[col], ret.data[line] );
                if ( boolean_( v ) ) {
                    ret.data[line].data.push_back( -v / ret.data[col].data.back() );
                    ret.data[line].indices.push_back( col );
                    hash[ line ].add( col );
                }
            }
        }
        /// on diag
        if ( m.data[line].data.size() ) {
            ret.data[line].data.push_back( sqrt( m.data[line].data.back() - norm_2_p2( ret.data[line].data.begin(), ret.data[line].data.size() ) ) );
            ret.data[line].indices.push_back( m.data[line].indices.back() );
        }
    }
}

// template<class T> void incomplete_chol_factorize( Mat<T,Sym<>,SparseLine<> > &m, int level = 1  ) {
//     static const unsigned NN = 16; /// ajouté par Raphaël
//     Vec<HashCH<NN> > hash; hash.resize( m.nb_rows() ); /// ajouté par Raphaël
//     for(unsigned line=0;line<m.nb_rows();++line) {
//         for(unsigned ind=0;ind<m.data[line].indices.size()-1;++ind) {
//             // m_ij != 0
//             unsigned col = m.data[line].indices[ind];
//             if ( m.data[col].data.size() == 0 ) continue; /// ajouté par Raphaël
//             m.data[line].data[ind] = ( m.data[line].data[ind] - dot_chol_factorize( m.data[col], m.data[line] ) ) * m.data[col].data.back();
//             hash[ line ].add( col ); /// ajouté par Raphaël
//             // m_ij == 0
//             unsigned ie = min( m.data[line].indices[ind+1], col+level );
//             while ( ++col < ie ) {
//                 if ( not HashCH<NN>::cor( hash[col], hash[ line ] ) ) continue;
//
//                 T v = dot_chol_factorize( m.data[col], m.data[line] );
//                 if ( boolean_( v ) ) {
//                     unsigned os = m.data[line].indices.size();
//                     m.data[line].indices.resize( os+1 );
//                     m.data[line].data.resize( os+1 );
//                     ++ind;
//                     for(unsigned k=os;k>ind;--k) {
//                         m.data[line].indices[k] = m.data[line].indices[k-1];
//                         m.data[line].data[k] = m.data[line].data[k-1];
//                     }
//                     m.data[line].data[ind] = -v * m.data[col].data.back(); /// dans la version commentée suivante c'est une /
//                     m.data[line].indices[ind] = col;
//                     hash[ line ].add( col );
//                 }
//             }
// //             while ( ++col < ie ) {
// //                 T v = dot_chol_factorize( m.data[col], m.data[line] );
// //                 if ( v ) {
// //                     unsigned os = m.data[line].indices.size();
// //                     m.data[line].indices.resize( os+1 );
// //                     m.data[line].data.resize( os+1 );
// //                     ++ind;
// //                     for(unsigned k=os;k>ind;--k) {
// //                         m.data[line].indices[k] = m.data[line].indices[k-1];
// //                         m.data[line].data[k] = m.data[line].data[k-1];
// //                     }
// //                     m.data[line].data[ind] = -v / m.data[col].data.back();
// //                     m.data[line].indices[ind] = col;
// //                 }
// //             }
//        }
//         // on diag
//         //#ifdef DO_NOT_SQRT_DIAG_CHOL
//         //    m.data[line].data.back() = m.data[line].data.back() - norm_2_p2( m.data[line].data.begin(), m.data[line].data.size()-1 );
//         //#else
//         if ( m.data[line].data.size() )
//             m.data[line].data.back() = 1.0 / sqrt( m.data[line].data.back() - norm_2_p2( m.data[line].data.begin(), m.data[line].data.size()-1 ) );
//         //#endif
//     }
// }


///
template<class T,unsigned simplified>
void incomplete_chol_factorize( Mat<T,Sym<>,SparseLine<> > &m, Number<simplified> ) {
    for(unsigned line = 0; line < m.nb_rows(); ++line ) {
        for(unsigned ind = 0; ind < m.data[line].indices.size() - 1; ++ind ) {
            /// m_ij != 0
            unsigned col = m.data[line].indices[ind];
            m.data[line].data[ind] = ( m.data[line].data[ind] - dot_chol_factorize( m.data[col], m.data[line] ) ) * m.data[col].data.back();
            /// m_ij == 0
            //             unsigned ie = min( m.data[line].indices[ind+1], col+2 );
            //             while ( ++col < ie ) {
            //                 T v = dot_chol_factorize( m.data[col], m.data[line] );
            //                 if ( boolean_( v ) ) {
            //                     unsigned os = m.data[line].indices.size();
            //                     m.data[line].indices.resize( os+1 );
            //                     m.data[line].data.resize( os+1 );
            //                     ++ind;
            //                     for(unsigned k=os;k>ind;--k) {
            //                         m.data[line].indices[k] = m.data[line].indices[k-1];
            //                         m.data[line].data[k] = m.data[line].data[k-1];
            //                     }
            //                     if ( simplified )
            //                         m.data[line].data[ind] = - v * m.data[col].data.back();
            //                     else
            //                         m.data[line].data[ind] = - v / m.data[col].data.back();
            //                     m.data[line].indices[ind] = col;
            //                 }
            //             }
        }

        T d = m.data[line].data.back() - norm_2_p2( m.data[line].data.begin(), m.data[line].data.size() - 1 );
        m.data[line].data.back() = T(1) / sqrt( abs( d ) + ( d == T(0) ) );
    }
}

/// ancienne version de incomplete_chol_factorize
template<class T> 
void incomplete_chol_factorize( Mat<T,Sym<>,SparseLine<> > &m, bool simplified = true ) {
    if ( simplified ) {
        for(unsigned line = 0; line < m.nb_rows(); ++line ) {
            for(unsigned ind = 0; ind < m.data[line].indices.size() - 1; ++ind ) {
                /// m_ij != 0
                unsigned col = m.data[line].indices[ind];
                m.data[line].data[ind] = ( m.data[line].data[ind] - dot_chol_factorize( m.data[col], m.data[line] ) ) * m.data[col].data.back();
            }
    
            T d = m.data[line].data.back() - norm_2_p2( m.data[line].data.begin(), m.data[line].data.size() - 1 );
            m.data[line].data.back() = typename TypePromote<Sqrt,typename TypePromote<Abs,T>::T>::T(1) / sqrt( abs( d ) + ( d == T(0) ) );
        }
    } else {
        for(unsigned line = 0; line < m.nb_rows(); ++line ) {
            for(unsigned ind = 0; ind < m.data[line].indices.size() - 1; ++ind ) {
                /// m_ij != 0
                unsigned col = m.data[line].indices[ind];
                m.data[line].data[ind] = ( m.data[line].data[ind] - dot_chol_factorize( m.data[col], m.data[line] ) ) / m.data[col].data.back();
            }
    
            T d = m.data[line].data.back() - norm_2_p2( m.data[line].data.begin(), m.data[line].data.size() - 1 );
            m.data[line].data.back() = T( sqrt( abs( d ) + ( d == T(0) ) ) );
        }        
    }
}

// template<class T> 
// void incomplete_chol_factorize_bis( Mat<T,Sym<>,SparseLine<> > &m ) {
//     Vec<T> front_jcol;
//     T pivot, ipivot;
//     
//     front_jcol.resize( m.nb_rows(), 0 );
// 
//     for(unsigned line = 0; line < m.nb_rows(); ++line ) {
//     
//         pivot = m.data[line].data.back();
//         m.data[line].data.back() = 0;
//         pivot = sqrt( pivot - dot_chol_factorize( m.data[line], m.data[line] ) );
//         ipivot = 1. / pivot;
//          
//         for(unsigned i = line + 1; i < m.nb_rows(); ++i ) {
//             unsigned ind_j = front_jcol[ i ];
//             unsigned j = m.data[ i ].indices[ front_jcol[ i ] ];
//             if (  j == line ) {
//                 m.data[ i ].data[ ind_j ] = ( m.data[ i ].data[ ind_j ] - dot_chol_factorize( m.data[i], m.data[line] ) ) * ipivot;
//                 front_jcol[ i ]++;
//             } else {
//                 if ( j < line )
//                     front_jcol[ i ]++;
//             }
//         }
//         
//         m.data[line].data.back() = pivot;
//     }
// }


/*!
 \internal
*/
// template<class TV,class TM>
// struct PICF {
//     unsigned i, line;
//      TV *cr;
//      TM *m;
// };

/*!
 \internal
*/
// template<class TV,class TM>
// void *apply_incomplete_chol_factorize( void *params ) {
// 
//     PICF<TV,TM> &picf ( *static_cast< PICF<TV,TM> *>( params ) );
//     typedef typename TV::template SubType<0,0>::T T;
// 
//     T t = (*picf.cr)[ picf.i - picf.line - 1 ];
// 
//     for( unsigned icol = 0; icol < picf.m->data[ picf.i ].indices.size(); ++icol ) {
//         unsigned j = picf.m->data[ picf.i ].indices[ icol ];
//         if ( j > picf.line ) 
//             picf.m->data[ picf.i ].data[ icol ] -= t * (*picf.cr)[ j - picf.line - 1 ];
//     }
// 
//     return (void *)NULL;
// }

// template<class T> 
// void incomplete_chol_factorize_bis( Mat<T,Sym<>,SparseLine<> > &m, unsigned nb_threads = 2 ) {
// 
//     typedef Vec<T> TV;
//     typedef Mat<T,Sym<>,SparseLine<> > TM; 
//     
//     T ipivot, t;
//     TV current_row; current_row.resize( m.nb_rows() - 1 );
// 
//     PICF< TV, TM > *picf = new PICF< TV, TM >[ nb_threads ];
//     for( unsigned th = 0; th < nb_threads; ++th ) {
//         picf[ th ].m  = &m;
//         picf[ th ].cr = &current_row;
//     }
//     
//     pthread_t *tid = new pthread_t[ nb_threads ];
//     
//     pthread_attr_t pthread_attr;
//     pthread_attr_init( &pthread_attr ); 
// 
//     for( unsigned line = 0; line < m.nb_rows(); ++line ) {
//     
//         if ( m.data[line].indices.back() == line ) {
//             t = sqrt( m.data[line].data.back() );
//             ipivot = (T)1 / t;
//             m.data[line].data.back() = t;
//         } else
//             ipivot = 1e5; /// problème : pivot nulle !
//         
//         for( unsigned i = line + 1; i < m.nb_rows(); ++i ) {
//             unsigned icol = 0;
//             while ( ( icol < m.data[ i ].indices.size() ) and ( m.data[ i ].indices[ icol ] < line ) ) icol++;
//             /// on ne peut pas avoir que des zéros à partir de line sur la ligne line en théorie...
//             if ( m.data[ i ].indices[ icol ] == line ) {
//                 t = m.data[ i ].data[ icol ] * ipivot;
//                 current_row[ i - line - 1 ] = t;
//                 m.data[ i ].data[ icol ] = t;                
//             } else 
//                 current_row[ i - line - 1 ] = (T)0;    
//         }
//         
//         for( unsigned th = 0; th < nb_threads; ++th )
//             picf[ th ].line = line;
//         
//         for( unsigned i = line + 1; i < m.nb_rows(); i += nb_threads ) {
//             unsigned ith = min( i + nb_threads, m.nb_rows() ) - i;
//             
//             memset( (void*) tid, 0, nb_threads * sizeof( pthread_t ) );
//             
//             for( unsigned th = 0; th < ith; ++th ) {
//                 picf[ th ].i = i + th;
//                 pthread_create( &tid[ th ], &pthread_attr, apply_incomplete_chol_factorize< TV, TM >, static_cast<void *> ( picf + th ) );
//                 std::cout << "line = " << line << " i = " << i << " th = " << th << " tid = " << tid[ th ] << std::endl;
//             }
// 
//             for(unsigned th = 0; th < ith; ++th )
//                 pthread_join( tid[ th ], (void **)picf );
// 
//         }
//     }
//     
//     delete [] tid;
//     delete [] picf;
// }

/*!
    m is assumed to be factorized
    sol sontient le second membre et res contiendra la solution.

    \keyword Mathématiques/Algèbre linéaire/Décomposition de matrices
    \friend samir.amrouche@lmt.ens-cachan.fr
    \friend hugo.leclerc@lmt.ens-cachan.fr
*/
template<class T,int s,int s2> void solve_using_chol_factorize( const Mat<T,Sym<s>,SparseLine<> > &mat, const Vec<T> &sol, Vec<T,s2> &res ) {
    unsigned nb_lines = mat.nb_rows();

    res.resize( nb_lines );
    //
    for(unsigned line=0;line<nb_lines;++line) {
        T v = sol[line];
        for(int i=0;i<(int)mat.data[line].data.size()-1;++i)
            v -= mat.data[line].data[i] * res[ mat.data[line].indices[i] ];
        if ( mat.data[line].data.size() == 0 ) {
            std::cerr << "Matrix with a void line." << std::endl;
            return;
        }
        res[line] = v * mat.data[line].data.back();
    }

    //
    Vec<T,s2> tmpvec = res;
    while (nb_lines--) {
        T tmp = tmpvec[nb_lines] * mat.data[nb_lines].data.back();
        for(int i=0;i<(int)mat.data[nb_lines].data.size()-1;++i)
            tmpvec[ mat.data[nb_lines].indices[i] ] -= mat.data[nb_lines].data[i] * tmp;
        res[nb_lines] = tmp;
    }
}

/*!
    m is assumed to be factorized
    sol sontient le second membre et res contiendra la solution.

    \keyword Mathématiques/Algèbre linéaire/Décomposition de matrices
    \friend samir.amrouche@lmt.ens-cachan.fr
    \friend hugo.leclerc@lmt.ens-cachan.fr
*/
template<class T,int s,int s2> void solve_using_chol_factorize( const Mat<T,Herm<s>,SparseLine<> > &mat, const Vec<T> &sol, Vec<T,s2> &res ) {
    unsigned nb_lines = mat.nb_rows();

    res.resize( nb_lines );
    //
    for(unsigned line=0;line<nb_lines;++line) {
        T v = sol[line];
        for(int i=0;i<(int)mat.data[line].data.size()-1;++i)
            v -= LMT::conj( mat.data[line].data[i] ) * res[ mat.data[line].indices[i] ];
        res[line] = v / LMT::conj( mat.data[line].data.back() );
    }

    //
    Vec<T,s2> tmpvec = res;
    while (nb_lines--) {
        T tmp = tmpvec[nb_lines] / mat.data[nb_lines].data.back();
        for(int i=0;i<(int)mat.data[nb_lines].data.size()-1;++i)
            tmpvec[ mat.data[nb_lines].indices[i] ] -= mat.data[nb_lines].data[i] * tmp;
        res[nb_lines] = tmp;
    }
}

template<class T,int s2> 
int solve_using_incomplete_chol_factorize( const Mat<T,Sym<>,SparseLine<> > &mp, const Mat<T,Sym<>,SparseLine<> > &A, const Vec<T> &b, Vec<T,s2> &x, T crit = 1e-4, bool disp_r = true ) {
    if ( x.size() <= A.nb_rows() )
        x.resize( A.nb_rows(), T(0) );
    //
    Vec<T> r, d, q, s;

    r = b - A * x;
    for(unsigned i=0;;++i) { 
        if ( i==r.size() ) return 0; 
        if ( abs(r[i]) > abs(crit) ) break;
    }
    solve_using_chol_factorize( mp, r, d );


    T deltn = dot( r, d );
    unsigned cpt = 0;
    while ( /*cpt < 200*/ true ) {
        T delto = deltn;

        q = A * d;
        T alpha = deltn / dot( d, q );
        x += alpha * d;
        // r -= alpha * q;
        r = b - A * x;
        for(unsigned i=0;;++i) { if ( i==r.size() ) return cpt; if ( abs(r[i]) > abs(crit) ) break; }

        solve_using_chol_factorize( mp, r, s );
        deltn = dot( r, s );

        T beta = deltn / delto;
        d = s + beta * d;
        ++cpt;
        if ( disp_r )
            std::cout << "Iteration " << cpt << " : norme inf du residu = " << max(abs(r)) << std::endl;
    }
    return -1;
}


// ---------------------------------------- LU ----------------------------------------
/*!
vector_permutation contient le vecteur permutation. cad : si
0 -> s(0)
1 -> s(1)
2 -> s(2)
etc...
alors
vector_permutation[0] = s(0)
vector_permutation[1] = s(1)
vector_permutation[2] = s(2)
etc...
Soit P la matrice de permutation associée. On a   LU = PA
Pour des infos sur la matrice de permutation, allez voir la page :
    \relates http://en.wikipedia.org/wiki/Permutation_matrix

ATTENTION : le nombre de coefficients par ligne ne doit pas dépasser 2^31-1 pour une architecture 32 bits !!!!

*/

template<class T>
bool lu_factorize( Mat<T,Gen<>,SparseLU> &m, Vec<int> &vector_permutation ) {
    int n = m.nb_rows();
    int jj,ii,i,ipivot,j2,ind,indc;
    int ideb, k;
    typename TypePromote<AbsIndication,T>::T norme_maxi;
    T big_in_abs;
    T tmp;

    vector_permutation.resize(n);
    for(int k=0;k<n;k++)
        vector_permutation[k] = k;

    for(int j=0;j<n;++j) {
        // something on diag ?   ==== on le met ici car après un pivotement on peut se retrouver avec un coeff sur la diagonale inexistant.
        for(jj=0;jj<n;++jj) {
            if (m.U[jj].indices.size() > 0) {
                if ( m.U[jj].indices.back() != jj ) {
                    m.U[jj].indices.push_back( jj );
                    m.U[jj].data.push_back( (T)0 );
                }
            } else {
                m.U[jj].indices.push_back( jj );
                m.U[jj].data.push_back( (T)0 );
            }
        }

        // calcul  de la vraie colonne j de U
        for( ind=0;ind<m.U[j].indices.size();++ind) {
            // u_ij == 0
            int ie = 0;
            if ( ind > 0 )
                ie = m.U[j].indices[ind-1] + 1;
            ii = m.U[j].indices[ind];
            indc = 0;
            for(i=ie;i<ii;i++) {
                tmp = - dot_chol_factorize( m.U[j], m.L[i] );
                if ( abs_indication( tmp ) ) {
                    int os = m.U[j].indices.size();
                    m.U[j].indices.resize( os+1 );
                    m.U[j].data.resize( os+1 );
                    for( k=os;k>ind+indc;--k) {
                        m.U[j].indices[k] = m.U[j].indices[k-1];
                        m.U[j].data[k] = m.U[j].data[k-1];
                    }
                    m.U[j].data[ind+indc] = tmp;
                    m.U[j].indices[ind+indc] = i;
                    ++indc;
                }
            }
            // u_ij != 0
            //i = m.U[j].indices[ind+indc];
            ind += indc;
            m.U[j].data[ind] -=  dot_chol_factorize( m.U[j], m.L[ii] );
            m.U[j].indices[ind] = ii;
        }


        // pseudo calcul de la vraie colonne j de L. Comme L est stckée en ligne les choses sont bien plus complexes que celles de U...
        // en même temps on recherche le meilleur pivot partiel.
        big_in_abs = m.U[j].data.back() ; // on y met le coefficient de la diagonale.
        ipivot = j ; // pivot a priori
        for(i=j+1;i<n;i++) {
            // on détermine s'il y a un coefficient non nul à la colonne j.
            for( k=0;k<m.L[i].indices.size();++k)
                if (m.L[i].indices[k]>=j)
                    break;
            if (k<m.L[i].indices.size()) {
                // on a dépassé j.
                if (m.L[i].indices[k] == j) {
                    // il y a un coefficient à la colonne j.
                    tmp = m.L[i].data[k] - dot_chol_factorize( m.U[j], m.L[i] );
                    if (abs_indication(tmp) > abs_indication(big_in_abs)) {
                        big_in_abs = tmp;
                        ipivot = i;
                    }
                    m.L[i].data[k] = tmp;

                } else {
                    // il n' y en a pas. C'est le plus galère...
                    tmp = - dot_chol_factorize( m.U[j], m.L[i] );
                    if (abs_indication( tmp ) ) {
                        unsigned os = m.L[i].indices.size();
                        m.L[i].indices.resize( os+1 );
                        m.L[i].data.resize( os+1 );
                        for(ii=os;ii>k;--ii) {
                            m.L[i].indices[ii] = m.L[i].indices[ii-1];
                            m.L[i].data[ii] = m.L[i].data[ii-1];
                        }
                        m.L[i].indices[k] = j;
                        m.L[i].data[k] = tmp;
                    }
                    if (abs_indication(tmp) > abs_indication(big_in_abs)) {
                        big_in_abs = tmp;
                        ipivot = i;
                    }
                }
            } else {
                // aucun coefficient non nul n'est après j ou à la colonne j.
                tmp = - dot_chol_factorize( m.U[j], m.L[i] );
                if (abs_indication( tmp ) ) {
                    m.L[i].indices.push_back(j);
                    m.L[i].data.push_back( tmp );
                }
                if (abs_indication(tmp) > abs_indication(big_in_abs)) {
                    big_in_abs = tmp;
                    ipivot = i;
                }

            }

        }
        ///permutation des lignes si nécessaire.////////////////////////////////////////
        if (ipivot > j) {

            // on permute les lignes horizontales line et line2.
            swap(m.L[j],m.L[ipivot]) ; // attention : la ligne ipivot est bien trop longue!

            // je mets le mauvais pivot de la ligne line dans la ligne line2.
            m.L[ipivot].indices.push_back(j);
            m.L[ipivot].data.push_back( m.U[j].data.back() );
            // puis je l'enlève
            m.U[j].indices.pop_back();
            m.U[j].data.pop_back();

            // je complète la ligne 2 avec les éventuels élements de U qui sont à la ligne horizontale line.
            for( jj=j+1;jj<ipivot;++jj) {
                for( ii=0;ii<m.U[jj].indices.size();++ii)
                    if (m.U[jj].indices[ii] >= j)
                        break;
                if (m.U[jj].indices[ii] == j) {
                    // on a trouvé un élément de U sur la ligne horizontale line
                    // on le copie où il faut
                    m.L[ipivot].indices.push_back(jj);
                    m.L[ipivot].data.push_back( m.U[jj].data[ii] );
                    // on l'élimine de U
                    for( k = ii+1;k<m.U[jj].indices.size();++k) {
                        m.U[jj].data[k-1] = m.U[jj].data[k];
                        m.U[jj].indices[k-1] = m.U[jj].indices[k];
                    }
                    m.U[jj].indices.pop_back();
                    m.U[jj].data.pop_back();
                }
            }

            // Ensuite les éléments de la ligne horizontale d'indice line qui dépasse la diagonale sont placés où il faut dans la matrice U...
            // on saute les éléments qui sont sous la diagonale
            for( ind=0;ind<m.L[j].indices.size();++ind)
                if (m.L[j].indices[ind]>=j)
                    break;
            // seuls les éléments après la diagonale nous intéressent.
            jj = ind;
            for( ; ind<m.L[j].indices.size();++ind) {
                // on allonge la ligne
                m.U[m.L[j].indices[ind]].indices.push_back(n-1);
                m.U[m.L[j].indices[ind]].data.push_back( T(0) );
                // on recopie les derniers éléments
                for( k = m.U[m.L[j].indices[ind]].indices.size()-1;k>=1; --k)// important que k soit du type int.
                    if (m.U[m.L[j].indices[ind]].indices[k-1] > j ) {
                        m.U[m.L[j].indices[ind]].indices[k] = m.U[m.L[j].indices[ind]].indices[k-1];
                        m.U[m.L[j].indices[ind]].data[k] = m.U[m.L[j].indices[ind]].data[k-1];
                    } else
                        break;
                // on met l'élément dans U et on enlève celui de L
                if (k>=0) {
                    m.U[m.L[j].indices[ind]].indices[k] = j;
                    m.U[m.L[j].indices[ind]].data[k] = m.L[j].data[ind];
                }
            }
            // on enlève les élements de L de la ligne horizontale line qui dépassent.
            m.L[j].indices.resize(jj);
            m.L[j].data.resize(jj);

            // Enfin on échange aussi les éléments de U qui sont horizontalement en indice j et ipivot entre line2 et n (verticalement)...
            for(j2=ipivot;j2<n;++j2) {
                // on commence par savoir s'il y a des éléments d'indice line et line2.
                // il y a trois cas à traiter :
                // * on a les deux éléments : alors on les permute.
                // * on a un seul élément : alors on décale et on le place.
                // * on a aucun élément (mon cas préféré) : on ne fait rien ;-)
                ii = n;
                jj = n;
                for(k=0;k<m.U[j2].indices.size();++k) {
                    if (m.U[j2].indices[k] == j) {
                        ii = k;
                        break;
                    }
                }
                for(k=0;k<m.U[j2].indices.size();++k) {
                    if (m.U[j2].indices[k] == ipivot) {
                        jj = k;
                        break;
                    }
                }
                if ((ii<n) && (jj<n)) {
                    swap(m.U[j2].data[ii],m.U[j2].data[jj]);
                } else
                    if (ii<n) {
                        tmp = m.U[j2].data[ii];
                        for(k=ii;k<m.U[j2].indices.size()-1;++k)
                            if (m.U[j2].indices[k+1]<ipivot) {
                                m.U[j2].indices[k] = m.U[j2].indices[k+1];
                                m.U[j2].data[k] = m.U[j2].data[k+1];
                            } else
                                break;
                        m.U[j2].indices[k] = ipivot;
                        m.U[j2].data[k] = tmp;
                    } else
                        if (jj<n) {
                            tmp = m.U[j2].data[jj];
                            for(k=jj;k>0;--k)
                                if (m.U[j2].indices[k-1]>j) {
                                    m.U[j2].indices[k] = m.U[j2].indices[k-1];
                                    m.U[j2].data[k] = m.U[j2].data[k-1];
                                } else
                                  break;
                            m.U[j2].indices[k] = j;
                            m.U[j2].data[k] = tmp;
                        }
            }


            // on n'oublie pas de permuter aussi le vecteur permutation.
            k = vector_permutation[ipivot];
            vector_permutation[ipivot] = vector_permutation[j];
            vector_permutation[j] = k;
        }/// fin de la permutation   ////////////////////////////////////////



        if (abs_indication(big_in_abs) )
            big_in_abs = 1. / big_in_abs;
        else {
            std::cerr << " pivot nul" << std::endl;
            return false;
        }


        /// Enfin on multiplie toutes les coefficients des lignes j+1 à n et colonne j par l'inverse du pivot partiel
        for(i=j+1;i<n;i++) {
            // on détermine s'il y a un coefficient non nul à la colonne j.
            for( k=0;k<m.L[i].indices.size();++k)
                if (m.L[i].indices[k]>=j)
                    break;
            if (k<m.L[i].indices.size()) // on a dépassé j.
                if (m.L[i].indices[k] == j)
                    m.L[i].data[k] *= big_in_abs;
        }


    }
    return true;
}

/// m is assumed to be factorized
template<class T,int s,int s2> void solve_using_lu_factorize( const Mat<T,Gen<s>,SparseLU> &mat, const Vec<int> &permutation, const Vec<T> &sol, Vec<T,s2> &res /*, bool allow_permutation = true*/ ) {
    //if ( allow_permutation ) {
        Vec<T,s2> tmp_sol = sol[ permutation ];
        solve_using_lu_factorize( mat, tmp_sol, res );
        //res = tmp_res;
    //} else
    //    solve_using_lu_factorize( mat, sol, res );
}

/// in place...
template<class T> void lu_factorize( Mat<T,Gen<>,SparseLU> &m ) {
    unsigned n = m.nb_rows();
    // something on diag ?
    for(unsigned line=0;line<n;++line) {
        if ( m.U[line].indices.back() != line ) {
            m.U[line].indices.push_back( line );
            m.U[line].data.push_back( T(0) );
        }
    }
    //
    for(unsigned line=0;line<n;++line) {
        // L
        for(unsigned ind=0;ind<m.L[line].indices.size();++ind) {
            // l_ij != 0
            unsigned col = m.L[line].indices[ind];
            m.L[line].data[ind] = ( m.L[line].data[ind] - dot_chol_factorize( m.U[col], m.L[line] ) ) / m.U[col].data.back();
            // l_ij == 0
            unsigned ie = line;
            if ( ind < m.L[line].indices.size()-1 )
                ie = m.L[line].indices[ind+1];
            while ( ++col < ie ) {
                T v = dot_chol_factorize( m.U[col], m.L[line] );
                if ( abs_indication( v ) ) {
                    unsigned os = m.L[line].indices.size();
                    m.L[line].indices.resize( os+1 );
                    m.L[line].data.resize( os+1 );
                    ++ind;
                    for(unsigned k=os;k>ind;--k) {
                        m.L[line].indices[k] = m.L[line].indices[k-1];
                        m.L[line].data[k] = m.L[line].data[k-1];
                    }
                    m.L[line].data[ind] = -v / m.U[col].data.back();
                    m.L[line].indices[ind] = col;
                }
            }
        }
        // U
        for(unsigned ind=0;ind<m.U[line].indices.size();++ind) {
            // u_ij != 0
            unsigned col = m.U[line].indices[ind];
            m.U[line].data[ind] = m.U[line].data[ind] - dot_chol_factorize( m.U[line], m.L[col] );
            // u_ij == 0
            unsigned ie = line;
            if ( ind < m.U[line].indices.size()-1 )
                ie = m.U[line].indices[ind+1];
            while ( ++col < ie ) {
                T v = dot_chol_factorize( m.U[line], m.L[col] );
                if ( abs_indication( v ) ) {
                    unsigned os = m.U[line].indices.size();
                    m.U[line].indices.resize( os+1 );
                    m.U[line].data.resize( os+1 );
                    ++ind;
                    for(unsigned k=os;k>ind;--k) {
                        m.U[line].indices[k] = m.U[line].indices[k-1];
                        m.U[line].data[k] = m.U[line].data[k-1];
                    }
                    m.U[line].data[ind] = -v;
                    m.U[line].indices[ind] = col;
                }
            }
        }
    }
}

/// m is assumed to be factorized
template<class T,int s,int s2> void solve_using_lu_factorize( const Mat<T,Gen<s>,SparseLU> &mat, const Vec<T> &sol, Vec<T,s2> &res ) {
    unsigned nb_lines = mat.nb_rows();
    //
    if ( res.size() <= nb_lines )
        res.resize( nb_lines, T(0) );
    //
    for(unsigned line=0;line<nb_lines;++line) {
        T v = sol[line];
        for(unsigned i=0;i<mat.L[line].data.size();++i)
            v -= mat.L[line].data[i] * res[ mat.L[line].indices[i] ];
        res[line] = v;
    }
    //
    Vec<T,s2> tmpvec = res;
    while (nb_lines--) {
        assert( mat.U[nb_lines].data.size() );
        T tmp = tmpvec[nb_lines] / mat.U[nb_lines].data.back();
        for(unsigned i=0;i<mat.U[nb_lines].data.size()-1;++i)
            tmpvec[ mat.U[nb_lines].indices[i] ] -= mat.U[nb_lines].data[i] * tmp;
        res[nb_lines] = tmp;
    }
}

/// in place...
template<class T> void incomplete_lu_factorize( Mat<T,Gen<>,SparseLU> &m ) {
    unsigned n = m.nb_rows();
    for(unsigned line=0;line<n;++line) {
        // L
        for(unsigned ind=0;ind<m.L[line].indices.size();++ind) {
            // l_ij != 0
            unsigned col = m.L[line].indices[ind];
            m.L[line].data[ind] = ( m.L[line].data[ind] - dot_chol_factorize( m.U[col], m.L[line] ) ) / m.U[col].data.back();
        }
        // U
        for(unsigned ind=0;ind<m.U[line].indices.size();++ind) {
            // u_ij != 0
            unsigned col = m.U[line].indices[ind];
            m.U[line].data[ind] = m.U[line].data[ind] - dot_chol_factorize( m.U[line], m.L[col] );
        }
    }
}

template<class T,int s2> void solve_using_incomplete_lu_factorize( const Mat<T,Gen<>,SparseLU> &mp, const Mat<T,Gen<>,SparseLU> &A, const Vec<T> &b, Vec<T,s2> &x, T crit = 1e-4 ) {
    assert(0); /// TODO
//     Vec<T> r, d, q, s;
//
//     r = b - A * x;
//     for(unsigned i=0;;++i) { if ( i==r.size() ) return; if ( abs(r[i]) > crit ) break; }
//     solve_using_lu_factorize( mp, r, d );
//
//     T deltn = dot(r,d);
//     unsigned cpt = 0;
//     while ( true ) {
//         T delto = deltn;
//
//         q = A * d;
//         T alpha = deltn / dot( d, q );
//         x += alpha * d;
//         r -= alpha * q; //r = b - A * x;
//         for(unsigned i=0;;++i) { if ( i==r.size() ) return; if ( abs(r[i]) > crit ) break; }
//
//         solve_using_lu_factorize( mp, r, s );
//         deltn = dot( r, s );
//
//         T beta = deltn / delto;
//         d = s + beta * d;
//         ++cpt;
//         //std::cout << max(abs(r)) << std::endl;
//     }
}

};

#endif // LMT_matinvsparse_HEADER

