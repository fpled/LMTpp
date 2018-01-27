//
// C++ Interface: Penta
//
// Description:
//
//
// Author: Alain CAIGNOT <caignot@lmt.ens-cachan.fr>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMTPENTA_H
#define LMTPENTA_H

#include "Bar.h"

namespace LMT {

// --------------------------------------------------------------------------------------------------------
/*!
    Pentaèdre à 5 noeuds

    \relates Mesh
    \relates Element
    \keyword Maillage/Elément
    \friend samir.amrouche@lmt.ens-cachan.fr
    \friend hugo.leclerc@lmt.ens-cachan.fr
*/
struct Penta {
    static const unsigned nb_var_inter = 2;
    static const unsigned nb_nodes = 5;
    static const char *name() { return "Penta"; }
    static const char *can_directly_be_represented_by() { return "Penta"; }
};

// --------------------------------------------------------------------------------------------------------
template<> struct NbChildrenElement<Penta,1> { static const unsigned res = 5; };
template<> struct NbChildrenElement<Penta,2> { static const unsigned res = 5; };

template<unsigned n> struct TypeChildrenElement<Penta,0,n> { typedef Penta T; };
template<unsigned n> struct TypeChildrenElement<Penta,1,n> { typedef Bar T; };
template<unsigned n> struct TypeChildrenElement<Penta,2,n> { typedef NodalElement T; };

template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Penta,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<1> nvi_to_subs) {
    het.add_element(e,ch,Bar(),e.node(0),e.node(1));
    het.add_element(e,ch,Bar(),e.node(1),e.node(2));
    het.add_element(e,ch,Bar(),e.node(2),e.node(3));
    het.add_element(e,ch,Bar(),e.node(3),e.node(4));
    het.add_element(e,ch,Bar(),e.node(4),e.node(0));
}

template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Penta,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<2> nvi_to_subs) {
    het.add_element(e,ch,NodalElement(),e.node(0));
    het.add_element(e,ch,NodalElement(),e.node(1));
    het.add_element(e,ch,NodalElement(),e.node(2));
    het.add_element(e,ch,NodalElement(),e.node(3));
    het.add_element(e,ch,NodalElement(),e.node(4));
}

// --------------------------------------------------------------------------------------------------------
template<class TN,class TNG,class TD,unsigned NET,class TM,class T>
void update_edge_ratio(const Element<Penta,TN,TNG,TD,NET> &e,TM &m,T &edge_ratio) {
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
typename TypePromote<Abs,typename TNG::T>::T measure( const Element<Penta,TN,TNG,TD,NET> &e ) {
    std::cout << "measure not implemented for Penta" << std::endl;
	assert(0);
    return typename TypePromote<Abs,typename TNG::T>::T(-1.);
}

template<class TN,class TNG,class TD,unsigned NET,class TM>
bool divide_element(Element<Penta,TN,TNG,TD,NET> &e,TM &m,TNG **nnodes) {
    std::cout << "divide_element not implemented for Quad_42" << std::endl;
    assert(0);
    return false;
}

template<class TN,class TNG,class TD,unsigned NET,class TM>
bool divide_element_using_elem_children(Element<Penta,TN,TNG,TD,NET> &e,TM &m,TNG **nnodes) {
    return divide_element(e,m,nnodes);
}

}

//#include "element_Penta.h"

#endif // LMTPENTA_H
