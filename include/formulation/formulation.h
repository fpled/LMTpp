//
// C++ Interface: formulation
//
// Description:
//
//
// Author: Hugo LECLERC <hugo.leclerc@lmt.ens-cachan.fr>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMT_formulation_HEADER
#define LMT_formulation_HEADER

#include "../mesh/mesh.h"

#include "../containers/heterogeneouspack.h"
#include "convergence_criteria.h"
#include "constraint.h"
#include "sollicitation.h"
#include "../util/symamd.h"
#include "../util/symrcm.h"
#include "../util/solveLDL.h"
#include "../util/MUMPS_solver.h"
#include "formulation_ancestor.h"
#include "../containers/matcholamd.h"
#include "../containers/matumfpack.h"

namespace LMT {

template<class NameFormulation, int dim, class ScalarType_>
struct LocalOperator {
    template<class TM,class TF>
    void local_update(TM *m ,TF *f){  };
    template<class TM>
    void update_variables(TM *m){ };
};

/*! To be redefined for each new formulations */
template<class TF>
void read_material_to_mesh_( const XmlNode &n, TF &f ) {}

/**
*/
template<class NameFormulation,unsigned dim,class T,class NameVariant=DefaultBehavior>
class CaracFormulation {
public:
    static const unsigned nb_nodal_unknowns = 0; /// number of nodal unknowns
    static const unsigned nb_global_unknowns = 0; /// number of global unknowns
    static const int nb_vectors = 4; /// if -1, nb is dynamic
    static const int nb_matrices = 4; /// if -1, nb is dynamic
    static const bool matrix_will_be_definite_positive = true; /// in order to choose resolution algorithm (but if lagrange constraints,
                                                               /// matrix will not be definite_positive).
    static const bool all_vector_should_be_cleared = false; ///

    template<unsigned num_mat,unsigned inner=0> struct NodalMatricesCarac {
        static const bool symm = true;
        static const bool herm = false;
        static const bool diag = false;
    };
    template<unsigned num_mat,unsigned inner=0> struct GlobalMatricesCarac {
        static const bool symm = true;
        static const bool herm = false;
        static const bool diag = false;
    };
};
/**
*/
template<class NameFormulation,class TElem,class NameVariant,class T>
class CaracFormulationForElement {
public:
    static const unsigned nb_elementary_unknowns = 0; /// number of unknowns associated to this element
    template<unsigned num_mat,unsigned inner=0> struct ElemMatricesCarac {
        static const bool symm = true;
        static const bool herm = false;
        static const bool diag = false;
    };
};

/*!
    Pour le choix du solveur :
    
    - si wont_add_nz=true (utilisation d'un solveur externe si possible (CholMod si sym=true, UMFPACK si sym=false par exemple), solveur interne (LDL, Inv,...) sinon),
        - si sym=false (pas CholMod, pas LDL), appel du solveur UMFPACK (si "-DWITH_UMFPACK" dans directive de compil) ou MUMPS (si "-DWITH_MUMPS" dans directive de compil) ou Inv (sinon) -> il faudrait peut-être inverser l'appel entre MUMPS et UMFPACK (si "-DWITH_UMFPACK -DWITH_MUMPS" dans directive de compil), car il me semble que MUMPS est plus rapide que UMFPACK, quand penses-tu Hugal ? Ca vaudrait le coup de faire un test...
        - si sym=true, appel du solveur CholMod (si "-DWITH_CHOLMOD" dans directive de compil) ou MUMPS (si "-DWITH_MUMPS" dans directive de compil) ou LDL (si "-DWITH_LDL" dans directive de compil) ou Inv (sinon) -> OK, cas classique.
        
    - si wont_add_nz=false (utilisation d'un solveur interne, donc pas CholMod, pas UMFPACK, pas MUMPS qui font appel à des librairies externes),
        - si sym=false (pas CholdMod, pas LDL), appel du solveur Inv -> à réfléchir, c'est très lent, pas efficace...
        - si sym=true, appel du solveur MUMPS (si "-DWITH_MUMPS" dans directive de compil) ou LDL (si "-DWITH_LDL" dans directive de compil) ou Inv (sinon) -> là on fait appel au solveur MUMPS bien que ce soit un solveur externe, car si "-DWITH_MUMPS -DWITH_LDL" dans directive de compil, on privilegie MUMPS
        
    Mais qu'est-ce qu'une directive de compilation ?
        cf. CPPFLAGS dans vasoSConstuction

    \relates Comment resoudre un probleme elements finis de facon generale
    \relates Exemple de probleme d'evolution, l'equation de la chaleur
    \relates Exemple de formulation

    \keyword Formulation
*/
template<class TM_,class NameFormulation_,class NameVariant_=DefaultBehavior,class ScalarType_=typename TM_::Tpos,bool wont_add_nz=false>
class Formulation : public FormulationAncestor<ScalarType_> {
public:
    typedef TM_ TM;
    typedef NameFormulation_ NameFormulation;
    typedef NameVariant_ NameVariant;
    typedef ScalarType_ ScalarType;
    typedef CaracFormulation<NameFormulation,TM::dim,ScalarType,NameVariant> Carac;
    static const unsigned nb_nodal_unknowns = Carac::nb_nodal_unknowns;
    static const unsigned nb_global_unknowns = Carac::nb_global_unknowns;
    static const unsigned nb_vectors = Carac::nb_vectors + 16;
    static const unsigned nb_der_var = Carac::nb_der_var;
    typedef TM TMESH;
    typedef typename FormulationAncestor<ScalarType>::LinearizedConstraint LinearizedConstraint;

    Formulation( TM &mm ) : m( &mm ) {
        init();
    }

    Formulation() : m( 0 ) {
        init();
    }

    void init() {
        localOP = new LocalOperator<NameFormulation, TM::dim, ScalarType>;
        mat_def_pos_if_sym = Carac::matrix_will_be_definite_positive;
        time = ScalarType(0);
        time_steps = ScalarType(1e40);
        initial_condition_initialized = false;
        user_want_pierre_precond = true;
        //this->order_integration_when_integration_totale = Carac::order_integration;

        time_symbol = Codegen::symbol("time");
        symbols.push_back( time_symbol );

        allocated = false;
        vectors_assembly = NULL;
        indice_elem = indice_elem_internal;
        indice_noda = &indice_noda_internal;
        indice_glob = &indice_glob_internal;
        f_nodal = &f_nodal_internal;
        offset_lagrange_multipliers = 0;
        
        is_numerically_enriched = false;
    }
    virtual std::string get_name() const { return Carac::name(); }
    virtual void set_mesh( void *m_ ) { m = reinterpret_cast<TM *>( m_ ); }
    virtual unsigned get_nb_nodal_unknowns() { return nb_nodal_unknowns; }
    virtual unsigned get_nb_global_unknowns(){ return nb_global_unknowns; }
    virtual unsigned get_nb_vectors(){ return nb_vectors; }

private:
#if WITH_MUMPS
    MUMPS_solver solver;
#elif WITH_LDL
    LDL_solver solver;
#endif
    template<unsigned nm,unsigned n,unsigned tne> struct MatCaracElem {
        typedef typename TM::TElemList::template SubType<n>::T TE;
        typedef CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType> CFE;
        static const bool symm = ((CFE::nb_elementary_unknowns+nb_nodal_unknowns)==0 || CFE::template ElemMatricesCarac<nm>::symm) && MatCaracElem<nm,n+1,tne>::symm;
        static const bool herm = ((CFE::nb_elementary_unknowns+nb_nodal_unknowns)==0 || CFE::template ElemMatricesCarac<nm>::herm) && MatCaracElem<nm,n+1,tne>::herm;
        static const bool diag = ((CFE::nb_elementary_unknowns+nb_nodal_unknowns)==0 || CFE::template ElemMatricesCarac<nm>::diag) && MatCaracElem<nm,n+1,tne>::diag;
    };
    template<unsigned nm,unsigned tne> struct MatCaracElem<nm,tne,tne> {
        static const bool symm = true;
        static const bool herm = true;
        static const bool diag = true;
    };
public:
    template<unsigned num_mat> struct MatCarac {
        static const bool symm =
            (nb_nodal_unknowns==0 || Carac::template NodalMatricesCarac<num_mat>::symm) &&
            (nb_global_unknowns==0 || Carac::template GlobalMatricesCarac<num_mat>::symm) &&
            MatCaracElem<num_mat,0,TM::TElemList::nb_sub_type>::symm;
        static const bool herm =
            (nb_nodal_unknowns==0 || Carac::template NodalMatricesCarac<num_mat>::herm) &&
            (nb_global_unknowns==0 || Carac::template GlobalMatricesCarac<num_mat>::herm) &&
            MatCaracElem<num_mat,0,TM::TElemList::nb_sub_type>::herm;
        static const bool diag =
            (nb_nodal_unknowns==0 || Carac::template NodalMatricesCarac<num_mat>::diag) &&
            (nb_global_unknowns==0 || Carac::template GlobalMatricesCarac<num_mat>::diag) &&
            MatCaracElem<num_mat,0,TM::TElemList::nb_sub_type>::diag;
    };
    void Get_Nb_UnknownByElement( unsigned* nb_elem_of_type, unsigned &size, unsigned* nb_unknowns_for_type){
        TM::TElemList::apply_static_with_n( GetNbUnknownByElement(), nb_elem_of_type, size, nb_unknowns_for_type );
    }
private:
    struct GetNbUnknownByElement {
        template<class TE,unsigned nt> void operator()(const StructForType<TE> &se,const Number<nt> &nv,unsigned *nb_elem_of_type,
                unsigned &size,unsigned *nb_unknowns_for_type) const {
            nb_unknowns_for_type[nt] = CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType>::nb_elementary_unknowns;
            size += nb_elem_of_type[nt] * nb_unknowns_for_type[nt];
        }
    };
    struct ResizeMat {
        struct GetNZ {
            template<class TE> void operator()(const TE &e,Formulation &f,Vec<Vec<unsigned> > &v) const {
//                 assert( (CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType>::nb_elementary_unknowns) == 0 );
                if ( nb_nodal_unknowns ) {
                    for(unsigned i=0;i<TE::nb_nodes;++i) {
                        unsigned a = (*f.indice_noda)[ f.m->node_list.number(*e.node(i)) ];
                        for(unsigned j=0;j<TE::nb_nodes;++j) {
                            unsigned b = (*f.indice_noda)[ f.m->node_list.number(*e.node(j)) ];
                            if ( std::find(v[a].begin(),v[a].end(),b)==v[a].end() ) {
                                for(unsigned k=0;k<nb_nodal_unknowns;++k)
                                    for(unsigned l=0;l<nb_nodal_unknowns;++l)
                                        v[a+k].push_back( b+l );
                            }
                        }
                    }
                }
            }
        };

        template<class TM2> void operator()(TM2 &m,unsigned s,Formulation &f) const {
            Vec<Vec<unsigned> > v; v.resize( s );
            for(unsigned i=0;i<s;++i) v[i].reserve( 32 );
            apply( f.m->elem_list, GetNZ(), f, v ); // loop on all elements to find non zero terms
            for(unsigned i=0;i<s;++i) if (v[i].size() == 0) v[i].push_back(i);
            for(unsigned i=0;i<s;++i) std::sort( v[i].begin(), v[i].end() );

            //             std::cout << "size -> " << s << std::endl;
            m.allocate(v);
            // m.resize(s,s);
        }
        // /// this kind of matrice need pre-allocation
        //         void operator()(Mat<double, Sym<>, SparseCholMod> &m,unsigned s,Formulation &f) const {
        //             Vec<Vec<unsigned> > v; v.resize( s );
        //             for(unsigned i=0;i<s;++i) v[i].reserve( 32 );
        //             apply( f.m->elem_list, GetNZ(), f, v ); // loop on all elements to find non zero terms
        //             for(unsigned i=0;i<s;++i) std::sort( v[i].begin(), v[i].end() );
        //
        //             m.allocate(v);
        //             //m.print();
        //         }
    };

    struct ClearMat { template<class TM2> void operator()(TM2 &m) const { m.clear(); } };
public:
    struct PackMatrices {
        template<unsigned n> struct SubType {
            typedef typename AlternativeOnType< (n<Carac::nb_matrices),
                void,
                Mat<
                    ScalarType,
                    typename AlternativeOnType< MatCarac<n>::diag,
                        typename AlternativeOnType< MatCarac<n>::symm + 2 * MatCarac<n>::herm,
                            Gen<>,
                            Sym<>,
                            Herm<>
                        >::T,
                        Diag<>
                    >::T,
                    typename AlternativeOnType< MatCarac<n>::symm,
                        typename AlternativeOnType< wont_add_nz, SparseLU,
                            #ifdef WITH_UMFPACK
                            SparseUMFPACK
                            #else
                            SparseLU
                            #endif
                        >::T,
                        typename AlternativeOnType< wont_add_nz, SparseLine<>,
                            #ifdef WITH_CHOLMOD
                            SparseCholMod
                            #else
                            SparseLine<>
                            #endif
                        >::T
                    >::T
                >
            >::T T;
        };
    };
    typedef typename PackMatrices::template SubType<0>::T TMAT0;
    /**
     * Set matrix and vector sizes...
     * if amd, use a permutation able to minimize fill-in sparse mat... in this case, use chol_factorize and solve_using_chol_factorize
     */
    virtual unsigned update_connectivity() {
        mat_has_been_allocated_with_symamd = this->want_amd;
        mat_has_been_allocated_with_symrcm = this->want_rcm;

        unsigned size = 0;
        // global unknowns
        unsigned ng = nb_global_unknowns;
        size += ng;
        // nodal unknowns
        unsigned nn = m->node_list.size() * nb_nodal_unknowns;
        size += nn;
        // elem unknowns. nb_elem_of_type will = nb_unknowns
        unsigned nb_elem_of_type[ TM::TElemList::nb_sub_type ];
        unsigned nb_unknowns_for_type[ TM::TElemList::nb_sub_type ];
        m->elem_list.get_sizes(nb_elem_of_type);
        TM::TElemList::apply_static_with_n( GetNbUnknownByElement(), nb_elem_of_type, size, nb_unknowns_for_type );
        // indices
        *indice_glob = 0;
        unsigned nnu = nb_nodal_unknowns;
        unsigned ngu = nb_global_unknowns;
        *indice_noda = range( m->node_list.size() ) * nnu + ngu;
        //std::cout << "indice_noda " << indice_noda << std::endl;
        unsigned ind = m->node_list.size() * nnu + ngu;
        unsigned nb_unk_elem = 0;

        for(unsigned i=0,ne=0;i<TM::TElemList::nb_sub_type;++i) {
            indice_elem[i] = range( ne, ne + nb_elem_of_type[i] ) * nb_unknowns_for_type[i] + ind;
            ne += nb_elem_of_type[i];
            ind += nb_unknowns_for_type[i] * nb_elem_of_type[i];
            nb_unk_elem += nb_unknowns_for_type[i];
        }
        if ( this->want_amd ) {
            assert( (*indice_glob)==0 ); // not managed
            assert( nb_unk_elem==0 ); // not managed
            *indice_noda = symamd( *m ) * nnu;
        }
        if ( this->want_rcm ) {
            assert( (*indice_glob)==0 ); // not managed
            assert( nb_unk_elem==0 ); // not managed
            *indice_noda = symrcm( *m ) * nnu;
        }
        return size;
    }
    //
    virtual void free_matrices() {
        allocated = false;
        matrices( Number<0>() ).free();
        matrices( Number<1>() ).free();
        matrices( Number<2>() ).free();
        matrices( Number<3>() ).free();
        sollicitation.free();
        for(unsigned i=0;i<vectors.size();++i)
            vectors[i].free();
    }
    //
    virtual void allocate_matrices(bool allocate_mat=true,bool allocate_vec=true) {
        if ( allocated )
            return;
        allocated = true;
        mat_has_been_allocated_with_symamd = this->want_amd;
        mat_has_been_allocated_with_symrcm = this->want_rcm;

        unsigned di = m->dim;
        unsigned size = 0;
        // global unknowns
        unsigned ng = nb_global_unknowns;
        size += ng;
        // nodal unknowns
        unsigned nn = m->node_list.size() * nb_nodal_unknowns;
        size += nn;
        // elem unknowns. nb_elem_of_type will = nb_unknowns
        unsigned nb_elem_of_type[ TM::TElemList::nb_sub_type ];
        unsigned nb_unknowns_for_type[ TM::TElemList::nb_sub_type ];
        m->elem_list.get_sizes( nb_elem_of_type );
        TM::TElemList::apply_static_with_n( GetNbUnknownByElement(), nb_elem_of_type, size, nb_unknowns_for_type );
//         if (is_numerically_enriched) {
//             for(int i=0;i<m->elem_list.size();i++)
//                 size += (interface.elem_enrichment_ID[i].size() - 1 )*3;
//         }

        //
        offset_lagrange_multipliers = size;
        for(unsigned i=0;i<constraints.size();++i) size += ( constraints[i].penalty_value == ScalarType(0) );

        // resize vectors
        for(unsigned i=0;i<vectors.size();++i)
            vectors[i].resize( size );
        sollicitation.resize( size );

        // indices
        *indice_glob = 0;
        unsigned nnu = nb_nodal_unknowns;
        unsigned ngu = nb_global_unknowns;
        *indice_noda = range( m->node_list.size() ) * nnu + ngu;
        unsigned ind = m->node_list.size() * nnu + ngu;
        unsigned nb_unk_elem = 0;

        for(unsigned i=0,ne=0;i<TM::TElemList::nb_sub_type;++i) {
//             if (is_numerically_enriched) {
//                 indice_elem[i].resize(m->elem_list.size());
//                 unsigned sum = 0;
//                 for (int j=0;j<m->elem_list.size();++j) {
//                     indice_elem[i][j] = sum + ind;
//                     sum +=(interface.elem_enrichment_ID[j].size())*3;
//                 }
//                 ind += sum * nb_elem_of_type[i];
//                 nb_unk_elem += sum;
//             } else {
                indice_elem[i] = range( ne, ne + nb_elem_of_type[i] ) * nb_unknowns_for_type[i] + ind;
                ne += nb_elem_of_type[i];
                ind += nb_unknowns_for_type[i] * nb_elem_of_type[i];
                nb_unk_elem += nb_unknowns_for_type[i];
//             }
        }

        if ( this->want_amd ) {
            assert( (*indice_glob)==0 ); // not managed
            assert( nb_unk_elem==0 ); // not managed
            *indice_noda = symamd( *m ) * nnu;
        }
        if ( this->want_rcm ) {
            assert( (*indice_glob)==0 ); // not managed
            assert( nb_unk_elem==0 ); // not managed
            *indice_noda = symrcm( *m ) * nnu;
        }

        // matrice allocation
        if(allocate_mat)
            matrices.apply( ResizeMat(), size, *this );
    }
    virtual void resize() {
        unsigned size = 0;
        unsigned ng = nb_global_unknowns;
        size += ng;
        unsigned nn = m->node_list.size() * nb_nodal_unknowns;
        size += nn;

        matrices.apply( ResizeMat(), size, *this );
    }
    virtual void shift(int nb=1) {
        while (nb--) {
            time += time_steps[0];
            for(unsigned i=nb_vectors-1;i>0;--i) {
                time_steps[i] = time_steps[i-1];
                vectors[i] = vectors[i-1];
            }
            time_steps[0] = time_steps[1];
        }
    }
    virtual void unshift(int nb=1) {
        while (nb--) {
            time -= time_steps[1];
            for(unsigned i=0;i<nb_vectors-2;++i) {
                time_steps[i] = time_steps[i+1];
                vectors[i] = vectors[i+1];
            }
            time_steps[nb_vectors-1]=time_steps[nb_vectors-2];
            vectors[nb_vectors-1]=vectors[nb_vectors-2];
        }
    }
    virtual Vec<ScalarType> get_nodal_forces() {
        ScalarType old_premul_KUn_in_sollicitation = this->premul_KUn_in_sollicitation;
        this->premul_KUn_in_sollicitation = ScalarType(0);
        Vec<ScalarType> old_sollicitation = sollicitation;
        //
        assemble_clean_mat( false, true );
        Vec<ScalarType> res = sollicitation;
        //
        sollicitation = old_sollicitation;
        this->premul_KUn_in_sollicitation = old_premul_KUn_in_sollicitation;
        return res;
    }
private:
    template<bool assemble_mat,bool assemble_vec>
    struct AssembleNode {
        template<class TN> void operator()(const TN &n,const Vec<unsigned> &indice_noda,Formulation &f) const {
            if ( nb_global_unknowns ) {
                if ( nb_nodal_unknowns ) {
                    unsigned ind[ 2 ] = { indice_noda[f.m->node_list.number(n)], *f.indice_glob };
                    add_nodal_matrix( f, f.matrices(Number<0>()), f.sollicitation, f.vectors, Number<MatCarac<0>::symm>(), Number<assemble_mat>(), Number<assemble_vec>(), n, ind );
                }
                else
                    add_nodal_matrix( f, f.matrices(Number<0>()), f.sollicitation, f.vectors, Number<MatCarac<0>::symm>(), Number<assemble_mat>(), Number<assemble_vec>(), n, f.indice_glob );
            }
            else
                add_nodal_matrix( f, f.matrices(Number<0>()), f.sollicitation, f.vectors, Number<MatCarac<0>::symm>(), Number<assemble_mat>(), Number<assemble_vec>(), n, &indice_noda[f.m->node_list.number(n)]);
        }
        //
        template<class TN,class TMA,class TVE> void operator()(const TN &n, Formulation &f, TMA &K, TVE &F) const {
            if ( nb_global_unknowns ) {
                assert( nb_global_unknowns==0 /*not yet implemented*/ );
                if ( nb_nodal_unknowns ) {
                    unsigned ind[ 2 ] = { ((*f.indice_noda)[f.m->node_list.number(n)]), *f.indice_glob };
                    add_nodal_matrix( f, K, F, *vectors, Number<MatCarac<0>::symm>(), Number<assemble_mat>(), Number<assemble_vec>(), n, ind );
                }
                else
                    add_nodal_matrix( f, K, F, *vectors, Number<MatCarac<0>::symm>(), Number<assemble_mat>(), Number<assemble_vec>(), n, f.indice_glob );
            }
            else {
                unsigned ind[ 2 ] = { ((*f.indice_noda)[f.m->node_list.number(n)]) };
                //std::cerr << "plip plop ind[ 2 ] " << *ind << std::endl;
                add_nodal_matrix( f, K, F, *vectors, Number<MatCarac<0>::symm>(), Number<assemble_mat>(), Number<assemble_vec>(), n, ind );
            }
        }
        Vec<Vec<ScalarType> > *vectors;
    };
    
