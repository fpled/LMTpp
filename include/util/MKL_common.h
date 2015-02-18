#ifndef LMTPP_MKL_COMMON_H
#define LMTPP_MKL_COMMON_H

#ifdef WITH_MKL

#ifdef METIL_COMP_DIRECTIVE

#pragma inc_path /opt/intel/composer_xe_2015.1.133/mkl/include

#endif

#include <cmath>
#include <utility> /// struct pair
#include "mkl_types.h"

#include "mkl_rci.h"
#include "mkl_blas.h"
#include "mkl_spblas.h"
#include "mkl_service.h"

#endif

#include "containers/mat.h"
#include "containers/algo.h"  /// sort
#include "containers/matinvsparse.h"

#ifdef WITH_MKL

namespace LMT {

/*!
    on confond le type de stockage CSR et la matrice mais comme on ne gère qu'un seul type de stockage, cela ne pose pas de problème

*/
struct MKL_CSR_matrix {

    MKL_CSR_matrix() : ia( NULL ), ja( NULL ), a( NULL ), n( 0 ), is_one_based_indexing( 1 ) {
        for( unsigned i = 0; i < s_matdes; ++i ) 
            matdes[ i ] = '\0';
    }
    
    virtual ~MKL_CSR_matrix() {
        free();
    }
    
    template<class TM>
    void init( const TM &mat ) { /// type de matrice inconnue !
         assert( 0 );
    }
    
    template<int sr, int sc>
    void init( const Mat<double, Gen<sr,sc>, SparseLine<Col> > &mat ) {
    
        free();
    
        n = mat.nb_rows();
        unsigned nz = 0;
        for( int i = 0; i < n; ++i )
            nz += mat.data[ i ].indices.size();
        ia = new int[ n + 1 ];
        ja = new int[ nz ];
        a  = new double[ nz ];
        for( int i = 0, index = 0; i < n; ++i ) {
            for( int j = 0; j < mat.data[ i ].indices.size(); ++j, index++ ) {
                ja[ index ] = mat.data[ i ].indices[ j ] + is_one_based_indexing; /// + 1 -> convention Fortran
                a[ index ] = mat.data[ i ].data[ j ];
            }    
        }
        
        for( int i = 0, inz = 0; i < n; ++i ) {
            ia[ i ] = inz + is_one_based_indexing; /// + 1 -> convention Fortran
            inz += mat.data[ i ].indices.size();
        }
        
        ia[ n ] = nz + is_one_based_indexing; /// + 1 -> convention Fortran
    
        matdes[ 0 ] = 'g'; /// general
        matdes[ 1 ] = 'u'; /// upper
        matdes[ 2 ] = 'n'; /// non unit
        if ( is_one_based_indexing ) 
            matdes[ 3 ] = 'f'; /// fortran
        else
            matdes[ 3 ] = 'c'; /// c
            
        //         std::cout << " ia = " ;
        //         for( int i = 0; i <= n; ++i )
        //             std::cout << ia[ i ] << " ";
        //         std::cout << std::endl;
        //         std::cout << " ja = " ;
        //         for( int i = 0; i < nz; ++i )
        //             std::cout << ja[ i ] << " ";
        //         std::cout << std::endl;
        //         std::cout << " a = " ;
        //         for( int i = 0; i < nz; ++i )
        //             std::cout << a[ i ] << " ";
        //         std::cout << std::endl;  
    }
    
