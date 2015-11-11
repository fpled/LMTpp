//
// C++ Interface: pyramid
//
// Description: 
//
//
// Author: David Violeau <violeau@lmt.ens-cachan.fr>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMT_pyramid_HEADER
#define LMT_pyramid_HEADER

#include "quad.h"
#include "triangle.h"
#include "tetra.h"

namespace LMT {

// --------------------------------------------------------------------------------------------------------
/*!
    Pyramide à 5 noeuds, à base carrée dans le plan xOy.
    
    Dans l'espace de référence cela correspond au volume de points M(x,y,z) tels que :
        * x, y , z >= 0
        * x + z <= 1
        * y + z <= 1
        
    \keyword Maillage/Elément
    \friend samir.amrouche@lmt.ens-cachan.fr
    \friend hugo.leclerc@lmt.ens-cachan.fr
*/
struct Pyramid {
    static const unsigned nb_var_inter = 3;
    static const unsigned nb_nodes = 5;
    static const char *name() { return "Pyramid"; }
    static const char *can_directly_be_represented_by() { return "Pyramid"; }
};

// --------------------------------------------------------------------------------------------------------
template<> struct NbChildrenElement<Pyramid,1> { static const unsigned res = 5; };
template<> struct NbChildrenElement<Pyramid,2> { static const unsigned res = 8; };
template<> struct NbChildrenElement<Pyramid,3> { static const unsigned res = 5; };


template<unsigned n> struct TypeChildrenElement<Pyramid,0,n> { typedef Pyramid T; };
template<> struct TypeChildrenElement<Pyramid,1,0> { typedef Quad T; };
template<unsigned n> struct TypeChildrenElement<Pyramid,1,n> { typedef Triangle T; };
template<unsigned n> struct TypeChildrenElement<Pyramid,2,n> { typedef Bar T; };
template<unsigned n> struct TypeChildrenElement<Pyramid,3,n> { typedef NodalElement T; };

template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Pyramid,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<1> nvi_to_subs) {
    het.add_element(e,ch,Quad(),e.node(0),e.node(1),e.node(2),e.node(3));
    het.add_element(e,ch,Triangle(),e.node(0),e.node(1),e.node(4));
    het.add_element(e,ch,Triangle(),e.node(1),e.node(2),e.node(4));
    het.add_element(e,ch,Triangle(),e.node(2),e.node(3),e.node(4));
    het.add_element(e,ch,Triangle(),e.node(3),e.node(0),e.node(4));
}

template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Pyramid,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<2> nvi_to_subs) {
    het.add_element(e,ch,Bar(),e.node(0),e.node(1));
    het.add_element(e,ch,Bar(),e.node(0),e.node(3));
    het.add_element(e,ch,Bar(),e.node(0),e.node(4));
    het.add_element(e,ch,Bar(),e.node(1),e.node(2));
    het.add_element(e,ch,Bar(),e.node(1),e.node(4));
    het.add_element(e,ch,Bar(),e.node(2),e.node(4));
    het.add_element(e,ch,Bar(),e.node(2),e.node(3));
    het.add_element(e,ch,Bar(),e.node(3),e.node(4));
}
template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Pyramid,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<3> nvi_to_subs) {
    het.add_element(e,ch,NodalElement(),e.node(0));
    het.add_element(e,ch,NodalElement(),e.node(1));
    het.add_element(e,ch,NodalElement(),e.node(2));
    het.add_element(e,ch,NodalElement(),e.node(3));
    het.add_element(e,ch,NodalElement(),e.node(4));
}

