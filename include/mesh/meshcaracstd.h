//
// C++ Interface: meshcaracstd
//
// Description: 
//
//
// Author: Hugo LECLERC <leclerc@lmt.ens-cachan.fr>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMTMESHCARACSTD_H
#define LMTMESHCARACSTD_H

#include "mesh.h"
#include "element.h"
#include "bar.h"
#include "bar_3.h"
#include "triangle.h"
#include "triangle_6.h"
#include "quad.h"
#include "quad_8.h"
#include "quad_9.h"
#include "tetra.h"
#include "tetra_10.h"
#include "hexa.h"
#include "hexa_20.h"

namespace LMT {

// template<int nb_var_inter,bool want_cubic=false,unsigned degree=1> struct ElementChoiceMeshCaracStd { typedef void NE; };
// template<> struct ElementChoiceMeshCaracStd<3,false,1> { typedef Tetra NE; };
// template<> struct ElementChoiceMeshCaracStd<2,false,1> { typedef Triangle NE; };
// template<> struct ElementChoiceMeshCaracStd<1,false,1> { typedef Bar NE; };
// template<> struct ElementChoiceMeshCaracStd<0,false,1> { typedef NodalElement NE; };
// 
// template<> struct ElementChoiceMeshCaracStd<3,false,2> { typedef Tetra_10 NE; };
// template<> struct ElementChoiceMeshCaracStd<2,false,2> { typedef Triangle_6 NE; };
// template<> struct ElementChoiceMeshCaracStd<1,false,2> { typedef Bar_3 NE; };
// template<> struct ElementChoiceMeshCaracStd<0,false,2> { typedef NodalElement NE; };
// 
// template<> struct ElementChoiceMeshCaracStd<3,true,1> { typedef Hexa NE; };
// template<> struct ElementChoiceMeshCaracStd<2,true,1> { typedef Quad NE; };
// template<> struct ElementChoiceMeshCaracStd<1,true,1> { typedef Bar NE; };
// template<> struct ElementChoiceMeshCaracStd<0,true,1> { typedef NodalElement NE; };
// 
// template<> struct ElementChoiceMeshCaracStd<3,true,2> { typedef Hexa_20 NE; };
// template<> struct ElementChoiceMeshCaracStd<2,true,2> { typedef Quad_8 NE; };
// template<> struct ElementChoiceMeshCaracStd<1,true,2> { typedef Bar_3 NE; };
// template<> struct ElementChoiceMeshCaracStd<0,true,2> { typedef NodalElement NE; };

template<int nb_var_inter, unsigned nb=0,unsigned degree=1> struct ElementChoiceMeshCaracStd { typedef void NE; };
template<> struct ElementChoiceMeshCaracStd<3,0,1> { typedef Tetra NE; };
template<> struct ElementChoiceMeshCaracStd<2,0,1> { typedef Triangle NE; };
template<> struct ElementChoiceMeshCaracStd<1,0,1> { typedef Bar NE; };
template<> struct ElementChoiceMeshCaracStd<0,0,1> { typedef NodalElement NE; };

template<> struct ElementChoiceMeshCaracStd<3,0,2> { typedef Tetra_10 NE; };
template<> struct ElementChoiceMeshCaracStd<2,0,2> { typedef Triangle_6 NE; };
template<> struct ElementChoiceMeshCaracStd<1,0,2> { typedef Bar_3 NE; };
template<> struct ElementChoiceMeshCaracStd<0,0,2> { typedef NodalElement NE; };

template<> struct ElementChoiceMeshCaracStd<3,1,1> { typedef Hexa NE; };
template<> struct ElementChoiceMeshCaracStd<2,1,1> { typedef Quad NE; };
template<> struct ElementChoiceMeshCaracStd<1,1,1> { typedef Bar NE; };
template<> struct ElementChoiceMeshCaracStd<0,1,1> { typedef NodalElement NE; };

template<> struct ElementChoiceMeshCaracStd<3,1,2> { typedef Hexa_20 NE; };
template<> struct ElementChoiceMeshCaracStd<2,1,2> { typedef Quad_8 NE; };
template<> struct ElementChoiceMeshCaracStd<1,1,2> { typedef Bar_3 NE; };
template<> struct ElementChoiceMeshCaracStd<0,1,2> { typedef NodalElement NE; };

template<> struct ElementChoiceMeshCaracStd<3,2,2> { typedef Hexa_20 NE; }; /// il faudrait un Hexa_26 !!!
template<> struct ElementChoiceMeshCaracStd<2,2,2> { typedef Quad_9 NE; };
template<> struct ElementChoiceMeshCaracStd<1,2,2> { typedef Bar_3 NE; };
template<> struct ElementChoiceMeshCaracStd<0,2,2> { typedef NodalElement NE; };
  
  
/*!

    Cette classe peut servir de paramètre à une classe Mesh pour définir un maillage sans champs (autre que pos, la position dans l'espace). On ne mémorise que la position des noeuds. On trouve d'autres exemples de MeshCarac pour des formulations dans le dossier formulation (n'hésitez pas à faire du copier/coller). Pour générer un MeshCarac personnel, on peut utiliser la fonction make_pb() dans le fichier Sconstruct (voir les tutoriels TODO  \a [Comment résoudre un problème éléments finis de façon générale] ).

    Voici des cas particuliers courants que vous pouvez copier/coller :
  
    \table exemples de définition de maillage 
        \a Tetra                       &   Mesh< MeshCaracStd< 3, 3, 0, double, 1  > > 
        \a Hexa                        &   Mesh< MeshCaracStd< 3, 3, 1, double, 1  > > 
        \a Triangle dans l'espace      &   Mesh< MeshCaracStd< 3, 2, 0, double, 1  > >  
        \a Quad dans l'espace          &   Mesh< MeshCaracStd< 3, 2, 1, double, 1  > >  
        \a Tetra_10                    &   Mesh< MeshCaracStd< 3, 3, 0, double, 2  > >
        \a Hexa_20                     &   Mesh< MeshCaracStd< 3, 3, 1, double, 2  > >  
        \a Triangle_6 dans l'espace    &   Mesh< MeshCaracStd< 3, 2, 0, double, 2  > >
        \a Quad_8 dans l'espace        &   Mesh< MeshCaracStd< 3, 2, 1, double, 2  > >
        \a Quad_9 dans l'espace        &   Mesh< MeshCaracStd< 3, 2, 2, double, 2  > >        
        \a Triangle                    &   Mesh< MeshCaracStd< 2, 2, 0, double, 1  > >  
        \a Quad                        &   Mesh< MeshCaracStd< 2, 2, 1, double, 1  > >  
        \a Triangle_6                  &   Mesh< MeshCaracStd< 2, 2, 0, double, 2  > >
        \a Quad_8                      &   Mesh< MeshCaracStd< 2, 2, 1, double, 2  > >
        \a Quad_9                      &   Mesh< MeshCaracStd< 2, 2, 2, double, 2  > > 
        

MeshCarac class (usable as Mesh<MeshCaracStd<...> > to instantiate a mesh) with
Tetras if nb_var_inter==3
Triangles if nb_var_inter==2
Bars if nb_var_inter==1
NodalElements if nb_var_inter==0

*/
template<unsigned d,unsigned nb_var_inter,unsigned nb=0,class T=double,unsigned degree=1,class NodalData=NodalStaticDataStd<T,d>,class GlobalData=VoidDMSet>
struct MeshCaracStd {
    ///
    typedef T Tpos;
    ///
    static const unsigned dim = d;
    ///
    typedef Vec<T,d> Pvec;
    ///
    typedef NodalData NodalStaticData;
    ///
    typedef GlobalData GlobalStaticData;
    /// inner is a workaround. NE can be Triangle, Tetra, ... end of elements<nvi_to_subs,skin> is marked by NE=void
    template<unsigned nvi_to_subs,unsigned skin,unsigned num_sub_element,unsigned inner=0>
    struct ElementChoice {
        typedef void NE;
        typedef DefaultBehavior BE;
        typedef VoidDMSet TData;
    };
    ///
    template<unsigned nvi_to_subs,unsigned skin,unsigned inner>
    struct ElementChoice<nvi_to_subs,skin,0,inner> {
        typedef typename ElementChoiceMeshCaracStd<nb_var_inter-(int)nvi_to_subs,nb,degree>::NE NE;
        typedef DefaultBehavior BE;
        typedef VoidDMSet TData;
    };
};
// template<unsigned d,unsigned nb_var_inter,bool want_cubic=false,class T=double,unsigned degree=1,class NodalData=NodalStaticDataStd<T,d>,class GlobalData=VoidDMSet>
// struct MeshCaracStd {
//     ///
//     typedef T Tpos;
//     ///
//     static const unsigned dim = d;
//     ///
//     typedef Vec<T,d> Pvec;
//     ///
//     typedef NodalData NodalStaticData;
//     ///
//     typedef GlobalData GlobalStaticData;
//     /// inner is a workaround. NE can be Triangle, Tetra, ... end of elements<nvi_to_subs,skin> is marked by NE=void
//     template<unsigned nvi_to_subs,unsigned skin,unsigned num_sub_element,unsigned inner=0>
//     struct ElementChoice {
//         typedef void NE;
//         typedef DefaultBehavior BE;
//         typedef VoidDMSet TData;
//     };
//     ///
//     template<unsigned nvi_to_subs,unsigned skin,unsigned inner>
//     struct ElementChoice<nvi_to_subs,skin,0,inner> {
//         typedef typename ElementChoiceMeshCaracStd<nb_var_inter-(int)nvi_to_subs,want_cubic,degree>::NE NE;
//         typedef DefaultBehavior BE;
//         typedef VoidDMSet TData;
//     };
// };


}

#endif