    template<int s>
    void init( const Mat<double, Sym<s,true>, SparseLine<Col> > &mat ) {
    
        free();
    
        n = mat.nb_rows();
        
        Vec<int> add_zero_in_diag;
        unsigned nz = 0;
        for( int i = 0; i < n; ++i ) {
            nz += mat.data[ i ].indices.size();
            if ( mat.data[ i ].indices.size() ) {
                if ( mat.data[ i ].indices[ 0 ] != i ) {
                    add_zero_in_diag.push_back( i );
                    nz++;
                }
            } else {
                add_zero_in_diag.push_back( i );
                nz++;            
            }
        }
        
        ia = new int[ n + 1 ];
        ja = new int[ nz ];
        a  = new double[ nz ];
        for( int i = 0, index = 0; i < n; ++i ) {
            for( int j = 0; j < mat.data[ i ].indices.size(); ++j, index++ ) {
                unsigned p = add_zero_in_diag.find( i );
                if ( p < add_zero_in_diag.size() ) {
                    ja[ index ] = i + is_one_based_indexing; /// + 1 -> convention Fortran
                    a[  index ] = 0;
                    add_zero_in_diag.erase_elem_nb( p );
                } else {
                    ja[ index ] = mat.data[ i ].indices[ j ] + is_one_based_indexing; /// + 1 -> convention Fortran
                    a[ index ] = mat.data[ i ].data[ j ];
                }
            }    
        }
        
        for( int i = 0, inz = 0; i < n; ++i ) {
            ia[ i ] = inz + is_one_based_indexing; /// + 1 -> convention Fortran
            inz += mat.data[ i ].indices.size();
        }
        
        ia[ n ] = nz + is_one_based_indexing; /// + 1 -> convention Fortran

        matdes[ 0 ] = 's'; /// symetric
        matdes[ 1 ] = 'u'; /// upper
        matdes[ 2 ] = 'n'; /// non unit
        if ( is_one_based_indexing ) 
            matdes[ 3 ] = 'f'; /// fortran
        else
            matdes[ 3 ] = 'c'; /// c
            
        //         std::cout << " ia = " ;
        //         for( int i = 0; i <= n; ++i )
        //             std::cout << ia[ i ] << " ";
        //         std::cout << std::endl;
        //         std::cout << " ja = " ;
        //         for( int i = 0; i < nz; ++i )
        //             std::cout << ja[ i ] << " ";
        //         std::cout << std::endl;
        //         std::cout << " a = " ;
        //         for( int i = 0; i < nz; ++i )
        //             std::cout << a[ i ] << " ";
        //         std::cout << std::endl;
    }
    
    /// Mat<TT,Sym<sr,0>,SparseLine<Col>,void>
    template<int s>
    void init(  const Mat<double, Sym<s,false>, SparseLine<Col> > &mat ) {    
    
        free();
            
        n = mat.nb_rows();        
        Vec<int> offsets, add_zero_in_diag; 
        offsets.resize( n, 0 );
        
        for( int i = 0; i < n; ++i ) {
            for( int j = 0; j < mat.data[ i ].indices.size(); ++j )
                ++offsets[ mat.data[ i ].indices[ j ] ];
            
            if ( mat.data[ i ].indices.size() ) {    
                if ( mat.data[ i ].indices.back() != i ) {
                    ++offsets[ i ];
                    add_zero_in_diag.push_back( i );
                }
            } else {
                ++offsets[ i ];
                add_zero_in_diag.push_back( i );
            }
        }
        
        int acc = 0;
        for( int i = 0; i < offsets.size(); ++i ) {
            int val = offsets[ i ];
            offsets[ i ] = acc;
            acc += val;
        }
    
        ia = new int[ n + 1 ];
        for( int i = 0; i < n; ++i )
            ia[ i ] = offsets[ i ] + 1;
        ia[ n ] = acc + is_one_based_indexing; /// + 1 -> convention Fortran
        
        ja = new int[ acc ];
        a  = new double[ acc ];
            
        for( int i = 0; i < offsets.size(); ++i ) {
            int d = add_zero_in_diag.find( i );
            if ( d  < add_zero_in_diag.size() ) {
                int pos = offsets[ i ]++;
                ja[ pos ] = i + is_one_based_indexing; /// + 1 -> convention Fortran
                a[ pos ] = 0;
                add_zero_in_diag.erase_elem_nb( d );            
            }
            for( int j = 0; j < mat.data[ i ].indices.size(); ++j ) {
                int pos = offsets[ mat.data[ i ].indices[ j ] ]++;
                ja[ pos ] = i + is_one_based_indexing; /// + 1 -> convention Fortran
                a[ pos ] = mat.data[ i ].data[ j ];
            }
        }
        
        matdes[ 0 ] = 's'; /// symetric
        matdes[ 1 ] = 'u'; /// upper
        matdes[ 2 ] = 'n'; /// non unit
        if ( is_one_based_indexing ) 
            matdes[ 3 ] = 'f'; /// fortran
        else
            matdes[ 3 ] = 'c'; /// c
            
        //         PRINT( offsets );
        //         
        //         std::cout << " ia = " ;
        //         for( int i = 0; i <= n; ++i )
        //             std::cout << ia[ i ] << " ";
        //         std::cout << std::endl;
        //         std::cout << " ja = " ;
        //         for( int i = 0; i < acc; ++i )
        //             std::cout << ja[ i ] << " ";
        //         std::cout << std::endl;
        //         std::cout << " a = " ;
        //         for( int i = 0; i < acc; ++i )
        //             std::cout << a[ i ] << " ";
        //         std::cout << std::endl;
    }    
    
