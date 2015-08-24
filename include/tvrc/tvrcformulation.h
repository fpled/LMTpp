//
// C++ Interface: tvrcformulation
//
// Description: 
//
//
// Author: Hugo LECLERC <leclerc@lmt.ens-cachan.fr>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMT_tvrcformulation_HEADER
#define LMT_tvrcformulation_HEADER

#include "../containers/mat.h"
#include "tvrcgeomset.h"
#include "../containers/arglist.h"
#include "../containers/apply_ij.h"

namespace LMT {

template<class Carac,class TypeFormulation,class THighPrec=double>
class TvrcFormulation {
public:
    static const unsigned dim = Carac::dim;
    typedef typename Carac::T T;
    typedef std::complex<T> TC;
    typedef std::complex<THighPrec> TLC;
    typedef Vec<T,dim> Pvec;
    typedef Vec<TC,dim> PvecC;
    typedef Vec<TC,dim-(dim==2)> RvecC;
    typedef typename TvrcGeomSet<Carac>::TGeomSet TGeomSet; 
    
    ///
    TvrcFormulation(TvrcGeomSet<Carac> *tvrc_geom_set_):tvrc_geom_set(tvrc_geom_set_) {
        epsilon_fact = 1e-16;
        repr_have_been_updated = false;
        output_system = false;
    }
    ///
    void read_xml_data(const XmlNode &sn) throw(IoException) {
        sn.get_attribute("output_system",output_system,false);
        sn.get_attribute("epsilon_fact",epsilon_fact,1e-6);
    }
    ///
    void display_mat(std::ostream &os,const char *n) const {
        os << "# Created by TVRC" << std::endl;
        os << "# name: " << n << std::endl;
        os << "# type: complex matrix" << std::endl;
        os << "# rows: " << mat.nb_rows() << std::endl;
        os << "# columns: " << mat.nb_cols() << std::endl;
        for(unsigned i=0;i<mat.nb_rows();++i) {
            for(unsigned j=0;j<mat.nb_rows();++j)
                os << LMT::real(mat(i,j)) << "+" << LMT::imag(mat(i,j)) << "i ";
            os << std::endl;
        }
    }
    ///
    void display_vec(std::ostream &os,const char *n) const {
        os << "# Created by TVRC" << std::endl;
        os << "# name: " << n << std::endl;
        os << "# type: complex matrix" << std::endl;
        os << "# rows: " << 1 << std::endl;
        os << "# columns: " << vec.size() << std::endl;
        for(unsigned i=0;i<vec.size();++i)
            os << LMT::real(vec[i]) << "+" << LMT::imag(vec[i]) << "i ";
        os << std::endl;
    }
private:
    struct UpdateVariableMo { template<class TMode> void operator()(TMode &mode,TC* &p) const { mode.amplitude = *(p++); } };
    struct UpdateVariableGe { template<class GE> void operator()(GE *ge,TC* &p) const { apply(ge->data.modes,UpdateVariableMo(),p); } };
public:
    /** put data in vec (assumed to be caller after solve). Set repr_have_been_updated to false
    */
    void update_variables() {
        // conversion 'high' precision complex to T complex
        Vec<TC> vecc = vec;
        // data in modes
        if ( vecc.size() ) {
            TC *p = &vecc[0];
            apply( tvrc_geom_set->geomset.geometric_entities, UpdateVariableGe(), p );
        }
        repr_have_been_updated = false;
    }
private:
    //
    struct GetMatVecSizeGe { template<class GE> void operator()(GE *ge,unsigned &res) const {
        ge->data.num_in_matrix = res;
        res += ge->data.modes.size();
    } };
    //
    template<class TCO,class TEP> struct AssembleConstraintsOnElement {
        template<class TM1,class TM2> void operator()(const TM1 &mode1,unsigned i,const TM2 &mode2,unsigned j) {
            if ( i==j )
                th->type_form.assemble_constraint(mode1,num_in_matrix+i,ep,*th,*constraint);
            th->type_form.assemble_constraint(mode1,num_in_matrix+i,mode2,num_in_matrix+j,ep,*th,*constraint);
        }
        TEP ep;
        const TCO *constraint;
        unsigned num_in_matrix;
        TvrcFormulation *th;
    };
    //
    struct AssembleConstraints {
        template<class TCO,class TE,class GE,class GEP> void operator()(const TCO &constraint,const TE &e,const ArgList2_0<GE,GEP> &al,unsigned toto) {
            typedef Element<typename TE::NE,typename TE::BE,typename GEP::GE::TM::TNode,typename TE::TD,0> TEP;
            AssembleConstraintsOnElement<TCO,TEP> ace;
            ace.constraint = &constraint; ace.th = th; ace.num_in_matrix = al.a1.ge->data.num_in_matrix;
            for(unsigned i=0;i<TE::nb_nodes;++i)
                ace.ep.nodes[i] = al.a1.proj_nodes[ e.node(i)->number_in_original_mesh() ];
            apply_ij( al.a1.ge->data.modes, al.a1.ge->data.modes, ace );
        }
        template<class TCO,class TE,class GE,class GEP1,class GEP2> void operator()(const TCO &constraint,const TE &e,const ArgList3_0<GE,GEP1,GEP2> &al,unsigned toto) {
            typedef Element<typename TE::NE,typename TE::BE,typename GEP2::GE::TM::TNode,typename TE::TD,0> TEP;
            AssembleConstraintsOnElement<TCO,TEP> ace;
            ace.constraint = &constraint; ace.th = th; ace.num_in_matrix = al.a2.ge->data.num_in_matrix;
            for(unsigned i=0;i<TE::nb_nodes;++i)
                ace.ep.nodes[i] = al.a2.proj_nodes[ al.a1.proj_nodes[ e.node(i)->number_in_original_mesh() ]->number_in_original_mesh() ];
            apply_ij( al.a2.ge->data.modes, al.a2.ge->data.modes, ace );
        }
        template<class TCO,class TE,class GE,class GEP1,class GEP2,class GEP3> void operator()(const TCO &constraint,const TE &e,const ArgList4_0<GE,GEP1,GEP2,GEP3> &al,unsigned toto) {
            typedef Element<typename TE::NE,typename TE::BE,typename GEP3::GE::TM::TNode,typename TE::TD,0> TEP;
            AssembleConstraintsOnElement<TCO,TEP> ace;
            ace.constraint = &constraint; ace.th = th; ace.num_in_matrix = al.a3.ge->data.num_in_matrix;
            for(unsigned i=0;i<TE::nb_nodes;++i)
                ace.ep.nodes[i] = al.a3.proj_nodes[ al.a2.proj_nodes[ al.a1.proj_nodes[ e.node(i)->number_in_original_mesh() ]->number_in_original_mesh() ]->number_in_original_mesh() ];
            apply_ij( al.a3.ge->data.modes, al.a3.ge->data.modes, ace );
        }
        
