#ifndef EIGEN_H
#define EIGEN_H

#include "vec.h"

namespace LMT {

//#include <containers/vec.h>
    
/*!
    Cette fonction calcule les valeurs propres et les vecteurs propres associés à une matrice symétrique 2x2.
    Si on note la matrice ainsi :
    \verbatim
        | m11 m12 |
        | m12 m22 |
   
    vec doit contenir dans cette ordre : m11, m22 et m12
    Voici un exemple de code qu'on peut télécharger [[include/containers/eigen_value.tar.gz|ici]] :
    \code C/C++
        #include <containers/vec.h>
        #include <containers/eigen.h>

        #include <cmath>
        #include <cstdlib>

        using namespace LMT;
        using namespace std ;


        int main(int argc, char* argv[] ) {

            typedef double T;
            Vec<T,3> vec(1,2,3);
            Vec <T,2> eigenvalues;
            Vec <Vec<T,2>,2>  eigenvectors;

            get_eigen_values_and_vectors (vec, eigenvalues, eigenvectors);
            PRINT( eigenvalues );
            PRINT( eigenvectors[0] );
            PRINT( eigenvectors[1] );
            
            return 0 ;
        }

    avec le SConstruct :
    \code
        from LMT import *
        from LMT.formal_lf import *

        timdaviscppflags = file("/opt/MATH/timdavis/cppflags","r").read().strip()
        timdavisldflags = file("/opt/MATH/timdavis/ldflags","r").read().strip()

        env = Environment(
                CPPPATH = [ '#LMT/include' ],
                LIBS = [ 'pthread','libcholmod' ],
                CPPFLAGS = cppflags( ['xml2-config'] ) + " " + timdaviscppflags + " -O3",
                LINKFLAGS = linkflags( ['xml2-config'] ) + " " + timdavisldflags ,
        )
        make_dep_py(env)

        env.BuildDir( 'build/LMT', 'LMT/include', duplicate=0 )
        libs = SConscript( 'LMT/include/SConscript', exports='env', build_dir='build/LMT' )

        env.Program( "main", ["main.cpp"], build_dir='build/LMT' )

    Et le Makefile :
    \code
        all: codegen
            scons
            ./main
        codegen:
            cd LMT/include/codegen; scons 

   compute eigen values and vectors.
   input : vec in vector form
   output : eigenvalues, eigenvectors
    \keyword Mathématiques/Algèbre linéaire/Vecteurs et valeurs propres

    \friend samir.amrouche@lmt.ens-cachan.fr
    \friend hugo.leclerc@lmt.ens-cachan.fr
 */
template<class T>
void get_eigen_values_and_vectors( const Vec<T,3> & vec, Vec<T,2> & eigenvalues, Vec<Vec<T,2>,2> & eigenvectors ) {
    T b = vec[0]+vec[1];
    T delta = sqrt( b*b - 4 * ( vec[0]*vec[1] - vec[2]*vec[2] ) );
    eigenvalues[0] = 0.5 * ( b - delta );
    eigenvalues[1] = 0.5 * ( b + delta );
    
    //
    if ( LMT::abs(vec[0]-eigenvalues[0]) > LMT::abs( vec[2] ) )
        eigenvectors[0] = Vec<T,2>( vec[2] / (eigenvalues[0]-vec[0]), 1.0 );
    else if ( abs( vec[2] ) >  abs( eigenvalues[0]-vec[1] ) )
        eigenvectors[0] = Vec<T,2>( 1.0, (eigenvalues[0]-vec[0]) / vec[2] );
    else 
        eigenvectors[0] = Vec<T,2>( 1.0, vec[2] / (eigenvalues[0]-vec[1]) );
           
    eigenvectors[0] /= length( eigenvectors[0] );
    
    eigenvectors[1] = Vec<T,2>( -eigenvectors[0][1], eigenvectors[0][0] );
}

}


#endif // EIGEN_H