    template<bool assemble_mat_A,bool assemble_mat_B>
    struct AssembleNodeEig {
        template<class TN> void operator()(const TN &n,const Vec<unsigned> &indice_noda,Formulation &f) const {
            if ( nb_global_unknowns ) {
                if ( nb_nodal_unknowns ) {
                    unsigned ind[ 2 ] = { indice_noda[f.m->node_list.number(n)], *f.indice_glob };
                    add_nodal_matrix( f, f.matrices(Number<1>()), f.matrices(Number<2>()), f.vectors, Number<MatCarac<1>::symm>(), Number<MatCarac<2>::symm>(), Number<assemble_mat_A>(), Number<assemble_mat_B>(), n, ind );
                }
                else
                    add_nodal_matrix( f, f.matrices(Number<1>()), f.matrices(Number<2>()), f.vectors, Number<MatCarac<1>::symm>(), Number<MatCarac<2>::symm>(), Number<assemble_mat_A>(), Number<assemble_mat_B>(), n, f.indice_glob );
            }
            else
                add_nodal_matrix( f, f.matrices(Number<1>()), f.matrices(Number<2>()), f.vectors, Number<MatCarac<1>::symm>(), Number<MatCarac<2>::symm>(), Number<assemble_mat_A>(), Number<assemble_mat_B>(), n, &indice_noda[f.m->node_list.number(n)]);
        }
        //
        template<class TN,class TMA> void operator()(const TN &n, Formulation &f, TMA &A, TMA &B) const {
            if ( nb_global_unknowns ) {
                assert( nb_global_unknowns==0 /*not yet implemented*/ );
                if ( nb_nodal_unknowns ) {
                    unsigned ind[ 2 ] = { ((*f.indice_noda)[f.m->node_list.number(n)]), *f.indice_glob };
                    add_nodal_matrix( f, A, B, vectors, Number<MatCarac<1>::symm>(), Number<MatCarac<2>::symm>(), Number<assemble_mat_A>(), Number<assemble_mat_B>(), n, ind );
                }
                else
                    add_nodal_matrix( f, A, B, vectors, Number<MatCarac<1>::symm>(), Number<MatCarac<2>::symm>(), Number<assemble_mat_A>(), Number<assemble_mat_B>(), n, f.indice_glob );
            }
            else {
                unsigned ind[ 2 ] = { ((*f.indice_noda)[f.m->node_list.number(n)]) };
                //std::cerr << "plip plop ind[ 2 ] " << *ind << std::endl;
                add_nodal_matrix( f, A, B, vectors, Number<MatCarac<1>::symm>(), Number<MatCarac<2>::symm>(), Number<assemble_mat_A>(), Number<assemble_mat_B>(), n, ind );
            }
        }
        Vec<Vec<ScalarType> > *vectors;
    };
    
    template<bool assemble_mat>
    struct AssembleNodeEq {
        template<class TN> void operator()(const TN &n,const Vec<unsigned> &indice_noda,Formulation &f) const {
            if ( nb_global_unknowns ) {
                if ( nb_nodal_unknowns ) {
                    unsigned ind[ 2 ] = { indice_noda[f.m->node_list.number(n)], *f.indice_glob };
                    add_nodal_matrix( f, f.matrices(Number<3>()), f.vectors, Number<MatCarac<3>::symm>(), Number<assemble_mat>(), n, ind );
                }
                else
                    add_nodal_matrix( f, f.matrices(Number<3>()), f.vectors, Number<MatCarac<3>::symm>(), Number<assemble_mat>(), n, f.indice_glob );
            }
            else
                add_nodal_matrix( f, f.matrices(Number<3>()), f.vectors, Number<MatCarac<3>::symm>(), Number<assemble_mat>(), n, &indice_noda[f.m->node_list.number(n)]);
        }
        //
        template<class TN,class TMA> void operator()(const TN &n, Formulation &f, TMA &C) const {
            if ( nb_global_unknowns ) {
                assert( nb_global_unknowns==0 /*not yet implemented*/ );
                if ( nb_nodal_unknowns ) {
                    unsigned ind[ 2 ] = { ((*f.indice_noda)[f.m->node_list.number(n)]), *f.indice_glob };
                    add_nodal_matrix( f, C, vectors, Number<MatCarac<3>::symm>(), Number<assemble_mat>(), n, ind );
                }
                else
                    add_nodal_matrix( f, C, vectors, Number<MatCarac<3>::symm>(), Number<assemble_mat>(), n, f.indice_glob );
            }
            else {
                unsigned ind[ 2 ] = { ((*f.indice_noda)[f.m->node_list.number(n)]) };
                //std::cerr << "plip plop ind[ 2 ] " << *ind << std::endl;
                add_nodal_matrix( f, C, vectors, Number<MatCarac<3>::symm>(), Number<assemble_mat>(), n, ind );
            }
        }
        Vec<Vec<ScalarType> > *vectors;
    };

    template<bool assemble_mat,bool assemble_vec>
    struct AssembleElem {

        template<class TMA,class TVE,class TVEVE,class TE,class TCE,unsigned n> void adsem(
            Formulation &f,TMA &K,TVE &F,TVEVE &vectors,
            const Number<true> &n1,const Number<false> &n2,
            const TE &e,const TCE *ce,const Number<n> &nn,unsigned *in
        ) const {
             add_skin_elem_matrix(
                f,
                K, F, vectors,
                Number<MatCarac<0>::symm>(),
                Number<assemble_mat>(),
                Number<assemble_vec>(),
                e, *ce, nn, in
            );
        }
        
        template<class TE,class TMA,class TVE,class TVEVE,unsigned n>
        void ass_skin_elem(const TE &e,unsigned *in,Formulation &f,TMA &K,TVE &F,TVEVE &vectors,const Number<n> &nn,const Number<n> &nn2) const {}

        template<class TE,class TMA,class TVE,class TVEVE,unsigned n,unsigned n2>
        void ass_skin_elem(const TE &e,unsigned *in,Formulation &f,TMA &K,TVE &F,TVEVE &vectors,const Number<n> &nn,const Number<n2> &nn2) const {
            ass_skin_elem( e, in, f, K, F, vectors, Number<n+1>(), nn2 );
            if ( f.m->sub_mesh(Number<1>()).get_parents_of_EA( f.m->get_children_of(e,Number<1>())[n] ).size()!=1 )
                return;

            typedef typename TM::template SubMesh<1>::T TSubMesh;
            typedef typename TSubMesh::template TElem<typename TypeChildrenElement<typename TE::NE,1,n>::T>::TE TCH;
            TCH *child_elem = static_cast<TCH *>( f.m->get_children_of(e,Number<1>())[n] );
            adsem( f, K, F, vectors, Number<true>(), Number<false>(), e, child_elem, nn, in );
        }
        
        template<class TMA,class TVE,class TVEVE,class TE,class TCE,unsigned n> void adifm(
            Formulation &f,TMA &K,TVE &F,TVEVE &vectors,
            const Number<true> &n1,const Number<false> &n2,
            const TE &e,const TCE *ce,const Number<n> &nn,unsigned *in
        ) const {
             add_internal_face_matrix(
                f,
                K, F, vectors,
                Number<MatCarac<0>::symm>(),
                Number<assemble_mat>(),
                Number<assemble_vec>(),
                e, *ce, nn, in
            );
        }
        
        template<class TE,class TMA,class TVE,class TVEVE,unsigned n>
        void ass_internal_face(const TE &e,unsigned *in,Formulation &f,TMA &K,TVE &F,TVEVE &vectors,const Number<n> &nn,const Number<n> &nn2) const {}

        template<class TE,class TMA,class TVE,class TVEVE,unsigned n,unsigned n2>
        void ass_internal_face(const TE &e,unsigned *in,Formulation &f,TMA &K,TVE &F,TVEVE &vectors,const Number<n> &nn,const Number<n2> &nn2) const {
            ass_internal_face( e, in, f, K, F, vectors, Number<n+1>(), nn2 );
            if ( f.m->sub_mesh(Number<1>()).get_parents_of_EA( f.m->get_children_of(e,Number<1>())[n] ).size()==1 )
                return;

            typedef typename TM::template SubMesh<1>::T TSubMesh;
            typedef typename TSubMesh::template TElem<typename TypeChildrenElement<typename TE::NE,1,n>::T>::TE TCH;
            TCH *child_elem = static_cast<TCH *>( f.m->get_children_of(e,Number<1>())[n] );
            adifm( f, K, F, vectors, Number<true>(), Number<false>(), e, child_elem, nn, in );
        }
        
        template<class TE> void operator()( const TE &e,const Vec<unsigned> &indice_noda,const Vec<unsigned> *indice_elem,Formulation &f ) const {
            unsigned in[ TE::nb_nodes + 1 + nb_global_unknowns ];

            if ( nb_nodal_unknowns )
                for(unsigned i=0;i<TE::nb_nodes;++i)
                    in[i] = indice_noda[ f.m->node_list.number(*e.node(i)) ];

            typedef CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType> CFE;
            if ( CFE::nb_elementary_unknowns )
                in[ TE::nb_nodes * (nb_nodal_unknowns!=0) ] = indice_elem[TE::num_in_elem_list][e.number];

            if ( nb_global_unknowns )
                in[ TE::nb_nodes * (nb_nodal_unknowns!=0) + (CFE::nb_elementary_unknowns!=0) ] = *f.indice_glob;

            add_elem_matrix( f, f.matrices(Number<0>()), f.sollicitation, f.vectors, Number<MatCarac<0>::symm>(), Number<assemble_mat>(), Number<assemble_vec>(), e, in );

            // skin elements
            if ( not f.assume_skin_not_needed ) {
                ass_skin_elem( e, in, f, f.matrices(Number<0>()), f.sollicitation, f.vectors, Number<0>(), Number<NbChildrenElement<typename TE::NE,1>::res>() );
                ass_internal_face( e, in, f, f.matrices(Number<0>()), f.sollicitation, f.vectors, Number<0>(), Number<NbChildrenElement<typename TE::NE,1>::res>() );
            }
        }

        //
        template<class TE,class TMA,class TVE> void operator()( const TE &e, Formulation &f, TMA &K, TVE &F ) const {
            unsigned in[ TE::nb_nodes + 1 + nb_global_unknowns ];
            Formulation::get_ind_for_elem( f, e, in );
            add_elem_matrix( f, K, F, *vectors, Number<MatCarac<0>::symm>(), Number<assemble_mat>(), Number<assemble_vec>(), e, in );

            // skin elements
            if ( not f.assume_skin_not_needed ) {
                ass_skin_elem( e, in, f, K, F, *vectors, Number<0>(), Number<NbChildrenElement<typename TE::NE,1>::res>() );
                ass_internal_face( e, in, f, K, F, *vectors, Number<0>(), Number<NbChildrenElement<typename TE::NE,1>::res>() );
            }
        }
        Vec<Vec<ScalarType> > *vectors;
    };
    
    template<bool assemble_mat_A,bool assemble_mat_B>
    struct AssembleElemEig {

        template<class TMA,class TVEVE,class TE,class TCE,unsigned n> void adsem(
            Formulation &f,TMA &A,TMA &B,TVEVE &vectors,
            const Number<true> &n1,const Number<false> &n2,
            const TE &e,const TCE *ce,const Number<n> &nn,unsigned *in
        ) const {
             add_skin_elem_matrix(
                f,
                A, B, vectors,
                Number<MatCarac<1>::symm>(),
                Number<MatCarac<2>::symm>(),
                Number<assemble_mat_A>(),
                Number<assemble_mat_B>(),
                e, *ce, nn, in
            );
        }

        template<class TE,class TMA,class TVEVE,unsigned n>
        void ass_skin_elem(const TE &e,unsigned *in,Formulation &f,TMA &A,TMA &B,TVEVE &vectors,const Number<n> &nn,const Number<n> &nn2) const {}

        template<class TE,class TMA,class TVEVE,unsigned n,unsigned n2>
        void ass_skin_elem(const TE &e,unsigned *in,Formulation &f,TMA &A,TMA &B,TVEVE &vectors,const Number<n> &nn,const Number<n2> &nn2) const {
            ass_skin_elem( e, in, f, A, B, vectors, Number<n+1>(), nn2 );
            if ( f.m->sub_mesh(Number<1>()).get_parents_of_EA( f.m->get_children_of(e,Number<1>())[n] ).size()!=1 )
                return;

            typedef typename TM::template SubMesh<1>::T TSubMesh;
            typedef typename TSubMesh::template TElem<typename TypeChildrenElement<typename TE::NE,1,n>::T>::TE TCH;
            TCH *child_elem = static_cast<TCH *>( f.m->get_children_of(e,Number<1>())[n] );
            adsem( f, A, B, vectors, Number<true>(), Number<false>(), e, child_elem, nn, in );
        }
        
        template<class TMA,class TVEVE,class TE,class TCE,unsigned n> void adifm(
            Formulation &f,TMA &A,TMA &B,TVEVE &vectors,
            const Number<true> &n1,const Number<false> &n2,
            const TE &e,const TCE *ce,const Number<n> &nn,unsigned *in
        ) const {
             add_internal_face_matrix(
                f,
                A, B, vectors,
                Number<MatCarac<1>::symm>(),
                Number<MatCarac<2>::symm>(),
                Number<assemble_mat_A>(),
                Number<assemble_mat_B>(),
                e, *ce, nn, in
            );
        }
        
        template<class TE,class TMA,class TVEVE, unsigned n>
        void ass_internal_face(const TE &e,unsigned *in,Formulation &f,TMA &A,TMA &B,TVEVE &vectors,const Number<n> &nn,const Number<n> &nn2) const {}

        template<class TE,class TMA,class TVEVE,unsigned n,unsigned n2>
        void ass_internal_face(const TE &e,unsigned *in,Formulation &f,TMA &A,TMA &B,TVEVE &vectors,const Number<n> &nn,const Number<n2> &nn2) const {
            ass_internal_face( e, in, f, A, B, vectors, Number<n+1>(), nn2 );
            if ( f.m->sub_mesh(Number<1>()).get_parents_of_EA( f.m->get_children_of(e,Number<1>())[n] ).size()==1 )
                return;

            typedef typename TM::template SubMesh<1>::T TSubMesh;
            typedef typename TSubMesh::template TElem<typename TypeChildrenElement<typename TE::NE,1,n>::T>::TE TCH;
            TCH *child_elem = static_cast<TCH *>( f.m->get_children_of(e,Number<1>())[n] );
            adifm( f, A, B, vectors, Number<true>(), Number<false>(), e, child_elem, nn, in );
        }

