//
// C++ Interface: geomset
//
// Description: 
//
//
// Author: Hugo LECLERC <leclerc@lmt.ens-cachan.fr>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMT_geomset_HEADER
#define LMT_geomset_HEADER

#include "geometricentity.h"
#include "stdsurfaces.h"
#include "stdcurves.h"
#include "volume.h"
#include "../mesh/refinement.h"
//#include "../mesh/meshcaracstd.h"

namespace LMT {

template<class Carac>
class GeomSet {
public:
    typedef typename Carac::T T;
    static const unsigned dim = Carac::dim;
    typedef Vec<T,dim> Pvec;
private:
    struct DeleteGE { template<class GE> void operator()(GE *ge) const { delete ge; } };
    struct UpdateRepr { template<class GE> void operator()(GE *ge,T ar) const { ge->update_repr(ar); } };
public:
    ///
    ~GeomSet() { apply( geometric_entities, DeleteGE() ); }

    ///
    const char *type_geom_ent(unsigned num_type) {
        const char *types[] = { "point", "curve", "surface", "volume", "geom_4d", "geom_5d", "geom_6d" };
        return types[num_type];
    }
    ///
    struct GeomEntName {
        template<unsigned nvi,unsigned inner=0> struct SubType { typedef GeometricEntity<Carac,nvi> *T; };
        template<unsigned inner> struct SubType<dim+1,inner> { typedef void T; };
    };
    /// contain volumes...
    Vec<Heterogeneous<GeomEntName> > geometric_entities;
    
    ///
    void read_xml_data(const XmlNode &sn) throw(IoException) {
        read_xml_data_rec(sn,Number<0>());
    }
    
    ///
    void update_repr(T angular_prec_in_degrees) { apply( geometric_entities, UpdateRepr(),angular_prec_in_degrees); }

    ///
    template<unsigned nvi> GeometricEntity<Carac,nvi> *get_geom_ent_with_ref(const std::string &t) {
        typename Vec<Heterogeneous<GeomEntName> >::template SubVec<nvi>::TS &vec = geometric_entities.sub_vec(Number<nvi>());
        for(unsigned i=0;i<vec.size();++i)
            if ( vec[i]->get_ref() == t )
                return vec[i];
        return NULL;
    }
protected:
    ///
    void read_xml_data_rec(const XmlNode &sn,const Number<dim+1> &nn) throw(IoException) {}
    ///
    template<unsigned n> void read_xml_data_rec(const XmlNode &sn,const Number<n> &nn) throw(IoException) {
        std::string name_elems = type_geom_ent(n)+std::string("s");
        if ( sn.nb_elements( name_elems.c_str() ) ) {
            /// data for XmlFactory
            if ( ! factory.has_obj_of_type(type_geom_ent(n)) ) register_std_geom_ent<Carac>(nn);
            
            const XmlNode &xn = sn.get_element( name_elems.c_str() );
            unsigned nb_e = xn.nb_elements();
            geometric_entities.sub_vec(Number<n>()).resize( nb_e );
            for(unsigned i=0;i<nb_e;++i) {
                try {
                    xn.get_element_nb(i).get_object_with_parameter( geometric_entities.data(i,Number<n>()), this, type_geom_ent(n) );
                }
                catch(const IoException &e) {
                    geometric_entities.sub_vec(Number<n>()).resize( i );
                    throw e;
                }
            }
        }
        
        read_xml_data_rec(sn,Number<n+1>());
    }
    struct GetRepr { template<class GE> void operator()(const GE *ge,DisplayParaview &dp) const { dp.add_mesh(ge->repr); } };
    struct AppendRepr { template<class GE,class TM> void operator()(const GE *ge,TM &m) const { m.append(ge->repr); } };
    struct RefineRepr { template<class GE> void operator()(GE *ge,T dist) const { refinement_if_length_sup(ge->repr,dist); refinement_if_length_sup(ge->repr,dist); } };
public:
    template<class DP> void add_display_data_to(DP &dp) const { apply( geometric_entities, GetRepr(), dp ); }
    template<unsigned nvi,class DP> void add_partial_display_data_to(DP &dp) const {
        typename GeometricEntity<Carac,nvi>::TM m;
        apply( geometric_entities.sub_vec(Number<nvi>()), AppendRepr(), m );
        dp.add_mesh( m );
    }
    void refine_repr(T dist) {
        apply( geometric_entities, RefineRepr(), dist );
    }
};


};

#endif // LMT_geomset_HEADER
