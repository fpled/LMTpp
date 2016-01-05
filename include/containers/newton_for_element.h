#ifndef NEWTON_FOR_ELEMENT_H
#define NEWTON_FOR_ELEMENT_H

#include "system_Poly_common.h"
#include "mat.h"
#include "determinant.h"
#include "../mesh/Hexa.h"
#include "../mesh/Wedge.h"
namespace LMT {

/*!
    Objectif : 
        Ces fonctions renvoie la matrice jacobienne J(x) de nb_vi polynômes où nb_vi est le nombre d'indéterminées. La représentation de ces polynômes est très simple puisqu'on stocke leurs coefficients dans un vecteur avec l'ordre suivant 
            * cas de 3 indéterminées :
                1 U V W U^2 UV UW V^2 VW W^2 U^3 U^2V U^2W UV^2 UVW UW^2 V^3 V^2W VW^2 W^3 etc... 
            * cas de 2 indéterminées :
                1 U V U^2 UV V^2 U^3 U^2V UV^2 V^3 etc...


    Paramètres :
        * <strong> NE </strong> est le type d'élément géométrique ( e.g. \a Hexa , \a Wedge , \a Pyramid , \a Triangle_6 ),
        * <strong> f </strong> est le système de fonctions polynômes,
        * <strong> vi </strong> est la variable "x".
*/ 

template<class T, int s, class NE >
Mat<T, Gen<NE::nb_var_inter,NE::nb_var_inter>, Dense<> > jacobian_for_element( const NE &ne, 
                                                                               const Vec< Vec<T,s>, NE::nb_var_inter> &f, 
                                                                               const Vec<T, NE::nb_var_inter > &vi ) {
    assert( 0 );
    return Mat<T, Gen<NE::nb_var_inter,NE::nb_var_inter>, Dense<> >( 0 );
}

template<class T>
Mat<T, Gen<Hexa::nb_var_inter,Hexa::nb_var_inter>, Dense<> > jacobian_for_element( const Hexa &ne, 
                                                                                   const Vec< Vec<T,20>, Hexa::nb_var_inter> &f, 
                                                                                   const Vec<T, Hexa::nb_var_inter > &vi ) {
                         
    Mat<T, Gen<Hexa::nb_var_inter,Hexa::nb_var_inter>, Dense<> > J;
    
    for( unsigned i = 0; i < Hexa::nb_var_inter; i++ ) {
        J( i, 0 ) = f[i][1]+f[i][5]*vi[1]+f[i][6]*vi[2]+f[i][14]*vi[1]*vi[2];
        J( i, 1 ) = f[i][2]+f[i][5]*vi[0]+f[i][8]*vi[2]+f[i][14]*vi[0]*vi[2];
        J( i, 2 ) = f[i][3]+f[i][6]*vi[0]+f[i][8]*vi[1]+f[i][14]*vi[0]*vi[1];
    }    
    return J;                     
}

template<class T>
Mat<T, Gen<Wedge::nb_var_inter,Wedge::nb_var_inter>, Dense<> > jacobian_for_element( const Wedge &ne, 
                                                                                   const Vec< Vec<T,10>, Wedge::nb_var_inter> &f, 
                                                                                   const Vec<T, Wedge::nb_var_inter > &vi ) {
                         
    Mat<T, Gen<Wedge::nb_var_inter,Wedge::nb_var_inter>, Dense<> > J;
    
    for( unsigned i = 0; i < Wedge::nb_var_inter; i++ ) {
        J( i, 0 ) = f[i][1]+f[i][6]*vi[2];
        J( i, 1 ) = f[i][2]+f[i][8]*vi[2];
        J( i, 2 ) = f[i][3]+f[i][6]*vi[0]+f[i][8]*vi[1];
    }    
    return J;      
}

template<class T>
Mat<T, Gen<Pyramid::nb_var_inter,Pyramid::nb_var_inter>, Dense<> > jacobian_for_element( const Pyramid &ne, 
                                                                                   const Vec< Vec<T,10>, Pyramid::nb_var_inter> &f, 
                                                                                   const Vec<T, Pyramid::nb_var_inter > &vi ) {
                         
    Mat<T, Gen<Pyramid::nb_var_inter,Pyramid::nb_var_inter>, Dense<> > J;
    
    for( unsigned i = 0; i < Pyramid::nb_var_inter; i++ ) {
        J( i, 0 ) = f[i][1];
        J( i, 1 ) = f[i][2]+f[i][8]*vi[2];
        J( i, 2 ) = f[i][3]+f[i][8]*vi[1];
    }    
                         
}

/*!
    Objectif :
        Cette fonction calcule des itérés de l'opérateur de Newton-Raphson x -> x - df(x)^(-1) f(x) pour un système polynomial à stockage simple. Plus précisément un polynôme est représenté par ses coefficients stockés dans un vecteur dans l'ordre suivant
            * cas de 3 indéterminées :
                1 U V W U^2 UV UW V^2 VW W^2 U^3 U^2V U^2W UV^2 UVW UW^2 V^3 V^2W VW^2 W^3 etc... 
            * cas de 2 indéterminées :
                1 U V U^2 UV V^2 U^3 U^2V UV^2 V^3 etc...
        
    Paramètres :
        * <strong> NE </strong> est le type d'élément géométrique ( e.g. \a Hexa , \a Wedge , \a Pyramid , \a Triangle_6 ),
        * <strong> f </strong> est le système de fonctions polynômes,
        * <strong> vi </strong> est la variable "x",
        * <strong> residu </strong> est la valeur de f en "x" : "f(x)".
        * <strong> iter </strong> est le nombre d'itérations
*/
template<class T, int s, class NE >
bool newton_for_element( const NE &ne, 
                         const Vec< Vec<T,s>, NE::nb_var_inter> &f, 
                         Vec<T, NE::nb_var_inter > &vi, 
                         Vec<T, NE::nb_var_inter > &residu, 
                         unsigned iter = 2 ) {

    typedef Vec<T, NE::nb_var_inter > V;
    
    V res = 0;
    Mat<T, Gen<NE::nb_var_inter,NE::nb_var_inter>, Dense<> > J;
    T n_0 = norm_inf( residu );
    T epsi = std::ldexp( std::numeric_limits<T>::epsilon(), 5  );
    
    J = jacobian_for_element( ne, f, vi );
    
    if ( std::abs( determinant( J ) ) > std::ldexp( 1, 4 - ( std::numeric_limits<T>::digits >> 2 ) ) ) {
         
        for( unsigned it = 0; it  < iter - 1; ++it ) {
            vi -= inv( J ) * residu;
            J = jacobian_for_element( ne, f, vi );
            for( unsigned d = 0; d < NE::nb_var_inter; ++d )
                residu[ d ] = polyres( f[ d ], vi );
            //residu = V3( polyres( f[0], vi ), polyres( f[1], vi ), polyres( f[2], vi ) );
            //PRINT( residu );
        }
        vi -= inv( J ) * residu;
    }
    
    for( unsigned d = 0; d < NE::nb_var_inter; ++d )
        residu[ d ] = polyres( f[ d ], vi );
    T n_1 = norm_inf( residu );
    if ( ( n_1 <= n_0 ) or ( n_1 < epsi ) )
        return true;
    else
        return false;
}

// template<class T>
// bool newton_for_element( const Hexa &ne, const Vec< Vec<T,20>, 3> &f, Vec<T,3> &vi, Vec<T,3> &residu, unsigned iter = 2 ) {
//     typedef Vec<T, 3> V3;
//     
//     V3 res = 0;
//     Mat<T, Gen<3,3>, Dense<> > J;
//     T n_0 = norm_inf( residu );
//     T epsi = std::ldexp( std::numeric_limits<T>::epsilon(), 5  );
//     
//     for( unsigned i = 0; i < 3; i++ ) {
//         J( i, 0 ) = f[i][1]+f[i][5]*vi[1]+f[i][6]*vi[2]+f[i][14]*vi[1]*vi[2];
//         J( i, 1 ) = f[i][2]+f[i][5]*vi[0]+f[i][8]*vi[2]+f[i][14]*vi[0]*vi[2];
//         J( i, 2 ) = f[i][3]+f[i][6]*vi[0]+f[i][8]*vi[1]+f[i][14]*vi[0]*vi[1];
//     }
//     
//     if ( std::abs( determinant( J ) ) > std::ldexp( 1, 4 - ( std::numeric_limits<T>::digits >> 2 ) ) ) {
//          
//         for( unsigned it = 0; it  < iter - 1; ++it ) {
//             vi -= inv( J ) * residu;
//             for( unsigned i = 0; i < 3; i++ ) {
//                     J( i, 0 ) = f[i][1]+f[i][5]*vi[1]+f[i][6]*vi[2]+f[i][14]*vi[1]*vi[2];
//                     J( i, 1 ) = f[i][2]+f[i][5]*vi[0]+f[i][8]*vi[2]+f[i][14]*vi[0]*vi[2];
//                     J( i, 2 ) = f[i][3]+f[i][6]*vi[0]+f[i][8]*vi[1]+f[i][14]*vi[0]*vi[1];           
//             }
//             residu = V3( polyres( f[0], vi ), polyres( f[1], vi ), polyres( f[2], vi ) );
//             //PRINT( residu );
//         }
//         vi -= inv( J ) * residu;
//     }
//     
//     T n_1 = norm_inf( V3( polyres( f[0], vi ), polyres( f[1], vi ), polyres( f[2], vi ) ) );
//     if ( ( n_1 <= n_0 ) or ( n_1 < epsi ) )
//         return true;
//     else
//         return false;
// }

// template<class T>
// bool newton_for_element( const Wedge &ne, const Vec< Vec<T,10>, 3> &f, Vec<T,3> &vi, Vec<T,3> &residu, unsigned iter = 2 ) {
//     typedef Vec<T, 3> V3;
//     
//     V3 res = 0;
//     Mat<T, Gen<3,3>, Dense<> > J;
//     T n_0 = norm_inf( residu );
//     T epsi = std::ldexp( std::numeric_limits<T>::epsilon(), 5  );
//     
//     for( unsigned i = 0; i < 3; i++ ) {
//         J( i, 0 ) = f[i][1]+f[i][6]*vi[2];
//         J( i, 1 ) = f[i][2]+f[i][8]*vi[2];
//         J( i, 2 ) = f[i][3]+f[i][6]*vi[0]+f[i][8]*vi[1];
//     }
//     
//     if ( std::abs( determinant( J ) ) > std::ldexp( 1, 4 - ( std::numeric_limits<T>::digits >> 2 ) ) ) {
//          
//         for( unsigned it = 0; it  < iter - 1; ++it ) {
//             vi -= inv( J ) * residu;
//             for( unsigned i = 0; i < 3; i++ ) {
//                 J( i, 0 ) = f[i][1]+f[i][6]*vi[2];
//                 J( i, 1 ) = f[i][2]+f[i][8]*vi[2];
//                 J( i, 2 ) = f[i][3]+f[i][6]*vi[0]+f[i][8]*vi[1];           
//             }
//             residu = V3( polyres( f[0], vi ), polyres( f[1], vi ), polyres( f[2], vi ) );
//         }
//         vi -= inv( J ) * residu;
//     }
//     
//     T n_1 = norm_inf( V3( polyres( f[0], vi ), polyres( f[1], vi ), polyres( f[2], vi ) ) );
//     if ( ( n_1 <= n_0 ) or ( n_1 < epsi ) )
//         return true;
//     else
//         return false;
// }

}

#endif /// NEWTON_FOR_ELEMENT_H
