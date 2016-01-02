//
// C++ Interface: Wedge
//
// Description: 
//
//
// Author: Alain CAIGNOT <caignot@lmt.ens-cachan.fr>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMTWEDGE_H
#define LMTWEDGE_H

#include "Quad.h"
#include "Tetra.h"

namespace LMT {

// --------------------------------------------------------------------------------------------------------
/*!
    Prisme à 6 noeuds
    \verbatim
    .                        5
    .                       /|\
    .                      / | \
    .                     /  |  \
    .                    3___|___4
    .                    |   |   |
    .                    |   2   |
    .                    |  / \  |
    .                    | /   \ |
    .                    |/     \|
    .                    0______ 1
    Dans l'espace de référence cela correspond au volume de points M(x,y,z) tels que :
        * x, y , z >= 0
        * x <= 1
        * y <= 1
        * x+z <= 1
    \relates Mesh
    \relates Element
    \keyword Maillage/Elément
    \friend samir.amrouche@lmt.ens-cachan.fr
    \friend hugo.leclerc@lmt.ens-cachan.fr
*/
struct Wedge {
    static const unsigned nb_var_inter = 3;
    static const unsigned nb_nodes = 6;
    static const char *name() { return "Wedge"; }
    static const char *avs_name() { return "prism"; }
    static const char *can_directly_be_represented_by() { return "Wedge"; }
};

// --------------------------------------------------------------------------------------------------------
template<> struct NbChildrenElement<Wedge,0> { static const unsigned res = 1; };
template<> struct NbChildrenElement<Wedge,1> { static const unsigned res = 5; };
template<> struct NbChildrenElement<Wedge,2> { static const unsigned res = 9; };
template<> struct NbChildrenElement<Wedge,3> { static const unsigned res = 6; };

template<unsigned n> struct TypeChildrenElement<Wedge,0,n> { typedef Wedge T; };
template<> struct TypeChildrenElement<Wedge,1,0> { typedef Quad T; };
template<> struct TypeChildrenElement<Wedge,1,1> { typedef Quad T; };
template<> struct TypeChildrenElement<Wedge,1,2> { typedef Quad T; };
template<> struct TypeChildrenElement<Wedge,1,3> { typedef Triangle T; };
template<> struct TypeChildrenElement<Wedge,1,4> { typedef Triangle T; };
template<unsigned n> struct TypeChildrenElement<Wedge,2,n> { typedef Bar T; };
template<unsigned n> struct TypeChildrenElement<Wedge,3,n> { typedef NodalElement T; };

template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Wedge,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<0> nvi_to_subs) {
    het.add_element(e,ch,Wedge(),e.node(0),e.node(1),e.node(2),e.node(3),e.node(4),e.node(5));
}
template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Wedge,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<1> nvi_to_subs) {
    het.add_element(e,ch,Quad(),e.node(0),e.node(1),e.node(4),e.node(3));
    het.add_element(e,ch,Quad(),e.node(0),e.node(3),e.node(5),e.node(2));
    het.add_element(e,ch,Quad(),e.node(1),e.node(2),e.node(5),e.node(4));
    het.add_element(e,ch,Triangle(),e.node(0),e.node(2),e.node(1));
    het.add_element(e,ch,Triangle(),e.node(3),e.node(4),e.node(5));
}

template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Wedge,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<2> nvi_to_subs) {
    het.add_element(e,ch,Bar(),e.node(0),e.node(1));
    het.add_element(e,ch,Bar(),e.node(1),e.node(2));
    het.add_element(e,ch,Bar(),e.node(2),e.node(0));
    het.add_element(e,ch,Bar(),e.node(3),e.node(4));
    het.add_element(e,ch,Bar(),e.node(4),e.node(5));
    het.add_element(e,ch,Bar(),e.node(5),e.node(3));
    het.add_element(e,ch,Bar(),e.node(0),e.node(3));
    het.add_element(e,ch,Bar(),e.node(1),e.node(4));
    het.add_element(e,ch,Bar(),e.node(2),e.node(5));
}
template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Wedge,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<3> nvi_to_subs) {
    het.add_element(e,ch,NodalElement(),e.node(0));
    het.add_element(e,ch,NodalElement(),e.node(1));
    het.add_element(e,ch,NodalElement(),e.node(2));
    het.add_element(e,ch,NodalElement(),e.node(3));
    het.add_element(e,ch,NodalElement(),e.node(4));
    het.add_element(e,ch,NodalElement(),e.node(5));
}

