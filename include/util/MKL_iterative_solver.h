#ifndef LMTPP_MKL_ITERATIVE_SOLVER_H
#define LMTPP_MKL_ITERATIVE_SOLVER_H

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

#include <cmath>

#include "mkl_rci.h"
#include "mkl_blas.h"
#include "mkl_spblas.h"
#include "mkl_service.h"
#include "MKL_common.h"
#endif

#include "containers/mat.h"

/*!
    Cette classe encapsule la décomposition LDL ou Cholesky d'une matrice symétrique ou la décomposition LU d'une matrice carré quelconque à l'aide de la librairie MKL d'intel .
    On l'utilise comme \a LDL_solver ou \a MUMPS_solver :
        
    Pour se servir de cette classe, le plus délicat est la compilation et surtout l'édition des liens.
    
    * Il faut aussi l'option de compilation -DWITH_MKL .
    
    REMARQUE : pour l'instant la classe ne gère pas le calcul sur plusieurs noeuds.
        
    
    \friend raphael.pasquier@lmt.ens-cachan.fr
    \friend hugo.leclerc@lmt.ens-cachan.fr
*/

#ifdef WITH_MKL


namespace LMT {

enum MKL_preconditioner {
    MKL_unknown_preconditioner = -1,
    MKL_no_preconditioner = 0,
    MKL_Jacobi_preconditioner  = 1,
    MKL_incomplete_chol_preconditioner = 2,
    MKL_SSOR_preconditioner = 3
    //LMTpp_preconditioner = 100
};

/*!
    Cette classe encapsule l'implémentation du gradient conjugué de la librairie MKL.
    On l'utilise par exemple ainsi :
    \code C/C++
        int main( int argc, char* argv[] ) {
        
            Mat<double, Sym<>, SparseLine<> > m( 3, 3 );
            m(0,0) = 1; m(0,1) = 2; m(0,2) = -3;
                        m(1,1) = 5; m(1,2) = -7;
                                    m(2,2) = 13;
            Vec<double, 3> s( 0., 0.1, 0.1 ), s_expexted( 11, 2, 4 );
            Vec<double, 3> b(3,4,5);
        
            MKL_iterative_solver solver( m, false );

            solver.set_preconditioner( MKL_incomplete_chol_preconditioner, m );

            unsigned nb_iter = solver.solve( b, s, MKL_norm_inf_residual_stopping( 1e-10 ), 100 );
        
            PRINT( nb_iter );
            PRINT( s );
        
            return 0;
        }
    
    le paramètre <strong> false </strong> du constructeur de \a MKL_iterative_solver permet de ne pas effacer la matrice <strong> m </strong> .
    Car on s'en sert ensuite pour générer le pré-conditionneur Cholesky incomplet à la ligne suivante via la méthode <strong> set_preconditioner() </strong> .
    Il y a un autre pré-conditionneur :
    * MKL_Jacobi_preconditioner
    Par défaut aucun pré-conditionneur n'est utilisé (i.e. si la méthode set_preconditioner() n'est pas utilisée ).
    
    Enfin on résout le système via la méthode <strong> solve() </strong> .
    Ces paramètres sont :
    * <strong> b </strong> le second membre du système.
    * <strong> s </strong> la valeur initiale puis une "meilleure" approximation de la solution comme retour.
    * le critère d'arrêt. Il y a différents choix possibles :
        * norm2( residu ) < 1e-5 ( critère par défaut ),
        * si le paramètre est un double de nom epsilon, alors le test est norm2( residu ) < epsilon, 
        * si le paramètre est de type \a MKL_norm_inf_residual_stopping ( 1e-10 ) alors le test est max( residu[ i ] ) < 1e-10,

    Elle retourne le nombre d'itérations effectuées.
    
*/
struct MKL_iterative_solver {

    template< class TM>
    MKL_iterative_solver( TM &mat, bool want_free = true ) {
        assert( 0 );
        if ( want_free )
            mat.free();    
    }
    
    template<int sr, bool stored_in_upper_part_>
    MKL_iterative_solver( Mat<double, Sym< sr, stored_in_upper_part_>, SparseLine<> > &mat, bool want_free = true, bool is_positive_definite = false ) {

        m_cond = NULL;
        
        m.init( mat );
        mtype = m.get_mtype();
        
        if ( is_positive_definite )
            mtype = 2;
        if ( want_free )
            mat.free(); 
            
        dpar[ 1 ] = 1e-6;
        display_norm2_residual = false;
    }
    
