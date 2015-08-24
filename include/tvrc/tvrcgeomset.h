//
// C++ Interface: tvrcgeomset
//
// Description: 
//
//
// Author: Hugo LECLERC <leclerc@lmt.ens-cachan.fr>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMT_tvrcgeomset_HEADER
#define LMT_tvrcgeomset_HEADER

#include "../geometry/geomset.h"
#include "../mesh/refinement.h"
#include "../containers/materialpoint.h"
#include "../util/discretevalues.h"
#include "../containers/heteroexplpack.h"
#include "tvrcmode.h"
#include "tvrcmaterial.h"
#include "meshcaractvrc.h"
#include "constraint.h"
//#include "../codegen/codegen.h"
#include <set>

namespace LMT {

// ------------------------------------------------------------------------------------------------------------------------
/// class that stores data for tvrc (geometry, modes, representation, ...)
template<class Carac>
class TvrcGeomSet {
public:
    struct CaracGeometry {
        static const unsigned dim = Carac::dim;
        typedef typename Carac::T T;
        template<unsigned nvi,unsigned inner=0> struct TData {
            unsigned num_in_matrix;
            Vec<Heterogeneous<typename Carac::template TModes<nvi>::Modes> > modes;
            Vec<Heterogeneous<typename Carac::template TConstraints<nvi>::Constraints> > constraints;
            typename Carac::template TMaterial<nvi> mat;
        };
    };
    typedef GeomSet<CaracGeometry> TGeomSet;
    static const unsigned dim = Carac::dim;
    typedef typename Carac::T T;
    typedef std::complex<T> TC;
    typedef Vec<T,dim> Pvec;
    typedef Mesh<Meshcaractvrc<dim,T> > TM;
    
