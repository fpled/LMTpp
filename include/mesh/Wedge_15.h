//
// C++ Interface: Wedge_15
//
// Description:
//
//
// Author: Alain CAIGNOT <caignot@lmt.ens-cachan.fr>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMTWEDGE_15_H
#define LMTWEDGE_15_H

#include "Quad_8.h"
#include "Triangle_6.h"

namespace LMT {

// --------------------------------------------------------------------------------------------------------
/*!
    Prisme à 15 noeuds
    \verbatim
    .                        5
    .                       /|\
    .                     11 | 10
    .                     / 14  \
    .                    3___9___4
    .                    |   |   |
    .                    |   2   |
    .                    12  / \  13
    .                    | 8   7 |
    .                    |/     \|
    .                    0___6__ 1
    \relates Mesh
    \relates Element
    \keyword Maillage/Elément
    \friend samir.amrouche@lmt.ens-cachan.fr
    \friend hugo.leclerc@lmt.ens-cachan.fr
*/
struct Wedge_15 {
    static const unsigned nb_var_inter = 3;
    static const unsigned nb_nodes = 15;
    static const char *name() { return "Wedge_15"; }
    static const char *can_directly_be_represented_by() { return "Wedge_15"; }
};

// --------------------------------------------------------------------------------------------------------
template<> struct NbChildrenElement<Wedge_15,0> { static const unsigned res = 1; };
template<> struct NbChildrenElement<Wedge_15,1> { static const unsigned res = 5; };
template<> struct NbChildrenElement<Wedge_15,2> { static const unsigned res = 9; };
template<> struct NbChildrenElement<Wedge_15,3> { static const unsigned res = 15; };

template<unsigned n> struct TypeChildrenElement<Wedge_15,0,n> { typedef Wedge_15 T; };
template<> struct TypeChildrenElement<Wedge_15,1,0> { typedef Quad_8 T; };
template<> struct TypeChildrenElement<Wedge_15,1,1> { typedef Quad_8 T; };
template<> struct TypeChildrenElement<Wedge_15,1,2> { typedef Quad_8 T; };
template<> struct TypeChildrenElement<Wedge_15,1,3> { typedef Triangle_6 T; };
template<> struct TypeChildrenElement<Wedge_15,1,4> { typedef Triangle_6 T; };
template<unsigned n> struct TypeChildrenElement<Wedge_15,2,n> { typedef Bar_3 T; };
template<unsigned n> struct TypeChildrenElement<Wedge_15,3,n> { typedef NodalElement T; };

template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Wedge_15,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<0> nvi_to_subs) {
    het.add_element(e,ch,Wedge_15(),e.node(0),e.node(1),e.node(2),e.node(3),e.node(4),e.node(5),e.node(6),e.node(7),e.node(8),e.node(9),e.node(10),e.node(11),e.node(12),e.node(13),e.node(15));
}
template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Wedge_15,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<1> nvi_to_subs) {
    het.add_element(e,ch,Quad_8(),e.node(0),e.node(1),e.node(4),e.node(3),e.node(6),e.node(13),e.node(9),e.node(12));
    het.add_element(e,ch,Quad_8(),e.node(0),e.node(2),e.node(5),e.node(3),e.node(8),e.node(14),e.node(11),e.node(12));
    het.add_element(e,ch,Quad_8(),e.node(1),e.node(2),e.node(5),e.node(4),e.node(7),e.node(14),e.node(10),e.node(13));
    het.add_element(e,ch,Triangle_6(),e.node(0),e.node(1),e.node(2),e.node(6),e.node(7),e.node(8));
    het.add_element(e,ch,Triangle_6(),e.node(3),e.node(4),e.node(5),e.node(9),e.node(10),e.node(11));
}
template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Wedge_15,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<2> nvi_to_subs) {
    het.add_element(e,ch,Bar_3(),e.node(0),e.node(6),e.node(1));
    het.add_element(e,ch,Bar_3(),e.node(1),e.node(7),e.node(2));
    het.add_element(e,ch,Bar_3(),e.node(2),e.node(8),e.node(0));
    het.add_element(e,ch,Bar_3(),e.node(3),e.node(9),e.node(4));
    het.add_element(e,ch,Bar_3(),e.node(4),e.node(10),e.node(5));
    het.add_element(e,ch,Bar_3(),e.node(5),e.node(11),e.node(3));
    het.add_element(e,ch,Bar_3(),e.node(0),e.node(12),e.node(3));
    het.add_element(e,ch,Bar_3(),e.node(1),e.node(13),e.node(4));
    het.add_element(e,ch,Bar_3(),e.node(2),e.node(14),e.node(5));
}
template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Wedge_15,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<3> nvi_to_subs) {
    het.add_element(e,ch,NodalElement(),e.node(0));
    het.add_element(e,ch,NodalElement(),e.node(1));
    het.add_element(e,ch,NodalElement(),e.node(2));
    het.add_element(e,ch,NodalElement(),e.node(3));
    het.add_element(e,ch,NodalElement(),e.node(4));
    het.add_element(e,ch,NodalElement(),e.node(5));
    het.add_element(e,ch,NodalElement(),e.node(6));
    het.add_element(e,ch,NodalElement(),e.node(7));
    het.add_element(e,ch,NodalElement(),e.node(8));
    het.add_element(e,ch,NodalElement(),e.node(9));
    het.add_element(e,ch,NodalElement(),e.node(10));
    het.add_element(e,ch,NodalElement(),e.node(11));
    het.add_element(e,ch,NodalElement(),e.node(12));
    het.add_element(e,ch,NodalElement(),e.node(13));
    het.add_element(e,ch,NodalElement(),e.node(14));
}

