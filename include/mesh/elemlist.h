#ifndef ELEMLIST_H
#define ELEMLIST_H

#include "../containers/typeifnotvoid.h"
#include "../util/valorptr.h"
#include "../containers/indexof.h"
#include "dynamicdata.h"

namespace LMT {

template<class Carac_,unsigned max_sub_meshes> class Mesh;
template<class MeshCarac,class TNode,unsigned nvi_to_subs,unsigned skin> struct VecElemList {};
template<class MeshCarac,class TNode,unsigned nvi_to_subs,unsigned skin> struct IsVecOp<VecElemList<MeshCarac,TNode,nvi_to_subs,skin> > { typedef int T; };

/// heterogeneous list of elements
template<class MeshCarac,class TNode,unsigned nvi_to_subs,unsigned skin>
class Vec<VecElemList<MeshCarac,TNode,nvi_to_subs,skin>,-1,int> {
private:
    struct CaracNEBE {
        template<unsigned n,unsigned inner=0> struct SubType {
            typedef typename MeshCarac::template ElementChoice<nvi_to_subs,skin,n> TC;
            typedef typename TypeIfNotVoid<std::pair<typename TC::NE,typename TC::BE>,typename TC::NE>::T T;
        };
    };
    struct GetNvi { template<class TE> struct Val { static const unsigned res = TE::nb_var_inter; }; };
    
    template<class TL,class TE=typename TL::T>
    struct GetNviFromHeterList {
        static const unsigned cur = TE::nb_var_inter;
        static const unsigned nex = GetNviFromHeterList<typename TL::TNext>::res;
        static const unsigned res = ( nex > cur ? nex : cur );
    };
    template<class TL>
    struct GetNviFromHeterList<TL,void> {
        static const unsigned res = 0;
    };
public:
    static const int static_size = -1;
    static const bool fixed_size = false;
    static const int sparsity_rate = 0;
    template<unsigned ptr> struct ElemNum {
        template<unsigned n,unsigned inner=0> struct SubType {
            typedef typename MeshCarac::template ElementChoice<nvi_to_subs,skin,n> TC;
            typedef typename TC::NE NE;
            typedef typename TC::BE BE;
            typedef typename TC::TData TData;
            //typedef typename MeshCarac::template ElementaryStaticData<NE,BE,nvi_to_subs,skin> TData;
            typedef typename Element<NE,BE,TNode,TData,n>::TClass TClass;
            typedef typename ValOrPtr<TClass,ptr>::T T;
        };
    };
    template<unsigned ptr> struct CaracVec {
        template<unsigned n,unsigned inner=0> struct SubType {
            typedef typename MeshCarac::template ElementChoice<nvi_to_subs,skin,n> TC;
            typedef typename TC::NE NE;
            typedef typename TC::BE BE;
            typedef typename TC::TData TData;
            typedef typename Element<NE,BE,TNode,TData,n>::TClass TTClass;
            typedef typename ValOrPtr<TTClass,ptr>::T TClass;
            typedef typename TypeIfNotVoid<
                Splitted<TClass,16>,
                NE
            >::T T;
        };
    };
    /// list of elements
    typedef Vec<Heterogeneous<CaracVec<0> > > TList;
    /// list of pointers on elements
    typedef Vec<Heterogeneous<CaracVec<1> > > TListPtr;
    /// list of pointers on pointers on elements
    typedef Vec<Heterogeneous<CaracVec<2> > > TListPtrPtr;
    /// nb type of elements 
    static const unsigned nb_elem_type = TList::nb_sub_type;
    ///
    static const unsigned nvi = GetNviFromHeterList<TList>::res;
    ///
    template<class NE,class BE> struct IndexOf { static const unsigned res = LMT::IndexOf<CaracNEBE,std::pair<NE,BE> >::res; };
    ///
    template<class NE,class BE> struct TElem { typedef typename ElemNum<0>::template SubType<IndexOf<NE,BE>::res>::T TE; };
    /// EA * can be a pointer on all kind of elements
    typedef ElementAncestor<TNode> EA;

