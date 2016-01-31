#ifndef EIG_LAPACK_H
#define EIG_LAPACK_H

#include "mat.h"

#ifdef METIL_COMP_DIRECTIVE
#pragma lib_name lapack
#endif

extern "C" {
    int dsyev_(
    char *jobz, char *uplo, int *n, double *a,
    int *lda, double *w, double *work, int *lwork,
    int *info );
    int dsygv_(
    int *itype, char *jobz, char *uplo, int *n, double *a, int *lda, double *b, int *ldb, 
    double *w, double *work, int *lwork,
    int *info );
}

namespace LMT {

/*!
    Get a vector (eig_val) of eigen values and, optionally, a full matrix (eig_vec) whose rows are the corresponding eigen vectors of a real symetric matrix (A) using lapack so that the real symmetric eigen problem is A * trans(eig_vec) = trans(eig_vec) * diag(eig_val).
    A is assumed to be symmetric and will be converted to a dense matrix (compatible with lapack).
    you will need to add gfortran and lapack in libraries.
    eig_val[ i ] to get the i-th eigen value.
    eig_vec.row( i ) to get the i-th eigen vector.

    Example:
    \code C/C++
        #include "containers/eig_lapack.h"

        int main ( int argc, char **argv ) {
            typedef double T;

            Mat<T, Sym<> > Ms( 3, 3, 0. );
            Ms.diag() = 5.;
            Ms( 1, 0 ) = 2.;
            Ms( 2, 0 ) = 1.;
            Ms( 2, 1 ) = -0.3;

            Mat<T> A( Ms );
            PRINTN( A );

            Vec<T> eig_val;
            get_eig_val_sym( A, eig_val );
            PRINT( eig_val );

            Mat<T> eig_vec;
            get_eig_sym( A, eig_val, eig_vec );
            PRINT( eig_val );
            PRINTN( eig_vec );

            PRINTN( eig_vec.row( 0 ) );
            PRINTN( A * eig_vec.row( 0 ) - eig_val[ 0 ] * eig_vec.row( 0 ) );
            PRINTN( A * trans( eig_vec ) - trans( eig_vec ) * diag( eig_val ) );
            PRINTN( A - trans( eig_vec ) * diag( eig_val ) * eig_vec );

            return 0.;
        }

    On créé une matrice Mat<T,Sym<> > Ms pour la définition puis on la convertit en une matrice dense Mat<T> M en faisant par exemple :
    \code C/C++
        Mat<T> M( Ms );

    \keyword Mathématiques/Algèbre linéaire/Vecteurs et valeurs propres

    \author Hugo Leclerc
*/
template<class TM>
void get_eig_val_sym( const TM &m, Vec<double> &eig_val ) {
    char jobz = 'N'; // Compute eigenvalues only.
    char uplo = 'L'; // Lower triangle of m is stored.
    int n = m.nb_rows();
    Vec<double> A; A.resize( n * n );
    for(unsigned j=0,c=0;j<m.nb_cols();++j)
        for(unsigned i=0;i<m.nb_rows();++i,++c)
            A[ c ] = m( i, j );
    eig_val.resize( n );
    int lwork = 3*n-1;
    Vec<double> work; work.resize( lwork );
    int info;
    dsyev_( &jobz, &uplo, &n, A.ptr(), &n, eig_val.ptr(), work.ptr(), &lwork, &info );
    if ( info ) {
        std::cout << "info =  " << info << std::endl;
        throw "pb with eig sym using lapack";
    }
}

template<class TM>
void get_eig_sym( const TM &A, Vec<double> &eig_val, Mat<double> &eig_vec ) {
    char jobz = 'V'; // Compute eigenvalues and eigenvectors.
    char uplo = 'L'; // Lower triangle of m is stored.
    int n = A.nb_rows();
    Vec<double> A_data; A_data.resize( n * n );
    for(unsigned j=0,c=0;j<A.nb_cols();++j)
        for(unsigned i=0;i<A.nb_rows();++i,++c)
            A_data[ c ] = A( i, j );
    eig_val.resize( n );
    int lwork = 3*n-1;
    Vec<double> work; work.resize( lwork );
    int info;
    dsyev_( &jobz, &uplo, &n, A_data.ptr(), &n, eig_val.ptr(), work.ptr(), &lwork, &info );
    if ( info ) {
        std::cout << "info =  " << info << std::endl;
        throw "pb with eig sym using lapack";
    }
    //
    eig_vec.resize( n, n );
    for(int i=0,c=0;i<n;++i)
        for(int j=0;j<n;++j,++c)
            eig_vec( i, j ) = A_data[ c ];
}

template<class TM,class T>
Vec<T> solve_using_eig_sym( const TM &A, const Vec<T> &v, T rm_eig_if_inf_rel = 1e-5 ) {
    Vec<T> eig_val;
    Mat<T> eig_vec;
    get_eig_sym( A, eig_val, eig_vec );

    Vec<T> valid = ( abs( eig_val ) > rm_eig_if_inf_rel * norm_inf( eig_val ) );
    Mat<T, Sym<>, SparseLine<> > t( A.nb_rows() );
    t.diag() = valid / ( eig_val + 1 - valid );
    return trans( eig_vec ) * Vec<T>( t * Vec<T>( eig_vec * v ) );
}

/*!
    Get a vector (eig_val) of generalized eigen values and, optionally, a full matrix (eig_vec) whose rows are the corresponding eigen vectors of a real generalized symmetric-definite eigenproblem of the form A * trans(eig_vec) = B * trans(eig_vec) * diag(eig_val) using lapack.
    A and B are assumed to be symmetric and B is also positive definite.
    you will need to add gfortran and lapack in libraries.
    eig_val[ i ] to get the i-th generalized eigen value.
    eig_vec.row( i ) to get the i-th generalized eigen vector.

    Example:
    \code C/C++
        #include "containers/eig_lapack.h"

        int main ( int argc, char **argv ) {
            typedef double T;

            Mat<T, Sym<> > Ms( 3, 3, 0. );
            Ms.diag() = 5.;
            Ms( 1, 0 ) = 2.;
            Ms( 2, 0 ) = 1.;
            Ms( 2, 1 ) = -0.3;

            Mat<T> A( Ms );
            PRINTN( A );

            Ms.set( 0. );
            Ms.diag() = 1.;
            Ms( 1, 1 ) = 2.;
            Mat<T> B( Ms );
            PRINTN( B );

            get_eig_val_gen( A, B, eig_val );
            PRINT( eig_val );

            get_eig_gen( A, B, eig_val, eig_vec );
            PRINT( eig_val );
            PRINTN( eig_vec );

            PRINTN( eig_vec.row( 0 ) );
            PRINTN( A * eig_vec.row( 0 ) - eig_val[ 0 ] * B * eig_vec.row( 0 ) );
            PRINTN( A * trans( eig_vec ) - B * trans( eig_vec ) * diag( eig_val ) );

            return 0.;
        }

    \keyword Mathématiques/Algèbre linéaire/Vecteurs et valeurs propres

    \author Hugo Leclerc
*/
template<class TM1, class TM2> void get_eig_val_gen( const TM1 &A, const TM2 &B, Vec<double> &eig_val ) {
    char jobz = 'N'; // Compute eigenvalues only.
    char uplo = 'L'; // Lower triangles of A and B are stored.
    int n = A.nb_rows();
    Vec<double> A_data; A_data.resize( n * n );
    for(unsigned j=0,c=0;j<A.nb_cols();++j)
        for(unsigned i=0;i<A.nb_rows();++i,++c)
            A_data[ c ] = A( i, j );
    Vec<double> B_data; B_data.resize( n * n );
    for(unsigned j=0,c=0;j<B.nb_cols();++j)
        for(unsigned i=0;i<B.nb_rows();++i,++c)
            B_data[ c ] = B( i, j );
    eig_val.resize( n );
    int lwork = 3*n-1;
    Vec<double> work; work.resize(lwork);
    int info;
    int itype = 1; // Problem type to be solved:
                   // = 1:  A*(eig_vec) = (eig_val)*B*(eig_vec)
                   // = 2:  A*B*(eig_vec) = (eig_val)*(eig_vec)
                   // = 3:  B*A*(eig_vec) = (eig_val)*(eig_vec)
    dsygv_( &itype, &jobz, &uplo, &n, A_data.ptr(), &n, B_data.ptr(), &n, eig_val.ptr(), work.ptr(), &lwork, &info );
    if ( info ) {
        std::cout << "info =  " << info << std::endl;
        throw "pb with eig gen using lapack";
    }
}

template<class TM1, class TM2> void get_eig_gen( const TM1 &A, const TM2 &B, Vec<double> &eig_val, Mat<double> &eig_vec ) {
    char jobz = 'V'; // Compute eigenvalues and eigenvectors.
    char uplo = 'L'; // Lower triangles of A and B are stored.
    int n = A.nb_rows();
    Vec<double> A_data; A_data.resize( n * n );
    for(unsigned j=0,c=0;j<A.nb_cols();++j)
        for(unsigned i=0;i<A.nb_rows();++i,++c)
            A_data[ c ] = A( i, j );
    Vec<double> B_data; B_data.resize( n * n );
    for(unsigned j=0,c=0;j<B.nb_cols();++j)
        for(unsigned i=0;i<B.nb_rows();++i,++c)
            B_data[ c ] = B( i, j );
    eig_val.resize( n );
    int lwork = 3*n-1;
    Vec<double> work; work.resize(lwork);
    int info;
    int itype = 1; // Problem type to be solved:
                   // = 1:  A*(eig_vec) = (eig_val)*B*(eig_vec)
                   // = 2:  A*B*(eig_vec) = (eig_val)*(eig_vec)
                   // = 3:  B*A*(eig_vec) = (eig_val)*(eig_vec)
    dsygv_( &itype, &jobz, &uplo, &n, A_data.ptr(), &n, B_data.ptr(), &n, eig_val.ptr(), work.ptr(), &lwork, &info );
    if ( info ) {
        std::cout << "info =  " << info << std::endl;
        throw "pb with eig gen using lapack";
    }
    //
    eig_vec.resize( n, n );
    for(int i=0,c=0;i<n;++i)
        for(int j=0;j<n;++j,++c)
            eig_vec( i, j ) = A_data[ c ];
}

}
#endif // EIG_LAPACK_H

