#ifndef LMTPP_MUMPS_SOLVER_H
#define LMTPP_MUMPS_SOLVER_H

#ifdef WITH_MUMPS
#include "dmumps_c.h"
#endif

#include "../containers/mat.h"

#define ICNTL(I) icntl[(I)-1] /*! macro s.t. indices match documentation */

using namespace LMT;

/*!
    Cette classe encapsule la décomposition LDL^T d'une matrice symétrique ou la décomposition LU d'une matrice carré quelconque à l'aide de la librairie [[http://graal.ens-lyon.fr/MUMPS/|mumps]] .
    On l'utilise comme \a LDL_solver :
    \code C/C++
        Mat< double, Sym<>, SparseLine<> > A;
        Vec< double > b1, b2, b3, x;
        // ensuite on construit la matrice A et les seconds membres b1,b2, ...
        MUMPS_solver mumps;
        
        mumps.get_factorization( A ); // on effectue la factorisation et le résultat est stocké dans mumps
        
        /// résolution de l'équation A x = b1 
        x = mumps.solve( b1 ); /// x contient maintenant la solution
        
        x = mumps.solve( b2 ); 
         
    Pour la seconde résolution, on a bien-sûr réutilisé la factorisation. 
        
    Pour se servir de cette classe, le plus délicat est la compilation et surtout l'édition des liens.
    Si vous avez installé la librairie mumps avec un paquet (ce qui est recommandé ) et si vous utilisez [[http://www.open-mpi.org/|openmpi]] comme implémentation de MPI, votre fichier SConstruct sera par exemple : 
    
    \code Python
        from LMT import *
        from LMT.formal_lf import *
        
        env = Environment(
            LIBS = [ 'pthread', 'blas' , 'dmumps' , 'mumps_common' ],
            CPPPATH = [ '#LMT/include' , '/usr/lib/openmpi/include' , '/usr/include/suitesparse'  ],
            CPPFLAGS = cppflags( ['xml2-config'] ) + " -O3 -g " + " -DWITH_MUMPS " ,
            LINKFLAGS = linkflags( ['xml2-config'] ),
        )
        make_dep_py(env)
        
        env.BuildDir( 'build/LMT', 'LMT/include', duplicate=0 )
        libs = SConscript( 'LMT/include/SConscript', exports='env', build_dir='build/LMT' )
        
        env.Program( "main", ["main.cpp" ] + libs + [ "/usr/lib/openmpi/lib/libmpi_cxx.so" ] , build_dir='build/LMT' )
        
    * Vérifiez vos chemins openmpi : /usr/lib/openmpi/include et /usr/lib/openmpi/lib/libmpi_cxx.so
    * et que vous avez bien les librairies /usr/lib/libdmumps.so et /usr/lib/libmumps_common.so
    * Il faut aussi l'option de compilation -DWITH_MUMPS .
    
    REMARQUE : pour l'instant la classe ne gère pas le calcul sur plusieurs noeuds.
        
    
    \friend samir.amrouche@lmt.ens-cachan.fr
    \friend hugo.leclerc@lmt.ens-cachan.fr
*/
#ifdef WITH_MUMPS

struct MUMPS_solver {
    typedef double T;

    MUMPS_solver();

    ~MUMPS_solver();

    bool already_factorized;
    
    /// factorise la matrice passée en paramètre et libère sa mémoire si want_free == true.
    void get_factorization( Mat<double, Sym<>, SparseLine<> > &mat, bool want_free = true, bool is_definite_positive = true );
    void get_factorization( Mat<double, Gen<>, SparseLine<> > &mat, bool want_free = true );
    template<class TM> 
    void get_factorization( const TM &mat, bool want_free = true ) { assert(0); /*! TODO */ } /// cas général
    /// résout le sytème de second membre b et renvoie le résultat dans b
    template<int s>
    Vec<double> solve( const Vec<double, s> &b, int numbers_of_iterative_refinement = 2 ) {
    
        Vec<double> x;
        id.ICNTL( 10 ) =  numbers_of_iterative_refinement;
    
        if ( myid == 0 ) {
            x = b;
            id.rhs = x.ptr();
            
            id.job = job_solve;
            dmumps_c( &id );
        }
        
        return x;
    }
    void free();

    /// attributs:
    DMUMPS_STRUC_C id;
    int myid, ierr; /// MPI
    static bool MPI_is_initialized;

  private:
    void init_MPI( int argc = 0, char* argv[] = NULL );
    
    template<class TM>
    void load_matrix( const TM &mat ) {
        assert( 0 );
    }
    
    void print_id() const {
        for( int i = 1; i <= 40; ++i )
            std::cout << "id.ICNTL( " << i << " ) = " << id.ICNTL( i ) << std::endl;
    }
    
    template<class STR>
    void load_matrix( const Mat<double, STR, SparseLine<> > &mat ) {
        id.n = mat.nb_rows();
        for( int i = 0; i < mat.nb_rows(); ++i )
            id.nz += mat.data[ i ].indices.size();
        id.irn = new int[ id.nz ];
        id.jcn = new int[ id.nz ];
        id.a   = new double[ id.nz ];
        for( int i = 0, index = 0; i < mat.nb_rows(); ++i ) {
            for( int j = 0; j < mat.data[ i ].indices.size(); ++j, index++ ) {
                id.irn[ index ] = i + 1; /// + 1 -> convention Fortran
                id.jcn[ index ] = mat.data[ i ].indices[ j ] + 1;  /// + 1 -> convention Fortran
                id.a[ index ] = mat.data[ i ].data[ j ];
            }    
        }
    }    

    static const int job_init = -1;
    static const int job_end  = -2;
    static const int job_analyse = 1;
    static const int job_factorize = 2;
    static const int job_solve = 3;
    static const int use_comm_word = -987654;
};
#endif /// WITH_MUMPS

#endif // LMTPP_MUMPS_SOLVER_H

