#ifndef eigen_problem_using_lapack_h
#define eigen_problem_using_lapack_h

#include <containers/mat.h>

extern "C" {
    void dsyev_(const char &JOBZ, const char &UPLO, const int &N, double* A, const int &LDA, double* W, double* WORK, const int &LWORK, int &INFO);
    void dsygv_(const int &ITYPE, const char &JOBZ, const char &UPLO, const int &N, double* A, const int &LDA, double* B, const int &LDB, double* W, double* WORK, const int &LWORK, int &INFO);
}

/*!
    Get a vector (D) of eigen values and, optionally, a full matrix (V) whose columns are the corresponding eigen vectors of a real symetric matrix (m) using lapack so that the real symmetric eigen problem is A * V = V * diag(D).
    A is assumed to be symmetric and will be converted to a dense matrix (compatible with lapack).
    you will need to add gfortran and lapack in libraries.
    D[ i ] to get the i-th eigen value.
    V.col( i ) to get the i-th eigen vector.

    Example:
    \code C/C++
        #include "LMT/include/containers/eigen_problem_using_lapack.h"

        int main ( int argc, char **argv ) {
            typedef double T;

            Mat<T, Sym<> > Ms( 3, 3, 0. );
            Ms.diag() = 5.;
            Ms( 1, 0 ) = 2.;
            Ms( 2, 0 ) = 1.;
            Ms( 2, 1 ) = -0.3;

            Mat<T> A( Ms );
            PRINTN( A );

            Vec<T> D;
            eigen_values_using_lapack( A, D );
            PRINT( D );

            Mat<T> V;
            eigen_problem_using_lapack( A, D, V );
            PRINT( D );
            PRINTN( V );

            PRINTN( V.col( 0 ) );
            PRINTN( A * V.col( 0 ) - D[ 0 ] * V.col( 0 ) );
            PRINTN( A * V - trans( V ) * diag( D ) );
            PRINTN( A - V * diag( D ) * trans( V ) );

            return 0.;
        }

    On créé une matrice Mat<T,Sym<> > Ms pour la définition puis on la convertit en une matrice dense Mat<T> M en faisant par exemple :
    \code C/C++
        Mat<T> M( Ms );

    \keyword Mathématiques/Algèbre linéaire/Vecteurs et valeurs propres

    \author Hugo Leclerc
*/
template<class MatType, class VecType> void eigen_values_using_lapack(const MatType &A, VecType &D)
{
    unsigned dim = A.nb_cols();
//     PRINT(dim);

    LMT::Vec<double> A_data;
    A_data.resize(dim*dim);
    unsigned A_cpt = 0;
    for(unsigned j=0; j<dim; j++)
    {
        for(unsigned i=0; i<dim; i++)
        {
            A_data[A_cpt++] = A(i, j);
        }
    }
//     PRINT(A);
//     PRINT(A_data);

    D.resize(dim);

    unsigned lwork = 3*dim-1;
//     PRINT(lwork);

    LMT::Vec<double> work;
    work.resize(lwork);
//     PRINT(work);

    int info = 0;
//     PRINT(info);

    dsyev_('N', 'U', dim, A_data.ptr(), dim, D.ptr(), work.ptr(), lwork, info);
//     PRINT(info);
//     PRINT(A_data);
//     PRINT(D);
}

template<class MatType1, class VecType, class MatType2> void eigen_problem_using_lapack(const MatType1 &A, VecType &D, MatType2 &V)
{
    unsigned dim = A.nb_cols();
//     PRINT(dim);

    LMT::Vec<double> A_data;
    A_data.resize(dim*dim);
    unsigned A_cpt = 0;
    for(unsigned j=0; j<dim; j++)
    {
        for(unsigned i=0; i<dim; i++)
        {
            A_data[A_cpt++] = A(i, j);
        }
    }
//     PRINT(A);
//     PRINT(A_data);

    D.resize(dim);

    unsigned lwork = 3*dim-1;
//     PRINT(lwork);

    LMT::Vec<double> work;
    work.resize(lwork);
    work.set(0.);
//     PRINT(work);

    int info = 0;
//     PRINT(info);

    dsyev_('V', 'U', dim, A_data.ptr(), dim, D.ptr(), work.ptr(), lwork, info);
//     PRINT(info);
//     PRINT(A_data);
//     PRINT(D);

    V.resize(dim,dim);

    unsigned V_cpt = 0;
    for(unsigned j=0; j<dim; j++)
    {
        for(unsigned i=0; i<dim; i++)
        {
            V(i, j) = A_data[V_cpt++];
        }
    }
//     PRINT(V);
}