        template<class TE> void operator()( const TE &e,const Vec<unsigned> &indice_noda,const Vec<unsigned> *indice_elem,Formulation &f ) const {
            unsigned in[ TE::nb_nodes + 1 + nb_global_unknowns ];

            if ( nb_nodal_unknowns )
                for(unsigned i=0;i<TE::nb_nodes;++i)
                    in[i] = indice_noda[ f.m->node_list.number(*e.node(i)) ];

            typedef CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType> CFE;
            if ( CFE::nb_elementary_unknowns )
                in[ TE::nb_nodes * (nb_nodal_unknowns!=0) ] = indice_elem[TE::num_in_elem_list][e.number];

            if ( nb_global_unknowns )
                in[ TE::nb_nodes * (nb_nodal_unknowns!=0) + (CFE::nb_elementary_unknowns!=0) ] = *f.indice_glob;

            add_elem_matrix( f, f.matrices(Number<1>()), f.matrices(Number<2>()), f.vectors, Number<MatCarac<1>::symm>(), Number<MatCarac<2>::symm>(), Number<assemble_mat_A>(),  Number<assemble_mat_B>(), e, in );

            // skin elements
            if ( not f.assume_skin_not_needed ) {
                ass_skin_elem( e, in, f, f.matrices(Number<1>()), f.matrices(Number<2>()), f.vectors, Number<0>(), Number<NbChildrenElement<typename TE::NE,1>::res>() );
                ass_internal_face( e, in, f, f.matrices(Number<1>()), f.matrices(Number<2>()), f.vectors, Number<0>(), Number<NbChildrenElement<typename TE::NE,1>::res>() );
            }
        }
        
        //
        template<class TE,class TMA> void operator()( const TE &e, Formulation &f, TMA &A, TMA &B ) const {
            unsigned in[ TE::nb_nodes + 1 + nb_global_unknowns ];
            Formulation::get_ind_for_elem( f, e, in );
            add_elem_matrix( f, A, B, *vectors, Number<MatCarac<1>::symm>(), Number<MatCarac<2>::symm>(), Number<assemble_mat_A>(), Number<assemble_mat_B>(), e, in );

            // skin elements
            if ( not f.assume_skin_not_needed ) {
                ass_skin_elem( e, in, f, A, B, *vectors, Number<0>(), Number<NbChildrenElement<typename TE::NE,1>::res>() );
                ass_internal_face( e, in, f, A, B, *vectors, Number<0>(), Number<NbChildrenElement<typename TE::NE,1>::res>() );
            }
        }
        Vec<Vec<ScalarType> > *vectors;
    };
    
    template<bool assemble_mat>
    struct AssembleElemEq {

        template<class TMA,class TVEVE,class TE,class TCE,unsigned n> void adsem(
            Formulation &f,TMA &C,TVEVE &vectors,
            const Number<true> &n1,const Number<false> &n2,
            const TE &e,const TCE *ce,const Number<n> &nn,unsigned *in
        ) const {
             add_skin_elem_matrix(
                f,
                C, vectors,
                Number<MatCarac<3>::symm>(),
                Number<assemble_mat>(),
                e, *ce, nn, in
            );
        }

        template<class TE,class TMA,class TVEVE,unsigned n>
        void ass_skin_elem(const TE &e,unsigned *in,Formulation &f,TMA &C,TVEVE &vectors,const Number<n> &nn,const Number<n> &nn2) const {}

        template<class TE,class TMA,class TVEVE,unsigned n,unsigned n2>
        void ass_skin_elem(const TE &e,unsigned *in,Formulation &f,TMA &C,TVEVE &vectors,const Number<n> &nn,const Number<n2> &nn2) const {
            ass_skin_elem( e, in, f, C, vectors, Number<n+1>(), nn2 );
            if ( f.m->sub_mesh(Number<1>()).get_parents_of_EA( f.m->get_children_of(e,Number<1>())[n] ).size()!=1 )
                return;

            typedef typename TM::template SubMesh<1>::T TSubMesh;
            typedef typename TSubMesh::template TElem<typename TypeChildrenElement<typename TE::NE,1,n>::T>::TE TCH;
            TCH *child_elem = static_cast<TCH *>( f.m->get_children_of(e,Number<1>())[n] );
            adsem( f, C, vectors, Number<true>(), Number<false>(), e, child_elem, nn, in );
        }
        
        template<class TMA,class TVEVE,class TE,class TCE,unsigned n> void adifm(
            Formulation &f,TMA &C,TVEVE &vectors,
            const Number<true> &n1,const Number<false> &n2,
            const TE &e,const TCE *ce,const Number<n> &nn,unsigned *in
        ) const {
             add_internal_face_matrix(
                f,
                C, vectors,
                Number<MatCarac<3>::symm>(),
                Number<assemble_mat>(),
                e, *ce, nn, in
            );
        }
        
        template<class TE,class TMA,class TVEVE, unsigned n>
        void ass_internal_face(const TE &e,unsigned *in,Formulation &f,TMA &C,TVEVE &vectors,const Number<n> &nn,const Number<n> &nn2) const {}

        template<class TE,class TMA,class TVEVE,unsigned n,unsigned n2>
        void ass_internal_face(const TE &e,unsigned *in,Formulation &f,TMA &C,TVEVE &vectors,const Number<n> &nn,const Number<n2> &nn2) const {
            ass_internal_face( e, in, f, C, vectors, Number<n+1>(), nn2 );
            if ( f.m->sub_mesh(Number<1>()).get_parents_of_EA( f.m->get_children_of(e,Number<1>())[n] ).size()==1 )
                return;

            typedef typename TM::template SubMesh<1>::T TSubMesh;
            typedef typename TSubMesh::template TElem<typename TypeChildrenElement<typename TE::NE,1,n>::T>::TE TCH;
            TCH *child_elem = static_cast<TCH *>( f.m->get_children_of(e,Number<1>())[n] );
            adifm( f, C, vectors, Number<true>(), Number<false>(), e, child_elem, nn, in );
        }

        template<class TE> void operator()( const TE &e,const Vec<unsigned> &indice_noda,const Vec<unsigned> *indice_elem,Formulation &f ) const {
            unsigned in[ TE::nb_nodes + 1 + nb_global_unknowns ];

            if ( nb_nodal_unknowns )
                for(unsigned i=0;i<TE::nb_nodes;++i)
                    in[i] = indice_noda[ f.m->node_list.number(*e.node(i)) ];

            typedef CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType> CFE;
            if ( CFE::nb_elementary_unknowns )
                in[ TE::nb_nodes * (nb_nodal_unknowns!=0) ] = indice_elem[TE::num_in_elem_list][e.number];

            if ( nb_global_unknowns )
                in[ TE::nb_nodes * (nb_nodal_unknowns!=0) + (CFE::nb_elementary_unknowns!=0) ] = *f.indice_glob;

            add_elem_matrix( f, f.matrices(Number<3>()), f.vectors, Number<MatCarac<3>::symm>(), Number<assemble_mat>(),  e, in );

            // skin elements
            if ( not f.assume_skin_not_needed ) {
                ass_skin_elem( e, in, f, f.matrices(Number<3>()), f.vectors, Number<0>(), Number<NbChildrenElement<typename TE::NE,1>::res>() );
                ass_internal_face( e, in, f, f.matrices(Number<3>()), f.vectors, Number<0>(), Number<NbChildrenElement<typename TE::NE,1>::res>() );
            }
        }
        
        //
        template<class TE,class TMA> void operator()( const TE &e, Formulation &f, TMA &C ) const {
            unsigned in[ TE::nb_nodes + 1 + nb_global_unknowns ];
            Formulation::get_ind_for_elem( f, e, in );
            add_elem_matrix( f, C, *vectors, Number<MatCarac<3>::symm>(), Number<assemble_mat>(), e, in );

            // skin elements
            if ( not f.assume_skin_not_needed ) {
                ass_skin_elem( e, in, f, C, *vectors, Number<0>(), Number<NbChildrenElement<typename TE::NE,1>::res>() );
                ass_internal_face( e, in, f, C, *vectors, Number<0>(), Number<NbChildrenElement<typename TE::NE,1>::res>() );
            }
        }
        Vec<Vec<ScalarType> > *vectors;
    };

    struct AssembleResidual {
        template<class TE>
        void operator()( const TE &e, Vec<ScalarType> &res, const Formulation &f ) const {
            unsigned in[ TE::nb_nodes + 1 + nb_global_unknowns ];
            Formulation::get_ind_for_elem( f, e, in );
            add_elem_residual( f, res, f.vectors, e, in );
        }
    };

    template<class TE>
    static void get_ind_for_elem( const Formulation &f, const TE &e, unsigned *in ) {
        if ( nb_nodal_unknowns )
            for(unsigned i=0;i<TE::nb_nodes;++i)
                in[i] = (*f.indice_noda)[ f.m->node_list.number(*e.node(i)) ];

        typedef CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType> CFE;
        if ( CFE::nb_elementary_unknowns )
            assert( 0 );

        if ( nb_global_unknowns )
            assert( 0 );
    };

public:
    template<class TE>
    Vec<unsigned,TE::nb_nodes+1+nb_global_unknowns> indices_for_element( const TE &e ) const {
        Vec<unsigned,TE::nb_nodes+1+nb_global_unknowns> in;

        if ( nb_nodal_unknowns )
            for(unsigned i=0;i<TE::nb_nodes;++i)
                in[i] = (*indice_noda)[ m->node_list.number(*e.node(i)) ];

        typedef CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType> CFE;
        if ( CFE::nb_elementary_unknowns )
            in[ TE::nb_nodes * (nb_nodal_unknowns!=0) ] = indice_elem[TE::num_in_elem_list][e.number];

        if ( nb_global_unknowns )
            in[ TE::nb_nodes * (nb_nodal_unknowns!=0) + (CFE::nb_elementary_unknowns!=0) ] = *indice_glob;

        return in;
    }

    ///
    Codegen::Ex::MapExNum val_sub_sym( const Constraint<Formulation> &sc, const Codegen::Ex::SetEx &sub_symbols ) const {
        using namespace Codegen;
        Ex::MapExNum res = val_sub_sym( sub_symbols );
        for( int i = 0; i < sc.coeffs.size(); ++i ) {
            int p = sc.coeffs[ i ].num_in_fmat( *this );
            if ( p < vectors[ 0 ].size() )
                res[ sc.coeffs[ i ].sym ] = abs( real( vectors[ 0 ][ p ] ) );
        }
        return res;
    }
    Codegen::Ex::MapExNum val_sub_sym( const Codegen::Ex::SetEx &sub_symbols ) const {
        using namespace Codegen;
        Ex::MapExNum res;
        for(Ex::SetEx::const_iterator iter=sub_symbols.begin();iter!=sub_symbols.end();++iter) {
            std::string str = iter->to_string();
            if ( str=="time" )
                res[ *iter ] = abs( real( time ) );
            else
                std::cout << "Unknown symbol : " << str << std::endl;
        }
        return res;
    }
    ///
    virtual void clean_mats() {
        matrices.apply( ClearMat() );
    }
    ///
    virtual void assemble_clean_mat(bool assemble_mat=true,bool assemble_vec=true) {
        if ( not initial_condition_initialized ) { // old_vectors
            get_initial_conditions();
            shift();
        }
        
        pthread_mutex_init(&mutex, NULL);

        if ( assemble_vec ) // preinitialisation
            sollicitation.set(ScalarType(0));

        if ( not this->assume_skin_not_needed )
            m->update_skin();

        if ( assemble_mat ) {
            clean_mats();
            if ( assemble_vec ) {
                add_global_matrix( *this, matrices(Number<0>()), sollicitation, vectors, Number<MatCarac<0>::symm>(), Number<true>(), Number<true>(), *indice_glob ); // global
//                 apply( m->node_list, AssembleNode<true,true >(), *indice_noda, *this ); // nodal
//                 apply( m->elem_list, AssembleElem<true,true >(), *indice_noda, indice_elem, *this ); // element (and skin elements)
                apply_mt( m->node_list, this->nb_threads_assemble_matrix, AssembleNode<true,true >(), *indice_noda, *this ); // nodal
                apply_mt( m->elem_list, this->nb_threads_assemble_matrix, AssembleElem<true,true >(), *indice_noda, indice_elem, *this ); // element (and skin elements)
            }
            else {
                add_global_matrix( *this, matrices(Number<0>()), sollicitation, vectors, Number<MatCarac<0>::symm>(), Number<true>(), Number<false>(), *indice_glob ); // global
                //apply( m->node_list, AssembleNode<true,false >(), *indice_noda, *this ); // nodal
                //apply( m->elem_list, AssembleElem<true,false >(), *indice_noda, indice_elem, *this); // element (and skin elements)
                apply_mt( m->node_list, this->nb_threads_assemble_matrix, AssembleNode<true,false >(), *indice_noda, *this ); // nodal
                apply_mt( m->elem_list, this->nb_threads_assemble_matrix, AssembleElem<true,false >(), *indice_noda, indice_elem, *this); // element (and skin elements)
            }
            add_global_matrix( *this, matrices(Number<1>()), matrices(Number<2>()), vectors, Number<MatCarac<1>::symm>(), Number<MatCarac<2>::symm>(), Number<true>(), Number<true>(), *indice_glob ); // global
            add_global_matrix( *this, matrices(Number<3>()), vectors, Number<MatCarac<3>::symm>(), Number<true>(), *indice_glob ); // global
//             apply( m->node_list, AssembleNodeEig<true,true >(), *indice_noda, *this ); // nodal
//             apply( m->elem_list, AssembleElemEig<true,true >(), *indice_noda, indice_elem, *this ); // element (and skin elements)
//             apply( m->node_list, AssembleNodeEq<true >(), *indice_noda, *this ); // nodal
//             apply( m->elem_list, AssembleElemEq<true >(), *indice_noda, indice_elem, *this ); // element (and skin elements)
            apply_mt( m->node_list, this->nb_threads_assemble_matrix, AssembleNodeEig<true,true >(), *indice_noda, *this ); // nodal
            apply_mt( m->elem_list, this->nb_threads_assemble_matrix, AssembleElemEig<true,true >(), *indice_noda, indice_elem, *this ); // element (and skin elements)
            apply_mt( m->node_list, this->nb_threads_assemble_matrix, AssembleNodeEq<true >(), *indice_noda, *this ); // nodal
            apply_mt( m->elem_list, this->nb_threads_assemble_matrix, AssembleElemEq<true >(), *indice_noda, indice_elem, *this ); // element (and skin elements)
        }
        else {
            if ( assemble_vec ) {
                add_global_matrix( *this, matrices(Number<0>()), sollicitation, vectors, Number<MatCarac<0>::symm>(), Number<false>(), Number<true>(), *indice_glob ); // global
                apply( m->node_list, AssembleNode<false,true >(), *indice_noda, *this ); // nodal
                apply( m->elem_list, AssembleElem<false,true >(), *indice_noda, indice_elem, *this ); // element (and skin elements)
//                 apply_mt( m->node_list, this->nb_threads_assemble_matrix, AssembleNode<false,true >(), *indice_noda, indice_elem, *this ); // nodal
//                 apply_mt( m->elem_list, this->nb_threads_assemble_matrix, AssembleElem<false,true >(), *indice_noda, indice_elem, *this ); // element (and skin elements)
            }
            else {
                add_global_matrix( *this, matrices(Number<0>()), sollicitation, vectors, Number<MatCarac<0>::symm>(), Number<false>(), Number<false>(), *indice_glob ); // global
                apply( m->node_list, AssembleNode<false,false>(), *indice_noda, *this ); // nodal
                apply( m->elem_list, AssembleElem<false,false>(), *indice_noda, indice_elem, *this ); // element (and skin elements)
//                 apply_mt( m->node_list, this->nb_threads_assemble_matrix, AssembleNode<false,false >(), *indice_noda, indice_elem, *this ); // nodal
//                 apply_mt( m->elem_list, this->nb_threads_assemble_matrix, AssembleElem<false,false >(), *indice_noda, indice_elem, *this ); // element (and skin elements)
            }
            add_global_matrix( *this, matrices(Number<1>()), matrices(Number<2>()), vectors, Number<MatCarac<1>::symm>(), Number<MatCarac<2>::symm>(), Number<false>(), Number<false>(), *indice_glob ); // global
            add_global_matrix( *this, matrices(Number<3>()), vectors, Number<MatCarac<3>::symm>(), Number<false>(), *indice_glob ); // global
            apply( m->node_list, AssembleNodeEig<false,false >(), *indice_noda, *this ); // nodal
            apply( m->elem_list, AssembleElemEig<false,false >(), *indice_noda, indice_elem, *this ); // element (and skin elements)
            apply( m->node_list, AssembleNodeEq<false >(), *indice_noda, *this ); // nodal
            apply( m->elem_list, AssembleElemEq<false >(), *indice_noda, indice_elem, *this ); // element (and skin elements)
//             apply_mt( m->node_list, this->nb_threads_assemble_matrix, AssembleNodeEig<false,false >(), *indice_noda, *this ); // nodal
//             apply_mt( m->elem_list, this->nb_threads_assemble_matrix, AssembleElemEig<false,false >(), *indice_noda, indice_elem, *this ); // element (and skin elements)
//             apply_mt( m->node_list, this->nb_threads_assemble_matrix, AssembleNodeEq<false >(), *indice_noda, *this ); // nodal
//             apply_mt( m->elem_list, this->nb_threads_assemble_matrix, AssembleElemEq<false >(), *indice_noda, indice_elem, *this ); // element (and skin elements)
        }
    }
    ///
    virtual Vec<LinearizedConstraint> get_linearized_constraints() {
        Vec<LinearizedConstraint> linearized_constraints; linearized_constraints.resize( constraints.size() );
        for(unsigned nc=0;nc<constraints.size();++nc) {
            LinearizedConstraint &lc = linearized_constraints[nc];
            // calculation of res
            using namespace Codegen;
            Ex res = constraints[ nc ].res;
            Ex::SetEx sub_symbols; res.get_sub_symbols(sub_symbols);
            Codegen::Ex::MapExNum vss = val_sub_sym( constraints[ nc ], sub_symbols );
            lc.val = (ScalarType)res.subs_numerical( vss );
            // calculation of coeffs
            lc.coeffs.resize( constraints[nc].coeffs.size() );
            // Vec<unsigned> num_in_fmat; num_in_fmat.resize( constraints[nc].coeffs.size() );
            for(unsigned j=0;j<constraints[nc].coeffs.size();++j) {
                lc.coeffs[j].num_in_vec = constraints[nc].coeffs[j].num_in_fmat(*this);
                Ex coeff = constraints[nc].coeffs[j].val;
                Ex::SetEx sub_symbols; res.get_sub_symbols(sub_symbols);
                Codegen::Ex::MapExNum vss = val_sub_sym( constraints[ nc ], sub_symbols );
                lc.coeffs[j].val = (ScalarType)coeff.subs_numerical( vss );
            }
        }
        return linearized_constraints;
    }
    ///
    virtual void assemble_constraints(bool assemble_mat=true,bool assemble_vec=true) {
        if ( boolean_(assemble_mat) and ( constraints.size() or boolean_(this->levenberg_marquadt) ) )
            this->max_diag = max(abs(matrices(Number<0>()).diag()));

        //
        if ( boolean_(assemble_mat) and boolean_(this->levenberg_marquadt) )
            matrices( Number<0>() ).diag() += this->max_diag * this->levenberg_marquadt;

        // constraints
        if ( constraints.size() ) {
            for(unsigned i=0;i<constraints.size();++i) {
                // calculation of res
                using namespace Codegen;
                Ex res = constraints[ i ].res;
                Ex::SetEx sub_symbols;
                res.get_sub_symbols(sub_symbols);
                Codegen::Ex::MapExNum vss = val_sub_sym( constraints[ i ], sub_symbols );
                ScalarType ress = (ScalarType)res.subs_numerical( vss );
                // calculation of coeffs
                Vec<ScalarType> coeffs; coeffs.resize( constraints[i].coeffs.size() );
                Vec<unsigned> num_in_fmat; num_in_fmat.resize( constraints[i].coeffs.size() );
                for(unsigned j=0;j<constraints[i].coeffs.size();++j) {
                    num_in_fmat[ j ] = constraints[ i ].coeffs[ j ].num_in_fmat( *this );
                    Ex coeff = constraints[i].coeffs[j].val;
                    Ex::SetEx sub_symbols;
                    res.get_sub_symbols(sub_symbols);
                    Codegen::Ex::MapExNum vss = val_sub_sym( constraints[ i ], sub_symbols );
                    coeffs[j] = (ScalarType)coeff.subs_numerical( vss );
                }
                // add to vec and mat
                if ( constraints[i].penalty_value != ScalarType( 0 ) ) { // -> penalty
                    for(unsigned j=0;j<coeffs.size();++j) {
                        ScalarType C = coeffs[j] * ScalarType( this->max_diag ) * constraints[i].penalty_value;
                        if ( assemble_vec )
                            sollicitation[ num_in_fmat[ j ] ] += C * ress;
                        if ( assemble_mat ) {
                            if ( MatCarac<0>::symm or MatCarac<0>::herm )
                                for(unsigned k=0;k<=j;++k)
                                    matrices(Number<0>())( num_in_fmat[ j ], num_in_fmat[ k ] ) += C * coeffs[ k ];
                            else
                                for(unsigned k=0;k<coeffs.size();++k)
                                    matrices(Number<0>())( num_in_fmat[ j ], num_in_fmat[ k ] ) += C * coeffs[ k ];
                        }
                    }
                } else { // -> lagrange
                    // assert( coeffs.size() == 1 );
                    // vec
                    if ( assemble_vec )
                        sollicitation[ offset_lagrange_multipliers + i ] += ress;
                    // mat
                    if ( assemble_mat ) {
                        for(unsigned j=0;j<coeffs.size();++j) {
                            if ( MatCarac<0>::symm or MatCarac<0>::herm ) {
                                matrices(Number<0>())( offset_lagrange_multipliers + i, num_in_fmat[j] ) += coeffs[j];
                            } else {
                                matrices(Number<0>())( num_in_fmat[j], offset_lagrange_multipliers + i ) += coeffs[j];
                                matrices(Number<0>())( offset_lagrange_multipliers + i, num_in_fmat[j] ) += coeffs[j];
                            }
                        }
                    }
                }
            }
        }
    }
    ///
    virtual void assemble_sollicitations(bool assemble_mat=true,bool assemble_vec=true) {
        // sollicitations
        if ( sollicitations.size() ) {
            for(unsigned i=0;i<sollicitations.size();++i) {
                // calculation of res
                using namespace Codegen;
                Ex res = sollicitations[i].val;
                Ex::SetEx sub_symbols;
                res.get_sub_symbols(sub_symbols);
                Codegen::Ex::MapExNum vss = val_sub_sym( sub_symbols );
                ScalarType ress = (ScalarType)res.subs_numerical( vss );
                // add to vec and mat
                unsigned num_in_fmat = sollicitations[i].num_in_fmat(*this);
                if ( assemble_vec )
                    sollicitation[num_in_fmat] += ress;
            }
        }
    }
    /// ...
    virtual void assemble(bool assemble_mat=true,bool assemble_vec=true) {
        // assemble_mat
        // std::cout << "assemble_mat -> " << time_of_day_in_sec() << std::endl;
        assemble_clean_mat(assemble_mat,assemble_vec);
        // constraints
        // std::cout << "assemble_con -> " << time_of_day_in_sec() << std::endl;
        assemble_constraints(assemble_mat,assemble_vec);
        // sollicitations
        // std::cout << "assemble_sol -> " << time_of_day_in_sec() << std::endl;
        assemble_sollicitations(assemble_mat,assemble_vec);

        if ( user_want_pierre_precond )
            precond();
    }

