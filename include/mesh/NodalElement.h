//
// C++ Interface: NodalElement
//
// Description: 
//
//
// Author: Hugo LECLERC <leclerc@lmt.ens-cachan.fr>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMTNODALELEMENT_H
#define LMTNODALELEMENT_H

#include "meshcarac.h"

namespace LMT {

struct NodalElement {
    static const unsigned nb_var_inter = 0;
    static const unsigned nb_nodes = 1;
    static const char *name() { return "NodalElement"; }
    static const char *can_directly_be_represented_by() { return "NodalElement"; }
};

template<class TN,class TNG,class TD,unsigned NET>
typename TypePromote<Abs,typename TNG::T>::T measure( const Element<NodalElement,TN,TNG,TD,NET> &e ) {
    return typename TypePromote<Abs,typename TNG::T>::T(0);
}

template<class TN,class TNG,class TD,unsigned NET,class T>
void subdivision_element(const Element<NodalElement,TN,TNG,TD,NET> &e,Vec<TNG> &new_nodes,T max_dist) {
    new_nodes.free();
    DM::copy( *e.node(0), *new_nodes.new_elem() );
}

inline unsigned vtk_num( StructForType<NodalElement> ) { return 1; }

};

#include "element_NodalElement.h"

#endif // LMTNODALELEMENT_H
