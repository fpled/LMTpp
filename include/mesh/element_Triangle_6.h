#ifndef LMT_TRIANGLE_6
#define LMT_TRIANGLE_6
#include "node.h"
namespace LMT {
inline const double *gauss_point_for_order(unsigned order, const Triangle_6 &elem) { /// order -> degre du polynome a integrer exactement
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
template<class TNode> void permutation_if_jac_neg(const Triangle_6 &elem,TNode **nodes) {
    typedef typename TNode::T T;
    T reg0=nodes[0]->pos[0]+nodes[2]->pos[0]; T reg1=nodes[1]->pos[1]+nodes[0]->pos[1]; T reg2=2*nodes[3]->pos[1]; T reg3=nodes[2]->pos[1]+nodes[0]->pos[1]; T reg4=2*nodes[3]->pos[0];
    T reg5=nodes[0]->pos[0]+nodes[1]->pos[0]; T reg6=2*nodes[4]->pos[0]; reg3=reg3-reg2; T reg7=2*nodes[4]->pos[1]; reg0=reg0-reg4;
    reg5=reg5-reg4; reg1=reg1-reg2; reg5=reg5+reg6; T reg8=2*nodes[5]->pos[0]; reg3=reg3+reg7;
    T reg9=2*nodes[5]->pos[1]; reg0=reg6+reg0; reg1=reg7+reg1; reg0=reg0-reg8; reg1=reg1-reg9;
    reg3=reg3-reg9; reg5=reg5-reg8; reg3=reg5*reg3; reg0=reg1*reg0; T det_jac=reg3-reg0;

    if ( det_jac < 0 ) {
        TNode *tmp[] = {nodes[5],nodes[4],nodes[3],nodes[2],nodes[1],nodes[0]};
        for(unsigned i=0;i<6;++i) nodes[i] = tmp[i];
    }
}
template<class PosNodes,class Pvec,class TVI> void get_var_inter_linear(const Triangle_6 &elem,const PosNodes &pos_nodes,const Pvec &pos,TVI &var_inter) {
typedef typename Pvec::template SubType<0>::T T;
    T reg0=1-var_inter[0]; reg0=reg0-var_inter[1]; T reg1=2*var_inter[0]; T reg2=2*reg0; T reg3=2*var_inter[1];
    T reg4=reg3-1; T reg5=reg1-1; T reg6=reg2-1; T reg7=4*var_inter[0]; T reg8=4*reg0;
    reg3=reg4+reg3; reg1=reg1+reg5; reg2=reg2+reg6; T reg9=reg3*pos_nodes[2][1]; T reg10=pos_nodes[0][1]*reg2;
    T reg11=pos_nodes[1][1]*reg1; reg6=reg0*reg6; T reg12=reg3*pos_nodes[2][0]; reg5=reg5*var_inter[0]; T reg13=pos_nodes[0][0]*reg2;
    T reg14=pos_nodes[1][0]*reg1; T reg15=reg8-reg7; T reg16=4*var_inter[1]; reg4=var_inter[1]*reg4; T reg17=pos_nodes[1][1]*reg5;
    T reg18=reg6*pos_nodes[0][1]; T reg19=reg7*pos_nodes[3][1]; reg9=reg9-reg10; T reg20=reg5*pos_nodes[1][0]; T reg21=reg6*pos_nodes[0][0];
    T reg22=reg7*pos_nodes[3][0]; reg12=reg12-reg13; reg10=reg11-reg10; reg11=reg15*pos_nodes[3][1]; reg13=reg14-reg13;
    reg14=reg15*pos_nodes[3][0]; reg20=reg21+reg20; reg21=reg4*pos_nodes[2][0]; T reg23=reg7*pos_nodes[4][0]; reg22=reg12-reg22;
    reg12=reg8-reg16; reg8=reg8*var_inter[0]; reg17=reg18+reg17; reg18=reg16*pos_nodes[4][1]; reg11=reg10+reg11;
    reg10=reg4*pos_nodes[2][1]; T reg24=reg16*pos_nodes[4][0]; reg14=reg13+reg14; reg13=reg7*pos_nodes[4][1]; reg19=reg9-reg19;
    reg20=reg21+reg20; reg9=reg8*pos_nodes[3][0]; reg21=var_inter[1]*reg7; reg23=reg22+reg23; reg22=reg12*pos_nodes[5][0];
    T reg25=reg16*pos_nodes[5][0]; reg24=reg14+reg24; reg14=reg16*pos_nodes[5][1]; T reg26=reg12*pos_nodes[5][1]; reg18=reg11+reg18;
    reg13=reg19+reg13; reg11=reg8*pos_nodes[3][1]; reg17=reg10+reg17; reg10=reg21*pos_nodes[4][1]; reg11=reg17+reg11;
    reg23=reg22+reg23; reg25=reg24-reg25; reg17=reg0*reg16; reg13=reg26+reg13; reg14=reg18-reg14;
    reg9=reg20+reg9; reg18=reg21*pos_nodes[4][0]; reg19=reg17*pos_nodes[5][1]; reg10=reg11+reg10; reg18=reg9+reg18;
    reg9=reg25*reg13; reg11=reg17*pos_nodes[5][0]; reg20=reg23*reg14; reg10=reg19+reg10; reg18=reg11+reg18;
    reg20=reg9-reg20; reg18=pos[0]-reg18; reg11=reg25/reg20; reg19=reg23/reg20; reg22=reg14/reg20;
    reg20=reg13/reg20; reg10=pos[1]-reg10; reg20=reg18*reg20; reg19=reg10*reg19; reg22=reg18*reg22;
    reg11=reg10*reg11; var_inter[0]=reg20-reg19; var_inter[1]=reg11-reg22;

}
template<class PosNodes,class Pvec,class TVI> void get_var_inter(const Triangle_6 &elem,const PosNodes &pos_nodes,const Pvec &pos,TVI &var_inter) {
typedef typename Pvec::template SubType<0>::T T;
    T reg0=1-var_inter[0]; reg0=reg0-var_inter[1]; T reg1=2*var_inter[1]; T reg2=2*var_inter[0]; T reg3=2*reg0;
    T reg4=reg3-1; T reg5=reg2-1; T reg6=reg1-1; reg3=reg4+reg3; reg2=reg2+reg5;
    reg1=reg6+reg1; T reg7=4*var_inter[0]; T reg8=4*reg0; T reg9=pos_nodes[0][1]*reg3; T reg10=pos_nodes[1][1]*reg2;
    T reg11=pos_nodes[2][1]*reg1; T reg12=pos_nodes[2][0]*reg1; T reg13=pos_nodes[0][0]*reg3; T reg14=pos_nodes[1][0]*reg2; T reg15=reg8-reg7;
    reg5=reg5*var_inter[0]; reg4=reg0*reg4; reg10=reg10-reg9; T reg16=pos_nodes[3][1]*reg15; reg6=reg6*var_inter[1];
    T reg17=pos_nodes[1][0]*reg5; T reg18=pos_nodes[0][0]*reg4; reg9=reg11-reg9; reg11=reg7*pos_nodes[3][1]; T reg19=reg7*pos_nodes[3][0];
    reg12=reg12-reg13; T reg20=pos_nodes[0][1]*reg4; T reg21=pos_nodes[3][0]*reg15; T reg22=4*var_inter[1]; reg13=reg14-reg13;
    reg14=reg5*pos_nodes[1][1]; T reg23=reg8*var_inter[0]; T reg24=reg22*pos_nodes[4][1]; reg14=reg20+reg14; reg16=reg10+reg16;
    reg10=reg7*pos_nodes[4][1]; reg17=reg18+reg17; reg18=reg6*pos_nodes[2][0]; reg11=reg9-reg11; reg19=reg12-reg19;
    reg9=reg22*pos_nodes[4][0]; reg12=reg7*pos_nodes[4][0]; reg8=reg8-reg22; reg20=reg6*pos_nodes[2][1]; reg21=reg13+reg21;
    reg11=reg10+reg11; reg10=reg23*pos_nodes[3][1]; reg13=reg22*pos_nodes[5][1]; reg16=reg24+reg16; reg14=reg20+reg14;
    reg20=pos_nodes[5][1]*reg8; reg21=reg9+reg21; reg9=reg22*pos_nodes[5][0]; reg24=reg7*var_inter[1]; T reg25=reg23*pos_nodes[3][0];
    reg17=reg18+reg17; reg19=reg12+reg19; reg12=pos_nodes[5][0]*reg8; reg18=reg0*reg22; T reg26=reg24*pos_nodes[4][1];
    reg20=reg11+reg20; reg9=reg21-reg9; reg12=reg19+reg12; reg11=reg24*pos_nodes[4][0]; reg13=reg16-reg13;
    reg25=reg17+reg25; reg10=reg14+reg10; reg10=reg26+reg10; reg14=reg18*pos_nodes[5][1]; reg16=reg9*reg20;
    reg25=reg11+reg25; reg11=reg18*pos_nodes[5][0]; reg17=reg12*reg13; reg14=reg10+reg14; reg17=reg16-reg17;
    reg11=reg25+reg11; reg10=reg12/reg17; reg19=reg13/reg17; reg21=reg9/reg17; reg17=reg20/reg17;
    reg14=pos[1]-reg14; reg11=pos[0]-reg11; reg17=reg11*reg17; reg10=reg14*reg10; reg19=reg11*reg19;
    reg21=reg14*reg21; var_inter[0]+=reg17-reg10; var_inter[1]+=reg21-reg19;

}
template<> struct ElemVarInterFromPosNonLinear<Triangle_6> { static const bool res = 2; };
#ifndef STRUCT_Elementary_mul_nb_nodes_of_each_children_elem
#define STRUCT_Elementary_mul_nb_nodes_of_each_children_elem
struct Elementary_mul_nb_nodes_of_each_children_elem {};
#endif // STRUCT_Elementary_mul_nb_nodes_of_each_children_elem
template<class TVI,class TVAL,class T> void get_interp(const Triangle_6 &ne,const Elementary_mul_nb_nodes_of_each_children_elem &n,const TVI &var_inter,const TVAL &val,T &res) {
    res=val[0];

}
#ifndef STRUCT_Elementary_mul_nb_nodes
#define STRUCT_Elementary_mul_nb_nodes
struct Elementary_mul_nb_nodes {};
#endif // STRUCT_Elementary_mul_nb_nodes
template<class TVI,class TVAL,class T> void get_interp(const Triangle_6 &ne,const Elementary_mul_nb_nodes &n,const TVI &var_inter,const TVAL &val,T &res) {
    res=val[0];

}
#ifndef STRUCT_Nodal
#define STRUCT_Nodal
struct Nodal {};
#endif // STRUCT_Nodal
template<class TVI,class TVAL,class T> void get_interp(const Triangle_6 &ne,const Nodal &n,const TVI &var_inter,const TVAL &val,T &res) {
    T reg0=1-var_inter[0]; reg0=reg0-var_inter[1]; T reg1=2*reg0; T reg2=2*var_inter[0]; reg2=reg2-1;
    reg1=reg1-1; T reg3=2*var_inter[1]; reg2=var_inter[0]*reg2; reg3=reg3-1; reg1=reg1*reg0;
    reg2=val[1]*reg2; reg1=val[0]*reg1; T reg4=4*reg0; reg3=var_inter[1]*reg3; reg4=var_inter[0]*reg4;
    T reg5=4*var_inter[0]; reg2=reg1+reg2; reg3=val[2]*reg3; reg1=4*var_inter[1]; reg5=var_inter[1]*reg5;
    reg4=val[3]*reg4; reg3=reg2+reg3; reg1=reg1*reg0; reg5=val[4]*reg5; reg4=reg3+reg4;
    reg1=val[5]*reg1; reg5=reg4+reg5; res=reg5+reg1;

}
#ifndef STRUCT_Global
#define STRUCT_Global
struct Global {};
#endif // STRUCT_Global
template<class TVI,class TVAL,class T> void get_interp(const Triangle_6 &ne,const Global &n,const TVI &var_inter,const TVAL &val,T &res) {
    res=val[0];

}
#ifndef STRUCT_Gauss_0
#define STRUCT_Gauss_0
struct Gauss_0 {};
#endif // STRUCT_Gauss_0
template<class TVI,class TVAL,class T> void get_interp(const Triangle_6 &ne,const Gauss_0 &n,const TVI &var_inter,const TVAL &val,T &res) {
    res=val[0];

}
#ifndef STRUCT_Gauss
#define STRUCT_Gauss
struct Gauss {};
#endif // STRUCT_Gauss
template<class TVI,class TVAL,class T> void get_interp(const Triangle_6 &ne,const Gauss &n,const TVI &var_inter,const TVAL &val,T &res) {
    res=val[0];

}
#ifndef STRUCT_Gauss_2
#define STRUCT_Gauss_2
struct Gauss_2 {};
#endif // STRUCT_Gauss_2
template<class TVI,class TVAL,class T> void get_interp(const Triangle_6 &ne,const Gauss_2 &n,const TVI &var_inter,const TVAL &val,T &res) {
    res=val[0];

}
#ifndef STRUCT_Flat_interpolation
#define STRUCT_Flat_interpolation
struct Flat_interpolation {};
#endif // STRUCT_Flat_interpolation
template<class TVI,class TVAL,class T> void get_interp(const Triangle_6 &ne,const Flat_interpolation &n,const TVI &var_inter,const TVAL &val,T &res) {
    res=val[0];

}
#ifndef STRUCT_Der_nodal
#define STRUCT_Der_nodal
struct Der_nodal {};
#endif // STRUCT_Der_nodal
template<class TVI,class TVAL,class T> void get_interp(const Triangle_6 &ne,const Der_nodal &n,const TVI &var_inter,const TVAL &val,T &res) {
    res=val[0];

}
#ifndef STRUCT_Nodal_3
#define STRUCT_Nodal_3
struct Nodal_3 {};
#endif // STRUCT_Nodal_3
template<class TVI,class TVAL,class T> void get_interp(const Triangle_6 &ne,const Nodal_3 &n,const TVI &var_inter,const TVAL &val,T &res) {
    T reg0=1-var_inter[0]; reg0=reg0-var_inter[1]; T reg1=val[0]*reg0; T reg2=val[1]*var_inter[0]; reg2=reg1+reg2;
    reg1=val[2]*var_inter[1]; res=reg2+reg1;

}
#ifndef STRUCT_Elementary
#define STRUCT_Elementary
struct Elementary {};
#endif // STRUCT_Elementary
template<class TVI,class TVAL,class T> void get_interp(const Triangle_6 &ne,const Elementary &n,const TVI &var_inter,const TVAL &val,T &res) {
    res=val[0];

}
#ifndef STRUCT_Skin_elementary
#define STRUCT_Skin_elementary
struct Skin_elementary {};
#endif // STRUCT_Skin_elementary
template<class TVI,class TVAL,class T> void get_interp(const Triangle_6 &ne,const Skin_elementary &n,const TVI &var_inter,const TVAL &val,T &res) {
    res=val[0];

}
template<class TVI,class TVAL> void get_shape_functions(const Triangle_6 &ne,const TVI &var_inter,TVAL &res) {
    typedef typename TVAL::template SubType<0>::T T;
    T reg0=1-var_inter[0]; reg0=reg0-var_inter[1]; T reg1=2*reg0; T reg2=2*var_inter[1]; T reg3=2*var_inter[0];
    reg3=reg3-1; T reg4=4*var_inter[1]; T reg5=4*var_inter[0]; T reg6=4*reg0; reg2=reg2-1;
    reg1=reg1-1; reg6=var_inter[0]*reg6; res[3]=reg6; reg5=var_inter[1]*reg5; res[4]=reg5;
    reg4=reg4*reg0; res[5]=reg4; reg2=var_inter[1]*reg2; res[2]=reg2; reg1=reg1*reg0;
    res[0]=reg1; reg3=var_inter[0]*reg3; res[1]=reg3;

}
#ifndef AUTORIZEDPERM
#define AUTORIZEDPERM
template<class T> struct AuthorizedPerm;
#endif // AUTORIZEDPERM
template<> struct AuthorizedPerm<Triangle_6> {
    static const unsigned nb_permutations = 0;
    template<class TE> static void make_permutation( TE &e, unsigned nb_perm ) {
    }
    template<class TE> static void unmake_permutation( TE &e, unsigned nb_perm ) {
    }
};

template<class TN,class TNG,class TD,unsigned NET,class TVI>
typename TNG::T get_det_jac( const Element<Triangle_6,TN,TNG,TD,NET> &elem, const TVI &var_inter ) {
    typedef typename TNG::T T;
    T reg0=1-var_inter[0]; reg0=reg0-var_inter[1]; T reg1=2*reg0; T reg2=2*var_inter[0]; T reg3=reg1-1;
    T reg4=reg2-1; reg2=reg2+reg4; reg1=reg1+reg3; T reg5=4*var_inter[0]; T reg6=4*reg0;
    T reg7=reg1*elem.pos(0)[0]; T reg8=elem.pos(0)[1]*reg1; T reg9=elem.pos(1)[0]*reg2; T reg10=elem.pos(1)[1]*reg2; T reg11=reg6-reg5;
    reg9=reg9-reg7; reg10=reg10-reg8; T reg12=elem.pos(3)[0]*reg11; T reg13=4*var_inter[1]; T reg14=elem.pos(0)[2]*reg1;
    T reg15=elem.pos(3)[1]*reg11; reg2=elem.pos(1)[2]*reg2; T reg16=2*var_inter[1]; T reg17=elem.pos(4)[0]*reg13; reg2=reg2-reg14;
    reg15=reg10+reg15; reg10=elem.pos(4)[1]*reg13; reg9=reg12+reg9; reg11=elem.pos(3)[2]*reg11; reg9=reg17+reg9;
    reg2=reg11+reg2; reg11=elem.pos(5)[0]*reg13; reg12=reg16-1; reg17=elem.pos(4)[2]*reg13; T reg18=elem.pos(5)[1]*reg13;
    reg10=reg15+reg10; reg10=reg10-reg18; reg15=elem.pos(5)[2]*reg13; reg16=reg16+reg12; reg9=reg9-reg11;
    reg2=reg17+reg2; reg17=pow(reg9,2); T reg19=pow(reg10,2); T reg20=elem.pos(2)[1]*reg16; T reg21=elem.pos(2)[0]*reg16;
    reg2=reg2-reg15; reg21=reg21-reg7; T reg22=elem.pos(3)[1]*reg5; T reg23=elem.pos(3)[0]*reg5; reg16=elem.pos(2)[2]*reg16;
    reg20=reg20-reg8; T reg24=pow(reg2,2); reg19=reg17+reg19; reg21=reg21-reg23; reg16=reg16-reg14;
    reg24=reg19+reg24; reg17=elem.pos(3)[2]*reg5; reg20=reg20-reg22; reg19=elem.pos(4)[0]*reg5; T reg25=elem.pos(4)[1]*reg5;
    T reg26=reg6-reg13; reg21=reg19+reg21; reg24=pow(reg24,0.5); reg19=elem.pos(5)[1]*reg26; T reg27=elem.pos(5)[0]*reg26;
    reg16=reg16-reg17; reg25=reg20+reg25; reg20=elem.pos(4)[2]*reg5; T reg28=reg10/reg24; T reg29=reg9/reg24;
    reg20=reg16+reg20; reg26=elem.pos(5)[2]*reg26; reg25=reg19+reg25; reg21=reg27+reg21; reg26=reg20+reg26;
    reg16=reg29*reg21; reg19=reg28*reg25; reg24=reg2/reg24; reg19=reg16+reg19; reg16=reg24*reg26;
    reg16=reg19+reg16; reg19=reg28*reg16; reg20=reg29*reg16; reg27=reg24*reg16; reg19=reg25-reg19;
    reg20=reg21-reg20; reg27=reg26-reg27; T reg30=pow(reg20,2); T reg31=pow(reg19,2); T reg32=pow(reg27,2);
    reg31=reg30+reg31; reg31=reg32+reg31; reg31=pow(reg31,0.5); reg19=reg19/reg31; reg20=reg20/reg31;
    reg29=reg29*reg9; reg28=reg28*reg10; reg9=reg20*reg9; reg31=reg27/reg31; reg25=reg19*reg25;
    reg21=reg20*reg21; reg10=reg19*reg10; reg25=reg21+reg25; reg19=reg2*reg31; reg31=reg26*reg31;
    reg9=reg10+reg9; reg28=reg29+reg28; reg2=reg24*reg2; reg31=reg25+reg31; reg2=reg28+reg2;
    reg19=reg9+reg19; reg9=reg16*reg19; reg10=reg31*reg2; reg9=reg10-reg9; return reg9;

}
template<class TN,class T,class TNodalStaticData,class TD,unsigned NET>
Vec<T,2> barycenter( const Element<Triangle_6,TN,Node<2,T,TNodalStaticData>,TD,NET> &elem ) {
    Vec<T,2> res;
    T reg0=0.78379396366385999995*elem.pos(0)[1]; T reg1=0.56758792732771999991*elem.pos(1)[0]; T reg2=0.78379396366385999995*elem.pos(0)[0]; T reg3=0.56758792732771999991*elem.pos(1)[1]; T reg4=0.78379396366385999995*elem.pos(2)[0];
    T reg5=0.78379396366385999995*elem.pos(1)[1]; T reg6=0.56758792732771999991*elem.pos(0)[0]; T reg7=0.78379396366385999995*elem.pos(1)[0]; T reg8=0.56758792732771999991*elem.pos(0)[1]; T reg9=0.78379396366385999995*elem.pos(2)[1];
    T reg10=reg1+reg2; T reg11=reg0+reg3; T reg12=0.084730493093977855013*elem.pos(0)[0]; T reg13=0.56758792732771999991*elem.pos(2)[0]; T reg14=0.048208377815511963759*elem.pos(1)[0];
    T reg15=0.084730493093977855013*elem.pos(0)[1]; T reg16=1.78379396366386*elem.pos(3)[0]; T reg17=reg6+reg4; T reg18=reg9+reg8; T reg19=0.048208377815511963759*elem.pos(1)[1];
    T reg20=0.084730493093977855013*elem.pos(1)[0]; T reg21=0.56758792732771999991*elem.pos(2)[1]; T reg22=1.78379396366386*elem.pos(3)[1]; T reg23=reg7+reg6; T reg24=1.3513818909915799999*elem.pos(3)[0];
    T reg25=0.048208377815511963759*elem.pos(0)[0]; T reg26=1.3513818909915799999*elem.pos(3)[1]; T reg27=reg8+reg5; T reg28=0.048208377815511963759*elem.pos(0)[1]; T reg29=0.084730493093977855013*elem.pos(1)[1];
    reg9=reg9-reg0; T reg30=0.43241207267228000009*elem.pos(3)[1]; T reg31=0.63369514596091600003*elem.pos(1)[1]; T reg32=reg2+reg13; T reg33=0.63369514596091600003*elem.pos(2)[1];
    T reg34=reg24-reg10; T reg35=2.2673902919218320001*elem.pos(0)[0]; T reg36=0.63369514596091600003*elem.pos(1)[0]; T reg37=reg0+reg21; T reg38=2.2673902919218320001*elem.pos(0)[1];
    T reg39=reg26-reg11; T reg40=reg20+reg25; T reg41=0.43241207267228000009*elem.pos(3)[0]; reg4=reg4-reg2; reg27=reg27-reg26;
    T reg42=0.048208377815511963759*elem.pos(2)[1]; T reg43=reg15+reg19; reg17=reg17-reg16; T reg44=1.78379396366386*elem.pos(4)[0]; reg23=reg23-reg24;
    T reg45=reg28+reg29; T reg46=reg14+reg12; T reg47=0.048208377815511963759*elem.pos(2)[0]; T reg48=1.78379396366386*elem.pos(4)[1]; reg18=reg18-reg22;
    T reg49=0.63369514596091600003*elem.pos(2)[0]; T reg50=reg19+reg28; T reg51=reg42+reg45; T reg52=0.084730493093977855013*elem.pos(2)[1]; reg16=reg16+reg32;
    T reg53=reg14+reg25; T reg54=0.084730493093977855013*elem.pos(2)[0]; T reg55=0.19283351126204785504*elem.pos(3)[1]; T reg56=reg43+reg42; T reg57=0.51763234198767131696*elem.pos(0)[1];
    T reg58=0.074803807748196670769*elem.pos(1)[1]; reg0=reg5-reg0; reg5=reg33+reg38; T reg59=2.2673902919218320001*elem.pos(1)[1]; T reg60=0.63369514596091600003*elem.pos(0)[0];
    T reg61=2.2673902919218320001*elem.pos(1)[0]; T reg62=0.366304854039084*elem.pos(3)[1]; T reg63=0.63369514596091600003*elem.pos(0)[1]; T reg64=0.51763234198767131696*elem.pos(0)[0]; T reg65=0.074803807748196670769*elem.pos(1)[0];
    T reg66=0.366304854039084*elem.pos(3)[0]; T reg67=reg35+reg49; reg35=reg36+reg35; T reg68=2.9010854378827480001*elem.pos(3)[0]; T reg69=2.9010854378827480001*elem.pos(3)[1];
    reg38=reg38+reg31; reg23=reg23+reg44; T reg70=1.78379396366386*elem.pos(5)[1]; T reg71=reg47+reg40; T reg72=1.78379396366386*elem.pos(5)[0];
    T reg73=0.43241207267228000009*elem.pos(4)[0]; T reg74=1.3513818909915799999*elem.pos(5)[0]; reg17=reg44+reg17; reg41=reg4-reg41; reg4=0.19283351126204785504*elem.pos(3)[0];
    reg22=reg22+reg37; reg30=reg9-reg30; reg9=0.43241207267228000009*elem.pos(4)[1]; reg27=reg48+reg27; reg34=reg44+reg34;
    reg39=reg48+reg39; reg2=reg7-reg2; reg7=reg46+reg47; reg18=reg18+reg48; T reg75=1.3513818909915799999*elem.pos(5)[1];
    T reg76=0.79548022620090607244*elem.pos(4)[1]; reg17=reg17-reg74; T reg77=reg55-reg56; reg59=reg63+reg59; T reg78=0.366304854039084*elem.pos(4)[0];
    T reg79=reg68-reg35; reg30=reg30+reg9; reg34=reg34-reg72; T reg80=0.51763234198767131696*elem.pos(1)[1]; T reg81=reg69-reg38;
    T reg82=2.2673902919218320001*elem.pos(2)[1]; reg61=reg61+reg60; T reg83=reg67+reg66; reg57=reg57-reg58; T reg84=0.074803807748196670763*elem.pos(0)[1];
    T reg85=0.79548022620090607244*elem.pos(4)[0]; T reg86=reg4-reg7; reg64=reg64-reg65; T reg87=0.074803807748196670769*elem.pos(2)[0]; T reg88=3.2673902919218320001*elem.pos(3)[1];
    T reg89=reg63-reg33; T reg90=0.074803807748196670769*elem.pos(2)[1]; reg2=reg73+reg2; T reg91=0.43241207267228000009*elem.pos(5)[0]; reg0=reg9+reg0;
    reg9=0.43241207267228000009*elem.pos(5)[1]; reg48=reg48-reg22; reg44=reg44-reg16; T reg92=0.074803807748196670763*elem.pos(0)[0]; T reg93=0.51763234198767131696*elem.pos(1)[0];
    T reg94=reg50+reg52; T reg95=0.79548022620090607244*elem.pos(3)[1]; reg27=reg27-reg70; T reg96=0.19283351126204785504*elem.pos(4)[0]; reg4=reg4-reg71;
    T reg97=reg53+reg54; T reg98=0.79548022620090607244*elem.pos(3)[0]; reg72=reg23-reg72; reg18=reg18-reg75; reg23=reg60-reg49;
    T reg99=0.366304854039084*elem.pos(4)[1]; T reg100=reg5+reg62; reg70=reg39-reg70; reg73=reg41+reg73; reg39=3.2673902919218320001*elem.pos(3)[0];
    reg41=2.2673902919218320001*elem.pos(2)[0]; T reg101=0.19283351126204785504*elem.pos(4)[1]; reg55=reg55-reg51; reg95=reg95-reg94; reg57=reg57-reg90;
    reg82=reg63+reg82; T reg102=0.79548022620090607244*elem.pos(5)[1]; reg61=reg61-reg68; reg88=reg89-reg88; reg89=3.2673902919218320001*elem.pos(4)[1];
    reg55=reg55+reg101; reg17=reg27*reg17; reg93=reg93-reg92; reg27=3.2673902919218320001*elem.pos(4)[0]; reg59=reg59-reg69;
    T reg103=reg60-reg36; reg39=reg23-reg39; reg80=reg80-reg84; reg23=0.366304854039084*elem.pos(5)[0]; reg79=reg79+reg78;
    T reg104=2.9010854378827480001*elem.pos(5)[1]; reg34=reg30*reg34; reg30=reg99-reg100; reg73=reg70*reg73; reg41=reg60+reg41;
    reg72=reg18*reg72; reg18=0.074803807748196670769*elem.pos(0)[1]; reg98=reg98-reg97; reg4=reg4+reg96; reg70=0.79548022620090607244*elem.pos(5)[0];
    T reg105=0.074803807748196670769*elem.pos(0)[0]; reg44=reg74+reg44; reg48=reg75+reg48; reg9=reg0-reg9; reg91=reg2-reg91;
    reg0=0.29921523099278668308*elem.pos(3)[1]; reg2=2.710505431213761085e-20*elem.pos(3)[0]; T reg106=0.19283351126204785504*elem.pos(5)[1]; T reg107=0.29921523099278668308*elem.pos(3)[0]; reg64=reg64-reg87;
    reg76=reg77+reg76; reg77=reg63-reg31; T reg108=2.9010854378827480001*elem.pos(5)[0]; T reg109=reg78-reg83; reg85=reg86+reg85;
    reg86=0.19283351126204785504*elem.pos(5)[0]; T reg110=2.710505431213761085e-20*elem.pos(3)[1]; T reg111=0.366304854039084*elem.pos(5)[1]; reg81=reg99+reg81; reg66=reg41-reg66;
    reg77=reg77-reg110; reg62=reg82-reg62; reg41=reg65+reg105; reg82=0.51763234198767131696*elem.pos(2)[0]; reg76=reg76+reg106;
    reg103=reg103-reg2; reg102=reg55+reg102; reg98=reg96+reg98; reg55=reg58+reg18; reg96=0.29921523099278668305*elem.pos(3)[1];
    reg73=reg34-reg73; reg30=reg30+reg104; reg79=reg79-reg23; reg80=reg80-reg90; reg81=reg81-reg111;
    reg85=reg85+reg86; reg109=reg109+reg108; reg44=reg9*reg44; reg107=reg64+reg107; reg9=0.033544811523148658458*elem.pos(4)[0];
    reg70=reg4+reg70; reg4=0.51763234198767131696*elem.pos(2)[1]; reg34=0.033544811523148658458*elem.pos(4)[1]; reg0=reg57+reg0; reg88=reg88+reg89;
    reg57=5.42101086242752217e-20*elem.pos(5)[1]; reg17=reg72-reg17; reg61=reg78+reg61; reg59=reg99+reg59; reg95=reg101+reg95;
    reg39=reg39+reg27; reg64=5.42101086242752217e-20*elem.pos(5)[0]; reg93=reg93-reg87; reg72=0.29921523099278668305*elem.pos(3)[0]; reg91=reg48*reg91;
    reg4=reg4-reg55; reg48=0.033544811523148658455*elem.pos(3)[1]; reg101=0.29921523099278668308*elem.pos(4)[1]; reg96=reg80+reg96; reg80=0.29921523099278668308*elem.pos(5)[1];
    reg76=reg17*reg76; reg34=reg0+reg34; reg95=reg106+reg95; reg102=reg73*reg102; reg109=reg81*reg109;
    reg82=reg82-reg41; reg66=reg78+reg66; reg0=3.2673902919218320001*elem.pos(5)[1]; reg77=reg89+reg77; reg78=3.2673902919218320001*elem.pos(5)[0];
    reg103=reg27+reg103; reg62=reg99+reg62; reg9=reg107+reg9; reg27=0.29921523099278668308*elem.pos(5)[0]; reg81=0.29921523099278668308*elem.pos(4)[0];
    reg88=reg88-reg57; reg72=reg93+reg72; reg23=reg61-reg23; reg39=reg39-reg64; reg111=reg59-reg111;
    reg44=reg91-reg44; reg98=reg86+reg98; reg70=reg73*reg70; reg59=0.033544811523148658455*elem.pos(3)[0]; reg79=reg30*reg79;
    reg85=reg17*reg85; reg23=reg88*reg23; reg59=reg82+reg59; reg39=reg111*reg39; reg17=0.111690794839005*reg17;
    reg48=reg4+reg48; reg102=0.111690794839005*reg102; reg70=0.111690794839005*reg70; reg72=reg72+reg81; reg4=0.033544811523148658451*elem.pos(5)[0];
    reg9=reg9+reg27; reg73=0.111690794839005*reg73; reg95=reg44*reg95; reg85=0.111690794839005*reg85; reg34=reg34+reg80;
    reg66=reg66-reg108; reg0=reg77-reg0; reg98=reg44*reg98; reg96=reg96+reg101; reg109=reg79-reg109;
    reg78=reg103-reg78; reg30=0.033544811523148658451*elem.pos(5)[1]; reg76=0.111690794839005*reg76; reg62=reg62-reg104; reg70=reg85+reg70;
    reg30=reg96+reg30; reg48=reg101+reg48; reg98=0.111690794839005*reg98; reg9=reg109*reg9; reg34=reg109*reg34;
    reg95=0.111690794839005*reg95; reg39=reg23-reg39; reg102=reg76+reg102; reg59=reg81+reg59; reg4=reg72+reg4;
    reg73=reg17+reg73; reg44=0.111690794839005*reg44; reg78=reg62*reg78; reg66=reg0*reg66; reg48=reg80+reg48;
    reg59=reg27+reg59; reg30=reg39*reg30; reg44=reg73+reg44; reg109=0.054975871827660999999*reg109; reg98=reg70+reg98;
    reg9=0.054975871827660999999*reg9; reg34=0.054975871827660999999*reg34; reg66=reg78-reg66; reg95=reg102+reg95; reg4=reg39*reg4;
    reg9=reg98+reg9; reg48=reg66*reg48; reg59=reg66*reg59; reg34=reg95+reg34; reg39=0.054975871827660999999*reg39;
    reg109=reg44+reg109; reg30=0.054975871827660999999*reg30; reg4=0.054975871827660999999*reg4; reg59=0.054975871827660999999*reg59; reg30=reg34+reg30;
    reg4=reg9+reg4; reg39=reg109+reg39; reg66=0.054975871827660999999*reg66; reg48=0.054975871827660999999*reg48; reg48=reg30+reg48;
    reg59=reg4+reg59; reg66=reg39+reg66; res[0]=reg59/reg66; res[1]=reg48/reg66;

    return res;
}
template<class TN,class T,class TNodalStaticData,class TD,unsigned NET>
Vec<T,3> barycenter( const Element<Triangle_6,TN,Node<3,T,TNodalStaticData>,TD,NET> &elem ) {
    Vec<T,3> res;
    T reg0=0.78379396366385999995*elem.pos(1)[1]; T reg1=0.56758792732771999991*elem.pos(0)[1]; T reg2=0.56758792732771999991*elem.pos(0)[0]; T reg3=0.78379396366385999995*elem.pos(1)[0]; T reg4=0.78379396366385999995*elem.pos(0)[1];
    T reg5=0.56758792732771999991*elem.pos(1)[1]; T reg6=0.78379396366385999995*elem.pos(0)[0]; T reg7=0.56758792732771999991*elem.pos(1)[0]; T reg8=0.56758792732771999991*elem.pos(1)[2]; T reg9=0.78379396366385999995*elem.pos(0)[2];
    T reg10=0.78379396366385999995*elem.pos(1)[2]; T reg11=0.56758792732771999991*elem.pos(0)[2]; T reg12=1.3513818909915799999*elem.pos(3)[1]; T reg13=reg5+reg4; T reg14=reg3+reg2;
    T reg15=1.3513818909915799999*elem.pos(3)[0]; T reg16=reg0+reg1; T reg17=reg7+reg6; T reg18=reg8+reg9; T reg19=reg10+reg11;
    T reg20=1.3513818909915799999*elem.pos(3)[2]; reg16=reg16-reg12; T reg21=0.63369514596091600003*elem.pos(1)[1]; T reg22=1.78379396366386*elem.pos(4)[0]; T reg23=2.2673902919218320001*elem.pos(0)[0];
    T reg24=reg12-reg13; T reg25=0.63369514596091600003*elem.pos(1)[0]; T reg26=1.78379396366386*elem.pos(4)[1]; T reg27=2.2673902919218320001*elem.pos(0)[1]; T reg28=reg15-reg17;
    reg14=reg14-reg15; reg16=reg16+reg26; T reg29=2.9010854378827480001*elem.pos(3)[1]; T reg30=reg21+reg27; T reg31=1.78379396366386*elem.pos(5)[1];
    reg24=reg26+reg24; reg19=reg19-reg20; T reg32=1.78379396366386*elem.pos(4)[2]; T reg33=0.63369514596091600003*elem.pos(0)[1]; T reg34=2.2673902919218320001*elem.pos(1)[1];
    T reg35=0.63369514596091600003*elem.pos(0)[0]; reg28=reg22+reg28; T reg36=2.2673902919218320001*elem.pos(1)[0]; T reg37=reg25+reg23; T reg38=0.63369514596091600003*elem.pos(1)[2];
    T reg39=2.2673902919218320001*elem.pos(0)[2]; T reg40=2.9010854378827480001*elem.pos(3)[0]; T reg41=0.43241207267228000009*elem.pos(4)[1]; T reg42=1.78379396366386*elem.pos(5)[0]; reg14=reg14+reg22;
    reg0=reg0-reg4; T reg43=reg20-reg18; reg3=reg3-reg6; T reg44=0.43241207267228000009*elem.pos(4)[0]; reg36=reg36+reg35;
    T reg45=0.43241207267228000009*elem.pos(5)[1]; reg28=reg28-reg42; reg0=reg41+reg0; reg10=reg10-reg9; T reg46=0.43241207267228000009*elem.pos(5)[0];
    reg3=reg44+reg3; T reg47=reg40-reg37; T reg48=0.366304854039084*elem.pos(4)[0]; T reg49=2.9010854378827480001*elem.pos(3)[2]; T reg50=reg38+reg39;
    T reg51=0.43241207267228000009*elem.pos(4)[2]; T reg52=0.366304854039084*elem.pos(4)[1]; T reg53=reg29-reg30; reg19=reg19+reg32; reg43=reg32+reg43;
    T reg54=1.78379396366386*elem.pos(5)[2]; reg16=reg16-reg31; T reg55=0.63369514596091600003*elem.pos(0)[2]; T reg56=2.2673902919218320001*elem.pos(1)[2]; reg34=reg34+reg33;
    reg31=reg24-reg31; reg42=reg14-reg42; reg14=0.366304854039084*elem.pos(5)[1]; reg53=reg53+reg52; reg24=pow(reg31,2);
    T reg57=pow(reg16,2); T reg58=reg49-reg50; T reg59=0.366304854039084*elem.pos(4)[2]; T reg60=0.366304854039084*elem.pos(5)[0]; reg47=reg47+reg48;
    reg56=reg56+reg55; reg43=reg43-reg54; T reg61=2.710505431213761085e-20*elem.pos(3)[1]; T reg62=pow(reg42,2); reg45=reg0-reg45;
    reg0=pow(reg28,2); reg36=reg36-reg40; T reg63=2.710505431213761085e-20*elem.pos(3)[0]; T reg64=0.78379396366385999995*elem.pos(2)[0]; reg34=reg34-reg29;
    T reg65=reg35-reg25; reg54=reg19-reg54; reg19=0.78379396366385999995*elem.pos(2)[1]; reg46=reg3-reg46; reg10=reg51+reg10;
    reg3=0.43241207267228000009*elem.pos(5)[2]; T reg66=reg33-reg21; T reg67=pow(reg46,2); reg47=reg47-reg60; reg24=reg0+reg24;
    reg3=reg10-reg3; reg0=0.56758792732771999991*elem.pos(2)[0]; reg10=0.56758792732771999991*elem.pos(2)[1]; T reg68=pow(reg45,2); T reg69=pow(reg43,2);
    reg66=reg66-reg61; reg65=reg65-reg63; T reg70=3.2673902919218320001*elem.pos(4)[1]; T reg71=3.2673902919218320001*elem.pos(4)[0]; reg56=reg56-reg49;
    reg34=reg52+reg34; reg57=reg62+reg57; reg62=pow(reg54,2); T reg72=reg55-reg38; reg36=reg48+reg36;
    T reg73=reg2+reg64; T reg74=1.78379396366386*elem.pos(3)[0]; T reg75=reg1+reg19; T reg76=1.78379396366386*elem.pos(3)[1]; T reg77=2.710505431213761085e-20*elem.pos(3)[2];
    reg53=reg53-reg14; T reg78=0.78379396366385999995*elem.pos(2)[2]; T reg79=0.366304854039084*elem.pos(5)[2]; reg58=reg58+reg59; T reg80=1.78379396366386*elem.pos(3)[2];
    reg62=reg57+reg62; reg58=reg58-reg79; reg14=reg34-reg14; reg56=reg59+reg56; reg34=3.2673902919218320001*elem.pos(5)[1];
    reg57=pow(reg47,2); T reg81=3.2673902919218320001*elem.pos(4)[2]; T reg82=pow(reg53,2); reg69=reg24+reg69; reg68=reg67+reg68;
    reg65=reg71+reg65; reg24=3.2673902919218320001*elem.pos(5)[0]; reg66=reg70+reg66; reg67=reg4+reg10; reg4=reg19-reg4;
    reg19=reg6+reg0; reg72=reg72-reg77; T reg83=pow(reg3,2); T reg84=0.63369514596091600003*elem.pos(2)[1]; T reg85=0.43241207267228000009*elem.pos(3)[1];
    reg75=reg75-reg76; T reg86=0.56758792732771999991*elem.pos(2)[2]; T reg87=0.43241207267228000009*elem.pos(3)[0]; reg6=reg64-reg6; reg73=reg73-reg74;
    reg64=0.63369514596091600003*elem.pos(2)[0]; T reg88=reg11+reg78; reg60=reg36-reg60; reg36=0.43241207267228000009*elem.pos(3)[2]; reg78=reg78-reg9;
    reg34=reg66-reg34; reg72=reg81+reg72; reg66=3.2673902919218320001*elem.pos(5)[2]; reg24=reg65-reg24; reg79=reg56-reg79;
    reg56=pow(reg14,2); reg65=pow(reg60,2); T reg89=0.63369514596091600003*elem.pos(2)[2]; T reg90=0.366304854039084*elem.pos(3)[1]; reg27=reg27+reg84;
    reg83=reg68+reg83; reg68=0.366304854039084*elem.pos(3)[0]; reg74=reg74+reg19; reg23=reg23+reg64; reg76=reg76+reg67;
    reg9=reg9+reg86; T reg91=pow(reg58,2); reg82=reg57+reg82; reg87=reg6-reg87; reg85=reg4-reg85;
    reg73=reg22+reg73; reg4=1.3513818909915799999*elem.pos(5)[0]; reg6=1.3513818909915799999*elem.pos(5)[1]; reg88=reg88-reg80; reg75=reg26+reg75;
    reg69=pow(reg69,0.5); reg62=pow(reg62,0.5); reg83=pow(reg83,0.5); reg66=reg72-reg66; reg80=reg80+reg9;
    reg57=pow(reg34,2); reg72=pow(reg24,2); T reg92=reg27+reg90; T reg93=reg28/reg69; T reg94=reg35-reg64;
    T reg95=reg31/reg69; T reg96=3.2673902919218320001*elem.pos(3)[0]; reg73=reg73-reg4; T reg97=3.2673902919218320001*elem.pos(3)[1]; T reg98=reg33-reg84;
    reg39=reg39+reg89; reg44=reg87+reg44; reg87=0.366304854039084*elem.pos(3)[2]; reg26=reg26-reg76; reg41=reg85+reg41;
    reg36=reg78-reg36; reg22=reg22-reg74; reg78=1.3513818909915799999*elem.pos(5)[2]; reg88=reg32+reg88; reg91=reg82+reg91;
    reg82=reg16/reg62; reg85=reg42/reg62; T reg99=2.2673902919218320001*elem.pos(2)[1]; reg56=reg65+reg56; reg65=pow(reg79,2);
    T reg100=2.2673902919218320001*elem.pos(2)[0]; reg75=reg75-reg6; T reg101=reg23+reg68; reg26=reg6+reg26; T reg102=reg45/reg83;
    reg32=reg32-reg80; reg62=reg54/reg62; reg96=reg94-reg96; reg94=reg46/reg83; reg69=reg43/reg69;
    T reg103=reg39+reg87; reg65=reg56+reg65; reg22=reg4+reg22; reg99=reg33+reg99; reg56=2.9010854378827480001*elem.pos(5)[0];
    reg88=reg88-reg78; T reg104=reg82*reg75; reg100=reg35+reg100; T reg105=reg48-reg101; reg51=reg36+reg51;
    reg36=reg93*reg44; T reg106=2.2673902919218320001*elem.pos(2)[2]; T reg107=pow(reg66,2); reg57=reg72+reg57; reg72=reg85*reg73;
    T reg108=3.2673902919218320001*elem.pos(3)[2]; T reg109=reg55-reg89; T reg110=reg52-reg92; T reg111=2.9010854378827480001*elem.pos(5)[1]; T reg112=reg95*reg41;
    reg97=reg98-reg97; reg91=pow(reg91,0.5); reg98=reg62*reg88; T reg113=2.9010854378827480001*elem.pos(5)[2]; T reg114=reg47/reg91;
    T reg115=reg53/reg91; reg104=reg72+reg104; reg105=reg56+reg105; reg72=reg102*reg26; T reg116=reg59-reg103;
    reg110=reg110+reg111; reg32=reg78+reg32; reg112=reg36+reg112; reg36=reg94*reg22; reg83=reg3/reg83;
    reg65=pow(reg65,0.5); T reg117=reg69*reg51; reg108=reg109-reg108; reg107=reg57+reg107; reg106=reg55+reg106;
    reg57=5.42101086242752217e-20*elem.pos(5)[0]; reg71=reg96+reg71; reg90=reg99-reg90; reg96=5.42101086242752217e-20*elem.pos(5)[1]; reg70=reg97+reg70;
    reg68=reg100-reg68; reg97=reg114*reg105; reg107=pow(reg107,0.5); reg68=reg48+reg68; reg91=reg58/reg91;
    reg90=reg52+reg90; reg87=reg106-reg87; reg117=reg112+reg117; reg98=reg104+reg98; reg116=reg116+reg113;
    reg72=reg36+reg72; reg36=reg60/reg65; reg48=reg14/reg65; reg52=reg83*reg32; reg71=reg71-reg57;
    reg99=reg115*reg110; reg70=reg70-reg96; reg81=reg108+reg81; reg100=5.42101086242752217e-20*elem.pos(5)[2]; reg104=reg34/reg107;
    reg99=reg97+reg99; reg68=reg68-reg56; reg97=reg36*reg71; reg106=reg24/reg107; reg90=reg90-reg111;
    reg65=reg79/reg65; reg108=reg48*reg70; reg87=reg59+reg87; reg81=reg81-reg100; reg59=reg91*reg116;
    reg109=reg85*reg98; reg112=reg82*reg98; T reg118=reg95*reg117; T reg119=reg93*reg117; reg52=reg72+reg52;
    reg72=reg104*reg90; reg109=reg73-reg109; reg118=reg41-reg118; reg112=reg75-reg112; T reg120=reg62*reg98;
    reg119=reg44-reg119; T reg121=reg65*reg81; reg87=reg87-reg113; T reg122=reg69*reg117; T reg123=reg102*reg52;
    reg108=reg97+reg108; reg59=reg99+reg59; reg97=reg94*reg52; reg99=reg106*reg68; reg107=reg66/reg107;
    T reg124=pow(reg109,2); T reg125=reg114*reg59; T reg126=pow(reg112,2); T reg127=pow(reg119,2); T reg128=reg115*reg59;
    reg120=reg88-reg120; reg72=reg99+reg72; reg123=reg26-reg123; reg121=reg108+reg121; reg99=pow(reg118,2);
    reg108=reg83*reg52; reg122=reg51-reg122; T reg129=reg107*reg87; reg97=reg22-reg97; T reg130=reg48*reg121;
    T reg131=reg91*reg59; reg108=reg32-reg108; T reg132=reg36*reg121; reg129=reg72+reg129; reg126=reg124+reg126;
    reg72=pow(reg120,2); reg124=pow(reg123,2); reg99=reg127+reg99; reg127=pow(reg97,2); T reg133=pow(reg122,2);
    reg128=reg110-reg128; reg125=reg105-reg125; T reg134=reg65*reg121; reg124=reg127+reg124; reg127=pow(reg128,2);
    T reg135=pow(reg108,2); reg131=reg116-reg131; reg130=reg70-reg130; T reg136=pow(reg125,2); reg133=reg99+reg133;
    reg132=reg71-reg132; reg72=reg126+reg72; reg99=reg106*reg129; reg126=reg104*reg129; T reg137=pow(reg130,2);
    reg133=pow(reg133,0.5); reg72=pow(reg72,0.5); reg99=reg68-reg99; reg126=reg90-reg126; reg135=reg124+reg135;
    reg127=reg136+reg127; reg134=reg81-reg134; reg124=pow(reg132,2); reg136=pow(reg131,2); T reg138=reg107*reg129;
    T reg139=0.084730493093977855013*elem.pos(1)[0]; T reg140=0.048208377815511963759*elem.pos(1)[0]; T reg141=0.048208377815511963759*elem.pos(0)[0]; T reg142=0.084730493093977855013*elem.pos(0)[2]; T reg143=0.048208377815511963759*elem.pos(1)[2];
    T reg144=0.084730493093977855013*elem.pos(0)[0]; T reg145=0.084730493093977855013*elem.pos(1)[1]; T reg146=0.048208377815511963759*elem.pos(0)[1]; T reg147=0.048208377815511963759*elem.pos(1)[1]; reg136=reg127+reg136;
    reg118=reg118/reg133; reg112=reg112/reg72; reg109=reg109/reg72; reg127=pow(reg99,2); T reg148=pow(reg126,2);
    reg138=reg87-reg138; reg119=reg119/reg133; T reg149=0.084730493093977855013*elem.pos(0)[1]; T reg150=pow(reg134,2); reg135=pow(reg135,0.5);
    reg137=reg124+reg137; reg124=0.084730493093977855013*elem.pos(1)[2]; T reg151=0.048208377815511963759*elem.pos(0)[2]; reg82=reg16*reg82; reg44=reg44*reg119;
    reg85=reg42*reg85; T reg152=0.048208377815511963759*elem.pos(2)[0]; T reg153=reg144+reg140; T reg154=reg141+reg139; T reg155=reg146+reg145;
    reg41=reg41*reg118; reg133=reg122/reg133; reg93=reg28*reg93; reg95=reg31*reg95; reg119=reg28*reg119;
    reg28=reg149+reg147; reg118=reg31*reg118; reg148=reg127+reg148; reg31=pow(reg138,2); reg122=0.048208377815511963759*elem.pos(2)[1];
    reg150=reg137+reg150; reg127=reg142+reg143; reg137=0.048208377815511963759*elem.pos(2)[2]; reg123=reg123/reg135; reg16=reg16*reg112;
    reg42=reg42*reg109; reg97=reg97/reg135; reg72=reg120/reg72; reg136=pow(reg136,0.5); reg120=reg151+reg124;
    reg112=reg75*reg112; reg109=reg73*reg109; reg31=reg148+reg31; reg26=reg26*reg123; reg150=pow(reg150,0.5);
    reg22=reg22*reg97; reg73=reg137+reg120; reg125=reg125/reg136; reg128=reg128/reg136; reg75=reg43*reg133;
    reg148=reg122+reg28; T reg156=0.19283351126204785504*elem.pos(3)[1]; T reg157=reg140+reg141; T reg158=0.084730493093977855013*elem.pos(2)[0]; T reg159=0.51763234198767131696*elem.pos(0)[0];
    T reg160=0.074803807748196670769*elem.pos(1)[0]; reg41=reg44+reg41; reg133=reg51*reg133; reg44=reg54*reg72; reg16=reg42+reg16;
    reg72=reg88*reg72; reg112=reg109+reg112; reg118=reg119+reg118; reg42=0.074803807748196670769*elem.pos(1)[2]; reg51=0.51763234198767131696*elem.pos(0)[2];
    reg88=reg143+reg151; reg123=reg45*reg123; reg97=reg46*reg97; reg109=0.084730493093977855013*elem.pos(2)[2]; reg135=reg108/reg135;
    reg108=reg147+reg146; reg119=0.084730493093977855013*elem.pos(2)[1]; T reg161=reg152+reg154; reg94=reg46*reg94; reg82=reg85+reg82;
    reg102=reg45*reg102; reg62=reg54*reg62; reg45=reg122+reg155; reg46=reg137+reg127; reg69=reg43*reg69;
    reg43=0.074803807748196670769*elem.pos(1)[1]; reg54=reg153+reg152; reg85=0.51763234198767131696*elem.pos(0)[1]; T reg162=0.19283351126204785504*elem.pos(3)[0]; T reg163=0.19283351126204785504*elem.pos(3)[2];
    reg95=reg93+reg95; reg93=reg3*reg135; reg114=reg47*reg114; T reg164=reg109+reg88; reg123=reg97+reg123;
    reg115=reg53*reg115; reg97=0.79548022620090607244*elem.pos(3)[2]; T reg165=reg162-reg54; T reg166=0.074803807748196670769*elem.pos(2)[2]; T reg167=0.79548022620090607244*elem.pos(4)[0];
    T reg168=0.19283351126204785504*elem.pos(4)[2]; reg85=reg85-reg43; reg159=reg159-reg160; T reg169=0.074803807748196670769*elem.pos(2)[0]; T reg170=0.51763234198767131696*elem.pos(1)[2];
    T reg171=0.074803807748196670763*elem.pos(0)[2]; T reg172=0.074803807748196670769*elem.pos(2)[1]; reg133=reg41+reg133; reg62=reg82+reg62; reg51=reg51-reg42;
    reg72=reg112+reg72; reg41=0.074803807748196670763*elem.pos(0)[1]; reg82=0.79548022620090607244*elem.pos(3)[1]; reg112=0.51763234198767131696*elem.pos(1)[0]; reg83=reg3*reg83;
    reg102=reg94+reg102; reg3=reg119+reg108; reg69=reg95+reg69; reg53=reg53*reg128; reg47=reg47*reg125;
    reg94=0.79548022620090607244*elem.pos(3)[0]; reg95=reg157+reg158; T reg173=0.79548022620090607244*elem.pos(4)[1]; T reg174=reg163-reg46; T reg175=reg156-reg148;
    T reg176=0.79548022620090607244*elem.pos(4)[2]; reg75=reg118+reg75; reg156=reg156-reg45; reg136=reg131/reg136; reg128=reg110*reg128;
    reg110=0.19283351126204785504*elem.pos(4)[1]; reg125=reg105*reg125; reg105=0.19283351126204785504*elem.pos(4)[0]; reg163=reg163-reg73; reg135=reg32*reg135;
    reg32=0.074803807748196670763*elem.pos(0)[0]; reg118=0.51763234198767131696*elem.pos(1)[1]; reg31=pow(reg31,0.5); reg26=reg22+reg26; reg130=reg130/reg150;
    reg44=reg16+reg44; reg162=reg162-reg161; reg132=reg132/reg150; reg163=reg163+reg168; reg16=0.19283351126204785504*elem.pos(5)[1];
    reg22=0.074803807748196670769*elem.pos(0)[2]; reg170=reg170-reg171; reg131=0.79548022620090607244*elem.pos(5)[2]; reg173=reg175+reg173; reg175=0.79548022620090607244*elem.pos(5)[1];
    reg156=reg156+reg110; reg97=reg97-reg164; T reg177=0.29921523099278668308*elem.pos(3)[2]; reg51=reg51-reg166; reg82=reg82-reg3;
    reg85=reg85-reg172; reg99=reg99/reg31; reg126=reg126/reg31; reg176=reg174+reg176; reg174=0.29921523099278668308*elem.pos(3)[1];
    T reg178=0.074803807748196670769*elem.pos(0)[1]; reg118=reg118-reg41; T reg179=0.19283351126204785504*elem.pos(5)[2]; reg72=reg62*reg72; reg62=0.074803807748196670769*elem.pos(0)[0];
    reg94=reg94-reg95; T reg180=0.19283351126204785504*elem.pos(5)[0]; reg91=reg58*reg91; T reg181=0.29921523099278668308*elem.pos(3)[0]; T reg182=reg14*reg130;
    T reg183=reg60*reg132; reg44=reg98*reg44; reg116=reg116*reg136; reg93=reg123+reg93; reg150=reg134/reg150;
    reg112=reg112-reg32; reg75=reg117*reg75; reg132=reg71*reg132; reg128=reg125+reg128; reg130=reg70*reg130;
    reg167=reg165+reg167; reg36=reg60*reg36; reg135=reg26+reg135; reg115=reg114+reg115; reg83=reg102+reg83;
    reg133=reg69*reg133; reg48=reg14*reg48; reg136=reg58*reg136; reg162=reg162+reg105; reg159=reg159-reg169;
    reg53=reg47+reg53; reg14=0.79548022620090607244*elem.pos(5)[0]; reg174=reg85+reg174; reg93=reg52*reg93; reg44=reg72-reg44;
    reg177=reg51+reg177; reg97=reg168+reg97; reg26=0.29921523099278668305*elem.pos(3)[0]; reg112=reg112-reg169; reg47=0.033544811523148658458*elem.pos(4)[2];
    reg135=reg83*reg135; reg75=reg133-reg75; reg91=reg115+reg91; reg173=reg173+reg16; reg131=reg163+reg131;
    reg94=reg105+reg94; reg82=reg110+reg82; reg162=reg14+reg162; reg65=reg79*reg65; reg48=reg36+reg48;
    reg175=reg156+reg175; reg167=reg180+reg167; reg81=reg81*reg150; reg14=0.29921523099278668305*elem.pos(3)[2]; reg182=reg183+reg182;
    reg150=reg79*reg150; reg118=reg118-reg172; reg36=0.29921523099278668305*elem.pos(3)[1]; reg116=reg128+reg116; reg51=0.51763234198767131696*elem.pos(2)[2];
    reg52=reg160+reg62; reg58=0.51763234198767131696*elem.pos(2)[0]; reg60=reg42+reg22; reg136=reg53+reg136; reg53=0.51763234198767131696*elem.pos(2)[1];
    reg106=reg24*reg106; reg104=reg34*reg104; reg69=reg43+reg178; reg68=reg68*reg99; reg130=reg132+reg130;
    reg90=reg90*reg126; reg31=reg138/reg31; reg170=reg170-reg166; reg99=reg24*reg99; reg24=0.033544811523148658458*elem.pos(4)[1];
    reg126=reg34*reg126; reg176=reg176+reg179; reg34=0.033544811523148658458*elem.pos(4)[0]; reg181=reg159+reg181; reg70=0.033544811523148658455*elem.pos(3)[1];
    reg53=reg53-reg69; reg24=reg174+reg24; reg71=0.29921523099278668308*elem.pos(5)[1]; reg136=reg59*reg136; reg176=reg44*reg176;
    reg26=reg112+reg26; reg116=reg91*reg116; reg59=0.29921523099278668308*elem.pos(4)[0]; reg167=reg167*reg44; reg72=0.29921523099278668308*elem.pos(4)[1];
    reg82=reg16+reg82; reg36=reg118+reg36; reg107=reg66*reg107; reg104=reg106+reg104; reg16=0.033544811523148658455*elem.pos(3)[2];
    reg51=reg51-reg60; reg79=0.033544811523148658455*elem.pos(3)[0]; reg58=reg58-reg52; reg83=0.29921523099278668308*elem.pos(4)[2]; reg14=reg170+reg14;
    reg150=reg182+reg150; reg81=reg130+reg81; reg90=reg68+reg90; reg87=reg87*reg31; reg126=reg99+reg126;
    reg31=reg66*reg31; reg175=reg75*reg175; reg65=reg48+reg65; reg48=0.29921523099278668308*elem.pos(5)[2]; reg47=reg177+reg47;
    reg34=reg181+reg34; reg66=0.29921523099278668308*elem.pos(5)[0]; reg94=reg180+reg94; reg97=reg179+reg97; reg173=reg44*reg173;
    reg131=reg75*reg131; reg93=reg135-reg93; reg162=reg162*reg75; reg16=reg51+reg16; reg136=reg116-reg136;
    reg131=0.111690794839005*reg131; reg51=0.033544811523148658451*elem.pos(5)[2]; reg14=reg14+reg83; reg94=reg94*reg93; reg97=reg93*reg97;
    reg34=reg66+reg34; reg176=0.111690794839005*reg176; reg167=0.111690794839005*reg167; reg47=reg47+reg48; reg162=0.111690794839005*reg162;
    reg173=0.111690794839005*reg173; reg175=0.111690794839005*reg175; reg44=0.111690794839005*reg44; reg82=reg93*reg82; reg26=reg26+reg59;
    reg24=reg24+reg71; reg75=0.111690794839005*reg75; reg68=0.033544811523148658451*elem.pos(5)[0]; reg36=reg36+reg72; reg85=0.033544811523148658451*elem.pos(5)[1];
    reg31=reg126+reg31; reg70=reg53+reg70; reg107=reg104+reg107; reg79=reg58+reg79; reg150=reg121*reg150;
    reg81=reg65*reg81; reg87=reg90+reg87; reg31=reg129*reg31; reg47=reg136*reg47; reg150=reg81-reg150;
    reg75=reg44+reg75; reg51=reg14+reg51; reg93=0.111690794839005*reg93; reg94=0.111690794839005*reg94; reg97=0.111690794839005*reg97;
    reg79=reg59+reg79; reg16=reg83+reg16; reg131=reg176+reg131; reg87=reg107*reg87; reg162=reg167+reg162;
    reg175=reg173+reg175; reg85=reg36+reg85; reg70=reg72+reg70; reg34=reg34*reg136; reg82=0.111690794839005*reg82;
    reg24=reg136*reg24; reg26=reg68+reg26; reg85=reg150*reg85; reg34=0.054975871827660999999*reg34; reg47=0.054975871827660999999*reg47;
    reg31=reg87-reg31; reg26=reg26*reg150; reg51=reg150*reg51; reg24=0.054975871827660999999*reg24; reg93=reg75+reg93;
    reg136=0.054975871827660999999*reg136; reg94=reg162+reg94; reg82=reg175+reg82; reg97=reg131+reg97; reg79=reg66+reg79;
    reg70=reg71+reg70; reg16=reg48+reg16; reg26=0.054975871827660999999*reg26; reg51=0.054975871827660999999*reg51; reg70=reg31*reg70;
    reg85=0.054975871827660999999*reg85; reg16=reg31*reg16; reg34=reg94+reg34; reg47=reg97+reg47; reg24=reg82+reg24;
    reg79=reg79*reg31; reg150=0.054975871827660999999*reg150; reg136=reg93+reg136; reg16=0.054975871827660999999*reg16; reg31=0.054975871827660999999*reg31;
    reg150=reg136+reg150; reg70=0.054975871827660999999*reg70; reg26=reg34+reg26; reg51=reg47+reg51; reg79=0.054975871827660999999*reg79;
    reg85=reg24+reg85; reg70=reg85+reg70; reg79=reg26+reg79; reg31=reg150+reg31; reg16=reg51+reg16;
    res[2]=reg16/reg31; res[0]=reg79/reg31; res[1]=reg70/reg31;

    return res;
}
}
#endif // LMT_TRIANGLE_6