// --------------------------------------------------------------------------------------------------------
template<class TN,class TNG,class TD,unsigned NET,class TM,class T>
void update_edge_ratio(const Element<Pyramid,TN,TNG,TD,NET> &e,TM &m,T &edge_ratio) {
    T edge_length_0 = (m.get_children_of( e, Number<1>() )[ 0 ])->measure_virtual();
    T edge_length_1 = (m.get_children_of( e, Number<1>() )[ 1 ])->measure_virtual();
    T edge_length_2 = (m.get_children_of( e, Number<1>() )[ 2 ])->measure_virtual();
    T edge_length_3 = (m.get_children_of( e, Number<1>() )[ 3 ])->measure_virtual();
    T edge_length_4 = (m.get_children_of( e, Number<1>() )[ 4 ])->measure_virtual();
    T edge_min_1 = min( edge_length_0, edge_length_1, edge_length_2 );
    T edge_min_2 = min( edge_length_3, edge_length_4 );
    T edge_max_1 = max( edge_length_0, edge_length_1, edge_length_2 );
    T edge_max_2 = max( edge_length_3, edge_length_4 );
    edge_ratio = min( edge_min_1, edge_min_2 ) / max( edge_max_1, edge_max_2 );
}

template<class TN,class TNG,class TD,unsigned NET>
typename TypePromote<Abs,typename TNG::T>::T measure( const Element<Pyramid,TN,TNG,TD,NET> &e ) {
    std::cerr << "measure not implemented for Pyramid" << std::endl;
    assert(0);
    return typename TypePromote<Abs,typename TNG::T>::T(-1.);
}

template<class TN,class TNG,class TD,unsigned NET,class TM>
bool divide_element(Element<Pyramid,TN,TNG,TD,NET> &e,TM &m,TNG **nnodes) {
    std::cout << "divide_element not implemented for Pyramid" << std::endl;
    assert(0);
    return false;
}

// extern unsigned valid_pyramid_face_center[];
// extern int div_pyramid_face_center[];
// 
// template<class TN,class TNG,class TD,unsigned NET,class TM>
// bool divide_element_using_elem_children(Element<Pyramid,TN,TNG,TD,NET> &e,TM &m,TNG **nnodes) {
//     unsigned index = 31 + bool(nnodes[0])*32 + bool(nnodes[1])*64 + bool(nnodes[2])*128 + bool(nnodes[3])*256 + bool(nnodes[4])*512;
//     for(const int *a = div_pyramid_face_center + index * 32; *a>=0; ++a ) {
//         TNG *nn[4];
//         for(unsigned i=0;i<4;++i)
//             if ( valid_pyramid_face_center[*a * 4 + i] < 6 ) // si le noeud appartient à l'ancien maillage
//                 nn[i] = e.node( valid_pyramid_face_center[*a * 4 + i] );
//             else
//                 nn[i] = nnodes[ valid_pyramid_face_center[*a * 4 + i] - 6 ];
//         DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
//     }
//     return *( div_pyramid_face_center + index * 32 ) >= 0;
// }
template<class TN,class TNG,class TD,unsigned NET,class TM>
bool divide_element_using_elem_children(Element<Pyramid,TN,TNG,TD,NET> &e,TM &m,TNG **nnodes) {
    return divide_element(e,m,nnodes);
}

template<class TV,class T>
bool var_inter_is_inside( const Pyramid &, const TV &var_inter, T tol = 0 ) {
    return heaviside( var_inter[0] + tol ) *  
           heaviside( var_inter[1] + tol ) * 
           heaviside( var_inter[2] + tol ) * 
           heaviside( 1 - var_inter[2] - var_inter[0] + tol ) *
           heaviside( 1 - var_inter[2] - var_inter[1] + tol ) ;
}

/// >= 0 -> inside, < 0 -> outside
template<class T,class TV>
T var_inter_insideness( const Pyramid &e, const TV &var_inter ) {
    return min( min( min( var_inter[0], var_inter[1] ), 1 - var_inter[0] - var_inter[2] ), 1 - var_inter[1] - var_inter[2] );
}

inline unsigned vtk_num( StructForType<Pyramid> ) { return 14; }

};

#endif // LMT_pyramid_HEADER