    template<class TM>
    MKL_CSR_matrix( const TM &mat ) : ia( NULL ), ja( NULL ), a( NULL ), n( 0 ), is_one_based_indexing( 1 ) { init( mat ); }
    
    /*! 
        renvoie le type de la matrice pour le solver PARDISO
        La structure matdes ne peut contenir l'information : matrice symmétrique définie positive. Attention. 
    */
    int get_mtype() const {
        
        char c = matdes[ 0 ];
        if ( c == 'g' ) {
            return 11;  /// matrice quelconque réelle
        } else {
            if ( c == 's' )
                return -2; /// matrice symmétrique
            else
                return 11; /// retour par défaut
        }
    }

    template<int s>
    Vec<double,s> operator*( const Vec<double, s> &x ) const {

        assert( x.size() == n );
        
        Vec<double, s> res;
        res.resize( x.size() );
        if ( matdes[ 0 ] == 'g' ) {
            char transa = 'n';
            mkl_dcsrgemv( const_cast<char*>( &transa ), 
                          const_cast<int*>( &n ), 
                          const_cast<double*>( a ), 
                          const_cast<int*>( ia ), 
                          const_cast<int*>( ja ), 
                          const_cast<double*>( x.ptr() ), 
                          res.ptr() );
        } else {
            if ( matdes[ 0 ] == 's' )
                mkl_dcsrsymv( const_cast<char*>( &matdes[ 1 ] ),
                              const_cast<int*>(&n ), 
                              const_cast<double*>( a ), 
                              const_cast<int*>( ia ),
                              const_cast<int*>( ja ),
                              const_cast<double*>( x.ptr() ),
                              res.ptr() );
            else {
                res.set( 0 );
                std::cerr << "Error : MKL_BSR_matrix * Vec " << std::endl;
            }
        }
        return res;
    }

    /// applique la matrice vue comme un conditionneur
    /// par défaut je ne fais rien
    virtual void apply_as_conditioner( double *x, double *y ) const {}
    
    void free() {
        if ( n ) {
            delete[] ia;
            delete[] ja;
            delete[] a;
            n = 0;
        }
    }

    int *ia;
    int *ja;
    double *a;
    static const unsigned s_matdes = 6;
    char matdes[ s_matdes ];
    int n; /// nb of rows of the matrix
    int is_one_based_indexing; /// 0 <-> faux   ,   1 <-> vrai
    /// sur certaines versions de la MKL, l'indéxation C ne marche pas donc on reste toujours en indexation Fortran
    /// si on change d'avis, les fonctions MKL seront préfixées par mkl_cspblas au lieu mkl_  ( c.f. page 204 de la doc de MKL )
};

template<class T>
void symetrise( T* a, int is_one_based_indexing_, int K_ ) {

    if ( is_one_based_indexing_ ) {
        /// stockage Fortran en colonne
        for( int i = 1; i < K_; ++i )
            for( int j = 0; j < i; ++j ) {
                a[ i * K_ + j ] = a[ j * K_ + i ];
            }
            
    } else {
        /// stockage en ligne comme en C++
        for( int i = 0; i < K_ - 1; ++i )
            for( int j = i + 1; j < K_; ++j ) {
                a[ i * K_ + j ] = a[ j * K_ + i ];
            } 
    }
}

/// on lower
struct MKL_BSR_matrix {

