//
// C++ Interface: vec
//
// Description: 
//
//
// Author: Hugo LECLERC <leclerc@lmt.ens-cachan.fr>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMT_CONTAINERS_vec_HEADER
#define LMT_CONTAINERS_vec_HEADER

#include "staticassert.h"
#include "allocator.h"
#include "ptrconstif.h"
#include "functional.h"
#include "simd.h"
#include "staticvalif.h"
#include "storebyrefif.h"
#include "delayedassignement.h"
#include "aresametype.h"
#include "typereduction.h"
#include "string_help.h"
#include "storage.h"
#include "structfortype.h"
#include "heteroexplpack.h"
#include "opwithincrement.h"

#include <typeinfo>
#include <sstream>
#include <string>

#define VECSTASSERTSAMESIZE(V1,V2)
// STATICASSERT(( V1.fixed_size==false || V2.fixed_size==false || V1.static_size==V2.static_size ))
#ifndef __INTEL_COMPILER
    #define VECASSERTSAMESIZE(V1,V2) { VECSTASSERTSAMESIZE(V1,V2) DEBUGASSERT( V1.size()==V2.size() ); }
#else
    #define VECASSERTSAMESIZE(V1,V2)
#endif
#define VECASSERTSAMESIZETHAN(V2,s2) { STATICASSERT( fixed_size==false || s2<0 || static_size==s2); DEBUGASSERT( size()==V2.size() ); }

namespace LMT {
    // ------------------------------------------------------------------------------------------------------------------------------------------------
    /*! typedef int T if VECOP is of kind Range<>, Heter, ... and so on. Usefull to avoid ambiguous
    */
    template<class VECOP> struct IsVecOp { typedef void T; };
    /*! res = true if Vec contains simd(i) method
    */
    template<class TV> struct VecSupportSIMD {
        static const unsigned res = false;
    };
    /*!
    */
    template<class TV> struct SimdSizeForVec {
        static const unsigned res = SimdSize<typename TV::template SubType<0>::T>::res;
    };
    /*!
    */
    template<class VECOP> struct HasIncrIndexAcces { static const bool res = false; };
    /*!
    */
    template<class VECOP> struct HasIndexAccess { static const bool res = true; };

/*!
\generic_comment Vec

        = description générale

            La classe générique Vec est une classe conteneur général qui permet de stocker des éléments de même types ou non. Les éléments sont repérés par un undice entier, le premier indice étant zéro. La taille peut être fixée à la compilation avec un gain en performance ou être variable, la gestion de la mémoire étant transparente. 
            Elle ressemble donc aux classes vector et valarray de la librairie standard avec davantage de fonctionnalités (utiles pour le calcul scientifique).
            Elle est très optimisée pour le calcul intensif et permet de faire simplement des opérations sur la totalité ou une partie des éléments. 
            On peut facilement récupérer les données de Matlab et les mettre dans un "Vec", travailler avec des threads Linux.
            Exemple de déclaration et d'utilisation :
            \code C/C++
                Vec<double> vx,vy,vz; // vecteur de double de taille variable.
                Vec<string,3> v1,v2,v3,v4("if","then","else"); // vecteur de taille 3 de chaine de caracères.
                double* tab ;

                    vz.resize(7);
                    vz(tab); // copie les 7 premiers éléments du tableau tab dans vz.
                    vx += abs(vy); // fait l'opération vx[i] = vx[i] + abs(vy[i]) pour tout i tq 0 <= i < vy.size() 
                    v3 = v1 + v2; // ou tout autre opération compatible avec le type.
                    vx.push_back( 1.0 ); // ajoute l'élément 1 à la fin du vecteur.
                    vx.pop_back();       // le retire.
                    std::cout << vx << std:endl ; // affiche vx 

        = Methodes et fonctions les plus utiles

            Les méthodes de la classe Vec sont :
                * v( std::ifstream("monfichier.txt") )  stocke dans v le contenu du fichier "monfichier.txt"
                * push_back(a) ajoute une copie de a dans le vecteur,
                * pop_back() retire le dernier élément ajouté,
                * back() retourne une copie du dernier élément ajouté,
                * front() retourne une copie du premier élément,
                * begin() retourne un pointeur (cad l'adresse) du premier élément  
                * resize(int n) qui fixe la taille du vecteur à n,
                * free() qui libère la mémoire,
                * et ne pas oublier reserve( unsigned int n) qui réserve de la place pour n éléments. Ainsi si vous connaissez environ le nombre d'éléments à ajouter via push_back() par exemple, vos appels push_back() seront bien plus rapides si vous appelez préalablement reserve().
                * random( size_vec ) qui est une méthode statique de Vec pour générer un vecteur aléatoire de double

            <strong> IMPORTANT : </strong> On ne peut pas parler de la classe Vec sans parler de la fonction \a apply et de ses varaintes qui permettent de manipuler les éléments d'un vecteur sans les parcourir avec une boucle for. Alors n'hésitez pas à consulter la doc de \a apply . 
            De plus il existe des exemples \a [Utilisation des conteneurs]

        = Des classes spécialisées

            Pour certains usages, certaines spécialisations de la classe Vec sont plus recommandées :
                * \a Vec<Sparse<TT>,static_size_> pour les vecteurs creux, i.e. on ne mémorise que les valeurs non nulles. 
                * \a Vec<VecCst<TT>,static_size_,int> pour vecteur constant. En fait on ne mémorise q'une seule valeur d'où un gain en mémoire.
                * \a Vec<VecDirac<TT>,static_size_,int> pour vecteur dont tous les éléments sont nuls sauf un (pensez à la base canonique).
                * \a Vec<Heterogeneous<Carac,nt,TT>,static_size_,int> pour des vecteurs d'éléments de type différent.
                * \a VecSelect pour vecteur permettant de filtrer les éléments d'un autre vecteur. 
                * \a Vec<VecHeavyside<TT>,static_size_,int> fait référence à la fonction de Heavyside qui est nulle pour x<0 et qui vaut un pour x>=0.
                * \a Vec<Intersection<V0,V1,const_v0,false,false>,-1,int> Cette classe permet après spécialisation de déterminer l'intersction de deux vecteurs.
                * \a Vec<Range<TT,static_begin,static_step>,static_size_,int> Cette classe permet après spécialisation de définir des vecteurs dont les éléments non nuls sont placés de façon périodique.
                * \a Vec<Replace<TV,TR>,static_size_,int> (à faire).
                * \a Vec<Splitted<TT,atomic_size>,-1,int> Cette classe permet après spécialisation de définir des vecteurs dont les éléments sont stockés par blocs. Ce stockage améliore les performances pour certains algorithmes. 
                * \a SubVec (à faire).
                * \a VecElemList (à faire).
                * \a Vec<VecPointedValues<TT>,static_size_,int> Cette classe permet après spécialisation de définir un vecteur de pointeurs.

        = Divers
            Vous pouvez aussi Consulter aussi la FAQ du wiki \a http://intranet/mediawiki/index.php/Vecteurs#id2442799

    \keyword Fonctionalité/Conteneur
    \keyword Vecteur
    \friend samir.amrouche@lmt.ens-cachan.fr
    \friend hugo.leclerc@lmt.ens-cachan.fr
    \keyword Mathématiques/Algèbre linéaire
    \author Hugo Leclerc
*/

