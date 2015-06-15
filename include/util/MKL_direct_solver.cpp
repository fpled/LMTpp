
#ifdef WITH_MKL

#ifdef METIL_COMP_DIRECTIVE

#pragma lib_path /opt/intel/composer_xe_2015.1.133/compiler/lib/intel64
#pragma lib_path /opt/intel/composer_xe_2015.1.133/mkl/lib/intel64
#pragma lib_name mkl_intel_lp64
#pragma lib_name mkl_gnu_thread
#pragma lib_name mkl_core
#pragma lib_name iomp5
#pragma lnk_flag /usr/lib/libpthread_nonshared.a

#endif

#include "MKL_direct_solver.h"

MKL_direct_solver::MKL_direct_solver() {
    /** -------------------------------------------------------------------- */
    /** .. Setup Pardiso control parameters. */
    /** -------------------------------------------------------------------- */
    for ( unsigned i = 0; i < 64; i++ )
        iparm[i] = 0;
    iparm[0] = 1; /** No solver default */
    iparm[1] = 2; /** Fill-in reordering from METIS */
    iparm[3] = 0; /** No iterative-direct algorithm */
    iparm[4] = 0; /** No user fill-in reducing permutation */
    //iparm[5] = 1; /** Write solution into b */
    iparm[7] = 2; /** Max numbers of iterative refinement steps */
    iparm[9] = 13; /** Perturb the pivot elements with 1E-13 */
    iparm[10] = 1; /** Use nonsymmetric permutation and scaling MPS */
    iparm[12] = 0; /** Maximum weighted matching algorithm is switched-off (default for symmetric). Try iparm[12] = 1 in case of inappropriate accuracy */
    iparm[13] = 0; /** Output: Number of perturbed pivots */
    iparm[17] = -1; /** Output: Number of nonzeros in the factor LU */
    iparm[18] = -1; /** Output: Mflops for LU factorization */
    iparm[19] = 0; /** Output: Numbers of CG Iterations */
    /// le flag 34 ne marche pas pour toutes les versions de MKL :-(
    //iparm[34] = 1; /** PARDISO use C-style indexing for ia and ja arrays */
//    iparm[59] = 1; /** out of core*/
    /** -------------------------------------------------------------------- */
    /** .. Initialize the internal solver memory pointer. This is only */
    /** necessary for the FIRST call of the PARDISO solver. */
    /** -------------------------------------------------------------------- */
    for ( unsigned i = 0; i < 64; i++ )
        pt[i] = 0;

}

#endif