    ///
    TvrcGeomSet():w(-1.0) {
        //         x_sym[0] = Codegen::Ex("x"); x_sym[1] = Codegen::Ex("y"); x_sym[2] = Codegen::Ex("z");
        //         xabs_sym[0] = Codegen::Ex("xabs"); xabs_sym[1] = Codegen::Ex("yabs"); xabs_sym[2] = Codegen::Ex("zabs");
        //         xHat_sym[0] = Codegen::Ex("X");    xHat_sym[1] = Codegen::Ex("Y");    xHat_sym[2] = Codegen::Ex("Z");
        //         w_sym = Codegen::Ex("w");
        //         imag_sym = Codegen::Ex("i");
    }
    ~TvrcGeomSet() {}
    
private:
    struct SetW {
        template<class TMode,class GE> void operator()(TMode &mode,GE *ge) const { mode.set_param_for_w(w,ge->data.mat,*ge); }
        template<class GE> void operator()(GE *ge) const { apply( ge->data.modes, *this, ge ); }
        T w;
    };
    struct MinWaveLength {
        template<class TMode> void operator()(const TMode &mode) { res = min(res,mode.get_min_wave_length()); }
        template<class GE> void operator()(const GE *ge,const Number<0> &fake) { apply( ge->data.modes, *this ); }
        T res;
    };
public:
    /// set k and mode parameters according to given w, geometry and materials...
    void set_w(T W) {
        w = W;
        // w_sym.set_val( W );
        //
        SetW sw;
        sw.w = W;
        apply( geomset.geometric_entities, sw );
    }
    /// look in each present modes the minimal wavelength
    T get_min_wavelength() const {
        MinWaveLength mw;
        mw.res = 1e40;
        apply( geomset.geometric_entities, mw, Number<0>() );
        return mw.res;
    }
private:
    struct ConstructModes {
        template<class TMode,class GE> void operator()(const StructForType<TMode> &nimp,GE &ge) const {
            if ( modes.nb_elements(TMode::name().c_str()) ) {
                XmlNode mn = modes.get_element(TMode::name().c_str());
                std::string apply_on = mn.get_attribute("apply_on") + " ";
                if ( apply_on=="* " or (apply_on.find( ge.ref+" " ))!=-1 )
                    TMode::push_dist( ge.data.modes, mn );
            }
        }
        template<class GE> void operator()(GE *ge) const { ge->data.modes.apply_static(*this,*ge); }
        XmlNode modes;
    };
    struct ConstructConstraints {
        template<class TConstraint,class GE> void operator()(const StructForType<TConstraint> &nimp,GE &ge) const {
            if ( constraint.get_name() == TConstraint::name() )
                ge.data.constraints.new_elem(StructForType<TConstraint>())->read_xml_data(constraint);
        }
        template<class GE> void operator()(GE *ge) const {
            std::string apply_on = constraint.get_attribute("apply_on") + " ";
            if ( (int)apply_on.find(ge->ref+" ") != -1 )
                ge->data.constraints.apply_static(*this,*ge);
        }
        XmlNode constraint;
    };
    struct ApplyMaterial {
        template<class GE> bool operator()(GE *ge,const std::string &name) const {
            DM::get_data_from_xml(ge->data.mat,defaultmat);
            if ( ge->ref != name )
                return false;
            DM::get_data_from_xml(ge->data.mat,localmat);
            return true;
        }
        XmlNode defaultmat, localmat;
    };
public:
    ///
    void read_xml_data(const XmlNode &sn) throw(IoException) {
        // read geometric data
        geomset.read_xml_data(sn);
        double angular_precision;
        sn.get_attribute("angular_precision",angular_precision,5);
        geomset.update_repr(angular_precision);
        
        // read materials
        readXmlMaterials(sn);
        
        // read modes
        ConstructModes cm;
        cm.modes = sn.get_element("modes");
        apply( geomset.geometric_entities, cm );
        
        // read constraints
        for(unsigned i=0;i<sn.get_element("constraints").nb_elements();++i) {
            ConstructConstraints cc;
            cc.constraint = sn.get_element("constraints").get_element_nb(i);
            apply( geomset.geometric_entities, cc );
        }
    }
private:
    ///
    void readXmlMaterials(const XmlNode &sn) throw(IoException) {
        const XmlNode &mn( sn.get_element("materials") );
        ApplyMaterial am;
        am.defaultmat = mn.get_element("default_values");
        for(unsigned i=0;i<mn.nb_elements("material");++i) {
            am.localmat = mn.get_element("material",i);
            std::istringstream apply_on( am.localmat.get_attribute("apply_on") );
            std::string a;
            while ( apply_on >> a ) {
                apply( geomset.geometric_entities, am, a );
                geomobj_for_which_mat_has_been_set.insert(a);
            }
        }
    }
    struct GetModeContribution {
        template<class TC,unsigned dim> void add_dep(const MaterialPoint<TC,dim> &dep) const {
            node->amp_real += LMT::real(dep.dep);
            node->amp_imag += LMT::imag(dep.dep);
            node->rot_real += LMT::real(dep.rot);
            node->rot_imag += LMT::imag(dep.rot);
        }
        template<class TC> void add_dep(const Vec<TC,2> &dep) const {
            node->amp_real += LMT::real(dep[0]);
            node->amp_imag += LMT::imag(dep[0]);
            node->rot_real += LMT::real(dep[1]);
            node->rot_imag += LMT::imag(dep[1]);
        }
        template<class TMode,class TNodeRepr> void operator()(const TMode &mode,TNodeRepr &node_repr) const {
            add_dep( mode.get_value( node_repr ) * mode.amplitude );
        }
        typename TM::TNode *node;
    };
    struct AppemdToRepr {
        template<class GE> void operator()(const GE *ge,TM &repr) {
            // tmp will be ge->repr refined
            typename GE::TM tmp;
            tmp.append( ge->repr );

            // refinement
            typename TM::Pvec xmin, xmax;
            get_min_max( tmp.node_list, ExtractDM<pos_DM>(), xmin, xmax );
            T mw = max( min_w / 16.0 , max(xmax-xmin) / 2000.0 );
            //unsigned cpt = 0;
            //while( refinement_if_length_sup(tmp,mw) && ++cpt<=3 );
            refinement_if_length_sup(tmp,mw);
            refinement_if_length_sup(tmp,mw);
            refinement_if_length_sup(tmp,mw);
            refinement_if_length_sup(tmp,mw);
            
            //             DisplayParaview dp;
            //             dp.add_mesh(tmp);
            //             dp.exec();
            
            // append to repr
            unsigned os = repr.node_list.size();
            repr.append( tmp );
            
            // set amp_... rot_...
            for(unsigned i=0;i<tmp.node_list.size();++i) {
                typename TM::TNode &node( repr.node_list[ i + os ] );
                node.amp_real = 0.0; node.amp_imag = 0.0;
                node.rot_real = 0.0; node.rot_imag = 0.0;
                // get contribution for each mode
                GetModeContribution gmc;
                gmc.node = &node;
                apply( ge->data.modes, gmc, tmp.node_list[i] );

                T amp = 0.0;
                for(unsigned j=0;j<dim;++j) {
                    node.amp_norm[j] = sqrt(pow(node.amp_real[j],2)+pow(node.amp_imag[j],2));
                    amp += node.amp_norm[j] * node.amp_norm[j];
                }
                for(unsigned j=0;j<dim-(dim==2);++j)
                    node.rot_norm[j] = sqrt(pow(node.rot_real[j],2)+pow(node.rot_imag[j],2));
                max_amplitude = max( max_amplitude, sqrt(amp) );
            }
        }
        T min_w, max_amplitude;
    };
public:
    /// update repr with mode data (do not update geometric representation from geomset but this->repr according to
    ///  mode amplitude and so on). repr depends on w.
    /// @return max amplitude
    T update_repr() {
        assert( w >= 0.0 );
        repr.free();
        
        T min_w = get_min_wavelength();
        AppemdToRepr ar;
        ar.min_w = min_w;
        ar.max_amplitude = 0.0;
        apply( geomset.geometric_entities, ar, repr );
        return ar.max_amplitude;
    }
    ///
    template<class DP> void add_geometry_data_to(DP &dp) const { geomset.add_display_data_to(dp); }
    /// return \f$ w \int A A^* \f$
    T get_power() const {
        assert(0.0);
        return 0.0;
    }
    
    /// frequency on which we are working
    T w;
    ///
    TM repr;
    ///
    TGeomSet geomset;
    /// used to discriminate geometric_entities (by ref) which have material data
    std::set<std::string> geomobj_for_which_mat_has_been_set;
    /// symbols which can be used in constraints.
    //     Codegen::Ex x_sym[3], xabs_sym[3], xHat_sym[3], w_sym, imag_sym;
};

};

#endif // LMT_tvrcgeomset_HEADER