    Vec() {
        nb_hash_val = 0;
        //         has_elements_with_hash_val.resize(nb_hash_val);
    }
    struct RemetNousLesElementsDansLaTableDeHashageDeMaxPecas {
        template<class TE,class TM> void operator()(TE &e,TM &m) const { m.elem_list.register_element_in_hash_table( &e, m ); }
    };
    template<class TM> void change_hash_size(TM &m,unsigned n) {
        nb_hash_val = n;
        has_elements_with_hash_val.free();
        has_elements_with_hash_val.resize( n );
        unsigned ns[ nb_elem_type + 1 ];
        for(unsigned i=0;i<nb_elem_type;++i)
            ns[i] = 0;
        for(unsigned i=0;i<n;++i)
            has_elements_with_hash_val[i].resize(ns);
        apply( *this, RemetNousLesElementsDansLaTableDeHashageDeMaxPecas(), m );
    }
    
    template<unsigned n> struct SubType { typedef typename ElemNum<0>::template SubType<n>::T T; };
    static const unsigned nb_sub_type = TList::nb_sub_type;
    
    template<unsigned n> struct SubVec { typedef typename TList::template SubVec<n>::TS TS; };
    template<unsigned n> typename SubVec<n>::TS &sub_vec(const Number<n> &nn) { return hp.sub_vec(nn); }
    template<unsigned n> const typename SubVec<n>::TS &sub_vec(const Number<n> &nn) const { return hp.sub_vec(nn); }

    unsigned size() const { return hp.size(); } ///
    void get_sizes(unsigned *s) const { hp.get_sizes(s); } /// -> get sizes of each type
    
    unsigned index_of_down_cast(const EA *ea) const { return hp.index_of_down_cast(ea); } /// return index of type of EA which can be an ancestor of an element in the list

    template<class DM> const typename ExtractDM<DM>::template ReturnType< typename CaracVec<0>::template SubType<0>::TTClass >::T &val_elem_nb( unsigned i, const DM &dm = DM() ) const {
        return hp.val_elem_nb( i, dm );
    }
    template<class DM> typename ExtractDM<DM>::template ReturnType< typename CaracVec<0>::template SubType<0>::TTClass >::T &val_elem_nb( unsigned i, const DM &dm = DM() ) {
        return hp.val_elem_nb( i, dm );
    }

    template<class DM> static const typename ExtractDM<DM>::template ReturnType< typename CaracVec<0>::template SubType<0>::TTClass >::T &val_elem( const EA *e, const DM &dm = DM() ) {
        return TList::val_elem( e, dm );
    }
    template<class DM> static typename ExtractDM<DM>::template ReturnType< typename CaracVec<0>::template SubType<0>::TTClass >::T &val_elem( EA *e, const DM &dm = DM() ) {
        return TList::val_elem( e, dm );
    }

