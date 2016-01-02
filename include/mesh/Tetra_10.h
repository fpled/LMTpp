//
// C++ Interface: Tetra_10
//
// Description:
//
//
// Author: Hugo LECLERC <leclerc@lmt.ens-cachan.fr>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMTTETRA_10_H
#define LMTTETRA_10_H

#include "Triangle_6.h"

namespace LMT {

// --------------------------------------------------------------------------------------------------------
/*!
    Tétraèdre à 10 noeuds
    \verbatim
    .                          3
    .                         /|\
    .                        / |9\
    .                      7/  |  \8
    .                      /  /2\  \
    .                     / /6   5\ \
    .                    0/--- 4----\1
    \relates Mesh
    \relates Element
    \keyword Maillage/Elément
    \friend samir.amrouche@lmt.ens-cachan.fr
    \friend hugo.leclerc@lmt.ens-cachan.fr
*/
struct Tetra_10 {
    static const unsigned nb_var_inter = 3;
    static const unsigned nb_nodes = 10;
    static const char *name() { return "Tetra_10"; }
    static const char *can_directly_be_represented_by() { return "Tetra_10"; }
};

// --------------------------------------------------------------------------------------------------------
template<> struct NbChildrenElement<Tetra_10,0> { static const unsigned res = 1; };
template<> struct NbChildrenElement<Tetra_10,1> { static const unsigned res = 4; };
template<> struct NbChildrenElement<Tetra_10,2> { static const unsigned res = 6; };
template<> struct NbChildrenElement<Tetra_10,3> { static const unsigned res = 10; };

template<unsigned n> struct TypeChildrenElement<Tetra_10,0,n> { typedef Tetra_10 T; };
template<unsigned n> struct TypeChildrenElement<Tetra_10,1,n> { typedef Triangle_6 T; };
template<unsigned n> struct TypeChildrenElement<Tetra_10,2,n> { typedef Bar_3 T; };
template<unsigned n> struct TypeChildrenElement<Tetra_10,3,n> { typedef NodalElement T; };

template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Tetra_10,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<0> nvi_to_subs) {
    het.add_element(e,ch,Tetra_10(),e.node(0),e.node(1),e.node(2),e.node(3),e.node(4),e.node(5),e.node(6),e.node(7),e.node(8),e.node(9));
}
template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Tetra_10,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<1> nvi_to_subs) {
    het.add_element(e,ch,Triangle_6(),e.node(0),e.node(2),e.node(1),e.node(6),e.node(5),e.node(4));
    het.add_element(e,ch,Triangle_6(),e.node(0),e.node(1),e.node(3),e.node(4),e.node(8),e.node(7));
    het.add_element(e,ch,Triangle_6(),e.node(0),e.node(3),e.node(2),e.node(7),e.node(9),e.node(6));
    het.add_element(e,ch,Triangle_6(),e.node(1),e.node(2),e.node(3),e.node(5),e.node(9),e.node(8));
}
template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Tetra_10,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<2> nvi_to_subs) {
    het.add_element(e,ch,Bar_3(),e.node(0),e.node(4),e.node(1));
    het.add_element(e,ch,Bar_3(),e.node(1),e.node(5),e.node(2));
    het.add_element(e,ch,Bar_3(),e.node(2),e.node(6),e.node(0));
    het.add_element(e,ch,Bar_3(),e.node(0),e.node(7),e.node(3));
    het.add_element(e,ch,Bar_3(),e.node(1),e.node(8),e.node(3));
    het.add_element(e,ch,Bar_3(),e.node(2),e.node(9),e.node(3));
}
template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Tetra_10,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<3> nvi_to_subs) {
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
}

// --------------------------------------------------------------------------------------------------------
template<class TN,class TNG,class TD,unsigned NET,class Pvec,class T>
void update_circum_center(const Element<Tetra_10,TN,TNG,TD,NET> &e,Pvec &C,T &R) {
    C = getCenterOfCircumSphere( e.node(0)->pos, e.node(1)->pos, e.node(2)->pos, e.node(3)->pos );
    R = length( e.node(0)->pos - C );
}

template<class TN,class TNG,class TD,unsigned NET,class Pvec,class T>
void update_in_center(const Element<Tetra_10,TN,TNG,TD,NET> &e,Pvec &C,T &R) {
    C = getCenterOfInSphere( e.node(0)->pos, e.node(1)->pos, e.node(2)->pos, e.node(3)->pos );

    Pvec P0 = e.node(0)->pos, P1 = e.node(1)->pos, P2 = e.node(2)->pos, P3 = e.node(3)->pos;
    Pvec P01 = P1-P0, P02 = P2-P0, P03 = P3-P0, P12 = P2-P1, P13 = P3-P1;

    T x1 = length(P01);
    T x2 = length(P02);
    T x12 = length(P13);

    Pvec P01_N = P01 / x1;
    Pvec P02_N = P02 / x2;
    Pvec P12_N = P12 / x12;

    Pvec H1 = P03 - dot( P03, P01_N ) * P01_N;
    T y1 = length( H1 );
    T S1 = x1 * y1 / 2;

    Pvec H2 = P02 - dot( P02, P01_N ) * P01_N;
    T y2 = length( H2 );
    T S2 = x1 * y2 / 2;

    Pvec H3 = P03 - dot( P03, P02_N ) * P02_N;
    T y3 = length( H3 );
    T S3 = x2 * y3 / 2;

    Pvec H4 = P13 - dot( P13, P12_N ) * P12_N;
    T y4 = length( H4 );
    T S4 = x12 * y4 / 2;

    R = 3 * measure(e) / ( S1 + S2 + S3 + S4 );
}