    ///
    virtual void assemble_clean_mat( Mat<ScalarType,Sym<>,SparseLine<> > &K, Vec<ScalarType> &F, Vec<Vec<ScalarType> > &vectors_, bool assemble_mat = true, bool assemble_vec = true ) {
        //
        // PRINT( F.size() );
        vectors_.resize( nb_vectors );
        for( int i = 0; i < nb_vectors; ++i )
            vectors_[ i ].resize( F.size(), ScalarType(0) );

        if ( not initial_condition_initialized ) { // old_vectors
            get_initial_conditions( vectors_ );
        }

        if ( not this->assume_skin_not_needed )
            m->update_skin();

        // std::cerr << "nb_processors : " << nb_processors << std::endl;
        // m->update_skin();
        assert( nb_global_unknowns == 0 /* add_global_matrix not yet implemeted */ );
        if ( assemble_mat ) {
            if ( assemble_vec ) {
                AssembleNode<true,true > assnode;
                assnode.vectors = &vectors_;
                // apply( m->node_list, assnode, *this, K, F ); // nodal
                apply_mt( m->node_list, this->nb_threads_assemble_matrix, assnode, *this, K, F ); // nodal
                AssembleElem<true,true > asselem;
                asselem.vectors = &vectors_;
                // apply( m->elem_list, asselem, *this, K, F ); // element (and skin elements)
                apply_mt( m->elem_list, this->nb_threads_assemble_matrix, asselem, *this, K, F ); // element (and skin elements)
            }
            else{
                AssembleNode<true,false > assnode;
                assnode.vectors = &vectors_;
                // apply( m->node_list, assnode, *this, K, F ); // nodal
                apply_mt( m->node_list, this->nb_threads_assemble_matrix, assnode, *this, K, F ); // nodal
                AssembleElem<true,false > asselem;
                asselem.vectors = &vectors_;
                // apply( m->elem_list, asselem, *this, K, F ); // element (and skin elements)
                apply_mt( m->elem_list, this->nb_threads_assemble_matrix, asselem, *this, K, F ); // element (and skin elements)
            }
        }
        else {
            AssembleNode<false,true > assnode;
            assnode.vectors = &vectors_;
            apply( m->node_list, assnode, *this, K, F ); // nodal
            // apply_mt( m->node_list, this->nb_threads_assemble_matrix, assnode, *this, K, F );
            AssembleElem<false,true > asselem;
            asselem.vectors = &vectors_;
            apply( m->elem_list, asselem, *this , K, F ); // element (and skin elements)
            // apply_mt( m->elem_list, this->nb_threads_assemble_matrix, asselem, *this, K, F );
        }
    }
    
    ///
    virtual void assemble_clean_mat( Mat<ScalarType,Sym<>,SparseLine<> > &A, Mat<ScalarType,Sym<>,SparseLine<> > &B, Vec<Vec<ScalarType> > &vectors_, bool assemble_mat = true ) {
        //
        // PRINT( F.size() );
        vectors_.resize( nb_vectors );
        for( int i = 0; i < nb_vectors; ++i )
            vectors_[ i ].resize( A.nb_cols(), ScalarType(0) );

        if ( not initial_condition_initialized ) { // old_vectors
            get_initial_conditions( vectors_ );
        }

        if ( not this->assume_skin_not_needed )
            m->update_skin();

        // std::cerr << "nb_processors : " << nb_processors << std::endl;
        // m->update_skin();
        assert( nb_global_unknowns == 0 /* add_global_matrix not yet implemeted */ );
        if ( assemble_mat ) {
            AssembleNodeEig<true,true > assnodeeig;
            assnodeeig.vectors = &vectors_;
            // apply( m->node_list, assnodeeig, *this, A, B ); // nodal
            apply_mt( m->node_list, this->nb_threads_assemble_matrix, assnodeeig, *this, A, B ); // nodal
            AssembleElemEig<true,true > asselemeig;
            asselemeig.vectors = &vectors_;
            // apply( m->elem_list, asselemeig, *this, A, B ); // element (and skin elements)
            apply_mt( m->elem_list, this->nb_threads_assemble_matrix, asselemeig, *this, A, B ); // element (and skin elements)
        }
        else {
            AssembleNodeEig<false,false > assnodeeig;
            assnodeeig.vectors = &vectors_;
            apply( m->node_list, assnodeeig, *this, A, B ); // nodal
            // apply_mt( m->node_list, this->nb_threads_assemble_matrix, assnodeeig, *this, A, B );
            AssembleElemEig<false,false > asselemeig;
            asselemeig.vectors = &vectors_;
            apply( m->elem_list, asselemeig, *this , A, B ); // element (and skin elements)
            // apply_mt( m->elem_list, this->nb_threads_assemble_matrix, asselemeig, *this, A, B );
        }
    }
    
    ///
    virtual void assemble_clean_mat( Mat<ScalarType,Gen<>,SparseLine<> > &C, Vec<Vec<ScalarType> > &vectors_, bool assemble_mat = true ) {
        //
        // PRINT( F.size() );
        vectors_.resize( nb_vectors );
        for( int i = 0; i < nb_vectors; ++i )
            vectors_[ i ].resize( C.nb_cols(), ScalarType(0) );

        if ( not initial_condition_initialized ) { // old_vectors
            get_initial_conditions( vectors_ );
        }

        if ( not this->assume_skin_not_needed )
            m->update_skin();

        // std::cerr << "nb_processors : " << nb_processors << std::endl;
        // m->update_skin();
        assert( nb_global_unknowns == 0 /* add_global_matrix not yet implemeted */ );
        if ( assemble_mat ) {
            AssembleNodeEq<true > assnodeeq;
            assnodeeq.vectors = &vectors_;
            // apply( m->node_list, assnodeeq, *this, C ); // nodal
            apply_mt( m->node_list, this->nb_threads_assemble_matrix, assnodeeq, *this, C ); // nodal
            AssembleElemEq<true > asselemeq;
            asselemeq.vectors = &vectors_;
            // apply( m->elem_list, asselemeq, *this, C ); // element (and skin elements)
            apply_mt( m->elem_list, this->nb_threads_assemble_matrix, asselemeq, *this, C ); // element (and skin elements)
        }
        else {
            AssembleNodeEq<false > assnodeeq;
            assnodeeq.vectors = &vectors_;
            apply( m->node_list, assnodeeq, *this, C ); // nodal
            // apply_mt( m->node_list, this->nb_threads_assemble_matrix, assnodeeq, *this, C );
            AssembleElemEq<false > asselemeq;
            asselemeq.vectors = &vectors_;
            apply( m->elem_list, asselemeq, *this , C ); // element (and skin elements)
            // apply_mt( m->elem_list, this->nb_threads_assemble_matrix, asselemeq, *this, C );
        }
    }
    
    virtual void read_material_to_mesh( const XmlNode &n ){
        read_material_to_mesh_( n, *this );
    }
    //
    virtual unsigned localOP_local_update(){
        unsigned olda = 0;
        localOP->local_update( m, this );                         ///< Local loop
        return olda;
    }
    //
    virtual void localOP_update_variables(){
       localOP->update_variables(m);
    }
    ///
    virtual void assemble_constraints(Mat<ScalarType,Sym<>,SparseLine<> > &K, Vec<ScalarType> &F, Vec<Vec<ScalarType> > &vectors_, const ScalarType &M, bool assemble_mat=true,bool assemble_vec=true) {
        // constraints
        if ( constraints.size() ) {
            for(unsigned i=0;i<constraints.size();++i) {
                // calculation of res
                using namespace Codegen;
                Ex res = constraints[ i ].res;
                Ex::SetEx sub_symbols;
                res.get_sub_symbols(sub_symbols);
                Codegen::Ex::MapExNum vss = val_sub_sym( constraints[ i ], sub_symbols );
                ScalarType ress = (ScalarType)res.subs_numerical( vss );
                // calculation of coeffs
                Vec<ScalarType> coeffs; coeffs.resize( constraints[i].coeffs.size() );
                Vec<unsigned> num_in_fmat; num_in_fmat.resize( constraints[i].coeffs.size() );
                for(unsigned j=0;j<constraints[i].coeffs.size();++j) {
                    num_in_fmat[j] = constraints[i].coeffs[j].num_in_fmat(*this);
                    Ex coeff = constraints[i].coeffs[j].val;
                    Ex::SetEx sub_symbols;
                    res.get_sub_symbols(sub_symbols);
                    Codegen::Ex::MapExNum vss = val_sub_sym( constraints[ i ], sub_symbols );
                    coeffs[j] = (ScalarType)coeff.subs_numerical( vss );
                }
                // add to vec and mat
                for(unsigned j=0;j<coeffs.size();++j) {
                    ScalarType C = coeffs[j] * ScalarType(M) * constraints[i].penalty_value;
                    if ( assemble_vec )
                        F[num_in_fmat[j]] += C * ress;
                    if ( assemble_mat ) {
                        if ( MatCarac<0>::symm or MatCarac<0>::herm )
                            for(unsigned k=0;k<=j;++k)
                                K(num_in_fmat[j],num_in_fmat[k]) += C * coeffs[k];
                        else
                            for(unsigned k=0;k<coeffs.size();++k)
                                K(num_in_fmat[j],num_in_fmat[k]) += C * coeffs[k];
                    }
                }
            }
        }
    }
    ///
    virtual void assemble_sollicitations(Mat<ScalarType,Sym<>,SparseLine<> > &K, Vec<ScalarType> &F,Vec<Vec<ScalarType> > &vectors_, bool assemble_mat=true,bool assemble_vec=true) {
        // sollicitations
        if ( sollicitations.size() ) {
            for(unsigned i=0;i<sollicitations.size();++i) {
                // calculation of res
                using namespace Codegen;
                Ex res = sollicitations[i].val;
                Ex::SetEx sub_symbols;
                res.get_sub_symbols(sub_symbols);
                Codegen::Ex::MapExNum vss = val_sub_sym( sub_symbols );
                ScalarType ress = (ScalarType)res.subs_numerical( vss );
                // add to vec and mat
                unsigned num_in_fmat = sollicitations[i].num_in_fmat(*this);
                std::cerr << "num_in_fmat " << num_in_fmat << std::endl;
                if ( assemble_vec )
                    sollicitation[num_in_fmat] += ress;
            }
        }
    }
    ///
    virtual void assemble( Mat<ScalarType,Sym<>,SparseLine<> > &K, Vec<ScalarType> &F, Vec<Vec<ScalarType> > &vectors_, bool assemble_mat = true, bool assemble_vec = true ) {
        // assemble_mat
        assemble_clean_mat( K, F, vectors_, assemble_mat, assemble_vec );
        // constraints
        ScalarType M = max( abs( K.diag() ) );
        assemble_constraints( K, F, vectors_,  M, assemble_mat, assemble_vec );
        // sollicitations
        assemble_sollicitations( K, F, vectors_,  assemble_mat, assemble_vec );
    }
    ///
    virtual void assemble( Mat<ScalarType,Sym<>,SparseLine<> > &A, Mat<ScalarType,Sym<>,SparseLine<> > &B, Vec<Vec<ScalarType> > &vectors_, bool assemble_mat = true ) {
        // assemble_mat
        assemble_clean_mat( A, B, vectors_, assemble_mat );
    }
    ///
    virtual void assemble(  Mat<ScalarType,Gen<>,SparseLine<> > &C, Vec<Vec<ScalarType> > &vectors_, bool assemble_mat = true ) {
        // assemble_mat
        assemble_clean_mat( C, vectors_, assemble_mat );
    }
    ///
    bool solve_system_(ScalarType iterative_criterium, const Number<1> &n_wont_add_nz, const Number<0> &sym) {
        try {
            if ( boolean_(iterative_criterium) ) {
//                std::cout << "Solveur iteratif type factorisation LU incomplete" << std::endl << std::endl;
                Mat<ScalarType, Gen<>, SparseLU > mm = matrices(Number<0>());
                lu_factorize( mm );
                solve_using_incomplete_lu_factorize( mm, Mat<ScalarType, Gen<>, SparseLU >(matrices(Number<0>())), sollicitation, vectors[0], iterative_criterium );
            }
            else {
                #if WITH_UMFPACK
//                std::cout << "Solveur UMFPACK" << std::endl << std::endl;
                if ( not matrices(Number<0>()).get_factorization() ) {
                    std::cout << "Bing. Inversion error" << std::endl << std::endl;
                    return false;
                }
                vectors[0] = matrices(Number<0>()).solve( sollicitation );
                #elif WITH_MUMPS
//                std::cout << "Solveur MUMPS" << std::endl << std::endl;
                solver.get_factorization( matrices(Number<0>()), false, true );
                vectors[0] = solver.solve( sollicitation );
				#else
//                std::cout << "Solveur Inv" << std::endl << std::endl;
                vectors[0] = inv(matrices(Number<0>())) * sollicitation;
				#endif
            }

        } catch(const SolveException &e) { std::cerr << "system not inversible" << std::endl; return false; }
        return true;
    }
    ///
    bool solve_system_(ScalarType iterative_criterium, const Number<1> &n_wont_add_nz, const Number<1> &sym) {
        try {
            if ( boolean_(iterative_criterium) ) {
//                std::cout << "Solveur iteratif type factorisation de Cholesky incomplete" << std::endl << std::endl;
                Mat<ScalarType,Sym<>,SparseLine<> > mm = matrices(Number<0>());
                incomplete_chol_factorize( mm );
                solve_using_incomplete_chol_factorize( mm, Mat<ScalarType,Sym<>,SparseLine<> >( matrices(Number<0>()) ), sollicitation, vectors[0], iterative_criterium );
            }
            else {
                #if WITH_CHOLMOD
//                std::cout << "Solveur CholMod" << std::endl << std::endl;
                if ( not matrices(Number<0>()).get_factorization() ) {
                    std::cout << "Bing. Inversion error" << std::endl << std::endl;
                    return false;
                }
                vectors[0] = matrices(Number<0>()).solve( sollicitation );
                #elif WITH_MUMPS
//                std::cout << "Solveur MUMPS" << std::endl << std:: endl;
                solver.get_factorization( matrices( Number<0>() ), false, true );
                vectors[0] = solver.solve( sollicitation );
                #elif WITH_LDL
//                std::cout << "Solveur LDL" << std::endl << std:: endl;
                solver.get_factorization( matrices(Number<0>()), false );
                vectors[0] = sollicitation;
                solver.solve( vectors[0] );
                #else
//                std::cout << "Solveur Inv" << std::endl << std::endl << std:: endl;
                vectors[0] = inv(matrices(Number<0>())) * sollicitation;
                #endif
            }
        } catch(const SolveException &e) { std::cerr << "system not inversible" << std::endl; return false; }
        return true;
    }

