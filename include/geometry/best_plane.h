#ifndef BEST_PLANE_H
#define BEST_PLANE_H

#include "../containers/vec.h"
#include "../containers/eig_lapack.h"

namespace LMT {

/*!
    Objectif :
        Déterminer l'équation du plan ax+by+cz+d=0 qui est le plus près au sens des moindre carrés de la liste de point de l'espace list_points[ 0 ], list_points[ 1 ], etc...
    
    Paramètres :
        * Elle retourne dans les paramètres a, b, c et d les coefficients avec la contrainte a^2+b^2+c^2 = 1.
        * list_points est la liste des points. 
        
    Retour :
        Elle retourne le résidu : i.e. somme( i = 0 .. nb_points-1, ( ax(i)+by(i)+cz(i)+d )^2 ) 
    
    \friend hugo.leclerc@lmt.ens-cachan.fr
    \friend samir.amrouche@lmt.ens-cachan.fr
*/

template< class T , int s, int s2 >
T best_plane( T &a, T &b, T &c, T &d, const Vec< Vec< T, s >, s2 > & list_points ) {

    Vec<T> x, y, z;
    x.reserve( list_points.size() );
    y.reserve( list_points.size() );
    z.reserve( list_points.size() );
    for( unsigned i = 0; i < list_points.size(); ++i ) {
        x.push_back( list_points[ i ][ 0 ] );
        y.push_back( list_points[ i ][ 1 ] );
        z.push_back( list_points[ i ][ 2 ] );
    }
    
    T xmean = mean( x ), ymean = mean( y ), zmean = mean( z ), res = 0;
    //Mat< T, Sym<3> > m;
    Mat< T, Sym<> > m( 3, 3 );
    m( 0, 0 ) = variance( x );
    m( 1, 1 ) = variance( y );
    m( 2, 2 ) = variance( z ); 
    m( 0, 1 ) = dot( x, y ) / list_points.size() - xmean * ymean;
    m( 0, 2 ) = dot( x, z ) / list_points.size() - xmean * zmean;
    m( 1, 2 ) = dot( y, z ) / list_points.size() - ymean * zmean;
    //Mat<T, Gen<3,3> > eigen_vector;
    //Vec<T, 3 > eigen_value, v;
    Mat<T> eigen_vector;
    Vec<T> eigen_value;
    
    get_eig_sym( m, eigen_value, eigen_vector );
 
    /// hypothèse : eigen_value[ 0 ] est la valeur propre la plus proche de zéro.
    a = eigen_vector( 0, 0 );
    b = eigen_vector( 0, 1 );
    c = eigen_vector( 0, 2 ); 
    d = -xmean * a - ymean * b - zmean * c;
    
    for( unsigned i = 0; i < list_points.size(); ++i )
        res += std::pow( a * x[ i ] + b * y[ i ] + c * z[ i ] + d, 2 );
    return res;
}

/*!
    Objectif :
        Cette classe est un foncteur qui s'applique sur les éléments d'un maillage 3D pour déterminer l'applatissement de ses éléments.
        
    Exemple d'utilisation :
    \code C/C++
        TM m; /// maillage 3D
        
        Applatissement<> app;
        app.init();
        apply( m.elem_list, app );
        PRINT( app.applatissement ); /// affiche l'applatissement "minimale" des éléments de m.
        
        
    \keyword Maillage/Opération
    \friend leclerc@lmt.ens-cachan.fr
*/
template <class T = double>
struct Applatissement {
    Applatissement() {
        init();
    }
    
    void init() {
        applatissement = 1e100;
    }
    
    template<class TE>
    void operator() ( const TE &e ) {
        Vec< typename TE::Pvec, TE::nb_nodes > list_points;
        T a, b, c, d, res;
        
        for( unsigned j = 0 ; j < TE::nb_nodes; ++j ) 
            list_points[ j ] = e.pos( j );
        res = best_plane( a, b, c ,d, list_points );
        if ( res < applatissement )
            applatissement = res;
    }    


    T applatissement;
};

};

#endif /// BEST_PLANE_H