    template<class Op> static void apply_on_down_cast(const EA *ea,const Op& op) { TList::apply_on_down_cast(ea,op); }
    template<class Op> static void apply_on_down_cast(EA *ea,const Op& op) { TList::apply_on_down_cast(ea,op); }
    template<class Op> static void apply_on_down_cast(const EA *ea,Op& op) { TList::apply_on_down_cast(ea,op); }
    template<class Op> static void apply_on_down_cast(EA *ea,Op& op) { TList::apply_on_down_cast(ea,op); }
    template<class Op,class TP0> static void apply_on_down_cast(const EA *ea,const Op& op,const TP0 &p0) { TList::apply_on_down_cast(ea,op,p0); }
    template<class Op,class TP0> static void apply_on_down_cast(EA *ea,const Op& op,const TP0 &p0) { TList::apply_on_down_cast(ea,op,p0); }
    template<class Op,class TP0> static void apply_on_down_cast(const EA *ea,Op& op,const TP0 &p0) { TList::apply_on_down_cast(ea,op,p0); }
    template<class Op,class TP0> static void apply_on_down_cast(EA *ea,Op& op,const TP0 &p0) { TList::apply_on_down_cast(ea,op,p0); }
    template<class Op,class TP0> static void apply_on_down_cast(const EA *ea,const Op& op,TP0 &p0) { TList::apply_on_down_cast(ea,op,p0); }
    template<class Op,class TP0> static void apply_on_down_cast(EA *ea,const Op& op,TP0 &p0) { TList::apply_on_down_cast(ea,op,p0); }
    template<class Op,class TP0> static void apply_on_down_cast(const EA *ea,Op& op,TP0 &p0) { TList::apply_on_down_cast(ea,op,p0); }
    template<class Op,class TP0> static void apply_on_down_cast(EA *ea,Op& op,TP0 &p0) { TList::apply_on_down_cast(ea,op,p0); }
    template<class Op,class TP0,class TP1> static void apply_on_down_cast(const EA *ea,const Op& op,const TP0 &p0,const TP1 &p1) { TList::apply_on_down_cast(ea,op,p0,p1); }
    template<class Op,class TP0,class TP1> static void apply_on_down_cast(EA *ea,const Op& op,const TP0 &p0,const TP1 &p1) { TList::apply_on_down_cast(ea,op,p0,p1); }
    template<class Op,class TP0,class TP1> static void apply_on_down_cast(const EA *ea,Op& op,const TP0 &p0,const TP1 &p1) { TList::apply_on_down_cast(ea,op,p0,p1); }
    template<class Op,class TP0,class TP1> static void apply_on_down_cast(EA *ea,Op& op,const TP0 &p0,const TP1 &p1) { TList::apply_on_down_cast(ea,op,p0,p1); }
    template<class Op,class TP0,class TP1> static void apply_on_down_cast(const EA *ea,const Op& op,TP0 &p0,const TP1 &p1) { TList::apply_on_down_cast(ea,op,p0,p1); }
    template<class Op,class TP0,class TP1> static void apply_on_down_cast(EA *ea,const Op& op,TP0 &p0,const TP1 &p1) { TList::apply_on_down_cast(ea,op,p0,p1); }
    template<class Op,class TP0,class TP1> static void apply_on_down_cast(const EA *ea,Op& op,TP0 &p0,const TP1 &p1) { TList::apply_on_down_cast(ea,op,p0,p1); }
    template<class Op,class TP0,class TP1> static void apply_on_down_cast(EA *ea,Op& op,TP0 &p0,const TP1 &p1) { TList::apply_on_down_cast(ea,op,p0,p1); }
    template<class Op,class TP0,class TP1> static void apply_on_down_cast(const EA *ea,const Op& op,const TP0 &p0,TP1 &p1) { TList::apply_on_down_cast(ea,op,p0,p1); }
    template<class Op,class TP0,class TP1> static void apply_on_down_cast(EA *ea,const Op& op,const TP0 &p0,TP1 &p1) { TList::apply_on_down_cast(ea,op,p0,p1); }
    template<class Op,class TP0,class TP1> static void apply_on_down_cast(const EA *ea,Op& op,const TP0 &p0,TP1 &p1) { TList::apply_on_down_cast(ea,op,p0,p1); }
    template<class Op,class TP0,class TP1> static void apply_on_down_cast(EA *ea,Op& op,const TP0 &p0,TP1 &p1) { TList::apply_on_down_cast(ea,op,p0,p1); }
    template<class Op,class TP0,class TP1> static void apply_on_down_cast(const EA *ea,const Op& op,TP0 &p0,TP1 &p1) { TList::apply_on_down_cast(ea,op,p0,p1); }
    template<class Op,class TP0,class TP1> static void apply_on_down_cast(EA *ea,const Op& op,TP0 &p0,TP1 &p1) { TList::apply_on_down_cast(ea,op,p0,p1); }
    template<class Op,class TP0,class TP1> static void apply_on_down_cast(const EA *ea,Op& op,TP0 &p0,TP1 &p1) { TList::apply_on_down_cast(ea,op,p0,p1); }
    template<class Op,class TP0,class TP1> static void apply_on_down_cast(EA *ea,Op& op,TP0 &p0,TP1 &p1) { TList::apply_on_down_cast(ea,op,p0,p1); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(const EA *ea,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(EA *ea,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(const EA *ea,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(EA *ea,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(const EA *ea,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(EA *ea,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(const EA *ea,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(EA *ea,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(const EA *ea,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(EA *ea,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(const EA *ea,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(EA *ea,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(const EA *ea,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(EA *ea,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(const EA *ea,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(EA *ea,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(const EA *ea,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(EA *ea,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(const EA *ea,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(EA *ea,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(const EA *ea,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(EA *ea,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(const EA *ea,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(EA *ea,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(const EA *ea,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(EA *ea,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(const EA *ea,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(EA *ea,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(const EA *ea,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(EA *ea,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(const EA *ea,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_on_down_cast(EA *ea,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { TList::apply_on_down_cast(ea,op,p0,p1,p2); }
    


    EA *operator[](unsigned num) { return reinterpret_cast<EA *>(hp.get_ptr(num)); }
    const EA *operator[](unsigned num) const { return reinterpret_cast<const EA *>(hp.get_ptr(num)); }

    template<class Op> static void apply_static(const Op& op) { TList::apply_static(op); }
    template<class Op> static void apply_static(Op& op) { TList::apply_static(op); }
    template<class Op,class TP0> static void apply_static(const Op& op,const TP0 &p0) { TList::apply_static(op,p0); }
    template<class Op,class TP0> static void apply_static(Op& op,const TP0 &p0) { TList::apply_static(op,p0); }
    template<class Op,class TP0> static void apply_static(const Op& op,TP0 &p0) { TList::apply_static(op,p0); }
    template<class Op,class TP0> static void apply_static(Op& op,TP0 &p0) { TList::apply_static(op,p0); }
    template<class Op,class TP0,class TP1> static void apply_static(const Op& op,const TP0 &p0,const TP1 &p1) { TList::apply_static(op,p0,p1); }
    template<class Op,class TP0,class TP1> static void apply_static(Op& op,const TP0 &p0,const TP1 &p1) { TList::apply_static(op,p0,p1); }
    template<class Op,class TP0,class TP1> static void apply_static(const Op& op,TP0 &p0,const TP1 &p1) { TList::apply_static(op,p0,p1); }
    template<class Op,class TP0,class TP1> static void apply_static(Op& op,TP0 &p0,const TP1 &p1) { TList::apply_static(op,p0,p1); }
    template<class Op,class TP0,class TP1> static void apply_static(const Op& op,const TP0 &p0,TP1 &p1) { TList::apply_static(op,p0,p1); }
    template<class Op,class TP0,class TP1> static void apply_static(Op& op,const TP0 &p0,TP1 &p1) { TList::apply_static(op,p0,p1); }
    template<class Op,class TP0,class TP1> static void apply_static(const Op& op,TP0 &p0,TP1 &p1) { TList::apply_static(op,p0,p1); }
    template<class Op,class TP0,class TP1> static void apply_static(Op& op,TP0 &p0,TP1 &p1) { TList::apply_static(op,p0,p1); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static(const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { TList::apply_static(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static(Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { TList::apply_static(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static(const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { TList::apply_static(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static(Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { TList::apply_static(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static(const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { TList::apply_static(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static(Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { TList::apply_static(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static(const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { TList::apply_static(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static(Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { TList::apply_static(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static(const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { TList::apply_static(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static(Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { TList::apply_static(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static(const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { TList::apply_static(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static(Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { TList::apply_static(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static(const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { TList::apply_static(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static(Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { TList::apply_static(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static(const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { TList::apply_static(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static(Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { TList::apply_static(op,p0,p1,p2); }

    template<class Op> static void apply_static_with_n(const Op& op) { TList::apply_static_with_n(op); }
    template<class Op> static void apply_static_with_n(Op& op) { TList::apply_static_with_n(op); }
    template<class Op,class TP0> static void apply_static_with_n(const Op& op,const TP0 &p0) { TList::apply_static_with_n(op,p0); }
    template<class Op,class TP0> static void apply_static_with_n(Op& op,const TP0 &p0) { TList::apply_static_with_n(op,p0); }
    template<class Op,class TP0> static void apply_static_with_n(const Op& op,TP0 &p0) { TList::apply_static_with_n(op,p0); }
    template<class Op,class TP0> static void apply_static_with_n(Op& op,TP0 &p0) { TList::apply_static_with_n(op,p0); }
    template<class Op,class TP0,class TP1> static void apply_static_with_n(const Op& op,const TP0 &p0,const TP1 &p1) { TList::apply_static_with_n(op,p0,p1); }
    template<class Op,class TP0,class TP1> static void apply_static_with_n(Op& op,const TP0 &p0,const TP1 &p1) { TList::apply_static_with_n(op,p0,p1); }
    template<class Op,class TP0,class TP1> static void apply_static_with_n(const Op& op,TP0 &p0,const TP1 &p1) { TList::apply_static_with_n(op,p0,p1); }
    template<class Op,class TP0,class TP1> static void apply_static_with_n(Op& op,TP0 &p0,const TP1 &p1) { TList::apply_static_with_n(op,p0,p1); }
    template<class Op,class TP0,class TP1> static void apply_static_with_n(const Op& op,const TP0 &p0,TP1 &p1) { TList::apply_static_with_n(op,p0,p1); }
    template<class Op,class TP0,class TP1> static void apply_static_with_n(Op& op,const TP0 &p0,TP1 &p1) { TList::apply_static_with_n(op,p0,p1); }
    template<class Op,class TP0,class TP1> static void apply_static_with_n(const Op& op,TP0 &p0,TP1 &p1) { TList::apply_static_with_n(op,p0,p1); }
    template<class Op,class TP0,class TP1> static void apply_static_with_n(Op& op,TP0 &p0,TP1 &p1) { TList::apply_static_with_n(op,p0,p1); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static_with_n(const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { TList::apply_static_with_n(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static_with_n(Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { TList::apply_static_with_n(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static_with_n(const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { TList::apply_static_with_n(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static_with_n(Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { TList::apply_static_with_n(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static_with_n(const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { TList::apply_static_with_n(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static_with_n(Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { TList::apply_static_with_n(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static_with_n(const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { TList::apply_static_with_n(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static_with_n(Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { TList::apply_static_with_n(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static_with_n(const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { TList::apply_static_with_n(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static_with_n(Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { TList::apply_static_with_n(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static_with_n(const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { TList::apply_static_with_n(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static_with_n(Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { TList::apply_static_with_n(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static_with_n(const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { TList::apply_static_with_n(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static_with_n(Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { TList::apply_static_with_n(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static_with_n(const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { TList::apply_static_with_n(op,p0,p1,p2); }
    template<class Op,class TP0,class TP1,class TP2> static void apply_static_with_n(Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { TList::apply_static_with_n(op,p0,p1,p2); }

    /// registering a DynamicData field can be pertinent when functions like mesh.filter_element(...) are used :
    ///  data in dda will automatically be re-ordered.
    template<class T> void reg_dyn(DynamicData<T,nb_elem_type> *dda) const {
        dyn_data.push_back(dda);
        synchronize_dyn(dda);
    }
    /// 
    template<class T> void unreg_dyn(DynamicData<T,nb_elem_type> *dda) const { dyn_data.erase_first(dda); }
    ///
    void synchronize_dyn(DynamicDataAncestor *dda) const {
        unsigned ss[nb_elem_type+(nb_elem_type==0)]; hp.get_sizes(ss);
        for(unsigned i=0;i<nb_elem_type;++i) dda->resize(i,ss[i]);
    }
    /// autmatically called by signal_connectivity_change() in Mesh<>
    void synchronize_all_dyn() {
        unsigned ss[nb_elem_type+(nb_elem_type==0)]; hp.get_sizes(ss);
        for(unsigned i=0;i<dyn_data.size();++i)
            for(unsigned j=0;j<nb_elem_type;++j)
                dyn_data[i]->resize(j,ss[j]);
    }
    ///
    unsigned nb_dyn_data() const { return dyn_data.size(); }
    ///
    DynamicDataAncestor *get_dyn_data(unsigned i) { return dyn_data[i]; }
    ///
    const DynamicDataAncestor *get_dyn_data(unsigned i) const { return dyn_data[i]; }
    
    /// uniform interface for static and dynamic data.
    template<class T> static T &get_data(DynamicData<T,nb_elem_type> &dda,const EA &e) { return dda.get_data(e.num_in_elem_list_virtual(),e.number);}
    template<class T> static const T &get_data(const DynamicData<T,nb_elem_type> &dda,const EA &e) { return dda.get_data(e.num_in_elem_list_virtual(),e.number); }
    ///
    template<class T,class TE> static T &get_data(DynamicData<T,nb_elem_type> &dda,const TE &e) { return dda.get_data(e.num_in_elem_list,e.number);}
    template<class T,class TE> static const T &get_data(const DynamicData<T,nb_elem_type> &dda,const TE &e) { return dda.get_data(e.num_in_elem_list,e.number); }
    ///
    template<class NDM,class TE> typename TE::template SubTypeByName0<NDM>::TT &get_data(const NDM &ndm,TE &n) { return n.member_named(ndm); }
    template<class NDM,class TE> const typename TE::template SubTypeByName0<NDM>::TT &get_data(const NDM &ndm,const TE &n) const { return n.member_named(ndm); }
    
    ///
    template<class TE,class TM> static unsigned hash_val(const TE &e,const TM &m,unsigned nb_hash_val) {
        unsigned res=0; for(unsigned i=0;i<e.nb_nodes_virtual();++i) res+=m.node_list.number(*e.node_virtual(i)); return res % nb_hash_val;
    }
    ///
    unsigned nb_hash_val;
protected:
    template<class TE>
    struct FindElemWithNE {
        FindElemWithNE():res(NULL) {}
        template<class TE2> bool operator()(const TE2 *e) { return false; }
        bool operator()(TE *e) {
            unsigned nb_sim = 0;
            for(unsigned i=0;i<TE::nb_nodes;++i)
                for(unsigned j=0;j<TE::nb_nodes;++j)
                    nb_sim += ( e->node(i)==n[j] );
            res = e;
            return (nb_sim==TE::nb_nodes);
        }
        TNode **n;
        TE *res;
    };
public:
    ///
    template<class NE,class BE,class TM> typename TElem<NE,BE>::TE *find(const NE &ne,const BE &be,const TM &m,TNode **n) {
        typedef typename TElem<NE,BE>::TE TE;
        unsigned hash_val = 0;
        for(unsigned i=0;i<NE::nb_nodes;hash_val += m.node_list.number(*n[i++]));
        hash_val %= nb_hash_val;
        FindElemWithNE<TE> few;
        few.n = n;
        
        if ( LMT::find( has_elements_with_hash_val[hash_val], few ) )
            return few.res;
        return (TE *)NULL;
    }
    
    ///
    TList hp;
    
    /// internal use. assumes that this element has not been registered
    template<class TE> void register_element_in_hash_table(TE *e,unsigned hash_val) { has_elements_with_hash_val[hash_val].push_back( e ); }
    /// internal use. assumes that this element has not been registered AND node are well numbered
    template<class TE,class TM> void register_element_in_hash_table(TE *e,const TM &m) { register_element_in_hash_table(e,hash_val(*e,m,nb_hash_val)); }
protected:
    ///
    void free() {
        hp.free();
        for(unsigned i=0;i<dyn_data.size();++i) dyn_data[i]->free();
        for(unsigned i=0;i<has_elements_with_hash_val.size();++i) has_elements_with_hash_val[i].free();
        has_elements_with_hash_val.free();
        nb_hash_val = 0;
    }
    /// internal use (protected exposure...)
    void pop_back_and_copy_to(DynamicDataAncestor *dda,unsigned i0,unsigned i1) const {
        dda->pop_back_and_copy_to(i0,i1);
    }
    ///
    void push_back_dyn(unsigned num) { for(unsigned i=0;i<dyn_data.size();++i) dyn_data[i]->push_back(num); }

    /// update number of element that will be displaced, dyn_data and cpt_elem
    template<class Op> struct RemoveIfElement {
        template<class TE> bool operator()(TE &e) {
            if ( (*op)(e)==false ) return false;
            /// else, we eliminate this element
            el->hp.sub_vec( Number< IndexOf<typename TE::NE,typename TE::BE>::res >() ).back().number = e.number;
            for(unsigned i=0;i<el->dyn_data.size();++i)
                el->pop_back_and_copy_to(el->dyn_data[i],TE::num_in_elem_list,e.number);
            return true;
        }
        Op *op;
        Vec *el;
    };
    template<class Op> friend struct RemoveIfElement;
    ///
    template<class Op> void remove_if(Op &op) {
        RemoveIfElement<Op> fop;
        fop.op = &op;
        fop.el = this;
        LMT::remove_if( hp, fop );
    }
    ///
    void remove_nb(unsigned nl,unsigned nil) {
        hp.remove_nb( nl, nil );
    }
    ///
    Vec<TListPtr> has_elements_with_hash_val;
    ///
    mutable Vec<DynamicDataAncestor *> dyn_data;

    template<class Carac_,unsigned max_sub_meshes> friend class Mesh;
    template<class Carac_,unsigned nvi_to_subs_,unsigned skin_> friend class MeshAncestor;
    template<class Carac_,unsigned nvi_to_subs_,unsigned skin_,unsigned end_nvi_to_subs_> friend class MeshGeneric;
};

} // namespace LMT

#include "elemlistapply.h"
  
#endif // ELEMLIST_H