    ///
    bool solve_system_(ScalarType iterative_criterium, const Number<0> &n_wont_add_nz, const Number<0> &sym) {
        try {
            if ( boolean_(iterative_criterium) ) {
//                std::cout << "Solveur iteratif type factorisation LU incomplete" << std::endl << std::endl;
                Mat<ScalarType, Gen<>, SparseLU > mm = matrices(Number<0>());
                lu_factorize( mm );
                solve_using_incomplete_lu_factorize( mm, Mat<ScalarType, Gen<>, SparseLU >( matrices(Number<0>()) ), sollicitation, vectors[0], iterative_criterium );
            }
            else {
//                std::cout << "Solveur Inv" << std::endl << std:: endl;
                vectors[0] = inv(matrices(Number<0>())) * sollicitation;
            }
        } catch(const SolveException &e) { std::cerr << "system not inversible" << std::endl; return false; }
        return true;
    }
    bool solve_system_(ScalarType iterative_criterium, const Number<0> &n_wont_add_nz, const Number<1> &sym) {
        try {
            if ( boolean_(iterative_criterium) ) {
//                 std::cout << "Solveur iteratif type factorisation de Cholesky incomplete" << std::endl << std::endl;
                Mat<ScalarType,Sym<>,SparseLine<> > mm = matrices(Number<0>());
                incomplete_chol_factorize( mm );
                solve_using_incomplete_chol_factorize( mm, Mat<ScalarType,Sym<>,SparseLine<> >( matrices(Number<0>()) ), sollicitation, vectors[0], iterative_criterium );
            }
            else {
                #if WITH_MUMPS
//                std::cout << "Solveur MUMPS" << std::endl << std:: endl;
                solver.get_factorization( matrices( Number<0>() ), false, true );
                vectors[0] = solver.solve( sollicitation );
                #elif WITH_LDL
//                std::cout << "Solveur LDL" << std::endl << std:: endl;
                solver.get_factorization( matrices(Number<0>()), false );
                vectors[0] = sollicitation;
                solver.solve( vectors[0] );
                #else
//                std::cout << "Solveur Inv" << std::endl << std:: endl;
                vectors[0] = inv(matrices(Number<0>())) * sollicitation;
                #endif
            }
            //PRINT( vectors[0] );
        } catch(const SolveException &e) { std::cerr << "system not inversible" << std::endl; return false; }
        return true;
    }
    ///
    template<unsigned nddl,unsigned sym,class TTT,unsigned wna> bool solve_system_iterative_block(ScalarType iterative_criterium, Number<nddl>/*nb_nodal_unknowns*/, Number<sym>/*sym*/,StructForType<TTT>, Number<wna>) {
        assert( 0 );
        return false;
    }
    ///
    template<int nb_n>
    bool solve_system_iterative_block(ScalarType iterative_criterium, Number<nb_n>, Number<MatCarac<0>::symm>, StructForType<ScalarType>, Number<wont_add_nz>) {
        return false;
    }
    ///
    bool solve_system_iterative_block(ScalarType iterative_criterium, Number<3>/*nb_nodal_unknowns*/, Number<true>/*sym*/,StructForType<double>, Number<false>/*wont_add_nz*/ ) {
        MatWithTinyBlocks<ScalarType,Sym<3> > M( matrices(Number<0>()) );
        MatWithTinyBlocks<ScalarType,Sym<3> > F = M; F.chol_incomp();
        solve_using_incomplete_chol_factorize( F, M, sollicitation, vectors[0], iterative_criterium );
        return true;
    }
    ///
    bool solve_system(ScalarType iterative_criterium=ScalarType(0),bool disp_timing=false) {
        bool res;
        double t0 = time_of_day_in_sec();
        if ( boolean_(iterative_criterium) and nb_nodal_unknowns==3 and sollicitation.size() % 3 == 0 and TypeInformation<ScalarType>::type()=="double" and wont_add_nz == false )
            res = solve_system_iterative_block( iterative_criterium, Number<nb_nodal_unknowns>(), Number<MatCarac<0>::symm>(), StructForType<ScalarType>(), Number<wont_add_nz>() );
        else
            res = solve_system_( iterative_criterium, Number<wont_add_nz>(), Number<MatCarac<0>::symm>() );
        if ( disp_timing )
            std::cout << time_of_day_in_sec() - t0 << std::endl;
        return res;
    }

    struct ChecKNLConv {
        template<class TE> bool operator()( const TE &e, const Formulation *f ) const {
            typedef CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType> CFE;
            assert( CFE::nb_elementary_unknowns == 0 /*TODO*/ );
            return false;
        }
    };
    /**
     * call all functions to get solution...
     * @return
     */
    bool solve( ScalarType iterative_criterium=ScalarType(0), bool disp_timing=false ) {
        allocate_matrices();
        shift();
        //
        Vec<ScalarType> old_vec;
        //if ( boolean_(this->non_linear_iterative_criterium) or this->non_linear_iterative_criterium_vec.size() )
        //    old_vec.resize( vectors[0].size(), ScalarType(0) );
        //
        unsigned nb_iterations = 0;
        while ( true ) {
            //PRINT( __LINE__ );
            assemble();
            //PRINT( __LINE__ );
            if ( solve_system(iterative_criterium,disp_timing) == false )
                return false;

            if ( this->non_linear_iterative_criterium == ScalarType(0.) and this->non_linear_iterative_criterium_vec.size() == 0 ) // assuming linear system
                break;

            if ( not old_vec.size() )
                old_vec = 0 * vectors[ 0 ];
            
            PRINT( norm_inf( old_vec - vectors[0] ) );

            if ( boolean_(this->non_linear_iterative_criterium) and norm_inf( old_vec - vectors[0] ) <= abs( this->non_linear_iterative_criterium ) )
                break;
            if ( this->non_linear_iterative_criterium_vec.size() ) {
                bool converged = true;
                unsigned num_unk = 0;
                for(unsigned n=0;n<nb_nodal_unknowns;++n,++num_unk) {
                    for(unsigned i=0;i<m->node_list.size() and converged;++i) {
                        unsigned ind = (*indice_noda)[i] + n;
                        converged &= abs( old_vec[ind] - vectors[0][ind] ) <= abs( this->non_linear_iterative_criterium_vec[ num_unk ] );
                    }
                }
                converged &= not find( m->elem_list, ChecKNLConv(), this );
                for(unsigned n=0;n<nb_global_unknowns;++n,++num_unk) {
                    unsigned ind = *indice_glob + n;
                    converged &= abs( old_vec[ind] - vectors[0][ind] ) <= abs( this->non_linear_iterative_criterium_vec[ num_unk ] );
                }
                if ( converged )
                    break;
            }

            if ( nb_iterations++ >= this->max_non_linear_iteration )
                throw SolveException();
            old_vec = vectors[0];
        }
        update_variables();
        call_after_solve();
        return true;
    }

    //    ///
    //    template<class SD>
    //    void add_delta_derivatives( const Vec<ScalarType> &d, const ST &sd ) {
    //        for( int s = 0, c = 0; s < sd.nb_steps(); ++s ) {
    //            sd.add_delta_derivatives( *m, s );
    //            for( unsigned i = 0; i < nb_der_var; ++i, ++c ) {
    //                assemble_vector_der_var( i );
    //                der[ c ] = I * sollicitation;
    //            }
    //        }
    //    }
    /// SD stands for "step derivative". SD can be for example StdStepDer...
    template<class SD>
    bool solve_and_get_derivatives( Vec<Vec<ScalarType> > &der, const SD &sd, bool der_in_base_node_ordering, Number<0> sym ) {
        assert( 0 /*TODO*/ );
    }

    /// SD stands for "step derivative". SD can be for example StdStepDer...
    template<class SD>
    bool solve_and_get_derivatives( Vec<Vec<ScalarType> > &der, const SD &sd, bool der_in_base_node_ordering, Number<1> sym ) {
        assert( this->non_linear_iterative_criterium == ScalarType(0.) );
        assert( MatCarac<0>::symm );
        //
        allocate_matrices();
        shift();
        assemble();

        //
        #if WITH_CHOLMOD
        Mat<double,Sym<>,SparseCholMod> I( matrices(Number<0>()) );
        I.get_factorization();
        vectors[ 0 ] = I.solve( sollicitation );
        #else
        Inv<ScalarType,Sym<>,SparseLine<> > I( matrices(Number<0>()) );
        vectors[ 0 ] = I * sollicitation;
        #endif
        update_variables();
        call_after_solve();

        Vec<Vec<ScalarType> > tmp;
        tmp.resize( nb_der_var * sd.nb_steps() );
        for( int s = 0, c = 0; s < sd.nb_steps(); ++s ) {
            sd( *m, s );
            for( unsigned i = 0; i < nb_der_var; ++i, ++c ) {
                assemble_vector_der_var( i );
                #if WITH_CHOLMOD
                tmp[ c ] = I.solve( sollicitation );
                #else
                tmp[ c ] = I * sollicitation;
                #endif
            }
        }

        // der
        if ( der_in_base_node_ordering ) {
            der.resize( tmp.size() );
            for( int i = 0; i < der.size(); ++i ) {
                der[ i ].resize( m->dim * m->node_list.size() );
                for( int j = 0; j < m->node_list.size(); ++j )
                    for( int d = 0; d < m->dim; ++d )
                        der[ i ][ j * m->dim + d ] = tmp[ i ][ get_indice_noda( j ) + d ];
            }
        } else
            der = tmp;

        return true;
    }

    /// SD stands for "step derivative". SD can be for example StdStepDer...
    template<class SD>
    bool solve_and_get_derivatives( Vec<Vec<ScalarType> > &der, const SD &sd, bool der_in_base_node_ordering = false ) {
        return solve_and_get_derivatives( der, sd, der_in_base_node_ordering, Number<MatCarac<0>::symm>() );
    }

    /// @see solve_and_get_derivatives
    struct StdStepDer {
        int nb_steps() const { return 1; }
        void operator()( TM &m, int num ) const {}
    };

    /// assumes that operator inv() si available for matrices(Number<0>()) and system is linear symmetric
    /// der_in_base_node_ordering = true if user wants something like indice_node = range( nb_nodes ) * dim
    virtual bool solve_and_get_derivatives( Vec<Vec<ScalarType> > &der, bool der_in_base_node_ordering = false ) {
        return solve_and_get_derivatives( der, StdStepDer(), der_in_base_node_ordering );
    }



    virtual void get_precond() { get_precond( Number<MatCarac<0>::symm>() ); }
    virtual void solve_system_using_precond(ScalarType iterative_criterium) { solve_system_using_precond( iterative_criterium, Number<MatCarac<0>::symm>() ); }
    virtual void get_factorization_matrix() { get_factorization_matrix( Number<MatCarac<0>::symm>() ); }
    virtual void solve_system_using_factorization_matrix() { solve_system_using_factorization_matrix( Number<MatCarac<0>::symm>() ); }

    virtual unsigned get_nb_nodal_unknowns() const { return nb_nodal_unknowns; }
    virtual unsigned get_nb_global_unknowns() const { return nb_global_unknowns; }
    virtual unsigned get_nb_elem_unknowns() const { return nb_nodal_unknowns; }


private:
    template<unsigned num_der_var>
    struct AssembleNodeDerVar {
        template<class TN> void operator()( const TN &n, Formulation &f ) const {
            if ( nb_global_unknowns ) {
                if ( nb_nodal_unknowns ) {
                    unsigned ind[ 2 ] = { (*f.indice_noda)[f.m->node_list.number(n)], *f.indice_glob };
                    add_nodal_vector_der_var(
                        f,
                        f.matrices(Number<0>()),
                        f.sollicitation,
                        f.vectors,
                        n, ind, Number<num_der_var>() );
                }
                else
                    add_nodal_vector_der_var(
                        f,
                        f.matrices(Number<0>()),
                        f.sollicitation,
                        f.vectors,
                        n, f.indice_glob, Number<num_der_var>() );
            }
            else
                add_nodal_vector_der_var(
                    f,
                    f.matrices(Number<0>()),
                    f.sollicitation,
                    f.vectors,
                    n, &(*f.indice_noda)[f.m->node_list.number(n)], Number<num_der_var>() );
        }
    };
    template<unsigned num_der_var>
    struct AssembleElemDerVar {
        template<class TE,class TCE,unsigned n> void adsem(Formulation &f,const Number<true> &n1,const Number<false> &n2,
                const TE &e,const TCE *ce, const Number<n> &nn,unsigned *in) const {
             add_skin_elem_vector_der_var( f, f.matrices(Number<0>()), f.sollicitation, f.vectors, e, *ce, nn, in, Number<num_der_var>() );
        }
        template<class TE,unsigned n,unsigned n2> void ass_skin_elem(const TE &e,unsigned *in,Formulation &f,
                    const Number<n> &nn,const Number<n2> &nn2) const { // sub_mesh(Number<1>()).
            ass_skin_elem( e, in, f, Number<n+1>(), nn2 );
            if ( f.m->sub_mesh(Number<1>()).get_parents_of_EA( f.m->get_children_of(e,Number<1>())[n] ).size()!=1 )
                return;
            //PRINTTYPE( typeid( typename TypeChildrenElement<typename TE::NE,1,n>::T ) );

            typedef typename TM::template SubMesh<1>::T TSubMesh;
            typedef typename TSubMesh::template TElem<typename TypeChildrenElement<typename TE::NE,1,n>::T>::TE TCH;
            TCH *child_elem = static_cast<TCH *>( f.m->get_children_of(e,Number<1>())[n] );
            adsem( f, Number<true>(),Number<false>(), e, child_elem, nn, in );
        }
        template<class TE,unsigned n> void ass_skin_elem( const TE &e, unsigned *in, Formulation &f, const Number<n> &nn, const Number<n> &nn2 ) const {}
        
        template<class TE,class TCE,unsigned n> void adifm(Formulation &f,const Number<true> &n1,const Number<false> &n2,
                const TE &e,const TCE *ce, const Number<n> &nn,unsigned *in) const {
             add_internal_face_vector_der_var( f, f.matrices(Number<0>()), f.sollicitation, f.vectors, e, *ce, nn, in, Number<num_der_var>() );
        }
        template<class TE,unsigned n,unsigned n2> void ass_internal_face(const TE &e,unsigned *in,Formulation &f,
                    const Number<n> &nn,const Number<n2> &nn2) const { // sub_mesh(Number<1>()).
            ass_internal_face( e, in, f, Number<n+1>(), nn2 );
            if ( f.m->sub_mesh(Number<1>()).get_parents_of_EA( f.m->get_children_of(e,Number<1>())[n] ).size()==1 )
                return;
            //PRINTTYPE( typeid( typename TypeChildrenElement<typename TE::NE,1,n>::T ) );

            typedef typename TM::template SubMesh<1>::T TSubMesh;
            typedef typename TSubMesh::template TElem<typename TypeChildrenElement<typename TE::NE,1,n>::T>::TE TCH;
            TCH *child_elem = static_cast<TCH *>( f.m->get_children_of(e,Number<1>())[n] );
            adifm( f, Number<true>(),Number<false>(), e, child_elem, nn, in );
        }
        template<class TE,unsigned n> void ass_internal_face( const TE &e, unsigned *in, Formulation &f, const Number<n> &nn, const Number<n> &nn2 ) const {}
        
        template<class TE> void operator()( const TE &e, const Vec<unsigned> *indice_elem, Formulation &f ) const {
            unsigned in[ TE::nb_nodes + 1 + nb_global_unknowns ];

            if ( nb_nodal_unknowns )
                for(unsigned i=0;i<TE::nb_nodes;++i)
                    in[i] = (*f.indice_noda)[ f.m->node_list.number(*e.node(i)) ];

            typedef CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType> CFE;
            if ( CFE::nb_elementary_unknowns )
                in[ TE::nb_nodes * (nb_nodal_unknowns!=0) ] = indice_elem[TE::num_in_elem_list][e.number];

            if ( nb_global_unknowns )
                in[ TE::nb_nodes * (nb_nodal_unknowns!=0) + (CFE::nb_elementary_unknowns!=0) ] = *f.indice_glob;

            add_elem_vector_der_var(
                f,
                f.matrices(Number<0>()),
                f.sollicitation,
                f.vectors,
                e, in, Number<num_der_var>() );

            // skin elements
            if ( not f.assume_skin_not_needed ) {
                ass_skin_elem( e, in, f, Number<0>() , Number<NbChildrenElement<typename TE::NE,1>::res>() );
                ass_internal_face( e, in, f, Number<0>() , Number<NbChildrenElement<typename TE::NE,1>::res>() );
            }
        }
    };
public:
    ///
    template<unsigned num_der_var>
    void assemble_clean_vector_der_var( Number<num_der_var>, unsigned n ) {
        if ( num_der_var == n ) {
            sollicitation.set(ScalarType(0.0));

            add_global_vector_der_var( *this, matrices(Number<0>()), sollicitation, vectors, Number<num_der_var>() ); // global
            apply( m->node_list, AssembleNodeDerVar<num_der_var>(), *this ); // nodal
            apply( m->elem_list, AssembleElemDerVar<num_der_var>(), indice_elem, *this ); // element (and skin elements)
        } else
            assemble_clean_vector_der_var( Number<num_der_var+1>(), n );
    }
    ///
    void assemble_clean_vector_der_var( Number<nb_der_var>, unsigned n ) {}

    ///
    virtual void assemble_vector_der_var( unsigned num_der_var ) {
        assemble_clean_vector_der_var( Number<0>(), num_der_var );
    }


