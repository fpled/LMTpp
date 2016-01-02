//
// C++ Interface: Quad_16
//
// Description:
//
//
// Author: Hugo LECLERC <leclerc@lmt.ens-cachan.fr>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMTQUAD_16_H
#define LMTQUAD_16_H

#include "../containers/basicops.h"
#include "Bar_4.h"

namespace LMT {

// --------------------------------------------------------------------------------------------------------
/*!
    Carré à 16 noeuds
    \verbatim
    .                    3-----9-----8-----2
    .                    |                 |
    .                    |                 |
    .                   10    14    15     7
    .                    |                 |
    .                    |                 |
    .                   11    12    13     6
    .                    |                 |
    .                    |                 |
    .                    0-----4-----5-----1
    \relates Mesh
    \relates Element
    \keyword Maillage/Elément
    \friend raphael.pasquier@lmt.ens-cachan.fr
    \friend hugo.leclerc@lmt.ens-cachan.fr
*/
struct Quad_16 {
    static const unsigned nb_var_inter = 2;
    static const unsigned nb_nodes = 16;
    static const char *name() { return "Quad_16"; }
    static const char *can_directly_be_represented_by() { return "Quad_16"; }
};

// --------------------------------------------------------------------------------------------------------
template<> struct NbChildrenElement<Quad_16,0> { static const unsigned res = 1; };
template<> struct NbChildrenElement<Quad_16,1> { static const unsigned res = 4; };
template<> struct NbChildrenElement<Quad_16,2> { static const unsigned res = 16; };

template<unsigned n> struct TypeChildrenElement<Quad_16,0,n> { typedef Quad_16 T; };
template<unsigned n> struct TypeChildrenElement<Quad_16,1,n> { typedef Bar_4 T; };
template<unsigned n> struct TypeChildrenElement<Quad_16,2,n> { typedef NodalElement T; };

template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Quad_16,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<0> nvi_to_subs) {
    het.add_element(e,ch,Quad_16(),e.node(0),e.node(1),e.node(2),e.node(3),e.node(4),e.node(5),e.node(6),e.node(7),e.node(8),e.node(9),e.node(10),e.node(11),e.node(12),e.node(13),e.node(14),e.node(15));
}
template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Quad_16,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<1> nvi_to_subs) {
    het.add_element(e,ch,Bar_4(),e.node(0),e.node(1),e.node(4),e.node(5));
    het.add_element(e,ch,Bar_4(),e.node(1),e.node(2),e.node(6),e.node(7));
    het.add_element(e,ch,Bar_4(),e.node(2),e.node(3),e.node(8),e.node(9));
    het.add_element(e,ch,Bar_4(),e.node(3),e.node(0),e.node(10),e.node(11));
}
template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Quad_16,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<2> nvi_to_subs) {
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
}

// --------------------------------------------------------------------------------------------------------
template<class TN,class TNG,class TD,unsigned NET,class TM,class T>
void update_edge_ratio(const Element<Quad_16,TN,TNG,TD,NET> &e,TM &m,T &edge_ratio) {
    T edge_length_0 = (m.get_children_of( e, Number<1>() )[ 0 ])->measure_virtual();
    T edge_length_1 = (m.get_children_of( e, Number<1>() )[ 1 ])->measure_virtual();
    T edge_length_2 = (m.get_children_of( e, Number<1>() )[ 2 ])->measure_virtual();
    T edge_length_3 = (m.get_children_of( e, Number<1>() )[ 3 ])->measure_virtual();
    edge_ratio = min( edge_length_0, edge_length_1, edge_length_2, edge_length_3 ) / max( edge_length_0, edge_length_1, edge_length_2, edge_length_3 );
}

