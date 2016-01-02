//
// C++ Interface: Hexa
//
// Description:
//
//
// Author: Hugo LECLERC <leclerc@lmt.ens-cachan.fr>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMTHEXA_H
#define LMTHEXA_H

#include "Quad.h"

namespace LMT {

// --------------------------------------------------------------------------------------------------------
/*!
    Pavé à 8 noeuds
    \verbatim
    .                        7----------6
    .                       /|         /|
    .                      / |        / |
    .                     /  |       /  |
    .                    4----------5   |
    .                    |   |      |   |
    .                    |   3------|---2
    .                    |  /       |  /
    .                    | /        | /
    .                    |/         |/
    .                    0----------1
    \relates Mesh
    \relates Element
    \keyword Maillage/Elément
    \friend samir.amrouche@lmt.ens-cachan.fr
    \friend hugo.leclerc@lmt.ens-cachan.fr
*/

struct Hexa {
    static const unsigned nb_var_inter = 3;
    static const unsigned nb_nodes = 8;
    static const char *name() { return "Hexa"; }
    static const char *avs_name() { return "hex"; }
    static const char *can_directly_be_represented_by() { return "Hexa"; }
};

// --------------------------------------------------------------------------------------------------------
template<> struct NbChildrenElement<Hexa,0> { static const unsigned res = 1; };
template<> struct NbChildrenElement<Hexa,1> { static const unsigned res = 6; };
template<> struct NbChildrenElement<Hexa,2> { static const unsigned res = 12; };
template<> struct NbChildrenElement<Hexa,3> { static const unsigned res = 8; };

template<unsigned n> struct TypeChildrenElement<Hexa,0,n> { typedef Hexa T; };
template<unsigned n> struct TypeChildrenElement<Hexa,1,n> { typedef Quad T; };
template<unsigned n> struct TypeChildrenElement<Hexa,2,n> { typedef Bar T; };
template<unsigned n> struct TypeChildrenElement<Hexa,3,n> { typedef NodalElement T; };

template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Hexa,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<0> nvi_to_subs) {
    het.add_element(e,ch,Hexa(),e.node(0),e.node(1),e.node(2),e.node(3),e.node(4),e.node(5),e.node(6),e.node(7));
}
template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Hexa,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<1> nvi_to_subs) {
    het.add_element(e,ch,Quad(),e.node(0),e.node(3),e.node(2),e.node(1));
    het.add_element(e,ch,Quad(),e.node(4),e.node(5),e.node(6),e.node(7));
    het.add_element(e,ch,Quad(),e.node(0),e.node(1),e.node(5),e.node(4));
    het.add_element(e,ch,Quad(),e.node(3),e.node(7),e.node(6),e.node(2));
    het.add_element(e,ch,Quad(),e.node(0),e.node(4),e.node(7),e.node(3));
    het.add_element(e,ch,Quad(),e.node(1),e.node(2),e.node(6),e.node(5));
}
template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Hexa,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<2> nvi_to_subs) {
    het.add_element(e,ch,Bar(),e.node(0),e.node(1));
    het.add_element(e,ch,Bar(),e.node(1),e.node(2));
    het.add_element(e,ch,Bar(),e.node(2),e.node(3));
    het.add_element(e,ch,Bar(),e.node(3),e.node(0));
    het.add_element(e,ch,Bar(),e.node(4),e.node(5));
    het.add_element(e,ch,Bar(),e.node(5),e.node(6));
    het.add_element(e,ch,Bar(),e.node(6),e.node(7));
    het.add_element(e,ch,Bar(),e.node(7),e.node(4));    
    het.add_element(e,ch,Bar(),e.node(0),e.node(4));
    het.add_element(e,ch,Bar(),e.node(1),e.node(5));
    het.add_element(e,ch,Bar(),e.node(2),e.node(6));
    het.add_element(e,ch,Bar(),e.node(3),e.node(7));
}
template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Hexa,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<3> nvi_to_subs) {
    het.add_element(e,ch,NodalElement(),e.node(0));
    het.add_element(e,ch,NodalElement(),e.node(1));
    het.add_element(e,ch,NodalElement(),e.node(2));
    het.add_element(e,ch,NodalElement(),e.node(3));
    het.add_element(e,ch,NodalElement(),e.node(4));
    het.add_element(e,ch,NodalElement(),e.node(5));
    het.add_element(e,ch,NodalElement(),e.node(6));
    het.add_element(e,ch,NodalElement(),e.node(7));
}

// --------------------------------------------------------------------------------------------------------
// template<class TN,class TNG,class TD,unsigned NET,class Pvec,class T>
// void update_circum_center(const Element<Hexa,TN,TNG,TD,NET> &e,Pvec &C,T &R) {
//     C = getCenterOfCircumSphere( e.node(0)->pos, e.node(1)->pos, e.node(2)->pos, e.node(3)->pos, e.node(4)->pos, e.node(5)->pos, e.node(6)->pos, e.node(7)->pos );
//     R = length( e.node(0)->pos - C );
// }

