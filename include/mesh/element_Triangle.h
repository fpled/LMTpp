#ifndef LMT_TRIANGLE
#define LMT_TRIANGLE
#include "node.h"
namespace LMT {
inline const double *gauss_point_for_order(unsigned order, const Triangle &elem) { /// order -> degre du polynome a integrer exactement
    static const unsigned offset[] = { 0, 4, 8, 18, 31, 50, 72, 109, 158, 207, }; // fonction de lordre du poly
    static const double values[] = {
        0.5,0.33333333333333333333,0.33333333333333333333,
        0.0,
        0.5,0.33333333333333333333,0.33333333333333333333,
        0.0,
        0.16666666666666666667,0.5,0.0,
        0.16666666666666666667,0.5,0.5,
        0.16666666666666666667,0.0,0.5,
        0.0,
        -0.28125,0.33333333333333333333,0.33333333333333333333,
        0.26041666666666666667,0.2,0.2,
        0.26041666666666666667,0.2,0.6,
        0.26041666666666666667,0.6,0.2,
        0.0,
        0.111690794839005,0.445948490915965,0.445948490915965,
        0.111690794839005,0.108103018168070,0.445948490915965,
        0.111690794839005,0.445948490915965,0.108103018168070,
        0.054975871827661,0.091576213509771,0.091576213509771,
        0.054975871827661,0.816847572980458,0.091576213509771,
        0.054975871827661,0.091576213509771,0.816847572980458,
        0.0,
        0.1125,0.33333333333333333333,0.33333333333333333333,
        0.066197076394253,0.4701420641051151,0.4701420641051151,
        0.066197076394253,0.05971587178976989,0.4701420641051151,
        0.066197076394253,0.4701420641051151,0.05971587178976989,
        0.062969590272414,0.1012865073234563,0.1012865073234563,
        0.062969590272414,0.7974269853530873,0.1012865073234563,
        0.062969590272414,0.1012865073234563,0.7974269853530873,
        0.0,
        0.025422453185103,0.063089014491502,0.063089014491502,
        0.025422453185103,0.873821971016996,0.063089014491502,
        0.025422453185103,0.063089014491502,0.873821971016996,
        0.058393137863189,0.249286745170910,0.501426509658180,
        0.058393137863189,0.501426509658180,0.249286745170910,
        0.058393137863189,0.249286745170910,0.501426509658180,
        0.041425537809187,0.310352451033785,0.053145049844816,
        0.041425537809187,0.053145049844816,0.310352451033785,
        0.041425537809187,0.310352451033785,0.636502499121399,
        0.041425537809187,0.636502499121399,0.310352451033785,
        0.041425537809187,0.053145049844816,0.636502499121399,
        0.041425537809187,0.636502499121399,0.053145049844816,
        0.0,
        0.072157803838894,0.33333333333333333333,0.33333333333333333333,
        0.051608685267359,0.170569307751760,0.170569307751760,
        0.051608685267359,0.658861384496480,0.170569307751760,
        0.051608685267359,0.170569307751760,0.658861384496480,
        0.016229248811599,0.050547228317031,0.050547228317031,
        0.016229248811599,0.898905543365938,0.050547228317031,
        0.016229248811599,0.050547228317031,0.898905543365938,
        0.047545817133642,0.459292588292723,0.459292588292723,
        0.047545817133642,0.081414823414554,0.459292588292723,
        0.047545817133642,0.459292588292723,0.081414823414554,
        0.013615157087217,0.728492392955404,0.263112829634638,
        0.013615157087217,0.263112829634638,0.728492392955404,
        0.013615157087217,0.728492392955404,0.008394777409958,
        0.013615157087217,0.008394777409958,0.728492392955404,
        0.013615157087217,0.263112829634638,0.008394777409958,
        0.013615157087217,0.008394777409958,0.263112829634638,
        0.0,
        0.072157803838894,0.33333333333333333333,0.33333333333333333333,
        0.051608685267359,0.170569307751760,0.170569307751760,
        0.051608685267359,0.658861384496480,0.170569307751760,
        0.051608685267359,0.170569307751760,0.658861384496480,
        0.016229248811599,0.050547228317031,0.050547228317031,
        0.016229248811599,0.898905543365938,0.050547228317031,
        0.016229248811599,0.050547228317031,0.898905543365938,
        0.047545817133642,0.459292588292723,0.459292588292723,
        0.047545817133642,0.081414823414554,0.459292588292723,
        0.047545817133642,0.459292588292723,0.081414823414554,
        0.013615157087217,0.728492392955404,0.263112829634638,
        0.013615157087217,0.263112829634638,0.728492392955404,
        0.013615157087217,0.728492392955404,0.008394777409958,
        0.013615157087217,0.008394777409958,0.728492392955404,
        0.013615157087217,0.263112829634638,0.008394777409958,
        0.013615157087217,0.008394777409958,0.263112829634638,
        0.0,
        0.072157803838894,0.33333333333333333333,0.33333333333333333333,
        0.051608685267359,0.170569307751760,0.170569307751760,
        0.051608685267359,0.658861384496480,0.170569307751760,
        0.051608685267359,0.170569307751760,0.658861384496480,
        0.016229248811599,0.050547228317031,0.050547228317031,
        0.016229248811599,0.898905543365938,0.050547228317031,
        0.016229248811599,0.050547228317031,0.898905543365938,
        0.047545817133642,0.459292588292723,0.459292588292723,
        0.047545817133642,0.081414823414554,0.459292588292723,
        0.047545817133642,0.459292588292723,0.081414823414554,
        0.013615157087217,0.728492392955404,0.263112829634638,
        0.013615157087217,0.263112829634638,0.728492392955404,
        0.013615157087217,0.728492392955404,0.008394777409958,
        0.013615157087217,0.008394777409958,0.728492392955404,
        0.013615157087217,0.263112829634638,0.008394777409958,
        0.013615157087217,0.008394777409958,0.263112829634638,
        0.0,
    };
    return values + offset[order];
}
template<class TNode> void permutation_if_jac_neg(const Triangle &elem,TNode **nodes) {
    typedef typename TNode::T T;
    T reg0=nodes[1]->pos[0]-nodes[0]->pos[0]; T reg1=nodes[2]->pos[1]-nodes[0]->pos[1]; T reg2=nodes[1]->pos[1]-nodes[0]->pos[1]; T reg3=nodes[2]->pos[0]-nodes[0]->pos[0]; reg1=reg0*reg1;
    reg3=reg2*reg3; T det_jac=reg1-reg3;

    if ( det_jac < 0 ) {
        TNode *tmp[] = {nodes[0],nodes[2],nodes[1]};
        for(unsigned i=0;i<3;++i) nodes[i] = tmp[i];
    }
}
template<class PosNodes,class Pvec,class TVI> void get_var_inter_linear(const Triangle &elem,const PosNodes &pos_nodes,const Pvec &pos,TVI &var_inter) {
typedef typename Pvec::template SubType<0>::T T;
    T reg0=pos_nodes[2][1]-pos_nodes[0][1]; T reg1=pos_nodes[1][1]-pos_nodes[0][1]; T reg2=pos_nodes[2][0]-pos_nodes[0][0]; T reg3=pos_nodes[1][0]-pos_nodes[0][0]; T reg4=reg3*reg0;
    T reg5=reg2*reg1; reg5=reg4-reg5; T reg6=reg3/reg5; T reg7=reg2/reg5; T reg8=reg1/reg5;
    reg5=reg0/reg5; T reg9=pos[1]-pos_nodes[0][1]; T reg10=pos[0]-pos_nodes[0][0]; reg6=reg9*reg6; reg8=reg10*reg8;
    reg7=reg9*reg7; reg5=reg10*reg5; var_inter[0]=reg5-reg7; var_inter[1]=reg6-reg8;

}
template<class PosNodes,class Pvec,class TVI> void get_var_inter(const Triangle &elem,const PosNodes &pos_nodes,const Pvec &pos,TVI &var_inter) {
typedef typename Pvec::template SubType<0>::T T;
    T reg0=1-var_inter[0]; reg0=reg0-var_inter[1]; T reg1=pos_nodes[1][0]*var_inter[0]; T reg2=pos_nodes[0][0]*reg0; T reg3=pos_nodes[2][0]-pos_nodes[0][0];
    T reg4=pos_nodes[1][0]-pos_nodes[0][0]; T reg5=pos_nodes[1][1]-pos_nodes[0][1]; T reg6=pos_nodes[2][1]-pos_nodes[0][1]; T reg7=pos_nodes[0][1]*reg0; T reg8=var_inter[0]*pos_nodes[1][1];
    T reg9=pos_nodes[2][1]*var_inter[1]; reg1=reg2+reg1; reg2=reg3*reg5; T reg10=reg4*reg6; reg8=reg7+reg8;
    reg7=pos_nodes[2][0]*var_inter[1]; reg1=reg7+reg1; reg8=reg9+reg8; reg2=reg10-reg2; reg7=reg6/reg2;
    reg1=pos[0]-reg1; reg9=reg4/reg2; T reg11=reg3/reg2; reg8=pos[1]-reg8; reg2=reg5/reg2;
    reg9=reg9*reg8; reg2=reg2*reg1; reg11=reg11*reg8; reg7=reg7*reg1; var_inter[1]+=reg9-reg2;
    var_inter[0]+=reg7-reg11;

}
template<> struct ElemVarInterFromPosNonLinear<Triangle> { static const bool res = 0; };
#ifndef STRUCT_Global
#define STRUCT_Global
struct Global {};
#endif // STRUCT_Global
template<class TVI,class TVAL,class T> void get_interp(const Triangle &ne,const Global &n,const TVI &var_inter,const TVAL &val,T &res) {
    res=val[0];

}
#ifndef STRUCT_Der_nodal
#define STRUCT_Der_nodal
struct Der_nodal {};
#endif // STRUCT_Der_nodal
template<class TVI,class TVAL,class T> void get_interp(const Triangle &ne,const Der_nodal &n,const TVI &var_inter,const TVAL &val,T &res) {
    res=val[0];

}
#ifndef STRUCT_Nodal_3
#define STRUCT_Nodal_3
struct Nodal_3 {};
#endif // STRUCT_Nodal_3
template<class TVI,class TVAL,class T> void get_interp(const Triangle &ne,const Nodal_3 &n,const TVI &var_inter,const TVAL &val,T &res) {
    T reg0=1-var_inter[0]; reg0=reg0-var_inter[1]; T reg1=val[0]*reg0; T reg2=val[1]*var_inter[0]; reg1=reg1+reg2;
    T reg3=val[2]*var_inter[1]; res=reg1+reg3;

}
#ifndef STRUCT_Elementary
#define STRUCT_Elementary
struct Elementary {};
#endif // STRUCT_Elementary
template<class TVI,class TVAL,class T> void get_interp(const Triangle &ne,const Elementary &n,const TVI &var_inter,const TVAL &val,T &res) {
    res=val[0];

}
#ifndef STRUCT_Nodal
#define STRUCT_Nodal
struct Nodal {};
#endif // STRUCT_Nodal
template<class TVI,class TVAL,class T> void get_interp(const Triangle &ne,const Nodal &n,const TVI &var_inter,const TVAL &val,T &res) {
    T reg0=1-var_inter[0]; reg0=reg0-var_inter[1]; T reg1=val[0]*reg0; T reg2=val[1]*var_inter[0]; reg1=reg1+reg2;
    T reg3=val[2]*var_inter[1]; res=reg1+reg3;

}
#ifndef STRUCT_Skin_elementary
#define STRUCT_Skin_elementary
struct Skin_elementary {};
#endif // STRUCT_Skin_elementary
template<class TVI,class TVAL,class T> void get_interp(const Triangle &ne,const Skin_elementary &n,const TVI &var_inter,const TVAL &val,T &res) {
    res=val[0];

}
#ifndef STRUCT_Bubble
#define STRUCT_Bubble
struct Bubble {};
#endif // STRUCT_Bubble
template<class TVI,class TVAL,class T> void get_interp(const Triangle &ne,const Bubble &n,const TVI &var_inter,const TVAL &val,T &res) {
    T reg0=1-var_inter[0]; reg0=reg0-var_inter[1]; T reg1=var_inter[0]*reg0; reg1=var_inter[1]*reg1; reg1=81*reg1;
    T reg2=1-reg1; T reg3=var_inter[0]*reg2; T reg4=reg0*reg2; reg4=val[0]*reg4; reg2=var_inter[1]*reg2;
    reg3=val[1]*reg3; reg2=val[2]*reg2; reg4=reg3+reg4; reg1=val[3]*reg1; reg2=reg4+reg2;
    res=reg2+reg1;

}
#ifndef STRUCT_Elementary_mul_nb_nodes_of_each_children_elem
#define STRUCT_Elementary_mul_nb_nodes_of_each_children_elem
struct Elementary_mul_nb_nodes_of_each_children_elem {};
#endif // STRUCT_Elementary_mul_nb_nodes_of_each_children_elem
template<class TVI,class TVAL,class T> void get_interp(const Triangle &ne,const Elementary_mul_nb_nodes_of_each_children_elem &n,const TVI &var_inter,const TVAL &val,T &res) {
    res=val[0];

}
#ifndef STRUCT_Gauss_0
#define STRUCT_Gauss_0
struct Gauss_0 {};
#endif // STRUCT_Gauss_0
template<class TVI,class TVAL,class T> void get_interp(const Triangle &ne,const Gauss_0 &n,const TVI &var_inter,const TVAL &val,T &res) {
    res=val[0];

}
#ifndef STRUCT_Gauss
#define STRUCT_Gauss
struct Gauss {};
#endif // STRUCT_Gauss
template<class TVI,class TVAL,class T> void get_interp(const Triangle &ne,const Gauss &n,const TVI &var_inter,const TVAL &val,T &res) {
    res=val[0]+val[1];

}
#ifndef STRUCT_Gauss_2
#define STRUCT_Gauss_2
struct Gauss_2 {};
#endif // STRUCT_Gauss_2
template<class TVI,class TVAL,class T> void get_interp(const Triangle &ne,const Gauss_2 &n,const TVI &var_inter,const TVAL &val,T &res) {
    res=val[0]+val[1];

}
#ifndef STRUCT_P_plus_7
#define STRUCT_P_plus_7
struct P_plus_7 {};
#endif // STRUCT_P_plus_7
template<class TVI,class TVAL,class T> void get_interp(const Triangle &ne,const P_plus_7 &n,const TVI &var_inter,const TVAL &val,T &res) {
    T reg0=val[1]*var_inter[0]; T reg1=val[0]+reg0; T reg2=pow(var_inter[0],2); T reg3=val[2]*var_inter[1]; T reg4=var_inter[0]*var_inter[1];
    T reg5=val[3]*reg2; reg1=reg1+reg3; reg4=val[4]*reg4; T reg6=pow(var_inter[1],2); reg5=reg1+reg5;
    reg4=reg5+reg4; reg1=val[5]*reg6; reg5=pow(var_inter[0],3); reg1=reg4+reg1; reg4=val[6]*reg5;
    T reg7=var_inter[1]*reg2; T reg8=var_inter[0]*reg6; reg4=reg1+reg4; reg7=val[7]*reg7; reg1=pow(var_inter[1],3);
    reg8=val[8]*reg8; reg7=reg4+reg7; reg4=pow(var_inter[0],4); T reg9=val[9]*reg1; reg8=reg7+reg8;
    reg9=reg8+reg9; reg7=val[10]*reg4; reg8=var_inter[1]*reg5; reg7=reg9+reg7; reg8=val[11]*reg8;
    reg9=reg2*reg6; reg8=reg7+reg8; reg9=val[12]*reg9; reg7=var_inter[0]*reg1; reg7=val[13]*reg7;
    T reg10=pow(var_inter[1],4); reg9=reg8+reg9; reg7=reg9+reg7; reg8=val[14]*reg10; reg9=pow(var_inter[0],5);
    reg8=reg7+reg8; reg7=val[15]*reg9; T reg11=var_inter[1]*reg4; reg7=reg8+reg7; reg11=val[16]*reg11;
    reg8=reg6*reg5; reg11=reg7+reg11; reg8=val[17]*reg8; reg7=reg2*reg1; reg8=reg11+reg8;
    reg7=val[18]*reg7; reg11=var_inter[0]*reg10; reg7=reg8+reg7; reg11=val[19]*reg11; reg8=pow(var_inter[1],5);
    reg11=reg7+reg11; reg7=val[20]*reg8; T reg12=pow(var_inter[0],6); reg7=reg11+reg7; reg11=val[21]*reg12;
    T reg13=var_inter[1]*reg9; T reg14=reg6*reg4; reg13=val[22]*reg13; reg11=reg7+reg11; reg7=reg5*reg1;
    reg14=val[23]*reg14; reg13=reg11+reg13; reg14=reg13+reg14; reg7=val[24]*reg7; reg11=reg2*reg10;
    reg13=var_inter[0]*reg8; reg7=reg14+reg7; reg11=val[25]*reg11; reg14=pow(var_inter[1],6); reg13=val[26]*reg13;
    reg11=reg7+reg11; reg7=val[27]*reg14; reg13=reg11+reg13; reg11=pow(var_inter[0],7); T reg15=var_inter[1]*reg12;
    reg7=reg13+reg7; reg13=val[28]*reg11; reg13=reg7+reg13; reg15=val[29]*reg15; reg7=reg6*reg9;
    reg15=reg13+reg15; reg7=val[30]*reg7; reg13=reg1*reg4; reg7=reg15+reg7; reg13=val[31]*reg13;
    reg15=reg5*reg10; T reg16=reg2*reg8; reg15=val[32]*reg15; reg13=reg7+reg13; reg15=reg13+reg15;
    reg16=val[33]*reg16; reg7=var_inter[0]*reg14; reg16=reg15+reg16; reg7=val[34]*reg7; reg13=pow(var_inter[1],7);
    reg7=reg16+reg7; reg15=val[35]*reg13; reg15=reg7+reg15; res=reg15;

}
#ifndef STRUCT_P_plus_2
#define STRUCT_P_plus_2
struct P_plus_2 {};
#endif // STRUCT_P_plus_2
template<class TVI,class TVAL,class T> void get_interp(const Triangle &ne,const P_plus_2 &n,const TVI &var_inter,const TVAL &val,T &res) {
    T reg0=val[1]*var_inter[0]; T reg1=val[2]*var_inter[1]; T reg2=pow(var_inter[0],2); T reg3=val[0]+reg0; reg3=reg3+reg1;
    T reg4=val[3]*reg2; T reg5=var_inter[0]*var_inter[1]; reg4=reg3+reg4; reg5=val[4]*reg5; reg3=pow(var_inter[1],2);
    reg5=reg4+reg5; reg4=val[5]*reg3; reg4=reg5+reg4; res=reg4;

}
#ifndef STRUCT_P_plus_3
#define STRUCT_P_plus_3
struct P_plus_3 {};
#endif // STRUCT_P_plus_3
template<class TVI,class TVAL,class T> void get_interp(const Triangle &ne,const P_plus_3 &n,const TVI &var_inter,const TVAL &val,T &res) {
    T reg0=val[1]*var_inter[0]; T reg1=val[2]*var_inter[1]; T reg2=pow(var_inter[0],2); T reg3=val[0]+reg0; reg3=reg3+reg1;
    T reg4=val[3]*reg2; T reg5=var_inter[0]*var_inter[1]; reg4=reg3+reg4; reg5=val[4]*reg5; reg3=pow(var_inter[1],2);
    T reg6=pow(var_inter[0],3); T reg7=val[5]*reg3; reg5=reg4+reg5; reg7=reg5+reg7; reg4=val[6]*reg6;
    reg5=var_inter[1]*reg2; T reg8=var_inter[0]*reg3; reg5=val[7]*reg5; reg4=reg7+reg4; reg5=reg4+reg5;
    reg8=val[8]*reg8; reg4=pow(var_inter[1],3); reg8=reg5+reg8; reg5=val[9]*reg4; reg5=reg8+reg5;
    res=reg5;

}
#ifndef STRUCT_P_plus_0
#define STRUCT_P_plus_0
struct P_plus_0 {};
#endif // STRUCT_P_plus_0
template<class TVI,class TVAL,class T> void get_interp(const Triangle &ne,const P_plus_0 &n,const TVI &var_inter,const TVAL &val,T &res) {
    res=val[0];

}
#ifndef STRUCT_P_plus_1
#define STRUCT_P_plus_1
struct P_plus_1 {};
#endif // STRUCT_P_plus_1
template<class TVI,class TVAL,class T> void get_interp(const Triangle &ne,const P_plus_1 &n,const TVI &var_inter,const TVAL &val,T &res) {
    T reg0=val[1]*var_inter[0]; T reg1=val[0]+reg0; T reg2=val[2]*var_inter[1]; reg1=reg1+reg2; res=reg1;

}
#ifndef STRUCT_P_plus_6
#define STRUCT_P_plus_6
struct P_plus_6 {};
#endif // STRUCT_P_plus_6
template<class TVI,class TVAL,class T> void get_interp(const Triangle &ne,const P_plus_6 &n,const TVI &var_inter,const TVAL &val,T &res) {
    T reg0=val[1]*var_inter[0]; T reg1=val[0]+reg0; T reg2=pow(var_inter[0],2); T reg3=val[2]*var_inter[1]; T reg4=var_inter[0]*var_inter[1];
    T reg5=val[3]*reg2; reg1=reg1+reg3; reg4=val[4]*reg4; T reg6=pow(var_inter[1],2); reg5=reg1+reg5;
    reg4=reg5+reg4; reg1=val[5]*reg6; reg5=pow(var_inter[0],3); T reg7=var_inter[1]*reg2; T reg8=val[6]*reg5;
    reg1=reg4+reg1; reg7=val[7]*reg7; reg8=reg1+reg8; reg1=var_inter[0]*reg6; reg4=pow(var_inter[1],3);
    reg1=val[8]*reg1; reg7=reg8+reg7; reg8=val[9]*reg4; reg1=reg7+reg1; reg7=pow(var_inter[0],4);
    reg8=reg1+reg8; reg1=val[10]*reg7; T reg9=var_inter[1]*reg5; reg9=val[11]*reg9; reg1=reg8+reg1;
    reg8=reg2*reg6; reg9=reg1+reg9; reg1=var_inter[0]*reg4; reg8=val[12]*reg8; T reg10=pow(var_inter[1],4);
    reg1=val[13]*reg1; reg8=reg9+reg8; reg1=reg8+reg1; reg8=val[14]*reg10; reg9=pow(var_inter[0],5);
    T reg11=val[15]*reg9; T reg12=var_inter[1]*reg7; reg8=reg1+reg8; reg11=reg8+reg11; reg12=val[16]*reg12;
    reg1=reg6*reg5; reg12=reg11+reg12; reg1=val[17]*reg1; reg8=reg2*reg4; reg1=reg12+reg1;
    reg8=val[18]*reg8; reg11=var_inter[0]*reg10; reg8=reg1+reg8; reg11=val[19]*reg11; reg1=pow(var_inter[1],5);
    reg12=pow(var_inter[0],6); reg11=reg8+reg11; reg8=val[20]*reg1; T reg13=val[21]*reg12; reg8=reg11+reg8;
    reg11=var_inter[1]*reg9; reg11=val[22]*reg11; T reg14=reg6*reg7; reg13=reg8+reg13; reg8=reg5*reg4;
    reg14=val[23]*reg14; reg11=reg13+reg11; reg14=reg11+reg14; reg8=val[24]*reg8; reg11=reg2*reg10;
    reg8=reg14+reg8; reg11=val[25]*reg11; reg13=var_inter[0]*reg1; reg11=reg8+reg11; reg13=val[26]*reg13;
    reg8=pow(var_inter[1],6); reg13=reg11+reg13; reg11=val[27]*reg8; reg11=reg13+reg11; res=reg11;

}
#ifndef STRUCT_Elementary_mul_nb_nodes
#define STRUCT_Elementary_mul_nb_nodes
struct Elementary_mul_nb_nodes {};
#endif // STRUCT_Elementary_mul_nb_nodes
template<class TVI,class TVAL,class T> void get_interp(const Triangle &ne,const Elementary_mul_nb_nodes &n,const TVI &var_inter,const TVAL &val,T &res) {
    res=val[0];

}
#ifndef STRUCT_P_plus_4
#define STRUCT_P_plus_4
struct P_plus_4 {};
#endif // STRUCT_P_plus_4
template<class TVI,class TVAL,class T> void get_interp(const Triangle &ne,const P_plus_4 &n,const TVI &var_inter,const TVAL &val,T &res) {
    T reg0=val[1]*var_inter[0]; T reg1=val[2]*var_inter[1]; T reg2=pow(var_inter[0],2); T reg3=val[0]+reg0; T reg4=var_inter[0]*var_inter[1];
    reg3=reg3+reg1; T reg5=val[3]*reg2; T reg6=pow(var_inter[1],2); reg4=val[4]*reg4; reg5=reg3+reg5;
    reg3=pow(var_inter[0],3); T reg7=val[5]*reg6; reg4=reg5+reg4; reg5=val[6]*reg3; T reg8=var_inter[1]*reg2;
    reg7=reg4+reg7; reg5=reg7+reg5; reg8=val[7]*reg8; reg4=var_inter[0]*reg6; reg7=pow(var_inter[1],3);
    reg4=val[8]*reg4; reg8=reg5+reg8; reg4=reg8+reg4; reg5=val[9]*reg7; reg8=pow(var_inter[0],4);
    reg5=reg4+reg5; reg4=val[10]*reg8; T reg9=var_inter[1]*reg3; T reg10=reg2*reg6; reg9=val[11]*reg9;
    reg4=reg5+reg4; reg9=reg4+reg9; reg10=val[12]*reg10; reg4=var_inter[0]*reg7; reg10=reg9+reg10;
    reg4=val[13]*reg4; reg5=pow(var_inter[1],4); reg4=reg10+reg4; reg9=val[14]*reg5; reg9=reg4+reg9;
    res=reg9;

}
#ifndef STRUCT_P_plus_5
#define STRUCT_P_plus_5
struct P_plus_5 {};
#endif // STRUCT_P_plus_5
template<class TVI,class TVAL,class T> void get_interp(const Triangle &ne,const P_plus_5 &n,const TVI &var_inter,const TVAL &val,T &res) {
    T reg0=val[1]*var_inter[0]; T reg1=val[2]*var_inter[1]; T reg2=pow(var_inter[0],2); T reg3=val[0]+reg0; T reg4=var_inter[0]*var_inter[1];
    T reg5=val[3]*reg2; reg3=reg3+reg1; T reg6=pow(var_inter[1],2); reg4=val[4]*reg4; reg5=reg3+reg5;
    reg3=pow(var_inter[0],3); T reg7=val[5]*reg6; reg4=reg5+reg4; reg5=var_inter[1]*reg2; T reg8=val[6]*reg3;
    reg7=reg4+reg7; reg4=var_inter[0]*reg6; reg5=val[7]*reg5; reg8=reg7+reg8; reg4=val[8]*reg4;
    reg5=reg8+reg5; reg7=pow(var_inter[1],3); reg8=pow(var_inter[0],4); T reg9=val[9]*reg7; reg4=reg5+reg4;
    reg5=val[10]*reg8; T reg10=var_inter[1]*reg3; reg9=reg4+reg9; reg5=reg9+reg5; reg10=val[11]*reg10;
    reg4=reg2*reg6; reg9=var_inter[0]*reg7; reg10=reg5+reg10; reg4=val[12]*reg4; reg5=pow(var_inter[1],4);
    reg9=val[13]*reg9; reg4=reg10+reg4; reg10=pow(var_inter[0],5); reg9=reg4+reg9; reg4=val[14]*reg5;
    T reg11=val[15]*reg10; T reg12=var_inter[1]*reg8; reg4=reg9+reg4; reg11=reg4+reg11; reg12=val[16]*reg12;
    reg4=reg6*reg3; reg9=reg2*reg7; reg4=val[17]*reg4; reg12=reg11+reg12; reg4=reg12+reg4;
    reg9=val[18]*reg9; reg11=var_inter[0]*reg5; reg9=reg4+reg9; reg11=val[19]*reg11; reg4=pow(var_inter[1],5);
    reg11=reg9+reg11; reg9=val[20]*reg4; reg9=reg11+reg9; res=reg9;

}
#ifndef STRUCT_P_plus_8
#define STRUCT_P_plus_8
struct P_plus_8 {};
#endif // STRUCT_P_plus_8
template<class TVI,class TVAL,class T> void get_interp(const Triangle &ne,const P_plus_8 &n,const TVI &var_inter,const TVAL &val,T &res) {
    T reg0=val[1]*var_inter[0]; T reg1=pow(var_inter[0],2); T reg2=val[0]+reg0; T reg3=val[2]*var_inter[1]; reg2=reg2+reg3;
    T reg4=val[3]*reg1; T reg5=var_inter[0]*var_inter[1]; reg4=reg2+reg4; reg5=val[4]*reg5; reg2=pow(var_inter[1],2);
    reg5=reg4+reg5; reg4=val[5]*reg2; T reg6=pow(var_inter[0],3); reg4=reg5+reg4; reg5=val[6]*reg6;
    T reg7=var_inter[1]*reg1; reg7=val[7]*reg7; reg5=reg4+reg5; reg4=var_inter[0]*reg2; T reg8=pow(var_inter[1],3);
    reg7=reg5+reg7; reg4=val[8]*reg4; reg5=pow(var_inter[0],4); T reg9=val[9]*reg8; reg4=reg7+reg4;
    reg7=val[10]*reg5; reg9=reg4+reg9; reg4=var_inter[1]*reg6; T reg10=reg1*reg2; reg7=reg9+reg7;
    reg4=val[11]*reg4; reg9=var_inter[0]*reg8; reg10=val[12]*reg10; reg4=reg7+reg4; reg10=reg4+reg10;
    reg9=val[13]*reg9; reg4=pow(var_inter[1],4); reg9=reg10+reg9; reg7=val[14]*reg4; reg10=pow(var_inter[0],5);
    reg7=reg9+reg7; reg9=val[15]*reg10; T reg11=var_inter[1]*reg5; reg9=reg7+reg9; reg11=val[16]*reg11;
    reg7=reg2*reg6; reg11=reg9+reg11; reg7=val[17]*reg7; reg9=reg1*reg8; reg7=reg11+reg7;
    reg11=var_inter[0]*reg4; reg9=val[18]*reg9; reg9=reg7+reg9; reg11=val[19]*reg11; reg7=pow(var_inter[1],5);
    reg11=reg9+reg11; reg9=val[20]*reg7; T reg12=pow(var_inter[0],6); reg9=reg11+reg9; reg11=val[21]*reg12;
    T reg13=var_inter[1]*reg10; reg11=reg9+reg11; reg9=reg2*reg5; reg13=val[22]*reg13; reg13=reg11+reg13;
    reg9=val[23]*reg9; reg11=reg6*reg8; T reg14=reg1*reg4; reg11=val[24]*reg11; reg9=reg13+reg9;
    reg11=reg9+reg11; reg14=val[25]*reg14; reg9=var_inter[0]*reg7; reg14=reg11+reg14; reg9=val[26]*reg9;
    reg11=pow(var_inter[1],6); reg13=pow(var_inter[0],7); T reg15=val[27]*reg11; reg9=reg14+reg9; reg14=var_inter[1]*reg12;
    T reg16=val[28]*reg13; reg15=reg9+reg15; reg9=reg2*reg10; reg14=val[29]*reg14; reg16=reg15+reg16;
    reg15=reg8*reg5; reg9=val[30]*reg9; reg14=reg16+reg14; reg15=val[31]*reg15; reg9=reg14+reg9;
    reg14=reg6*reg4; reg15=reg9+reg15; reg14=val[32]*reg14; reg9=reg1*reg7; reg14=reg15+reg14;
    reg9=val[33]*reg9; reg15=var_inter[0]*reg11; reg16=pow(var_inter[1],7); reg9=reg14+reg9; reg15=val[34]*reg15;
    reg15=reg9+reg15; reg9=val[35]*reg16; reg14=pow(var_inter[0],8); reg9=reg15+reg9; reg14=val[36]*reg14;
    reg13=var_inter[1]*reg13; reg12=reg2*reg12; reg13=val[37]*reg13; reg14=reg9+reg14; reg12=val[38]*reg12;
    reg13=reg14+reg13; reg10=reg8*reg10; reg10=val[39]*reg10; reg4=reg5*reg4; reg12=reg13+reg12;
    reg4=val[40]*reg4; reg7=reg6*reg7; reg10=reg12+reg10; reg7=val[41]*reg7; reg11=reg1*reg11;
    reg4=reg10+reg4; reg7=reg4+reg7; reg11=val[42]*reg11; reg16=var_inter[0]*reg16; reg11=reg7+reg11;
    reg16=val[43]*reg16; reg1=pow(var_inter[1],8); reg16=reg11+reg16; reg1=val[44]*reg1; res=reg16+reg1;

}
#ifndef STRUCT_Flat_interpolation
#define STRUCT_Flat_interpolation
struct Flat_interpolation {};
#endif // STRUCT_Flat_interpolation
template<class TVI,class TVAL,class T> void get_interp(const Triangle &ne,const Flat_interpolation &n,const TVI &var_inter,const TVAL &val,T &res) {
    res=val[0];

}
template<class TVI,class TVAL> void get_shape_functions(const Triangle &ne,const TVI &var_inter,TVAL &res) {
    typedef typename TVAL::template SubType<0>::T T;
    res[1]=var_inter[0]; res[2]=var_inter[1]; T reg0=1-var_inter[0]; reg0=reg0-var_inter[1]; res[0]=reg0;

}
#ifndef AUTORIZEDPERM
#define AUTORIZEDPERM
template<class T> struct AuthorizedPerm;
#endif // AUTORIZEDPERM
template<> struct AuthorizedPerm<Triangle> {
    static const unsigned nb_permutations = 2;
    template<class TE> static void make_permutation( TE &e, unsigned nb_perm ) {
        if ( nb_perm == 0 ) {
            typename TE::TNode *tmp[] = {e.nodes[1],e.nodes[2],e.nodes[0]};
            for(unsigned i=0;i<3;++i) e.nodes[i] = tmp[i];
            return;
        }
        if ( nb_perm == 1 ) {
            typename TE::TNode *tmp[] = {e.nodes[2],e.nodes[0],e.nodes[1]};
            for(unsigned i=0;i<3;++i) e.nodes[i] = tmp[i];
            return;
        }
    }
    template<class TE> static void unmake_permutation( TE &e, unsigned nb_perm ) {
        if ( nb_perm == 0 ) {
            typename TE::TNode *tmp[] = {e.nodes[2],e.nodes[0],e.nodes[1]};
            for(unsigned i=0;i<3;++i) e.nodes[i] = tmp[i];
            return;
        }
        if ( nb_perm == 1 ) {
            typename TE::TNode *tmp[] = {e.nodes[1],e.nodes[2],e.nodes[0]};
            for(unsigned i=0;i<3;++i) e.nodes[i] = tmp[i];
            return;
        }
    }
};

template<class TN,class TNG,class TD,unsigned NET,class TVI>
typename TNG::T get_det_jac( const Element<Triangle,TN,TNG,TD,NET> &elem, const TVI &var_inter ) {
    typedef typename TNG::T T;
    T reg0=elem.pos(1)[0]-elem.pos(0)[0]; T reg1=elem.pos(1)[1]-elem.pos(0)[1]; T reg2=elem.pos(1)[2]-elem.pos(0)[2]; T reg3=pow(reg0,2); T reg4=pow(reg1,2);
    T reg5=pow(reg2,2); reg4=reg3+reg4; reg5=reg4+reg5; reg5=pow(reg5,0.5); reg3=elem.pos(2)[1]-elem.pos(0)[1];
    reg4=reg0/reg5; T reg6=elem.pos(2)[0]-elem.pos(0)[0]; T reg7=reg1/reg5; reg5=reg2/reg5; T reg8=reg7*reg3;
    T reg9=reg6*reg4; T reg10=elem.pos(2)[2]-elem.pos(0)[2]; T reg11=reg5*reg10; reg9=reg8+reg9; reg11=reg9+reg11;
    reg8=reg7*reg11; reg9=reg4*reg11; T reg12=reg5*reg11; reg9=reg6-reg9; reg8=reg3-reg8;
    T reg13=pow(reg8,2); T reg14=pow(reg9,2); reg12=reg10-reg12; T reg15=pow(reg12,2); reg13=reg14+reg13;
    reg15=reg13+reg15; reg15=pow(reg15,0.5); reg9=reg9/reg15; reg8=reg8/reg15; reg13=reg1*reg8;
    reg14=reg0*reg9; reg15=reg12/reg15; reg0=reg4*reg0; reg8=reg3*reg8; reg9=reg6*reg9;
    reg1=reg7*reg1; reg13=reg14+reg13; reg1=reg0+reg1; reg5=reg5*reg2; reg2=reg15*reg2;
    reg10=reg15*reg10; reg8=reg9+reg8; reg13=reg2+reg13; reg10=reg8+reg10; reg5=reg1+reg5;
    reg0=reg13*reg11; reg1=reg5*reg10; reg0=reg1-reg0; return reg0;

}
template<class TN,class T,class TNodalStaticData,class TD,unsigned NET>
Vec<T,2> barycenter( const Element<Triangle,TN,Node<2,T,TNodalStaticData>,TD,NET> &elem ) {
    Vec<T,2> res;
    T reg0=0.33333333333333331483*elem.pos(1)[1]; T reg1=0.33333333333333337034*elem.pos(0)[1]; T reg2=0.33333333333333331483*elem.pos(1)[0]; T reg3=0.33333333333333337034*elem.pos(0)[0]; T reg4=elem.pos(2)[1]-elem.pos(0)[1];
    T reg5=elem.pos(1)[0]-elem.pos(0)[0]; T reg6=elem.pos(1)[1]-elem.pos(0)[1]; T reg7=elem.pos(2)[0]-elem.pos(0)[0]; T reg8=reg6*reg7; T reg9=reg5*reg4;
    reg2=reg3+reg2; reg3=0.33333333333333331483*elem.pos(2)[0]; reg0=reg1+reg0; reg1=0.33333333333333331483*elem.pos(2)[1]; reg0=reg1+reg0;
    reg8=reg9-reg8; reg3=reg2+reg3; reg0=reg8*reg0; reg3=reg8*reg3; reg0=0.5*reg0;
    reg3=0.5*reg3; reg1=0.5*reg8; res[1]=reg0/reg1; res[0]=reg3/reg1;

    return res;
}
template<class TN,class T,class TNodalStaticData,class TD,unsigned NET>
Vec<T,3> barycenter( const Element<Triangle,TN,Node<3,T,TNodalStaticData>,TD,NET> &elem ) {
    Vec<T,3> res;
    T reg0=elem.pos(1)[1]-elem.pos(0)[1]; T reg1=elem.pos(1)[0]-elem.pos(0)[0]; T reg2=pow(reg1,2); T reg3=elem.pos(1)[2]-elem.pos(0)[2]; T reg4=pow(reg0,2);
    reg4=reg2+reg4; reg2=pow(reg3,2); reg2=reg4+reg2; reg2=pow(reg2,0.5); reg4=reg0/reg2;
    T reg5=elem.pos(2)[0]-elem.pos(0)[0]; T reg6=elem.pos(2)[1]-elem.pos(0)[1]; T reg7=reg1/reg2; reg2=reg3/reg2; T reg8=elem.pos(2)[2]-elem.pos(0)[2];
    T reg9=reg7*reg5; T reg10=reg4*reg6; reg10=reg9+reg10; reg9=reg2*reg8; reg9=reg10+reg9;
    reg10=reg4*reg9; T reg11=reg7*reg9; T reg12=reg2*reg9; reg11=reg5-reg11; reg10=reg6-reg10;
    reg12=reg8-reg12; T reg13=pow(reg11,2); T reg14=pow(reg10,2); reg14=reg13+reg14; reg13=pow(reg12,2);
    reg13=reg14+reg13; reg13=pow(reg13,0.5); reg10=reg10/reg13; reg11=reg11/reg13; reg13=reg12/reg13;
    reg7=reg1*reg7; reg4=reg0*reg4; reg6=reg6*reg10; reg5=reg5*reg11; reg11=reg1*reg11;
    reg10=reg0*reg10; reg8=reg8*reg13; reg6=reg5+reg6; reg13=reg3*reg13; reg10=reg11+reg10;
    reg2=reg3*reg2; reg4=reg7+reg4; reg0=0.33333333333333331483*elem.pos(1)[2]; reg1=0.33333333333333337034*elem.pos(0)[2]; reg3=0.33333333333333331483*elem.pos(1)[1];
    reg5=0.33333333333333337034*elem.pos(0)[1]; reg7=0.33333333333333337034*elem.pos(0)[0]; reg11=0.33333333333333331483*elem.pos(1)[0]; reg13=reg10+reg13; reg2=reg4+reg2;
    reg8=reg6+reg8; reg0=reg1+reg0; reg1=0.33333333333333331483*elem.pos(2)[1]; reg3=reg5+reg3; reg4=reg2*reg8;
    reg5=0.33333333333333331483*elem.pos(2)[2]; reg7=reg11+reg7; reg6=reg9*reg13; reg10=0.33333333333333331483*elem.pos(2)[0]; reg5=reg0+reg5;
    reg7=reg10+reg7; reg1=reg3+reg1; reg6=reg4-reg6; reg5=reg6*reg5; reg7=reg6*reg7;
    reg1=reg6*reg1; reg5=0.5*reg5; reg1=0.5*reg1; reg7=0.5*reg7; reg0=0.5*reg6;
    res[2]=reg5/reg0; res[1]=reg1/reg0; res[0]=reg7/reg0;

    return res;
}
}
#endif // LMT_TRIANGLE
