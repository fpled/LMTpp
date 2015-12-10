//
// C++ Interface: node
//
// Description: 
//
//
// Author: Hugo LECLERC <leclerc@lmt.ens-cachan.fr>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMTBAR_H
#define LMTBAR_H

#include "../containers/staticassert.h"
#include "nodalelement.h"

namespace LMT {

// --------------------------------------------------------------------------------------------------------
/*!
    Barre sans dimension ou segment à 2 noeuds
    \verbatim
    .                    0--1
    \relates Mesh
    \relates Element
    \keyword Maillage/Elément
    \friend samir.amrouche@lmt.ens-cachan.fr
    \friend hugo.leclerc@lmt.ens-cachan.fr
*/
struct Bar {
    static const unsigned nb_var_inter = 1;
    static const unsigned nb_nodes = 2;
    static const char *name() { return "Bar"; }
    static const char *can_directly_be_represented_by() { return "Bar"; }
};

// --------------------------------------------------------------------------------------------------------
template<> struct NbChildrenElement<Bar,1> { static const unsigned res = 2; };

template<unsigned n> struct TypeChildrenElement<Bar,1,n> { typedef NodalElement T; };

template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Bar,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<1> nvi_to_subs) {
    het.add_element(e,ch,NodalElement(),e.node(0));
    het.add_element(e,ch,NodalElement(),e.node(1));
}
//template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
//void append_skin_elements(Element<Bar,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<1> nvi_to_subs) {}

// --------------------------------------------------------------------------------------------------------
/*!
    in 3D, all z are assumed to be identical

*/
template<class TN,class TNG,class TD,unsigned NET,class Pvec,class T>
void intersection(const Element<Bar,TN,TNG,TD,NET> &e,Pvec P1,Pvec P2,T &numP1P2,T &dist_ext) {
    Pvec P3( e.node(0)->pos );
    Pvec P4( e.node(1)->pos );
    
    double den = (P4[1]-P3[1])*(P2[0]-P1[0])-(P4[0]-P3[0])*(P2[1]-P1[1]);
    if ( fabs(den) <= 1e-20 ) numP1P2 = -1e40;
    den = 1.0 / den;
    numP1P2 = ( (P4[0]-P3[0])*(P1[1]-P3[1])-(P4[1]-P3[1])*(P1[0]-P3[0]) ) * den;
    dist_ext = ( (P2[0]-P1[0])*(P1[1]-P3[1])-(P2[1]-P1[1])*(P1[0]-P3[0]) ) * den;
    dist_ext = (dist_ext>0.5) * (dist_ext-1.0) - (dist_ext<=0.5) * dist_ext;
}

template<class TN,class TNG,class TD,unsigned NET>
typename TypePromote<Abs,typename TNG::T>::T measure( const Element<Bar,TN,TNG,TD,NET> &e ) {
    return abs( length( e.node(1)->pos - e.node(0)->pos ) );
}

template<class TN,class TNG,class TD,unsigned NET>
typename TNG::Pvec sample_normal(const Element<Bar,TN,TNG,TD,NET> &e) {
    DEBUGASSERT( (TNG::dim==2) );
    typename TNG::Pvec res;
    res.assign_and_complete_with_last( e.pos(1)[1]-e.pos(0)[1], e.pos(0)[0]-e.pos(1)[0] );
    res /= length(res);
    return res;
}

template<class TN,class TNG,class TD,unsigned NET>
typename TNG::Pvec sample_tangent(const Element<Bar,TN,TNG,TD,NET> &e) {
    typename TNG::Pvec res = e.nodes[1]->pos - e.nodes[0]->pos;
    return res / length( res );
}

template<class TN,class TNG,class TD,unsigned NET,class TM>
bool divide_element(Element<Bar,TN,TNG,TD,NET> &e,TM &m,TNG **nodes) {
    if ( ! nodes[0] ) return false;
    DM::copy( e, *m.add_element( Bar(), TN(), e.node(0), nodes[0] ) );
    DM::copy( e, *m.add_element( Bar(), TN(), nodes[0], e.node(1) ) );
    return true;
}

/** 
    new_nodes are independant nodes created to make a fine grid for integration. Used in TvrcFormulation
    \keyword Elément/Opération
    \friend samir.amrouche@lmt.ens-cachan.fr
    \friend hugo.leclerc@lmt.ens-cachan.fr
    \relates Triangle
    \relates length
    \relates get_ponderation
    \relates Element 
    \relates Bar
*/
template<class TN,class TNG,class TD,unsigned NET,class T>
void subdivision_element(const Element<Bar,TN,TNG,TD,NET> &e,Vec<TNG> &new_nodes,T max_dist) {
    T l = length( e.pos(1) - e.pos(0) );
    
    unsigned n = (unsigned)(l / max_dist);
    n = max( 1, n );
    for(unsigned i=0;i<n;++i) {
        TNG *nn = new_nodes.new_elem();
        std::pair<T,const TNG *> pond_list[] = {
            std::pair<T,const TNG *>( (i+1.0)/(n+1.0),     e.node(0) ),
            std::pair<T,const TNG *>( 1.0-(i+1.0)/(n+1.0), e.node(1) )
        };
        DM::get_ponderation( pond_list, 2, *nn );
    }
}

inline unsigned vtk_num( StructForType<Bar> ) { return 3; }

};

#include "element_Bar.h"

#endif // LMTBAR_H