template<class TN,class TNG,class TD,unsigned NET,class TM,class T>
void update_edge_ratio(const Element<Hexa,TN,TNG,TD,NET> &e,TM &m,T &edge_ratio) {
    T edge_length_0 = (m.get_children_of( e, Number<1>() )[ 0 ])->measure_virtual();
    T edge_length_1 = (m.get_children_of( e, Number<1>() )[ 1 ])->measure_virtual();
    T edge_length_2 = (m.get_children_of( e, Number<1>() )[ 2 ])->measure_virtual();
    T edge_length_3 = (m.get_children_of( e, Number<1>() )[ 3 ])->measure_virtual();
    T edge_length_4 = (m.get_children_of( e, Number<1>() )[ 4 ])->measure_virtual();
    T edge_length_5 = (m.get_children_of( e, Number<1>() )[ 5 ])->measure_virtual();
	T edge_min_1 = min( edge_length_0, edge_length_1, edge_length_2, edge_length_3 );
	T edge_min_2 = min( edge_length_4, edge_length_5 );
	T edge_max_1 = max( edge_length_0, edge_length_1, edge_length_2, edge_length_3 );
	T edge_max_2 = max( edge_length_4, edge_length_5 );
    edge_ratio = min( edge_min_1, edge_min_2 ) / max( edge_max_1, edge_max_2 );
}

