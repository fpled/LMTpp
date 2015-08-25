//
// C++ Interface: meshdata
//
// Description: 
//
//
// Author: Hugo LECLERC <leclerc@lmt.ens-cachan.fr>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMTMESHDATA_H
#define LMTMESHDATA_H

#ifndef DONT_WANT_XML
    #include "../io/xmlnode.h"
#endif
#include "../containers/mat.h"

namespace LMT {

/// Ex : CARACDMEXTNAME(0,double,pressure);
/// Used when pressure_DM is defined outward
/**
Cette macro permet l'introspection du code C++ pour obtenir par exemple les attributs et les méthodes d'une classe.

    \friend hugo.leclerc@lmt.ens-cachan.fr
*/
#define CARACDMEXTNAME(number,mytype,myname,unit) \
    template<unsigned n,unsigned ioena=0> struct SubType##number { typedef void TT; typedef void NT; }; \
    template<unsigned ioena> struct SubType0<number,ioena> { typedef mytype TT; typedef myname##_DM NT; }; \
    template<class NDM,unsigned ioena=0> struct SubTypeByName##number { typedef void TT; typedef void NT; }; \
    template<unsigned ioena> struct SubTypeByName0<myname##_DM,ioena> { typedef mytype TT; }; \
    mytype &member_nb(const Number<number> &__n__) { return myname; } \
    const mytype &member_nb(const Number<number> &__n__) const { return myname; } \
    mytype &member_named(const myname##_DM &__n__) { return myname; } \
    const mytype &member_named(const myname##_DM &__n__) const { return myname; } \
    static const char *name_member_nb(const Number<number> &__n__) { return __STRING(myname); } \
    static const char *unit_member_nb(const Number<number> &__n__) { return unit; } \
    static unsigned nb_comp(const Number<number> &__n__) { return DM::NbComponents<mytype>::n; } \
    template<class NDM,class TTT> void set_member_named##number( const NDM &ndm, const TTT &__val__ ) {} \
    template<class NDM,class TTT> void set_member_named_if_same_type##number( const NDM &ndm, const TTT &__val__ ) {} \
    template<class TTT> void set_member_named0( const myname##_DM &ndm, const TTT &__val__ ) { myname = __val__; } \
    void set_member_named_if_same_type0( const myname##_DM &ndm, const mytype &__val__ ) { myname = __val__; } \
    template<class TTT> void set_member_named_if_same_type0( const myname##_DM &ndm, const TTT &__val__ ) {} \
    mytype myname

/// Ex : CARACDM(0,double,pressure);
#define CARACDM(number,mytype,myname,unit) \
    struct myname##_DM { typedef mytype TT; }; \
    CARACDMEXTNAME(number,mytype,myname,unit)
    
/// rq : declare static const unsigned nb_var = nb_var_of_parent
#define DMSETINHERITSFROM(PARENT) \
    template<unsigned dn,unsigned ioena=0> struct SubType0 { \
        typedef typename PARENT::template SubType0<dn,0>::TT TT; typedef typename PARENT::template SubType0<dn,0>::NT NT; }; \
    template<class NDM,unsigned ioena=0> struct SubTypeByName0 { typedef typename PARENT::template SubTypeByName0<NDM,0>::TT TT; }; \
    template<unsigned dn> typename PARENT::template SubType0<dn>::TT &member_nb(const Number<dn> &__n__) { return PARENT::member_nb(__n__); } \
    template<unsigned dn> const typename PARENT::template SubType0<dn>::TT &member_nb(const Number<dn> &__n__) const { return PARENT::member_nb(__n__); } \
    template<unsigned dn> static const char *name_member_nb(const Number<dn> &__n__) { return PARENT::name_member_nb(__n__); } \
    template<unsigned dn> static const char *unit_member_nb(const Number<dn> &__n__) { return PARENT::unit_member_nb(__n__); } \
    template<unsigned dn> static unsigned nb_comp(const Number<dn> &__n__) { return PARENT::nb_comp(__n__); } \
    template<class NDM,class TTT> void set_member_named0( const NDM &ndm, const TTT &__val__ ) { PARENT::set_member_named0(ndm,__val__); } \
    static const unsigned nb_var_parent = DM::NbFields<PARENT>::res

#define VOIDDMSET \
    template<unsigned dntoto,unsigned ioenatoto=0> struct SubType0 { typedef void TT; }; \
    template<class NDM,class TTT> void set_member_named0( const NDM &ndm, const TTT &__val__ ) {} \
    static const unsigned nb_params = 0
    
/// struct Foo : public VoidDMSet {}; create a DMset with 0 fields
struct VoidDMSet {
    VOIDDMSET;
    template<class TT> void dm_data_set_field( const std::string field_name, TT value, bool disp = true ) {
        if ( disp ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
    }
    template<class TT> TT dm_data_get_field( const std::string field_name, StructForType<TT>, bool disp = true ) const {
        if ( disp ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
        return TT();
    }
};

namespace DM {

    template<class T> struct NbComponents { static const unsigned n = 1; };
    template<> struct NbComponents<std::string> { static const unsigned n = 0; };
    template<> struct NbComponents<const char *> { static const unsigned n = 0; };
    template<class T,int dim> struct NbComponents<Vec<T,dim> > { static const unsigned n = int(NbComponents<T>::n*(MAX(dim,0))); };
    template<class T,int d1,int d2> struct NbComponents<Mat<T,Gen<d1,d2> > > { static const unsigned n = NbComponents<T>::n*MAX(d1,0)*MAX(d2,0); };
    template<class T,int d> struct NbComponents<Mat<T,Sym<d> > > { static const unsigned n = NbComponents<T>::n*MAX(d,0)*MAX(d+1,0)/2; };
    #ifndef WITHOUTCOMPLEX
        template<class T> struct NbComponents<std::complex<T> > { static const unsigned n = NbComponents<T>::n*2; };
    #endif

    namespace DMPRIVATE {
        template<class DMSet,class T,unsigned n> struct NbFieldsBis {
            static const unsigned res = NbFieldsBis<DMSet,typename DMSet::template SubType0<n+1>::TT,n+1>::res;
        };
        template<class DMSet,unsigned n> struct NbFieldsBis<DMSet,void,n> { static const unsigned res = n; };
        template<class DMSet,class Top,unsigned number>
        void apply(const DMSet &dm_set,Top &op,const Number<number> &n1,const Number<number> &nt) {}
        template<class DMSet,class Top,unsigned number,unsigned total_number>
        void apply(const DMSet &dm_set,Top &op,const Number<number> &n1,const Number<total_number> &nt) {
            op( number, dm_set.member_nb(Number<number>()) );
            apply(dm_set,op,Number<number+1>(),nt);
        }
        template<class DMSet,class Top,unsigned number>
        void apply(const DMSet &dm_set,const Top &op,const Number<number> &n1,const Number<number> &nt) {}
        template<class DMSet,class Top,unsigned number,unsigned total_number>
        void apply(const DMSet &dm_set,const Top &op,const Number<number> &n1,const Number<total_number> &nt) {
            op( number, dm_set.member_nb(n1) );
            apply(dm_set,op,Number<number+1>(),nt);
        }
        template<class DMSet,unsigned tn>
        void get_names(const StructForType<DMSet> &t,const char **names,const Number<tn> &n1,const Number<tn> &nt) {}
        template<class DMSet,unsigned number,unsigned tn>
        void get_names(const StructForType<DMSet> &t,const char **names,const Number<number> &n1,const Number<tn> &nt) {
            names[number] = DMSet::name_member_nb(n1);
            get_names(t,names,Number<number+1>(),nt);
        }
        ///
        template<class DMSet,class Op,unsigned tn>
        void get_types(const StructForType<DMSet> &t,Op &op,const Number<tn> &n1,const Number<tn> &nt) {}
        template<class DMSet,class Op,unsigned number,unsigned tn>
        void get_types(const StructForType<DMSet> &t,Op &op,const Number<number> &n1,const Number<tn> &nt) {
            op( n1, StructForType<typename DMSet::template SubType0<number>::TT>() );
            get_types(t,op,Number<number+1>(),nt);
        }
        ///
        template<class DMSet,unsigned tn>
        void get_nb_comp(const StructForType<DMSet> &t,unsigned *res,const Number<tn> &n1,const Number<tn> &nt) {
        }
        template<class DMSet,unsigned number,unsigned tn>
        void get_nb_comp(const StructForType<DMSet> &t,unsigned *res,const Number<number> &n1,const Number<tn> &nt) {
            res[number] = DMSet::nb_comp(n1);
            get_nb_comp(t,res,Number<number+1>(),nt);
        }
        template<class DMSet,class Top,unsigned number>
        void apply_with_names(const DMSet &dm_set,Top &op,const Number<number> &n1,const Number<number> &nt) {}
        template<class DMSet,class Top,unsigned number,unsigned total_number>
        void apply_with_names(const DMSet &dm_set,Top &op,const Number<number> &n1,const Number<total_number> &nt) {
            op( number, dm_set.name_member_nb(Number<number>()), dm_set.member_nb(Number<number>()) );
            apply_with_names(dm_set,op,Number<number+1>(),nt);
        }
        template<class DMSet,class Top,unsigned number>
        void apply_with_names(DMSet &dm_set,Top &op,const Number<number> &n1,const Number<number> &nt) {}
        template<class DMSet,class Top,unsigned number,unsigned total_number>
        void apply_with_names(DMSet &dm_set,Top &op,const Number<number> &n1,const Number<total_number> &nt) {
            op( number, dm_set.name_member_nb(Number<number>()), dm_set.member_nb(Number<number>()) );
            apply_with_names(dm_set,op,Number<number+1>(),nt);
        }
        #ifndef DONT_WANT_XML
        template<class DMSet,unsigned total_number>
        void get_data_from_xml(DMSet &dm_set,const XmlNode &xn,const Number<total_number> &n1,const Number<total_number> &nt,bool want_warning_not_present_in_xml) {}
        template<class DMSet,unsigned number,unsigned total_number>
        void get_data_from_xml(DMSet &dm_set,const XmlNode &xn,const Number<number> &n1,const Number<total_number> &nt,bool want_warning_not_present_in_xml) {
            if ( xn.has_attribute( DMSet::name_member_nb(n1) ) )
                xn.get_attribute_with_unit( DMSet::name_member_nb(n1), DMSet::unit_member_nb(n1), dm_set.member_nb(n1), want_warning_not_present_in_xml );
            else if ( want_warning_not_present_in_xml )
                std::cerr << "Attribute " << DMSet::name_member_nb(n1) << " is not present in xml file -> Using value " << dm_set.member_nb(n1) << "." << std::endl;
            get_data_from_xml(dm_set,xn,Number<number+1>(),nt,want_warning_not_present_in_xml);
        }
        template<class DMSet,unsigned total_number>
        void send_data_to_xml(const DMSet &dm_set,XmlNode &xn,const Number<total_number> &n1,const Number<total_number> &nt) {}
        template<class DMSet,unsigned number,unsigned total_number>
        void send_data_to_xml(const DMSet &dm_set,XmlNode &xn,const Number<number> &n1,const Number<total_number> &nt) {
            xn.set_attribute_with_unit( DMSet::name_member_nb(n1), DMSet::unit_member_nb(n1), dm_set.member_nb(n1) );
            send_data_to_xml(dm_set,xn,Number<number+1>(),nt);
        }
        #endif
        template<class DMSet1,class DMSet2,unsigned total_number>
        void copy(const DMSet1 &dm_set1,DMSet2 &dm_set2,const Number<total_number> &n1,const Number<total_number> &nt) {}
        template<class DMSet1,class DMSet2,unsigned number,unsigned total_number>
        void copy(const DMSet1 &dm_set1,DMSet2 &dm_set2,const Number<number> &n1,const Number<total_number> &nt) {
            dm_set2.set_member_named_if_same_type0( typename DMSet1::template SubType0<number,0>::NT(), dm_set1.member_nb(n1) );
            copy(dm_set1,dm_set2,Number<number+1>(),nt);
        }
        template<class Tpond,class DMSet1,class DMSet2,unsigned total_number>
        void get_ponderation(const std::pair<Tpond,const DMSet1 *> *pond_list,unsigned nb_pond,DMSet2 &dm_set2,const Number<total_number> &n1,const Number<total_number> &nt ) {}
        template<class Tpond,class DMSet1,class DMSet2,unsigned number,unsigned total_number>
        void get_ponderation(const std::pair<Tpond,const DMSet1 *> *pond_list,unsigned nb_pond,DMSet2 &dm_set2,const Number<number> &n1,const Number<total_number> &nt) {
            typename DMSet1::template SubType0<number,0>::TT mean = pond_list[0].first * pond_list[0].second->member_nb(n1);
            for(unsigned i=1;i<nb_pond;++i)
                mean += pond_list[i].first * pond_list[i].second->member_nb(n1);
            dm_set2.set_member_named0( typename DMSet1::template SubType0<number,0>::NT(), mean );
            get_ponderation(pond_list,nb_pond,dm_set2,Number<number+1>(),nt);
        }
        //
        template<class P>
        void assign_or_assert_if_same_type_or_not( P *&r, P *v ) { r = v; }
        template<class P1,class P2>
        void assign_or_assert_if_same_type_or_not( P1 *&r, P2 *v ) { assert(0); }
        template<class DMSet,class P,unsigned total_number>
        void get_ptr_on( DMSet &dmset, const char *name, P *&res,const Number<total_number> &n1,const Number<total_number> &nt ) { res = NULL; }
        template<class DMSet,class P,unsigned number,unsigned total_number>
        void get_ptr_on( DMSet &dmset, const char *name, P *&res,const Number<number> &n1,const Number<total_number> &nt ) {
            if ( strcmp( name, dmset.name_member_nb(n1) ) == 0 )
                assign_or_assert_if_same_type_or_not( res, &dmset.member_nb(n1) );
            else
                get_ptr_on( dmset, name, res, Number<number+1>(), nt );
        }

    };
    
    /// Get number of fields of a structure containing VOIDDMSET; or one or several CARADM(...);
    template<class DMSet> struct NbFields {
        typedef typename DMSet::template SubType0<0,0> ST;
        static const unsigned res = DMPRIVATE::NbFieldsBis<DMSet,typename ST::TT,0>::res;
    };

    /// call op(number,variable) on each element of dm_set
    template<class DMSet,class Top>
    void apply(const DMSet &dm_set,Top &op) { DMPRIVATE::apply(dm_set,op,Number<0>(),Number<NbFields<DMSet>::res>()); }
    template<class DMSet,class Top>
    void apply(const DMSet &dm_set,const Top &op) { DMPRIVATE::apply(dm_set,op,Number<0>(),Number<NbFields<DMSet>::res>()); }
    
    /// call op(number,variable) on elements {0..n-1} of dm_set
    template<class DMSet,class Top,unsigned n>
    void apply_up_to(const DMSet &dm_set,const Top &op,const Number<n> &nn) { DMPRIVATE::apply(dm_set,op,Number<0>(),nn); }
    template<class DMSet,class Top,unsigned n>
    void apply_up_to(const DMSet &dm_set,Top &op,const Number<n> &nn) { DMPRIVATE::apply(dm_set,op,Number<0>(),nn); }
    
    /// call op(name,variable) on each element of dm_set
    template<class DMSet,class Top>
    void apply_with_names(const DMSet &dm_set,Top &op) { DMPRIVATE::apply_with_names(dm_set,op,Number<0>(),Number<NbFields<DMSet>::res>()); }
    template<class DMSet,class Top,unsigned n>
    void apply_with_names_up_to(const DMSet &dm_set,Top &op,const Number<n> &nn) { DMPRIVATE::apply_with_names(dm_set,op,Number<0>(),nn); }
    
    /// call op(name,variable) on each element of dm_set
    template<class DMSet,class Top>
    void apply_with_names(DMSet &dm_set,Top &op) { DMPRIVATE::apply_with_names(dm_set,op,Number<0>(),Number<NbFields<DMSet>::res>()); }
    template<class DMSet,class Top,unsigned n>
    void apply_with_names_up_to(DMSet &dm_set,Top &op,const Number<n> &nn) { DMPRIVATE::apply_with_names(dm_set,op,Number<0>(),nn); }
    
    /// get name of variables contained in DMSet (CARACDM(...))
    template<class DMSet>
    void get_names(const char **names) { DMPRIVATE::get_names(StructForType<DMSet>(),names,Number<0>(),Number<NbFields<DMSet>::res>()); }
    template<class DMSet,unsigned n>
    void get_names_up_to(const char **names,const Number<n> &nn) { DMPRIVATE::get_names(StructForType<DMSet>(),names,Number<0>(),nn); }

    /// get name of variables contained in DMSet (CARACDM(...))
    template<class DMSet,class Op>
    void get_types(Op &op) { DMPRIVATE::get_types(StructForType<DMSet>(),op,Number<0>(),Number<NbFields<DMSet>::res>()); }
    template<class DMSet,class Op,unsigned n>
    void get_types_up_to(Op &op,const Number<n> &nn) { DMPRIVATE::get_types(StructForType<DMSet>(),op,Number<0>(),nn); }

    /// get number of components (nb scalars) in each attribute
    template<class DMSet>
    void get_nb_comp(unsigned *res) { DMPRIVATE::get_nb_comp(StructForType<DMSet>(),res,Number<0>(),Number<NbFields<DMSet>::res>()); }
    template<class DMSet,unsigned n>
    void get_nb_comp_up_to(unsigned *res,const Number<n> &nn) { DMPRIVATE::get_nb_comp(StructForType<DMSet>(),res,Number<0>(),nn); }

    #ifndef DONT_WANT_XML
    ///
    template<class DMSet>
    void get_data_from_xml(DMSet &dm_set,const XmlNode &xn,bool want_warning_not_present_in_xml=true) { DMPRIVATE::get_data_from_xml(dm_set,xn,Number<0>(),Number<NbFields<DMSet>::res>(),want_warning_not_present_in_xml); }
    template<class DMSet,unsigned n>
    void get_data_from_xml_up_to(DMSet &dm_set,const XmlNode &xn,const Number<n> &nn,bool want_warning_not_present_in_xml=true) { DMPRIVATE::get_data_from_xml(dm_set,xn,Number<0>(),nn,want_warning_not_present_in_xml); }

    ///
    template<class DMSet>
    void send_data_to_xml(const DMSet &dm_set,XmlNode &xn) { DMPRIVATE::send_data_to_xml(dm_set,xn,Number<0>(),Number<NbFields<DMSet>::res>()); }
    template<class DMSet,unsigned n>
    void send_data_to_xml_up_to(const DMSet &dm_set,XmlNode &xn,const Number<n> &nn) { DMPRIVATE::send_data_to_xml(dm_set,xn,Number<0>(),nn); }
    #endif

    ///
    template<class DMSet1,class DMSet2>
    void copy( const DMSet1 &dm_set1, DMSet2 &dm_set2 ) { DMPRIVATE::copy(dm_set1,dm_set2,Number<0>(),Number<NbFields<DMSet1>::res>()); }
    template<class DMSet1,class DMSet2,unsigned n>
    void copy_up_to( const DMSet1 &dm_set1, DMSet2 &dm_set2, const Number<n> &nn ) { DMPRIVATE::copy(dm_set1,dm_set2,Number<0>(),nn); }

    /// data dm_set2 will be copied from sum_{i in {0..nb_pond}} pond_list[i].first * pond_list[i].second.member(...)
    template<class Tpond,class DMSet1,class DMSet2>
    void get_ponderation( const std::pair<Tpond,const DMSet1 *> *pond_list, unsigned nb_pond, DMSet2 &dm_set2 ) {
        if ( nb_pond==0 ) return;
        DMPRIVATE::get_ponderation(pond_list,nb_pond,dm_set2,Number<0>(),Number<NbFields<DMSet1>::res>());
    }
    /// data dm_set2 will be copied from sum_{i in {0..nb_pond}} pond_list[i].first * pond_list[i].second.member(...)
    template<class Tpond,class DMSet1,class DMSet2,unsigned n>
    void get_ponderation_up_to( const std::pair<Tpond,const DMSet1 *> *pond_list, unsigned nb_pond, DMSet2 &dm_set2,const Number<n> &nn ) {
        if ( nb_pond==0 ) return;
        DMPRIVATE::get_ponderation(pond_list,nb_pond,dm_set2,Number<0>(),nn);
    }
    

    ///
    template<class DMSet,class P>
    void get_ptr_on( DMSet &dmset, const char *name, P *&res ) { DMPRIVATE::get_ptr_on( dmset, name, res, Number<0>(), Number<NbFields<DMSet>::res>() ); }

} // namespace DM

template<class NameDM>
struct ExtractDM {
    template<class TNode> struct ReturnType { typedef typename TNode::template SubTypeByName0<NameDM>::TT T; };
    template<class TNode> struct ReturnType<TNode *> { typedef typename TNode::template SubTypeByName0<NameDM>::TT T; };
    template<class TNode> const typename TNode::template SubTypeByName0<NameDM>::TT &operator()(const TNode &node) const { return node.member_named( n ); }
    template<class TNode> typename TNode::template SubTypeByName0<NameDM>::TT &operator()(TNode &node) const { return node.member_named( n ); }
    NameDM n;
};

template<class NameDM>
struct ExtractDMi {
    ExtractDMi( int index ) : index( index ) {}
    template<class TNode> struct ReturnType { typedef typename TNode::template SubTypeByName0<NameDM>::TT::template SubType<0>::T T; };
    template<class TNode> struct ReturnType<TNode *> { typedef typename TNode::template SubTypeByName0<NameDM>::TT::template SubType<0>::T T; };
    template<class TNode> const typename TNode::template SubTypeByName0<NameDM>::TT::template SubType<0>::T &operator()(const TNode &node) const { return node.member_named( n )[ index ]; }
    template<class TNode> typename TNode::template SubTypeByName0<NameDM>::TT::template SubType<0>::T &operator()(TNode &node) const { return node.member_named( n )[ index ]; }
    int    index;
    NameDM n;
};

} // namespace LMT

#endif