    void precond() { } /// normalization of the matrix
private:
    void get_precond(const Number<0> &sym) {
        #ifndef WITH_UMFPACK
        precond_matrix = matrices(Number<0>()); incomplete_lu_factorize( precond_matrix );
        #endif
    }
    void get_precond(const Number<1> &sym) {
        #ifndef WITH_UMFPACK
        precond_matrix = matrices(Number<0>());
        incomplete_chol_factorize( precond_matrix );
        #endif
    }
    void solve_system_using_precond(ScalarType iterative_criterium,const Number<0> &sym) {
        #ifndef WITH_UMFPACK
        solve_using_incomplete_lu_factorize( precond_matrix, matrices(Number<0>()), sollicitation, vectors[0], iterative_criterium );
        #endif
    }
    void solve_system_using_precond(ScalarType iterative_criterium,const Number<1> &sym) {
        #ifndef WITH_UMFPACK
        solve_using_incomplete_chol_factorize( precond_matrix, matrices(Number<0>()), sollicitation, vectors[0], iterative_criterium );
        #endif
    }

    void get_factorization_matrix(const Number<0> &sym) {
        #ifndef WITH_LDL
        #ifndef WITH_UMFPACK
        precond_matrix = matrices(Number<0>());
        lu_factorize( precond_matrix );
        #endif
        #endif
    }
    void get_factorization_matrix(const Number<1> &sym) {
        #ifndef WITH_LDL
        #ifndef WITH_UMFPACK
        precond_matrix = matrices(Number<0>());
        chol_factorize( precond_matrix );
        #endif
        #endif
    }
    void solve_system_using_factorization_matrix(const Number<0> &sym) {
        #ifndef WITH_LDL
        #ifndef WITH_UMFPACK
        solve_using_lu_factorize( precond_matrix, sollicitation, vectors[0] );
        #endif
        #else
//         std::cout << "Solveur LDL dans solve_system_using_factorization_matrix" << std::endl;
        solver.get_factorization( matrices(Number<0>()), false );
        vectors[0] = sollicitation;
        solver.solve( vectors[0] );
        #endif
    }
    void solve_system_using_factorization_matrix(const Number<1> &sym) {
        #ifndef WITH_LDL
        #ifndef WITH_UMFPACK
        solve_using_chol_factorize( precond_matrix, sollicitation, vectors[0] );
        #endif
        #else
//         std::cout << "Solveur LDL dans solve_system_using_factorization_matrix" << std::endl;
        solver.get_factorization( matrices(Number<0>()), false );
        vectors[0] = sollicitation;
        solver.solve( vectors[0] );
        #endif
    }

    struct UpdateVarElem {
        template<class TE> void operator()(TE &e,Formulation &f) const {
            CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType>::set_elementary_unknowns(e,f,f.vectors,f.indice_elem[TE::num_in_elem_list][e.number]);
        }
        template<class TE> void operator()(TE &e,Formulation &f,Vec<Vec<ScalarType> > &vectors_) const {
            CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType>::set_elementary_unknowns(e,f,vectors_,f.indice_elem[TE::num_in_elem_list][e.number]);
        }
        template<class TE> void operator()(TE &e,Formulation &f,const ScalarType &partial_ts) const {
            CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType>::set_elementary_unknowns(e,f,f.vectors,f.indice_elem[TE::num_in_elem_list][e.number],partial_ts);
        }
    };
    struct GetInitialCond {
        template<class TE> void operator()(const TE &e,Formulation &f) const {
            CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType>::get_elementary_initial_conditions(e,f,f.vectors,f.indice_elem[TE::num_in_elem_list][e.number]);
        }
        template<class TE> void operator()(const TE &e,Formulation &f,Vec<Vec<ScalarType> > &vectors_) const {
            CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType>::get_elementary_initial_conditions(e,f,vectors_,f.indice_elem[TE::num_in_elem_list][e.number]);
        }
    };
    struct CallAfterSolve {
        template<class TE,class TV> void operator()(TE &e,Formulation &f,TV &vectors_) const {
            unsigned in[ TE::nb_nodes+1 ];
            for(unsigned i=0;i<TE::nb_nodes;++i)
                in[i] = (*f.indice_noda)[ f.m->node_list.number(*e.node(i)) ];
            in[ TE::nb_nodes ] = f.indice_elem[TE::num_in_elem_list][e.number];
            CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType>::after_solve(e,f,vectors_,in);
        }
    };
    //
    struct CallAfterSolve_2 {
        template<class TE,class TV> void operator()(TE &e,Formulation &f,TV &vectors_) const {
            unsigned in[ TE::nb_nodes+1 ];
            for(unsigned i=0;i<TE::nb_nodes;++i)
                in[i] = (*f.indice_noda)[ f.m->node_list.number(*e.node(i)) ];
            in[ TE::nb_nodes ] = f.indice_elem[TE::num_in_elem_list][e.number];
            CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType>::after_solve_2(e,f,vectors_,in);
        }
    };
    //
    struct CallAfterSolve_3 {
        template<class TE,class TV> void operator()(TE &e,Formulation &f,TV &vectors_) const {
            unsigned in[ TE::nb_nodes+1 ];
            for(unsigned i=0;i<TE::nb_nodes;++i)
                in[i] = (*f.indice_noda)[ f.m->node_list.number(*e.node(i)) ];
            in[ TE::nb_nodes ] = f.indice_elem[TE::num_in_elem_list][e.number];
            CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType>::after_solve_3(e,f,vectors_,in);
        }
    };
    //
    struct CallAfterSolve_4 {
        template<class TE,class TV> void operator()(TE &e,Formulation &f,TV &vectors_) const {
            unsigned in[ TE::nb_nodes+1 ];
            for(unsigned i=0;i<TE::nb_nodes;++i)
                in[i] = (*f.indice_noda)[ f.m->node_list.number(*e.node(i)) ];
            in[ TE::nb_nodes ] = f.indice_elem[TE::num_in_elem_list][e.number];
            CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType>::after_solve_4(e,f,vectors_,in);
        }
    };
    //
    struct CallAfterSolve_5 {
        template<class TE,class TV> void operator()(TE &e,Formulation &f,TV &vectors_) const {
            unsigned in[ TE::nb_nodes+1 ];
            for(unsigned i=0;i<TE::nb_nodes;++i)
                in[i] = (*f.indice_noda)[ f.m->node_list.number(*e.node(i)) ];
            in[ TE::nb_nodes ] = f.indice_elem[TE::num_in_elem_list][e.number];
            CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType>::after_solve_5(e,f,vectors_,in);
        }
    };
    //
    struct CallAfterSolve_6 {
        template<class TE,class TV> void operator()(TE &e,Formulation &f,TV &vectors_) const {
            unsigned in[ TE::nb_nodes+1 ];
            for(unsigned i=0;i<TE::nb_nodes;++i)
                in[i] = (*f.indice_noda)[ f.m->node_list.number(*e.node(i)) ];
            in[ TE::nb_nodes ] = f.indice_elem[TE::num_in_elem_list][e.number];
            CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType>::after_solve_6(e,f,vectors_,in);
        }
    };
    struct CallAfterSolve_7 {
        template<class TE,class TV> void operator()(TE &e,Formulation &f,TV &vectors_) const {
            unsigned in[ TE::nb_nodes+1 ];
            for(unsigned i=0;i<TE::nb_nodes;++i)
                in[i] = (*f.indice_noda)[ f.m->node_list.number(*e.node(i)) ];
            in[ TE::nb_nodes ] = f.indice_elem[TE::num_in_elem_list][e.number];
            CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType>::after_solve_7(e,f,vectors_,in);
        }
    };
    struct CallAfterSolve_8 {
        template<class TE,class TV> void operator()(TE &e,Formulation &f,TV &vectors_) const {
            unsigned in[ TE::nb_nodes+1 ];
            for(unsigned i=0;i<TE::nb_nodes;++i)
                in[i] = (*f.indice_noda)[ f.m->node_list.number(*e.node(i)) ];
            in[ TE::nb_nodes ] = f.indice_elem[TE::num_in_elem_list][e.number];
            CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType>::after_solve_8(e,f,vectors_,in);
        }
    };
    struct CallAfterSolve_9 {
        template<class TE,class TV> void operator()(TE &e,Formulation &f,TV &vectors_) const {
            unsigned in[ TE::nb_nodes+1 ];
            for(unsigned i=0;i<TE::nb_nodes;++i)
                in[i] = (*f.indice_noda)[ f.m->node_list.number(*e.node(i)) ];
            in[ TE::nb_nodes ] = f.indice_elem[TE::num_in_elem_list][e.number];
            CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType>::after_solve_9(e,f,vectors_,in);
        }
    };
    struct CallAfterSolve_10 {
        template<class TE,class TV> void operator()(TE &e,Formulation &f,TV &vectors_) const {
            unsigned in[ TE::nb_nodes+1 ];
            for(unsigned i=0;i<TE::nb_nodes;++i)
                in[i] = (*f.indice_noda)[ f.m->node_list.number(*e.node(i)) ];
            in[ TE::nb_nodes ] = f.indice_elem[TE::num_in_elem_list][e.number];
            CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType>::after_solve_10(e,f,vectors_,in);
        }
    };
    struct CallAfterSolve_11 {
        template<class TE,class TV> void operator()(TE &e,Formulation &f,TV &vectors_) const {
            unsigned in[ TE::nb_nodes+1 ];
            for(unsigned i=0;i<TE::nb_nodes;++i)
                in[i] = (*f.indice_noda)[ f.m->node_list.number(*e.node(i)) ];
            in[ TE::nb_nodes ] = f.indice_elem[TE::num_in_elem_list][e.number];
            CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType>::after_solve_11(e,f,vectors_,in);
        }
    };
    struct CallAfterSolve_12 {
        template<class TE,class TV> void operator()(TE &e,Formulation &f,TV &vectors_) const {
            unsigned in[ TE::nb_nodes+1 ];
            for(unsigned i=0;i<TE::nb_nodes;++i)
                in[i] = (*f.indice_noda)[ f.m->node_list.number(*e.node(i)) ];
            in[ TE::nb_nodes ] = f.indice_elem[TE::num_in_elem_list][e.number];
            CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType>::after_solve_12(e,f,vectors_,in);
        }
    };
    struct CallAfterSolve_13 {
        template<class TE,class TV> void operator()(TE &e,Formulation &f,TV &vectors_) const {
            unsigned in[ TE::nb_nodes+1 ];
            for(unsigned i=0;i<TE::nb_nodes;++i)
                in[i] = (*f.indice_noda)[ f.m->node_list.number(*e.node(i)) ];
            in[ TE::nb_nodes ] = f.indice_elem[TE::num_in_elem_list][e.number];
            CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType>::after_solve_13(e,f,vectors_,in);
        }
    };
    struct CallAfterSolve_14 {
        template<class TE,class TV> void operator()(TE &e,Formulation &f,TV &vectors_) const {
            unsigned in[ TE::nb_nodes+1 ];
            for(unsigned i=0;i<TE::nb_nodes;++i)
                in[i] = (*f.indice_noda)[ f.m->node_list.number(*e.node(i)) ];
            in[ TE::nb_nodes ] = f.indice_elem[TE::num_in_elem_list][e.number];
            CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType>::after_solve_14(e,f,vectors_,in);
        }
    };
    struct CallAfterSolve_15 {
        template<class TE,class TV> void operator()(TE &e,Formulation &f,TV &vectors_) const {
            unsigned in[ TE::nb_nodes+1 ];
            for(unsigned i=0;i<TE::nb_nodes;++i)
                in[i] = (*f.indice_noda)[ f.m->node_list.number(*e.node(i)) ];
            in[ TE::nb_nodes ] = f.indice_elem[TE::num_in_elem_list][e.number];
            CaracFormulationForElement<NameFormulation,TE,NameVariant,ScalarType>::after_solve_15(e,f,vectors_,in);
        }
    };
public:
    /**
     * call all after_solve in carac elem ( generated by apply_on_elements_after_solve() in formulation_...py )
     */
    virtual void set_vectors_assembly( Vec<Vec<ScalarType> > &vec ){
        vectors_assembly = &vec;
    }
    //
    virtual void set_indice_noda(Vec<unsigned> &vec){
        indice_noda = &vec;
    }
    //
    virtual void set_indice_elem(Vec<unsigned>* vec){
        indice_elem = vec;
    }
    //
    virtual void set_indice_glob(unsigned &val){
        indice_glob = &val;
    }
    virtual void set_f_nodal(Vec<ScalarType>* vec){
        f_nodal = vec;
    }
    virtual void call_after_solve() {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply( m->elem_list, CallAfterSolve(), *this, vectors );
        } else {
            apply( m->elem_list, CallAfterSolve(), *this, *vectors_assembly );
        }

    }
    virtual void call_after_solve(unsigned nb_thread) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply_mt( m->elem_list, nb_thread, CallAfterSolve(), *this, vectors );
        } else {
            apply_mt( m->elem_list, nb_thread, CallAfterSolve(), *this, *vectors_assembly );
        }

    }
    virtual void call_after_solve(Vec<Vec<ScalarType> > &vectors_) {
        apply( m->elem_list, CallAfterSolve(), *this, vectors_ );
    }
    virtual void call_after_solve(Vec<Vec<ScalarType> > &vectors_, unsigned nb_thread ) {
        apply_mt( m->elem_list, nb_thread, CallAfterSolve(), *this, vectors_ );
    }

    virtual void call_after_solve_2() {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply( m->elem_list, CallAfterSolve_2(), *this, vectors );
        } else {
            apply( m->elem_list, CallAfterSolve_2(), *this, *vectors_assembly );
        }
    }
    virtual void call_after_solve_2(unsigned nb_thread) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply_mt( m->elem_list, nb_thread, CallAfterSolve_2(), *this, vectors );
        } else {
            apply_mt( m->elem_list, nb_thread, CallAfterSolve_2(), *this, *vectors_assembly );
        }
    }
    virtual void call_after_solve_2(Vec<Vec<ScalarType> > &vectors_)  {
        apply( m->elem_list, CallAfterSolve_2(), *this, vectors_ );
    }
    virtual void call_after_solve_2(Vec<Vec<ScalarType> > &vectors_, unsigned nb_thread ) {
        apply_mt( m->elem_list, nb_thread, CallAfterSolve_2(), *this, vectors_ );
    }

    virtual void call_after_solve_3() {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply( m->elem_list, CallAfterSolve_3(), *this, vectors );
        } else {
            apply( m->elem_list, CallAfterSolve_3(), *this, *vectors_assembly );
        }
    }
    virtual void call_after_solve_3(unsigned nb_thread) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply_mt( m->elem_list, nb_thread, CallAfterSolve_3(), *this, vectors );
        } else {
            apply_mt( m->elem_list, nb_thread, CallAfterSolve_3(), *this, *vectors_assembly );
        }
    }
    virtual void call_after_solve_3(Vec<Vec<ScalarType> > &vectors_)  {
        apply( m->elem_list, CallAfterSolve_3(), *this, vectors_ );
    }
    virtual void call_after_solve_3(Vec<Vec<ScalarType> > &vectors_, unsigned nb_thread ) {
        apply_mt( m->elem_list, nb_thread, CallAfterSolve_3(), *this, vectors_ );
    }

    virtual void call_after_solve_4() {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply( m->elem_list, CallAfterSolve_4(), *this, vectors );
        } else {
            apply( m->elem_list, CallAfterSolve_4(), *this, *vectors_assembly );
        }
    }
    virtual void call_after_solve_4(unsigned nb_thread) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply_mt( m->elem_list, nb_thread, CallAfterSolve_4(), *this, vectors );
        } else {
            apply_mt( m->elem_list, nb_thread, CallAfterSolve_4(), *this, *vectors_assembly );
        }
    }
    virtual void call_after_solve_4(Vec<Vec<ScalarType> > &vectors_)  {
        apply( m->elem_list, CallAfterSolve_4(), *this, vectors_ );
    }
    virtual void call_after_solve_4(Vec<Vec<ScalarType> > &vectors_, unsigned nb_thread ) {
        apply_mt( m->elem_list, nb_thread, CallAfterSolve_4(), *this, vectors_ );
    }

    virtual void call_after_solve_5() {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply( m->elem_list, CallAfterSolve_5(), *this, vectors );
        } else {
            apply( m->elem_list, CallAfterSolve_5(), *this, *vectors_assembly );
        }
    }
    virtual void call_after_solve_5(unsigned nb_thread) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply_mt( m->elem_list, nb_thread, CallAfterSolve_5(), *this, vectors );
        } else {
            apply_mt( m->elem_list, nb_thread, CallAfterSolve_5(), *this, *vectors_assembly );
        }
    }
    virtual void call_after_solve_5(Vec<Vec<ScalarType> > &vectors_)  {
        apply( m->elem_list, CallAfterSolve_5(), *this, vectors_ );
    }
    virtual void call_after_solve_5(Vec<Vec<ScalarType> > &vectors_, unsigned nb_thread ) {
        apply_mt( m->elem_list, nb_thread, CallAfterSolve_5(), *this, vectors_ );
    }

    virtual void call_after_solve_6() {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply( m->elem_list, CallAfterSolve_6(), *this, vectors );
        } else {
            apply( m->elem_list, CallAfterSolve_6(), *this, *vectors_assembly );
        }
    }
    virtual void call_after_solve_6(unsigned nb_thread) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply_mt( m->elem_list, nb_thread, CallAfterSolve_6(), *this, vectors );
        } else {
            apply_mt( m->elem_list, nb_thread, CallAfterSolve_6(), *this, *vectors_assembly );
        }
    }
    virtual void call_after_solve_6(Vec<Vec<ScalarType> > &vectors_)  {
        apply( m->elem_list, CallAfterSolve_6(), *this, vectors_ );
    }
    virtual void call_after_solve_6(Vec<Vec<ScalarType> > &vectors_, unsigned nb_thread ) {
        apply_mt( m->elem_list, nb_thread, CallAfterSolve_6(), *this, vectors_ );
    }

    virtual void call_after_solve_7() {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply( m->elem_list, CallAfterSolve_7(), *this, vectors );
        } else {
            apply( m->elem_list, CallAfterSolve_7(), *this, *vectors_assembly );
        }
    }
    virtual void call_after_solve_7(unsigned nb_thread) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply_mt( m->elem_list, nb_thread, CallAfterSolve_7(), *this, vectors );
        } else {
            apply_mt( m->elem_list, nb_thread, CallAfterSolve_7(), *this, *vectors_assembly );
        }
    }
    virtual void call_after_solve_7(Vec<Vec<ScalarType> > &vectors_)  {
        apply( m->elem_list, CallAfterSolve_7(), *this, vectors_ );
    }
    virtual void call_after_solve_7(Vec<Vec<ScalarType> > &vectors_, unsigned nb_thread ) {
        apply_mt( m->elem_list, nb_thread, CallAfterSolve_7(), *this, vectors_ );
    }

    virtual void call_after_solve_8() {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply( m->elem_list, CallAfterSolve_8(), *this, vectors );
        } else {
            apply( m->elem_list, CallAfterSolve_8(), *this, *vectors_assembly );
        }
    }
    virtual void call_after_solve_8(unsigned nb_thread) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply_mt( m->elem_list, nb_thread, CallAfterSolve_8(), *this, vectors );
        } else {
            apply_mt( m->elem_list, nb_thread, CallAfterSolve_8(), *this, *vectors_assembly );
        }
    }
    virtual void call_after_solve_8(Vec<Vec<ScalarType> > &vectors_)  {
        apply( m->elem_list, CallAfterSolve_8(), *this, vectors_ );
    }
    virtual void call_after_solve_8(Vec<Vec<ScalarType> > &vectors_, unsigned nb_thread ) {
        apply_mt( m->elem_list, nb_thread, CallAfterSolve_8(), *this, vectors_ );
    }

    virtual void call_after_solve_9() {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply( m->elem_list, CallAfterSolve_9(), *this, vectors );
        } else {
            apply( m->elem_list, CallAfterSolve_9(), *this, *vectors_assembly );
        }
    }
    virtual void call_after_solve_9(unsigned nb_thread) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply_mt( m->elem_list, nb_thread, CallAfterSolve_9(), *this, vectors );
        } else {
            apply_mt( m->elem_list, nb_thread, CallAfterSolve_9(), *this, *vectors_assembly );
        }
    }
    virtual void call_after_solve_9(Vec<Vec<ScalarType> > &vectors_)  {
        apply( m->elem_list, CallAfterSolve_9(), *this, vectors_ );
    }
    virtual void call_after_solve_9(Vec<Vec<ScalarType> > &vectors_, unsigned nb_thread ) {
        apply_mt( m->elem_list, nb_thread, CallAfterSolve_9(), *this, vectors_ );
    }

    virtual void call_after_solve_10() {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply( m->elem_list, CallAfterSolve_10(), *this, vectors );
        } else {
            apply( m->elem_list, CallAfterSolve_10(), *this, *vectors_assembly );
        }
    }
    virtual void call_after_solve_10(unsigned nb_thread) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply_mt( m->elem_list, nb_thread, CallAfterSolve_10(), *this, vectors );
        } else {
            apply_mt( m->elem_list, nb_thread, CallAfterSolve_10(), *this, *vectors_assembly );
        }
    }
    virtual void call_after_solve_10(Vec<Vec<ScalarType> > &vectors_)  {
        apply( m->elem_list, CallAfterSolve_10(), *this, vectors_ );
    }
    virtual void call_after_solve_10(Vec<Vec<ScalarType> > &vectors_, unsigned nb_thread ) {
        apply_mt( m->elem_list, nb_thread, CallAfterSolve_10(), *this, vectors_ );
    }

    virtual void call_after_solve_11() {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply( m->elem_list, CallAfterSolve_11(), *this, vectors );
        } else {
            apply( m->elem_list, CallAfterSolve_11(), *this, *vectors_assembly );
        }
    }
    virtual void call_after_solve_11(unsigned nb_thread) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply_mt( m->elem_list, nb_thread, CallAfterSolve_11(), *this, vectors );
        } else {
            apply_mt( m->elem_list, nb_thread, CallAfterSolve_11(), *this, *vectors_assembly );
        }
    }
    virtual void call_after_solve_11(Vec<Vec<ScalarType> > &vectors_)  {
        apply( m->elem_list, CallAfterSolve_11(), *this, vectors_ );
    }
    virtual void call_after_solve_11(Vec<Vec<ScalarType> > &vectors_, unsigned nb_thread ) {
        apply_mt( m->elem_list, nb_thread, CallAfterSolve_11(), *this, vectors_ );
    }

    virtual void call_after_solve_12() {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply( m->elem_list, CallAfterSolve_12(), *this, vectors );
        } else {
            apply( m->elem_list, CallAfterSolve_12(), *this, *vectors_assembly );
        }
    }
    virtual void call_after_solve_12(unsigned nb_thread) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply_mt( m->elem_list, nb_thread, CallAfterSolve_12(), *this, vectors );
        } else {
            apply_mt( m->elem_list, nb_thread, CallAfterSolve_12(), *this, *vectors_assembly );
        }
    }
    virtual void call_after_solve_12(Vec<Vec<ScalarType> > &vectors_)  {
        apply( m->elem_list, CallAfterSolve_12(), *this, vectors_ );
    }
    virtual void call_after_solve_12(Vec<Vec<ScalarType> > &vectors_, unsigned nb_thread ) {
        apply_mt( m->elem_list, nb_thread, CallAfterSolve_12(), *this, vectors_ );
    }

    virtual void call_after_solve_13() {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply( m->elem_list, CallAfterSolve_13(), *this, vectors );
        } else {
            apply( m->elem_list, CallAfterSolve_13(), *this, *vectors_assembly );
        }
    }
    virtual void call_after_solve_13(unsigned nb_thread) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply_mt( m->elem_list, nb_thread, CallAfterSolve_13(), *this, vectors );
        } else {
            apply_mt( m->elem_list, nb_thread, CallAfterSolve_13(), *this, *vectors_assembly );
        }
    }
    virtual void call_after_solve_13(Vec<Vec<ScalarType> > &vectors_)  {
        apply( m->elem_list, CallAfterSolve_13(), *this, vectors_ );
    }
    virtual void call_after_solve_13(Vec<Vec<ScalarType> > &vectors_, unsigned nb_thread ) {
        apply_mt( m->elem_list, nb_thread, CallAfterSolve_13(), *this, vectors_ );
    }

    virtual void call_after_solve_14() {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply( m->elem_list, CallAfterSolve_14(), *this, vectors );
        } else {
            apply( m->elem_list, CallAfterSolve_14(), *this, *vectors_assembly );
        }
    }
    virtual void call_after_solve_14(unsigned nb_thread) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply_mt( m->elem_list, nb_thread, CallAfterSolve_14(), *this, vectors );
        } else {
            apply_mt( m->elem_list, nb_thread, CallAfterSolve_14(), *this, *vectors_assembly );
        }
    }
    virtual void call_after_solve_14(Vec<Vec<ScalarType> > &vectors_)  {
        apply( m->elem_list, CallAfterSolve_14(), *this, vectors_ );
    }
    virtual void call_after_solve_14(Vec<Vec<ScalarType> > &vectors_, unsigned nb_thread ) {
        apply_mt( m->elem_list, nb_thread, CallAfterSolve_14(), *this, vectors_ );
    }

    virtual void call_after_solve_15() {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply( m->elem_list, CallAfterSolve_15(), *this, vectors );
        } else {
            apply( m->elem_list, CallAfterSolve_15(), *this, *vectors_assembly );
        }
    }
    virtual void call_after_solve_15(unsigned nb_thread) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            apply_mt( m->elem_list, nb_thread, CallAfterSolve_15(), *this, vectors );
        } else {
            apply_mt( m->elem_list, nb_thread, CallAfterSolve_15(), *this, *vectors_assembly );
        }
    }
    virtual void call_after_solve_15(Vec<Vec<ScalarType> > &vectors_)  {
        apply( m->elem_list, CallAfterSolve_15(), *this, vectors_ );
    }
    virtual void call_after_solve_15(Vec<Vec<ScalarType> > &vectors_, unsigned nb_thread ) {
        apply_mt( m->elem_list, nb_thread, CallAfterSolve_15(), *this, vectors_ );
    }

    virtual void call_after_solve(const Vec<void *> &elem_list) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            for(unsigned i=0;i<elem_list.size();++i)
                TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve(), *this, vectors );
        } else {
            for(unsigned i=0;i<elem_list.size();++i)
            TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve(), *this, *vectors_assembly);
        }
    }
    virtual void call_after_solve_2(const Vec<void *> &elem_list) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            for(unsigned i=0;i<elem_list.size();++i)
                TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve_2(), *this, vectors );
        } else {
            for(unsigned i=0;i<elem_list.size();++i)
                TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve_2(), *this, *vectors_assembly);
        }
    }
    virtual void call_after_solve_3(const Vec<void *> &elem_list) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            for(unsigned i=0;i<elem_list.size();++i)
                TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve_3(), *this, vectors );
        } else {
            for(unsigned i=0;i<elem_list.size();++i)
                TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve_3(), *this, *vectors_assembly);
        }
    }
    virtual void call_after_solve_4(const Vec<void *> &elem_list) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            for(unsigned i=0;i<elem_list.size();++i)
                TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve_4(), *this, vectors );
        } else {
            for(unsigned i=0;i<elem_list.size();++i)
                TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve_4(), *this, *vectors_assembly);
        }
    }
    virtual void call_after_solve_5(const Vec<void *> &elem_list) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            for(unsigned i=0;i<elem_list.size();++i)
                TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve_5(), *this, vectors );
        } else {
            for(unsigned i=0;i<elem_list.size();++i)
                TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve_5(), *this, *vectors_assembly);
        }
    }
    virtual void call_after_solve_6(const Vec<void *> &elem_list) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            for(unsigned i=0;i<elem_list.size();++i)
                TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve_6(), *this, vectors );
        } else {
            for(unsigned i=0;i<elem_list.size();++i)
                TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve_6(), *this, *vectors_assembly);
        }
    }
    virtual void call_after_solve_7(const Vec<void *> &elem_list) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            for(unsigned i=0;i<elem_list.size();++i)
                TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve_7(), *this, vectors );
        } else {
            for(unsigned i=0;i<elem_list.size();++i)
                TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve_7(), *this, *vectors_assembly);
        }
    }
    virtual void call_after_solve_8(const Vec<void *> &elem_list) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            for(unsigned i=0;i<elem_list.size();++i)
                TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve_8(), *this, vectors );
        } else {
            for(unsigned i=0;i<elem_list.size();++i)
                TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve_8(), *this, *vectors_assembly);
        }
    }
    virtual void call_after_solve_9(const Vec<void *> &elem_list) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            for(unsigned i=0;i<elem_list.size();++i)
                TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve_9(), *this, vectors );
        } else {
            for(unsigned i=0;i<elem_list.size();++i)
                TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve_9(), *this, *vectors_assembly);
        }
    }
    virtual void call_after_solve_10(const Vec<void *> &elem_list) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            for(unsigned i=0;i<elem_list.size();++i)
                TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve_10(), *this, vectors );
        } else {
            for(unsigned i=0;i<elem_list.size();++i)
                TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve_10(), *this, *vectors_assembly);
        }
    }
    virtual void call_after_solve_11(const Vec<void *> &elem_list) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            for(unsigned i=0;i<elem_list.size();++i)
                TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve_11(), *this, vectors );
        } else {
            for(unsigned i=0;i<elem_list.size();++i)
                TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve_11(), *this, *vectors_assembly);
        }
    }
    virtual void call_after_solve_12(const Vec<void *> &elem_list) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            for(unsigned i=0;i<elem_list.size();++i)
                TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve_12(), *this, vectors );
        } else {
            for(unsigned i=0;i<elem_list.size();++i)
                TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve_12(), *this, *vectors_assembly);
        }
    }
    virtual void call_after_solve_13(const Vec<void *> &elem_list) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            for(unsigned i=0;i<elem_list.size();++i)
                TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve_13(), *this, vectors );
        } else {
            for(unsigned i=0;i<elem_list.size();++i)
                TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve_13(), *this, *vectors_assembly);
        }
    }
    virtual void call_after_solve_14(const Vec<void *> &elem_list) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            for(unsigned i=0;i<elem_list.size();++i)
                TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve_14(), *this, vectors );
        } else {
            for(unsigned i=0;i<elem_list.size();++i)
                TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve_14(), *this, *vectors_assembly);
        }
    }
    virtual void call_after_solve_15(const Vec<void *> &elem_list) {
        if (vectors_assembly == NULL){
            if ( not allocated )
                allocate_matrices();
            for(unsigned i=0;i<elem_list.size();++i)
                TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve_15(), *this, vectors );
        } else {
            for(unsigned i=0;i<elem_list.size();++i)
                TM::TElemList::apply_on_down_cast( reinterpret_cast<typename TM::EA *>(elem_list[i]), CallAfterSolve_15(), *this, *vectors_assembly);
        }
    }
    /*!
      update variable at end of the time step
     */
    virtual void update_variables() {
        for(unsigned i=0;i<m->node_list.size();++i)
            carac.set_nodal_unknowns(m->node_list[i],*this,vectors,(*indice_noda)[i]);
        apply( m->elem_list, UpdateVarElem(), *this );
        carac.set_global_unknowns(m,*this,vectors,*indice_glob);
    }
    //
    virtual void update_variables(Vec<Vec<ScalarType> > &vectors_) {
        for(unsigned i=0;i<m->node_list.size();++i)
            carac.set_nodal_unknowns(m->node_list[i],*this,vectors_,(*indice_noda)[i]);
        apply( m->elem_list, UpdateVarElem(), *this, vectors_ );
        carac.set_global_unknowns(m,*this,vectors_,*indice_glob);
    }
    /*!
      update variable at time t_n + partial_ts
     */
    virtual void update_variables(ScalarType partial_ts) {
        for(unsigned i=0;i<m->node_list.size();++i)
            carac.set_nodal_unknowns(m->node_list[i],*this,vectors,(*indice_noda)[i],partial_ts);
        apply( m->elem_list, UpdateVarElem(), *this, partial_ts );
        carac.set_global_unknowns(m,*this,vectors,*indice_glob,partial_ts);
    }
    /*!
      fill vectors[x] using dern_unknown
     */
    virtual void get_initial_conditions() {
        allocate_matrices();
        for(unsigned i=0;i<m->node_list.size();++i)
            carac.get_nodal_initial_conditions(m->node_list[i],*this,vectors,(*indice_noda)[i]);
        apply( m->elem_list, GetInitialCond(), *this );
        carac.get_global_initial_conditions(m,*this,vectors,*indice_glob);
        initial_condition_initialized = true;
    }
    //
    virtual void get_initial_conditions(Vec<Vec<ScalarType> > &vectors_) {
        for(unsigned i=0;i<m->node_list.size();++i)
            carac.get_nodal_initial_conditions(m->node_list[i],*this,vectors_,(*indice_noda)[i]);
        apply( m->elem_list, GetInitialCond(), *this, vectors_ );
        carac.get_global_initial_conditions(m,*this,vectors_,*indice_glob);
        initial_condition_initialized = true;
    }
    /*!
      add a constraint which will be added to system during assembly
      @param txt constraint in analytic form. Example : "node[0].temperature + time**2 - 10"
      @param penalty_value constraint will be * by max(abs(diag))*penalty_value
      @return number of constraint (usefull in order to remove it...)
    */
    virtual unsigned add_constraint(const std::string &txt,const ScalarType &penalty_value=0) { return add_constraint( Constraint<Formulation>(txt,penalty_value,*this) ); }
    virtual unsigned add_constraint(const Constraint<Formulation> &c) { constraints.push_back( c ); return constraints.size()-1; }

    virtual void add_sollicitation(int type_var,const std::string &val,unsigned nb_in_type,unsigned num_in_vec=0) {
        sollicitations.push_back( Sollicitation<Formulation>(type_var,val,nb_in_type,num_in_vec) );
    }
    virtual void set_initial_time_step( ScalarType ts ) { time_steps = ts; time = -ScalarType(2) * ts; }
    virtual ScalarType get_next_time_step() const { return time_steps[0]; }
    virtual ScalarType get_time() const { return time; }
    virtual void set_time( ScalarType ts ) { time = ts; } /// Attention, pilotage a faire soi-meme si utilisation de cette fonction....