template<class TN,class TNG,class TD,unsigned NET>
typename TypePromote<Abs,typename TNG::T>::T measure( const Element<Hexa,TN,TNG,TD,NET> &elem ) {
typedef typename TNG::T T;
T V = 0.;
    T reg0=0.45412414523193150862*elem.pos(0)[2]; T reg1=0.045875854768068491385*elem.pos(1)[1]; T reg2=0.045875854768068491385*elem.pos(0)[1]; T reg3=0.072362291867717009992*elem.pos(1)[1]; T reg4=0.072362291867717009992*elem.pos(0)[1];
   T reg5=0.45412414523193150862*elem.pos(1)[2]; T reg6=0.39433756729740643276*elem.pos(1)[2]; T reg7=0.39433756729740643276*elem.pos(0)[2]; T reg8=0.072362291867717009992*elem.pos(1)[2]; T reg9=0.072362291867717009992*elem.pos(0)[2];
   T reg10=0.39433756729740643276*elem.pos(1)[1]; T reg11=0.39433756729740643276*elem.pos(0)[1]; T reg12=0.71631284272709585555*elem.pos(0)[2]; T reg13=0.71631284272709585555*elem.pos(1)[2]; T reg14=0.045875854768068491385*elem.pos(0)[2];
   T reg15=0.045875854768068491385*elem.pos(1)[2]; T reg16=0.71631284272709585555*elem.pos(0)[1]; T reg17=0.71631284272709585555*elem.pos(1)[1]; T reg18=0.45412414523193150862*elem.pos(0)[1]; T reg19=0.45412414523193150862*elem.pos(1)[1];
   T reg20=0.71631284272709585555*elem.pos(2)[1]; T reg21=0.19193544773676716169*elem.pos(1)[2]; T reg22=0.019389417668419972776*elem.pos(0)[2]; reg12=reg13-reg12; reg13=reg15+reg14;
   T reg23=reg1+reg2; reg11=reg10+reg11; reg10=0.045875854768068491385*elem.pos(2)[2]; T reg24=0.39433756729740643276*elem.pos(2)[1]; T reg25=0.45412414523193150862*elem.pos(2)[1];
   T reg26=0.19193544773676716169*elem.pos(1)[1]; T reg27=0.71631284272709585555*elem.pos(2)[2]; T reg28=0.045875854768068491385*elem.pos(2)[1]; reg9=reg8-reg9; reg8=0.39433756729740643276*elem.pos(2)[2];
   T reg29=0.072362291867717009992*elem.pos(2)[1]; T reg30=0.10566243270259356724*elem.pos(0)[2]; T reg31=reg0+reg5; T reg32=0.10566243270259356724*elem.pos(1)[2]; reg6=reg7+reg6;
   reg7=0.10566243270259356724*elem.pos(1)[1]; T reg33=0.45412414523193150862*elem.pos(2)[2]; T reg34=0.019389417668419972776*elem.pos(0)[1]; reg16=reg17-reg16; reg17=reg19+reg18;
   reg4=reg3-reg4; reg3=0.10566243270259356724*elem.pos(0)[1]; T reg35=0.072362291867717009992*elem.pos(2)[2]; reg16=reg29+reg16; reg29=0.71631284272709585555*elem.pos(3)[2];
   reg9=reg27+reg9; reg27=0.19193544773676716169*elem.pos(0)[2]; T reg36=0.072362291867717009992*elem.pos(3)[1]; T reg37=0.019389417668419972776*elem.pos(1)[1]; T reg38=0.072362291867717009992*elem.pos(0)[0];
   T reg39=0.19193544773676716169*elem.pos(0)[1]; T reg40=0.19193544773676716169*elem.pos(2)[1]; T reg41=0.072362291867717009992*elem.pos(3)[2]; reg12=reg35+reg12; reg35=0.39433756729740643276*elem.pos(3)[1];
   T reg42=0.072362291867717009992*elem.pos(1)[0]; T reg43=0.45412414523193150862*elem.pos(3)[2]; reg11=reg24-reg11; reg13=reg33+reg13; reg24=0.19193544773676716169*elem.pos(2)[2];
   reg17=reg28+reg17; reg22=reg21+reg22; reg21=0.045875854768068491385*elem.pos(3)[2]; T reg44=0.71631284272709585555*elem.pos(3)[1]; reg20=reg4+reg20;
   reg30=reg32-reg30; reg4=0.10566243270259356724*elem.pos(2)[2]; reg34=reg26+reg34; reg5=reg14+reg5; reg31=reg10+reg31;
   reg14=0.39433756729740643276*elem.pos(3)[2]; reg23=reg25+reg23; reg26=0.71631284272709585555*elem.pos(0)[0]; reg32=0.71631284272709585555*elem.pos(1)[0]; T reg45=0.10566243270259356724*elem.pos(2)[1];
   T reg46=0.45412414523193150862*elem.pos(3)[1]; T reg47=0.019389417668419972776*elem.pos(1)[2]; T reg48=0.045875854768068491385*elem.pos(3)[1]; T reg49=0.39433756729740643276*elem.pos(1)[0]; reg6=reg8-reg6;
   reg19=reg2+reg19; reg2=0.39433756729740643276*elem.pos(0)[0]; reg3=reg7-reg3; reg2=reg49+reg2; reg7=0.045875854768068491385*elem.pos(4)[2];
   reg8=0.019389417668419972776*elem.pos(3)[1]; reg39=reg37+reg39; reg27=reg47+reg27; reg37=0.39433756729740643276*elem.pos(2)[0]; reg47=0.10566243270259356724*elem.pos(3)[1];
   reg49=0.45412414523193150862*elem.pos(1)[0]; T reg50=0.45412414523193150862*elem.pos(0)[0]; T reg51=0.019389417668419972776*elem.pos(2)[1]; reg34=reg40-reg34; reg23=reg46+reg23;
   reg3=reg45+reg3; reg40=0.019389417668419972776*elem.pos(2)[2]; reg13=reg43+reg13; reg45=0.19193544773676716169*elem.pos(1)[0]; T reg52=0.019389417668419972776*elem.pos(0)[0];
   T reg53=0.045875854768068491385*elem.pos(4)[1]; T reg54=0.019389417668419972776*elem.pos(4)[2]; reg29=reg9-reg29; reg9=0.10566243270259356724*elem.pos(3)[2]; reg19=reg25+reg19;
   reg25=0.10566243270259356724*elem.pos(1)[0]; T reg55=0.10566243270259356724*elem.pos(0)[0]; T reg56=0.072362291867717009992*elem.pos(2)[0]; T reg57=0.045875854768068491385*elem.pos(1)[0]; T reg58=0.045875854768068491385*elem.pos(0)[0];
   reg26=reg32-reg26; reg4=reg30+reg4; reg31=reg21+reg31; reg30=0.10566243270259356724*elem.pos(4)[2]; reg14=reg6+reg14;
   reg6=0.019389417668419972776*elem.pos(4)[1]; reg5=reg33+reg5; reg44=reg20-reg44; reg20=0.45412414523193150862*elem.pos(4)[2]; reg36=reg16-reg36;
   reg16=0.19193544773676716169*elem.pos(4)[1]; reg38=reg42-reg38; reg17=reg48+reg17; reg32=0.71631284272709585555*elem.pos(2)[0]; reg33=0.019389417668419972776*elem.pos(3)[2];
   reg0=reg15+reg0; reg15=0.45412414523193150862*elem.pos(4)[1]; reg42=0.10566243270259356724*elem.pos(4)[1]; reg11=reg35+reg11; reg22=reg24-reg22;
   reg18=reg1+reg18; reg41=reg12-reg41; reg1=0.19193544773676716169*elem.pos(4)[2]; reg31=reg20-reg31; reg12=0.45412414523193150862*elem.pos(5)[1];
   reg24=0.19193544773676716169*elem.pos(3)[2]; reg17=reg15-reg17; reg35=0.072362291867717009992*elem.pos(4)[1]; T reg59=0.19193544773676716169*elem.pos(2)[0]; reg16=reg36-reg16;
   reg36=0.19193544773676716169*elem.pos(5)[2]; T reg60=0.19193544773676716169*elem.pos(5)[1]; reg1=reg41-reg1; reg52=reg45+reg52; reg19=reg48+reg19;
   reg41=0.10566243270259356724*elem.pos(2)[0]; reg26=reg56+reg26; reg34=reg8+reg34; reg8=0.072362291867717009992*elem.pos(3)[0]; reg45=reg49+reg50;
   reg55=reg25-reg55; reg5=reg21+reg5; reg21=0.045875854768068491385*elem.pos(2)[0]; reg25=0.45412414523193150862*elem.pos(5)[2]; reg48=0.045875854768068491385*elem.pos(5)[2];
   reg39=reg51-reg39; reg51=0.71631284272709585555*elem.pos(3)[0]; reg38=reg32+reg38; reg0=reg10+reg0; reg10=0.10566243270259356724*elem.pos(5)[1];
   reg42=reg11-reg42; reg22=reg33+reg22; reg11=0.072362291867717009992*elem.pos(4)[2]; reg54=reg29-reg54; reg29=0.019389417668419972776*elem.pos(5)[2];
   reg18=reg28+reg18; reg28=0.45412414523193150862*elem.pos(2)[0]; reg32=reg58+reg57; reg33=0.019389417668419972776*elem.pos(1)[0]; reg56=0.19193544773676716169*elem.pos(0)[0];
   T reg61=0.019389417668419972776*elem.pos(5)[1]; reg6=reg44-reg6; reg30=reg14-reg30; reg14=0.10566243270259356724*elem.pos(5)[2]; reg9=reg4-reg9;
   reg4=0.39433756729740643276*elem.pos(4)[2]; reg44=0.39433756729740643276*elem.pos(4)[1]; T reg62=0.19193544773676716169*elem.pos(3)[1]; reg47=reg3-reg47; reg3=0.39433756729740643276*elem.pos(3)[0];
   reg23=reg53-reg23; T reg63=0.045875854768068491385*elem.pos(5)[1]; reg2=reg37-reg2; reg13=reg7-reg13; reg27=reg40-reg27;
   reg37=0.45412414523193150862*elem.pos(3)[0]; reg40=0.045875854768068491385*elem.pos(3)[0]; reg32=reg32+reg28; reg8=reg26-reg8; reg26=0.19193544773676716169*elem.pos(4)[0];
   T reg64=0.39433756729740643276*elem.pos(5)[2]; reg5=reg7-reg5; reg7=0.019389417668419972776*elem.pos(2)[0]; reg27=reg24+reg27; reg14=reg30-reg14;
   reg55=reg41+reg55; reg24=0.045875854768068491385*elem.pos(6)[2]; reg30=0.71631284272709585555*elem.pos(4)[2]; reg41=0.19193544773676716169*elem.pos(6)[1]; T reg65=0.39433756729740643276*elem.pos(5)[1];
   reg61=reg6+reg61; reg6=0.10566243270259356724*elem.pos(4)[0]; reg56=reg33+reg56; reg33=0.10566243270259356724*elem.pos(6)[2]; reg4=reg9-reg4;
   reg31=reg25+reg31; reg9=0.10566243270259356724*elem.pos(3)[0]; reg17=reg12+reg17; T reg66=0.045875854768068491385*elem.pos(6)[1]; reg2=reg3+reg2;
   reg3=0.019389417668419972776*elem.pos(6)[1]; T reg67=0.45412414523193150862*elem.pos(6)[1]; reg51=reg38-reg51; reg38=0.019389417668419972776*elem.pos(4)[0]; reg16=reg60+reg16;
   reg60=0.71631284272709585555*elem.pos(4)[1]; reg49=reg58+reg49; reg35=reg34-reg35; reg19=reg53-reg19; reg34=0.019389417668419972776*elem.pos(6)[2];
   reg0=reg43+reg0; reg10=reg42-reg10; reg39=reg62+reg39; reg52=reg59-reg52; reg42=0.10566243270259356724*elem.pos(6)[1];
   reg23=reg63+reg23; reg45=reg21+reg45; reg54=reg29+reg54; reg13=reg48+reg13; reg1=reg36+reg1;
   reg29=0.71631284272709585555*elem.pos(5)[1]; reg36=0.019389417668419972776*elem.pos(3)[0]; reg43=0.19193544773676716169*elem.pos(6)[2]; reg53=0.45412414523193150862*elem.pos(6)[2]; reg44=reg47-reg44;
   reg18=reg46+reg18; reg11=reg22-reg11; reg22=0.71631284272709585555*elem.pos(5)[2]; reg46=0.71631284272709585555*elem.pos(6)[1]; reg64=reg4+reg64;
   reg4=0.71631284272709585555*elem.pos(6)[2]; reg17=reg66+reg17; reg9=reg55-reg9; reg49=reg28+reg49; reg28=0.39433756729740643276*elem.pos(6)[1];
   reg44=reg65+reg44; reg29=reg35-reg29; reg35=0.39433756729740643276*elem.pos(6)[2]; reg22=reg11-reg22; reg50=reg57+reg50;
   reg41=reg61+reg41; reg11=0.19193544773676716169*elem.pos(3)[0]; reg47=0.10566243270259356724*elem.pos(5)[0]; reg6=reg2-reg6; reg30=reg27-reg30;
   reg2=0.072362291867717009992*elem.pos(5)[2]; reg32=reg32+reg37; reg23=reg67+reg23; reg27=0.45412414523193150862*elem.pos(7)[1]; reg13=reg53+reg13;
   reg55=0.45412414523193150862*elem.pos(7)[2]; reg38=reg51-reg38; reg51=0.072362291867717009992*elem.pos(5)[1]; reg60=reg39-reg60; reg39=0.019389417668419972776*elem.pos(5)[0];
   reg0=reg20-reg0; reg10=reg42+reg10; reg20=0.10566243270259356724*elem.pos(7)[1]; reg42=0.19193544773676716169*elem.pos(7)[2]; reg54=reg43+reg54;
   reg18=reg15-reg18; reg15=0.045875854768068491385*elem.pos(4)[0]; reg43=0.045875854768068491385*elem.pos(7)[1]; reg57=0.39433756729740643276*elem.pos(4)[0]; reg31=reg24+reg31;
   reg58=0.045875854768068491385*elem.pos(7)[2]; reg26=reg8-reg26; reg8=0.19193544773676716169*elem.pos(5)[0]; reg5=reg25+reg5; reg33=reg14+reg33;
   reg14=0.019389417668419972776*elem.pos(7)[2]; reg1=reg34+reg1; reg19=reg12+reg19; reg12=0.019389417668419972776*elem.pos(7)[1]; reg16=reg3+reg16;
   reg56=reg7-reg56; reg52=reg36+reg52; reg45=reg40+reg45; reg3=0.45412414523193150862*elem.pos(4)[0]; reg7=0.072362291867717009992*elem.pos(4)[0];
   reg25=0.10566243270259356724*elem.pos(7)[2]; reg34=0.19193544773676716169*elem.pos(7)[1]; reg64=reg35+reg64; reg56=reg11+reg56; reg11=0.39433756729740643276*elem.pos(7)[2];
   reg35=0.71631284272709585555*elem.pos(4)[0]; reg36=0.072362291867717009992*elem.pos(7)[1]; reg18=reg63+reg18; reg0=reg48+reg0; reg48=0.072362291867717009992*elem.pos(6)[1];
   reg2=reg30-reg2; reg30=0.072362291867717009992*elem.pos(6)[2]; reg50=reg21+reg50; reg21=0.71631284272709585555*elem.pos(5)[0]; reg7=reg52-reg7;
   reg19=reg67+reg19; reg29=reg46+reg29; reg46=0.39433756729740643276*elem.pos(5)[0]; reg5=reg53+reg5; reg57=reg9-reg57;
   reg31=reg58+reg31; reg25=reg33+reg25; reg17=reg43+reg17; reg47=reg6-reg47; reg13=reg55+reg13;
   reg6=0.10566243270259356724*elem.pos(6)[0]; reg34=reg41-reg34; reg23=reg27+reg23; reg45=reg3-reg45; reg38=reg39+reg38;
   reg51=reg60-reg51; reg9=0.45412414523193150862*elem.pos(5)[0]; reg33=0.19193544773676716169*elem.pos(6)[0]; reg32=reg15-reg32; reg14=reg1-reg14;
   reg22=reg4+reg22; reg1=0.072362291867717009992*elem.pos(7)[2]; reg4=0.019389417668419972776*elem.pos(6)[0]; reg39=0.39433756729740643276*elem.pos(7)[1]; reg44=reg28+reg44;
   reg42=reg54-reg42; reg26=reg8+reg26; reg8=0.045875854768068491385*elem.pos(5)[0]; reg49=reg40+reg49; reg10=reg20+reg10;
   reg12=reg16-reg12; reg19=reg43+reg19; reg16=0.10566243270259356724*elem.pos(7)[0]; reg2=reg30+reg2; reg48=reg51+reg48;
   reg20=0.45412414523193150862*elem.pos(6)[0]; reg28=0.71631284272709585555*elem.pos(6)[0]; reg47=reg6+reg47; reg6=reg34*reg13; reg45=reg9+reg45;
   reg30=0.045875854768068491385*elem.pos(6)[0]; reg40=reg42*reg23; reg21=reg7-reg21; reg18=reg66+reg18; reg7=0.71631284272709585555*elem.pos(7)[2];
   reg50=reg37+reg50; reg32=reg32+reg8; reg0=reg24+reg0; reg49=reg15-reg49; reg39=reg44-reg39;
   reg29=reg36+reg29; reg15=0.072362291867717009992*elem.pos(5)[0]; reg24=0.39433756729740643276*elem.pos(6)[0]; reg38=reg33+reg38; reg33=0.19193544773676716169*elem.pos(7)[0];
   reg36=reg14*reg17; reg37=reg12*reg31; reg11=reg64-reg11; reg17=reg25*reg17; reg31=reg10*reg31;
   reg57=reg46+reg57; reg13=reg10*reg13; reg23=reg25*reg23; reg22=reg1+reg22; reg1=0.019389417668419972776*elem.pos(7)[0];
   reg26=reg4+reg26; reg35=reg56-reg35; reg5=reg58+reg5; reg4=0.71631284272709585555*elem.pos(7)[1]; reg50=reg3-reg50;
   reg18=reg27+reg18; reg34=reg25*reg34; reg0=reg55+reg0; reg40=reg6-reg40; reg42=reg42*reg10;
   reg3=0.45412414523193150862*elem.pos(7)[0]; reg47=reg16+reg47; reg33=reg38-reg33; reg23=reg13-reg23; reg2=reg7+reg2;
   reg49=reg9+reg49; reg15=reg35-reg15; reg36=reg37-reg36; reg17=reg31-reg17; reg57=reg24+reg57;
   reg6=0.39433756729740643276*elem.pos(7)[0]; reg1=reg26-reg1; reg7=reg29*reg5; reg14=reg10*reg14; reg12=reg25*reg12;
   reg9=reg22*reg19; reg10=0.072362291867717009992*elem.pos(7)[0]; reg4=reg48+reg4; reg45=reg30+reg45; reg13=0.072362291867717009992*elem.pos(6)[0];
   reg32=reg32+reg20; reg19=reg11*reg19; reg5=reg39*reg5; reg16=0.045875854768068491385*elem.pos(7)[0]; reg21=reg28+reg21;
   reg22=reg39*reg22; reg24=reg4*reg0; reg49=reg20+reg49; reg20=reg11*reg18; reg0=reg39*reg0;
   reg42=reg34-reg42; reg15=reg13+reg15; reg29=reg11*reg29; reg13=0.71631284272709585555*elem.pos(7)[0]; reg36=reg47*reg36;
   reg17=reg1*reg17; reg45=reg16+reg45; reg32=reg32+reg3; reg50=reg8+reg50; reg6=reg57-reg6;
   reg18=reg2*reg18; reg19=reg5-reg19; reg14=reg12-reg14; reg40=reg47*reg40; reg23=reg33*reg23;
   reg21=reg10+reg21; reg9=reg7-reg9; reg18=reg24-reg18; reg15=reg13+reg15; reg20=reg0-reg20;
   reg49=reg16+reg49; reg36=reg17-reg36; reg29=reg22-reg29; reg14=reg45*reg14; reg9=reg6*reg9;
   reg11=reg4*reg11; reg19=reg21*reg19; reg42=reg32*reg42; reg2=reg39*reg2; reg40=reg23-reg40;
   reg50=reg30+reg50; reg36=reg14+reg36; reg29=reg49*reg29; reg40=reg42+reg40; reg50=reg3+reg50;
   reg11=reg2-reg11; reg19=reg9-reg19; reg20=reg15*reg20; reg18=reg6*reg18; reg50=reg11*reg50;
   reg19=reg29+reg19; reg20=reg18-reg20; reg40=0.25*reg40; reg36=0.25*reg36; reg20=reg50+reg20;
   reg19=0.25*reg19; reg36=reg40+reg36; reg20=0.25*reg20; reg19=reg36+reg19; V+=reg19+reg20;

return abs(V);
}