    MKL_BSR_matrix() : ia( NULL ), ja( NULL ), a( NULL ), K( 0 ), n( 0 ), is_one_based_indexing( 1 ) {
        for( unsigned i = 0; i < s_matdes; ++i ) 
            matdes[ i ] = '\0';
    }
    
    virtual ~MKL_BSR_matrix() {
        free();
    }
    
    template<class TM>
    void init( const TM &mat, int p_size_block ) { /// type de matrice inconnue !
         assert( 0 );
    }
    
    template<class Stru >
    void init_gen_sym_lower_col( const Mat< double, Stru, SparseLine<Col> > & mat, int p_size_block ) {
    
        free();
    
        if ( mat.nb_rows() % p_size_block ) 
            assert( 0 );
        
        K = p_size_block;
        n = mat.nb_rows() / p_size_block;
        
        ia = new int[ n + 1 ];
        ia[ 0 ] = 0;
        
        /// détermination du nombre de blocks
        Vec<int> indice;
        Vec< Vec<int> > col_list; col_list.resize( n );

        for( int i = 0, r = 0; i < mat.nb_rows(); i += K, ++r ) {
            /// travail sur un paquet de K lignes
            indice.resize( 0 );
            for( int i2 = i; i2 < i + K; i2++ )
                for( int ind = 0; ind < mat.data[ i2 ].indices.size(); ++ind ) {
                    int c = mat.data[ i2 ].indices[ ind ] / K;
                    if ( indice.find( c ) == indice.size() )
                        indice.push_back( c );
                }
            ia[ r + 1 ] = ia[ r ] + indice.size();
            sort( indice );
            col_list[ r ] = indice;
            
        }
        
        a  = new double[ K * K * ia[ n ] ];
        ja = new int[ ia[ n ] ]; 
        
        memset( ( void* ) a, 0, sizeof( double ) * K * K * ia[ n ] );
        
        /// initialisation de ja
        for( int i = 0, l = 0; i < n; i++ )
            for( int j = 0; j < col_list[ i ].size(); j++, l++ )
                ja[ l ] = col_list[ i ][ j ];

        //for( int i = 0, l = 0; i < n; i++ )
        //    PRINT( col_list[ i ] );

        /// remplissage des éléments non nuls de a
        /// pour la version avec indéxation commençant à 1 ( i.e. indéxation Fortran ), les éléments d'un block sont rangés par colonne
        if ( is_one_based_indexing ) {
            for( int i = 0, r = 0; i < mat.nb_rows(); i += K, r++ ) {
                /// travail sur un paquet de K lignes
                for( int i2 = i; i2 < i + K; i2++ )
                    for( int ind = 0; ind < mat.data[ i2 ].indices.size(); ++ind ) {
                        int q = mat.data[ i2 ].indices[ ind ] / K;
                        int j = mat.data[ i2 ].indices[ ind ] - q * K;
                        int jb = ia[ r ];
                        for( ; jb < ia[ r + 1 ]; ++ jb )
                            if ( ja[ jb ] == q ) {
                                a[ jb * K * K + j * K + i2 - i ] = mat.data[ i2 ].data[ ind ];
                                break;
                            }
                        if ( jb == ia[ r + 1 ] )
                            std::cerr << "Error MKL_BSR_matrix::init() : impossible to put the number" <<std::endl;
                    }
            }
        } else {
            for( int i = 0, r = 0; i < mat.nb_rows(); i += K, r++ ) {
                /// travail sur un paquet de K lignes
                for( int i2 = i; i2 < i + K; i2++ )
                    for( int ind = 0; ind < mat.data[ i2 ].indices.size(); ++ind ) {
                        int q = mat.data[ i2 ].indices[ ind ] / K;
                        int j = mat.data[ i2 ].indices[ ind ] - q * K;
                        int jb = ia[ r ];
                        for( ; jb < ia[ r + 1 ]; ++ jb )
                            if ( ja[ jb ] == q ) {
                                a[ jb * K * K + ( i2 - i ) * K + j ] = mat.data[ i2 ].data[ ind ];
                                break;
                            }
                        if ( jb == ia[ r + 1 ] )
                            std::cerr << "Error MKL_BSR_matrix::init() : impossible to put the number" <<std::endl;
                    }
            }
        }       
             
    }
    