        template<class TE,class AL> void operator()(const TE &e,const AL &al,unsigned toto) {
            apply( al.a0.data.constraints, *this, e, al, 1 );
        }
        template<class GEPar,class AL> void operator()(const GEPar &gepar,const AL &al) {
            apply( gepar.ge->parents, *this, append_to_arglist(al,gepar) ); // parents level n+1
            
            apply( al.a0.repr.elem_list, *this, append_to_arglist(al,gepar), 1 );
        }
        template<class GE> void operator()(const GE *ge) { if( ge->data.constraints.size() ) apply( ge->parents, *this, arglist(*ge) ); }
        TvrcFormulation *th;
    };
    //
    struct AssembleLinksTerII {
        template<class TM1,class TM2,class AL> void operator()(const TM1 &m1,unsigned i,const TM2 &m2,unsigned j,const AL &elems) {
            th->type_form.assemble_link_mode_ii(m1,num_in_matrix1+i,m2,num_in_matrix2+j,elems.a0,*th);
        }
        TvrcFormulation *th;
        unsigned num_in_matrix1, num_in_matrix2;
    };
    struct AssembleLinksTerIJ {
        template<class TM1,class TM2,class AL> void operator()(const TM1 &m1,unsigned i,const TM2 &m2,unsigned j,const AL &elems) {
            th->type_form.assemble_link_mode_ij(m1,num_in_matrix1+i,elems.a0,m2,num_in_matrix2+j,elems.a1,*th);
        }
        TvrcFormulation *th;
        unsigned num_in_matrix1, num_in_matrix2;
    };
    //
    template<class GE> struct AssembleLinksBis {
        template<class TE,class GP1,class GP2> void operator()(const TE &e,const GP1 &p1,const GP2 &p2) {
            typedef Element<typename TE::NE,typename TE::BE,typename GP1::GE::TM::TNode,typename TE::TD,0> TEP1;
            typedef Element<typename TE::NE,typename TE::BE,typename GP2::GE::TM::TNode,typename TE::TD,0> TEP2;
            TEP1 ep1;
            TEP2 ep2;
            for(unsigned i=0;i<TE::nb_nodes;++i) {
                ep1.nodes[i] = p1.proj_nodes[ e.node(i)->number_in_original_mesh() ];
                ep2.nodes[i] = p2.proj_nodes[ e.node(i)->number_in_original_mesh() ];
            }
            
            // sim modes
            if ( (void *)&p1==(void *)&p2 ) {
                AssembleLinksTerII  ata; ata.th = th;
                ata.num_in_matrix1 = p1.ge->data.num_in_matrix; ata.num_in_matrix2 = p1.ge->data.num_in_matrix;
                apply_ij(p1.ge->data.modes,p1.ge->data.modes,ata,arglist(ep1,ep1)); // _supeq_
            }
            else {
                AssembleLinksTerIJ ats; ats.th = th;
                ats.num_in_matrix1 = p1.ge->data.num_in_matrix; ats.num_in_matrix2 = p2.ge->data.num_in_matrix;
                apply_ij(p1.ge->data.modes,p2.ge->data.modes,ats,arglist(ep1,ep2));
            }
            //ata.num_in_matrix1 = p2.ge->data.num_in_matrix; ata.num_in_matrix2 = p2.ge->data.num_in_matrix;
            //apply_ij(p2.ge->data.modes,p2.ge->data.modes,ata,arglist(ep2,ep2)); // _supeq_
            
        }
        template<class GEPar1,class GEPar2> void operator()(const GEPar1 &p1,unsigned i,const GEPar2 &p2,unsigned j) { apply(ge->repr.elem_list,*this,p1,p2); }
        const GE *ge;
        TvrcFormulation *th;
    };
    //
    struct AssembleLinks {
        template<class GE> void operator()(const GE *ge,TvrcFormulation *th) {
            if( ge->parents.size()>=2 ) {
                AssembleLinksBis<GE> al; al.ge = ge; al.th = th;
                apply_ij(ge->parents,ge->parents,al); // 
            }
        }
    };
public:
    /** Update global matrix and sollicitation vector. Called by solve.
        
        Algorithm for constraints assembly :
        
        \code
for ge in geometric_entities:
    for gpar in parents of ge ( ex : parent surfaces ):
        for e in elements from ge ( ex : bar for a curve ):
            for C in constraints on ge:
                for m1,m2 in modes**2 of gpar:
                    if m1==m2:
                        add integral on e on dot(m1,C.dir)*C.v to formulation->vec
                    add integral on e on dot(m1,C.dir)*dot(m2,C.dir) to formulation->mat
        \endcode
     */
    void assemble() {
        min_wavelength = tvrc_geom_set->get_min_wavelength();
        allocate_mat_vec();
        
        // constraints
        AssembleConstraints ac;
        ac.th = this;
        apply( tvrc_geom_set->geomset.geometric_entities, ac );
        // links
        AssembleLinks al;
        apply( tvrc_geom_set->geomset.geometric_entities, al, this );
        
        add_epsilon_to_diag();
    }
    /// called by solve
    bool solve_system() {
        vec = inv(mat) * vec;
        return true;
    }
    /// called by assemble()
    void allocate_mat_vec() {
        unsigned size = 0;
        apply( tvrc_geom_set->geomset.geometric_entities, GetMatVecSizeGe(), size );

        //Vec<unsigned> lbounds;
        //lbounds.resize( size, 0 );
        mat.resize( size, size/*, lbounds*/ );
        mat.set((TC)0.0);
        
        vec.resize(size);
        vec.set((TC)0.0);
    }
private:
    struct GetResidualConstraints {
        struct XR { template<class TM,class AL> void operator()(const TM &m,const AL &al,T &res) const {
            res -= 2.0 * std::real( m.amplitude * dot_integration_on_elem( m, al.a0, al.a1, al.a1, al.a2 ) );
        } };
        template<class TM1,class TM2,class AL> void operator()(const TM1 &m1,unsigned i,const TM2 &m2,unsigned j,const AL &al) {
            res += std::real( m1.amplitude * std::conj(m2.amplitude) * dot_integration_with_constraint_dir_on_elem(m1,m2,al.a0,al.a1,al.a1,th->min_wavelength) );
        }
        template<class TCO,class TE,class GEPar,class GE> void operator()(const TCO &constraint,const TE &e,const GEPar &gepar,const GE *ge) {
            res += std::real( dot_integration_on_elem( constraint, constraint, e, e, th->min_wavelength ) );
            apply_ij( gepar.ge->data.modes, gepar.ge->data.modes, *this, arglist(constraint,e) ); // 
            apply(gepar.ge->data.modes,XR(),arglist(constraint,e,th->min_wavelength),res);
        }
        template<class TE,class GEPar,class GE> void operator()(const TE &e,const GEPar &gepar,const GE *ge) {
            typedef Element<typename TE::NE,typename TE::BE,typename GEPar::GE::TM::TNode,typename TE::TD,0> TEP;
            TEP ep;
            for(unsigned i=0;i<TE::nb_nodes;++i)
                ep.nodes[i] = gepar.proj_nodes[ e.node(i)->number_in_original_mesh() ];
            apply(ge->data.constraints,*this,ep,gepar,ge);
        }
        template<class GEPar,class GE> void operator()(const GEPar &gepar,const GE *ge) { apply(ge->repr.elem_list,*this,gepar,ge); }
        template<class GE> void operator()(const GE *ge) { if( ge->data.constraints.size() ) apply(ge->parents,*this,ge); }
        const TvrcFormulation *th;
        T res;
    };
    //
    //
    template<bool add> struct GetResidualLinksTer {
        GetResidualLinksTer() { res=0.0; }
        template<class TM1,class TM2,class AL> void operator()(const TM1 &m1,unsigned i,const TM2 &m2,unsigned j,const AL &elems) {
            T v = std::real( m1.amplitude * std::conj(m2.amplitude) * dot_integration_on_elem(m1,m2,elems.a0,elems.a1,th->min_wavelength) );
            if ( add ) res += v;
            else       res -= 2.0 * v;
        }
        const TvrcFormulation *th;
        T res;
    };
    //
    template<class GE> struct GetResidualLinksBis {
        GetResidualLinksBis() { res=0.0; }
        template<class TE,class GP1,class GP2> void operator()(const TE &e,const GP1 &p1,const GP2 &p2) {
            typedef Element<typename TE::NE,typename TE::BE,typename GP1::GE::TM::TNode,typename TE::TD,0> TEP1;
            typedef Element<typename TE::NE,typename TE::BE,typename GP2::GE::TM::TNode,typename TE::TD,0> TEP2;
            TEP1 ep1;
            TEP2 ep2;
            for(unsigned i=0;i<TE::nb_nodes;++i) {
                ep1.nodes[i] = p1.proj_nodes[ e.node(i)->number_in_original_mesh() ];
                ep2.nodes[i] = p2.proj_nodes[ e.node(i)->number_in_original_mesh() ];
            }
            // sim modes
            GetResidualLinksTer<true>  ata; ata.th = th;
            apply_i_supeq_j(p1.ge->data.modes,p1.ge->data.modes,ata,arglist(ep1,ep1));
            apply_i_supeq_j(p2.ge->data.modes,p2.ge->data.modes,ata,arglist(ep2,ep2));
            
            GetResidualLinksTer<false> ats; ats.th = th;
            apply_ij(p1.ge->data.modes,p2.ge->data.modes,ats,arglist(ep1,ep2));
            
            res += ats.res + ata.res;
        }
        template<class GEPar1,class GEPar2> void operator()(const GEPar1 &p1,unsigned i,const GEPar2 &p2,unsigned j) { apply(ge->repr.elem_list,*this,p1,p2); }
        const GE *ge;
        const TvrcFormulation *th;
        T res;
    };
    //
    struct GetResidualLinks {
        template<class GE> void operator()(const GE *ge,const TvrcFormulation *th) {
            if( ge->parents.size()>=2 ) {
                GetResidualLinksBis<GE> al; al.ge = ge; al.th = th;
                apply_i_sup_j(ge->parents,ge->parents,al);
                res += al.res;
            }
        }
        T res;
    };
public:
    /**
        assumes that update_variables has been called.
        
        Call tvrc_geom_set->update_repr()
        \todo Make MaterialPoint<> dimensionnaly consistent
     */
    T get_residual() const {
        // constraints
        GetResidualConstraints grc; grc.th = this; grc.res = 0.0;
        apply( tvrc_geom_set->geomset.geometric_entities, grc );
        // links
        GetResidualLinks grl; grl.res = 0.0;
        apply( tvrc_geom_set->geomset.geometric_entities, grl, this );
        return grc.res + grl.res;
    }
    /// add max(abs(diag))*epsilon_fact to all elements of diag
    void add_epsilon_to_diag() {
        T m = 0.0;
        for(unsigned i=0;i<mat.nb_cols();++i) m = max( m, LMT::abs(mat(i,i)) );
        if ( m==0.0 ) m = 1.0;
        m *= epsilon_fact;
        for(unsigned i=0;i<mat.nb_cols();++i) mat(i,i) += m;
    }
    ///
    void solve() {
        assemble();
        if ( output_system ) {
            display_mat(std::cout,"A");
            display_vec(std::cout,"V");
        }
        if ( solve_system() == false )
            std::cerr << "# error while solving system" << std::endl;
        else
            update_variables();
        if ( output_system )
            display_vec(std::cout,"res");
        
        residual = get_residual();
    }
    
    
    TvrcGeomSet<Carac> *tvrc_geom_set;
    //Mat<TLC,Herm<>,SkyLine<> > mat;
    Mat<TLC> mat;
    Vec<TLC> vec;
    T min_wavelength, epsilon_fact, residual;
    bool repr_have_been_updated, output_system;

    TypeFormulation type_form;
};

};

#endif // LMT_tvrcformulation_HEADER