/*!
    Get a vector (D) of generalized eigen values and, optionally, a full matrix (V) whose columns are the corresponding eigen vectors of a real generalized symmetric-definite eigenproblem of the form A * V = B * V * diag(D) using lapack.
    A and B are assumed to be symmetric and B is also positive definite.
    you will need to add gfortran and lapack in libraries.
    D[ i ] to get the i-th generalized eigen value.
    V.col( i ) to get the i-th generalized eigen vector.

    Example:
    \code C/C++
        #include "LMT/include/containers/eigen_problem_using_lapack.h"

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

            generalized_eigen_values_using_lapack( A, B, D );
            PRINT( D );

            generalized_eigen_problem_using_lapack( A, B, D, V );
            PRINT( D );
            PRINTN( V );

            PRINTN( V.col( 0 ) );
            PRINTN( A * V.col( 0 ) - D[ 0 ] * B * V.col( 0 ) );
            PRINTN( A * V - B * V * diag( D ) );

            return 0.;
        }

    \keyword Mathématiques/Algèbre linéaire/Vecteurs et valeurs propres
    \author Hugo Leclerc
*/
template<class MatType1, class MatType2, class VecType> void generalized_eigen_values_using_lapack(const MatType1 &A, const MatType2 &B, VecType &V)
{
    unsigned dim = A.nb_cols();
//     PRINT(dim);

    LMT::Vec<double> A_data;
    A_data.resize(dim*dim);
    unsigned A_cpt = 0;
    for(unsigned j=0; j<dim; j++)
    {
        for(unsigned i=0; i<dim; i++)
        {
            A_data[A_cpt++] = A(i, j);
        }
    }
//     PRINT(A);
//     PRINT(A_data);

    LMT::Vec<double> B_data;
    B_data.resize(dim*dim);
    unsigned B_cpt = 0;
    for(unsigned j=0; j<dim; j++)
    {
        for(unsigned i=0; i<dim; i++)
        {
            B_data[B_cpt++] = B(i, j);
        }
    }
//     PRINT(B);
//     PRINT(B_data);

    V.resize(dim);

    unsigned lwork = 3*dim-1;
//     PRINT(lwork);

    LMT::Vec<double> work;
    work.resize(lwork);
//     PRINT(work);

    int info = 0;
//     PRINT(info);

    dsygv_(1, 'N', 'U', dim, A_data.ptr(), dim, B_data.ptr(), dim, V.ptr(), work.ptr(), lwork, info);
//     PRINT(info);
//     PRINT(A_data);
//     PRINT(B_data);
//     PRINT(V);
}

template<class MatType1, class MatType2, class VecType, class MatType3> void generalized_eigen_problem_using_lapack(const MatType1 &A, const MatType2 &B, VecType &V, MatType3 &P)
{
    unsigned dim = A.nb_cols();
//     PRINT(dim);

    LMT::Vec<double> A_data;
    A_data.resize(dim*dim);
    unsigned A_cpt = 0;
    for(unsigned j=0; j<dim; j++)
    {
        for(unsigned i=0; i<dim; i++)
        {
            A_data[A_cpt++] = A(i, j);
        }
    }
//     PRINT(A);
//     PRINT(A_data);

    LMT::Vec<double> B_data;
    B_data.resize(dim*dim);
    unsigned B_cpt = 0;
    for(unsigned j=0; j<dim; j++)
    {
        for(unsigned i=0; i<dim; i++)
        {
            B_data[B_cpt++] = B(i, j);
        }
    }
//     PRINT(B);
//     PRINT(B_data);

    V.resize(dim);

    unsigned lwork = 3*dim-1;
//     PRINT(lwork);

    LMT::Vec<double> work;
    work.resize(lwork);
//     PRINT(work);

    int info = 0;
//     PRINT(info);

    dsygv_(1, 'V', 'U', dim, A_data.ptr(), dim, B_data.ptr(), dim, V.ptr(), work.ptr(), lwork, info);
//     PRINT(info);
//     PRINT(A_data);
//     PRINT(B_data);
//     PRINT(V);

    P.resize(dim,dim);

    unsigned P_cpt = 0;
    for(unsigned j=0; j<dim; j++)
    {
        for(unsigned i=0; i<dim; i++)
        {
            P(i, j) = A_data[P_cpt++];
        }
    }
//     PRINT(P);
}

#endif