template<class TN,class TNG,class TD,unsigned NET>
typename TypePromote<Abs,typename TNG::T>::T measure( const Element<Quad_16,TN,TNG,TD,NET> &e ) {
    typedef typename TNG::T P_T_pos;
    typename TNG::Pvec P0 = e.node(0)->pos, P1 = e.node(1)->pos, P2 = e.node(2)->pos, P3 = e.node(3)->pos;
    if ( TNG::dim==2 ) {
        P_T_pos D0 = 0.788675*P1[0]; P_T_pos D1 = 0.211325*P1[0]; P_T_pos D2 = 0.788675*P1[1]; P_T_pos D3 = 0.211325*P1[1]; 
        P_T_pos D4 = 0.788675*P2[0]; P_T_pos D5 = 0.211325*P2[0]; P_T_pos D6 = 0.211325*P2[1]; P_T_pos D7 = 0.788675*P2[1]; 
        P_T_pos D8 = 0.211325*P3[0]; P_T_pos D9 = 0.788675*P3[0]; P_T_pos D10 = 0.211325*P3[1]; P_T_pos D11 = 0.788675*P3[1]; 
        P_T_pos D12 = 0.211325*P0[0]; P_T_pos D13 = -D0-D12; D13 = D13+D4; D13 = D13+D8; D12 = D1-D12; D4 = D12+D4; 
        D4 = D4-D9; D12 = 0.788675*P0[0]; D0 = D0-D12; D0 = D0+D5; D0 = D0-D8; D1 = -D1-D12; D1 = D1+D5; D1 = D1+D9; 
        D5 = 0.788675*P0[1]; D8 = D2-D5; D8 = D8+D6; D8 = D8-D10; D9 = D13*D8; D8 = D8*D1; D5 = -D3-D5; D5 = D5+D6; 
        D5 = D5+D11; D6 = D0*D5; D6 = D6-D8; D6 = 0.25*D6; D5 = D4*D5; D8 = 0.211325*P0[1]; D2 = -D2-D8; D2 = D2+D7; 
        D2 = D2+D10; D4 = D2*D4; D0 = D2*D0; D0 = D0-D9; D0 = 0.25*D0; D2 = D3-D8; D2 = D2+D7; D2 = D2-D11; D3 = D13*D2; 
        D3 = D4-D3; D3 = 0.25*D3; D1 = D1*D2; D1 = D5-D1; D1 = 0.25*D1; D1 = D1+D3; D0 = D1+D0; D0 = D0+D6;
        return typename TypePromote<Abs,typename TNG::T>::T(abs(D0));
    }
    P_T_pos D0 = 0.211325*P0[0]; P_T_pos D1 = 0.788675*P0[0]; P_T_pos D2 = 0.788675*P0[1]; P_T_pos D3 = 0.211325*P0[1]; 
    P_T_pos D4 = 0.211325*P0[2]; P_T_pos D5 = 0.788675*P0[2]; P_T_pos D6 = 0.788675*P1[0]; P_T_pos D7 = -D6-D0; 
    D6 = D6-D1; P_T_pos D8 = 0.211325*P1[0]; D0 = D8-D0; D1 = -D8-D1; D8 = 0.788675*P1[1]; P_T_pos D9 = -D8-D3; 
    D8 = D8-D2; P_T_pos D10 = 0.211325*P1[1]; D2 = -D10-D2; D3 = D10-D3; D10 = 0.788675*P1[2]; P_T_pos D11 = D10-D5; 
    D10 = -D10-D4; P_T_pos D12 = 0.211325*P1[2]; D4 = D12-D4; D5 = -D12-D5; D12 = 0.788675*P2[0]; D7 = D7+D12; 
    D0 = D0+D12; D12 = 0.211325*P2[0]; D6 = D6+D12; D1 = D1+D12; D12 = 0.211325*P2[1]; D8 = D8+D12; D2 = D2+D12; 
    D12 = 0.788675*P2[1]; D9 = D9+D12; D3 = D3+D12; D12 = 0.788675*P2[2]; D10 = D10+D12; D4 = D4+D12; D12 = 0.211325*P2[2]; 
    D11 = D11+D12; D5 = D5+D12; D12 = 0.211325*P3[0]; D7 = D7+D12; D6 = D6-D12; D12 = 0.788675*P3[0]; D0 = D0-D12; 
    D1 = D1+D12; D12 = pow(D6,2); P_T_pos D13 = pow(D0,2); P_T_pos D14 = 0.211325*P3[1]; D9 = D9+D14; D8 = D8-D14; 
    D14 = 0.788675*P3[1]; D2 = D2+D14; D3 = D3-D14; D14 = pow(D8,2); D12 = D14+D12; D14 = pow(D3,2); D13 = D14+D13; 
    D14 = 0.211325*P3[2]; D11 = D11-D14; D10 = D10+D14; D14 = 0.788675*P3[2]; D4 = D4-D14; D5 = D5+D14; 
    D14 = pow(D11,2); D12 = D12+D14; D12 = pow(D12,0.5); D14 = D8/D12; P_T_pos D15 = D9*D14; P_T_pos D16 = D6/D12; 
    P_T_pos D17 = D7*D16; D15 = D15+D17; D12 = D11/D12; D17 = D10*D12; D15 = D15+D17; D17 = D15*D16; D17 = D7-D17; 
    P_T_pos D18 = D15*D14; D18 = D9-D18; P_T_pos D19 = D15*D12; D19 = D10-D19; P_T_pos D20 = D6*D16; P_T_pos D21 = D8*D14; 
    D20 = D20+D21; D21 = D11*D12; D20 = D20+D21; D21 = D2*D14; P_T_pos D22 = D1*D16; D21 = D21+D22; D22 = D5*D12; 
    D21 = D21+D22; D16 = D21*D16; D16 = D1-D16; D14 = D21*D14; D14 = D2-D14; D12 = D21*D12; D12 = D5-D12; D22 = pow(D17,2); 
    P_T_pos D23 = pow(D18,2); D22 = D22+D23; D23 = pow(D19,2); D22 = D22+D23; D22 = pow(D22,0.5); D17 = D17/D22; 
    D23 = D6*D17; D18 = D18/D22; P_T_pos D24 = D8*D18; D23 = D23+D24; D19 = D19/D22; D22 = D11*D19; D22 = D23+D22; 
    D15 = D15*D22; D17 = D7*D17; D18 = D9*D18; D17 = D17+D18; D18 = D10*D19; D17 = D17+D18; D17 = D20*D17; D15 = D17-D15; 
    D15 = 0.25*D15; D17 = pow(D16,2); D18 = pow(D14,2); D17 = D17+D18; D18 = pow(D12,2); D17 = D17+D18; D17 = pow(D17,0.5); 
    D16 = D16/D17; D6 = D6*D16; D14 = D14/D17; D8 = D8*D14; D6 = D6+D8; D8 = D12/D17; D11 = D11*D8; D6 = D6+D11; 
    D6 = D21*D6; D11 = D1*D16; D12 = D2*D14; D11 = D11+D12; D8 = D5*D8; D8 = D11+D8; D8 = D20*D8; D6 = D8-D6; D6 = 0.25*D6; 
    D8 = pow(D4,2); D8 = D13+D8; D8 = pow(D8,0.5); D11 = D3/D8; D12 = D9*D11; D13 = D2*D11; D14 = D0/D8; D16 = D7*D14; 
    D12 = D12+D16; D16 = D1*D14; D13 = D13+D16; D8 = D4/D8; D16 = D10*D8; D12 = D12+D16; D16 = D12*D14; D16 = D7-D16; 
    D17 = D12*D11; D17 = D9-D17; D18 = D12*D8; D18 = D10-D18; D19 = D5*D8; D13 = D13+D19; D19 = D13*D11; D19 = D2-D19; 
    D20 = D13*D14; D20 = D1-D20; D21 = D13*D8; D21 = D5-D21; D14 = D0*D14; D11 = D3*D11; D11 = D14+D11; D8 = D4*D8; 
    D8 = D11+D8; D11 = pow(D16,2); D14 = pow(D17,2); D11 = D11+D14; D14 = pow(D18,2); D11 = D11+D14; D11 = pow(D11,0.5); 
    D14 = D16/D11; D7 = D7*D14; D16 = D17/D11; D17 = D3*D16; D9 = D9*D16; D7 = D7+D9; D9 = D18/D11; D11 = D4*D9; 
    D9 = D10*D9; D7 = D7+D9; D7 = D7*D8; D9 = D0*D14; D9 = D17+D9; D9 = D9+D11; D9 = D12*D9; D7 = D7-D9; D7 = 0.25*D7; 
    D9 = pow(D19,2); D10 = pow(D20,2); D9 = D9+D10; D10 = pow(D21,2); D9 = D9+D10; D9 = pow(D9,0.5); D10 = D20/D9; 
    D0 = D0*D10; D11 = D19/D9; D3 = D3*D11; D0 = D0+D3; D3 = D21/D9; D4 = D4*D3; D0 = D0+D4; D0 = D13*D0; D1 = D1*D10; 
    D2 = D2*D11; D1 = D1+D2; D2 = D5*D3; D1 = D1+D2; D1 = D8*D1; D0 = D1-D0; D0 = 0.25*D0; D0 = D0+D7; D0 = D0+D15; 
    D0 = D0+D6; return typename TypePromote<Abs,typename TNG::T>::T(abs(D0));
}

template<class TN,class TNG,class TD,unsigned NET,class TM>
bool divide_element(Element<Quad_16,TN,TNG,TD,NET> &e,TM &m,TNG **nnodes) {
    std::cout << "divide_element not implemented for Quad_16" << std::endl;
    assert(0);
    return false;
}

template<class TN,class TNG,class TD,unsigned NET,class TM>
bool divide_element_using_elem_children(Element<Quad_16,TN,TNG,TD,NET> &e,TM &m,TNG **nnodes) {
    return divide_element(e,m,nnodes);
}

template<class TV,class T>
bool var_inter_is_inside( const Quad_16 &, const TV &var_inter, T tol = 0 ) {
    return heaviside( var_inter[0] + tol ) * heaviside( var_inter[1] + tol ) * heaviside( 1 - var_inter[0] + tol ) * heaviside( 1 - var_inter[1] + tol );
}

};

//#include "element_Quad_16.h"

#endif // LMTQUAD_16_H