// --------------------------------------------------------------------------------------------------------
template<class TN,class TNG,class TD,unsigned NET,class TM,class T>
void update_edge_ratio(const Element<Wedge,TN,TNG,TD,NET> &e,TM &m,T &edge_ratio) {
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
typename TypePromote<Abs,typename TNG::T>::T measure( const Element<Wedge,TN,TNG,TD,NET> &elem ) {
typename TNG::T V= 0.041666666666666664354*(((0.72360679774997899161*elem.pos(0)[0]+0.1381966011250105042*elem.pos(1)[0]+0.1381966011250105042*elem.pos(2)[0])-0.72360679774997899161*elem.pos(3)[0]-0.1381966011250105042*elem.pos(4)[0]-0.1381966011250105042*elem.pos(5)[0])*((0.1381966011250105042*elem.pos(2)[2]-0.1381966011250105042*elem.pos(0)[2]-0.8618033988749894958*elem.pos(3)[2]+0.8618033988749894958*elem.pos(5)[2])*(0.1381966011250105042*elem.pos(1)[1]-0.1381966011250105042*elem.pos(0)[1]-0.8618033988749894958*elem.pos(3)[1]+0.8618033988749894958*elem.pos(4)[1])-(0.1381966011250105042*elem.pos(1)[2]-0.1381966011250105042*elem.pos(0)[2]-0.8618033988749894958*elem.pos(3)[2]+0.8618033988749894958*elem.pos(4)[2])*(0.8618033988749894958*elem.pos(5)[1]+0.1381966011250105042*elem.pos(2)[1]-0.1381966011250105042*elem.pos(0)[1]-0.8618033988749894958*elem.pos(3)[1]))+(0.8618033988749894958*elem.pos(4)[0]+0.1381966011250105042*elem.pos(1)[0]-0.1381966011250105042*elem.pos(0)[0]-0.8618033988749894958*elem.pos(3)[0])*((0.8618033988749894958*elem.pos(5)[1]+0.1381966011250105042*elem.pos(2)[1]-0.1381966011250105042*elem.pos(0)[1]-0.8618033988749894958*elem.pos(3)[1])*((0.1381966011250105042*elem.pos(2)[2]+0.1381966011250105042*elem.pos(1)[2]+0.72360679774997899161*elem.pos(0)[2])-0.72360679774997899161*elem.pos(3)[2]-0.1381966011250105042*elem.pos(4)[2]-0.1381966011250105042*elem.pos(5)[2])-(0.1381966011250105042*elem.pos(2)[2]-0.1381966011250105042*elem.pos(0)[2]-0.8618033988749894958*elem.pos(3)[2]+0.8618033988749894958*elem.pos(5)[2])*((0.1381966011250105042*elem.pos(2)[1]+0.1381966011250105042*elem.pos(1)[1]+0.72360679774997899161*elem.pos(0)[1])-0.72360679774997899161*elem.pos(3)[1]-0.1381966011250105042*elem.pos(4)[1]-0.1381966011250105042*elem.pos(5)[1]))-(0.8618033988749894958*elem.pos(5)[0]+0.1381966011250105042*elem.pos(2)[0]-0.1381966011250105042*elem.pos(0)[0]-0.8618033988749894958*elem.pos(3)[0])*((0.1381966011250105042*elem.pos(1)[1]-0.1381966011250105042*elem.pos(0)[1]-0.8618033988749894958*elem.pos(3)[1]+0.8618033988749894958*elem.pos(4)[1])*((0.1381966011250105042*elem.pos(2)[2]+0.1381966011250105042*elem.pos(1)[2]+0.72360679774997899161*elem.pos(0)[2])-0.72360679774997899161*elem.pos(3)[2]-0.1381966011250105042*elem.pos(4)[2]-0.1381966011250105042*elem.pos(5)[2])-(0.1381966011250105042*elem.pos(1)[2]-0.1381966011250105042*elem.pos(0)[2]-0.8618033988749894958*elem.pos(3)[2]+0.8618033988749894958*elem.pos(4)[2])*((0.1381966011250105042*elem.pos(2)[1]+0.1381966011250105042*elem.pos(1)[1]+0.72360679774997899161*elem.pos(0)[1])-0.72360679774997899161*elem.pos(3)[1]-0.1381966011250105042*elem.pos(4)[1]-0.1381966011250105042*elem.pos(5)[1])))+0.041666666666666664354*(((0.1381966011250105042*elem.pos(2)[2]-0.1381966011250105042*elem.pos(0)[2]-0.8618033988749894958*elem.pos(3)[2]+0.8618033988749894958*elem.pos(5)[2])*(0.1381966011250105042*elem.pos(1)[1]-0.1381966011250105042*elem.pos(0)[1]-0.8618033988749894958*elem.pos(3)[1]+0.8618033988749894958*elem.pos(4)[1])-(0.1381966011250105042*elem.pos(1)[2]-0.1381966011250105042*elem.pos(0)[2]-0.8618033988749894958*elem.pos(3)[2]+0.8618033988749894958*elem.pos(4)[2])*(0.8618033988749894958*elem.pos(5)[1]+0.1381966011250105042*elem.pos(2)[1]-0.1381966011250105042*elem.pos(0)[1]-0.8618033988749894958*elem.pos(3)[1]))*((0.1381966011250105042*elem.pos(2)[0]+0.58541019662496851517*elem.pos(1)[0]+0.27639320225002098064*elem.pos(0)[0])-0.27639320225002098064*elem.pos(3)[0]-0.58541019662496851517*elem.pos(4)[0]-0.1381966011250105042*elem.pos(5)[0])+(0.8618033988749894958*elem.pos(4)[0]+0.1381966011250105042*elem.pos(1)[0]-0.1381966011250105042*elem.pos(0)[0]-0.8618033988749894958*elem.pos(3)[0])*((0.8618033988749894958*elem.pos(5)[1]+0.1381966011250105042*elem.pos(2)[1]-0.1381966011250105042*elem.pos(0)[1]-0.8618033988749894958*elem.pos(3)[1])*((0.1381966011250105042*elem.pos(2)[2]+0.58541019662496851517*elem.pos(1)[2]+0.27639320225002098064*elem.pos(0)[2])-0.27639320225002098064*elem.pos(3)[2]-0.58541019662496851517*elem.pos(4)[2]-0.1381966011250105042*elem.pos(5)[2])-(0.1381966011250105042*elem.pos(2)[2]-0.1381966011250105042*elem.pos(0)[2]-0.8618033988749894958*elem.pos(3)[2]+0.8618033988749894958*elem.pos(5)[2])*((0.1381966011250105042*elem.pos(2)[1]+0.58541019662496851517*elem.pos(1)[1]+0.27639320225002098064*elem.pos(0)[1])-0.27639320225002098064*elem.pos(3)[1]-0.58541019662496851517*elem.pos(4)[1]-0.1381966011250105042*elem.pos(5)[1]))-(0.8618033988749894958*elem.pos(5)[0]+0.1381966011250105042*elem.pos(2)[0]-0.1381966011250105042*elem.pos(0)[0]-0.8618033988749894958*elem.pos(3)[0])*((0.1381966011250105042*elem.pos(1)[1]-0.1381966011250105042*elem.pos(0)[1]-0.8618033988749894958*elem.pos(3)[1]+0.8618033988749894958*elem.pos(4)[1])*((0.1381966011250105042*elem.pos(2)[2]+0.58541019662496851517*elem.pos(1)[2]+0.27639320225002098064*elem.pos(0)[2])-0.27639320225002098064*elem.pos(3)[2]-0.58541019662496851517*elem.pos(4)[2]-0.1381966011250105042*elem.pos(5)[2])-(0.1381966011250105042*elem.pos(1)[2]-0.1381966011250105042*elem.pos(0)[2]-0.8618033988749894958*elem.pos(3)[2]+0.8618033988749894958*elem.pos(4)[2])*((0.1381966011250105042*elem.pos(2)[1]+0.58541019662496851517*elem.pos(1)[1]+0.27639320225002098064*elem.pos(0)[1])-0.27639320225002098064*elem.pos(3)[1]-0.58541019662496851517*elem.pos(4)[1]-0.1381966011250105042*elem.pos(5)[1])))+0.041666666666666664354*(((0.1381966011250105042*elem.pos(2)[2]-0.1381966011250105042*elem.pos(0)[2]-0.8618033988749894958*elem.pos(3)[2]+0.8618033988749894958*elem.pos(5)[2])*(0.1381966011250105042*elem.pos(1)[1]-0.1381966011250105042*elem.pos(0)[1]-0.8618033988749894958*elem.pos(3)[1]+0.8618033988749894958*elem.pos(4)[1])-(0.1381966011250105042*elem.pos(1)[2]-0.1381966011250105042*elem.pos(0)[2]-0.8618033988749894958*elem.pos(3)[2]+0.8618033988749894958*elem.pos(4)[2])*(0.8618033988749894958*elem.pos(5)[1]+0.1381966011250105042*elem.pos(2)[1]-0.1381966011250105042*elem.pos(0)[1]-0.8618033988749894958*elem.pos(3)[1]))*((0.58541019662496851517*elem.pos(2)[0]+0.1381966011250105042*elem.pos(1)[0]+0.27639320225002098064*elem.pos(0)[0])-0.27639320225002098064*elem.pos(3)[0]-0.1381966011250105042*elem.pos(4)[0]-0.58541019662496851517*elem.pos(5)[0])+(0.8618033988749894958*elem.pos(4)[0]+0.1381966011250105042*elem.pos(1)[0]-0.1381966011250105042*elem.pos(0)[0]-0.8618033988749894958*elem.pos(3)[0])*((0.8618033988749894958*elem.pos(5)[1]+0.1381966011250105042*elem.pos(2)[1]-0.1381966011250105042*elem.pos(0)[1]-0.8618033988749894958*elem.pos(3)[1])*((0.58541019662496851517*elem.pos(2)[2]+0.1381966011250105042*elem.pos(1)[2]+0.27639320225002098064*elem.pos(0)[2])-0.27639320225002098064*elem.pos(3)[2]-0.1381966011250105042*elem.pos(4)[2]-0.58541019662496851517*elem.pos(5)[2])-(0.1381966011250105042*elem.pos(2)[2]-0.1381966011250105042*elem.pos(0)[2]-0.8618033988749894958*elem.pos(3)[2]+0.8618033988749894958*elem.pos(5)[2])*((0.58541019662496851517*elem.pos(2)[1]+0.1381966011250105042*elem.pos(1)[1]+0.27639320225002098064*elem.pos(0)[1])-0.27639320225002098064*elem.pos(3)[1]-0.1381966011250105042*elem.pos(4)[1]-0.58541019662496851517*elem.pos(5)[1]))-(0.8618033988749894958*elem.pos(5)[0]+0.1381966011250105042*elem.pos(2)[0]-0.1381966011250105042*elem.pos(0)[0]-0.8618033988749894958*elem.pos(3)[0])*((0.1381966011250105042*elem.pos(1)[1]-0.1381966011250105042*elem.pos(0)[1]-0.8618033988749894958*elem.pos(3)[1]+0.8618033988749894958*elem.pos(4)[1])*((0.58541019662496851517*elem.pos(2)[2]+0.1381966011250105042*elem.pos(1)[2]+0.27639320225002098064*elem.pos(0)[2])-0.27639320225002098064*elem.pos(3)[2]-0.1381966011250105042*elem.pos(4)[2]-0.58541019662496851517*elem.pos(5)[2])-(0.1381966011250105042*elem.pos(1)[2]-0.1381966011250105042*elem.pos(0)[2]-0.8618033988749894958*elem.pos(3)[2]+0.8618033988749894958*elem.pos(4)[2])*((0.58541019662496851517*elem.pos(2)[1]+0.1381966011250105042*elem.pos(1)[1]+0.27639320225002098064*elem.pos(0)[1])-0.27639320225002098064*elem.pos(3)[1]-0.1381966011250105042*elem.pos(4)[1]-0.58541019662496851517*elem.pos(5)[1])))+0.041666666666666664354*(((0.72360679774997899161*elem.pos(0)[0]+0.1381966011250105042*elem.pos(1)[0]+0.1381966011250105042*elem.pos(2)[0])-0.72360679774997899161*elem.pos(3)[0]-0.1381966011250105042*elem.pos(4)[0]-0.1381966011250105042*elem.pos(5)[0])*((0.41458980337503148483*elem.pos(4)[1]+0.58541019662496851517*elem.pos(1)[1]-0.58541019662496851517*elem.pos(0)[1]-0.41458980337503148483*elem.pos(3)[1])*(0.41458980337503148483*elem.pos(5)[2]+0.58541019662496851517*elem.pos(2)[2]-0.58541019662496851517*elem.pos(0)[2]-0.41458980337503148483*elem.pos(3)[2])-(0.41458980337503148483*elem.pos(4)[2]+0.58541019662496851517*elem.pos(1)[2]-0.58541019662496851517*elem.pos(0)[2]-0.41458980337503148483*elem.pos(3)[2])*(0.41458980337503148483*elem.pos(5)[1]+0.58541019662496851517*elem.pos(2)[1]-0.58541019662496851517*elem.pos(0)[1]-0.41458980337503148483*elem.pos(3)[1]))+(0.41458980337503148483*elem.pos(4)[0]+0.58541019662496851517*elem.pos(1)[0]-0.58541019662496851517*elem.pos(0)[0]-0.41458980337503148483*elem.pos(3)[0])*(((0.1381966011250105042*elem.pos(2)[2]+0.1381966011250105042*elem.pos(1)[2]+0.72360679774997899161*elem.pos(0)[2])-0.72360679774997899161*elem.pos(3)[2]-0.1381966011250105042*elem.pos(4)[2]-0.1381966011250105042*elem.pos(5)[2])*(0.41458980337503148483*elem.pos(5)[1]+0.58541019662496851517*elem.pos(2)[1]-0.58541019662496851517*elem.pos(0)[1]-0.41458980337503148483*elem.pos(3)[1])-((0.1381966011250105042*elem.pos(2)[1]+0.1381966011250105042*elem.pos(1)[1]+0.72360679774997899161*elem.pos(0)[1])-0.72360679774997899161*elem.pos(3)[1]-0.1381966011250105042*elem.pos(4)[1]-0.1381966011250105042*elem.pos(5)[1])*(0.41458980337503148483*elem.pos(5)[2]+0.58541019662496851517*elem.pos(2)[2]-0.58541019662496851517*elem.pos(0)[2]-0.41458980337503148483*elem.pos(3)[2]))-(0.41458980337503148483*elem.pos(5)[0]+0.58541019662496851517*elem.pos(2)[0]-0.58541019662496851517*elem.pos(0)[0]-0.41458980337503148483*elem.pos(3)[0])*(((0.1381966011250105042*elem.pos(2)[2]+0.1381966011250105042*elem.pos(1)[2]+0.72360679774997899161*elem.pos(0)[2])-0.72360679774997899161*elem.pos(3)[2]-0.1381966011250105042*elem.pos(4)[2]-0.1381966011250105042*elem.pos(5)[2])*(0.41458980337503148483*elem.pos(4)[1]+0.58541019662496851517*elem.pos(1)[1]-0.58541019662496851517*elem.pos(0)[1]-0.41458980337503148483*elem.pos(3)[1])-((0.1381966011250105042*elem.pos(2)[1]+0.1381966011250105042*elem.pos(1)[1]+0.72360679774997899161*elem.pos(0)[1])-0.72360679774997899161*elem.pos(3)[1]-0.1381966011250105042*elem.pos(4)[1]-0.1381966011250105042*elem.pos(5)[1])*(0.41458980337503148483*elem.pos(4)[2]+0.58541019662496851517*elem.pos(1)[2]-0.58541019662496851517*elem.pos(0)[2]-0.41458980337503148483*elem.pos(3)[2])));

return abs(V);
}

template<class TN,class TNG,class TD,unsigned NET,class TM>
bool divide_element(Element<Wedge,TN,TNG,TD,NET> &e,TM &m,TNG **nnodes) {
    std::cout << "divide_element not implemented for Wedge" << std::endl;
    assert(0);
    return false;
}

extern unsigned valid_wedge_face_center[];
extern int div_wedge_face_center[];

template<class TN,class TNG,class TD,unsigned NET,class TM>
bool divide_element_using_elem_children(Element<Wedge,TN,TNG,TD,NET> &e,TM &m,TNG **nnodes) {
    unsigned index = 63 + bool(nnodes[0])*64 + bool(nnodes[1])*128 + bool(nnodes[2])*256 + bool(nnodes[3])*512 + bool(nnodes[4])*1024;
    for(const int *a = div_wedge_face_center + index * 32; *a>=0; ++a ) { // si le noeud appartient à l'ancien maillage
        TNG *nn[4];
        for(unsigned i=0;i<4;++i) {
            if ( valid_wedge_face_center[*a * 4 + i] < 6 )
                nn[i] = e.node( valid_wedge_face_center[*a * 4 + i] );
            else
                nn[i] = nnodes[ valid_wedge_face_center[*a * 4 + i] - 6 ];
        }
        DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
    }
    return *( div_wedge_face_center + index * 32 ) >= 0;
}

template<class TV,class T>
bool var_inter_is_inside( const Wedge &, const TV &var_inter, T tol = 0 ) {
    return heaviside( var_inter[0] + tol ) *  
            heaviside( var_inter[1] + tol ) * 
            heaviside( var_inter[2] + tol ) * 
            heaviside( 1 - var_inter[0] + tol ) *
            heaviside( 1 - var_inter[1] + tol ) *
            heaviside( 1 - var_inter[0] - var_inter[2] + tol );
}

/// >= 0 -> inside, < 0 -> outside
template<class T,class TV>
T var_inter_insideness( const Wedge &e, const TV &var_inter ) {
    return min( min( min( min( min( var_inter[0], var_inter[1] ), var_inter[2] ),1 - var_inter[0] ), 1 - var_inter[1] ), 1 - var_inter[0] - var_inter[2] );
}

inline unsigned vtk_num( StructForType<Wedge> ) { return 13; }

}

#include "element_Wedge.h"

#endif // LMTWEDGE_H
