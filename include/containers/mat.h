
//
// C++ Interface: mat
//
// Description:
//
//
// Author: Hugo LECLERC <hugo.leclerc@lmt.ens-cachan.fr>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMT_mat_HEADER
#define LMT_mat_HEADER

#include "vec.h"
#include "structure.h"
#include "storage.h"
#include "matelem.h"
#include <iomanip>
#include <fstream>
#include "evaluate_nb_cycles.h"

namespace LMT {

template<class MATOP> struct IsMatOp { typedef void T; };

/*!
\generic_comment Mat

    = Introduction

        Cette classe permet de stocker et manipuler des tableaux à deux dimensions (cad des matrices) comme sur Matlab mais avec la LMT++ vous irez beaucoup plus vite et vous pourrez construire de très grosses matrices.
        Si vous utilisez des matrices denses sans options particulières, la FAQ ci-dessous vous expliquera comment :
             * [[#defimat|définir et instancier une matrice]]
             * [[#manimat|manipuler une matrice]]
             * [[#affimat|afficher une matrice]]
             * [[#resolumat|résoudre un système]]

        Sinon il existe différentes types de matrices commes les matrices creuses ou symétrques ou les matrices adaptées pour certains algorithmes ou implémentations.

    = Spécialisations

        Pour des utilisations particulières, il est préférable d'utiliser des spécialisations de la classe Mat pour économiser des ressources ou pour aller plus vite.

            = Matrice creuse
                Les classes qui servent à préciser les types de stockage creux sont \a Sparse , \a SparseCholMod , \a SparseLU , \a SparseLine , \a SparseUMFPACK . Le format \a SparseLU qui sert pour la décomposition LU est traité à la dernière section.
                A FINIR
            = Matrice symétrique
                Les matrices symétriques se définissent en spécialisant le type \a Mat de base. On a les possibilités \a Sym , \a AntiSym , \a Herm . Pour des détails et des exemples, allez à la FAQ \a #1
            = Matrice spéciale pour la décomposition LU
                Pour la décomposition LU, il y a un format de stockage particulier spécialisé par la classe \a SparseLU . Pour un tutorial, allez à \a decompositiond'unematrice  et la FAQ \a #17 , \a #18
    = FAQ

        Cette FAQ est tirée de celle de l'intranet :

        * \anchor defimat Définition, instantiation
            * \a #1 Les paramètres template des matrices et exemples de déclaration
            * \a #8 Pour créer une matrice (ici taille 100,20) constituee du meme nombre (ici 1.0) partout
            * \a #5 Pour affecter la valeur 4 a toute la matrice
            * \a #3 Pour modifier la taille d'une matrice M
            * \a #15 Pour créer une matrice diagonale possedant le vecteur V sur sa diagonale;
            * \a #6 Pour assigner une matrice M2 à une matrice M1;
            * \a #11 Pour mettre un vecteur V dans une ligne ou colonne donnee i
            * \a #12 Pour mettre une matrice dans une autre
            * \a #20 Pour initialiser une matrice dont les valeurs sont contenues dans un fichier texte
            * \a #13 Pour concaténer deux matrices
            * \a #22 Pour extraire une sous-matrice (à faire).
        * \anchor manimat Manipulation
            *  pour accéder à l'élément de ligne i et de colonne j, on fait M(i,j) (les indices commencent à zéro).
            * \a #7 Pour faire des operations matrice-vecteur
            * \a #9 Pour obtenir le nombre de colonnes ou de lignes
            * \a #10 Pour transposer une matrice
            * \a #14 Pour modifier la diagonale de la matrice (ajouter 3)
            * \a #19 Pour vraiment inverser une matrice (à ne pas utiliser sans l'autorisation formelle de la Leclerc Corp. Inc.)
            * \a #21 Pour calculer la moyenne, la variance, le min, le max des éléments d'une matrice
        * \anchor affimat Affichage
            * \a #4 Pour afficher la matrice
            * \a #16 Pour afficher la forme des elements d'une matrice M sparse ou skyline
        * \anchor resolumat Résolution d'équations linéaires
            * \a #17 Pour faire une factorisation de cholesky d'une matrice M et la stocker dans une nouvelle matrice I (M doit etre du type : Mat<type, Sym<>, stockage >)
            * \a #18 Pour résoudre un problème directement sans factoriser préalablement ni inverser de matrice

        \anchor 1 Les paramtres template des matrices Mat<T,STR,STO> signifient :
        * T est le type scalaire de base (double, float, long double, std::complex<double>, ...).
        * STR représente les propriétés de symétrie de la matrice (générale, symétrique, antisymétique, hermitienne, diagonale, triangulaire supérieure, triangulaire inférieure).
            * Gen<s1,s2> pour matrice pleine sans propriétés. Si on connaît la taille de la matrice avant la compilation, s1,s2 sont respectivement le nombre de lignes et de colonnes sinon s1 et s2 valent -1 et la matrice pourra changer de taille pendant l'éxécution du programme.
            * Sym<s> pour matrice symétrique. s est la taille statique (fixée à la compilation) ou -1 si la taille n'est connue que durant l'éxécution.
                <strong >ATTENTION </strong> : contrairement à MATLAB, m(a,b)+=40 ajoute 40 en haut et en bas de la matrice (a,b et b,a).
            * Herm<s> pour matrice hermitienne.
            * AntiSym<s> pour matrice antisymetrique.
            * Diag<s> pour matrice diagonale.
            * TriUpper<s> pour matrice carré triangulaire superieure.
            * TriLower<s> pour matrice carré triangulaire inferieure.

        * STO représente le type de stockage (pleine, creuse).
            * Dense<Orientation> pour matrice pleine. Si Orientation= classe Row alors les éléments sont rangés par lignes sinon ils sont rangés par colonne (rangement par defaut) avec la classe Col donc on écrira soit Dense<Row>, Dense<Col>.
            * SparseLine<Orientation> pour matrice creuse. Pour la classe Orientation, c'est la même manière que la classe Dense.

        Voici quelques exemples importants de déclaration :

                \code C/C++
                    Mat<float> M; // équivaut à Mat<double,Gen<-1,-1> >  donc M est une matrice pleine dont la taille n'est pas fixée à la compilation.
                    Mat<double> M(10,10,0.); // matrice de taille 10 10 initialisée à 0.
                    Mat<float,SparseLine<> > M; // Matrice creuse sans symétrie avec rangement par défaut.
                            // laisser un espace entre les > >.

                    Mat<double,Sym<>,Dense<Col> > M;
                    int n ;

                    n=5;
                    M.resize(n) ;
                    M(4,2) = 7;
                    M[0] = 1;
                    M[1] = 2;
                    M[2] = 3;
                    M[3] = 4;
                    M[4] = 5;
                    M[5] = 6;
                    PRINTN(M);
                        // M ->
                        // 1 2 4 0 0
                        // 2 3 5 0 0
                        // 4 5 6 0 7
                        // 0 0 0 0 0
                        // 0 0 7 0 0

                    Mat<double,Sym<>,Dense<Row> > M; // REMARQUE : on a remplacé Col par Row.
                    int n ;

                    n=5;
                    M.resize(n) ;
                    M(4,2) = 7;
                    M[0] = 1;
                    M[1] = 2;
                    M[2] = 3;
                    M[3] = 4;
                    M[4] = 5;
                    M[5] = 6;
                    PRINTN(M);
                        // M ->
                        // 1 2 3 4 5
                        // 2 6 0 0 0
                        // 3 0 0 0 7
                        // 4 0 0 0 0
                        // 5 0 7 0 0
                        // remarquer la différence de stockage.


        \anchor 3 Pour modifier la taille d'une matrice M
            \code C/C++
                M.resize(m); // -> m*m
                M.resize(m,n); // -> m*n
                M.resize(m,n,10.0); // -> m*n et remplit avec des 10.0

        \anchor 4 Pour afficher la matrice
            \code C/C++
                std::cout << M << std::endl;
                PRINT(M);
                PRINTN(M); revient à la ligne
        \anchor 5 Pour affecter la valeur 4 a toute une matrice Dense
            \code C/C++
                M.set(4);
        \anchor 6 Pour affecter la valeur 0 à toute une matrice SparseLine
            \code C/C++
                M.clear();
        \anchor 7 Pour assigner une matrice M2 a une matrice M1;
            \code C/C++
                M1=M2;
        \anchor 8 Pour faire des operations matrice-vecteur
            \code C/C++
                M*V
        \anchor 9 Pour créer une matrice (ici de taille 100,20) constituée du meme nombre (ici 1.0) partout
            \code C/C++
                Mat<double> M(100,20,1.0);
                Pour obtenir le nombre de colonnes ou de lignes
                M.nb_rows(); // le nombre de lignes
                M.nb_cols(); // le nombre de colonnes
        \anchor 10 Pour transposer une matrice M
            \code C/C++
                M2=trans(M)
        \anchor 11 Pour mettre un vecteur V dans une ligne ou colonne donnee i
            \code C/C++
                M.row(i)=V;
                M.col(i)=V;
        \anchor 12 Pour mettre une matrice M2 dans une autre, disons M.
            On stocke dans un vecteur d'entiers V1 de même taille que le nombre de lignes de M2 les indices des lignes de M que l'on veut remplir : e.g. si V1[0]= 3 on mettra les éléments de la première ligne de M2 dans la ligne 3 de M.
            On stocke de même dans V2 les indices des colonnes de M. Puis on écrit :
            \code C/C++
                M(V1,V2)=M2;
                M[V1]=M2;  // Dans le cas où V2=V1
        \anchor 13 Pour concaténer deux matrices sur un exemple.
            \code C/C++
                Mat<double> m( 2, 2, 0.0 ), n( 2, 2, 0.0 );
                Mat<double> r( 2, 4, 0.0 );
                m.diag() = 1;
                n.diag() = 2;
                r( range(2), range(2) + 0 ) = m;
                r( range(2), range(2) + 2 ) = n;
                PRINTN( r );

        \anchor 14 Pour modifier la diagonale de la matrice (ajouter 3)
            \code C/C++
                M.diag() += 3;
        \anchor 15 Pour créer une matrice diagonale possedant le vecteur V sur sa diagonale;
            \code C/C++
                diag(V);
        \anchor 16 Pour afficher la forme des elements d'une matrice M sparse ou skyline
            \code C/C++
                display_structure(M);
        \anchor 17 Pour faire une factorisation de cholesky d'une matrice M et la stocker dans une nouvelle matrice I (M doit etre du type : Mat<type, Sym<>, stockage >)
            \code C/C++
                    Inv<type, Sym<>, stockage > I=inv(M);
        \anchor 18 Pour resoudre un probleme directement sans factoriser préalablement. Si vous devez résoudre des systèmes linéaires ayant la même matrice, il vaut mieux factoriser.
            \code C/C++
                    V=inv(M)*R; // V est la solution de l'équation M X= R

        \anchor 19 Pour vraiment inverser une matrice H et la mettre dans S:
            \code C/C++
                include <containers/mat_true_inv.h>

                LMT::Mat<double,LMT::Gen<6,6> > H;
                H = bla bla;
                LMT::Mat<double,LMT::Gen<6,6> > S = true_inv( H );

        \anchor 20 Pour initialiser une matrice dont les valeurs sont contenues dans un fichier texte
            \code C/C++
                using namespace LMT;

                int main() {

                    Mat<double> mat;

                    std::ifstream desc("chemin/fichier");
                    read_ascii_mat_file( mat, desc );
                }
        \anchor 21 Pour calculer la moyenne, la variance, le min et le max des éléments d'une matrice
            \code C/C++
                mean(M);variance(M);min(M);max(M);

    = Divers

        * Le produit de deux matrices A et B, A * B, peut être lent, voire très lent. Cela est dû à une mauvaise disposition des éléments de la matrice B qui sont rangées en lignes alors qu'une disposition en colonne est préférable. Comme à cette heure, on ne peut convertir une matrice stocké en lignes en une matrice stockée en colonne mais qu'il est possible de faire la conversion inverse (colonne en ligne) et que la fonction transpose transforme une matrice lignes en une matrice colonnes, on contourne le problème en écrivant  A * B = A * trans( TM( trans( B ) ) ) où TM est le type de votre matrice ligne.
        Un exemple :
        \code C/C++
           typedef Mat< double, SparseLine<> > TM;

           TM A, B, RES;

           RES = A * trans( TM( trans( B ) ) )

        * Le produit de deux matrices est représenté par un type qui n'est pas une matrice mais une classe qui peut calculer un terme quelconque du produit (A * B)_i_j. C'est très performant pour ensuite faire des produits matrice fois vecteur, ça ne l'est pas si on souhaite faire que des produits de matrices. Il faut alors dans ce cas, faire une conversion explicite en vrai matrice comme par exemple :
        \code C/C++
            typedef Mat< double, SparseLine<>> TM;

            TM A, B, C, RES;

            RES = A * TM( B * C )
            RES = A * B * C;

        Le premier calcul est bien plus performant que le second grâce à la conversion de <strong> B * C </strong> en <strong> TM </strong> .


        * si mat est de type matrice, mat.diag(), mat.row(), mat.col() peut être affecté (i.e. placé à gauche du signe =).

        * Il existe une FAQ sur l'intranet http://intranet/mediawiki/index.php/Matrices

        <strong> SIMD alignement for Row on Lower and Col on Upper </strong>

    \keyword Fonctionalité/Conteneur
    \keyword Matrice
    \friend samir.amrouche@lmt.ens-cachan.fr
    \friend hugo.leclerc@lmt.ens-cachan.fr
    \keyword Mathématiques/Algèbre linéaire
    \author Hugo Leclerc
*/

template<class T=double,class Structure=Gen<-1,-1>,class Storage=Dense<Col>,class ismatop=typename IsMatOp<T>::T>
class Mat;

template<class TM,bool const_tm,class TV1,class TV2=void> struct SubMat;

/*!
    \relates Mat
    \friend hugo.leclerc@lmt.ens-cachan.fr
    \keyword Mathématiques/Algèbre linéaire
    \author Hugo Leclerc
*/
template<class T,class Structure,class Storage,class OP>
void display( std::ostream &os, const Mat<T,Structure,Storage,OP> &m, int precision = 16 ) {
    int max_size = 0;
    for(unsigned i=0;i<m.nb_rows();++i)
        for(unsigned j=0;j<m.nb_cols();++j)
            max_size = max( max_size, (int)to_string( m( i, j ), precision ).size() );
    for(unsigned i=0;i<m.nb_rows();++i) {
        for(unsigned j=0;j<m.nb_cols();++j) {
            std::string s = to_string( m( i, j ), precision );
            os << s << std::string( max_size + 1 - s.size(), ' ' );
        }
        os << "\n";
    }
}

/*!
    \relates Mat
    \friend hugo.leclerc@lmt.ens-cachan.fr
    \keyword Mathématiques/Algèbre linéaire
    \author Hugo Leclerc
*/
template<class T,class Structure,class Storage,class OP>
std::ostream &operator<<( std::ostream &os, const Mat<T,Structure,Storage,OP> &m ) {
    display( os, m, 16 );
    return os;
}
/*!
    \relates Mat
    \friend hugo.leclerc@lmt.ens-cachan.fr
    \keyword Mathématiques/Algèbre linéaire
    \author Hugo Leclerc
*/
template<class T,class Structure,class Storage,class OP>
std::istream &operator>>(std::istream &is,Mat<T,Structure,Storage,OP> &m) {
    for(unsigned i=0;i<m.nb_rows();++i) {
        for(unsigned j=0;j<m.nb_cols();++j) {
            T v;
            is >> v;
            if ( v )
                m(i,j) = v;
        }
    }
    return is;
}

template<class TT,class Structure,class Storage,class OP>
std::istream &line_input( std::istream &s, Mat<TT,Structure,Storage,OP> &m ) {
    typename Mat<TT,Structure,Storage,OP>::T t;
    for(unsigned i=0;i<m.nb_rows();++i)
        for(unsigned j=(i+(!Structure::need_diag))*(1-Structure::need_lower);
                j<m.nb_cols()*Structure::need_upper+(i+Structure::need_diag)*(!Structure::need_upper);++j) {
            line_input( s, t);
            m(i,j) = t;
        }
    return s;
}
/*!
Pour les fichiers du genre
 0 1 2
 3 4 5
 6 7 8

\keyword Mathématiques/Algèbre linéaire/Opération
\keyword Matrice/Opération
\keyword Matrice/Import

*/
template<class T,class Structure,class Storage,class OP>
void read_ascii_mat_file( Mat<T,Structure,Storage,OP> &mat, std::istream &f ) {
    while ( true ) {
        std::string str;
        getline( f, str );
        if ( not f )
            break;
        std::istringstream ss( str );
        Vec<T> v;
        ss >> v;
        mat.resize( mat.nb_rows() + 1, v.size() );
        mat.row( mat.nb_rows() - 1 ) = v;
    }
}

template<class TTV,class Structure,class Storage,class OP>
struct TypeInformation<Mat<TTV,Structure,Storage,OP> > {
    typedef typename Mat<TTV,Structure,Storage,OP>::T TT;
    static const int res = TypeInformation<TT>::res;
    static const int englobing = 20 + TypeInformation<TT>::englobing;
    static const bool is_scalar = false;
    typedef TT SubType;
    template<class TV> struct Variant { typedef Mat<TV,Structure,Storage,OP> T; };
    template<class TV> struct DeepVariant { typedef Mat<typename TypeInformation<TT>::template DeepVariant<TV>::T,Structure,Storage,OP> T; };
    static std::string type() { return "Mat<"+TypeInformation<TT>::type()+"> "; }
    static const bool float_type = TypeInformation<TT>::float_type;
};

/**
*/
template<class STR,class Op> struct StructureTypeWithOp { typedef Gen<> T; };

template<int s,class Op> struct StructureTypeWithOp<AntiSym<s>,Op> {
    typedef typename AlternativeOnType< ConserveZeroes<Op>::res,
        Gen<s,s>,
        AntiSym<s>
    >::T T;
};
template<int s,class Op> struct StructureTypeWithOp<Diag<s>,Op> {
    typedef typename AlternativeOnType< ConserveZeroes<Op>::res,
        Gen<s,s>,
        Diag<s>
    >::T T;
};
template<int s1,int s2,class Op> struct StructureTypeWithOp<TriUpper<s1,s2>,Op> {
    typedef typename AlternativeOnType< ConserveZeroes<Op>::res,
        Gen<s1,s2>,
        TriUpper<s1,s2>
    >::T T;
};
template<int s1,int s2,class Op> struct StructureTypeWithOp<TriLower<s1,s2>,Op> {
    typedef typename AlternativeOnType< ConserveZeroes<Op>::res,
        Gen<s1,s2>,
        TriLower<s1,s2>
    >::T T;
};
template<int s,class Op> struct StructureTypeWithOp<Herm<s>,Op> {
    typedef typename AlternativeOnType< ConserveZeroes<Op>::res,
        Gen<s,s>,
        Herm<s>
    >::T T;
};

///
template<class T,class STR,class STO,class IO>
T mean(const Mat<T,STR,STO,IO> &m) {
    T res = T(0);
    for(unsigned i=0;i<m.nb_rows();++i)
        res += sum( m.row(i) );
    return res / T( m.nb_rows() * m.nb_cols() );
}

///
template<class T,class STR,class STO,class IO>
T variance(const Mat<T,STR,STO,IO> &m) {
    return mean( ( m - mean(m) ) * ( m - mean(m) ) );
}

template<class T,class STR,class STO,class IO>
inline T standard_deviation(const Mat<T,STR,STO,IO> &m) {
    return sqrt( variance( m ) );
}

///
template<class T,class STR,class STO,class IO>
typename Mat<T,STR,STO,IO>::T max(const Mat<T,STR,STO,IO> &m) {
    typename Mat<T,STR,STO,IO>::T res = -std::numeric_limits<T>::max();
    for(unsigned i=0;i<m.nb_rows();++i)
        res = max( res, max( m.row(i) ) );
    return res;
}

///
template<class T,class STR,class STO,class IO>
typename Mat<T,STR,STO,IO>::T min(const Mat<T,STR,STO,IO> &m) {
    typename Mat<T,STR,STO,IO>::T res = std::numeric_limits<T>::max();
    for(unsigned i=0;i<m.nb_rows();++i)
        res = min( res, min( m.row(i) ) );
    return res;
}

template<class T0,class Str0,class Sto0,class T1,class Str1,class Sto1>
void construct_matrix( Mat<T0,Str0,Sto0> &res, const Mat<T1,Str1,Sto1> &src );

};

