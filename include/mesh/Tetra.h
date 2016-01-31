//
// C++ Interface: Tetra
//
// Description: 
//
//
// Author: Hugo LECLERC <leclerc@lmt.ens-cachan.fr>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMTTETRA_H
#define LMTTETRA_H

#include "Triangle.h"

#ifdef METIL_COMP_DIRECTIVE
#pragma src_file refine_tetra_face.cpp
#endif


namespace LMT {

// --------------------------------------------------------------------------------------------------------
/*!
    Tétraèdre à 4 noeuds
    \verbatim
    .                          3
    .                         /|\
    .                        / | \
    .                       /  |  \
    .                      /  /2\  \
    .                     / /     \ \
    .                    0/---------\1
    \relates Mesh
    \relates Element
    \keyword Maillage/Elément
    \friend samir.amrouche@lmt.ens-cachan.fr
    \friend hugo.leclerc@lmt.ens-cachan.fr
*/
struct Tetra {
    static const unsigned nb_var_inter = 3;
    static const unsigned nb_nodes = 4;
    static const char *name() { return "Tetra"; }
    static const char *avs_name() { return "tet"; }
    static const char *can_directly_be_represented_by() { return "Tetra"; }
};

// --------------------------------------------------------------------------------------------------------
template<> struct NbChildrenElement<Tetra,0> { static const unsigned res = 1; };
template<> struct NbChildrenElement<Tetra,1> { static const unsigned res = 4; };
template<> struct NbChildrenElement<Tetra,2> { static const unsigned res = 6; };
template<> struct NbChildrenElement<Tetra,3> { static const unsigned res = 4; };

template<unsigned n> struct TypeChildrenElement<Tetra,0,n> { typedef Tetra T; };
template<unsigned n> struct TypeChildrenElement<Tetra,1,n> { typedef Triangle T; };
template<unsigned n> struct TypeChildrenElement<Tetra,2,n> { typedef Bar T; };
template<unsigned n> struct TypeChildrenElement<Tetra,3,n> { typedef NodalElement T; };

template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Tetra,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<0> nvi_to_subs) {
    het.add_element(e,ch,Tetra(),e.node(0),e.node(1),e.node(2),e.node(3));
}
// TODO : attention ordre compatible avec divide_element (ne correspond pas a celui de element_Tetra.py)
template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Tetra,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<1> nvi_to_subs) {
    het.add_element(e,ch,Triangle(),e.node(0),e.node(2),e.node(1));
    het.add_element(e,ch,Triangle(),e.node(0),e.node(1),e.node(3));
    het.add_element(e,ch,Triangle(),e.node(0),e.node(3),e.node(2));
    het.add_element(e,ch,Triangle(),e.node(1),e.node(2),e.node(3));
}
template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Tetra,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<2> nvi_to_subs) {
    het.add_element(e,ch,Bar(),e.node(0),e.node(1));
    het.add_element(e,ch,Bar(),e.node(0),e.node(2));
    het.add_element(e,ch,Bar(),e.node(0),e.node(3));
    het.add_element(e,ch,Bar(),e.node(1),e.node(2));
    het.add_element(e,ch,Bar(),e.node(1),e.node(3));
    het.add_element(e,ch,Bar(),e.node(2),e.node(3));
}
template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Tetra,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<3> nvi_to_subs) {
    het.add_element(e,ch,NodalElement(),e.node(0));
    het.add_element(e,ch,NodalElement(),e.node(1));
    het.add_element(e,ch,NodalElement(),e.node(2));
    het.add_element(e,ch,NodalElement(),e.node(3));
}

// --------------------------------------------------------------------------------------------------------
template<class TN,class TNG,class TD,unsigned NET,class Pvec,class T>
void update_circum_center(const Element<Tetra,TN,TNG,TD,NET> &e,Pvec &C,T &R) {
    C = getCenterOfCircumSphere( e.node(0)->pos, e.node(1)->pos, e.node(2)->pos, e.node(3)->pos );
    R = length( e.node(0)->pos - C );
}