private:
    template<class TMAT> void get_mat_( TMAT *&mat, const Number<0> &n ) {
        std::cerr << "Wrong matrix type. We expected a " << std::endl;
        PRINTTYPE( typeid(matrices(Number<0>())) );
        assert(0);
        mat = NULL;
    }
    template<class TMAT> void get_mat_( TMAT *&mat, const Number<1> &n ) { mat = &matrices(Number<0>()); }
    template<class TMAT> void get_mat_( TMAT *&mat ) { get_mat_( mat, Number< AreSameType< TMAT, TMAT0 >::res >() ); }
public:
    #ifdef WITH_UMFPACK
    virtual void get_mat( Mat<ScalarType,Gen<>,SparseUMFPACK> *&mat ) { get_mat_( mat ); }
    #endif
    virtual void get_mat( Mat<ScalarType,Gen<>,SparseLU> *&mat ) { get_mat_( mat ); }
    #ifdef WITH_CHOLMOD
    virtual void get_mat( Mat<ScalarType,Sym<>,SparseCholMod> *&mat ) { get_mat_( mat ); }
    #endif
    virtual void get_mat( Mat<ScalarType,Sym<>,SparseLine<> > *&mat ) { get_mat_( mat ); }

    virtual Vec<ScalarType> &get_sollicitation() { return sollicitation; }
    virtual Vec<ScalarType> &get_result(unsigned num=0) { return vectors[num]; }

    virtual unsigned nb_constraints() const { return constraints.size(); }
    virtual void erase_constraints() { constraints.resize(0); }
    virtual void erase_constraints_from(unsigned number) { constraints.resize(number); }

    virtual void     set_indice_noda(unsigned num_node, unsigned index) const { (*indice_noda)[num_node] = index; }
    virtual unsigned get_indice_noda(unsigned num_node) const { return (*indice_noda)[num_node]; }
    virtual unsigned num_in_vec_unknown(const std::string &name) const { return carac.num_in_vec_unknown( name ); }

    virtual void display_mat( std::ostream &os ) const {
        os << matrices( Number<0>() );
    }

    //     Vec<ScalarType> fit( const Vec<ScalarType> &exp_val, ScalarType conv ) {
    //         Vec<ScalarType> conv_vec; conv_vec.resize( nb_der_var, conv );
    //         Mat<ScalarType,Sym<>,SparseLine<> > exp_mat( exp_val.size() );
    //         exp_mat.diag() = 1.0;
    //         return fit( exp_mat, exp_val, 0.0, conv_vec );
    //     }

    void get_der_var_on_mesh( unsigned num_der_var ) {
        allocate_matrices();
        shift();
        assemble();
        assemble_vector_der_var( num_der_var );
        //vectors[ 0 ] = inv( matrices(Number<0>()) ) * sollicitation;
        solve_system();
        update_variables();
    }

    virtual Vec<bool> constrained_nodes() const {
        Vec<bool> res;
        res.resize( m->node_list.size(), false );
        for(unsigned i=0;i<constraints.size();++i)
            for(unsigned j=0;j<constraints[i].coeffs.size();++j)
                if ( constraints[i].coeffs[j].type_var == -1 )
                    res[ constraints[i].coeffs[j].num ] = true;
        return res;
    }

    virtual Vec<bool> constrained_nodes_in_dim( unsigned dim ) const {
        Vec<bool> res;
        res.resize( m->node_list.size(), false );
        for(unsigned i=0;i<constraints.size();++i)
            for(unsigned j=0;j<constraints[i].coeffs.size();++j)
                if ( constraints[i].coeffs[j].type_var == -1 and constraints[i].coeffs[j].num_in_vec == dim )
                    res[ constraints[i].coeffs[j].num ] = true;
        return res;
    }

    /// Ajout enrichissements numerique (pointeur vers autres formulation et table de voisinage)
    void add_num_enr(  LMT::FormulationAncestor<ScalarType>* enr_field , Vec< typename TM::TElemList::TListPtr > table_of_neig) {
        number_of_enrich = enrichissements.size()+1;
        enrichissements.push_back(enr_field);
        Neighbor_table.push_back(table_of_neig);
    }

    /*!
        Objectif :
            cette methode ajoute au vecteur <strong> res </strong>, la contribution au residu de l'element <strong> *elem </strong> .
            
        Parametres :
            * <strong> res </strong> le vecteur contenant le residu ( sa taille doit etre egale au nombre de degres de liberte du probleme),
            * <strong> elem </strong> pointeur sur l'element dont on veut obtenir la contribution.
    */
    void add_elem_contribution_to_residual( Vec<ScalarType> &res, const typename TM::EA *elem ) const {
        m->elem_list.apply_on_down_cast( elem, AssembleResidual(), res, *this );
    }

    virtual void add_elem_contribution_to_residual( Vec<ScalarType> &res, const void *elem ) const {
        add_elem_contribution_to_residual( res, reinterpret_cast<const typename TM::EA *>( elem ) );
    }

    virtual Vec<ScalarType> get_residual() const {
        Vec<ScalarType> res;
        res.resize( vectors[ 0 ].size() );
        res.set( ScalarType(0) );
        apply( m->elem_list, AssembleResidual(), res, *this );
        return res;
    }


    TMAT0 &mat() { return matrices( Number<0>() ); }

    TM *m;
    Carac carac;

    HeterogeneousPack<PackMatrices> matrices;
    TMAT0 precond_matrix; /// in case user has called get_precond()
    Vec<Vec<ScalarType>,nb_vectors> vectors;
    Vec<Vec<ScalarType> >* vectors_assembly;      ///< vectors from formulationassembly;
    Vec<ScalarType> sollicitation;
    Vec<ScalarType,nb_vectors> time_steps;
    ScalarType time; /// at end of current step
    Vec<ScalarType> old_vec;

    LocalOperator<NameFormulation, TM::dim, ScalarType>* localOP;

    Vec<Constraint<Formulation> > constraints;
    Vec<Sollicitation<Formulation> > sollicitations;

    bool mat_def_pos_if_sym, initial_condition_initialized, user_want_pierre_precond, mat_has_been_allocated_with_symamd, mat_has_been_allocated_with_symrcm;
    bool allocated;
    Vec<unsigned>* indice_elem;
    Vec<unsigned>* indice_noda;
    unsigned* indice_glob;

    std::vector<Codegen::Ex> symbols;
    Codegen::Ex time_symbol;

    virtual void *get_mesh() {
        return reinterpret_cast< void * > ( m );
    }

    unsigned number_of_enrich;      /// Number of numerical enrichment
    Vec< FormulationAncestor<ScalarType> *> enrichissements;  /// Storage of pointer to formulations used to do sub-level computations
    Vec< Vec < typename TM::TElemList::TListPtr > > Neighbor_table ;          /// Table of neighbor-element for each elem from m_macro in m_micro
    Vec<ScalarType>* f_nodal;

    bool is_numerically_enriched;                                /// Number of numerical enrichment

    pthread_mutex_t mutex;    /// autoverrou pour le multithread
    
    /// instance de l'interface micro-macro et des operateurs _values_ (c.f. Interface_micro_macro.h)
    //Interface_micro_macro<TM,TM::dim,typename TM::Tpos> interface;
    
    static const bool is_global;
    