    template<int sr, int sc>
    MKL_iterative_solver( Mat<double, Gen<sr,sc>, SparseLine<> > &mat, bool want_free = true ) {

        m_cond = NULL;
    
        m.init( mat );
        mtype = m.get_mtype();
        if ( want_free )
            mat.free();  
               
        dpar[ 1 ] = 1e-6;
        display_norm2_residual = false;
    }

    ~MKL_iterative_solver() {
        free();
        delete m_cond;
        mkl_free_buffers();
    }

    /// permet de libérer la mémoire avant la destruction de l'objet
    void free() {
        m.free();
        if ( m_cond )
            m_cond->free();
    }
    
    void set_no_preconditioner() {
        delete m_cond;
    }
    
        
    template<class TPreCond >
    void set_preconditioner( MKL_preconditioner p, const TPreCond &precond ) {
        switch( p ) {
            case MKL_unknown_preconditioner : 
                ipar[ 10 ] = 0; /// pas de pré-conditionneur
                delete m_cond;
                break;
            case MKL_no_preconditioner :  /// pas de pré-conditionneur
                ipar[ 10 ] = 0;
                delete m_cond;
                break;
            case MKL_Jacobi_preconditioner :
                ipar[ 10 ] = 1;
                delete m_cond;
                m_cond = new MKL_Jacobi_matrix( precond );
                break;
            case MKL_incomplete_chol_preconditioner :
                ipar[ 10 ] = 1;
                delete m_cond;
                m_cond = new MKL_incomplete_chol_matrix( precond );
                break;
            case MKL_SSOR_preconditioner :
                ipar[ 10 ] = 1;
                delete m_cond;
                m_cond = new MKL_SSOR_matrix( precond, m );
                break;
            default:
                ipar[ 10 ] = 0; /// pas de pré-conditionneur
                delete m_cond;                
        }    
    }
    
    /// comportement par défaut
    template<class CritOperator>
    void set_stopping_test( const CritOperator &s ) {
        ipar[8] = 1; /// test d'arrêt interne à MKL activé
        ipar[9] = 0; /// pas de test d'arrêt de l'utilisateur
           
        dpar[0] = 1e-5; /// precision relative    
    }
    
    void set_stopping_test( const double &s ) {
        ipar[8] = 0; /// test d'arrêt interne à MKL inactivé
        ipar[9] = 1; /// test d'arrêt de l'utilisateur activé
           
        dpar[ 1 ] = s; /// precision absolue    
    }
    
    void set_stopping_test( const MKL_norm2_residual_stopping &s ) {
        ipar[8] = 0; /// test d'arrêt interne à MKL inactivé
        ipar[9] = 1; /// test d'arrêt de l'utilisateur activé
     
        dpar[ 1 ] = s.eps; /// precision absolue    
    }
    
    void set_stopping_test( const MKL_norm_inf_residual_stopping &s ) {
        ipar[8] = 0; /// test d'arrêt interne à MKL inactivé
        ipar[9] = 1; /// test d'arrêt de l'utilisateur activé
     
        dpar[ 1 ] = s.eps; /// precision absolue    
    }

    /// comportement par défaut
    template<class CritOperator>
    bool apply_stopping_test( const CritOperator &s, double *x, double *b ) { return dpar[ 4 ] < dpar[ 1 ]; }
    
    bool apply_stopping_test( const double &s, double *x, double *b ) {
        return std::sqrt( dpar[ 4 ] ) < dpar[ 1 ];
    }
    
    bool apply_stopping_test( const MKL_norm2_residual_stopping &s, double *x, double *b ) {
        return std::sqrt( dpar[ 4 ] ) < dpar[ 1 ];
    }
    
    bool apply_stopping_test( const MKL_norm_inf_residual_stopping &s, double *x, double *b ) {
        /// calcul du résidu 
        double *temp = new double[ m.n ];
        char tr = 'u'; /// upper storage
        mkl_dcsrsymv( &tr, &m.n, m.a, m.ia, m.ja, x, temp );  /// temp = A x
        double a = -1;
        int incr = 1;
        daxpy( &m.n, &a, b, &incr, temp, &incr ); /// temp = temp + (-1) * b 
        double norminf = 0;
        for( int i = 0; i < m.n; ++i ) 
            norminf = std::max( norminf, std::abs( temp[ i ] ) );
        delete[] temp;
        return norminf < dpar[1];
    }
        
