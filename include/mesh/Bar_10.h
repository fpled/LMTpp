//
// C++ Interface: Bar_10
//
// Description: 
//
//
// Author: Hugo LECLERC <leclerc@lmt.ens-cachan.fr>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMTBAR_10_H
#define LMTBAR_10_H

#include "../containers/staticassert.h"
#include "NodalElement.h"

namespace LMT {

// --------------------------------------------------------------------------------------------------------
/*!
    Bar_10 représente une barre sans dimension ou segment avec dix noeud dessus.  
    \verbatim
    .                    0--2--3--4--5--6--7--8--9--1
    \relates Mesh
    \relates Element
    \keyword Maillage/Elément
    \friend hugo.leclerc@lmt.ens-cachan.fr
    \keyword Maillage/Elément
*/

struct Bar_10 {
    static const unsigned nb_var_inter = 1;
    static const unsigned nb_nodes = 10;
    static const char *name() { return "Bar_10"; }
    static const char *can_directly_be_represented_by() { return "Bar_10"; }
};

// --------------------------------------------------------------------------------------------------------
template<> struct NbChildrenElement<Bar_10,0> { static const unsigned res = 1; };
template<> struct NbChildrenElement<Bar_10,1> { static const unsigned res = 2; };

template<unsigned n> struct TypeChildrenElement<Bar_10,0,n> { typedef Bar_10 T; };
template<unsigned n> struct TypeChildrenElement<Bar_10,1,n> { typedef NodalElement T; };

template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Bar_10,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<0> nvi_to_subs) {
    het.add_element(e,ch,Bar_10(),e.node(0),e.node(1),node(2),e.node(3),node(4),e.node(5),node(6),e.node(7),node(8),e.node(9));
}
template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Bar_10,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<1> nvi_to_subs) {
    het.add_element(e,ch,NodalElement(),e.node(0));
    het.add_element(e,ch,NodalElement(),e.node(1));
}

// --------------------------------------------------------------------------------------------------------

template<class TN,class TNG,class TD,unsigned NET>
typename TNG::Pvec sample_normal(const Element<Bar_10,TN,TNG,TD,NET> &e) {
    DEBUGASSERT( (TNG::dim==2) );
    typename TNG::Pvec res;
    res.assign_and_complete_with_last( e.pos(1)[1]-e.pos(0)[1], e.pos(0)[0]-e.pos(1)[0] );
    res /= length(res);
    return res;
}


/* TODO : exact computation */
template<class TN,class TNG,class TD,unsigned NET>
typename TypePromote<Abs,typename TNG::T>::T measure( const Element<Bar_10,TN,TNG,TD,NET> &e ) {
    return abs( length( e.node(1)->pos - e.node(0)->pos ) );
}


}

//#include "element_Bar_10.h"

#endif // LMTBAR_10_H
