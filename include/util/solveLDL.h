#ifndef SOLVE_LDL_H
#define SOLVE_LDL_H

#include "../containers/mat.h"
//extern "C" {
#include "../amd/amd.h"
//}
#include "beep.h"

/*!
    Cette classe encapsule la décomposition LDL d'une matrice symétrique.
    On l'utilise ainsi :
    \code C/C++
        LDL_solver ldl;
        Mat<double,LMT::Sym<>,LMT::SparseLine<> > K;
        /// on construit K
        ldl.get_factorization( K );
        /// à ce stade, l'instance ldl contient une factorisation LDL de K
        Vec<double> f1, f2, ...;
        /// on construit f1, f2, ...
        ldl.solve( f1 );
        /// f1 contient maintenant la solution du problème Ku = f1
        ldl.solve( f2 );
        /// f2 contient maintenant la solution du problème Ku = f2
        ...
        
    Les méthodes get_factorization() ont les arguments <strong> want_free </strong> , <strong> want_semi_morse </strong> et <strong> want_amd_order </strong> :
        * <strong> want_free </strong> : s'il est mis à vrai (valeur par défaut), la méthode libère la mémoire de la matrice <strong> mat </strong> passée en argument sinon mat reste inchangée.
        * <strong> want_semi_morse </strong> : paramètre de l'algoritme de factorisation
        * <strong> want_amd_order </strong> : paramètre de l'algoritme de factorisation

*/
class LDL_solver {
public:
    typedef double T;

    LDL_solver() {
        pseudo_inverse=false;
        Pivots_To_block.resize(0);
    }
    bool pseudo_inverse;

    /// after that, data in mat won't be used anymore.
    void get_factorization( LMT::Mat<double,LMT::Sym<>,LMT::SparseLine<> > &mat, bool want_free=true, bool want_semi_morse=false, bool want_amd_order=true );
    /// after that, data in mat won't be used anymore.
    void get_factorization( LMT::Mat<double,LMT::Gen<>,LMT::SparseLine<> > &mat, bool want_free=true, bool want_semi_morse=false, bool want_amd_order=true );
    /// after that, data in mat won't be used anymore.
    void get_factorization( LMT::Mat<double,LMT::Sym<>,LMT::SparseLine<> > &mat, LMT::Vec<LMT::Vec<double> > &Kernel, bool want_free=true, bool want_semi_morse=false, bool want_amd_order=true ); // PK et PG (pseudo-inverse)
    void get_factorization( LMT::Mat<double,LMT::Gen<>,LMT::SparseLine<> > &mat, LMT::Vec<LMT::Vec<double> > &Kernel, bool want_free=true, bool want_semi_morse=false, bool want_amd_order=true ); // PK et PG (pseudo-inverse)
    void get_factorization( LMT::Mat<double,LMT::Sym<>,LMT::SparseLine<> > &mat, LMT::Vec<LMT::Vec<double> > &Kernel, LMT::Vec<int> &Pivots, bool want_free=true, bool want_semi_morse=false, bool want_amd_order=true ); // PK et PG (pseudo-inverse)
    void get_factorization( LMT::Mat<double,LMT::Gen<>,LMT::SparseLine<> > &mat, LMT::Vec<LMT::Vec<double> > &Kernel, LMT::Vec<int> &Pivots, bool want_free=true, bool want_semi_morse=false, bool want_amd_order=true ); // PK et PG (pseudo-inverse)
    /// ...
    template<class TM> void get_factorization( const TM &mat, bool want_free=true, bool want_semi_morse=false, bool want_amd_order=true ) { assert(0); /* TODO */ }
    /// ...
    void solve( LMT::Vec<double> &vec );

    LMT::Vec<int> Pivots_To_block;
    
private:
    ///
    void update_block_set_from_factorization();
    ///
    void semi_morse_lsolve( LMT::Vec<double> &Y );
    ///
    void semi_morse_ltsolve( LMT::Vec<double> &Y );
    
    /// LDL stuff
    double Info[AMD_INFO];
    LMT::Vec<double> Ax, Lx, B, D, X, Y;
    LMT::Vec<int> Ai, Ap, Li, Lp, P, Pinv, Flag, Pattern, Lnz, Parent, kernod;
    int lnz, n;
    
    /// semimorse stuff
    bool want_semi_morse_;
    LMT::Vec<T> data_for_block_set;
    LMT::Vec<LineOfBeeps> beep_lines;
};

#endif // SOLVE_LDL_H