    template<int sr, int sc>
    void init( const Mat<double, Gen<sr,sc>, SparseLine<Col> > &mat, int p_size_block ) {
        
        init_gen_sym_lower_col( mat, p_size_block );
        
//         std::cout << "********* ia = " << std::endl;
//         for( int i = 0; i <= n; ++i )
//             std::cout << ia[ i ] << " ";
//         std::cout << std::endl;
//         std::cout << "********* ja = " << std::endl;
//         for( int i = 0; i < ia[ n ]; ++i )
//             std::cout << ja[ i ] << " ";
//         std::cout << std::endl;
//         std::cout << "********* a = " << std::endl;
//         for( int i = 0; i < ia[ n ] * K * K; ++i )
//             std::cout  << a[ i ] << " ";
//         std::cout << std::endl; 
        
        if ( is_one_based_indexing ) {
            for( int i = 0; i <= n; ++i )
                ia[ i ] += 1;
            for( int i = 0; i < ia[ n ]; ++i )
                ja[ i ] += 1;                
        }
        
        matdes[ 0 ] = 'g'; /// general
        matdes[ 1 ] = 'l'; /// lower
        matdes[ 2 ] = 'n'; /// non unit
        if ( is_one_based_indexing ) 
            matdes[ 3 ] = 'f'; /// fortran
        else
            matdes[ 3 ] = 'c'; /// c 
    }
    
    /// Mat<TT,Sym<sr,0>,SparseLine<Col>,void>
    template<int s>
    void init(  const Mat<double, Sym<s,false>, SparseLine<Col> > &mat, int p_size_block ) { 
       
        init_gen_sym_lower_col( mat, p_size_block );
        
        /// pour les matrices symétriques, il y a un souci car les blocks diagonaux sont mal remplis.
        /// il s'agit de les "symétriser"
        for( int i = 0; i < n; ++i )
            if ( ja[ ia[ i + 1 ] - 1 ] == i ) /// le dernier block est un block diagonal
                symetrise( a + ( ia[ i + 1 ] - 1 ) * K * K, is_one_based_indexing, K );
        
/*        std::cout << "********* ia = " << std::endl;
        for( int i = 0; i <= n; ++i )
            std::cout << ia[ i ] << " ";
        std::cout << std::endl;
        std::cout << "********* ja = " << std::endl;
        for( int i = 0; i < ia[ n ]; ++i )
            std::cout << ja[ i ] << " ";
        std::cout << std::endl;
        std::cout << "********* a = " << std::endl;
        for( int i = 0; i < ia[ n ] * K * K; ++i )
            std::cout  << a[ i ] << " ";
        std::cout << std::endl;*/ 
        
        if ( is_one_based_indexing ) {
            for( int i = 0; i <= n; ++i )
                ia[ i ] += 1;
            for( int i = 0; i < ia[ n ]; ++i )
                ja[ i ] += 1;                
        }
        
        matdes[ 0 ] = 's'; /// general
        matdes[ 1 ] = 'l'; /// lower
        matdes[ 2 ] = 'n'; /// non unit
        if ( is_one_based_indexing ) 
            matdes[ 3 ] = 'f'; /// fortran
        else
            matdes[ 3 ] = 'c'; /// c        
    }    
    