template<class TN,class TNG,class TD,unsigned NET,class Pvec,class T>
void update_in_center(const Element<Tetra,TN,TNG,TD,NET> &e,Pvec &C,T &R) {
    C = getCenterOfInSphere( e.node(0)->pos, e.node(1)->pos, e.node(2)->pos, e.node(3)->pos );

    Pvec P0 = e.node(0)->pos, P1 = e.node(1)->pos, P2 = e.node(2)->pos, P3 = e.node(3)->pos;
    Pvec P01 = P1-P0, P02 = P2-P0, P03 = P3-P0, P12 = P2-P1, P13 = P3-P1;

    T x1 = length(P01);
    T x2 = length(P02);
    T x12 = length(P12);

    Pvec P01_N = P01 / x1;
    Pvec P02_N = P02 / x2;
    Pvec P12_N = P12 / x12;

    Pvec H1 = P02 - dot( P02, P01_N ) * P01_N;
    T y1 = length( H1 );
    T S1 = x1 * y1 / 2;

    Pvec H2 = P03 - dot( P03, P01_N ) * P01_N;
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
void update_radius_ratio(const Element<Tetra,TN,TNG,TD,NET> &e,T &radius_ratio) {
    Vec<T> C_circum, C_in;
    T R_circum, R_in;
    update_circum_center( e, C_circum, R_circum );
    update_in_center( e, C_in, R_in );
    radius_ratio = R_in / R_circum;
}

template<class TN,class TNG,class TD,unsigned NET,class TM,class T>
void update_edge_ratio(const Element<Tetra,TN,TNG,TD,NET> &e,TM &m,T &edge_ratio) {
    T edge_length_0 = (m.get_children_of( e, Number<1>() )[ 0 ])->measure_virtual();
    T edge_length_1 = (m.get_children_of( e, Number<1>() )[ 1 ])->measure_virtual();
    T edge_length_2 = (m.get_children_of( e, Number<1>() )[ 2 ])->measure_virtual();
    T edge_length_3 = (m.get_children_of( e, Number<1>() )[ 3 ])->measure_virtual();
    edge_ratio = min( edge_length_0, edge_length_1, edge_length_2, edge_length_3 ) / max( edge_length_0, edge_length_1, edge_length_2, edge_length_3 );
}

template<class TN,class TNG,class TD,unsigned NET>
typename TypePromote<Abs,typename TNG::T>::T measure( const Element<Tetra,TN,TNG,TD,NET> &e ) {
    typedef typename TNG::T P_T_pos;
    typename TNG::Pvec P0 = e.node(0)->pos, P1 = e.node(1)->pos, P2 = e.node(2)->pos, P3 = e.node(3)->pos;
    P_T_pos D0 = P1[0]-P0[0]; P_T_pos D1 = P2[0]-P0[0]; P_T_pos D2 = P3[0]-P0[0]; P_T_pos D3 = P2[1]-P0[1];
    P_T_pos D4 = P3[1]-P0[1]; P_T_pos D5 = P1[1]-P0[1]; P_T_pos D6 = P2[2]-P0[2]; P_T_pos D7 = D4*D6;
    P_T_pos D8 = P3[2]-P0[2]; P_T_pos D9 = D3*D8; D7 = D9-D7; D0 = D0*D7; D7 = D8*D5; D8 = P1[2]-P0[2];
    D4 = D4*D8; D4 = D7-D4; D1 = D1*D4; D0 = D0-D1; D1 = D6*D5; D3 = D3*D8; D1 = D1-D3; D1 = D1*D2; D0 = D0+D1; D1 = 0.075*D0;
    D0 = 0.133333*D0; D0 = D1-D0; D0 = D0+D1; D0 = D0+D1; D0 = D0+D1; return typename TypePromote<Abs,typename TNG::T>::T(abs(D0));
}

/*!
    Objectif:
        Cette fonction sert à couper un Tétra ou sous-Tétra
        
    Paramètres :
        * <strong> e </strong> est l'élément qu'on veut couper,
        * <strong> m </strong> es tle maillage auquel il appartient,
        * <strong> nodes </strong> est un vecteur de Node*, de taille le nombre de barres.
        
    Remarques:
    Cette fonction a été générée par le programmme *** qui est basé sur l'article de Detlef Ruprecht et Heinrich Müller, A Scheme for Edge-based Adaptive Tetrahedron Subdivision.
    Voici la numérotatoin des noeuds et des barres entre parenthèses dans un repère cartésien.
    \verbatim
        .                |3
        .                |
        .                |(2)   (5)
        .                |
        .           (4)  |___(1)___2
        .               /0         
        .              /
        .             /(0)     (3)
        .            /
        .            1

    J'ai choisi les schémas et les configurations de l'article avec la numération des noeuds et des barres suivante ( qui sont les mêmes que précédemment ) :
    \verbatim
        .                     D
        .                    /|\
        .                   / | \
        .               (2)/  |  \(5)
        .                 /   |   \
        .                /    |    \
        .             A /__(1)|_____\ C
        .               \     |     /
        .                \   (4)   / 
        .              (0)\   |   /(3)
        .                  \  |  /
        .                   \ | /
        .                    \|/
        .                     B

*/
template<class TN,class TNG,class TD,unsigned NET,class TM>
bool divide_element( Element< Tetra, TN, TNG, TD, NET> &e, TM &m, TNG **nodes ) {
    typedef typename TNG::T T;
    
    unsigned index = bool(nodes[0]) + bool(nodes[1])*2 + bool(nodes[2])*4 + bool(nodes[3])*8 + 
                     bool(nodes[4])*16 + bool(nodes[5])*32;

    TNG *nn[4];

    switch( index ) {
        case 0 : { /// configuration number 0
            return false;
        } break;
        case 1 : { /// configuration number 1
            nn[0] = e.node( 2 );
            nn[1] = e.node( 3 );
            nn[2] = e.node( 0 );
            nn[3] = nodes[ 0 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 2 );
            nn[1] = e.node( 3 );
            nn[2] = e.node( 1 );
            nn[3] = nodes[ 0 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
        } break;
        case 2 : { /// configuration number 1
            nn[0] = e.node( 1 );
            nn[1] = e.node( 3 );
            nn[2] = e.node( 0 );
            nn[3] = nodes[ 1 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 1 );
            nn[1] = e.node( 3 );
            nn[2] = e.node( 2 );
            nn[3] = nodes[ 1 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
        } break;
        case 3 : { /// configuration number 2
            bool AF;
            T l_AF = length( e.pos( 1 ) - nodes[ 1 ]->pos );
            T l_CE = length( e.pos( 2 ) - nodes[ 0 ]->pos );
            if ( l_AF < l_CE ) {
                AF = true;
            } else {
                if ( l_AF > l_CE )
                    AF = false;
                else {
                    if ( e.node( 1 )->number < e.node( 2 )->number ) AF = true; else AF = false; 
                }
            }
            if ( AF ) {
                nn[0] = e.node( 3 );
                nn[1] = e.node( 0 );
                nn[2] = nodes[ 0 ];
                nn[3] = nodes[ 1 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 1 );
                nn[1] = e.node( 3 );
                nn[2] = nodes[ 0 ];
                nn[3] = nodes[ 1 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 1 );
                nn[1] = e.node( 3 );
                nn[2] = e.node( 2 );
                nn[3] = nodes[ 1 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            } else {
                nn[0] = e.node( 3 );
                nn[1] = e.node( 0 );
                nn[2] = nodes[ 0 ];
                nn[3] = nodes[ 1 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 3 );
                nn[1] = e.node( 2 );
                nn[2] = nodes[ 0 ];
                nn[3] = nodes[ 1 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 1 );
                nn[1] = e.node( 3 );
                nn[2] = e.node( 2 );
                nn[3] = nodes[ 0 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            }
        } break;
        case 4 : { /// configuration number 1
            nn[0] = e.node( 1 );
            nn[1] = e.node( 2 );
            nn[2] = e.node( 0 );
            nn[3] = nodes[ 2 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 1 );
            nn[1] = e.node( 2 );
            nn[2] = e.node( 3 );
            nn[3] = nodes[ 2 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
        } break;
        case 5 : { /// configuration number 2
            bool AF;
            T l_AF = length( e.pos( 1 ) - nodes[ 2 ]->pos );
            T l_CE = length( e.pos( 3 ) - nodes[ 0 ]->pos );
            if ( l_AF < l_CE ) {
                AF = true;
            } else {
                if ( l_AF > l_CE )
                    AF = false;
                else {
                    if ( e.node( 1 )->number < e.node( 3 )->number ) AF = true; else AF = false; 
                }
            }
            if ( AF ) {
                nn[0] = e.node( 2 );
                nn[1] = e.node( 0 );
                nn[2] = nodes[ 0 ];
                nn[3] = nodes[ 2 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 1 );
                nn[1] = e.node( 2 );
                nn[2] = nodes[ 0 ];
                nn[3] = nodes[ 2 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 1 );
                nn[1] = e.node( 2 );
                nn[2] = e.node( 3 );
                nn[3] = nodes[ 2 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            } else {
                nn[0] = e.node( 2 );
                nn[1] = e.node( 0 );
                nn[2] = nodes[ 0 ];
                nn[3] = nodes[ 2 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 2 );
                nn[1] = e.node( 3 );
                nn[2] = nodes[ 0 ];
                nn[3] = nodes[ 2 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 1 );
                nn[1] = e.node( 2 );
                nn[2] = e.node( 3 );
                nn[3] = nodes[ 0 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            }
        } break;
        case 6 : { /// configuration number 2
            bool AF;
            T l_AF = length( e.pos( 2 ) - nodes[ 2 ]->pos );
            T l_CE = length( e.pos( 3 ) - nodes[ 1 ]->pos );
            if ( l_AF < l_CE ) {
                AF = true;
            } else {
                if ( l_AF > l_CE )
                    AF = false;
                else {
                    if ( e.node( 2 )->number < e.node( 3 )->number ) AF = true; else AF = false; 
                }
            }
            if ( AF ) {
                nn[0] = e.node( 1 );
                nn[1] = e.node( 0 );
                nn[2] = nodes[ 1 ];
                nn[3] = nodes[ 2 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 2 );
                nn[1] = e.node( 1 );
                nn[2] = nodes[ 1 ];
                nn[3] = nodes[ 2 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 2 );
                nn[1] = e.node( 1 );
                nn[2] = e.node( 3 );
                nn[3] = nodes[ 2 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            } else {
                nn[0] = e.node( 1 );
                nn[1] = e.node( 0 );
                nn[2] = nodes[ 1 ];
                nn[3] = nodes[ 2 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 1 );
                nn[1] = e.node( 3 );
                nn[2] = nodes[ 1 ];
                nn[3] = nodes[ 2 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 2 );
                nn[1] = e.node( 1 );
                nn[2] = e.node( 3 );
                nn[3] = nodes[ 1 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            }
        } break;
        case 7 : { /// configuration number 4
            std::cerr << "more complicated configuration number 7 to do" << std::endl;
            assert( 0 );
        } break;
        case 8 : { /// configuration number 1
            nn[0] = e.node( 0 );
            nn[1] = e.node( 3 );
            nn[2] = e.node( 1 );
            nn[3] = nodes[ 3 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 0 );
            nn[1] = e.node( 3 );
            nn[2] = e.node( 2 );
            nn[3] = nodes[ 3 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
        } break;
        case 9 : { /// configuration number 2
            bool AF;
            T l_AF = length( e.pos( 0 ) - nodes[ 3 ]->pos );
            T l_CE = length( e.pos( 2 ) - nodes[ 0 ]->pos );
            if ( l_AF < l_CE ) {
                AF = true;
            } else {
                if ( l_AF > l_CE )
                    AF = false;
                else {
                    if ( e.node( 0 )->number < e.node( 2 )->number ) AF = true; else AF = false; 
                }
            }
            if ( AF ) {
                nn[0] = e.node( 3 );
                nn[1] = e.node( 1 );
                nn[2] = nodes[ 0 ];
                nn[3] = nodes[ 3 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 0 );
                nn[1] = e.node( 3 );
                nn[2] = nodes[ 0 ];
                nn[3] = nodes[ 3 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 0 );
                nn[1] = e.node( 3 );
                nn[2] = e.node( 2 );
                nn[3] = nodes[ 3 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            } else {
                nn[0] = e.node( 3 );
                nn[1] = e.node( 1 );
                nn[2] = nodes[ 0 ];
                nn[3] = nodes[ 3 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 3 );
                nn[1] = e.node( 2 );
                nn[2] = nodes[ 0 ];
                nn[3] = nodes[ 3 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 0 );
                nn[1] = e.node( 3 );
                nn[2] = e.node( 2 );
                nn[3] = nodes[ 0 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            }
        } break;
        case 10 : { /// configuration number 2
            bool AF;
            T l_AF = length( e.pos( 0 ) - nodes[ 3 ]->pos );
            T l_CE = length( e.pos( 1 ) - nodes[ 1 ]->pos );
            if ( l_AF < l_CE ) {
                AF = true;
            } else {
                if ( l_AF > l_CE )
                    AF = false;
                else {
                    if ( e.node( 0 )->number < e.node( 1 )->number ) AF = true; else AF = false; 
                }
            }
            if ( AF ) {
                nn[0] = e.node( 3 );
                nn[1] = e.node( 2 );
                nn[2] = nodes[ 1 ];
                nn[3] = nodes[ 3 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 0 );
                nn[1] = e.node( 3 );
                nn[2] = nodes[ 1 ];
                nn[3] = nodes[ 3 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 0 );
                nn[1] = e.node( 3 );
                nn[2] = e.node( 1 );
                nn[3] = nodes[ 3 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            } else {
                nn[0] = e.node( 3 );
                nn[1] = e.node( 2 );
                nn[2] = nodes[ 1 ];
                nn[3] = nodes[ 3 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 3 );
                nn[1] = e.node( 1 );
                nn[2] = nodes[ 1 ];
                nn[3] = nodes[ 3 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 0 );
                nn[1] = e.node( 3 );
                nn[2] = e.node( 1 );
                nn[3] = nodes[ 1 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            }
        } break;
        case 11 : { /// configuration number 5
            nn[0] = e.node( 0 );
            nn[1] = e.node( 3 );
            nn[2] = nodes[ 0 ];
            nn[3] = nodes[ 1 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 3 );
            nn[1] = e.node( 1 );
            nn[2] = nodes[ 0 ];
            nn[3] = nodes[ 3 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 3 );
            nn[1] = nodes[ 0 ];
            nn[2] = nodes[ 1 ];
            nn[3] = nodes[ 3 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 3 );
            nn[1] = e.node( 2 );
            nn[2] = nodes[ 1 ];
            nn[3] = nodes[ 3 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
        } break;
        case 12 : { /// configuration number 3
            nn[0] = e.node( 0 );
            nn[1] = nodes[ 2 ];
            nn[2] = nodes[ 3 ];
            nn[3] = e.node( 2 );
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 0 );
            nn[1] = nodes[ 2 ];
            nn[2] = nodes[ 3 ];
            nn[3] = e.node( 1 );
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 3 );
            nn[1] = nodes[ 2 ];
            nn[2] = nodes[ 3 ];
            nn[3] = e.node( 2 );
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 3 );
            nn[1] = nodes[ 2 ];
            nn[2] = nodes[ 3 ];
            nn[3] = e.node( 1 );
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
        } break;
        case 13 : { /// configuration number 6
            std::cerr << "more complicated configuration number 13 to do" << std::endl;
            assert( 0 );
        } break;
        case 14 : { /// configuration number 6
            std::cerr << "more complicated configuration number 14 to do" << std::endl;
            assert( 0 );
        } break;
        case 15 : { /// configuration number 7
            std::cerr << "more complicated configuration number 15 to do" << std::endl;
            assert( 0 );
        } break;
        case 16 : { /// configuration number 1
            nn[0] = e.node( 0 );
            nn[1] = e.node( 2 );
            nn[2] = e.node( 1 );
            nn[3] = nodes[ 4 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 0 );
            nn[1] = e.node( 2 );
            nn[2] = e.node( 3 );
            nn[3] = nodes[ 4 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
        } break;
        case 17 : { /// configuration number 2
            bool AF;
            T l_AF = length( e.pos( 0 ) - nodes[ 4 ]->pos );
            T l_CE = length( e.pos( 3 ) - nodes[ 0 ]->pos );
            if ( l_AF < l_CE ) {
                AF = true;
            } else {
                if ( l_AF > l_CE )
                    AF = false;
                else {
                    if ( e.node( 0 )->number < e.node( 3 )->number ) AF = true; else AF = false; 
                }
            }
            if ( AF ) {
                nn[0] = e.node( 2 );
                nn[1] = e.node( 1 );
                nn[2] = nodes[ 0 ];
                nn[3] = nodes[ 4 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 0 );
                nn[1] = e.node( 2 );
                nn[2] = nodes[ 0 ];
                nn[3] = nodes[ 4 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 0 );
                nn[1] = e.node( 2 );
                nn[2] = e.node( 3 );
                nn[3] = nodes[ 4 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            } else {
                nn[0] = e.node( 2 );
                nn[1] = e.node( 1 );
                nn[2] = nodes[ 0 ];
                nn[3] = nodes[ 4 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 2 );
                nn[1] = e.node( 3 );
                nn[2] = nodes[ 0 ];
                nn[3] = nodes[ 4 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 0 );
                nn[1] = e.node( 2 );
                nn[2] = e.node( 3 );
                nn[3] = nodes[ 0 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            }
        } break;
        case 18 : { /// configuration number 3
            nn[0] = e.node( 0 );
            nn[1] = nodes[ 1 ];
            nn[2] = nodes[ 4 ];
            nn[3] = e.node( 3 );
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 0 );
            nn[1] = nodes[ 1 ];
            nn[2] = nodes[ 4 ];
            nn[3] = e.node( 1 );
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 2 );
            nn[1] = nodes[ 1 ];
            nn[2] = nodes[ 4 ];
            nn[3] = e.node( 3 );
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 2 );
            nn[1] = nodes[ 1 ];
            nn[2] = nodes[ 4 ];
            nn[3] = e.node( 1 );
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
        } break;
        case 19 : { /// configuration number 6
            std::cerr << "more complicated configuration number 19 to do" << std::endl;
            assert( 0 );
        } break;
        case 20 : { /// configuration number 2
            bool AF;
            T l_AF = length( e.pos( 0 ) - nodes[ 4 ]->pos );
            T l_CE = length( e.pos( 1 ) - nodes[ 2 ]->pos );
            if ( l_AF < l_CE ) {
                AF = true;
            } else {
                if ( l_AF > l_CE )
                    AF = false;
                else {
                    if ( e.node( 0 )->number < e.node( 1 )->number ) AF = true; else AF = false; 
                }
            }
            if ( AF ) {
                nn[0] = e.node( 2 );
                nn[1] = e.node( 3 );
                nn[2] = nodes[ 2 ];
                nn[3] = nodes[ 4 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 0 );
                nn[1] = e.node( 2 );
                nn[2] = nodes[ 2 ];
                nn[3] = nodes[ 4 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 0 );
                nn[1] = e.node( 2 );
                nn[2] = e.node( 1 );
                nn[3] = nodes[ 4 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            } else {
                nn[0] = e.node( 2 );
                nn[1] = e.node( 3 );
                nn[2] = nodes[ 2 ];
                nn[3] = nodes[ 4 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 2 );
                nn[1] = e.node( 1 );
                nn[2] = nodes[ 2 ];
                nn[3] = nodes[ 4 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 0 );
                nn[1] = e.node( 2 );
                nn[2] = e.node( 1 );
                nn[3] = nodes[ 2 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            }
        } break;
        case 21 : { /// configuration number 5
            nn[0] = e.node( 0 );
            nn[1] = e.node( 2 );
            nn[2] = nodes[ 0 ];
            nn[3] = nodes[ 2 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 2 );
            nn[1] = e.node( 1 );
            nn[2] = nodes[ 0 ];
            nn[3] = nodes[ 4 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 2 );
            nn[1] = nodes[ 0 ];
            nn[2] = nodes[ 2 ];
            nn[3] = nodes[ 4 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 2 );
            nn[1] = e.node( 3 );
            nn[2] = nodes[ 2 ];
            nn[3] = nodes[ 4 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
        } break;
        case 22 : { /// configuration number 6
            std::cerr << "more complicated configuration number 22 to do" << std::endl;
            assert( 0 );
        } break;
        case 23 : { /// configuration number 7
            std::cerr << "more complicated configuration number 23 to do" << std::endl;
            assert( 0 );
        } break;
        case 24 : { /// configuration number 2
            bool AF;
            T l_AF = length( e.pos( 2 ) - nodes[ 4 ]->pos );
            T l_CE = length( e.pos( 3 ) - nodes[ 3 ]->pos );
            if ( l_AF < l_CE ) {
                AF = true;
            } else {
                if ( l_AF > l_CE )
                    AF = false;
                else {
                    if ( e.node( 2 )->number < e.node( 3 )->number ) AF = true; else AF = false; 
                }
            }
            if ( AF ) {
                nn[0] = e.node( 0 );
                nn[1] = e.node( 1 );
                nn[2] = nodes[ 3 ];
                nn[3] = nodes[ 4 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 2 );
                nn[1] = e.node( 0 );
                nn[2] = nodes[ 3 ];
                nn[3] = nodes[ 4 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 2 );
                nn[1] = e.node( 0 );
                nn[2] = e.node( 3 );
                nn[3] = nodes[ 4 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            } else {
                nn[0] = e.node( 0 );
                nn[1] = e.node( 1 );
                nn[2] = nodes[ 3 ];
                nn[3] = nodes[ 4 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 0 );
                nn[1] = e.node( 3 );
                nn[2] = nodes[ 3 ];
                nn[3] = nodes[ 4 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 2 );
                nn[1] = e.node( 0 );
                nn[2] = e.node( 3 );
                nn[3] = nodes[ 3 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            }
        } break;
        case 25 : { /// configuration number 4
            std::cerr << "more complicated configuration number 25 to do" << std::endl;
            assert( 0 );
        } break;
        case 26 : { /// configuration number 6
            std::cerr << "more complicated configuration number 26 to do" << std::endl;
            assert( 0 );
        } break;
        case 27 : { /// configuration number 7
            std::cerr << "more complicated configuration number 27 to do" << std::endl;
            assert( 0 );
        } break;
        case 28 : { /// configuration number 6
            std::cerr << "more complicated configuration number 28 to do" << std::endl;
            assert( 0 );
        } break;
        case 29 : { /// configuration number 7
            std::cerr << "more complicated configuration number 29 to do" << std::endl;
            assert( 0 );
        } break;
        case 30 : { /// configuration number 8
            std::cerr << "more complicated configuration number 30 to do" << std::endl;
            assert( 0 );
        } break;
        case 31 : { /// configuration number 9
            std::cerr << "more complicated configuration number 31 to do" << std::endl;
            assert( 0 );
        } break;
        case 32 : { /// configuration number 1
            nn[0] = e.node( 0 );
            nn[1] = e.node( 1 );
            nn[2] = e.node( 2 );
            nn[3] = nodes[ 5 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 0 );
            nn[1] = e.node( 1 );
            nn[2] = e.node( 3 );
            nn[3] = nodes[ 5 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
        } break;
        case 33 : { /// configuration number 3
            nn[0] = e.node( 0 );
            nn[1] = nodes[ 0 ];
            nn[2] = nodes[ 5 ];
            nn[3] = e.node( 3 );
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 0 );
            nn[1] = nodes[ 0 ];
            nn[2] = nodes[ 5 ];
            nn[3] = e.node( 2 );
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 1 );
            nn[1] = nodes[ 0 ];
            nn[2] = nodes[ 5 ];
            nn[3] = e.node( 3 );
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 1 );
            nn[1] = nodes[ 0 ];
            nn[2] = nodes[ 5 ];
            nn[3] = e.node( 2 );
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
        } break;
        case 34 : { /// configuration number 2
            bool AF;
            T l_AF = length( e.pos( 0 ) - nodes[ 5 ]->pos );
            T l_CE = length( e.pos( 3 ) - nodes[ 1 ]->pos );
            if ( l_AF < l_CE ) {
                AF = true;
            } else {
                if ( l_AF > l_CE )
                    AF = false;
                else {
                    if ( e.node( 0 )->number < e.node( 3 )->number ) AF = true; else AF = false; 
                }
            }
            if ( AF ) {
                nn[0] = e.node( 1 );
                nn[1] = e.node( 2 );
                nn[2] = nodes[ 1 ];
                nn[3] = nodes[ 5 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 0 );
                nn[1] = e.node( 1 );
                nn[2] = nodes[ 1 ];
                nn[3] = nodes[ 5 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 0 );
                nn[1] = e.node( 1 );
                nn[2] = e.node( 3 );
                nn[3] = nodes[ 5 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            } else {
                nn[0] = e.node( 1 );
                nn[1] = e.node( 2 );
                nn[2] = nodes[ 1 ];
                nn[3] = nodes[ 5 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 1 );
                nn[1] = e.node( 3 );
                nn[2] = nodes[ 1 ];
                nn[3] = nodes[ 5 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 0 );
                nn[1] = e.node( 1 );
                nn[2] = e.node( 3 );
                nn[3] = nodes[ 1 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            }
        } break;
        case 35 : { /// configuration number 6
            std::cerr << "more complicated configuration number 35 to do" << std::endl;
            assert( 0 );
        } break;
        case 36 : { /// configuration number 2
            bool AF;
            T l_AF = length( e.pos( 0 ) - nodes[ 5 ]->pos );
            T l_CE = length( e.pos( 2 ) - nodes[ 2 ]->pos );
            if ( l_AF < l_CE ) {
                AF = true;
            } else {
                if ( l_AF > l_CE )
                    AF = false;
                else {
                    if ( e.node( 0 )->number < e.node( 2 )->number ) AF = true; else AF = false; 
                }
            }
            if ( AF ) {
                nn[0] = e.node( 1 );
                nn[1] = e.node( 3 );
                nn[2] = nodes[ 2 ];
                nn[3] = nodes[ 5 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 0 );
                nn[1] = e.node( 1 );
                nn[2] = nodes[ 2 ];
                nn[3] = nodes[ 5 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 0 );
                nn[1] = e.node( 1 );
                nn[2] = e.node( 2 );
                nn[3] = nodes[ 5 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            } else {
                nn[0] = e.node( 1 );
                nn[1] = e.node( 3 );
                nn[2] = nodes[ 2 ];
                nn[3] = nodes[ 5 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 1 );
                nn[1] = e.node( 2 );
                nn[2] = nodes[ 2 ];
                nn[3] = nodes[ 5 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 0 );
                nn[1] = e.node( 1 );
                nn[2] = e.node( 2 );
                nn[3] = nodes[ 2 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            }
        } break;
        case 37 : { /// configuration number 6
            std::cerr << "more complicated configuration number 37 to do" << std::endl;
            assert( 0 );
        } break;
        case 38 : { /// configuration number 5
            nn[0] = e.node( 0 );
            nn[1] = e.node( 1 );
            nn[2] = nodes[ 1 ];
            nn[3] = nodes[ 2 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 1 );
            nn[1] = e.node( 2 );
            nn[2] = nodes[ 1 ];
            nn[3] = nodes[ 5 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 1 );
            nn[1] = nodes[ 1 ];
            nn[2] = nodes[ 2 ];
            nn[3] = nodes[ 5 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 1 );
            nn[1] = e.node( 3 );
            nn[2] = nodes[ 2 ];
            nn[3] = nodes[ 5 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
        } break;
        case 39 : { /// configuration number 7
            std::cerr << "more complicated configuration number 39 to do" << std::endl;
            assert( 0 );
        } break;
        case 40 : { /// configuration number 2
            bool AF;
            T l_AF = length( e.pos( 1 ) - nodes[ 5 ]->pos );
            T l_CE = length( e.pos( 3 ) - nodes[ 3 ]->pos );
            if ( l_AF < l_CE ) {
                AF = true;
            } else {
                if ( l_AF > l_CE )
                    AF = false;
                else {
                    if ( e.node( 1 )->number < e.node( 3 )->number ) AF = true; else AF = false; 
                }
            }
            if ( AF ) {
                nn[0] = e.node( 0 );
                nn[1] = e.node( 2 );
                nn[2] = nodes[ 3 ];
                nn[3] = nodes[ 5 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 1 );
                nn[1] = e.node( 0 );
                nn[2] = nodes[ 3 ];
                nn[3] = nodes[ 5 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 1 );
                nn[1] = e.node( 0 );
                nn[2] = e.node( 3 );
                nn[3] = nodes[ 5 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            } else {
                nn[0] = e.node( 0 );
                nn[1] = e.node( 2 );
                nn[2] = nodes[ 3 ];
                nn[3] = nodes[ 5 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 0 );
                nn[1] = e.node( 3 );
                nn[2] = nodes[ 3 ];
                nn[3] = nodes[ 5 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 1 );
                nn[1] = e.node( 0 );
                nn[2] = e.node( 3 );
                nn[3] = nodes[ 3 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            }
        } break;
        case 41 : { /// configuration number 6
            std::cerr << "more complicated configuration number 41 to do" << std::endl;
            assert( 0 );
        } break;
        case 42 : { /// configuration number 4
            std::cerr << "more complicated configuration number 42 to do" << std::endl;
            assert( 0 );
        } break;
        case 43 : { /// configuration number 7
            std::cerr << "more complicated configuration number 43 to do" << std::endl;
            assert( 0 );
        } break;
        case 44 : { /// configuration number 6
            std::cerr << "more complicated configuration number 44 to do" << std::endl;
            assert( 0 );
        } break;
        case 45 : { /// configuration number 8
            std::cerr << "more complicated configuration number 45 to do" << std::endl;
            assert( 0 );
        } break;
        case 46 : { /// configuration number 7
            std::cerr << "more complicated configuration number 46 to do" << std::endl;
            assert( 0 );
        } break;
        case 47 : { /// configuration number 9
            std::cerr << "more complicated configuration number 47 to do" << std::endl;
            assert( 0 );
        } break;
        case 48 : { /// configuration number 2
            bool AF;
            T l_AF = length( e.pos( 1 ) - nodes[ 5 ]->pos );
            T l_CE = length( e.pos( 2 ) - nodes[ 4 ]->pos );
            if ( l_AF < l_CE ) {
                AF = true;
            } else {
                if ( l_AF > l_CE )
                    AF = false;
                else {
                    if ( e.node( 1 )->number < e.node( 2 )->number ) AF = true; else AF = false; 
                }
            }
            if ( AF ) {
                nn[0] = e.node( 0 );
                nn[1] = e.node( 3 );
                nn[2] = nodes[ 4 ];
                nn[3] = nodes[ 5 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 1 );
                nn[1] = e.node( 0 );
                nn[2] = nodes[ 4 ];
                nn[3] = nodes[ 5 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 1 );
                nn[1] = e.node( 0 );
                nn[2] = e.node( 2 );
                nn[3] = nodes[ 5 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            } else {
                nn[0] = e.node( 0 );
                nn[1] = e.node( 3 );
                nn[2] = nodes[ 4 ];
                nn[3] = nodes[ 5 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 0 );
                nn[1] = e.node( 2 );
                nn[2] = nodes[ 4 ];
                nn[3] = nodes[ 5 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = e.node( 1 );
                nn[1] = e.node( 0 );
                nn[2] = e.node( 2 );
                nn[3] = nodes[ 4 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            }
        } break;
        case 49 : { /// configuration number 6
            std::cerr << "more complicated configuration number 49 to do" << std::endl;
            assert( 0 );
        } break;
        case 50 : { /// configuration number 6
            std::cerr << "more complicated configuration number 50 to do" << std::endl;
            assert( 0 );
        } break;
        case 51 : { /// configuration number 8
            std::cerr << "more complicated configuration number 51 to do" << std::endl;
            assert( 0 );
        } break;
        case 52 : { /// configuration number 4
            std::cerr << "more complicated configuration number 52 to do" << std::endl;
            assert( 0 );
        } break;
        case 53 : { /// configuration number 7
            std::cerr << "more complicated configuration number 53 to do" << std::endl;
            assert( 0 );
        } break;
        case 54 : { /// configuration number 7
            std::cerr << "more complicated configuration number 54 to do" << std::endl;
            assert( 0 );
        } break;
        case 55 : { /// configuration number 9
            std::cerr << "more complicated configuration number 55 to do" << std::endl;
            assert( 0 );
        } break;
        case 56 : { /// configuration number 5
            nn[0] = e.node( 1 );
            nn[1] = e.node( 0 );
            nn[2] = nodes[ 3 ];
            nn[3] = nodes[ 4 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 0 );
            nn[1] = e.node( 2 );
            nn[2] = nodes[ 3 ];
            nn[3] = nodes[ 5 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 0 );
            nn[1] = nodes[ 3 ];
            nn[2] = nodes[ 4 ];
            nn[3] = nodes[ 5 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 0 );
            nn[1] = e.node( 3 );
            nn[2] = nodes[ 4 ];
            nn[3] = nodes[ 5 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
        } break;
        case 57 : { /// configuration number 7
            std::cerr << "more complicated configuration number 57 to do" << std::endl;
            assert( 0 );
        } break;
        case 58 : { /// configuration number 7
            std::cerr << "more complicated configuration number 58 to do" << std::endl;
            assert( 0 );
        } break;
        case 59 : { /// configuration number 9
            std::cerr << "more complicated configuration number 59 to do" << std::endl;
            assert( 0 );
        } break;
        case 60 : { /// configuration number 7
            std::cerr << "more complicated configuration number 60 to do" << std::endl;
            assert( 0 );
        } break;
        case 61 : { /// configuration number 9
            std::cerr << "more complicated configuration number 61 to do" << std::endl;
            assert( 0 );
        } break;
        case 62 : { /// configuration number 9
            std::cerr << "more complicated configuration number 62 to do" << std::endl;
            assert( 0 );
        } break;
        case 63 : { /// configuration number 10
            nn[0] = e.node( 0 );
            nn[1] = nodes[ 0 ];
            nn[2] = nodes[ 1 ];
            nn[3] = nodes[ 2 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 1 );
            nn[1] = nodes[ 0 ];
            nn[2] = nodes[ 3 ];
            nn[3] = nodes[ 4 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 2 );
            nn[1] = nodes[ 1 ];
            nn[2] = nodes[ 3 ];
            nn[3] = nodes[ 5 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            nn[0] = e.node( 3 );
            nn[1] = nodes[ 2 ];
            nn[2] = nodes[ 4 ];
            nn[3] = nodes[ 5 ];
            permutation_if_jac_neg ( Tetra(), nn );
            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            /// octaèdre interne
            int smaller;
            T l_EJ = length( nodes[ 0 ]->pos - nodes[ 5 ]->pos );
            T l_GH = length( nodes[ 2 ]->pos - nodes[ 3 ]->pos );
            T l_FI = length( nodes[ 1 ]->pos - nodes[ 4 ]->pos );
            if ( l_EJ < l_GH ) {
                if ( l_FI < l_EJ ) smaller = 2; else smaller = 0;
            } else {
                if ( l_FI < l_GH ) smaller = 2; else smaller = 1;
            }
            if ( smaller == 0 ) {
                nn[0] = nodes[ 0 ];
                nn[1] = nodes[ 5 ];
                nn[2] = nodes[ 2 ];
                nn[3] = nodes[ 4 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = nodes[ 0 ];
                nn[1] = nodes[ 5 ];
                nn[2] = nodes[ 3 ];
                nn[3] = nodes[ 4 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = nodes[ 0 ];
                nn[1] = nodes[ 5 ];
                nn[2] = nodes[ 2 ];
                nn[3] = nodes[ 1 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                nn[0] = nodes[ 0 ];
                nn[1] = nodes[ 5 ];
                nn[2] = nodes[ 3 ];
                nn[3] = nodes[ 1 ];
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
            } else {
                if ( smaller == 1 ) {
                    nn[0] = nodes[ 2 ];
                    nn[1] = nodes[ 3 ];
                    nn[2] = nodes[ 4 ];
                    nn[3] = nodes[ 5 ];
                    permutation_if_jac_neg ( Tetra(), nn );
                    DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                    nn[0] = nodes[ 2 ];
                    nn[1] = nodes[ 3 ];
                    nn[2] = nodes[ 0 ];
                    nn[3] = nodes[ 4 ];
                    permutation_if_jac_neg ( Tetra(), nn );
                    DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                    nn[0] = nodes[ 2 ];
                    nn[1] = nodes[ 3 ];
                    nn[2] = nodes[ 1 ];
                    nn[3] = nodes[ 5 ];
                    permutation_if_jac_neg ( Tetra(), nn );
                    DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                    nn[0] = nodes[ 2 ];
                    nn[1] = nodes[ 3 ];
                    nn[2] = nodes[ 0 ];
                    nn[3] = nodes[ 1 ];
                    permutation_if_jac_neg ( Tetra(), nn );
                    DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                } else {
                    nn[0] = nodes[ 1 ];
                    nn[1] = nodes[ 4 ];
                    nn[2] = nodes[ 0 ];
                    nn[3] = nodes[ 3 ];
                    permutation_if_jac_neg ( Tetra(), nn );
                    DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                    nn[0] = nodes[ 1 ];
                    nn[1] = nodes[ 4 ];
                    nn[2] = nodes[ 3 ];
                    nn[3] = nodes[ 5 ];
                    permutation_if_jac_neg ( Tetra(), nn );
                    DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                    nn[0] = nodes[ 1 ];
                    nn[1] = nodes[ 4 ];
                    nn[2] = nodes[ 5 ];
                    nn[3] = nodes[ 2 ];
                    permutation_if_jac_neg ( Tetra(), nn );
                    DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                    nn[0] = nodes[ 1 ];
                    nn[1] = nodes[ 4 ];
                    nn[2] = nodes[ 2 ];
                    nn[3] = nodes[ 0 ];
                    permutation_if_jac_neg ( Tetra(), nn );
                    DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
                }
            }
        } break;
        default : ;
    }

    return true;
}

// template<class TN,class TNG,class TD,unsigned NET,class TM>
// bool divide_element(Element<Tetra,TN,TNG,TD,NET> &e,TM &m,TNG **nodes) {
//     unsigned valid_tetra[] = {
//         0,1,2,3, 0,1,2,6, 0,1,2,8, 0,1,2,9, 1,0,3,5, 1,0,3,7, 1,0,3,9, 0,1,5,6, 0,1,5,8, 0,1,5,9, 1,0,6,7, 1,0,6,9, 0,1,7,8, 0,1,7,9, 1,0,8,9, 0,2,3,4, 0,2,3,7,
//         0,2,3,8, 2,0,4,6, 2,0,4,8, 2,0,4,9, 0,2,6,7, 0,2,6,8, 2,0,7,8, 2,0,7,9, 2,0,8,9, 0,3,4,5, 0,3,4,7, 0,3,4,9, 3,0,5,7, 3,0,5,8, 3,0,7,8, 0,3,7,9, 0,3,8,9,
//         0,4,5,6, 0,4,5,8, 0,4,5,9, 4,0,6,7, 4,0,6,9, 0,4,7,8, 0,4,7,9, 4,0,8,9, 0,5,6,7, 0,5,6,8, 5,0,7,8, 5,0,7,9, 5,0,8,9, 6,0,7,8, 0,6,7,9, 0,6,8,9, 7,0,8,9,
//         2,1,3,4, 2,1,3,5, 2,1,3,6, 1,2,4,6, 1,2,4,8, 1,2,4,9, 1,2,5,6, 1,2,5,8, 1,2,5,9, 1,2,6,8, 1,2,6,9, 3,1,4,5, 3,1,4,7, 3,1,4,9, 1,3,5,6, 3,1,5,7, 3,1,5,9,
//         3,1,6,7, 3,1,6,9, 4,1,5,6, 4,1,5,8, 4,1,5,9, 1,4,6,7, 1,4,6,9, 4,1,7,8, 4,1,7,9, 1,4,8,9, 1,5,6,7, 1,5,6,8, 1,5,6,9, 5,1,7,8, 5,1,7,9, 1,5,8,9, 6,1,7,8,
//         6,1,7,9, 1,6,8,9, 3,2,4,5, 3,2,4,6, 2,3,4,7, 2,3,4,8, 2,3,5,7, 2,3,5,8, 2,3,6,7, 2,3,6,8, 4,2,5,6, 4,2,5,8, 4,2,5,9, 4,2,6,7, 4,2,6,8, 4,2,6,9, 2,4,7,8,
//         2,4,7,9, 2,4,8,9, 5,2,6,7, 5,2,6,8, 2,5,7,8, 2,5,7,9, 2,5,8,9, 2,6,7,8, 2,6,7,9, 2,6,8,9, 4,3,5,6, 3,4,5,7, 3,4,5,8, 3,4,5,9, 3,4,6,7, 3,4,6,9, 3,4,7,8,
//         4,3,7,9, 4,3,8,9, 5,3,6,7, 5,3,6,8, 3,5,7,8, 5,3,7,9, 5,3,8,9, 3,6,7,8, 6,3,7,9, 6,3,8,9, 4,5,6,7, 4,5,6,8, 4,5,6,9, 5,4,7,8, 5,4,7,9, 6,4,7,8, 4,6,8,9,
//         7,4,8,9, 5,6,7,9, 5,6,8,9, 7,5,8,9, 7,6,8,9,
//     };
//     int div_tetra[] = {
//     0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     15,51,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     4,52,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     26,51,87,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     1,53,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     18,51,88,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     7,52,65,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     34,51,87,112,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     5,16,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     15,63,89,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     4,66,91,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     26,62,66,91,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     10,21,68,93,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     18,63,88,89,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     7,65,66,91,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     34,62,66,91,112,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     2,17,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     15,55,90,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     8,30,58,92,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     26,55,87,90,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     1,60,94,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     18,54,60,94,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     7,57,60,94,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     34,54,60,94,95,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     12,16,31,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     15,75,89,118,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     12,29,31,91,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     26,75,87,89,118,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     12,22,23,94,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     18,75,88,89,118,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     7,78,84,91,121,126,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     34,75,87,89,112,118,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     3,6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     20,28,56,64,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     4,59,67,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     26,59,62,67,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     1,61,69,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     18,54,61,69,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     7,57,61,69,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     34,54,61,69,95,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     5,24,32,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     24,27,32,63,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     4,66,107,124,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     26,62,66,107,124,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     11,13,24,69,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     18,63,98,110,116,127,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     7,65,66,107,124,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     34,62,66,107,112,124,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     2,25,33,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     19,25,33,55,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     9,14,33,59,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     26,55,96,108,114,125,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     1,60,111,128,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     18,54,60,111,128,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     7,57,60,111,128,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     34,54,60,95,111,128,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     12,23,25,33,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     19,25,33,75,101,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     9,14,33,82,107,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     26,75,107,113,118,124,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     11,13,24,86,128,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     18,75,99,101,111,128,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     7,80,82,86,107,128,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     34,70,80,82,86,107,128,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/* 0.166667 */
//     };
// 
// //     const unsigned valid_tetra[] = {
// //         0,1,2,3, 0,1,2,6, 0,1,2,8, 0,1,2,9, 0,1,3,5, 0,1,3,7, 0,1,3,9, 0,1,5,6, 0,1,5,8,
// //         0,1,5,9, 0,1,6,7, 0,1,6,9, 0,1,7,8, 0,1,7,9, 0,1,8,9, 0,2,3,4, 0,2,3,7, 0,2,3,8,
// //         0,2,4,6, 0,2,4,8, 0,2,4,9, 0,2,6,7, 0,2,6,8, 0,2,7,8, 0,2,7,9, 0,2,8,9, 0,3,4,5,
// //         0,3,4,7, 0,3,4,9, 0,3,5,7, 0,3,5,8, 0,3,7,8, 0,3,7,9, 0,3,8,9, 0,4,5,6, 0,4,5,8,
// //         0,4,5,9, 0,4,6,7, 0,4,6,9, 0,4,7,8, 0,4,7,9, 0,4,8,9, 0,5,6,7, 0,5,6,8, 0,5,7,8,
// //         0,5,7,9, 0,5,8,9, 0,6,7,8, 0,6,7,9, 0,6,8,9, 0,7,8,9, 1,2,3,4, 1,2,3,5, 1,2,3,6,
// //         1,2,4,6, 1,2,4,8, 1,2,4,9, 1,2,5,6, 1,2,5,8, 1,2,5,9, 1,2,6,8, 1,2,6,9, 1,3,4,5,
// //         1,3,4,7, 1,3,4,9, 1,3,5,6, 1,3,5,7, 1,3,5,9, 1,3,6,7, 1,3,6,9, 1,4,5,6, 1,4,5,8,
// //         1,4,5,9, 1,4,6,7, 1,4,6,9, 1,4,7,8, 1,4,7,9, 1,4,8,9, 1,5,6,7, 1,5,6,8, 1,5,6,9,
// //         1,5,7,8, 1,5,7,9, 1,5,8,9, 1,6,7,8, 1,6,7,9, 1,6,8,9, 2,3,4,5, 2,3,4,6, 2,3,4,7,
// //         2,3,4,8, 2,3,5,7, 2,3,5,8, 2,3,6,7, 2,3,6,8, 2,4,5,6, 2,4,5,8, 2,4,5,9, 2,4,6,7,
// //         2,4,6,8, 2,4,6,9, 2,4,7,8, 2,4,7,9, 2,4,8,9, 2,5,6,7, 2,5,6,8, 2,5,7,8, 2,5,7,9,
// //         2,5,8,9, 2,6,7,8, 2,6,7,9, 2,6,8,9, 3,4,5,6, 3,4,5,7, 3,4,5,8, 3,4,5,9, 3,4,6,7,
// //         3,4,6,9, 3,4,7,8, 3,4,7,9, 3,4,8,9, 3,5,6,7, 3,5,6,8, 3,5,7,8, 3,5,7,9, 3,5,8,9,
// //         3,6,7,8, 3,6,7,9, 3,6,8,9, 4,5,6,7, 4,5,6,8, 4,5,6,9, 4,5,7,8, 4,5,7,9, 4,6,7,8,
// //         4,6,8,9, 4,7,8,9, 5,6,7,9, 5,6,8,9, 5,7,8,9, 6,7,8,9,
// //     };
// //     const unsigned div[] = {
// //         /*   0      1 */    0,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*   1      1 */   15,  51,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*   2      1 */    4,  52,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*   3      1 */   26,  51,  87,1000,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*   4      1 */    1,  53,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*   5      1 */   18,  51,  88,1000,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*   6      1 */    7,  52,  65,1000,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*   7      1 */   34,  51,  87, 112,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*   8      1 */    5,  16,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*   9      1 */   15,  63,  89,1000,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  10      1 */    4,  66,  91,1000,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  11      1 */   26,  62,  66,  91,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  12      1 */    1,  68,  93,1000,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  13      1 */   16,  37,  63, 116,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  14      1 */    5,  42,  91, 121,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  15      1 */   27,  42,  63,  91, 121,1000,1000,1000,1000,1000,1000,1000,
// //         /*  16      1 */    2,  17,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  17      1 */   15,  55,  90,1000,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  18      1 */    2,  30,  92,1000,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  19      1 */   17,  35,  55,  96,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  20      1 */    1,  60,  94,1000,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  21      1 */   18,  54,  60,  94,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  22      1 */    2,  43,  92, 122,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  23      1 */   19,  43,  55,  92, 122,1000,1000,1000,1000,1000,1000,1000,
// //         /*  24      1 */   12,  16,  31,1000,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  25      1 */   15,  75,  89, 118,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  26      1 */    4,  81,  91, 123,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  27      1 */   17,  35,  71,  81, 106,1000,1000,1000,1000,1000,1000,1000,
// //         /*  28      1 */    1,  84,  93, 126,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  29      1 */   16,  37,  73,  84, 126,1000,1000,1000,1000,1000,1000,1000,
// //         /*  30      1 */    7,  57,  84,  93, 126,1000,1000,1000,1000,1000,1000,1000,
// //         /*  31      1 */   19,  43,  75,  92, 101, 122,1000,1000,1000,1000,1000,1000,
// //         /*  32      1 */    3,   6,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  33      1 */    3,  28,  64,1000,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  34      1 */    4,  59,  67,1000,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  35      1 */    6,  36,  56,  97,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  36      1 */    1,  61,  69,1000,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  37      1 */    3,  38,  64, 117,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  38      1 */    7,  57,  61,  69,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  39      1 */    9,  38,  59,  64, 117,1000,1000,1000,1000,1000,1000,1000,
// //         /*  40      1 */    5,  24,  32,1000,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  41      1 */    6,  20,  76, 102,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  42      1 */    4,  66, 107, 124,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  43      1 */    6,  36,  72,  82, 107,1000,1000,1000,1000,1000,1000,1000,
// //         /*  44      1 */    1,  68, 110, 127,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  45      1 */   11,  20,  69,  76, 102,1000,1000,1000,1000,1000,1000,1000,
// //         /*  46      1 */    5,  42, 104, 110, 127,1000,1000,1000,1000,1000,1000,1000,
// //         /*  47      1 */    9,  38,  64,  82, 107, 117,1000,1000,1000,1000,1000,1000,
// //         /*  48      1 */    2,  25,  33,1000,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  49      1 */    3,  28,  77, 120,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  50      1 */    2,  30, 108, 125,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  51      1 */    9,  28,  59,  77, 120,1000,1000,1000,1000,1000,1000,1000,
// //         /*  52      1 */    1,  60, 111, 128,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  53      1 */    3,  38,  74,  86, 128,1000,1000,1000,1000,1000,1000,1000,
// //         /*  54      1 */    2,  43, 105, 111, 128,1000,1000,1000,1000,1000,1000,1000,
// //         /*  55      1 */    9,  38,  59,  74,  86, 128,1000,1000,1000,1000,1000,1000,
// //         /*  56      1 */   12,  23,  25,  33,1000,1000,1000,1000,1000,1000,1000,1000,
// //         /*  57      1 */   13,  24,  28,  77, 120,1000,1000,1000,1000,1000,1000,1000,
// //         /*  58      1 */    4,  81, 106, 108, 125,1000,1000,1000,1000,1000,1000,1000,
// //         /*  59      1 */    9,  28,  77,  82, 107, 120,1000,1000,1000,1000,1000,1000,
// //         /*  60      1 */    1,  84, 109, 111, 128,1000,1000,1000,1000,1000,1000,1000,
// //         /*  61      1 */   11,  20,  76,  86, 102, 128,1000,1000,1000,1000,1000,1000,
// //         /*  62      1 */    7,  57,  84, 109, 111, 128,1000,1000,1000,1000,1000,1000,
// //         /*  63      1 */    9,  38,  74,  82,  86, 107, 128,1000,1000,1000,1000,1000,
// //     };
//     unsigned index = bool(nodes[0])*1 + bool(nodes[1])*2 + bool(nodes[2])*4 + bool(nodes[3])*8 + 
//                      bool(nodes[4])*16 + bool(nodes[5])*32;
//     if ( index==0 )
//         return false;
//     if ( index==63 ) {
//         TNG *node_6 = m.add_node( center(e) );
//         TNG *nn[4];
//         nn[ 0 ] = e.node(0);
//         nn[ 1 ] = nodes[0];
//         nn[ 2 ] = nodes[1];
//         nn[ 3 ] = nodes[2];
//         permutation_if_jac_neg ( Tetra(), nn );
//         DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
//         nn[ 0 ] = e.node(1);
//         nn[ 1 ] = nodes[0];
//         nn[ 2 ] = nodes[3];
//         nn[ 3 ] = nodes[4];
//         permutation_if_jac_neg ( Tetra(), nn );
//         DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
//         nn[ 0 ] = e.node(2);
//         nn[ 1 ] = nodes[1];
//         nn[ 2 ] = nodes[3];
//         nn[ 3 ] = nodes[5];
//         permutation_if_jac_neg ( Tetra(), nn );
//         DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
//         nn[ 0 ] = e.node(3);
//         nn[ 1 ] = nodes[2];
//         nn[ 2 ] = nodes[4];
//         nn[ 3 ] = nodes[5];
//         permutation_if_jac_neg ( Tetra(), nn );
//         DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
//         
//         nn[ 0 ] = nodes[0];
//         nn[ 1 ] = nodes[1];
//         nn[ 2 ] = nodes[2];
//         nn[ 3 ] = node_6;
//         permutation_if_jac_neg ( Tetra(), nn );
//         DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
//         nn[ 0 ] = nodes[0];
//         nn[ 1 ] = nodes[3];
//         nn[ 2 ] = nodes[4];
//         nn[ 3 ] = node_6;
//         permutation_if_jac_neg ( Tetra(), nn );
//         DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
//         nn[ 0 ] = nodes[1];
//         nn[ 1 ] = nodes[3];
//         nn[ 2 ] = nodes[5];
//         nn[ 3 ] = node_6;
//         permutation_if_jac_neg ( Tetra(), nn );
//         DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
//         nn[ 0 ] = nodes[2];
//         nn[ 1 ] = nodes[4];
//         nn[ 2 ] = nodes[5];
//         nn[ 3 ] = node_6;
//         permutation_if_jac_neg ( Tetra(), nn );
//         DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
//         
//         nn[ 0 ] = nodes[0];
//         nn[ 1 ] = nodes[2];
//         nn[ 2 ] = nodes[4];
//         nn[ 3 ] = node_6;
//         permutation_if_jac_neg ( Tetra(), nn );
//         DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
//         nn[ 0 ] = nodes[1];
//         nn[ 1 ] = nodes[2];
//         nn[ 2 ] = nodes[5];
//         nn[ 3 ] = node_6;
//         permutation_if_jac_neg ( Tetra(), nn );
//         DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
//         nn[ 0 ] = nodes[3];
//         nn[ 1 ] = nodes[4];
//         nn[ 2 ] = nodes[5];
//         nn[ 3 ] = node_6;
//         permutation_if_jac_neg ( Tetra(), nn );
//         DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
//         nn[ 0 ] = nodes[0];
//         nn[ 1 ] = nodes[1];
//         nn[ 2 ] = nodes[3];
//         nn[ 3 ] = node_6;
//         permutation_if_jac_neg ( Tetra(), nn );
//         DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
//     }
//     else {
//         for(const int *a = div_tetra + index * 32; *a!=-1; ++a ) {
//             TNG *nn[4];
//             for(unsigned i=0;i<4;++i)
//                 if ( valid_tetra[*a * 4 + i] < 4 )
//                     nn[i] = e.node( valid_tetra[*a * 4 + i] );
//                 else
//                     nn[i] = nodes[ valid_tetra[*a * 4 + i] - 4 ];
//             permutation_if_jac_neg ( Tetra(), nn );
//             DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
//         }
//     }
//     return true;
// }

extern unsigned valid_tetra_face_center[];
extern int div_tetra_face_center[];

template<class TN,class TNG,class TD,unsigned NET,class TM>
bool divide_element_using_elem_children(Element<Tetra,TN,TNG,TD,NET> &e,TM &m,TNG **nnodes) {
    unsigned index = 15 + bool(nnodes[0])*16 + bool(nnodes[1])*32 + bool(nnodes[2])*64 + bool(nnodes[3])*128;
    for(const int *a = div_tetra_face_center + index * 32; *a>=0; ++a ) { // si le noeud appartient à l'ancien maillage
        TNG *nn[4];
        for(unsigned i=0;i<4;++i)
            if ( valid_tetra_face_center[*a * 4 + i] < 4 )
                nn[i] = e.node( valid_tetra_face_center[*a * 4 + i] );
            else
                nn[i] = nnodes[ valid_tetra_face_center[*a * 4 + i] - 4 ];
        DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );
    }
    return *( div_tetra_face_center + index * 32 ) >= 0;
}

/** new_nodes are independant nodes created to make a fine grid for integration. Used in TvrcFormulation
    \relates Tetra
    \todo actually, we divide element n times, with n = max(edge length) / max_dist. For flat elements, it's far from optimality
*/
template<class TN,class TNG,class TD,unsigned NET,class T>
bool subdivision_element(const Element<Tetra,TN,TNG,TD,NET> &e,Vec<TNG> &new_nodes,T max_dist) {
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

template<class TV,class T>
bool var_inter_is_inside( const Tetra &e, const TV &var_inter, T tol = 0 ) {
    return heaviside( var_inter[0] + tol ) * heaviside( var_inter[1] + tol ) * heaviside( var_inter[2] + tol ) * heaviside( 1 - var_inter[0] - var_inter[1] - var_inter[2] + tol );
}

/// >= 0 -> inside, < 0 -> outside
template<class T,class TV>
T var_inter_insideness( const Tetra &e, const TV &var_inter ) {
    return min( min( min( var_inter[0], var_inter[1] ), var_inter[2] ), 1 - var_inter[0] - var_inter[1] - var_inter[2] );
}

/*!
    objectif :
        la fonction renvoie vrai si pos est dans le tétraèdre et faux sinon.
        
    paramètre :
        Tetra : le type d'élément
        pos_nodes : le position des sommets dans l'espace
        pos : la position du point dans l'espace

*/
template<class PosNodes, class Pvec>
bool is_inside_linear( const Tetra &elem, const PosNodes &pos_nodes, const Pvec &pos ) {

    typedef typename Pvec::template SubType<0>::T T;

    #define COEF_A( i, j, k ) ( - pos_nodes[i][1] * pos_nodes[j][2] + pos_nodes[i][1] * pos_nodes[k][2] + pos_nodes[i][2] * pos_nodes[j][1] - pos_nodes[i][2] * pos_nodes[k][1] - pos_nodes[j][1] * pos_nodes[k][2] + pos_nodes[j][2] * pos_nodes[k][1] )
    
    #define COEF_B( i, j, k ) ( pos_nodes[i][0] * pos_nodes[j][2] - pos_nodes[i][0] * pos_nodes[k][2] - pos_nodes[i][2] * pos_nodes[j][0] + pos_nodes[i][2] * pos_nodes[k][0] + pos_nodes[j][0] * pos_nodes[k][2] - pos_nodes[j][2] * pos_nodes[k][0] )
    
    #define COEF_C( i, j, k ) ( - pos_nodes[i][0] * pos_nodes[j][1] + pos_nodes[i][0] * pos_nodes[k][1] + pos_nodes[i][1] * pos_nodes[j][0] - pos_nodes[i][1] * pos_nodes[k][0] - pos_nodes[j][0] * pos_nodes[k][1] + pos_nodes[j][1] * pos_nodes[k][0] )
    
    #define COEF_D( i, j, k ) ( pos_nodes[i][0] * pos_nodes[j][1] * pos_nodes[k][2] - pos_nodes[i][0] * pos_nodes[j][2] * pos_nodes[k][1] - pos_nodes[i][1] * pos_nodes[j][0] * pos_nodes[k][2] + pos_nodes[i][1] * pos_nodes[j][2] * pos_nodes[k][0] + pos_nodes[i][2] * pos_nodes[j][0] * pos_nodes[k][1] - pos_nodes[i][2] * pos_nodes[j][1] * pos_nodes[k][0] )

    /// plan 0 2 1
    T det1 = COEF_A( 0, 2, 1 ) * pos[ 0 ] + COEF_B( 0, 2, 1 ) * pos[ 1 ] + COEF_C( 0, 2, 1 ) * pos[ 2 ] + COEF_D( 0, 2, 1 );
    /// plan 0 1 3  
    T det2 = COEF_A( 0, 1, 3 ) * pos[ 0 ] + COEF_B( 0, 1, 3 ) * pos[ 1 ] + COEF_C( 0, 1, 3 ) * pos[ 2 ] + COEF_D( 0, 1, 3 );
    if ( ( det1 * det2 < 0 ) ) return false;
    /// plan 0 3 2
    T det3 = COEF_A( 0, 3, 2 ) * pos[ 0 ] + COEF_B( 0, 3, 2 ) * pos[ 1 ] + COEF_C( 0, 3, 2 ) * pos[ 2 ] + COEF_D( 0, 3, 2 );
    if ( ( det1 * det3 < 0 ) ) return false;
    /// plan 1 2 3
    T det4 = COEF_A( 1, 2, 3 ) * pos[ 0 ] + COEF_B( 1, 2, 3 ) * pos[ 1 ] + COEF_C( 1, 2, 3 ) * pos[ 2 ] + COEF_D( 1, 2, 3 ); 
    if ( ( det1 * det4 < 0 ) ) return false;

    #undef COEF_A
    #undef COEF_B
    #undef COEF_C
    #undef COEF_D
    
    return true;
}

inline unsigned vtk_num( StructForType<Tetra> ) { return 10; }

}

#include "element_Tetra.h"

#endif // LMTTETRA_H