#include "vec_mt.h"
#include "vecsubmat.h"
#include "matdense.h"
#include "matsparse.h"
#include "matskyline.h"
#include "mattrans.h"
#include "matop.h"
#include "matunary.h"
#include "matbinary.h"
#include "matsubmat.h"
#include "matmult.h"
#include "matinv.h"
#include "matparticularcases.h"
#include "matpointed.h"
#include "matsparselu.h"
#include "MatWithTinyBlocks.h"
#include "construct_matrix.h"

namespace LMT {

/*!
Pour construire une matrice diagonale à partir d'un vecteur
\friend hugo.leclerc@lmt.ens-cachan.fr
\keyword Mathématiques/Algèbre linéaire/Opération
\relates Mat
*/
template<class TV,int s>
Mat<TV,Diag<s> > diag(const Vec<TV,s> &vec) {
    return Mat<TV,Diag<s> >(vec.size(),vec.size(),vec);
}


#ifdef MATLAB_MEX_FILE
    /** Convert a LMT::Mat to a matlab one
        \relates Mat
    */
/*!

    Convert a LMT::Mat to a matlab one
    \friend hugo.leclerc@lmt.ens-cachan.fr
    \keyword Mathématiques/Algèbre linéaire/Opération
    \author Hugo Leclerc
*/
    template<class TT,class STR,class O,class IO>
    mxArray *to_matlab(const Mat<TT,STR,Dense<O>,IO> &mat) {
        typedef typename Mat<TT,STR,Dense<O>,IO>::T TV;
        /* Test if double (mxReal) or complex<double> (mxComplex)*/
        if ( CanBeConvertedTo<TV,double>::res==false && CanBeConvertedTo<TV,std::complex<double> >::res==false ) {
            printf("error : in Mat -> mxArray, mat must be be convertible to Mat<double> or Mat<complex<double> >\n");
            return NULL;
        }
        /* Create the ouput matrix */
        mxArray *res = mxCreateDoubleMatrix(
                        mat.nb_rows(),
                        mat.nb_cols(),
                        ( CanBeConvertedTo<TV,double>::res ? mxREAL : mxCOMPLEX )
                    );
        /* Pointer to the real part*/
        double *pr = mxGetPr(res);
        for(unsigned int i=0;i<mat.nb_rows();++i) {
            for(unsigned int j=0;j<mat.nb_cols();++j) {
                pr[(mat.nb_rows()*j)+i]=LMT::real(mat(i,j));
            }
        }
        /* Pointer to the imaginary part if necessary*/
        if ( CanBeConvertedTo<TV,double>::res==false ) {

            double *pi = mxGetPi(res);
            for(unsigned int i=0;i<mat.nb_rows();++i) {
                for(unsigned int j=0;j<mat.nb_cols();++j) {
                    pi[(mat.nb_rows()*j)+i]=LMT::imag(mat(i,j));
                }
            }
        }
        return res;
    }
#endif

/*!
    Affichage de la structure d'une matrice sous forme d'image.
    Cette fonction fait une mise à l'echelle linéaire par morceaux par rapport à la valeur maximale de la matrice.
    C'est donc plus représentatif que le résultat de la fonction \a display_image .

    \keyword Utilitaires
    \keyword Fonctionalité/Affichage
    \friend samir.amrouche@lmt.ens-cachan.fr
    \friend hugo.leclerc@lmt.ens-cachan.fr
    \keyword Mathématiques/Algèbre linéaire/Opération
*/
template<class T,class Str,class Sto,class IO>
void display_structure(const Mat<T,Str,Sto,IO> &mat,const char *name_file="res") {
    typedef typename Mat<T,Str,Sto,IO>::T TT;
    using namespace std;

    ofstream f(name_file);

    double m = 0.0;
    for(unsigned l=0;l<mat.nb_rows();++l)
        for(unsigned c=0;c<mat.nb_cols();++c)
            m = max( m, std::abs((TT)mat(l,c)) );

    for(unsigned l=0;l<mat.nb_rows();++l)
        for(unsigned c=0;c<mat.nb_cols();++c)
            f.put(  255 - ((TT)mat(l,c)!=0.0) * 200 - (std::abs((TT)mat(l,c))>1e-6*m) * 55 );

    f.close();

    ostringstream s,s2;
    s << "display -depth 8 -size " << mat.nb_cols() << "x" << mat.nb_rows() << " gray:" << name_file;
    system(s.str().c_str());

    s2 << "convert -depth 8 -size " << mat.nb_cols() << "x" << mat.nb_rows() << " gray:" << name_file << " " << name_file << ".png";
    system(s2.str().c_str());
}

template <class T, class STR, class STO, class IO>
Mat<T,Sym<>,STO,IO> sym (const Mat<T,STR,STO,IO> &M) {
    return T(0.5)*(M+trans(M));
}

template <class T, class STR, class STO, class IO>
Mat<T,Sym<>,STO,IO> antisym (const Mat<T,STR,STO,IO> &M) {
    return T(0.5)*(M-trans(M));
}

template <class T, int s, bool b, class STO, class IO>
const Mat<T,Sym<s,b>,STO,IO> &sym (const Mat<T,Sym<s,b>,STO,IO> &M) {
    return M;
}

#ifndef WITHOUTCOMPLEX
    template <class T, class STR, class STO, class IO>
    Mat<T,Sym<>,STO,IO> sym (const Mat<std::complex<T>,STR,STO,IO> &M) {
        return T(0.5)*(M+trans(conj(M)));
    }
    template <class T, int s, class STO, class IO>
    const Mat<T,Sym<s>,STO,IO> &sym (const Mat<std::complex<T>,Herm<s>,STO,IO> &M) {
        return M;
    }
#endif

/*!
    Objectif :
        convertir un vecteur en une matrice M symétrque dense tel que M( i, j ) = v[ i ] * v[ j ]

    \keyword Matrice/Conversion

*/
template <class T, int s>
Mat<T,Sym<s>,Dense<> > vec2matsym (const Vec<T,s> &v) {
    Mat<T,Sym<>,Dense<> > res;
    res.resize(v.size());
    for (int i=0;i<v.size();i++)
        for (int j=i;j<v.size();j++)
            res(i,j)=v[i]*v[j];
    return res;
}

};

#endif // LMT_mat_HEADER
