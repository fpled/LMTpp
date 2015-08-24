#ifndef FORMULATION_ASSEMBLY_H
#define FORMULATION_ASSEMBLY_H
#include "../containers/mat.h"

#ifdef need_f_pointer
    extern LMT::Vec<double>* F_POINTER;
#endif

namespace LMT {

    template<class PB>
    struct FormulationAssembly {
        typedef typename PB::TM TM;
        typedef typename PB::T T;
        typedef typename TypePromote<Abs,T>::T AbsT;
        typedef typename TM::Pvec Pvec;
        static const unsigned dim = TM::dim;

        struct MeshAndForm {
            PB *pb;
            TM *m;
            Vec<unsigned> local_unknowns_to_global_ones;
            Vec<unsigned> indice_noda;
            Vec<unsigned> indice_elem[ TM::TElemList::nb_sub_type ];
            unsigned indice_glob;
            Vec<unsigned> local_ddl_to_global_ones;
            unsigned num_formulation;
        };
        //
        void set_nb_vectors(unsigned nb){
            nb_vectors = nb;
            vectors.resize(nb);
            time_steps.resize(nb);
        }
        //
        FormulationAssembly() {
            set_nb_vectors(unsigned (1));
            time = AbsT(0);
            time_steps[0] = AbsT(1e40);
            mat_already_allocated = false;
            connectivity_calculated =false;
            tol = AbsT(0.000001);
            #ifdef need_f_pointer
                F_POINTER = &f_nodal;
            #endif
        }
        //
        ~FormulationAssembly() {
            for(unsigned i=0;i<formulations.size();++i)
                delete formulations[i].pb;
        }
        //
        bool add( TM &m, const std::string &name ) {
            MeshAndForm *maf = formulations.new_elem();
            maf->pb = new PB( m );
            maf->m = &m;
            for(unsigned i=0;;++i) {
                if ( i == maf->pb->nb_formulations() ) {
                    std::cerr << "Error : unknown formulation " << name << std::endl;
                    return false;
                }
                if ( maf->pb->formulation_nb(i)->get_name() == name ) {
                    maf->num_formulation = i;
                    set_nb_vectors( max(nb_vectors, maf->pb->formulation_nb(i)->get_nb_vectors()));
                    formulation(formulations.size()-1)->set_vectors_assembly(vectors);
                    formulation(formulations.size()-1)->set_indice_noda(maf->indice_noda);
                    formulation(formulations.size()-1)->set_indice_glob(maf->indice_glob);
                    formulation(formulations.size()-1)->set_indice_elem(maf->indice_elem);
                    formulation(formulations.size()-1)->set_f_nodal(&f_nodal);
                    return true;
                }
            }
        }
        //
        FormulationAncestor<T> *formulation(unsigned i) {
            return formulations[i].pb->formulation_nb( formulations[i].num_formulation );
        }
        //
        bool solve(T iterative_criterium=T(0), bool want_f_reaction = true) {
            allocate_matrices();
            assemble();
            solve_system(want_f_reaction);
            call_after_solve();
            update_variables();
            return true;
        }
        //
        void set_initial_time_step(T ts=T(1)) {
            for (unsigned i=0; i<time_steps.size(); ++i) {
                time_steps[i] = ts;
            }
            for(unsigned i=0;i<formulations.size();++i)
                formulation(i)->set_initial_time_step(ts);
        }
        //
        T get_next_time_step() const { return time_steps[0]; }
        //
        T get_time() const { return time; }
        //
        void set_time( T ts ) { time = ts; } /// Attention, pilotage a faire soi-meme si utilisation de cette fonction....
        //
        void shift(int nb=1) {
            while (nb--) {
                time += time_steps[0];
                for(unsigned i=nb_vectors-1;i>0;--i) {
                    time_steps[i] = time_steps[i-1];
                    vectors[i] = vectors[i-1];
                }
                time_steps[0] = time_steps[1];
            }
            for(unsigned i=0;i<formulations.size();++i)
                formulation(i)->set_time( time );
        }
        //
        void unshift(int nb=1) {
            while (nb--) {
                time -= time_steps[1];
                for(unsigned i=0;i<nb_vectors-2;++i) {
                    time_steps[i] = time_steps[i+1];
                    vectors[i] = vectors[i+1];
                }
                time_steps[nb_vectors-1]=time_steps[nb_vectors-2];
                vectors[nb_vectors-1]=vectors[nb_vectors-2];
            }
            for(unsigned i=0;i<formulations.size();++i)
                formulation(i)->set_time( time );
        }
        //
        void allocate_matrices(bool amd=false) {
            if ( mat_already_allocated )
                return;
            mat_already_allocated = true;
            //
            update_connectivity();
            //
            unsigned nb_ddl_per_node = formulations[0].pb->formulation_nb(formulations[0].num_formulation)->get_nb_nodal_unknowns();
            for(unsigned i=0;i<vectors.size();++i)
                vectors[i].resize( pos.size() * nb_ddl_per_node );
            K.clear();
            K.resize( pos.size() * nb_ddl_per_node );
            K_before_constraints.clear();
            K_before_constraints.resize( pos.size() * nb_ddl_per_node );
            K_CL.clear();
            K_CL.resize( pos.size() * nb_ddl_per_node );
            F.resize( pos.size() * nb_ddl_per_node, T(0) );
            F_before_constraints.resize( pos.size() * nb_ddl_per_node, T(0) );
            F_CL.resize( pos.size() * nb_ddl_per_node, T(0) );
            f_reaction.resize( pos.size() * nb_ddl_per_node, T(0) );
            f_reaction.set(T(0));
            f_nodal.resize( pos.size() * nb_ddl_per_node, T(0) );
            X_before_constraints.resize( pos.size() * nb_ddl_per_node, T(0) );
            diag_before_constraints.resize( pos.size() * nb_ddl_per_node, T(0) );
        }
        //
        void assemble_clean_mat(bool assemble_mat=true, bool assemble_vec=true)
        {
            assemble_clean_mat(K, F, assemble_mat, assemble_vec);
        }
        //
        void assemble_clean_mat(Mat<T, Sym<>, SparseLine<> > &K, Vec<T> &F, bool assemble_mat=true, bool assemble_vec=true)
        {
            allocate_matrices();
            if (assemble_mat)
            {
                K.clear();
                K.resize(vectors[0].size());
                for (unsigned i=0; i<formulations.size(); ++i)
                {
                    formulation(i)->assemble_clean_mat(K, F, vectors, true, true); /// si on ne met pas true, true ça n'assemble rien!!
                }
                K_before_constraints = K;
                if (K.nb_rows() == 0)
                {
                    M = AbsT(0);
                }
                else
                {
                    M = max(abs(K.diag()));
                }
            }
            if (assemble_vec)
            {
                F.set(T(0));
                for (unsigned i=0; i<formulations.size(); ++i)
                {
                    formulation(i)->assemble_clean_mat(K, F, vectors, false, true);
                }
                F_before_constraints = F;
            }
        }
        //
        void assemble_sollicitations(bool assemble_mat=true, bool assemble_vec=true)
        {
            assemble_sollicitations(K, F, assemble_mat, assemble_vec);
        }
        //
        void assemble_sollicitations(Mat<T, Sym<>, SparseLine<> > &K, Vec<T> &F, bool assemble_mat=true, bool assemble_vec=true)
        {
            for (unsigned i=0; i<formulations.size(); ++i)
            {
                formulation(i)->assemble_sollicitations(K, F, vectors, assemble_mat, assemble_vec);
            }
        }
        //
        void assemble_constraints(bool assemble_mat=true, bool assemble_vec=true)
        {
            assemble_constraints(K, F, M, assemble_mat, assemble_vec);
        }
        //
        void assemble_constraints(Mat<T, Sym<>, SparseLine<> > &K, Vec<T> &F, AbsT M, bool assemble_mat=true, bool assemble_vec=true)
        {
            for (unsigned i=0; i<formulations.size(); ++i)
            {
                formulation(i)->assemble_constraints(K, F, vectors, M, assemble_mat, assemble_vec);
            }
        }
        //
        void assemble( bool assemble_mat=true, bool assemble_vec=true)
        {
            assemble_clean_mat(assemble_mat, assemble_vec);
            assemble_sollicitations(assemble_mat, assemble_vec);
            assemble_constraints(assemble_mat, assemble_vec);
        }
        //
        void assemble_u() {
            for(unsigned i=0;i<formulations.size();++i) {
                Vec<FormulationAncestor<double>::LinearizedConstraint> data;
                data = formulation(i)->get_linearized_constraints();
                for(unsigned j=0;j<data.size();++j)
                    if ( data[j].coeffs.size() == 1 )
                        vectors[0][data[j].coeffs[0].num_in_vec] = data[j].val;
            }
        }
        //
        void dispatch_results(bool want_f_reaction = true) { ///push result in each f.result
            for(unsigned i=0;i<formulations.size();++i)
                formulation(i)->get_result() = vectors[0][ formulations[i].local_ddl_to_global_ones ];
            if (want_f_reaction){
                f_reaction = K_before_constraints * vectors[0] - F_before_constraints;

#ifdef IFNDEF_f_nodal_2_DM
                for(unsigned i=0;i<formulations.size();++i){
                MeshAndForm &maf = formulations[i];
                for(unsigned j=0;j<maf.m->node_list.size();++j)
                    for(unsigned k=0;k<3;++k)
                        maf.m->node_list[j].f_nodal_2[k] = f_reaction[maf.local_ddl_to_global_ones][j*3+k];
                }
#endif // IFNDEF_f_nodal_2_DM
            }
        }
        //
        bool solve_system(T iterative_criterium=T(0), bool want_f_reaction = true) {
            vectors[0] = inv(K) * F;
            dispatch_results(want_f_reaction);
            return false;
        }
        //
        void update_skin() {
            for(unsigned i=0;i<formulations.size();++i)
                formulation(i)->update_skin();
        }
        //
        void local_update(Vec<unsigned> &TODO) {
            for(unsigned i=0;i<formulations.size();++i)
                if(TODO[i] == 1){
                    TODO[i] = formulation(i)->localOP_local_update();
                    }
        }
        //
        void call_after_solve() {
            for(unsigned i=0;i<formulations.size();++i)
                formulation(i)->call_after_solve(vectors);
        }
        void call_after_solve( unsigned nb_thread ) {
            for(unsigned i=0;i<formulations.size();++i)
                formulation(i)->call_after_solve( vectors, nb_thread );
        }
        //
        void call_after_solve_2() {
            for(unsigned i=0;i<formulations.size();++i)
                formulation(i)->call_after_solve_2(vectors);
        }
        void call_after_solve_2( unsigned nb_thread ) {
            for(unsigned i=0;i<formulations.size();++i)
                formulation(i)->call_after_solve_2( vectors, nb_thread );
        }
        //
        void call_after_solve_3() {
            for(unsigned i=0;i<formulations.size();++i)
                formulation(i)->call_after_solve_3(vectors);
        }
        void call_after_solve_3( unsigned nb_thread ) {
            for(unsigned i=0;i<formulations.size();++i)
                formulation(i)->call_after_solve_3( vectors, nb_thread );
        }
        //
        void call_after_solve_4() {
            for(unsigned i=0;i<formulations.size();++i)
                formulation(i)->call_after_solve_4(vectors);
        }
        void call_after_solve_4( unsigned nb_thread ) {
            for(unsigned i=0;i<formulations.size();++i)
                formulation(i)->call_after_solve_4( vectors, nb_thread );
        }
        //
        void call_after_solve_5() {
            for(unsigned i=0;i<formulations.size();++i)
                formulation(i)->call_after_solve_5(vectors);
        }
        void call_after_solve_5( unsigned nb_thread ) {
            for(unsigned i=0;i<formulations.size();++i)
                formulation(i)->call_after_solve_5( vectors, nb_thread );
        }
        //
        void call_after_solve_6() {
            for(unsigned i=0;i<formulations.size();++i)
                formulation(i)->call_after_solve_6(vectors);
        }
        void call_after_solve_6( unsigned nb_thread ) {
            for(unsigned i=0;i<formulations.size();++i)
                formulation(i)->call_after_solve_6( vectors, nb_thread );
        }
        //
/*        void call_after_solve(const Vec<void *> &elem_list) {
            for(unsigned i=0;i<formulations.size();++i)
                formulation(i)->call_after_solve(elem_list);
        }
        //
        void call_after_solve_2(const Vec<void *> &elem_list) {
            for(unsigned i=0;i<formulations.size();++i)
                formulation(i)->call_after_solve_2(elem_list);
        }
        //
        void call_after_solve_3(const Vec<void *> &elem_list) {
            for(unsigned i=0;i<formulations.size();++i)
                formulation(i)->call_after_solve_3(elem_list);
        }*/
        //
        void localOP_update_variables() {
            for(unsigned i=0;i<formulations.size();++i){
                formulation(i)->localOP_update_variables();
            }
        }
        //
        void update_variables() {
            for(unsigned i=0;i<formulations.size();++i){
                formulation(i)->update_variables(vectors);
            }
        }
        //
        void update_variables(T partial_ts) {
            for(unsigned i=0;i<formulations.size();++i){
                assert(0);
                formulation(i)->update_variables(partial_ts);
            }
        }
        //
        void get_initial_conditions() {
            for(unsigned i=0;i<formulations.size();++i)
                formulation(i)->get_initial_conditions( vectors );
        }
        //
        bool is_unknown(const std::string &s) {
            for(unsigned i=0;i<formulations.size();++i)
                if (formulation(i)->is_unknow(s))
                    return true;
            return false;
        }
        //
        void update_connectivity(){
            if ( connectivity_calculated )
                return;
            connectivity_calculated = true;
            unsigned size = 0;

            MeshAndForm &maf = formulations[0];
            unsigned nb_ddl_per_node = maf.pb->formulation_nb(maf.num_formulation)->get_nb_nodal_unknowns();

            // indice global unknowns
            maf.indice_glob = 0;
            unsigned ng = maf.pb->formulation_nb(maf.num_formulation)->get_nb_global_unknowns();
            for(unsigned i=1;i<formulations.size();++i) {
                //formulation(i)->update_connectivity();
                MeshAndForm &maf = formulations[i];
                maf.indice_glob = ng;
                ng += maf.pb->formulation_nb(maf.num_formulation)->get_nb_global_unknowns();
            }
            size +=ng;
            // nodal unknowns
            pos.resize( 0 );
            pos.reserve( maf.m->node_list.size());
            Vec<Pvec> pos_skin;
            Vec<unsigned> j_skin;
            pos_skin.resize( 0 );
            pos_skin.reserve( maf.m->node_list.size());
            j_skin.resize( 0 );
            j_skin.reserve( maf.m->node_list.size());
            //formulation(0)->update_connectivity();
            maf.local_unknowns_to_global_ones.resize(0);
            maf.local_unknowns_to_global_ones.resize( maf.m->node_list.size() );
            for(unsigned num_node=0;num_node < maf.m->node_list.size();++num_node) {
                if( abs( maf.m->node_list[num_node].is_on_skin ) ){
                    pos_skin.push_back( maf.m->node_list[num_node].pos );
                    j_skin.push_back( pos.size() );
                }
                maf.local_unknowns_to_global_ones[num_node] = pos.size() ;
                pos.push_back( maf.m->node_list[num_node].pos );
            }
            maf.local_ddl_to_global_ones.resize(0);
            maf.local_ddl_to_global_ones.reserve( maf.local_unknowns_to_global_ones.size() * nb_ddl_per_node );
            maf.indice_noda = maf.local_unknowns_to_global_ones * formulation(0)->get_nb_nodal_unknowns() + ng ;

            for(unsigned i=0;i<maf.local_unknowns_to_global_ones.size();++i)
                for(unsigned d=0;d<nb_ddl_per_node;++d)
                    maf.local_ddl_to_global_ones.push_back( maf.local_unknowns_to_global_ones[i] * nb_ddl_per_node + d );

            for(unsigned i=1;i<formulations.size();++i) {
                //formulation(i)->update_connectivity();
                MeshAndForm &maf = formulations[i];
                maf.local_unknowns_to_global_ones.resize( maf.m->node_list.size() );
                pos.reserve(pos.size() + maf.m->node_list.size() );
                pos_skin.reserve(pos_skin.size() + maf.m->node_list.size() );
                j_skin.reserve(j_skin.size() + maf.m->node_list.size() );
                for(unsigned num_node=0;num_node < maf.m->node_list.size();++num_node) {
                    const Vec<T,dim>* node_pos = &(maf.m->node_list[num_node].pos);
                    if ( abs( maf.m->node_list[num_node].is_on_skin ) ) {
                        for(unsigned j=0;;++j) {
                            if ( j == pos_skin.size() ) { // not found
                                maf.local_unknowns_to_global_ones[num_node]= pos.size();
                                pos_skin.push_back( maf.m->node_list[num_node].pos );
                                j_skin.push_back( pos.size() );
                                pos.push_back( *node_pos );
                                break;
                            }
                            if ( abs_indication(norm_2(pos_skin[j] - *node_pos))< abs_indication(tol) ) {
                                maf.local_unknowns_to_global_ones[num_node] = j_skin[j];
                                break;
                            }
                        }
                    } else {
                        maf.local_unknowns_to_global_ones[num_node] = pos.size();
                        pos.push_back( *node_pos );
                    }
                }
                //
                maf.indice_noda = formulations[i].local_unknowns_to_global_ones * formulation(i)->get_nb_nodal_unknowns();
                maf.local_ddl_to_global_ones.resize(maf.local_unknowns_to_global_ones.size() * nb_ddl_per_node );
                for(unsigned j=0;j<maf.local_unknowns_to_global_ones.size();++j)
                    for(unsigned d=0;d<nb_ddl_per_node;++d)
                        maf.local_ddl_to_global_ones[j*nb_ddl_per_node+d] = maf.local_unknowns_to_global_ones[j] * nb_ddl_per_node + d ;
            }
            size += pos.size() + nb_ddl_per_node;
            // elem unknowns. nb_elem_of_type will = nb_unknowns
            unsigned nb_elem_of_type[ TM::TElemList::nb_sub_type ];
            unsigned nb_unknowns_for_type[ TM::TElemList::nb_sub_type ];
            for(unsigned i=0;i<formulations.size();++i) {
                MeshAndForm &maf = formulations[i];
                maf.m->elem_list.get_sizes(nb_elem_of_type);
                //TM::TElemList::apply_static_with_n( maf.pb->formulation_nb(maf.num_formulation)->GetNbUnknownByElement(), nb_elem_of_type, size, nb_unknowns_for_type );
                maf.pb->formulation_nb(maf.num_formulation)->Get_Nb_UnknownByElement( nb_elem_of_type, size, nb_unknowns_for_type);

                unsigned nb_unk_elem = 0;
                for(unsigned i=0,ne=0;i<TM::TElemList::nb_sub_type;++i) {
                    maf.indice_elem[i] = range( ne, ne + nb_elem_of_type[i] ) * nb_unknowns_for_type[i] + size;
                    ne += nb_elem_of_type[i];
                    size += nb_unknowns_for_type[i] * nb_elem_of_type[i];
                    nb_unk_elem += nb_unknowns_for_type[i];
                }
            }

            pos.fit_memory();
            for(unsigned i=0;i<vectors.size();++i) {
                vectors[i].resize( pos.size() * nb_ddl_per_node );
                vectors[i].set(T(0));
            }
        }
        //
        Vec<Splitted<MeshAndForm,2> > formulations;
        Mat<T,Sym<>,SparseLine<> > K, K_before_constraints, K_CL;
        Vec<T> F, F_before_constraints, F_CL, diag_before_constraints, X_before_constraints, f_reaction;
        #ifdef need_f_pointer
            Vec<double> f_nodal;
        #else
            Vec<T> f_nodal;
        #endif
        unsigned nb_vectors;
        Vec<Vec<T> > vectors;
        Vec<AbsT> time_steps;
        AbsT M;                                   ///< Factor de penalisation
        AbsT time;                             ///< at end of current step
        bool mat_already_allocated;
        bool connectivity_calculated;
        Vec<Pvec > pos;
        AbsT tol;
    };

}

#endif // FORMULATION_ASSEMBLY_H