    template<class TO=double,int static_size=-1,class IVO=typename IsVecOp<TO>::T> class Vec;
    
    /*! Spécialisation très générale */
    template<> class Vec<void,-1,void> {
    public:
         template<unsigned n> struct SubType { typedef void T; };
         static const unsigned nb_sub_type = 0;
         
         static const int static_size = -1;
         static const bool fixed_size = false;
         static const int sparsity_rate = 0;        
    };
    
    /*! */
    template<class TT,int s,class TO> struct SubComplex<Vec<TT,s,TO> > {
        typedef Vec<TT,s,TO> TV;
        typedef typename TypeInformation<TV>::template Variant<typename SubComplex<typename TypeInformation<TV>::SubType>::T>::T T;
    };
    
    /*! puet */
    template<class TV,class TSV,bool const_TV,bool const_TSV> struct SubVec;
    /*! pouet
    */
    struct SetSize {};

    /*!
    */
    template<class TVEC> struct CostAccess { static const unsigned res = 1; };

    /*!
      used in constructor
    */
    struct SetValues {};
    
    template<class TM,class Structure,class Storage,unsigned alignement,int nr> struct MatElem;
    // ------------------------------------------------------------------------------------------------------------------------------------------------
    /*! pouet
    */
    struct AssignSelfOp {
        template<class T,class BOP,class T2> void operator()(T &val,unsigned index,const BOP &op,const T2 &a) const { val=op(val,a[index]); }
        template<class Source,class BOP,class T2> void operator()(DelayedAssignement<Source> val,unsigned index,const BOP &op,const T2 &a) const { val.source.apply(op,a[index]); }
        template<class T,class BOP,class T2> void operator()(T &v1,T &v2,T &v3,T &v4,unsigned index,const BOP &op,const T2 &a) const { v1=op(v1,a[index]); v2=op(v2,a[index+1]); v3=op(v3,a[index+2]); v4=op(v4,a[index+3]); }
        