// --------------------------------------------------------------------------------------------------------
template<class TN,class TNG,class TD,unsigned NET,class TM,class T>
void update_edge_ratio(const Element<Wedge_15,TN,TNG,TD,NET> &e,TM &m,T &edge_ratio) {
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
typename TypePromote<Abs,typename TNG::T>::T measure( const Element<Wedge_15,TN,TNG,TD,NET> &elem ) {
typename TNG::T V= 0.041666666666666664354*(((0.72360679774997899161*elem.pos(0)[0]+0.1381966011250105042*elem.pos(1)[0]+0.1381966011250105042*elem.pos(2)[0])-0.72360679774997899161*elem.pos(3)[0]-0.1381966011250105042*elem.pos(4)[0]-0.1381966011250105042*elem.pos(5)[0])*((0.1381966011250105042*elem.pos(2)[2]-0.1381966011250105042*elem.pos(0)[2]-0.8618033988749894958*elem.pos(3)[2]+0.8618033988749894958*elem.pos(5)[2])*(0.1381966011250105042*elem.pos(1)[1]-0.1381966011250105042*elem.pos(0)[1]-0.8618033988749894958*elem.pos(3)[1]+0.8618033988749894958*elem.pos(4)[1])-(0.1381966011250105042*elem.pos(1)[2]-0.1381966011250105042*elem.pos(0)[2]-0.8618033988749894958*elem.pos(3)[2]+0.8618033988749894958*elem.pos(4)[2])*(0.8618033988749894958*elem.pos(5)[1]+0.1381966011250105042*elem.pos(2)[1]-0.1381966011250105042*elem.pos(0)[1]-0.8618033988749894958*elem.pos(3)[1]))+(0.8618033988749894958*elem.pos(4)[0]+0.1381966011250105042*elem.pos(1)[0]-0.1381966011250105042*elem.pos(0)[0]-0.8618033988749894958*elem.pos(3)[0])*((0.8618033988749894958*elem.pos(5)[1]+0.1381966011250105042*elem.pos(2)[1]-0.1381966011250105042*elem.pos(0)[1]-0.8618033988749894958*elem.pos(3)[1])*((0.1381966011250105042*elem.pos(2)[2]+0.1381966011250105042*elem.pos(1)[2]+0.72360679774997899161*elem.pos(0)[2])-0.72360679774997899161*elem.pos(3)[2]-0.1381966011250105042*elem.pos(4)[2]-0.1381966011250105042*elem.pos(5)[2])-(0.1381966011250105042*elem.pos(2)[2]-0.1381966011250105042*elem.pos(0)[2]-0.8618033988749894958*elem.pos(3)[2]+0.8618033988749894958*elem.pos(5)[2])*((0.1381966011250105042*elem.pos(2)[1]+0.1381966011250105042*elem.pos(1)[1]+0.72360679774997899161*elem.pos(0)[1])-0.72360679774997899161*elem.pos(3)[1]-0.1381966011250105042*elem.pos(4)[1]-0.1381966011250105042*elem.pos(5)[1]))-(0.8618033988749894958*elem.pos(5)[0]+0.1381966011250105042*elem.pos(2)[0]-0.1381966011250105042*elem.pos(0)[0]-0.8618033988749894958*elem.pos(3)[0])*((0.1381966011250105042*elem.pos(1)[1]-0.1381966011250105042*elem.pos(0)[1]-0.8618033988749894958*elem.pos(3)[1]+0.8618033988749894958*elem.pos(4)[1])*((0.1381966011250105042*elem.pos(2)[2]+0.1381966011250105042*elem.pos(1)[2]+0.72360679774997899161*elem.pos(0)[2])-0.72360679774997899161*elem.pos(3)[2]-0.1381966011250105042*elem.pos(4)[2]-0.1381966011250105042*elem.pos(5)[2])-(0.1381966011250105042*elem.pos(1)[2]-0.1381966011250105042*elem.pos(0)[2]-0.8618033988749894958*elem.pos(3)[2]+0.8618033988749894958*elem.pos(4)[2])*((0.1381966011250105042*elem.pos(2)[1]+0.1381966011250105042*elem.pos(1)[1]+0.72360679774997899161*elem.pos(0)[1])-0.72360679774997899161*elem.pos(3)[1]-0.1381966011250105042*elem.pos(4)[1]-0.1381966011250105042*elem.pos(5)[1])))+0.041666666666666664354*(((0.1381966011250105042*elem.pos(2)[2]-0.1381966011250105042*elem.pos(0)[2]-0.8618033988749894958*elem.pos(3)[2]+0.8618033988749894958*elem.pos(5)[2])*(0.1381966011250105042*elem.pos(1)[1]-0.1381966011250105042*elem.pos(0)[1]-0.8618033988749894958*elem.pos(3)[1]+0.8618033988749894958*elem.pos(4)[1])-(0.1381966011250105042*elem.pos(1)[2]-0.1381966011250105042*elem.pos(0)[2]-0.8618033988749894958*elem.pos(3)[2]+0.8618033988749894958*elem.pos(4)[2])*(0.8618033988749894958*elem.pos(5)[1]+0.1381966011250105042*elem.pos(2)[1]-0.1381966011250105042*elem.pos(0)[1]-0.8618033988749894958*elem.pos(3)[1]))*((0.1381966011250105042*elem.pos(2)[0]+0.58541019662496851517*elem.pos(1)[0]+0.27639320225002098064*elem.pos(0)[0])-0.27639320225002098064*elem.pos(3)[0]-0.58541019662496851517*elem.pos(4)[0]-0.1381966011250105042*elem.pos(5)[0])+(0.8618033988749894958*elem.pos(4)[0]+0.1381966011250105042*elem.pos(1)[0]-0.1381966011250105042*elem.pos(0)[0]-0.8618033988749894958*elem.pos(3)[0])*((0.8618033988749894958*elem.pos(5)[1]+0.1381966011250105042*elem.pos(2)[1]-0.1381966011250105042*elem.pos(0)[1]-0.8618033988749894958*elem.pos(3)[1])*((0.1381966011250105042*elem.pos(2)[2]+0.58541019662496851517*elem.pos(1)[2]+0.27639320225002098064*elem.pos(0)[2])-0.27639320225002098064*elem.pos(3)[2]-0.58541019662496851517*elem.pos(4)[2]-0.1381966011250105042*elem.pos(5)[2])-(0.1381966011250105042*elem.pos(2)[2]-0.1381966011250105042*elem.pos(0)[2]-0.8618033988749894958*elem.pos(3)[2]+0.8618033988749894958*elem.pos(5)[2])*((0.1381966011250105042*elem.pos(2)[1]+0.58541019662496851517*elem.pos(1)[1]+0.27639320225002098064*elem.pos(0)[1])-0.27639320225002098064*elem.pos(3)[1]-0.58541019662496851517*elem.pos(4)[1]-0.1381966011250105042*elem.pos(5)[1]))-(0.8618033988749894958*elem.pos(5)[0]+0.1381966011250105042*elem.pos(2)[0]-0.1381966011250105042*elem.pos(0)[0]-0.8618033988749894958*elem.pos(3)[0])*((0.1381966011250105042*elem.pos(1)[1]-0.1381966011250105042*elem.pos(0)[1]-0.8618033988749894958*elem.pos(3)[1]+0.8618033988749894958*elem.pos(4)[1])*((0.1381966011250105042*elem.pos(2)[2]+0.58541019662496851517*elem.pos(1)[2]+0.27639320225002098064*elem.pos(0)[2])-0.27639320225002098064*elem.pos(3)[2]-0.58541019662496851517*elem.pos(4)[2]-0.1381966011250105042*elem.pos(5)[2])-(0.1381966011250105042*elem.pos(1)[2]-0.1381966011250105042*elem.pos(0)[2]-0.8618033988749894958*elem.pos(3)[2]+0.8618033988749894958*elem.pos(4)[2])*((0.1381966011250105042*elem.pos(2)[1]+0.58541019662496851517*elem.pos(1)[1]+0.27639320225002098064*elem.pos(0)[1])-0.27639320225002098064*elem.pos(3)[1]-0.58541019662496851517*elem.pos(4)[1]-0.1381966011250105042*elem.pos(5)[1])))+0.041666666666666664354*(((0.1381966011250105042*elem.pos(2)[2]-0.1381966011250105042*elem.pos(0)[2]-0.8618033988749894958*elem.pos(3)[2]+0.8618033988749894958*elem.pos(5)[2])*(0.1381966011250105042*elem.pos(1)[1]-0.1381966011250105042*elem.pos(0)[1]-0.8618033988749894958*elem.pos(3)[1]+0.8618033988749894958*elem.pos(4)[1])-(0.1381966011250105042*elem.pos(1)[2]-0.1381966011250105042*elem.pos(0)[2]-0.8618033988749894958*elem.pos(3)[2]+0.8618033988749894958*elem.pos(4)[2])*(0.8618033988749894958*elem.pos(5)[1]+0.1381966011250105042*elem.pos(2)[1]-0.1381966011250105042*elem.pos(0)[1]-0.8618033988749894958*elem.pos(3)[1]))*((0.58541019662496851517*elem.pos(2)[0]+0.1381966011250105042*elem.pos(1)[0]+0.27639320225002098064*elem.pos(0)[0])-0.27639320225002098064*elem.pos(3)[0]-0.1381966011250105042*elem.pos(4)[0]-0.58541019662496851517*elem.pos(5)[0])+(0.8618033988749894958*elem.pos(4)[0]+0.1381966011250105042*elem.pos(1)[0]-0.1381966011250105042*elem.pos(0)[0]-0.8618033988749894958*elem.pos(3)[0])*((0.8618033988749894958*elem.pos(5)[1]+0.1381966011250105042*elem.pos(2)[1]-0.1381966011250105042*elem.pos(0)[1]-0.8618033988749894958*elem.pos(3)[1])*((0.58541019662496851517*elem.pos(2)[2]+0.1381966011250105042*elem.pos(1)[2]+0.27639320225002098064*elem.pos(0)[2])-0.27639320225002098064*elem.pos(3)[2]-0.1381966011250105042*elem.pos(4)[2]-0.58541019662496851517*elem.pos(5)[2])-(0.1381966011250105042*elem.pos(2)[2]-0.1381966011250105042*elem.pos(0)[2]-0.8618033988749894958*elem.pos(3)[2]+0.8618033988749894958*elem.pos(5)[2])*((0.58541019662496851517*elem.pos(2)[1]+0.1381966011250105042*elem.pos(1)[1]+0.27639320225002098064*elem.pos(0)[1])-0.27639320225002098064*elem.pos(3)[1]-0.1381966011250105042*elem.pos(4)[1]-0.58541019662496851517*elem.pos(5)[1]))-(0.8618033988749894958*elem.pos(5)[0]+0.1381966011250105042*elem.pos(2)[0]-0.1381966011250105042*elem.pos(0)[0]-0.8618033988749894958*elem.pos(3)[0])*((0.1381966011250105042*elem.pos(1)[1]-0.1381966011250105042*elem.pos(0)[1]-0.8618033988749894958*elem.pos(3)[1]+0.8618033988749894958*elem.pos(4)[1])*((0.58541019662496851517*elem.pos(2)[2]+0.1381966011250105042*elem.pos(1)[2]+0.27639320225002098064*elem.pos(0)[2])-0.27639320225002098064*elem.pos(3)[2]-0.1381966011250105042*elem.pos(4)[2]-0.58541019662496851517*elem.pos(5)[2])-(0.1381966011250105042*elem.pos(1)[2]-0.1381966011250105042*elem.pos(0)[2]-0.8618033988749894958*elem.pos(3)[2]+0.8618033988749894958*elem.pos(4)[2])*((0.58541019662496851517*elem.pos(2)[1]+0.1381966011250105042*elem.pos(1)[1]+0.27639320225002098064*elem.pos(0)[1])-0.27639320225002098064*elem.pos(3)[1]-0.1381966011250105042*elem.pos(4)[1]-0.58541019662496851517*elem.pos(5)[1])))+0.041666666666666664354*(((0.72360679774997899161*elem.pos(0)[0]+0.1381966011250105042*elem.pos(1)[0]+0.1381966011250105042*elem.pos(2)[0])-0.72360679774997899161*elem.pos(3)[0]-0.1381966011250105042*elem.pos(4)[0]-0.1381966011250105042*elem.pos(5)[0])*((0.41458980337503148483*elem.pos(4)[1]+0.58541019662496851517*elem.pos(1)[1]-0.58541019662496851517*elem.pos(0)[1]-0.41458980337503148483*elem.pos(3)[1])*(0.41458980337503148483*elem.pos(5)[2]+0.58541019662496851517*elem.pos(2)[2]-0.58541019662496851517*elem.pos(0)[2]-0.41458980337503148483*elem.pos(3)[2])-(0.41458980337503148483*elem.pos(4)[2]+0.58541019662496851517*elem.pos(1)[2]-0.58541019662496851517*elem.pos(0)[2]-0.41458980337503148483*elem.pos(3)[2])*(0.41458980337503148483*elem.pos(5)[1]+0.58541019662496851517*elem.pos(2)[1]-0.58541019662496851517*elem.pos(0)[1]-0.41458980337503148483*elem.pos(3)[1]))+(0.41458980337503148483*elem.pos(4)[0]+0.58541019662496851517*elem.pos(1)[0]-0.58541019662496851517*elem.pos(0)[0]-0.41458980337503148483*elem.pos(3)[0])*(((0.1381966011250105042*elem.pos(2)[2]+0.1381966011250105042*elem.pos(1)[2]+0.72360679774997899161*elem.pos(0)[2])-0.72360679774997899161*elem.pos(3)[2]-0.1381966011250105042*elem.pos(4)[2]-0.1381966011250105042*elem.pos(5)[2])*(0.41458980337503148483*elem.pos(5)[1]+0.58541019662496851517*elem.pos(2)[1]-0.58541019662496851517*elem.pos(0)[1]-0.41458980337503148483*elem.pos(3)[1])-((0.1381966011250105042*elem.pos(2)[1]+0.1381966011250105042*elem.pos(1)[1]+0.72360679774997899161*elem.pos(0)[1])-0.72360679774997899161*elem.pos(3)[1]-0.1381966011250105042*elem.pos(4)[1]-0.1381966011250105042*elem.pos(5)[1])*(0.41458980337503148483*elem.pos(5)[2]+0.58541019662496851517*elem.pos(2)[2]-0.58541019662496851517*elem.pos(0)[2]-0.41458980337503148483*elem.pos(3)[2]))-(0.41458980337503148483*elem.pos(5)[0]+0.58541019662496851517*elem.pos(2)[0]-0.58541019662496851517*elem.pos(0)[0]-0.41458980337503148483*elem.pos(3)[0])*(((0.1381966011250105042*elem.pos(2)[2]+0.1381966011250105042*elem.pos(1)[2]+0.72360679774997899161*elem.pos(0)[2])-0.72360679774997899161*elem.pos(3)[2]-0.1381966011250105042*elem.pos(4)[2]-0.1381966011250105042*elem.pos(5)[2])*(0.41458980337503148483*elem.pos(4)[1]+0.58541019662496851517*elem.pos(1)[1]-0.58541019662496851517*elem.pos(0)[1]-0.41458980337503148483*elem.pos(3)[1])-((0.1381966011250105042*elem.pos(2)[1]+0.1381966011250105042*elem.pos(1)[1]+0.72360679774997899161*elem.pos(0)[1])-0.72360679774997899161*elem.pos(3)[1]-0.1381966011250105042*elem.pos(4)[1]-0.1381966011250105042*elem.pos(5)[1])*(0.41458980337503148483*elem.pos(4)[2]+0.58541019662496851517*elem.pos(1)[2]-0.58541019662496851517*elem.pos(0)[2]-0.41458980337503148483*elem.pos(3)[2])));

return abs(V);
}

template<class TN,class TNG,class TD,unsigned NET,class TM>
bool divide_element(Element<Wedge_15,TN,TNG,TD,NET> &e,TM &m,TNG **nnodes) {
    std::cout << "divide_element not implemented for Wedge_15" << std::endl;
    assert(0);
    return false;
}

extern unsigned valid_wedge_face_center[];
extern int div_wedge_face_center[];

template<class TN,class TNG,class TD,unsigned NET,class TM>
bool divide_element_using_elem_children(Element<Wedge_15,TN,TNG,TD,NET> &e,TM &m,TNG **nnodes) {
    return divide_element(e,m,nnodes);
}

inline unsigned vtk_num( StructForType<Wedge_15> ) { return 26; }

}

#include "element_Wedge_15.h"

#endif // LMTWEDGE_15_H