private:
    Vec<unsigned> indice_elem_internal[ TM::TElemList::nb_sub_type ];
    Vec<unsigned> indice_noda_internal;
    unsigned indice_glob_internal, offset_lagrange_multipliers;
    Vec<ScalarType> f_nodal_internal;

};

/*! To be redefined for each new formulations */
template<class TF,class TMA,class TVE,class TVEVE,unsigned _ms,unsigned _am,unsigned _av,class TN>
void add_nodal_matrix(
    TF &f,
    TMA &matrix,
    TVE &sollicitation,
    TVEVE &vectors,
    const Number<_ms> &matrix_is_sym,
    const Number<_am> &assemble_mat,
    const Number<_av> &assemble_vec,
    const TN &node,
    const unsigned *indices ) {
}

/*! To be redefined for each new formulations */
template<class TF,class TMA,class TVEVE,unsigned _mAs,unsigned _mBs,unsigned _amA,unsigned _amB,class TN>
void add_nodal_matrix(
    TF &f,
    TMA &matrix_A,
    TMA &matrix_B,
    TVEVE &vectors,
    const Number<_mAs> &matrix_A_is_sym,
    const Number<_mBs> &matrix_B_is_sym,
    const Number<_amA> &assemble_mat_A,
    const Number<_amB> &assemble_mat_B,
    const TN &node,
    const unsigned *indices ) {
}

/*! To be redefined for each new formulations */
template<class TF,class TMA,class TVEVE,unsigned _ms,unsigned _am,class TN>
void add_nodal_matrix(
    TF &f,
    TMA &matrix,
    TVEVE &vectors,
    const Number<_ms> &matrix_is_sym,
    const Number<_am> &assemble_mat,
    const TN &node,
    const unsigned *indices ) {
}

/*! To be redefined for each new formulations */
template<class TF,class TMA,class TVE,class TVEVE,unsigned _ms,unsigned _am,unsigned _av>
void add_global_matrix(
    TF &f,
    TMA &matrix,
    TVE &sollicitation,
    TVEVE &vectors,
    const Number<_ms> &matrix_is_sym,
    const Number<_am> &assemble_mat,
    const Number<_av> &assemble_vec,
    unsigned indice ) {
}

/*! To be redefined for each new formulations */
template<class TF,class TMA,class TVEVE,unsigned _mAs,unsigned _mBs,unsigned _amA,unsigned _amB>
void add_global_matrix(
    TF &f,
    TMA &matrix_A,
    TMA &matrix_B,
    TVEVE &vectors,
    const Number<_mAs> &matrix_A_is_sym,
    const Number<_mBs> &matrix_B_is_sym,
    const Number<_amA> &assemble_mat_A,
    const Number<_amB> &assemble_mat_B,
    unsigned indice ) {
}

/*! To be redefined for each new formulations */
template<class TF,class TMA,class TVEVE,unsigned _ms,unsigned _am>
void add_global_matrix(
    TF &f,
    TMA &matrix,
    TVEVE &vectors,
    const Number<_ms> &matrix_is_sym,
    const Number<_am> &assemble_mat,
    unsigned indice ) {
}

/*! To be redefined for each new formulations */
template<class T,class TF,class TMA,class TVE,class TVEVE,unsigned _ms,unsigned _am,unsigned _av,class TE>
void add_local_elem_matrix(
    T pond, const T *var_inter,
    TF &f,
    TMA &matrix,
    TVE &sollicitation,
    TVEVE &vectors,
    const Number<_ms> &matrix_is_sym,
    const Number<_am> &assemble_mat,
    const Number<_av> &assemble_vec,
    const TE &elem,
    const unsigned *indices ) {

    std::cerr << "Attention : ni add_elem_matrix, ni add_local_elem_matrix n'ont ete surdefinis pour la formulation " << f.get_name() << " avec l'element "
              << elem.name << "." << std::endl;
    assert( 0 );
}

/*! To be redefined for each new formulations */
template<class T,class TF,class TMA,class TVEVE,unsigned _mAs,unsigned _mBs,unsigned _amA,unsigned _amB,class TE>
void add_local_elem_matrix(
    T pond, const T *var_inter,
    TF &f,
    TMA &matrix_A,
    TMA &matrix_B,
    TVEVE &vectors,
    const Number<_mAs> &matrix_A_is_sym,
    const Number<_mBs> &matrix_B_is_sym,
    const Number<_amA> &assemble_mat_A,
    const Number<_amB> &assemble_mat_B,
    const TE &elem,
    const unsigned *indices ) {
}

/*! To be redefined for each new formulations */
template<class T,class TF,class TMA,class TVEVE,unsigned _ms,unsigned _am,class TE>
void add_local_elem_matrix(
    T pond, const T *var_inter,
    TF &f,
    TMA &matrix,
    TVEVE &vectors,
    const Number<_ms> &matrix_is_sym,
    const Number<_am> &assemble_mat,
    const TE &elem,
    const unsigned *indices ) {
}

/*! To be redefined for each new formulations */
template<class TF,class TMA,class TVE,class TVEVE,unsigned _ms,unsigned _am,unsigned _av,class TE>
void add_elem_matrix(
        TF &f,
        TMA &matrix,
        TVE &sollicitation,
        TVEVE &vectors,
        const Number<_ms> &matrix_is_sym,
        const Number<_am> &assemble_mat,
        const Number<_av> &assemble_vec,
        const TE &elem,
        const unsigned *indices ) {
    typedef typename TF::ScalarType T;
    typedef CaracFormulationForElement<typename TF::NameFormulation,TE,typename TF::NameVariant,typename TF::ScalarType> CFE;

    for( const double *gp = gauss_point_for_order( CFE::order_integration, typename TE::NameElem() ); *gp!=0.0; gp += elem.nb_var_inter + 1 )
        add_local_elem_matrix( *gp, gp+1, f, matrix, sollicitation , vectors, matrix_is_sym, assemble_mat, assemble_vec, elem, indices );
}

/*! To be redefined for each new formulations */
template<class TF,class TMA,class TVEVE,unsigned _mAs,unsigned _mBs,unsigned _amA,unsigned _amB,class TE>
void add_elem_matrix(
        TF &f,
        TMA &matrix_A,
        TMA &matrix_B,
        TVEVE &vectors,
        const Number<_mAs> &matrix_A_is_sym,
        const Number<_mBs> &matrix_B_is_sym,
        const Number<_amA> &assemble_mat_A,
        const Number<_amB> &assemble_mat_B,
        const TE &elem,
        const unsigned *indices ) {
    typedef typename TF::ScalarType T;
    typedef CaracFormulationForElement<typename TF::NameFormulation,TE,typename TF::NameVariant,typename TF::ScalarType> CFE;

    for( const double *gp = gauss_point_for_order( CFE::order_integration, typename TE::NameElem() ); *gp!=0.0; gp += elem.nb_var_inter + 1 )
        add_local_elem_matrix( *gp, gp+1, f, matrix_A, matrix_B, vectors, matrix_A_is_sym, matrix_B_is_sym, assemble_mat_A, assemble_mat_B, elem, indices );
}

/*! To be redefined for each new formulations */
template<class TF,class TMA,class TVEVE,unsigned _ms,unsigned _am,class TE>
void add_elem_matrix(
        TF &f,
        TMA &matrix,
        TVEVE &vectors,
        const Number<_ms> &matrix_is_sym,
        const Number<_am> &assemble_mat,
        const TE &elem,
        const unsigned *indices ) {
    typedef typename TF::ScalarType T;
    typedef CaracFormulationForElement<typename TF::NameFormulation,TE,typename TF::NameVariant,typename TF::ScalarType> CFE;

    for( const double *gp = gauss_point_for_order( CFE::order_integration, typename TE::NameElem() ); *gp!=0.0; gp += elem.nb_var_inter + 1 )
        add_local_elem_matrix( *gp, gp+1, f, matrix, vectors, matrix_is_sym, assemble_mat, elem, indices );
}

/*! To be redefined for each new formulations */
template<class TF,class TMA,class TVE,class TVEVE,unsigned _ms,unsigned _am,unsigned _av,class TE,class TCE,unsigned _n>
void add_skin_elem_matrix(
        TF &f,
        TMA &matrix,
        TVE &sollicitation,
        TVEVE &vectors,
        const Number<_ms> &matrix_is_sym,
        const Number<_am> &assemble_mat,
        const Number<_av> &assemble_vec,
        const TE &elem, 
        const TCE &skin_elem, 
        const Number<_n> &num_child,
        const unsigned *indices ) {
}

/*! To be redefined for each new formulations */
template<class TF,class TMA,class TVEVE,unsigned _mAs,unsigned _mBs,unsigned _amA,unsigned _amB,class TE,class TCE,unsigned _n>
void add_skin_elem_matrix(
        TF &f,
        TMA &matrix_A,
        TMA &matrix_B,
        TVEVE &vectors,
        const Number<_mAs> &matrix_A_is_sym,
        const Number<_mBs> &matrix_B_is_sym,
        const Number<_amA> &assemble_mat_A,
        const Number<_amB> &assemble_mat_B,
        const TE &elem, 
        const TCE &skin_elem, 
        const Number<_n> &num_child,
        const unsigned *indices ) {
}

/*! To be redefined for each new formulations */
template<class TF,class TMA,class TVEVE,unsigned _ms,unsigned _am,class TE,class TCE,unsigned _n>
void add_skin_elem_matrix(
        TF &f,
        TMA &matrix,
        TVEVE &vectors,
        const Number<_ms> &matrix_is_sym,
        const Number<_am> &assemble_mat,
        const TE &elem, 
        const TCE &skin_elem, 
        const Number<_n> &num_child,
        const unsigned *indices ) {
}

/*! To be redefined for each new formulations */
template<class TF,class TMA,class TVE,class TVEVE,unsigned _ms,unsigned _am,unsigned _av,class TE,class TCE,unsigned _n>
void add_internal_face_matrix(
        TF &f,
        TMA &matrix,
        TVE &sollicitation,
        TVEVE &vectors,
        const Number<_ms> &matrix_is_sym,
        const Number<_am> &assemble_mat,
        const Number<_av> &assemble_vec,
        const TE &elem, 
        const TCE &skin_elem, 
        const Number<_n> &num_child,
        const unsigned *indices ) {
}

/*! To be redefined for each new formulations */
template<class TF,class TMA,class TVEVE,unsigned _mAs,unsigned _mBs,unsigned _amA,unsigned _amB,class TE,class TCE,unsigned _n>
void add_internal_face_matrix(
        TF &f,
        TMA &matrix_A,
        TMA &matrix_B,
        TVEVE &vectors,
        const Number<_mAs> &matrix_A_is_sym,
        const Number<_mBs> &matrix_B_is_sym,
        const Number<_amA> &assemble_mat_A,
        const Number<_amB> &assemble_mat_B,
        const TE &elem, 
        const TCE &skin_elem, 
        const Number<_n> &num_child,
        const unsigned *indices ) {
}

/*! To be redefined for each new formulations */
template<class TF,class TMA,class TVEVE,unsigned _ms,unsigned _am,class TE,class TCE,unsigned _n>
void add_internal_face_matrix(
        TF &f,
        TMA &matrix,
        TVEVE &vectors,
        const Number<_ms> &matrix_is_sym,
        const Number<_am> &assemble_mat,
        const TE &elem, 
        const TCE &skin_elem, 
        const Number<_n> &num_child,
        const unsigned *indices ) {
}

template<class A,class B,class C,class D,class E,class F,class G>
void add_local_elem_residual( const A &a, const B &b, const C &c, const D &d, const E &e, const F &f, const G &g ) {
}

/*! To be redefined for each new formulations */
template<class TF,class TVE,class TVEVE,class TE>
void add_elem_residual(
        TF &f,
        TVE &sollicitation,
        TVEVE &vectors,
        const TE &elem,
        const unsigned *indices ) {
    typedef typename TF::ScalarType T;
    typedef CaracFormulationForElement<typename TF::NameFormulation,TE,typename TF::NameVariant,typename TF::ScalarType> CFE;

    for( const double *gp = gauss_point_for_order( CFE::order_integration, typename TE::NameElem() ); *gp!=0.0; gp += elem.nb_var_inter + 1 )
        add_local_elem_residual( *gp, gp+1, f, sollicitation, vectors, elem, indices );
}


/*! To be redefined for each new formulations */
template<class TF,class TMA,class TVE,class TVEVE,unsigned _ms,unsigned _am,unsigned _av,class TN,unsigned num_der_var>
void add_nodal_vector_der_var(
    TF    &f,
    TMA   &matrix,
    TVE   &sollicitation,
    TVEVE &vectors,
    const TN &node,
    const unsigned *indices,
    Number<num_der_var> ) {
}

/*! To be redefined for each new formulations */
template<class TF,class TMA,class TVE,class TVEVE,unsigned num_der_var>
void add_global_vector_der_var(
    TF    &f,
    TMA   &matrix,
    TVE   &sollicitation,
    TVEVE &vectors,
    Number<num_der_var> ) {
}

/*! To be redefined for each new formulations */
template<class T,class TF,class TMA,class TVE,class TVEVE,class TE,unsigned num_der_var>
void add_local_elem_vector_der_var(
    T pond, const T *var_inter,
    TF &f,
    TMA &matrix,
    TVE &sollicitation,
    TVEVE &vectors,
    const TE &elem,
    const unsigned *indices,
    Number<num_der_var> ) {

    assert( 0 );
}

/*! To be redefined for each new formulations */
template<class TF,class TMA,class TVE,class TVEVE,class TE,unsigned num_der_var>
void add_elem_vector_der_var(
        TF &f,
        TMA   &matrix,
        TVE   &sollicitation,
        TVEVE &vectors,
        const TE &elem,
        const unsigned *indices,
        Number<num_der_var> ) {
    typedef typename TF::ScalarType T;
    typedef CaracFormulationForElement<typename TF::NameFormulation,TE,typename TF::NameVariant,typename TF::ScalarType> CFE;

    for( const double *gp = gauss_point_for_order( CFE::order_integration, typename TE::NameElem() ); *gp!=0.0; gp += elem.nb_var_inter + 1 )
        add_local_elem_vector_der_var(
            *gp, gp+1,
            f,
            matrix,
            sollicitation,
            vectors,
            elem,
            indices,
            Number<num_der_var>()
        );
}

template<class a,class b,class c,class d,class e,class f,class g,class h,class i>
void add_internal_face_vector_der_var( const a&, const b&, const c&, const d&, const e&, const f&, const g&, const h&, const i& ) {
}

}

#endif // LMT_formulation_HEADER