    template<class TM>
    MKL_BSR_matrix( const TM &mat, int p_size_block ) : ia( NULL ), ja( NULL ), a( NULL ), K( 0 ), n( 0 ), is_one_based_indexing( 1 ) { 
        init( mat, p_size_block );
    }
    
    /*! 
        renvoie le type de la matrice pour le solver PARDISO
        La structure matdes ne peut contenir l'information : matrice symmétrique définie positive. Attention. 
    */
    int get_mtype() const {
        
        char c = matdes[ 0 ];
        if ( c == 'g' ) {
            return 11;  /// matrice quelconque réelle
        } else {
            if ( c == 's' )
                return -2; /// matrice symmétrique
            else
                return 11; /// retour par défaut
        }
    }
    
    template<int s>
    Vec<double,s> operator*( const Vec<double, s> &x ) const {

        assert( x.size() == n * K );
        
        Vec<double, s> res;
        res.resize( x.size() );
        if ( matdes[ 0 ] == 'g' ) {
            char transa = 'n';
            mkl_dbsrgemv( const_cast<char*>( &transa ), 
                          const_cast<int*>( &n ), 
                          const_cast<int*>( &K ), 
                          const_cast<double*>( a ), 
                          const_cast<int*>( ia ), 
                          const_cast<int*>( ja ), 
                          const_cast<double*>( x.ptr() ), 
                          res.ptr() );
        } else {
            if ( matdes[ 0 ] == 's' )
                mkl_dbsrsymv( const_cast<char*>( &matdes[ 1 ] ),
                              const_cast<int*>(&n ), 
                              const_cast<int*>( &K ),
                              const_cast<double*>( a ), 
                              const_cast<int*>( ia ),
                              const_cast<int*>( ja ),
                              const_cast<double*>( x.ptr() ),
                              res.ptr() );
            else {
                res.set( 0 );
                std::cerr << "Error : MKL_BSR_matrix * Vec " << std::endl;
            }
        }
        return res;
    }
    
    /// applique la matrice vue comme un conditionneur
    /// par défaut je ne fais rien
    virtual void apply_as_conditioner( double *x, double *y ) const {}
    
    void free() {
        if ( n ) {
            delete[] ia;
            delete[] ja;
            delete[] a;
            n = 0;
            K = 0;
        }
    }

    int *ia;
    int *ja;
    double *a;
    static const unsigned s_matdes = 6;
    char matdes[ s_matdes ];
    int K; /// nb of rows in a block
    int n; /// nb of blocks in a row of matrix
    int is_one_based_indexing; /// 0 <-> faux   ,   1 <-> vrai
    /// sur certaines versions de la MKL, l'indéxation C ne marche pas donc on reste toujours en indexation Fortran
    /// si on change d'avis, les fonctions MKL seront préfixées par mkl_cspblas au lieu mkl_  ( c.f. page 204 de la doc de MKL )
};

/*!
    Ce cas est très particulier. On stocke dans a l'inverse de la diagonale de la matrice passée en paramètre

*/
struct MKL_Jacobi_matrix : public MKL_CSR_matrix {

    template<class TM>
    int init( const TM &mat ) {
         
        n  = mat.nb_rows();
//         PRINT( n );
//         PRINT( mat );
        a  = new double[ n ];
        for( int i = 0; i < n; ++i ) { 
            double t = mat( i, i );
            if ( t ) 
                a[ i ] = 1 / t;
            else {
                a[ i ] = 0;
                std::cerr << "WARNING MKL_Jacobi_matrix.init() : element " << i << " of diagonal is zero." << std::endl;
            }
        }
        ia = NULL;
        ja = NULL;
        is_one_based_indexing = true;
        matdes[ 0 ] = 'd'; /// diagonal
        matdes[ 1 ] = 'u'; /// upper
        matdes[ 2 ] = 'n'; /// non unit
        if ( is_one_based_indexing ) 
            matdes[ 3 ] = 'f'; /// fortran
        else
            matdes[ 3 ] = 'c'; /// c
         
         return get_mtype();
    }

    template<class TM>
    MKL_Jacobi_matrix( const TM &mat ) { init( mat ); }
    
