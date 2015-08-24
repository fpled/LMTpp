#ifndef TRANFSERT_H
#define TRANFSERT_H

#include "../containers/algo.h"

namespace LMT {

///
struct Transfert_2_meshes_CorrespondingNodes { };

///
template<class TM1,class TM2,class Method=Transfert_2_meshes_CorrespondingNodes> class Transfert_2_meshes;

///
template<class TM1,class TM2>
class Transfert_2_meshes<TM1,TM2,Transfert_2_meshes_CorrespondingNodes> {
public:
    Transfert_2_meshes(TM1 &m1_,TM2 &m2_):m1(m1_),m2(m2_) {
        nodal_correspondance = intersection_ptr(m1.node_list, m2.node_list, Function<DistBetweenNodes>() < 1e-10 );
        elem_correspondance = intersection_ptr(m1.elem_list, m2.elem_list, Function<DistBetweenElemCenter>() < 1e-10 );
        PRINT( nodal_correspondance.size() );
        PRINT( elem_correspondance.size() );
    }
private:
    struct Trans12 {
        template<class T1,class T2> void operator()(std::pair<T1 *,T2 *> t) const { DM::copy( *t.first, *t.second ); }
        template<class T1,class T2,class Field> void operator()(std::pair<T1 *,T2 *> t,const Field &field) const {
            t.second->member_named(field) = t.first->member_named(field);
        }
    };
    struct Trans21 {
        template<class T1,class T2> void operator()(std::pair<T1 *,T2 *> t) const { DM::copy( *t.second, *t.first ); } 
        template<class T1,class T2,class Field> void operator()(std::pair<T1 *,T2 *> t,const Field &field) const {
            t.first->member_named(field) = t.second->member_named(field);
        }
    };
public:
    void m1_to_m2() { apply( nodal_correspondance, Trans12() ); apply( elem_correspondance, Trans12() ); }
    void m2_to_m1() { apply( nodal_correspondance, Trans21() ); apply( elem_correspondance, Trans21() ); }
    template<class Field> void m1_to_m2_elementary(const Field &field) { apply( elem_correspondance, Trans12(), field ); }
    template<class Field> void m2_to_m1_elementary(const Field &field) { apply( elem_correspondance, Trans21(), field ); }
    template<class Field> void m1_to_m2_nodal(const Field &field) { apply( nodal_correspondance, Trans12(), field ); }
    template<class Field> void m2_to_m1_nodal(const Field &field) { apply( nodal_correspondance, Trans21(), field ); }
    TM1 &m1;
    TM2 &m2;
    typename IntersectionCarac<typename TM1::TNodeList,typename TM2::TNodeList>::T nodal_correspondance;
    typename IntersectionCarac<typename TM1::TElemList,typename TM2::TElemList>::T elem_correspondance;
};

};

#endif // TRANFSERT_H