template<class TN,class TNG,class TD,unsigned NET,class TM>
bool divide_element(Element<Hexa,TN,TNG,TD,NET> &e,TM &m,TNG **nnodes) {
   std::cout << "divide_element not implemented for Hexa" << std::endl;
   assert(0);
   return false;
}
//template<class TN,class TNG,class TD,unsigned NET,class TM>
//bool divide_element(Element<Hexa,TN,TNG,TD,NET> &e,TM &m,TNG **nodes) {
//    const unsigned valid_tetra[] = {
//        0,1,2,3, 0,1,2,6, 0,1,2,8, 0,1,2,9, 0,1,3,5, 0,1,3,7, 0,1,3,9, 0,1,5,6, 0,1,5,8,
//        0,1,5,9, 0,1,6,7, 0,1,6,9, 0,1,7,8, 0,1,7,9, 0,1,8,9, 0,2,3,4, 0,2,3,7, 0,2,3,8,
//        0,2,4,6, 0,2,4,8, 0,2,4,9, 0,2,6,7, 0,2,6,8, 0,2,7,8, 0,2,7,9, 0,2,8,9, 0,3,4,5,
//        0,3,4,7, 0,3,4,9, 0,3,5,7, 0,3,5,8, 0,3,7,8, 0,3,7,9, 0,3,8,9, 0,4,5,6, 0,4,5,8,
//        0,4,5,9, 0,4,6,7, 0,4,6,9, 0,4,7,8, 0,4,7,9, 0,4,8,9, 0,5,6,7, 0,5,6,8, 0,5,7,8,
//        0,5,7,9, 0,5,8,9, 0,6,7,8, 0,6,7,9, 0,6,8,9, 0,7,8,9, 1,2,3,4, 1,2,3,5, 1,2,3,6,
//        1,2,4,6, 1,2,4,8, 1,2,4,9, 1,2,5,6, 1,2,5,8, 1,2,5,9, 1,2,6,8, 1,2,6,9, 1,3,4,5,
//        1,3,4,7, 1,3,4,9, 1,3,5,6, 1,3,5,7, 1,3,5,9, 1,3,6,7, 1,3,6,9, 1,4,5,6, 1,4,5,8,
//        1,4,5,9, 1,4,6,7, 1,4,6,9, 1,4,7,8, 1,4,7,9, 1,4,8,9, 1,5,6,7, 1,5,6,8, 1,5,6,9,
//        1,5,7,8, 1,5,7,9, 1,5,8,9, 1,6,7,8, 1,6,7,9, 1,6,8,9, 2,3,4,5, 2,3,4,6, 2,3,4,7,
//        2,3,4,8, 2,3,5,7, 2,3,5,8, 2,3,6,7, 2,3,6,8, 2,4,5,6, 2,4,5,8, 2,4,5,9, 2,4,6,7,
//        2,4,6,8, 2,4,6,9, 2,4,7,8, 2,4,7,9, 2,4,8,9, 2,5,6,7, 2,5,6,8, 2,5,7,8, 2,5,7,9,
//        2,5,8,9, 2,6,7,8, 2,6,7,9, 2,6,8,9, 3,4,5,6, 3,4,5,7, 3,4,5,8, 3,4,5,9, 3,4,6,7,
//        3,4,6,9, 3,4,7,8, 3,4,7,9, 3,4,8,9, 3,5,6,7, 3,5,6,8, 3,5,7,8, 3,5,7,9, 3,5,8,9,
//        3,6,7,8, 3,6,7,9, 3,6,8,9, 4,5,6,7, 4,5,6,8, 4,5,6,9, 4,5,7,8, 4,5,7,9, 4,6,7,8,
//        4,6,8,9, 4,7,8,9, 5,6,7,9, 5,6,8,9, 5,7,8,9, 6,7,8,9,
//    };
//    const unsigned div[] = {
//        /*   0      1 */    0,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*   1      1 */   15,  51,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*   2      1 */    4,  52,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*   3      1 */   26,  51,  87,1000,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*   4      1 */    1,  53,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*   5      1 */   18,  51,  88,1000,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*   6      1 */    7,  52,  65,1000,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*   7      1 */   34,  51,  87, 112,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*   8      1 */    5,  16,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*   9      1 */   15,  63,  89,1000,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  10      1 */    4,  66,  91,1000,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  11      1 */   26,  62,  66,  91,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  12      1 */    1,  68,  93,1000,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  13      1 */   16,  37,  63, 116,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  14      1 */    5,  42,  91, 121,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  15      1 */   27,  42,  63,  91, 121,1000,1000,1000,1000,1000,1000,1000,
//        /*  16      1 */    2,  17,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  17      1 */   15,  55,  90,1000,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  18      1 */    2,  30,  92,1000,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  19      1 */   17,  35,  55,  96,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  20      1 */    1,  60,  94,1000,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  21      1 */   18,  54,  60,  94,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  22      1 */    2,  43,  92, 122,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  23      1 */   19,  43,  55,  92, 122,1000,1000,1000,1000,1000,1000,1000,
//        /*  24      1 */   12,  16,  31,1000,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  25      1 */   15,  75,  89, 118,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  26      1 */    4,  81,  91, 123,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  27      1 */   17,  35,  71,  81, 106,1000,1000,1000,1000,1000,1000,1000,
//        /*  28      1 */    1,  84,  93, 126,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  29      1 */   16,  37,  73,  84, 126,1000,1000,1000,1000,1000,1000,1000,
//        /*  30      1 */    7,  57,  84,  93, 126,1000,1000,1000,1000,1000,1000,1000,
//        /*  31      1 */   19,  43,  75,  92, 101, 122,1000,1000,1000,1000,1000,1000,
//        /*  32      1 */    3,   6,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  33      1 */    3,  28,  64,1000,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  34      1 */    4,  59,  67,1000,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  35      1 */    6,  36,  56,  97,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  36      1 */    1,  61,  69,1000,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  37      1 */    3,  38,  64, 117,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  38      1 */    7,  57,  61,  69,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  39      1 */    9,  38,  59,  64, 117,1000,1000,1000,1000,1000,1000,1000,
//        /*  40      1 */    5,  24,  32,1000,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  41      1 */    6,  20,  76, 102,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  42      1 */    4,  66, 107, 124,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  43      1 */    6,  36,  72,  82, 107,1000,1000,1000,1000,1000,1000,1000,
//        /*  44      1 */    1,  68, 110, 127,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  45      1 */   11,  20,  69,  76, 102,1000,1000,1000,1000,1000,1000,1000,
//        /*  46      1 */    5,  42, 104, 110, 127,1000,1000,1000,1000,1000,1000,1000,
//        /*  47      1 */    9,  38,  64,  82, 107, 117,1000,1000,1000,1000,1000,1000,
//        /*  48      1 */    2,  25,  33,1000,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  49      1 */    3,  28,  77, 120,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  50      1 */    2,  30, 108, 125,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  51      1 */    9,  28,  59,  77, 120,1000,1000,1000,1000,1000,1000,1000,
//        /*  52      1 */    1,  60, 111, 128,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  53      1 */    3,  38,  74,  86, 128,1000,1000,1000,1000,1000,1000,1000,
//        /*  54      1 */    2,  43, 105, 111, 128,1000,1000,1000,1000,1000,1000,1000,
//        /*  55      1 */    9,  38,  59,  74,  86, 128,1000,1000,1000,1000,1000,1000,
//        /*  56      1 */   12,  23,  25,  33,1000,1000,1000,1000,1000,1000,1000,1000,
//        /*  57      1 */   13,  24,  28,  77, 120,1000,1000,1000,1000,1000,1000,1000,
//        /*  58      1 */    4,  81, 106, 108, 125,1000,1000,1000,1000,1000,1000,1000,
//        /*  59      1 */    9,  28,  77,  82, 107, 120,1000,1000,1000,1000,1000,1000,
//        /*  60      1 */    1,  84, 109, 111, 128,1000,1000,1000,1000,1000,1000,1000,
//        /*  61      1 */   11,  20,  76,  86, 102, 128,1000,1000,1000,1000,1000,1000,
//        /*  62      1 */    7,  57,  84, 109, 111, 128,1000,1000,1000,1000,1000,1000,
//        /*  63      1 */    9,  38,  74,  82,  86, 107, 128,1000,1000,1000,1000,1000,
//    };
//    unsigned index = bool(nodes[0])*1 + bool(nodes[1])*2 + bool(nodes[2])*4 + bool(nodes[3])*8 + 
//                     bool(nodes[4])*16 + bool(nodes[5])*32;
//    if ( index==0 )
//        return true;
//    if ( index==63 ) {
//        TNG *node_6 = m.add_node( center(e) );
//        m.add_element( Tetra(), TN(), e.node(0), nodes[0], nodes[1], nodes[2] );
//        m.add_element( Tetra(), TN(), e.node(1), nodes[0], nodes[3], nodes[4] );
//        m.add_element( Tetra(), TN(), e.node(2), nodes[1], nodes[3], nodes[5] );
//        m.add_element( Tetra(), TN(), e.node(3), nodes[2], nodes[4], nodes[5] );
//        
//        m.add_element( Tetra(), TN(), nodes[0], nodes[1], nodes[2], node_6 );
//        m.add_element( Tetra(), TN(), nodes[0], nodes[3], nodes[4], node_6 );
//        m.add_element( Tetra(), TN(), nodes[1], nodes[3], nodes[5], node_6 );
//        m.add_element( Tetra(), TN(), nodes[2], nodes[4], nodes[5], node_6 );
//        
//        m.add_element( Tetra(), TN(), nodes[0], nodes[2], nodes[4], node_6 );
//        m.add_element( Tetra(), TN(), nodes[1], nodes[2], nodes[5], node_6 );
//        m.add_element( Tetra(), TN(), nodes[3], nodes[4], nodes[5], node_6 );
//        m.add_element( Tetra(), TN(), nodes[0], nodes[1], nodes[3], node_6 );
//    }
//    else {
//        for(const unsigned *a = div + index * 12; *a!=1000; ++a ) {
//            TNG *nn[4];
//            for(unsigned i=0;i<4;++i)
//                if ( valid_tetra[*a * 4 + i] < 4 )
//                    nn[i] = e.node( valid_tetra[*a * 4 + i] );
//                else
//                    nn[i] = nodes[ valid_tetra[*a * 4 + i] - 4 ];
//            m.add_element( Tetra(), TN(), nn );
//        }
//    }
//    std::cout << "Surdiscretisation des Hexa non implentee" << std::endl;
//    return false;
//   
//}

