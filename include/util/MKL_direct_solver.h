#ifndef LMTPP_MKL_DIRECT_SOLVER_H
#define LMTPP_MKL_DIRECT_SOLVER_H

#ifdef WITH_MKL

#ifdef METIL_COMP_DIRECTIVE

#pragma lib_path /opt/intel/composer_xe_2015.1.133/compiler/lib/intel64
#pragma lib_path /opt/intel/composer_xe_2015.1.133/mkl/lib/intel64
#pragma lib_name mkl_intel_lp64
#pragma lib_name mkl_gnu_thread
#pragma lib_name mkl_core
#pragma lib_name iomp5
#pragma lnk_flag /usr/lib/libpthread_nonshared.a

#pragma inc_path /opt/intel/composer_xe_2015.1.133/mkl/include

#endif

#include "MKL_common.h"

#include <cmath>
#include "mkl_pardiso.h"
#include "mkl_types.h"

#endif

#include "containers/mat.h"

using namespace LMT;

/*!
    Cette classe encapsule la décomposition LDL ou Cholesky d'une matrice symétrique ou la décomposition LU d'une matrice carré quelconque à l'aide de la librairie MKL d'intel .
    On l'utilise comme \a LDL_solver ou \a MUMPS_solver :
        
    Pour se servir de cette classe, le plus délicat est la compilation et surtout l'édition des liens.
    
    * Il faut aussi l'option de compilation -DWITH_MKL .
    
    REMARQUE : pour l'instant la classe ne gère pas le calcul sur plusieurs noeuds.
        
*/

#ifdef WITH_MKL

struct MKL_direct_solver {
    typedef double T;

    MKL_direct_solver();

    ~MKL_direct_solver() {
        free();
    }

    void get_factorization_general() {
        int nrhs, maxfct, mnum, phase, error, msglvl;
        
        maxfct = 1; /** Maximum number of numerical factorizations. */
        mnum = 1; /** Which factorization to use. */
        msglvl = 0; /** No print statistical information in file */
        error = 0; /** Initialize error flag */
        nrhs = 1; /** number of right hand side */
        
        /** -------------------------------------------------------------------- */
        /** .. Reordering and Symbolic Factorization. This step also allocates */
        /** all memory that is necessary for the factorization. */
        /** -------------------------------------------------------------------- */
        phase = 11;
        PARDISO( pt, &maxfct, &mnum, &mtype, &phase, &m.n, m.a, m.ia, m.ja, &idum, &nrhs, iparm, &msglvl, &ddum, &ddum, &error);
        if ( error ) {
            std::cerr << "\nERROR during symbolic factorization: " <<  error << std::endl;
            free();
            return;
        }
        
        /** -------------------------------------------------------------------- */
        /** .. Numerical factorization. */
        /** -------------------------------------------------------------------- */
        phase = 22;
        PARDISO( pt, &maxfct, &mnum, &mtype, &phase, &m.n, m.a, m.ia, m.ja, &idum, &nrhs, iparm, &msglvl, &ddum, &ddum, &error );
        if ( error ) {
            std::cerr << "\nERROR during numerical factorization: " << error << std::endl;
            free();
            return;
        }
    }
    
    /// factorise la matrice passée en paramètre et libère sa mémoire si want_free == true.
    template<int sr, bool stored_in_upper_part_ >
    void get_factorization( Mat<double, Sym< sr, stored_in_upper_part_>, SparseLine<> > &mat, bool want_free = true, bool is_positive_definite = false ) {
        free();
        m.init( mat );
        mtype =  m.get_mtype();
        if ( is_positive_definite )
            mtype = 2;
        get_factorization_general();
        if ( want_free )
            mat.free();
    }
    
    /// factorise la matrice passée en paramètre et libère sa mémoire si want_free == true.
    template<int sr, int sc>
    void get_factorization( Mat<double, Gen<sr,sc>, SparseLine<> > &mat, bool want_free = true ) {
        free();
        m.init( mat );
        mtype = m.get_mtype();
        get_factorization_general();
        if ( want_free )
            mat.free();
    }
    
    /// résout le sytème de second membre b et renvoie le résultat dans b
    template<int s>
    Vec<double> solve( const Vec<double, s> &b, int numbers_of_iterative_refinement = 2 ) {
        Vec<double> x;
        int nrhs, maxfct, mnum, phase, error, msglvl;;
        
        maxfct = 1; /** Maximum number of numerical factorizations. */
        mnum = 1; /** Which factorization to use. */
        msglvl = 0; /** No print statistical information in file */
        error = 0; /** Initialize error flag */
        nrhs = 1; /** number of right hand side */
        
        if ( m.n )
            x.resize( m.n );
        else {
            std::cerr << "\nERROR during solution" << std::endl;
            return x;
        }
        phase = 33;
        iparm[7] = numbers_of_iterative_refinement; /** Max numbers of iterative refinement steps. */

        PARDISO( pt, &maxfct, &mnum, &mtype, &phase, &m.n, m.a, m.ia, m.ja, &idum, &nrhs, iparm, &msglvl, const_cast<double*>( b.ptr() ), x.ptr(), &error );
        if ( error ) {
            std::cerr << "\nERROR during solution: " << error << std::endl;
            return x;
        }
        
        return x;
    }
    
    void free() {
        if ( m.n ) {
            m.free();
            
            int nrhs, maxfct, mnum, phase, error, msglvl;
            
            maxfct = 1; /** Maximum number of numerical factorizations. */
            mnum = 1; /** Which factorization to use. */
            msglvl = 0; /** No print statistical information in file */
            error = 0; /** Initialize error flag */
            nrhs = 1; /** number of right hand side */
    
            phase = -1; /** Release internal memory. */
            PARDISO( pt, &maxfct, &mnum, &mtype, &phase, &m.n, &ddum, m.ia, m.ja, &idum, &nrhs, iparm, &msglvl, &ddum, &ddum, &error );
        }
    }

    /// attributs:   ///
    int iparm[64];
    MKL_CSR_matrix m;
    int mtype;
    void *pt[64];
    double *a;
    double ddum; /** Double dummy */
    int idum; /** Integer dummy. */
};
#endif /// ifdef WITH_MKL

#endif /// LMTPP_MKL_DIRECT_SOLVER_H