    /// résout le sytème de second membre b et renvoie le résultat dans b
    template<int s, class CritOperator>
    unsigned solve( const Vec<double, s> &b, Vec<double, s> &x, const CritOperator &crit_op, unsigned max_iter, bool display_norm2_residual = false ) {
        
        if ( m.n == 0 ) {
            std::cerr << "Warning Mkl_iterative_solver.solve() : not matrix" << std::endl;
            return 0;
        }
        
        int rci_request;
        double* tmp = new double[ 4 * m.n ];
        /// au cours des calculs,
        /// tmp[0:n-1] est la direction courante
        /// tmp[n:2n-1] est le produit de la matrice et de la direction courante
        /// tmp[2n:3n-1] est le résidu courant
        /// tmp[3n:4n-1] contient l'inverse du pré-conditionneur appliqué au résidu courant ( voir page 2547 de la doc de la MKL )  
        
        /// initialisation du solver pour matrice symétrique définie positive
        dcg_init( &m.n, x.ptr(), const_cast<double*>( b.ptr() ), &rci_request, ipar, dpar, tmp );
        if ( rci_request ) {
            delete [] tmp; //free_internal_buffer( tmp );
            std::cerr << "Error MKL_iterative_solver.solve() : bad initialization"<< std::endl;
            return 0;
        }

        ipar[4] = max_iter;

        set_stopping_test( crit_op );

        if ( m_cond )
            ipar[ 10 ] = 1; /// pré-conditionneur activé
            
        /**---------------------------------------------------------------------------*/
        /** Check the correctness and consistency of the newly set parameters         */
        /**---------------------------------------------------------------------------*/
        dcg_check( &m.n, x.ptr(), const_cast<double*>( b.ptr() ), &rci_request, ipar, dpar, tmp );
        if ( rci_request ) {
            delete [] tmp; //free_internal_buffer( tmp );
            std::cerr << "Error MKL_iterative_solver.solve() : bad correctness of parameters of solver"<< std::endl;
            return 0;        
        }
        
        while( true ) {
            dcg( &m.n, x.ptr(), const_cast<double*>( b.ptr() ), &rci_request, ipar, dpar, tmp );
            //PRINT( rci_request );
            switch( rci_request ) {
                case 0 :
                    return get_internal_solution( x.ptr(), const_cast<double*>( b.ptr() ) , tmp );
                case 1 : { /// compute the vector A*tmp[0] and put the result in vector tmp[n]
                    char tr = 'u';
                    mkl_dcsrsymv( &tr, &m.n, m.a, m.ia, m.ja, tmp, &tmp[m.n] );
                } break;
                case 2 :
                    if ( display_norm2_residual ) 
                        std::cout << "norm L2 of the current residual = "<< std::sqrt( dpar[ 4 ] ) << std::endl;
                    if ( apply_stopping_test( crit_op, x.ptr(), const_cast<double*>( b.ptr() ) ) )
                        return get_internal_solution( x.ptr(), const_cast<double*>( b.ptr() ) , tmp );
                break;
                case 3 : { /// applique le pré-conditionneur à tmp[2n:3n-1], le résidu courant,  et retourne le résultat dans tmp[3n:4n-1]
                    if ( m_cond )
                        m_cond->apply_as_conditioner( tmp + ( 2 * m.n ), tmp + ( 3 * m.n ) );
                    else
                        std::cerr << "WARNING : Mkl_iterative_solver.solve() : you want to apply a preconditioner but it's not defined" << std::endl;
                } break;
                default :
                    delete [] tmp; //free_internal_buffer( tmp );
                    std::cerr << "Error MKL_iterative_solver.solve() : dcg() bugs..."<< std::endl;
                    return 0; 
            }
        }
        
        delete [] tmp; //free_internal_buffer( tmp );
        return 0;
    }
    
    double last_norm_of_residual() const {
        return std::sqrt( dpar[ 4 ] );
    }
    
//     void free_internal_buffer( double *b ) {
//         delete[] b;
//     }
    
    unsigned get_internal_solution( double *b, double *x , double *tempon ) {
        int itercount, rci_request;
        
        dcg_get( &m.n, x, b, &rci_request, ipar, dpar, tempon, &itercount );
        delete [] tempon; //free_internal_buffer( tempon );
        return (unsigned) itercount;
    }
    
    /// *** attributs *** ///
    int ipar[128];
    double dpar[128];
    int mtype;
    MKL_CSR_matrix m, *m_cond;
    bool display_norm2_residual;
};

};

#endif /// ifdef WITH_MKL

#endif /// LMTPP_MKL_ITERATIVE_SOLVER_H