template<class TN,class TNG,class TD,unsigned NET,class T>
void update_radius_ratio(const Element<Tetra_10,TN,TNG,TD,NET> &e,T &radius_ratio) {
    Vec<T> C_circum, C_in;
    T R_circum, R_in;
    update_circum_center( e, C_circum, R_circum );
    update_in_center( e, C_in, R_in );
    radius_ratio = R_in / R_circum;
}

template<class TN,class TNG,class TD,unsigned NET,class TM,class T>
void update_edge_ratio(const Element<Tetra_10,TN,TNG,TD,NET> &e,TM &m,T &edge_ratio) {
    T edge_length_0 = (m.get_children_of( e, Number<1>() )[ 0 ])->measure_virtual();
    T edge_length_1 = (m.get_children_of( e, Number<1>() )[ 1 ])->measure_virtual();
    T edge_length_2 = (m.get_children_of( e, Number<1>() )[ 2 ])->measure_virtual();
    T edge_length_3 = (m.get_children_of( e, Number<1>() )[ 3 ])->measure_virtual();
    edge_ratio = min( edge_length_0, edge_length_1, edge_length_2, edge_length_3 ) / max( edge_length_0, edge_length_1, edge_length_2, edge_length_3 );
}

template<class TN,class TNG,class TD,unsigned NET>
typename TypePromote<Abs,typename TNG::T>::T measure( const Element<Tetra_10,TN,TNG,TD,NET> &e ) {
    typedef typename TNG::T P_T_pos;
    typename TNG::Pvec P0 = e.node(0)->pos, P1 = e.node(1)->pos, P2 = e.node(2)->pos, P3 = e.node(3)->pos;
    P_T_pos D0 = P1[0]-P0[0]; P_T_pos D1 = P2[0]-P0[0]; P_T_pos D2 = P3[0]-P0[0]; P_T_pos D3 = P2[1]-P0[1];
    P_T_pos D4 = P3[1]-P0[1]; P_T_pos D5 = P1[1]-P0[1]; P_T_pos D6 = P2[2]-P0[2]; P_T_pos D7 = D4*D6;
    P_T_pos D8 = P3[2]-P0[2]; P_T_pos D9 = D3*D8; D7 = D9-D7; D0 = D0*D7; D7 = D8*D5; D8 = P1[2]-P0[2];
    D4 = D4*D8; D4 = D7-D4; D1 = D1*D4; D0 = D0-D1; D1 = D6*D5; D3 = D3*D8; D1 = D1-D3; D1 = D1*D2; D0 = D0+D1; D1 = 0.075*D0;
    D0 = 0.133333*D0; D0 = D1-D0; D0 = D0+D1; D0 = D0+D1; D0 = D0+D1; return typename TypePromote<Abs,typename TNG::T>::T(abs(D0));
}

template<class TN,class TNG,class TD,unsigned NET,class TM>
bool divide_element(Element<Tetra_10,TN,TNG,TD,NET> &e,TM &m,TNG **nodes) {
    std::cout << "divide_element not implemented for Tetra_10" <<std::endl;
    assert(0);
    return false;
}

template<class TN,class TNG,class TD,unsigned NET,class TM>
bool divide_element_using_elem_children(Element<Tetra_10,TN,TNG,TD,NET> &e,TM &m,TNG **nnodes) {
    return divide_element(e,m,nnodes);
}

/** new_nodes are independant nodes created to make a fine grid for integration. Used in TvrcFormulation
    \relates Tetra_10
    \todo actually, we divide element n times, with n = max(edge length) / max_dist. For flat elements, it's far from optimallity
*/
template<class TN,class TNG,class TD,unsigned NET,class T>
bool subdivision_element(const Element<Tetra_10,TN,TNG,TD,NET> &e,Vec<TNG> &new_nodes,T max_dist) {
    T max_l = max(
            max( length( e.node(0)->pos - e.node(1)->pos ), length( e.node(0)->pos - e.node(2)->pos ), length( e.node(0)->pos - e.node(3)->pos ) ),
            max( length( e.node(1)->pos - e.node(2)->pos ), length( e.node(1)->pos - e.node(3)->pos ) ),
            length( e.node(2)->pos - e.node(3)->pos )
    );
    if ( max_l <= max_dist )
        return false;
    unsigned n = (unsigned)(max_l / max_dist);
    for(unsigned i=0;i<n;++i) {
        for(unsigned j=0;j<=i;++j) {
            for(unsigned k=0;k<=j;++k) {
                TNG *nn = new_nodes.new_elem();
                T f1 = (i+1.0)/(n+1.0);
                T f2 = (j+1.0)/(n+1.0);
                T f3 = (k+1.0)/(n+1.0);
                T f4 = 1.0-f1-f2-f3;
                std::pair<T,const TNG *> pond_list[] = {
                    std::pair<T,const TNG *>( f1, e.node(0) ),
                    std::pair<T,const TNG *>( f2, e.node(1) ),
                    std::pair<T,const TNG *>( f3, e.node(2) ),
                    std::pair<T,const TNG *>( f4, e.node(3) )
                };
                DM::get_ponderation( pond_list, 4, *nn );
            }
        }
    }
    return true;
}

/// >= 0 -> inside, < 0 -> outside
template<class T,class TV>
T var_inter_insideness( const Tetra_10 &e, const TV &var_inter ) {
    return min( min( min( var_inter[0], var_inter[1] ), var_inter[2] ), 1 - var_inter[0] - var_inter[1] - var_inter[2] );
}

inline unsigned vtk_num( StructForType<Tetra_10> ) { return 24; }

}

#include "element_Tetra_10.h"

#endif // LMTTETRA_10_H