template<class TN,class TNG,class TD,unsigned NET,class TM>
bool divide_element_using_elem_children(Element<Hexa,TN,TNG,TD,NET> &e,TM &m,TNG **nnodes) {
    return divide_element(e,m,nnodes);
}

/** new_nodes are independant nodes created to make a fine grid for integration. Used in TvrcFormulation
//    \relates Hexa
//    \todo actually, we divide element n times, with n = max(edge length) / max_dist. For flat elements, it's far from optimallity
//*/
//template<class TN,class TNG,class TD,unsigned NET,class T>
//bool subdivision_element(const Element<Tetra,TN,TNG,TD,NET> &e,Vec<TNG> &new_nodes,T max_dist) {
//    T max_l = max(
//            max( length( e.node(0)->pos - e.node(1)->pos ), length( e.node(0)->pos - e.node(2)->pos ), length( e.node(0)->pos - e.node(3)->pos ) ),
//            max( length( e.node(1)->pos - e.node(2)->pos ), length( e.node(1)->pos - e.node(3)->pos ) ),
//            length( e.node(2)->pos - e.node(3)->pos )
//    );
//    if ( max_l <= max_dist )
//        return false;
//    unsigned n = (unsigned)(max_l / max_dist);
//    for(unsigned i=0;i<n;++i) {
//        for(unsigned j=0;j<=i;++j) {
//            for(unsigned k=0;k<=j;++k) {
//                TNG *nn = new_nodes.new_elem();
//                T f1 = (i+1.0)/(n+1.0);
//                T f2 = (j+1.0)/(n+1.0);
//                T f3 = (k+1.0)/(n+1.0);
//                T f4 = 1.0-f1-f2-f3;
//                std::pair<T,const TNG *> pond_list[] = {
//                    std::pair<T,const TNG *>( f1, e.node(0) ),
//                    std::pair<T,const TNG *>( f2, e.node(1) ),
//                    std::pair<T,const TNG *>( f3, e.node(2) ),
//                    std::pair<T,const TNG *>( f4, e.node(3) )
//                };
//                DM::get_ponderation( pond_list, 4, *nn );
//            }
//        }
//    }
//    return true;
//}

template<class TV,class T>
bool var_inter_is_inside( const Hexa &, const TV &var_inter, T tol = 0 ) {
    return heaviside( var_inter[0] + tol ) * heaviside( 1 - var_inter[0] + tol ) * 
           heaviside( var_inter[1] + tol ) * heaviside( 1 - var_inter[1] + tol ) *
           heaviside( var_inter[2] + tol ) * heaviside( 1 - var_inter[2] + tol );
}

/// >= 0 -> inside, < 0 -> outside
template<class T,class TV>
T var_inter_insideness( const Hexa &e, const TV &var_inter ) {
    return min( min( min( min( var_inter[0], var_inter[1] ), 1 - var_inter[0] ), 1 - var_inter[1] ), 1 - var_inter[2] );
}

inline unsigned vtk_num( StructForType<Hexa> ) { return 12; }

}

#include "element_Hexa.h"

#endif // LMTHEXA_H