        template<class T,unsigned ne,class BOP,class T2> void operator()(SimdVec<T,ne> &val,unsigned index,const BOP &op,const SimdVec<T2,ne> &a) const { val=op(val,a[index]); }
        template<class T,unsigned ne,class BOP,class T2> void operator()(SimdVec<T,ne> &v1,SimdVec<T,ne> &v2,SimdVec<T,ne> &v3,SimdVec<T,ne> &v4,unsigned index,const BOP &op,const T2 &a) const {
            v1 = op( v1, a.simd<ne>(index  ) );
            v2 = op( v2, a.simd<ne>(index+1) );
            v3 = op( v3, a.simd<ne>(index+2) );
            v4 = op( v4, a.simd<ne>(index+3) );
        }
        
        template<class T,class BOP,class T2> void operator()(T &val,const BOP &op,const T2 &a) const { val = op( val, a ); }
        template<class S,class BOP,class T2> void operator()(DelayedAssignement<S> val,const BOP &op,const T2 &a) const { val.source.apply(op,a); }
        
        template<class TM,class Structure,class Storage,unsigned alignement,int nr,class BOP,class T2>
        void operator()(MatElem<TM,Structure,Storage,alignement,nr> val,const BOP &op,const T2 &a) const { val = op( val.conv(), a ); }

        
/*    
        template<class T2> void operator()(const T2 &v,unsigned i,TT *val) const { val[i] = (TT)v; }
        template<class T2> void operator()(const T2 &v1,const T2 &v2,const T2 &v3,const T2 &v4,unsigned i,TT *val) const { val[i+3] = (TT)v1; val[i+2] = (TT)v2; val[i+1] = (TT)v3; val[i+0] = (TT)v4; }
        template<class T2,unsigned ne> void operator()(const SimdVec<T2,ne> &v,unsigned i,TT *val) const { reinterpret_cast<SimdVec<TT,ne> &>(val[i]) = v; }
        template<class T2,unsigned ne> void operator()(const SimdVec<T2,ne> &v1,const SimdVec<T2,ne> &v2,const SimdVec<T2,ne> &v3,const SimdVec<T2,ne> &v4,unsigned i,TT *val) const {
            reinterpret_cast<SimdVec<TT,ne> &>(val[i+3*ne]) = v1;
            reinterpret_cast<SimdVec<TT,ne> &>(val[i+2*ne]) = v2;
            reinterpret_cast<SimdVec<TT,ne> &>(val[i+1*ne]) = v3;
            reinterpret_cast<SimdVec<TT,ne> &>(val[i+0*ne]) = v4;
        }*/

    };
    /*! pouet
    */
    template<class Op> struct VecSetBinOp1 { template<class TT,class VEC> void operator()(const TT &val,unsigned index,VEC &vec) const { vec[index] = op(val,vec[index]); } Op op; };
    /*! pouet
    */
    template<class Op> struct VecSetBinOp2 { template<class TT,class VEC> void operator()(TT &val,unsigned index,const VEC &vec) const { val = op(val,vec[index]); } Op op; };


    // ------------------------------------------------------------------------------------------------------------------------------------------------
    template<class Op> struct CastBool {
        static const bool support_implicit_cast_bool = false;
        template<class V> static bool cast_bool(const V &v,unsigned v0s,unsigned v1s) { Op::attention_conversion_automatique_vers_bool_non_autorisee(); return false; }
    };
    template<> struct CastBool<Equal> {
        static const bool support_implicit_cast_bool = true;
        template<class V> static bool cast_bool(const V &v,unsigned v0s,unsigned v1s) { return v0s==v1s and all(v); }
    };
    template<> struct CastBool<NotEqual> {
        static const bool support_implicit_cast_bool = true;
        template<class V> static bool cast_bool(const V &v,unsigned v0s,unsigned v1s) { return v0s!=v1s or at_least_one(v); }
    };

    // --------------------------------------------------------- TYPEPROMOTE --------------------------------------------------------
    template<class TT,int s> struct TypeInformation<Vec<TT,s> > {
        typedef typename Vec<TT,s>::template SubType<0>::T ST;
        static const int res = TypeInformation<ST>::res;
        static const int englobing = 10 + TypeInformation<ST>::englobing;
        static const bool is_scalar = false;
        typedef ST SubType;
        template<class TV> struct Variant { typedef Vec<TV,s> T; };
        template<class TV> struct DeepVariant { typedef Vec<typename TypeInformation<ST>::template DeepVariant<TV>::T,s> T; };
        static std::string type() {
            std::ostringstream ss;
            ss << "Vec<" << TypeInformation<ST>::type();
            if ( s>=0 )
                ss << "," << s;
            ss << "> ";
            return ss.str();
        }
        static const bool float_type = TypeInformation<ST>::float_type;
    };