    virtual void apply_as_conditioner( double *x, double *y ) const {
        for( int i = 0; i < n; ++i )
            y[ i ] = a[ i ] * x[ i ];
    }
};

struct MKL_incomplete_chol_matrix : public MKL_CSR_matrix {

    template<class TM>
    MKL_incomplete_chol_matrix( const TM &mat ) {
        TM K = mat;
        //PRINTN( K );
        incomplete_chol_factorize( K, false );
        //PRINTN( K );
        MKL_CSR_matrix::init( K );
        sol = new double[ n ]; 
    }
    
    virtual ~MKL_incomplete_chol_matrix() {
        delete[] sol;
    }

    virtual void apply_as_conditioner( double *x, double *y ) const {
        //char tr = 'u'; /// upper storage
        //mkl_dcsrsymv( &tr, const_cast<int*>( &n ), a, ia, ja, x, y ); /// perform y <- Sx where S is symetric
        char uplo = 'u'; /// upper
        char transa = 't'; /// A^T x = b 
        char diag = 'n'; /// diagonal is not unit
        /// performs solving triangular system
        mkl_dcsrtrsv( &uplo, &transa, &diag, const_cast<int*>( &n ), a, ia, ja, x, sol );
        transa = 'n'; /// A x = b 
        mkl_dcsrtrsv( &uplo, &transa, &diag, const_cast<int*>( &n ), a, ia, ja, sol, y );
    }
    
    double *sol;
};

struct MKL_SSOR_matrix : public MKL_CSR_matrix {

    template<class TM>
    int init( const TM &mat ) {
         
        n  = mat.nb_rows();
        a   = new double[ n ];
        sol = new double[ n ]; /// solution intermédiaire lors de la descente-remontée
        for( int i = 0; i < n; ++i ) 
            a[ i ] = mat( i, i );
        ia = NULL;
        ja = NULL;
        is_one_based_indexing = true;
        matdes[ 0 ] = 'd'; /// diagonal
        matdes[ 1 ] = 'u'; /// upper
        matdes[ 2 ] = 'n'; /// non unit
        if ( is_one_based_indexing ) 
            matdes[ 3 ] = 'f'; /// fortran
        else
            matdes[ 3 ] = 'c'; /// c
         
         return get_mtype();
    }

    template<class TM>
    MKL_SSOR_matrix( const TM &mat, MKL_CSR_matrix &m_0 ) {
        init( mat );
        
        m = &m_0; 
    }
    
    virtual ~MKL_SSOR_matrix() {
        delete [] sol;
    }

    virtual void apply_as_conditioner( double *x, double *y ) const {
    
        char transa = 't'; /// solve the transposed system A^T x = y
        char matdescra[ 6 ];
        matdescra[ 0 ] = 't'; /// triangular system
        matdescra[ 1 ] = 'u'; /// upper
        matdescra[ 2 ] = 'n'; /// diagonale normale de la matrice
        double alpha = 1;
        /// performs solving triangular system
        mkl_dcsrsv( &transa, &(m->n), &alpha, matdescra, m->a, m->ja, m->ia, ( m->ia ) + 1, x, sol );
        
        for( int i = 0; i < n; ++i )
            sol[ i ] = a[ i ] * sol[ i ];
        
        transa = 'n'; /// A x = b 
        /// performs solving triangular system
        mkl_dcsrsv( &transa, &(m->n), &alpha, matdescra, m->a, m->ja, m->ia, ( m->ia ) + 1, sol, y );        
    }
    
    MKL_CSR_matrix *m; /// matrice du système
    double *sol;
};


struct MKL_norm2_residual_stopping {
    MKL_norm2_residual_stopping( double epsilon ) : eps( epsilon ) {}
    double eps;
};

struct MKL_norm_inf_residual_stopping {
    MKL_norm_inf_residual_stopping( double epsilon ) : eps( epsilon ) {}
    double eps;
};

};

#endif /// ifdef WITH_MKL

#endif /// LMTPP_MKL_COMMON_H

