#ifndef LMT_TETRA_10
#define LMT_TETRA_10
#include "node.h"
namespace LMT {
inline const double *gauss_point_for_order(unsigned order, const Tetra_10 &elem) { /// order -> degre du polynome a integrer exactement
    static const unsigned offset[] = { 0, 5, 10, 27, 48, 109, 170, 231, 292, 353, }; // fonction de lordre du poly
    static const double values[] = {
        0.16666666666666666667,0.25,0.25,0.25,
        0.0,
        0.16666666666666666667,0.25,0.25,0.25,
        0.0,
        0.041666666666666666667,0.1381966011250105,0.1381966011250105,0.1381966011250105,
        0.041666666666666666667,0.5854101966249685,0.1381966011250105,0.1381966011250105,
        0.041666666666666666667,0.1381966011250105,0.5854101966249685,0.1381966011250105,
        0.041666666666666666667,0.1381966011250105,0.1381966011250105,0.5854101966249685,
        0.0,
        -0.13333333333333333333,0.25,0.25,0.25,
        0.075,0.16666666666666666667,0.16666666666666666667,0.16666666666666666667,
        0.075,0.5,0.16666666666666666667,0.16666666666666666667,
        0.075,0.16666666666666666667,0.5,0.16666666666666666667,
        0.075,0.16666666666666666667,0.16666666666666666667,0.5,
        0.0,
        0.044444444444444444444,0.25,0.25,0.25,
        -0.09642857142857143,0.16666666666666666667,0.16666666666666666667,0.16666666666666666667,
        -0.09642857142857143,0.5,0.16666666666666666667,0.16666666666666666667,
        -0.09642857142857143,0.16666666666666666667,0.5,0.16666666666666666667,
        -0.09642857142857143,0.16666666666666666667,0.16666666666666666667,0.5,
        0.05079365079365079,0.125,0.125,0.125,
        0.05079365079365079,0.625,0.125,0.125,
        0.05079365079365079,0.125,0.625,0.125,
        0.05079365079365079,0.125,0.125,0.625,
        0.05079365079365079,0.375,0.125,0.125,
        0.05079365079365079,0.125,0.375,0.125,
        0.05079365079365079,0.125,0.125,0.375,
        0.05079365079365079,0.375,0.375,0.125,
        0.05079365079365079,0.375,0.125,0.375,
        0.05079365079365079,0.125,0.375,0.375,
        0.0,
        0.044444444444444444444,0.25,0.25,0.25,
        -0.09642857142857143,0.16666666666666666667,0.16666666666666666667,0.16666666666666666667,
        -0.09642857142857143,0.5,0.16666666666666666667,0.16666666666666666667,
        -0.09642857142857143,0.16666666666666666667,0.5,0.16666666666666666667,
        -0.09642857142857143,0.16666666666666666667,0.16666666666666666667,0.5,
        0.05079365079365079,0.125,0.125,0.125,
        0.05079365079365079,0.625,0.125,0.125,
        0.05079365079365079,0.125,0.625,0.125,
        0.05079365079365079,0.125,0.125,0.625,
        0.05079365079365079,0.375,0.125,0.125,
        0.05079365079365079,0.125,0.375,0.125,
        0.05079365079365079,0.125,0.125,0.375,
        0.05079365079365079,0.375,0.375,0.125,
        0.05079365079365079,0.375,0.125,0.375,
        0.05079365079365079,0.125,0.375,0.375,
        0.0,
        0.044444444444444444444,0.25,0.25,0.25,
        -0.09642857142857143,0.16666666666666666667,0.16666666666666666667,0.16666666666666666667,
        -0.09642857142857143,0.5,0.16666666666666666667,0.16666666666666666667,
        -0.09642857142857143,0.16666666666666666667,0.5,0.16666666666666666667,
        -0.09642857142857143,0.16666666666666666667,0.16666666666666666667,0.5,
        0.05079365079365079,0.125,0.125,0.125,
        0.05079365079365079,0.625,0.125,0.125,
        0.05079365079365079,0.125,0.625,0.125,
        0.05079365079365079,0.125,0.125,0.625,
        0.05079365079365079,0.375,0.125,0.125,
        0.05079365079365079,0.125,0.375,0.125,
        0.05079365079365079,0.125,0.125,0.375,
        0.05079365079365079,0.375,0.375,0.125,
        0.05079365079365079,0.375,0.125,0.375,
        0.05079365079365079,0.125,0.375,0.375,
        0.0,
        0.044444444444444444444,0.25,0.25,0.25,
        -0.09642857142857143,0.16666666666666666667,0.16666666666666666667,0.16666666666666666667,
        -0.09642857142857143,0.5,0.16666666666666666667,0.16666666666666666667,
        -0.09642857142857143,0.16666666666666666667,0.5,0.16666666666666666667,
        -0.09642857142857143,0.16666666666666666667,0.16666666666666666667,0.5,
        0.05079365079365079,0.125,0.125,0.125,
        0.05079365079365079,0.625,0.125,0.125,
        0.05079365079365079,0.125,0.625,0.125,
        0.05079365079365079,0.125,0.125,0.625,
        0.05079365079365079,0.375,0.125,0.125,
        0.05079365079365079,0.125,0.375,0.125,
        0.05079365079365079,0.125,0.125,0.375,
        0.05079365079365079,0.375,0.375,0.125,
        0.05079365079365079,0.375,0.125,0.375,
        0.05079365079365079,0.125,0.375,0.375,
        0.0,
        0.044444444444444444444,0.25,0.25,0.25,
        -0.09642857142857143,0.16666666666666666667,0.16666666666666666667,0.16666666666666666667,
        -0.09642857142857143,0.5,0.16666666666666666667,0.16666666666666666667,
        -0.09642857142857143,0.16666666666666666667,0.5,0.16666666666666666667,
        -0.09642857142857143,0.16666666666666666667,0.16666666666666666667,0.5,
        0.05079365079365079,0.125,0.125,0.125,
        0.05079365079365079,0.625,0.125,0.125,
        0.05079365079365079,0.125,0.625,0.125,
        0.05079365079365079,0.125,0.125,0.625,
        0.05079365079365079,0.375,0.125,0.125,
        0.05079365079365079,0.125,0.375,0.125,
        0.05079365079365079,0.125,0.125,0.375,
        0.05079365079365079,0.375,0.375,0.125,
        0.05079365079365079,0.375,0.125,0.375,
        0.05079365079365079,0.125,0.375,0.375,
        0.0,
        0.044444444444444444444,0.25,0.25,0.25,
        -0.09642857142857143,0.16666666666666666667,0.16666666666666666667,0.16666666666666666667,
        -0.09642857142857143,0.5,0.16666666666666666667,0.16666666666666666667,
        -0.09642857142857143,0.16666666666666666667,0.5,0.16666666666666666667,
        -0.09642857142857143,0.16666666666666666667,0.16666666666666666667,0.5,
        0.05079365079365079,0.125,0.125,0.125,
        0.05079365079365079,0.625,0.125,0.125,
        0.05079365079365079,0.125,0.625,0.125,
        0.05079365079365079,0.125,0.125,0.625,
        0.05079365079365079,0.375,0.125,0.125,
        0.05079365079365079,0.125,0.375,0.125,
        0.05079365079365079,0.125,0.125,0.375,
        0.05079365079365079,0.375,0.375,0.125,
        0.05079365079365079,0.375,0.125,0.375,
        0.05079365079365079,0.125,0.375,0.375,
        0.0,
    };
    return values + offset[order];
}
template<class TNode> void permutation_if_jac_neg(const Tetra_10 &elem,TNode **nodes) {
    typedef typename TNode::T T;
    T reg0=3*nodes[0]->pos[2]; T reg1=3*nodes[0]->pos[1]; T reg2=2*nodes[4]->pos[1]; T reg3=nodes[2]->pos[1]+reg1; T reg4=4*nodes[4]->pos[1];
    T reg5=4*nodes[4]->pos[2]; T reg6=nodes[1]->pos[1]+reg1; T reg7=nodes[1]->pos[2]+reg0; T reg8=nodes[3]->pos[2]+reg0; T reg9=2*nodes[4]->pos[2];
    T reg10=nodes[3]->pos[1]+reg1; T reg11=nodes[2]->pos[2]+reg0; reg8=reg8-reg9; reg10=reg10-reg2; T reg12=2*nodes[6]->pos[1];
    T reg13=2*nodes[5]->pos[1]; reg2=reg3-reg2; reg3=2*nodes[6]->pos[2]; T reg14=2*nodes[5]->pos[2]; reg9=reg11-reg9;
    reg7=reg7-reg5; reg11=3*nodes[0]->pos[0]; reg6=reg6-reg4; reg6=reg13+reg6; reg9=reg9+reg14;
    T reg15=4*nodes[6]->pos[2]; T reg16=4*nodes[7]->pos[2]; reg8=reg8-reg3; T reg17=2*nodes[4]->pos[0]; reg7=reg14+reg7;
    reg14=nodes[1]->pos[0]+reg11; T reg18=4*nodes[4]->pos[0]; T reg19=nodes[2]->pos[0]+reg11; reg13=reg2+reg13; reg2=4*nodes[6]->pos[1];
    T reg20=4*nodes[7]->pos[1]; reg10=reg10-reg12; reg10=reg10-reg20; T reg21=nodes[3]->pos[0]+reg11; reg3=reg7-reg3;
    reg12=reg6-reg12; reg6=2*nodes[7]->pos[2]; reg9=reg9-reg15; reg19=reg19-reg17; reg7=2*nodes[8]->pos[1];
    reg14=reg14-reg18; T reg22=2*nodes[5]->pos[0]; reg13=reg13-reg2; T reg23=2*nodes[7]->pos[1]; reg8=reg8-reg16;
    T reg24=2*nodes[8]->pos[2]; reg3=reg3-reg6; T reg25=4*nodes[6]->pos[0]; reg19=reg22+reg19; T reg26=2*nodes[6]->pos[0];
    reg22=reg14+reg22; reg14=2*nodes[9]->pos[2]; reg8=reg8+reg24; reg17=reg21-reg17; reg10=reg10+reg7;
    reg12=reg12-reg23; reg6=reg9-reg6; reg23=reg13-reg23; reg9=2*nodes[9]->pos[1]; reg8=reg8+reg14;
    reg22=reg22-reg26; reg12=reg7+reg12; reg19=reg19-reg25; reg7=2*nodes[7]->pos[0]; reg3=reg24+reg3;
    reg26=reg17-reg26; reg10=reg9+reg10; reg13=4*nodes[7]->pos[0]; reg9=reg23+reg9; reg6=reg14+reg6;
    reg14=reg10*reg3; reg26=reg26-reg13; reg17=reg8*reg12; reg21=2*nodes[9]->pos[0]; reg19=reg19-reg7;
    reg7=reg22-reg7; reg22=2*nodes[8]->pos[0]; reg8=reg9*reg8; reg10=reg6*reg10; reg3=reg9*reg3;
    reg12=reg6*reg12; reg26=reg22+reg26; reg22=reg7+reg22; reg10=reg8-reg10; reg14=reg17-reg14;
    reg19=reg19+reg21; reg10=reg22*reg10; reg14=reg19*reg14; reg26=reg21+reg26; reg3=reg12-reg3;
    reg14=reg10-reg14; reg3=reg26*reg3; T det_jac=reg14+reg3;

    if ( det_jac < 0 ) {
        TNode *tmp[] = {nodes[0],nodes[1],nodes[3],nodes[2],nodes[4],nodes[8],nodes[7],nodes[6],nodes[5],nodes[9]};
        for(unsigned i=0;i<10;++i) nodes[i] = tmp[i];
    }
}
template<class PosNodes,class Pvec,class TVI> void get_var_inter_linear(const Tetra_10 &elem,const PosNodes &pos_nodes,const Pvec &pos,TVI &var_inter) {
typedef typename Pvec::template SubType<0>::T T;
    T reg0=1-var_inter[0]; reg0=reg0-var_inter[1]; reg0=reg0-var_inter[2]; T reg1=2*var_inter[2]; T reg2=2*reg0;
    T reg3=2*var_inter[1]; T reg4=reg1-1; T reg5=reg2-1; T reg6=reg3-1; T reg7=2*var_inter[0];
    reg2=reg5+reg2; reg1=reg1+reg4; reg3=reg3+reg6; T reg8=pos_nodes[0][2]*reg2; T reg9=pos_nodes[2][0]*reg3;
    T reg10=pos_nodes[0][0]*reg2; T reg11=pos_nodes[3][1]*reg1; T reg12=pos_nodes[2][2]*reg3; T reg13=pos_nodes[3][2]*reg1; T reg14=pos_nodes[2][1]*reg3;
    T reg15=pos_nodes[3][0]*reg1; T reg16=4*var_inter[0]; T reg17=reg7-1; T reg18=pos_nodes[0][1]*reg2; reg9=reg9-reg10;
    T reg19=4*var_inter[2]; T reg20=pos_nodes[4][0]*reg16; reg7=reg7+reg17; reg12=reg12-reg8; T reg21=pos_nodes[4][2]*reg16;
    T reg22=pos_nodes[4][1]*reg16; reg14=reg14-reg18; reg15=reg15-reg10; T reg23=4*reg0; reg11=reg11-reg18;
    reg13=reg13-reg8; T reg24=4*var_inter[1]; reg5=reg0*reg5; reg17=reg17*var_inter[0]; reg0=reg23-reg19;
    T reg25=pos_nodes[1][1]*reg7; T reg26=pos_nodes[1][0]*reg17; T reg27=reg5*pos_nodes[0][0]; reg11=reg11-reg22; reg9=reg9-reg20;
    T reg28=pos_nodes[5][0]*reg16; T reg29=pos_nodes[5][1]*reg16; reg22=reg14-reg22; reg14=reg24*pos_nodes[6][1]; reg20=reg15-reg20;
    reg6=var_inter[1]*reg6; reg13=reg13-reg21; reg15=pos_nodes[5][2]*reg16; reg21=reg12-reg21; reg12=pos_nodes[6][2]*reg24;
    T reg30=reg23-reg16; T reg31=reg23-reg24; T reg32=pos_nodes[1][0]*reg7; T reg33=pos_nodes[6][0]*reg24; T reg34=pos_nodes[1][1]*reg17;
    T reg35=reg5*pos_nodes[0][1]; T reg36=pos_nodes[7][1]*reg0; reg11=reg11-reg14; reg26=reg27+reg26; reg27=pos_nodes[1][2]*reg17;
    reg18=reg25-reg18; reg25=pos_nodes[4][1]*reg30; T reg37=pos_nodes[2][1]*reg6; T reg38=pos_nodes[0][2]*reg5; T reg39=pos_nodes[7][2]*reg0;
    reg13=reg13-reg12; T reg40=pos_nodes[6][2]*reg31; reg15=reg21+reg15; reg34=reg35+reg34; reg29=reg22+reg29;
    reg21=pos_nodes[1][2]*reg7; reg22=pos_nodes[6][1]*reg31; reg4=reg4*var_inter[2]; reg10=reg32-reg10; reg32=pos_nodes[4][0]*reg30;
    reg28=reg9+reg28; reg9=pos_nodes[6][0]*reg31; reg20=reg20-reg33; reg35=pos_nodes[7][0]*reg0; T reg41=pos_nodes[2][0]*reg6;
    reg32=reg10+reg32; reg22=reg29+reg22; reg34=reg37+reg34; reg10=pos_nodes[3][1]*reg4; reg29=pos_nodes[2][2]*reg6;
    reg9=reg28+reg9; reg28=pos_nodes[5][0]*reg24; reg39=reg13+reg39; reg13=reg23*var_inter[0]; reg27=reg38+reg27;
    reg37=pos_nodes[7][0]*reg19; reg38=pos_nodes[7][1]*reg19; T reg42=reg16*pos_nodes[8][2]; reg40=reg15+reg40; reg15=pos_nodes[3][0]*reg4;
    T reg43=pos_nodes[5][1]*reg24; reg25=reg18+reg25; reg8=reg21-reg8; reg35=reg20+reg35; reg18=reg16*pos_nodes[8][0];
    reg20=pos_nodes[4][2]*reg30; reg36=reg11+reg36; reg11=reg16*pos_nodes[8][1]; reg26=reg41+reg26; reg21=pos_nodes[7][2]*reg19;
    reg28=reg32+reg28; reg32=reg24*pos_nodes[9][1]; reg15=reg26+reg15; reg10=reg34+reg10; reg26=pos_nodes[4][1]*reg13;
    reg34=reg16*var_inter[1]; reg41=reg24*pos_nodes[9][2]; reg40=reg40-reg21; reg20=reg8+reg20; reg8=pos_nodes[5][2]*reg24;
    T reg44=pos_nodes[9][2]*reg19; reg18=reg35+reg18; reg35=pos_nodes[3][2]*reg4; reg43=reg25+reg43; reg27=reg29+reg27;
    reg25=pos_nodes[9][1]*reg19; reg29=reg24*pos_nodes[9][0]; reg9=reg9-reg37; T reg45=pos_nodes[4][0]*reg13; T reg46=pos_nodes[9][0]*reg19;
    reg22=reg22-reg38; reg11=reg36+reg11; reg42=reg39+reg42; reg8=reg20+reg8; reg20=pos_nodes[4][2]*reg13;
    reg35=reg27+reg35; reg32=reg11+reg32; reg18=reg29+reg18; reg33=reg28-reg33; reg11=pos_nodes[5][0]*reg34;
    reg14=reg43-reg14; reg40=reg44+reg40; reg27=reg23*var_inter[1]; reg42=reg41+reg42; reg22=reg25+reg22;
    reg9=reg46+reg9; reg25=pos_nodes[5][1]*reg34; reg26=reg10+reg26; reg45=reg15+reg45; reg10=reg18*reg40;
    reg15=reg9*reg42; reg28=reg32*reg40; reg29=reg22*reg42; reg36=pos_nodes[5][2]*reg34; reg20=reg35+reg20;
    reg23=reg23*var_inter[2]; reg35=pos_nodes[8][1]*reg19; reg38=reg14-reg38; reg12=reg8-reg12; reg8=pos_nodes[8][0]*reg19;
    reg37=reg33-reg37; reg14=pos_nodes[6][0]*reg27; reg11=reg45+reg11; reg25=reg26+reg25; reg26=reg27*pos_nodes[6][1];
    reg35=reg38+reg35; reg8=reg37+reg8; reg26=reg25+reg26; reg25=pos_nodes[7][1]*reg23; reg33=reg23*pos_nodes[7][0];
    reg37=reg16*var_inter[2]; reg38=pos_nodes[6][2]*reg27; reg28=reg29-reg28; reg39=reg9*reg32; reg14=reg11+reg14;
    reg36=reg20+reg36; reg10=reg15-reg10; reg21=reg12-reg21; reg11=pos_nodes[8][2]*reg19; reg12=reg18*reg22;
    reg20=reg37*pos_nodes[8][0]; reg38=reg36+reg38; reg36=pos_nodes[7][2]*reg23; reg33=reg14+reg33; reg14=reg8*reg28;
    reg41=reg35*reg10; reg12=reg39-reg12; reg43=reg24*var_inter[2]; reg11=reg21+reg11; reg25=reg26+reg25;
    reg21=reg37*pos_nodes[8][1]; reg26=reg9*reg11; reg44=reg8*reg40; reg45=reg18*reg11; reg46=reg8*reg42;
    T reg47=reg22*reg11; T reg48=reg35*reg40; T reg49=reg32*reg11; T reg50=reg35*reg42; T reg51=reg11*reg12;
    reg14=reg41-reg14; reg41=reg43*pos_nodes[9][0]; reg20=reg33+reg20; reg21=reg25+reg21; reg25=reg43*pos_nodes[9][1];
    reg36=reg38+reg36; reg33=reg37*pos_nodes[8][2]; reg45=reg46-reg45; reg20=reg41+reg20; reg26=reg44-reg26;
    reg38=reg43*pos_nodes[9][2]; reg47=reg48-reg47; reg41=reg8*reg32; reg49=reg50-reg49; reg33=reg36+reg33;
    reg36=reg18*reg35; reg51=reg14-reg51; reg14=reg8*reg22; T reg52=reg9*reg35; reg21=reg25+reg21;
    reg26=reg26/reg51; reg36=reg41-reg36; reg52=reg14-reg52; reg45=reg45/reg51; reg20=pos[0]-reg20;
    reg10=reg10/reg51; reg47=reg47/reg51; reg49=reg49/reg51; reg33=reg38+reg33; reg28=reg28/reg51;
    reg21=pos[1]-reg21; reg47=reg20*reg47; reg26=reg21*reg26; reg45=reg21*reg45; reg49=reg20*reg49;
    reg10=reg21*reg10; reg28=reg20*reg28; reg52=reg52/reg51; reg36=reg36/reg51; reg51=reg12/reg51;
    reg33=pos[2]-reg33; reg51=reg33*reg51; reg28=reg10-reg28; reg36=reg33*reg36; reg45=reg49-reg45;
    reg52=reg33*reg52; reg47=reg26-reg47; var_inter[0]=reg28-reg51; var_inter[1]=reg36+reg45; var_inter[2]=reg47-reg52;

}
template<class PosNodes,class Pvec,class TVI> void get_var_inter(const Tetra_10 &elem,const PosNodes &pos_nodes,const Pvec &pos,TVI &var_inter) {
typedef typename Pvec::template SubType<0>::T T;
    T reg0=1-var_inter[0]; reg0=reg0-var_inter[1]; reg0=reg0-var_inter[2]; T reg1=2*var_inter[2]; T reg2=2*reg0;
    T reg3=2*var_inter[1]; T reg4=reg1-1; T reg5=reg2-1; T reg6=reg3-1; reg1=reg1+reg4;
    T reg7=2*var_inter[0]; reg2=reg5+reg2; reg3=reg3+reg6; T reg8=pos_nodes[2][1]*reg3; T reg9=pos_nodes[3][2]*reg1;
    T reg10=4*var_inter[0]; T reg11=pos_nodes[3][0]*reg1; T reg12=pos_nodes[2][2]*reg3; T reg13=pos_nodes[0][2]*reg2; T reg14=pos_nodes[0][0]*reg2;
    T reg15=pos_nodes[2][0]*reg3; T reg16=pos_nodes[3][1]*reg1; T reg17=reg7-1; T reg18=pos_nodes[0][1]*reg2; reg8=reg8-reg18;
    T reg19=pos_nodes[4][1]*reg10; T reg20=pos_nodes[4][2]*reg10; reg12=reg12-reg13; T reg21=4*var_inter[2]; reg7=reg7+reg17;
    T reg22=pos_nodes[4][0]*reg10; reg15=reg15-reg14; reg16=reg16-reg18; reg17=var_inter[0]*reg17; reg5=reg5*reg0;
    reg11=reg11-reg14; reg9=reg9-reg13; T reg23=4*var_inter[1]; reg0=4*reg0; T reg24=reg17*pos_nodes[1][0];
    T reg25=pos_nodes[0][0]*reg5; T reg26=reg0-reg23; reg11=reg11-reg22; reg22=reg15-reg22; reg15=pos_nodes[5][0]*reg10;
    T reg27=reg23*pos_nodes[6][1]; T reg28=pos_nodes[5][1]*reg10; T reg29=pos_nodes[1][1]*reg7; reg8=reg8-reg19; T reg30=reg0-reg21;
    reg6=reg6*var_inter[1]; reg9=reg9-reg20; T reg31=pos_nodes[5][2]*reg10; reg20=reg12-reg20; reg12=reg23*pos_nodes[6][2];
    T reg32=reg0-reg10; reg19=reg16-reg19; reg16=reg23*pos_nodes[6][0]; T reg33=reg17*pos_nodes[1][1]; T reg34=pos_nodes[0][1]*reg5;
    T reg35=pos_nodes[1][0]*reg7; reg19=reg19-reg27; reg25=reg24+reg25; reg24=pos_nodes[2][0]*reg6; T reg36=pos_nodes[6][1]*reg26;
    T reg37=reg5*pos_nodes[0][2]; reg18=reg29-reg18; reg29=pos_nodes[4][1]*reg32; T reg38=pos_nodes[1][2]*reg17; T reg39=pos_nodes[7][1]*reg30;
    reg9=reg9-reg12; reg28=reg8+reg28; reg8=pos_nodes[7][2]*reg30; T reg40=pos_nodes[1][2]*reg7; T reg41=pos_nodes[6][2]*reg26;
    reg31=reg20+reg31; reg34=reg33+reg34; reg20=pos_nodes[2][1]*reg6; reg33=pos_nodes[4][0]*reg32; reg11=reg11-reg16;
    T reg42=pos_nodes[7][0]*reg30; T reg43=pos_nodes[6][0]*reg26; reg14=reg35-reg14; reg15=reg22+reg15; reg4=var_inter[2]*reg4;
    reg36=reg28+reg36; reg43=reg15+reg43; reg13=reg40-reg13; reg15=pos_nodes[4][2]*reg32; reg22=pos_nodes[7][2]*reg21;
    reg28=reg10*pos_nodes[8][1]; reg35=pos_nodes[5][0]*reg23; reg19=reg39+reg19; reg33=reg14+reg33; reg14=reg10*pos_nodes[8][2];
    reg9=reg8+reg9; reg8=pos_nodes[7][1]*reg21; reg39=pos_nodes[7][0]*reg21; reg40=var_inter[0]*reg0; reg24=reg25+reg24;
    reg25=pos_nodes[3][0]*reg4; T reg44=pos_nodes[2][2]*reg6; reg37=reg38+reg37; reg41=reg31+reg41; reg31=pos_nodes[3][1]*reg4;
    reg20=reg34+reg20; reg34=reg10*pos_nodes[8][0]; reg11=reg42+reg11; reg29=reg18+reg29; reg18=pos_nodes[5][1]*reg23;
    reg35=reg33+reg35; reg31=reg20+reg31; reg20=pos_nodes[4][1]*reg40; reg44=reg37+reg44; reg33=reg23*pos_nodes[9][1];
    reg37=pos_nodes[3][2]*reg4; reg28=reg19+reg28; reg25=reg24+reg25; reg19=pos_nodes[4][0]*reg40; reg24=reg10*var_inter[1];
    reg38=pos_nodes[9][2]*reg21; reg41=reg41-reg22; reg42=reg23*pos_nodes[9][0]; reg34=reg11+reg34; reg18=reg29+reg18;
    reg14=reg9+reg14; reg9=reg23*pos_nodes[9][2]; reg11=pos_nodes[9][0]*reg21; reg43=reg43-reg39; reg36=reg36-reg8;
    reg29=pos_nodes[9][1]*reg21; T reg45=pos_nodes[5][2]*reg23; reg15=reg13+reg15; reg9=reg14+reg9; reg38=reg41+reg38;
    reg13=reg0*var_inter[1]; reg45=reg15+reg45; reg37=reg44+reg37; reg27=reg18-reg27; reg11=reg43+reg11;
    reg29=reg36+reg29; reg16=reg35-reg16; reg33=reg28+reg33; reg20=reg31+reg20; reg14=pos_nodes[5][1]*reg24;
    reg15=pos_nodes[5][0]*reg24; reg19=reg25+reg19; reg42=reg34+reg42; reg18=pos_nodes[4][2]*reg40; reg0=var_inter[2]*reg0;
    reg15=reg19+reg15; reg19=pos_nodes[6][0]*reg13; reg25=reg42*reg38; reg28=reg11*reg9; reg31=reg33*reg38;
    reg34=reg29*reg9; reg8=reg27-reg8; reg27=pos_nodes[8][1]*reg21; reg12=reg45-reg12; reg35=pos_nodes[8][0]*reg21;
    reg39=reg16-reg39; reg16=pos_nodes[5][2]*reg24; reg14=reg20+reg14; reg20=reg13*pos_nodes[6][1]; reg18=reg37+reg18;
    reg36=reg13*pos_nodes[6][2]; reg31=reg34-reg31; reg37=reg11*reg33; reg16=reg18+reg16; reg25=reg28-reg25;
    reg18=var_inter[2]*reg10; reg41=reg42*reg29; reg43=pos_nodes[8][2]*reg21; reg22=reg12-reg22; reg19=reg15+reg19;
    reg27=reg8+reg27; reg35=reg39+reg35; reg8=pos_nodes[7][1]*reg0; reg12=reg0*pos_nodes[7][0]; reg20=reg14+reg20;
    reg14=reg35*reg31; reg15=reg18*pos_nodes[8][0]; reg19=reg12+reg19; reg12=reg27*reg25; reg36=reg16+reg36;
    reg20=reg8+reg20; reg8=reg18*pos_nodes[8][1]; reg41=reg37-reg41; reg16=var_inter[2]*reg23; reg39=pos_nodes[7][2]*reg0;
    reg43=reg22+reg43; reg22=reg43*reg41; reg44=reg29*reg43; reg45=reg27*reg9; reg14=reg12-reg14;
    reg12=reg33*reg43; reg15=reg19+reg15; reg19=reg16*pos_nodes[9][0]; T reg46=reg27*reg38; T reg47=reg35*reg9;
    T reg48=reg42*reg43; T reg49=reg35*reg38; T reg50=reg11*reg43; T reg51=reg18*pos_nodes[8][2]; reg36=reg39+reg36;
    reg8=reg20+reg8; reg20=reg16*pos_nodes[9][1]; reg44=reg46-reg44; reg48=reg47-reg48; reg50=reg49-reg50;
    reg39=reg35*reg33; reg12=reg45-reg12; T reg52=reg42*reg27; T reg53=reg35*reg29; T reg54=reg11*reg27;
    reg20=reg8+reg20; reg51=reg36+reg51; reg8=reg16*pos_nodes[9][2]; reg19=reg15+reg19; reg22=reg14-reg22;
    reg19=pos[0]-reg19; reg25=reg25/reg22; reg48=reg48/reg22; reg8=reg51+reg8; reg44=reg44/reg22;
    reg54=reg53-reg54; reg31=reg31/reg22; reg50=reg50/reg22; reg20=pos[1]-reg20; reg12=reg12/reg22;
    reg52=reg39-reg52; reg25=reg20*reg25; reg12=reg19*reg12; reg48=reg20*reg48; reg44=reg19*reg44;
    reg50=reg20*reg50; reg31=reg19*reg31; reg54=reg54/reg22; reg52=reg52/reg22; reg22=reg41/reg22;
    reg8=pos[2]-reg8; reg22=reg8*reg22; reg31=reg25-reg31; reg52=reg8*reg52; reg48=reg12-reg48;
    reg54=reg8*reg54; reg44=reg50-reg44; var_inter[0]+=reg31-reg22; var_inter[1]+=reg52+reg48; var_inter[2]+=reg44-reg54;

}
template<> struct ElemVarInterFromPosNonLinear<Tetra_10> { static const bool res = 3; };
#ifndef STRUCT_Elementary_mul_nb_nodes_of_each_children_elem
#define STRUCT_Elementary_mul_nb_nodes_of_each_children_elem
struct Elementary_mul_nb_nodes_of_each_children_elem {};
#endif // STRUCT_Elementary_mul_nb_nodes_of_each_children_elem
template<class TVI,class TVAL,class T> void get_interp(const Tetra_10 &ne,const Elementary_mul_nb_nodes_of_each_children_elem &n,const TVI &var_inter,const TVAL &val,T &res) {
    res=val[0];

}
#ifndef STRUCT_Elementary_mul_nb_nodes
#define STRUCT_Elementary_mul_nb_nodes
struct Elementary_mul_nb_nodes {};
#endif // STRUCT_Elementary_mul_nb_nodes
template<class TVI,class TVAL,class T> void get_interp(const Tetra_10 &ne,const Elementary_mul_nb_nodes &n,const TVI &var_inter,const TVAL &val,T &res) {
    res=val[0];

}
#ifndef STRUCT_Nodal
#define STRUCT_Nodal
struct Nodal {};
#endif // STRUCT_Nodal
template<class TVI,class TVAL,class T> void get_interp(const Tetra_10 &ne,const Nodal &n,const TVI &var_inter,const TVAL &val,T &res) {
    T reg0=1-var_inter[0]; reg0=reg0-var_inter[1]; reg0=reg0-var_inter[2]; T reg1=2*var_inter[0]; T reg2=2*reg0;
    T reg3=2*var_inter[1]; reg1=reg1-1; reg2=reg2-1; T reg4=2*var_inter[2]; reg3=reg3-1;
    reg1=var_inter[0]*reg1; reg2=reg2*reg0; reg4=reg4-1; reg3=var_inter[1]*reg3; reg1=val[1]*reg1;
    reg2=val[0]*reg2; reg4=var_inter[2]*reg4; reg3=val[2]*reg3; reg1=reg2+reg1; reg0=4*reg0;
    reg2=var_inter[0]*reg0; reg3=reg1+reg3; reg4=val[3]*reg4; reg1=4*var_inter[0]; T reg5=var_inter[1]*reg1;
    reg4=reg3+reg4; reg2=val[4]*reg2; reg3=var_inter[1]*reg0; reg5=val[5]*reg5; reg2=reg4+reg2;
    reg5=reg2+reg5; reg3=val[6]*reg3; reg0=var_inter[2]*reg0; reg2=4*var_inter[1]; reg1=var_inter[2]*reg1;
    reg0=val[7]*reg0; reg3=reg5+reg3; reg0=reg3+reg0; reg1=val[8]*reg1; reg2=var_inter[2]*reg2;
    reg1=reg0+reg1; reg2=val[9]*reg2; res=reg1+reg2;

}
#ifndef STRUCT_Global
#define STRUCT_Global
struct Global {};
#endif // STRUCT_Global
template<class TVI,class TVAL,class T> void get_interp(const Tetra_10 &ne,const Global &n,const TVI &var_inter,const TVAL &val,T &res) {
    res=val[0];

}
#ifndef STRUCT_Gauss_1
#define STRUCT_Gauss_1
struct Gauss_1 {};
#endif // STRUCT_Gauss_1
template<class TVI,class TVAL,class T> void get_interp(const Tetra_10 &ne,const Gauss_1 &n,const TVI &var_inter,const TVAL &val,T &res) {
    T reg0=var_inter[0]-0.25; T reg1=0.25+reg0; T reg2=var_inter[1]-0.25; reg1=reg1/0.25; T reg3=0.25+reg2;
    reg3=reg1*reg3; reg1=var_inter[2]-0.25; reg3=reg3/0.25; T reg4=0.25+reg1; reg4=reg3*reg4;
    reg4=reg4/0.25; res=val[0]*reg4;

}
#ifndef STRUCT_Gauss_0
#define STRUCT_Gauss_0
struct Gauss_0 {};
#endif // STRUCT_Gauss_0
template<class TVI,class TVAL,class T> void get_interp(const Tetra_10 &ne,const Gauss_0 &n,const TVI &var_inter,const TVAL &val,T &res) {
    res=val[0];

}
#ifndef STRUCT_Gauss_3
#define STRUCT_Gauss_3
struct Gauss_3 {};
#endif // STRUCT_Gauss_3
template<class TVI,class TVAL,class T> void get_interp(const Tetra_10 &ne,const Gauss_3 &n,const TVI &var_inter,const TVAL &val,T &res) {
    T reg0=var_inter[0]-0.5; T reg1=var_inter[0]-0.16666666666666666667; T reg2=var_inter[0]-0.25; T reg3=reg2*reg0; reg0=reg1*reg0;
    reg3=reg3/0.027777777777777777774; T reg4=var_inter[1]-0.25; reg0=reg0/0.020833333333333333332; T reg5=var_inter[1]-0.16666666666666666667; reg1=reg2*reg1;
    reg1=reg1/0.083333333333333333329; reg3=reg4*reg3; T reg6=var_inter[1]-0.5; reg0=reg0*reg5; reg1=reg4*reg1;
    T reg7=reg6*reg3; reg0=reg6*reg0; reg7=reg7/0.027777777777777777774; T reg8=var_inter[2]-0.16666666666666666667; reg0=reg0/0.020833333333333333332;
    T reg9=var_inter[2]-0.25; reg1=reg6*reg1; reg6=var_inter[2]-0.5; reg7=reg9*reg7; T reg10=reg0*reg8;
    reg3=reg5*reg3; reg1=reg1/0.027777777777777777774; reg5=reg6*reg7; reg1=reg9*reg1; reg10=reg10*reg6;
    reg3=reg3/0.083333333333333333329; reg10=reg10/0.020833333333333333332; reg5=reg5/0.027777777777777777774; reg1=reg6*reg1; reg3=reg9*reg3;
    reg1=reg1/0.027777777777777777774; reg3=reg6*reg3; reg5=val[1]*reg5; reg10=val[0]*reg10; reg10=reg5-reg10;
    reg1=val[2]*reg1; reg3=reg3/0.027777777777777777774; reg7=reg8*reg7; reg1=reg10+reg1; reg3=val[3]*reg3;
    reg7=reg7/0.083333333333333333329; reg3=reg1+reg3; reg7=val[4]*reg7; res=reg3+reg7;

}
#ifndef STRUCT_Gauss_2
#define STRUCT_Gauss_2
struct Gauss_2 {};
#endif // STRUCT_Gauss_2
template<class TVI,class TVAL,class T> void get_interp(const Tetra_10 &ne,const Gauss_2 &n,const TVI &var_inter,const TVAL &val,T &res) {
    T reg0=var_inter[0]-0.13819660112501051517; T reg1=var_inter[0]-0.58541019662496845445; reg0=reg0/0.44721359549995793928; reg1=reg1/0.44721359549995793928; T reg2=var_inter[1]-0.58541019662496845445;
    T reg3=var_inter[1]-0.13819660112501051517; reg0=reg2*reg0; reg2=reg1*reg2; reg2=reg2/0.44721359549995793928; T reg4=var_inter[2]-0.58541019662496845445;
    reg0=reg0/0.44721359549995793928; reg3=reg1*reg3; reg1=reg2*reg4; reg3=reg3/0.44721359549995793928; reg0=reg4*reg0;
    reg1=reg1/0.44721359549995793928; T reg5=var_inter[2]-0.13819660112501051517; reg0=reg0/0.44721359549995793928; reg3=reg4*reg3; reg3=reg3/0.44721359549995793928;
    reg5=reg2*reg5; reg4=val[1]*reg0; reg1=val[0]*reg1; reg1=reg4-reg1; reg4=val[2]*reg3;
    reg5=reg5/0.44721359549995793928; reg4=reg1+reg4; reg5=val[3]*reg5; res=reg4+reg5;

}
#ifndef STRUCT_Flat_interpolation
#define STRUCT_Flat_interpolation
struct Flat_interpolation {};
#endif // STRUCT_Flat_interpolation
template<class TVI,class TVAL,class T> void get_interp(const Tetra_10 &ne,const Flat_interpolation &n,const TVI &var_inter,const TVAL &val,T &res) {
    res=val[0];

}
#ifndef STRUCT_Der_nodal
#define STRUCT_Der_nodal
struct Der_nodal {};
#endif // STRUCT_Der_nodal
template<class TVI,class TVAL,class T> void get_interp(const Tetra_10 &ne,const Der_nodal &n,const TVI &var_inter,const TVAL &val,T &res) {
    res=val[0];

}
#ifndef STRUCT_Elementary
#define STRUCT_Elementary
struct Elementary {};
#endif // STRUCT_Elementary
template<class TVI,class TVAL,class T> void get_interp(const Tetra_10 &ne,const Elementary &n,const TVI &var_inter,const TVAL &val,T &res) {
    res=val[0];

}
#ifndef STRUCT_Skin_elementary
#define STRUCT_Skin_elementary
struct Skin_elementary {};
#endif // STRUCT_Skin_elementary
template<class TVI,class TVAL,class T> void get_interp(const Tetra_10 &ne,const Skin_elementary &n,const TVI &var_inter,const TVAL &val,T &res) {
    res=val[0];

}
template<class TVI,class TVAL> void get_shape_functions(const Tetra_10 &ne,const TVI &var_inter,TVAL &res) {
    typedef typename TVAL::template SubType<0>::T T;
    T reg0=1-var_inter[0]; reg0=reg0-var_inter[1]; reg0=reg0-var_inter[2]; T reg1=2*reg0; T reg2=2*var_inter[2];
    T reg3=2*var_inter[0]; T reg4=2*var_inter[1]; T reg5=4*var_inter[0]; reg2=reg2-1; reg4=reg4-1;
    reg3=reg3-1; T reg6=4*var_inter[1]; T reg7=4*reg0; reg1=reg1-1; T reg8=var_inter[2]*reg5;
    res[8]=reg8; T reg9=var_inter[2]*reg7; res[7]=reg9; reg6=var_inter[2]*reg6; res[9]=reg6;
    T reg10=var_inter[1]*reg7; res[6]=reg10; reg5=var_inter[1]*reg5; res[5]=reg5; reg2=var_inter[2]*reg2;
    res[3]=reg2; reg4=var_inter[1]*reg4; res[2]=reg4; reg3=var_inter[0]*reg3; res[1]=reg3;
    reg0=reg1*reg0; res[0]=reg0; reg7=var_inter[0]*reg7; res[4]=reg7;

}
#ifndef AUTORIZEDPERM
#define AUTORIZEDPERM
template<class T> struct AuthorizedPerm;
#endif // AUTORIZEDPERM
template<> struct AuthorizedPerm<Tetra_10> {
    static const unsigned nb_permutations = 0;
    template<class TE> static void make_permutation( TE &e, unsigned nb_perm ) {
    }
    template<class TE> static void unmake_permutation( TE &e, unsigned nb_perm ) {
    }
};

template<class TN,class TNG,class TD,unsigned NET,class TVI>
typename TNG::T get_det_jac( const Element<Tetra_10,TN,TNG,TD,NET> &elem, const TVI &var_inter ) {
    typedef typename TNG::T T;
    T reg0=1-var_inter[0]; reg0=reg0-var_inter[1]; reg0=reg0-var_inter[2]; T reg1=2*reg0; T reg2=2*var_inter[2];
    T reg3=2*var_inter[1]; T reg4=2*var_inter[0]; T reg5=reg2-1; T reg6=reg4-1; T reg7=reg3-1;
    T reg8=reg1-1; T reg9=4*reg0; T reg10=4*var_inter[0]; reg2=reg2+reg5; reg4=reg4+reg6;
    reg3=reg3+reg7; reg1=reg1+reg8; T reg11=elem.pos(1)[2]*reg4; T reg12=elem.pos(3)[1]*reg2; T reg13=reg1*elem.pos(0)[2];
    T reg14=elem.pos(1)[1]*reg4; T reg15=reg1*elem.pos(0)[1]; T reg16=elem.pos(2)[1]*reg3; T reg17=reg9-reg10; T reg18=elem.pos(3)[2]*reg2;
    T reg19=elem.pos(2)[2]*reg3; T reg20=elem.pos(4)[2]*reg10; reg14=reg14-reg15; T reg21=elem.pos(4)[1]*reg17; reg18=reg18-reg13;
    T reg22=elem.pos(4)[1]*reg10; reg12=reg12-reg15; T reg23=elem.pos(4)[2]*reg17; reg11=reg11-reg13; reg16=reg16-reg15;
    T reg24=4*var_inter[2]; T reg25=4*var_inter[1]; reg19=reg19-reg13; reg16=reg16-reg22; T reg26=elem.pos(5)[2]*reg10;
    reg12=reg12-reg22; T reg27=elem.pos(5)[2]*reg25; reg4=elem.pos(1)[0]*reg4; T reg28=elem.pos(6)[1]*reg25; reg21=reg14+reg21;
    reg18=reg18-reg20; reg14=reg9-reg24; reg11=reg23+reg11; reg19=reg19-reg20; reg23=elem.pos(5)[1]*reg25;
    T reg29=elem.pos(5)[1]*reg10; T reg30=elem.pos(6)[2]*reg25; T reg31=reg9-reg25; T reg32=reg1*elem.pos(0)[0]; reg3=elem.pos(2)[0]*reg3;
    reg11=reg27+reg11; reg23=reg21+reg23; reg2=elem.pos(3)[0]*reg2; reg4=reg4-reg32; reg21=elem.pos(6)[1]*reg31;
    reg18=reg18-reg30; reg27=elem.pos(7)[2]*reg14; T reg33=elem.pos(6)[2]*reg31; reg17=elem.pos(4)[0]*reg17; reg12=reg12-reg28;
    T reg34=elem.pos(7)[1]*reg14; reg3=reg3-reg32; T reg35=elem.pos(4)[0]*reg10; reg19=reg26+reg19; reg29=reg16+reg29;
    reg34=reg12+reg34; reg12=elem.pos(8)[1]*reg10; reg3=reg3-reg35; reg11=reg11-reg30; reg16=elem.pos(7)[2]*reg24;
    reg19=reg33+reg19; reg26=elem.pos(7)[1]*reg24; reg23=reg23-reg28; reg18=reg27+reg18; reg27=elem.pos(5)[0]*reg10;
    reg21=reg29+reg21; reg2=reg2-reg32; reg29=elem.pos(5)[0]*reg25; reg4=reg17+reg4; reg17=elem.pos(8)[2]*reg10;
    reg33=elem.pos(8)[1]*reg24; reg17=reg18+reg17; reg18=elem.pos(8)[2]*reg24; T reg36=elem.pos(9)[1]*reg25; reg3=reg27+reg3;
    reg27=elem.pos(6)[0]*reg25; reg4=reg29+reg4; reg23=reg23-reg26; reg29=elem.pos(9)[1]*reg24; reg21=reg21-reg26;
    T reg37=elem.pos(9)[2]*reg25; T reg38=elem.pos(9)[2]*reg24; reg31=elem.pos(6)[0]*reg31; reg11=reg11-reg16; reg19=reg19-reg16;
    reg12=reg34+reg12; reg2=reg2-reg35; reg4=reg4-reg27; reg36=reg12+reg36; reg2=reg2-reg27;
    reg19=reg38+reg19; reg33=reg23+reg33; reg29=reg21+reg29; reg37=reg17+reg37; reg18=reg11+reg18;
    reg31=reg3+reg31; reg14=elem.pos(7)[0]*reg14; reg3=elem.pos(7)[0]*reg24; reg11=elem.pos(8)[0]*reg24; reg12=reg29*reg37;
    reg31=reg31-reg3; reg17=elem.pos(9)[0]*reg24; reg21=elem.pos(8)[0]*reg10; reg23=reg33*reg37; reg34=reg36*reg18;
    reg14=reg2+reg14; reg2=reg19*reg36; reg4=reg4-reg3; reg17=reg31+reg17; reg31=elem.pos(9)[0]*reg25;
    reg2=reg12-reg2; reg14=reg21+reg14; reg4=reg11+reg4; reg11=reg19*reg33; reg34=reg23-reg34;
    reg12=reg29*reg18; reg12=reg11-reg12; reg11=reg2*reg4; reg21=reg34*reg17; reg14=reg31+reg14;
    reg21=reg11-reg21; reg11=reg12*reg14; reg21=reg11+reg21; return reg21;

}
template<class TN,class T,class TNodalStaticData,class TD,unsigned NET>
Vec<T,3> barycenter( const Element<Tetra_10,TN,Node<3,T,TNodalStaticData>,TD,NET> &elem ) {
    Vec<T,3> res;
    T reg0=0.44721359549995798321*elem.pos(1)[1]; T reg1=0.44721359549995798321*elem.pos(1)[2]; T reg2=0.44721359549995809424*elem.pos(0)[2]; T reg3=1.3416407864998740607*elem.pos(1)[1]; T reg4=0.44721359549995798321*elem.pos(3)[1];
    T reg5=0.44721359549995798321*elem.pos(2)[1]; T reg6=1.3416407864998739496*elem.pos(0)[1]; T reg7=0.44721359549995798321*elem.pos(2)[2]; T reg8=0.44721359549995798321*elem.pos(3)[2]; T reg9=1.3416407864998739496*elem.pos(0)[2];
    T reg10=1.3416407864998740607*elem.pos(1)[2]; T reg11=0.44721359549995809424*elem.pos(0)[1]; T reg12=1.7888543819998319329*elem.pos(4)[2]; T reg13=reg11-reg4; T reg14=reg6+reg0;
    T reg15=1.7888543819998319329*elem.pos(4)[1]; T reg16=1.7888543819998321549*elem.pos(4)[1]; T reg17=reg6+reg4; T reg18=2.3416407864998740607*elem.pos(4)[1]; reg6=reg5+reg6;
    T reg19=0.55278640450004201679*elem.pos(4)[1]; reg3=reg11+reg3; T reg20=reg9+reg7; reg10=reg2+reg10; T reg21=reg8+reg9;
    T reg22=0.55278640450004201679*elem.pos(4)[2]; T reg23=1.7888543819998321549*elem.pos(4)[2]; T reg24=1.3416407864998740607*elem.pos(2)[1]; T reg25=1.3416407864998740607*elem.pos(2)[2]; T reg26=reg11-reg5;
    T reg27=reg2-reg8; T reg28=2.3416407864998740607*elem.pos(4)[2]; reg9=reg9+reg1; T reg29=reg2-reg7; T reg30=0.099999999999999982674*elem.pos(0)[0];
    reg12=reg12-reg9; T reg31=2.3416407864998740607*elem.pos(5)[2]; T reg32=reg29-reg28; T reg33=0.10000000000000008145*elem.pos(1)[0]; T reg34=reg13-reg18;
    T reg35=0.10000000000000004421*elem.pos(0)[2]; T reg36=1.3416407864998740607*elem.pos(1)[0]; T reg37=1.3416407864998740607*elem.pos(3)[1]; T reg38=reg22+reg20; T reg39=0.55278640450004201679*elem.pos(5)[2];
    reg3=reg3-reg16; reg15=reg15-reg14; T reg40=0.099999999999999995089*elem.pos(1)[2]; T reg41=0.099999999999999995089*elem.pos(1)[1]; T reg42=0.10000000000000004421*elem.pos(0)[1];
    T reg43=0.099999999999999982674*elem.pos(0)[1]; reg18=reg26-reg18; T reg44=0.10000000000000008145*elem.pos(1)[1]; T reg45=reg19+reg17; T reg46=0.55278640450004201679*elem.pos(6)[1];
    T reg47=0.44721359549995809424*elem.pos(0)[0]; T reg48=2.3416407864998740607*elem.pos(5)[1]; T reg49=1.3416407864998740607*elem.pos(3)[2]; T reg50=0.44721359549995798321*elem.pos(2)[0]; reg28=reg27-reg28;
    T reg51=reg6+reg19; T reg52=reg2-reg1; T reg53=1.1102230246251565404e-16*elem.pos(4)[2]; T reg54=reg11-reg0; T reg55=1.1102230246251565404e-16*elem.pos(4)[1];
    T reg56=1.3416407864998739496*elem.pos(0)[0]; T reg57=0.44721359549995798321*elem.pos(1)[0]; T reg58=0.10000000000000008145*elem.pos(1)[2]; T reg59=0.099999999999999982674*elem.pos(0)[2]; reg25=reg2+reg25;
    T reg60=0.10000000000000004421*elem.pos(0)[0]; T reg61=0.099999999999999995089*elem.pos(1)[0]; reg24=reg11+reg24; reg10=reg10-reg23; T reg62=reg21+reg22;
    T reg63=0.55278640450004201679*elem.pos(6)[2]; T reg64=0.55278640450004201679*elem.pos(5)[1]; reg12=reg39+reg12; T reg65=1.3416407864998740607*elem.pos(2)[0]; reg32=reg32+reg31;
    T reg66=2.3416407864998740607*elem.pos(6)[1]; reg44=reg44-reg43; reg13=reg13-reg19; reg58=reg58-reg59; T reg67=0.44721359549995798321*elem.pos(3)[0];
    T reg68=1.1102230246251565404e-16*elem.pos(7)[2]; reg28=reg28-reg63; reg25=reg25-reg22; reg42=reg42-reg41; reg60=reg60-reg61;
    T reg69=0.099999999999999995089*elem.pos(2)[1]; T reg70=1.1102230246251565404e-16*elem.pos(6)[1]; reg35=reg35-reg40; reg18=reg18+reg48; T reg71=0.099999999999999995089*elem.pos(2)[0];
    reg24=reg24-reg19; reg33=reg33-reg30; reg27=reg27-reg22; T reg72=2.3416407864998740607*elem.pos(6)[2]; reg37=reg11+reg37;
    reg36=reg36+reg47; T reg73=1.7888543819998319329*elem.pos(7)[2]; T reg74=1.7888543819998321549*elem.pos(4)[0]; T reg75=reg62+reg63; T reg76=reg56+reg50;
    T reg77=0.55278640450004201679*elem.pos(4)[0]; T reg78=reg47-reg50; reg49=reg2+reg49; T reg79=reg39-reg38; T reg80=2.3416407864998740607*elem.pos(4)[0];
    T reg81=1.1102230246251565404e-16*elem.pos(7)[1]; reg34=reg34-reg46; T reg82=0.099999999999999995089*elem.pos(2)[2]; T reg83=1.7888543819998319329*elem.pos(6)[2]; T reg84=reg57+reg56;
    reg54=reg54-reg55; reg10=reg39+reg10; T reg85=1.1102230246251565404e-16*elem.pos(6)[2]; reg52=reg52-reg53; T reg86=reg64-reg51;
    reg3=reg64+reg3; T reg87=1.7888543819998319329*elem.pos(4)[0]; T reg88=reg45+reg46; reg15=reg64+reg15; T reg89=1.7888543819998319329*elem.pos(7)[1];
    T reg90=1.7888543819998319329*elem.pos(6)[1]; T reg91=2.3416407864998740607*elem.pos(5)[0]; T reg92=reg78-reg80; T reg93=1.7888543819998321549*elem.pos(6)[1]; reg24=reg64+reg24;
    reg27=reg27-reg72; reg31=reg31+reg52; reg48=reg48+reg54; reg26=reg26-reg19; reg36=reg36-reg74;
    T reg94=0.10000000000000008145*elem.pos(2)[0]; reg32=reg32-reg85; reg10=reg10-reg63; T reg95=reg61+reg30; reg13=reg13-reg66;
    reg29=reg29-reg22; T reg96=reg47-reg67; T reg97=2.3416407864998740607*elem.pos(8)[2]; reg28=reg28-reg68; T reg98=1.7888543819998321549*elem.pos(6)[2];
    reg65=reg47+reg65; T reg99=1.1102230246251565404e-16*elem.pos(4)[0]; T reg100=reg47-reg57; reg25=reg39+reg25; reg34=reg34-reg81;
    T reg101=2.3416407864998740607*elem.pos(8)[1]; reg22=reg49-reg22; reg18=reg18-reg70; reg3=reg3-reg46; reg56=reg56+reg67;
    reg44=reg44-reg69; reg12=reg12-reg63; reg15=reg15-reg46; reg49=0.10000000000000008145*elem.pos(2)[1]; T reg102=reg41+reg43;
    T reg103=reg76+reg77; T reg104=0.55278640450004201679*elem.pos(8)[1]; reg89=reg89-reg88; T reg105=0.55278640450004201679*elem.pos(7)[2]; reg83=reg79+reg83;
    reg79=0.55278640450004201679*elem.pos(8)[2]; reg73=reg73-reg75; reg35=reg35-reg82; T reg106=0.55278640450004201679*elem.pos(7)[1]; reg90=reg86+reg90;
    reg86=0.099999999999999995089*elem.pos(3)[2]; T reg107=0.55278640450004201679*elem.pos(5)[0]; reg87=reg87-reg84; reg58=reg58-reg82; T reg108=0.10000000000000008145*elem.pos(2)[2];
    T reg109=reg40+reg59; T reg110=0.099999999999999995089*elem.pos(3)[0]; reg60=reg60-reg71; reg19=reg37-reg19; reg33=reg33-reg71;
    reg42=reg42-reg69; reg37=0.099999999999999995089*elem.pos(3)[1]; reg87=reg87+reg107; T reg111=0.55278640450004201679*elem.pos(6)[0]; T reg112=0.32360679774997891248*elem.pos(4)[2];
    reg66=reg48-reg66; reg35=reg35-reg86; reg80=reg96-reg80; reg72=reg31-reg72; reg10=reg10-reg105;
    reg31=1.3416407864998740607*elem.pos(3)[0]; reg90=reg90-reg106; reg48=0.55278640450004201679*elem.pos(9)[1]; reg33=reg33-reg110; T reg113=0.32360679774997891248*elem.pos(4)[0];
    reg73=reg73+reg79; reg26=reg64+reg26; reg27=reg27-reg68; reg24=reg24-reg93; reg19=reg19-reg46;
    T reg114=1.7888543819998321549*elem.pos(7)[1]; T reg115=0.32360679774997896214*elem.pos(4)[0]; reg54=reg64+reg54; reg60=reg60-reg110; reg100=reg100-reg99;
    reg108=reg108-reg109; reg25=reg25-reg98; reg36=reg107+reg36; reg13=reg13-reg81; reg94=reg94-reg95;
    reg58=reg58-reg86; reg52=reg39+reg52; reg65=reg65-reg77; reg64=reg77+reg56; T reg116=0.32360679774997891248*elem.pos(4)[1];
    reg28=reg28+reg97; reg29=reg39+reg29; reg44=reg44-reg37; reg12=reg12-reg105; reg32=reg32-reg105;
    reg15=reg15-reg106; reg42=reg42-reg37; reg49=reg49-reg102; reg34=reg34+reg101; reg39=1.7888543819998321549*elem.pos(7)[2];
    reg22=reg22-reg63; T reg117=1.7888543819998319329*elem.pos(6)[0]; T reg118=reg107-reg103; reg3=reg3-reg106; T reg119=0.55278640450004201679*elem.pos(9)[2];
    reg83=reg83-reg105; T reg120=0.32360679774997896214*elem.pos(4)[1]; reg18=reg18-reg106; reg89=reg89+reg104; T reg121=1.1102230246251565404e-16*elem.pos(6)[0];
    reg92=reg92+reg91; T reg122=0.32360679774997896214*elem.pos(4)[2]; reg26=reg26-reg70; T reg123=0.076393202250021002874*elem.pos(4)[1]; T reg124=0.32360679774997897748*elem.pos(5)[1];
    reg42=reg120+reg42; reg120=0.076393202250021018215*elem.pos(5)[2]; reg78=reg78-reg77; reg44=reg116+reg44; reg116=2.3416407864998740607*elem.pos(9)[2];
    reg19=reg19-reg114; reg108=reg108-reg86; reg35=reg122+reg35; reg122=0.076393202250021018215*elem.pos(5)[1]; T reg125=reg82+reg109;
    T reg126=0.10000000000000008145*elem.pos(3)[1]; reg27=reg79+reg27; reg31=reg47+reg31; T reg127=0.10000000000000008145*elem.pos(3)[2]; reg63=reg52-reg63;
    reg29=reg29-reg85; reg52=reg69+reg102; T reg128=2.3416407864998740607*elem.pos(7)[2]; T reg129=1.7888543819998321549*elem.pos(6)[0]; reg65=reg107+reg65;
    reg72=reg72-reg105; reg96=reg96-reg77; T reg130=reg71+reg95; reg49=reg49-reg37; reg58=reg112+reg58;
    reg112=0.10000000000000008145*elem.pos(3)[0]; T reg131=2.3416407864998740607*elem.pos(9)[1]; reg13=reg104+reg13; reg66=reg66-reg106; T reg132=0.32360679774997897748*elem.pos(5)[2];
    T reg133=0.076393202250021002874*elem.pos(4)[2]; T reg134=2.3416407864998740607*elem.pos(7)[1]; reg105=reg25-reg105; reg22=reg22-reg39; reg46=reg54-reg46;
    reg89=reg48+reg89; reg94=reg94-reg110; reg32=reg119+reg32; reg34=reg48+reg34; reg25=0.076393202250021002874*elem.pos(4)[0];
    reg92=reg92-reg121; reg15=reg104+reg15; reg33=reg113+reg33; reg10=reg79+reg10; reg3=reg104+reg3;
    reg12=reg79+reg12; reg54=1.7888543819998319329*elem.pos(7)[0]; reg113=reg111+reg64; reg80=reg80-reg111; T reg135=1.1102230246251565404e-16*elem.pos(7)[0];
    reg87=reg87-reg111; T reg136=0.32360679774997897748*elem.pos(5)[0]; reg106=reg24-reg106; reg60=reg115+reg60; reg36=reg36-reg111;
    reg24=0.55278640450004201679*elem.pos(7)[0]; reg117=reg118+reg117; reg90=reg90+reg48; reg115=2.3416407864998740607*elem.pos(6)[0]; reg91=reg91+reg100;
    reg118=0.076393202250021018215*elem.pos(5)[0]; reg73=reg73+reg119; reg18=reg48+reg18; reg28=reg119+reg28; reg83=reg119+reg83;
    T reg137=reg34*reg10; reg112=reg112-reg130; T reg138=reg83*reg89; T reg139=reg90*reg73; reg63=reg63-reg128;
    T reg140=0.55278640450004201679*elem.pos(8)[0]; reg49=reg123+reg49; reg36=reg36-reg24; reg128=reg29-reg128; reg117=reg117-reg24;
    reg29=reg18*reg28; reg19=reg104+reg19; reg22=reg79+reg22; reg34=reg32*reg34; T reg141=0.55278640450004201679*elem.pos(9)[0];
    reg26=reg26-reg134; reg92=reg92-reg24; reg28=reg28*reg3; reg78=reg107+reg78; T reg142=0.076393202250021002874*elem.pos(6)[0];
    reg100=reg107+reg100; reg126=reg126-reg52; reg134=reg46-reg134; reg33=reg33+reg136; reg89=reg89*reg12;
    reg65=reg65-reg129; reg46=0.32360679774997896214*elem.pos(6)[1]; reg58=reg58+reg132; reg107=0.076393202250021002874*elem.pos(6)[2]; reg42=reg42+reg122;
    reg54=reg54-reg113; reg13=reg13+reg131; reg94=reg25+reg94; reg127=reg127-reg125; reg108=reg133+reg108;
    reg105=reg119+reg105; T reg143=0.32360679774997896214*elem.pos(6)[0]; reg60=reg60+reg118; reg91=reg91-reg115; reg27=reg27+reg116;
    reg106=reg48+reg106; reg72=reg79+reg72; reg73=reg73*reg15; reg77=reg31-reg77; reg80=reg80-reg135;
    reg31=2.3416407864998740607*elem.pos(8)[0]; reg44=reg44+reg124; reg79=0.32360679774997896214*elem.pos(6)[2]; T reg144=0.076393202250021002874*elem.pos(6)[1]; reg115=reg96-reg115;
    reg35=reg35+reg120; reg66=reg104+reg66; reg87=reg87-reg24; reg44=reg44+reg144; reg15=reg83*reg15;
    reg12=reg90*reg12; reg19=reg48+reg19; reg63=reg97+reg63; reg127=reg133+reg127; reg54=reg140+reg54;
    reg117=reg117+reg141; reg77=reg77-reg111; reg48=0.076393202250021002874*elem.pos(7)[1]; reg83=0.076393202250021002874*elem.pos(7)[0]; reg134=reg101+reg134;
    reg46=reg42+reg46; reg42=0.32360679774997896214*elem.pos(7)[1]; reg78=reg78-reg121; reg33=reg33+reg142; reg89=reg73-reg89;
    reg73=1.7888543819998321549*elem.pos(7)[0]; reg87=reg87+reg140; reg115=reg115-reg135; reg137=reg28-reg137; reg79=reg35+reg79;
    reg28=0.32360679774997896214*elem.pos(7)[2]; reg35=reg13*reg72; reg90=reg27*reg66; reg80=reg80+reg31; reg3=reg32*reg3;
    reg10=reg18*reg10; reg18=0.32360679774997896214*elem.pos(7)[0]; reg65=reg65-reg24; reg143=reg60+reg143; reg58=reg58+reg107;
    reg32=0.076393202250021002874*elem.pos(7)[2]; reg13=reg105*reg13; reg94=reg136+reg94; reg60=0.32360679774997891248*elem.pos(6)[0]; reg24=reg91-reg24;
    reg27=reg106*reg27; reg92=reg141+reg92; reg26=reg131+reg26; reg34=reg29-reg34; reg22=reg119+reg22;
    reg29=0.32360679774997891248*elem.pos(6)[1]; reg138=reg139-reg138; reg36=reg140+reg36; reg91=2.3416407864998740607*elem.pos(7)[0]; reg128=reg116+reg128;
    reg111=reg100-reg111; reg126=reg123+reg126; reg49=reg124+reg49; reg96=0.32360679774997891248*elem.pos(6)[2]; reg108=reg132+reg108;
    reg112=reg25+reg112; reg54=reg141+reg54; reg25=0.32360679774997897748*elem.pos(8)[2]; reg12=reg15-reg12; reg58=reg58+reg32;
    reg29=reg49+reg29; reg138=reg87*reg138; reg42=reg46+reg42; reg15=0.076393202250021018215*elem.pos(8)[1]; reg18=reg143+reg18;
    reg46=0.076393202250021018215*elem.pos(8)[0]; reg89=reg117*reg89; reg49=0.32360679774997897748*elem.pos(8)[1]; reg44=reg44+reg48; reg87=0.076393202250021018215*elem.pos(8)[2];
    reg28=reg79+reg28; reg126=reg122+reg126; reg96=reg108+reg96; reg33=reg33+reg83; reg77=reg77-reg73;
    reg79=0.32360679774997897748*elem.pos(8)[0]; reg137=reg92*reg137; reg66=reg105*reg66; reg92=reg26*reg22; reg34=reg36*reg34;
    reg35=reg90-reg35; reg72=reg106*reg72; reg36=reg128*reg19; reg13=reg27-reg13; reg60=reg94+reg60;
    reg112=reg118+reg112; reg80=reg141+reg80; reg19=reg19*reg63; reg111=reg111-reg91; reg22=reg22*reg134;
    reg91=reg78-reg91; reg127=reg120+reg127; reg24=reg140+reg24; reg10=reg3-reg10; reg65=reg141+reg65;
    reg3=2.3416407864998740607*elem.pos(9)[0]; reg115=reg140+reg115; reg36=reg92-reg36; reg115=reg115+reg3; reg33=reg33+reg79;
    reg29=reg48+reg29; reg72=reg66-reg72; reg111=reg31+reg111; reg112=reg142+reg112; reg27=0.32360679774997891248*elem.pos(7)[0];
    reg96=reg32+reg96; reg126=reg144+reg126; reg31=0.32360679774997891248*elem.pos(7)[1]; reg58=reg58+reg25; reg127=reg107+reg127;
    reg32=0.32360679774997891248*elem.pos(7)[2]; reg48=0.076393202250021018215*elem.pos(9)[0]; reg18=reg18+reg46; reg13=reg24*reg13; reg42=reg42+reg15;
    reg24=0.076393202250021018215*elem.pos(9)[1]; reg10=reg80*reg10; reg12=reg54*reg12; reg54=0.076393202250021018215*elem.pos(9)[2]; reg28=reg28+reg87;
    reg63=reg26*reg63; reg134=reg128*reg134; reg89=reg138-reg89; reg137=reg34-reg137; reg77=reg140+reg77;
    reg60=reg83+reg60; reg19=reg22-reg19; reg91=reg3+reg91; reg44=reg44+reg49; reg35=reg65*reg35;
    reg10=reg137+reg10; reg18=reg18+reg48; reg72=reg115*reg72; reg28=reg28+reg54; reg35=reg13-reg35;
    reg42=reg42+reg24; reg60=reg46+reg60; reg77=reg141+reg77; reg19=reg91*reg19; reg44=reg24+reg44;
    reg63=reg134-reg63; reg3=0.32360679774997897748*elem.pos(9)[0]; reg36=reg111*reg36; reg33=reg48+reg33; reg29=reg15+reg29;
    reg13=0.32360679774997897748*elem.pos(9)[1]; reg32=reg127+reg32; reg15=0.32360679774997897748*elem.pos(9)[2]; reg96=reg87+reg96; reg58=reg54+reg58;
    reg12=reg89+reg12; reg27=reg112+reg27; reg31=reg126+reg31; reg60=reg60+reg3; reg18=reg18*reg12;
    reg63=reg77*reg63; reg28=reg12*reg28; reg33=reg33*reg10; reg42=reg12*reg42; reg19=reg36-reg19;
    reg44=reg10*reg44; reg96=reg96+reg15; reg29=reg29+reg13; reg72=reg35+reg72; reg58=reg10*reg58;
    reg32=reg25+reg32; reg27=reg79+reg27; reg31=reg49+reg31; reg58=0.041666666666666664354*reg58; reg12=0.041666666666666664354*reg12;
    reg33=0.041666666666666664354*reg33; reg28=0.041666666666666664354*reg28; reg60=reg60*reg72; reg32=reg15+reg32; reg31=reg13+reg31;
    reg27=reg3+reg27; reg29=reg72*reg29; reg44=0.041666666666666664354*reg44; reg18=0.041666666666666664354*reg18; reg42=0.041666666666666664354*reg42;
    reg63=reg19+reg63; reg10=0.041666666666666664354*reg10; reg96=reg72*reg96; reg32=reg63*reg32; reg96=0.041666666666666664354*reg96;
    reg58=reg28+reg58; reg33=reg18+reg33; reg60=0.041666666666666664354*reg60; reg31=reg63*reg31; reg29=0.041666666666666664354*reg29;
    reg44=reg42+reg44; reg27=reg27*reg63; reg10=reg12+reg10; reg72=0.041666666666666664354*reg72; reg32=0.041666666666666664354*reg32;
    reg60=reg33+reg60; reg27=0.041666666666666664354*reg27; reg72=reg10+reg72; reg63=0.041666666666666664354*reg63; reg96=reg58+reg96;
    reg29=reg44+reg29; reg31=0.041666666666666664354*reg31; reg63=reg72+reg63; reg27=reg60+reg27; reg31=reg29+reg31;
    reg32=reg96+reg32; res[0]=reg27/reg63; res[1]=reg31/reg63; res[2]=reg32/reg63;

    return res;
}
}
#endif // LMT_TETRA_10