    // ---------------------------------------------------------------------------------------------------------------------------------------
    /*! \relates Vec */
    template<typename TV> SimdVec<typename TV::template SubType<0>::T,1> get_simd(const TV &v,unsigned i,Number<1>) {
        return SimdVec<typename TV::template SubType<0>::T,1>( v[i+0] );
    }
    /*! \relates Vec */
    template<typename TV> SimdVec<typename TV::template SubType<0>::T,2> get_simd(const TV &v,unsigned i,Number<2>) {
        return SimdVec<typename TV::template SubType<0>::T,2>( v[i+1], v[i+0] );
    }
    /*! \relates Vec */
    template<typename TV> SimdVec<typename TV::template SubType<0>::T,4> get_simd(const TV &v,unsigned i,Number<4>) {
        return SimdVec<typename TV::template SubType<0>::T,4>( v[i+3], v[i+2], v[i+1], v[i+0] );
    }
    
    /*! \relates Vec */
    template<class TV,bool const_v,bool with_simd=true> struct IncrementalAccess {
        IncrementalAccess(const TV &vv) { v.val = &vv; }
        IncrementalAccess(TV &vv) { v.val = &vv; }
        typename TV::template SubType<0>::T &operator[](unsigned i) { return (*v.val)[i]; }
        typename TV::template SubType<0>::T operator[](unsigned i) const { return (*v.val)[i]; }
        PtrConstIf<TV,const_v> v;
    };
    /*! \relates Vec */
    template<class TV,bool with_simd> IncrementalAccess<TV,true ,with_simd> getIncrementalAccess(const TV &v) { return v; }
    /*! \relates Vec */
    template<class TV,bool with_simd> IncrementalAccess<TV,false,with_simd> getIncrementalAccess(TV &v) { return v; }

}
namespace std {
    template<class T,int s,class O>
    struct numeric_limits<LMT::Vec<T,s,O> > {
        typedef LMT::Vec<T,s,O> V;
        static V max() { V v; v.set( numeric_limits<typename V::template SubType<0>::T>::max() ); return v; }
        static V epsilon() { V v; v.set( numeric_limits<typename V::template SubType<0>::T>::epsilon() ); return v; }
    };
}

// ------------------------------------------------------------------------------------------------------------------------------------------------
#include "vecfull.h"

#include "vecstdop.h"

#include "vecref.h"
#include "vecsubvec.h"
#include "vecsparse.h"
#include "vecrange.h"
#include "veccst.h"
#include "vecdirac.h"
#include "vecheavyside.h"
#include "vecop.h"
#include "vecheter.h"
#include "vecsplitted.h"
#include "vecintersection.h"
#include "vecappend.h"
#include "vecfilter.h"
#include "vecapply.h"
#include "vecreplace.h"

#include "vecparticularcases.h"

#include "vecunary.h"
#include "vecbinary.h"

#include "vecasm.h"

namespace LMT {
    /*!
    Ex : Vec<std::string>::tokenize("toto:tata:1",':') returns ["toto","tata","1"]. Sep must be a ScalarType<T2>
    \relates Vec
    */
    template<class T2,class T3> Vec<T2> tokenize(T2 t,const T3 &sep) {
        Vec<T2> res; unsigned deb=0;
        for(unsigned i=0;i<t.size();++i) {
            if ( t[i]==sep ) {
                if ( i )
                    res.push_back( T2(&t[deb],&t[i]) );
                deb = i+1;
            }
        }
        if ( deb < t.size() )
            res.push_back( T2(&t[deb],&t[t.size()]) );
        return res;
    }

