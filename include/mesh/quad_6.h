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
#ifndef LMTQUAD_6_H
#define LMTQUAD_6_H

#include "../containers/basicops.h"
#include "bar.h"
#include "bar_3.h"

namespace LMT {

// --------------------------------------------------------------------------------------------------------
/*!
    Carré à 6 noeuds
    A faire

    \keyword Maillage/Elément
    \friend samir.amrouche@lmt.ens-cachan.fr
    \friend hugo.leclerc@lmt.ens-cachan.fr
*/
struct Quad_6 {
    static const unsigned nb_var_inter = 2;
    static const unsigned nb_nodes = 6;
    static const char *name() { return "Quad_6"; }
    static const char *can_directly_be_represented_by() { return "Quad"; }
};

// --------------------------------------------------------------------------------------------------------
template<> struct NbChildrenElement<Quad_6,1> { static const unsigned res = 4; };
template<> struct NbChildrenElement<Quad_6,2> { static const unsigned res = 6; };

template<> struct TypeChildrenElement<Quad_6,1,0> { typedef Bar_3 T; };
template<> struct TypeChildrenElement<Quad_6,1,1> { typedef Bar_3 T; };
template<> struct TypeChildrenElement<Quad_6,1,2> { typedef Bar T; };
template<> struct TypeChildrenElement<Quad_6,1,3> { typedef Bar T; };
template<unsigned n> struct TypeChildrenElement<Quad_6,2,n> { typedef NodalElement T; };

template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Quad_6,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<1> nvi_to_subs) {
    het.add_element(e,ch,Bar_3(),e.node(0),e.node(4),e.node(1));
    het.add_element(e,ch,Bar_3(),e.node(2),e.node(5),e.node(3));
    het.add_element(e,ch,Bar(),e.node(1),e.node(2));
    het.add_element(e,ch,Bar(),e.node(3),e.node(0));
}
template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Quad_6,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<2> nvi_to_subs) {
    het.add_element(e,ch,NodalElement(),e.node(0));
    het.add_element(e,ch,NodalElement(),e.node(1));
    het.add_element(e,ch,NodalElement(),e.node(2));
    het.add_element(e,ch,NodalElement(),e.node(3));
    het.add_element(e,ch,NodalElement(),e.node(4));
    het.add_element(e,ch,NodalElement(),e.node(5));
}

// --------------------------------------------------------------------------------------------------------
template<class TN,class TNG,class TD,unsigned NET,class TM,class T>
void update_edge_ratio(const Element<Quad_6,TN,TNG,TD,NET> &e,TM &m,T &edge_ratio) {
    T edge_length_0 = (m.get_children_of( e, Number<1>() )[ 0 ])->measure_virtual();
    T edge_length_1 = (m.get_children_of( e, Number<1>() )[ 1 ])->measure_virtual();
    T edge_length_2 = (m.get_children_of( e, Number<1>() )[ 2 ])->measure_virtual();
    T edge_length_3 = (m.get_children_of( e, Number<1>() )[ 3 ])->measure_virtual();
    edge_ratio = min( edge_length_0, edge_length_1, edge_length_2, edge_length_3 ) / max( edge_length_0, edge_length_1, edge_length_2, edge_length_3 );
}

template<class TN,class TNG,class TD,unsigned NET>
typename TypePromote<Abs,typename TNG::T>::T measure( const Element<Quad_6,TN,TNG,TD,NET> &e ) {
    std::cerr << "measure not implemented for Quad_6" << std::endl;
    assert(0);
    return typename TypePromote<Abs,typename TNG::T>::T(-1.);
}

template<class TN,class TNG,class TD,unsigned NET,class TM>
bool divide_element(Element<Quad_6,TN,TNG,TD,NET> &e,TM &m,TNG **nnodes) {
    std::cout << "divide_element not implemented for Quad_6" << std::endl;
    assert(0);
    return false;
}

template<class TN,class TNG,class TD,unsigned NET,class TM>
bool divide_element_using_elem_children(Element<Quad_6,TN,TNG,TD,NET> &e,TM &m,TNG **nnodes) {
    return divide_element(e,m,nnodes);
}

template<class TV,class T>
bool var_inter_is_inside( const Quad_6 &, const TV &var_inter, T tol = 0 ) {
    return heaviside( var_inter[0] + tol ) * heaviside( var_inter[1] + tol ) * heaviside( 1 - var_inter[0] + tol ) * heaviside( 1 - var_inter[1] + tol );
}

/// >= 0 -> inside, < 0 -> outside
template<class T,class TV>
T var_inter_insideness( const Quad_6 &e, const TV &var_inter ) {
    return min( min( min( var_inter[0], var_inter[1] ), 1 - var_inter[0] ), 1 - var_inter[1] );
}

/*!
    objectif :
        la fonction renvoie vrai si pos est dans le \a Quad_6 et faux sinon ( sous la condition que le \a Quad_6 est dans le plan ).
        Mais pour cet élément non linéaire , on considère le \a Quad engendré par les 4 premiers noeuds.
        
    param :
        Quad_6 : le type d'élément
        pos_nodes : le position des sommets dans le plan. Il faut que le Quad ne soit pas "croisé".
        pos : la position du point dans le plan

*/
template< class PosNodes, class Pvec > 
bool is_inside_linear( const Quad_6 &elem, const PosNodes &pos_nodes, const Pvec &pos ) {
    typedef typename Pvec::template SubType<0>::T T;
    Pvec AB = pos_nodes[ 1 ] - pos_nodes[ 0 ];
    Pvec BC = pos_nodes[ 2 ] - pos_nodes[ 1 ];
    Pvec XM = pos - pos_nodes[ 0 ];
    
    T det1 = AB[ 0 ] * XM[ 1 ] - AB[ 1 ] * XM[ 0 ];
    XM = pos - pos_nodes[ 1 ];
    T det2 = BC[ 0 ] * XM[ 1 ] - BC[ 1 ] * XM[ 0 ];
    
    if ( ( det1 * det2 ) >= 0 ) {
        Pvec CD = pos_nodes[ 3 ] - pos_nodes[ 2 ];
        XM = pos - pos_nodes[ 2 ];
        T det3 = CD[ 0 ] * XM[ 1 ] - CD[ 1 ] * XM[ 0 ];
        
        if ( ( det1 * det3 ) >= 0 and ( det2 * det3 ) >= 0 ) {
            Pvec DA = pos_nodes[ 0 ] - pos_nodes[ 3 ];
            XM = pos - pos_nodes[ 3 ];
            T det4 = DA[ 0 ] * XM[ 1 ] - DA[ 1 ] * XM[ 0 ];
            if ( ( det1 * det4 ) >= 0 )
                return true;
            else
                return false;        
        } else
            return false;
    } else
        return false;
}

};

#include "element_Quad_6.h"

#endif // LMTQUAD_6_H