    #ifdef MATLAB_MEX_FILE
        struct VecSetReal { template<class TT,class VEC> void operator()(const TT &val,VEC &vec) { vec[index++] = LMT::real(val); } unsigned index; };
        struct VecSetImag { template<class TT,class VEC> void operator()(const TT &val,VEC &vec) { vec[index++] = LMT::imag(val); } unsigned index; };
        /*! Convert a LMT::Vec to a matlab one
            Type can be either Col or Row
            \relates Vec
        */
        template<class TT,int s,class Type> mxArray *to_matlab(const Vec<TT,s> &vec,const Type &t) {
            if ( Vec<TT,s>::nb_sub_type>1 ) {
                printf("error : in vec -> mxArray, mat must contain only one type\n");
                return NULL;
            }
            typedef typename Vec<TT,s>::template SubType<0>::T TV;
            if ( CanBeConvertedTo<TV,double>::res==false && CanBeConvertedTo<TV,std::complex<double> >::res==false ) {
                printf("error : in vec -> mxArray, vec must be be convertible to Vec<double> or Vec<complex<double> >\n");
                return NULL;
            }
            mxArray *res = mxCreateDoubleMatrix(
                Type::col_oriented     + vec.size() * (1-Type::col_oriented),
                (1-Type::col_oriented) + vec.size() * Type::col_oriented,
                ( CanBeConvertedTo<TV,double>::res ? mxREAL : mxCOMPLEX )
            );
            double *pr = mxGetPr(res);
            VecSetReal vr; vr.index = 0;
	    apply(vec, vr, pr );
            if ( CanBeConvertedTo<TV,double>::res==false ) {
                double *pr = mxGetPi(res);
                VecSetImag vi; vi.index = 0;
		apply(vec, vi, pr );
            }
            
            return res;
        }
    #endif

        /// contruct a simple static vector
        template<class T> Vec<T,1> vec( T a ) { return Vec<T,1>( a ); }

        /// contruct a simple static vector
        template<class T> Vec<T,2> vec( T a, T b ) { return Vec<T,2>( a, b ); }

        /// contruct a simple static vector
        template<class T> Vec<T,3> vec( T a, T b, T c ) { return Vec<T,3>( a, b, c ); }

        /// contruct a simple static vector
        template<class T> Vec<T,4> vec( T a, T b, T c, T d ) { return Vec<T,4>( a, b, c, d ); }

}

/*!
\generic_comment apply

    \friend samir.amrouche@lmt.ens-cachan.fr
    \friend hugo.leclerc@lmt.ens-cachan.fr

    = Description

        La fonction apply permet d'agir sur les éléments d'un vecteur sans recourir à une boucle (for). Cela améliore souvent les performances et le code s'en trouve aussi simplifié. 
        La syntaxe générale est :
        \code 
            apply( vecteur, opérateur, les paramètres (facultatifs) de l'opérateur)

        Certains types de fonctions apply agissent sur deux vecteurs en même temps. Leur syntaxe est alors :
        \code 
            apply( vecteur1, vecteur2, opérateur, les paramètres (facultatifs) de l'opérateur)


        Ainsi si vous souhaitez faire un calcul sur les éléments d'un vecteur, vous créerez un opérateur correspondant à l'algorithme avec éventuellement des paramètres si l'algorithme a des variantes. Cela fonctionne suivant le même principe que les filtres sur une image et comme les filtres, on peut aussi enchaîner simplement les traitements ; cad par exemple  si vous avez défini des opérateurs op1, op2, ... on peut créer un autre opérateur op utilisant les op1, op2 et l'appliquer sur un vecteur via apply.

        Suivant le même principe de fonctionnement on pourra aussi consulter la documentation des fonctions :
            * \a find
            * \a sort 
            * \a remove_if
            * \a find_elem

        Enfin la fabricaton de certains opérateurs peut être simplifiée par le concept de fonction lambda, voir ...
 
        Voici une liste d'exemples :
            * (à faire)
            * (à faire)


    = Les variantes

        = Un seul vecteur

            * \a apply_wi comme apply mais l'opération sur l'élément dépend de son indice dans le tableau (wi pour with index).
            * \a apply_range pour opérer sur des éléments consécutifs. 
            * \a apply_range_stride pour opérer sur des éléments régulièrement espacés.
            * \a apply_on_number pour opérér sur un seul élément (éxcepté pour les vecteurs hétérogènes, un simple v[i] est plus efficace). 
            * \a apply_range_by_n 
            * \a apply_nz_wi est plutôt utilisée pour les vecteurs creux ( \a Vec<Sparse<TT>,static_size_> ) car le traitement n'est fait que sur les éléments instanciés (donc à priori non nul).
            * \a aply_mt et ses variantes permet de faire les calculs sur plusieurs threads d'un même processeur.

        = Deux vecteurs

            * \a apply_ij qui opère sur toutes les paires formées d'un élément de chaque vecteur (équivaut à deux boucles imbriquées).  
            * \a apply_i_supeq_j même chose avec le premier indice supérieur ou égal au second.
            * \a apply_i_sup_j même chose avec le premier indice supérieur au second.
            * 

    \relates generate
    \relates find
    \relates sort
*/



#endif // LMT_CONTAINERS_vec_HEADER

