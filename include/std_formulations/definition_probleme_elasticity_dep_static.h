#ifndef PROBLEM_pb_elasticity_dep_static
#define PROBLEM_pb_elasticity_dep_static

#ifndef Mesh_carac_pb_elasticity_dep_static_HEADER
#define Mesh_carac_pb_elasticity_dep_static_HEADER
#include "mesh/displayparaview.h"
#include "mesh/Hexa.h"
namespace LMT {

template<class TP,unsigned dim> struct Mesh_carac_pb_elasticity_dep_static {};
#ifndef IFNDEF_elastic_modulus_DM
#define IFNDEF_elastic_modulus_DM
    struct elastic_modulus_DM { static std::string name() { return "elastic_modulus"; } };
#endif // IFNDEF_elastic_modulus_DM

#ifndef IFNDEF_tr_epsilon_DM
#define IFNDEF_tr_epsilon_DM
    struct tr_epsilon_DM { static std::string name() { return "tr_epsilon"; } };
#endif // IFNDEF_tr_epsilon_DM

#ifndef IFNDEF_dep_DM
#define IFNDEF_dep_DM
    struct dep_DM { static std::string name() { return "dep"; } };
#endif // IFNDEF_dep_DM

#ifndef IFNDEF_epsilon_DM
#define IFNDEF_epsilon_DM
    struct epsilon_DM { static std::string name() { return "epsilon"; } };
#endif // IFNDEF_epsilon_DM

#ifndef IFNDEF_density_DM
#define IFNDEF_density_DM
    struct density_DM { static std::string name() { return "density"; } };
#endif // IFNDEF_density_DM

#ifndef IFNDEF_pos_DM
#define IFNDEF_pos_DM
    struct pos_DM { static std::string name() { return "pos"; } };
#endif // IFNDEF_pos_DM

#ifndef IFNDEF_f_nodal_DM
#define IFNDEF_f_nodal_DM
    struct f_nodal_DM { static std::string name() { return "f_nodal"; } };
#endif // IFNDEF_f_nodal_DM

#ifndef IFNDEF_poisson_ratio_DM
#define IFNDEF_poisson_ratio_DM
    struct poisson_ratio_DM { static std::string name() { return "poisson_ratio"; } };
#endif // IFNDEF_poisson_ratio_DM

#ifndef IFNDEF_is_on_skin_DM
#define IFNDEF_is_on_skin_DM
    struct is_on_skin_DM { static std::string name() { return "is_on_skin"; } };
#endif // IFNDEF_is_on_skin_DM

#ifndef IFNDEF_f_surf_DM
#define IFNDEF_f_surf_DM
    struct f_surf_DM { static std::string name() { return "f_surf"; } };
#endif // IFNDEF_f_surf_DM

#ifndef IFNDEF_sigma_DM
#define IFNDEF_sigma_DM
    struct sigma_DM { static std::string name() { return "sigma"; } };
#endif // IFNDEF_sigma_DM

#ifndef IFNDEF_f_vol_DM
#define IFNDEF_f_vol_DM
    struct f_vol_DM { static std::string name() { return "f_vol"; } };
#endif // IFNDEF_f_vol_DM

template<class TP>
struct Mesh_carac_pb_elasticity_dep_static<TP,3> {
    typedef TP Tpos;
    static const unsigned dim = 3;
    typedef Vec<TP,3> Pvec;
    struct NodalStaticData {
        typedef Vec<Tpos,3> T0;
        typedef Tpos T1;
        NodalStaticData():dep(0.0),f_nodal(0.0,0.0,0.0),is_on_skin(0),f_surf(0.0,0.0,0.0) {}
        CARACDMEXTNAME( 0, T0, dep, "m" );
        CARACDMEXTNAME( 1, T0, pos, "m" );
        CARACDMEXTNAME( 2, T0, f_nodal, "N" );
        CARACDMEXTNAME( 3, T1, is_on_skin, "" );
        CARACDMEXTNAME( 4, T0, f_surf, "N/m^2" );
        static const unsigned nb_params = 5;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) {
            if ( field_name == "dep" ) { dep = value; return; }
            if ( field_name == "pos" ) { pos = value; return; }
            if ( field_name == "f_nodal" ) { f_nodal = value; return; }
            if ( field_name == "is_on_skin" ) { is_on_skin = value; return; }
            if ( field_name == "f_surf" ) { f_surf = value; return; }
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
        }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
        }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
        }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) {
            if ( field_name == "dep" ) { dep = value; return; }
            if ( field_name == "pos" ) { pos = value; return; }
            if ( field_name == "f_nodal" ) { f_nodal = value; return; }
            if ( field_name == "f_surf" ) { f_surf = value; return; }
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
        }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
        }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
        }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
        }
        void dm_data_set_field( const std::string field_name, const Mat<Tpos,Gen<1> > &value, bool disp = true ) {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
        }
        void dm_data_set_field( const std::string field_name, const Mat<Tpos,Gen<2> > &value, bool disp = true ) {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
        }
        void dm_data_set_field( const std::string field_name, const Mat<Tpos,Gen<3> > &value, bool disp = true ) {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
        }
        void dm_data_set_field( const std::string field_name, const Mat<Tpos,Gen<4> > &value, bool disp = true ) {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
        }
        void dm_data_set_field( const std::string field_name, const Mat<Tpos,Gen<5> > &value, bool disp = true ) {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
        }
        void dm_data_set_field( const std::string field_name, const Mat<Tpos,Gen<6> > &value, bool disp = true ) {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
        }
        Tpos dm_data_get_field( const std::string field_name, StructForType<Tpos>, bool disp = true ) const {
            if ( field_name == "is_on_skin" ) { return is_on_skin; }
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            return Tpos(0);
        }
        Vec<Tpos,1> dm_data_get_field( const std::string field_name, StructForType<Vec<Tpos,1> >, bool disp = true ) const {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            return Vec<Tpos,1>();
        }
        Vec<Tpos,2> dm_data_get_field( const std::string field_name, StructForType<Vec<Tpos,2> >, bool disp = true ) const {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            return Vec<Tpos,2>();
        }
        Vec<Tpos,3> dm_data_get_field( const std::string field_name, StructForType<Vec<Tpos,3> >, bool disp = true ) const {
            if ( field_name == "dep" ) { return dep; }
            if ( field_name == "pos" ) { return pos; }
            if ( field_name == "f_nodal" ) { return f_nodal; }
            if ( field_name == "f_surf" ) { return f_surf; }
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            return Vec<Tpos,3>();
        }
        Vec<Tpos,4> dm_data_get_field( const std::string field_name, StructForType<Vec<Tpos,4> >, bool disp = true ) const {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            return Vec<Tpos,4>();
        }
        Vec<Tpos,5> dm_data_get_field( const std::string field_name, StructForType<Vec<Tpos,5> >, bool disp = true ) const {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            return Vec<Tpos,5>();
        }
        Vec<Tpos,6> dm_data_get_field( const std::string field_name, StructForType<Vec<Tpos,6> >, bool disp = true ) const {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            return Vec<Tpos,6>();
        }
        Mat<Tpos,Gen<1> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Gen<1,1> > >, bool disp = true ) const {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            return Mat<Tpos,Gen<1,1> >();
        }
        Mat<Tpos,Gen<2> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Gen<2,2> > >, bool disp = true ) const {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            return Mat<Tpos,Gen<2,2> >();
        }
        Mat<Tpos,Gen<3> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Gen<3,3> > >, bool disp = true ) const {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            return Mat<Tpos,Gen<3,3> >();
        }
        Mat<Tpos,Gen<4> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Gen<4,4> > >, bool disp = true ) const {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            return Mat<Tpos,Gen<4,4> >();
        }
        Mat<Tpos,Gen<5> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Gen<5,5> > >, bool disp = true ) const {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            return Mat<Tpos,Gen<5,5> >();
        }
        Mat<Tpos,Gen<6> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Gen<6,6> > >, bool disp = true ) const {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            return Mat<Tpos,Gen<6,6> >();
        }
        Mat<Tpos,Sym<1> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Sym<1> > >, bool disp = true ) const {
            assert( 0 /*TODO*/ );
            return Mat<Tpos,Sym<1> >();
        }
        Mat<Tpos,Sym<2> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Sym<2> > >, bool disp = true ) const {
            assert( 0 /*TODO*/ );
            return Mat<Tpos,Sym<2> >();
        }
        Mat<Tpos,Sym<3> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Sym<3> > >, bool disp = true ) const {
            assert( 0 /*TODO*/ );
            return Mat<Tpos,Sym<3> >();
        }
    };
    struct GlobalStaticData {
        typedef Vec<Tpos,3> T1;
        typedef Tpos T0;
        GlobalStaticData():elastic_modulus(210e9),density(7800),poisson_ratio(0.33),f_vol(0.0,0.0,0.0) {}
        CARACDMEXTNAME( 0, T0, elastic_modulus, "N/m^2" );
        CARACDMEXTNAME( 1, T0, density, "kg/m^3" );
        CARACDMEXTNAME( 2, T0, poisson_ratio, "1" );
        CARACDMEXTNAME( 3, T1, f_vol, "N/m^3" );
        static const unsigned nb_params = 4;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) {
            if ( field_name == "elastic_modulus" ) { elastic_modulus = value; return; }
            if ( field_name == "density" ) { density = value; return; }
            if ( field_name == "poisson_ratio" ) { poisson_ratio = value; return; }
            if ( field_name == "f_vol" ) { f_vol = value; return; }
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
        }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
        }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
        }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) {
            if ( field_name == "f_vol" ) { f_vol = value; return; }
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
        }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
        }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
        }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
        }
        void dm_data_set_field( const std::string field_name, const Mat<Tpos,Gen<1> > &value, bool disp = true ) {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
        }
        void dm_data_set_field( const std::string field_name, const Mat<Tpos,Gen<2> > &value, bool disp = true ) {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
        }
        void dm_data_set_field( const std::string field_name, const Mat<Tpos,Gen<3> > &value, bool disp = true ) {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
        }
        void dm_data_set_field( const std::string field_name, const Mat<Tpos,Gen<4> > &value, bool disp = true ) {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
        }
        void dm_data_set_field( const std::string field_name, const Mat<Tpos,Gen<5> > &value, bool disp = true ) {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
        }
        void dm_data_set_field( const std::string field_name, const Mat<Tpos,Gen<6> > &value, bool disp = true ) {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
        }
        Tpos dm_data_get_field( const std::string field_name, StructForType<Tpos>, bool disp = true ) const {
            if ( field_name == "elastic_modulus" ) { return elastic_modulus; }
            if ( field_name == "density" ) { return density; }
            if ( field_name == "poisson_ratio" ) { return poisson_ratio; }
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            return Tpos(0);
        }
        Vec<Tpos,1> dm_data_get_field( const std::string field_name, StructForType<Vec<Tpos,1> >, bool disp = true ) const {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            return Vec<Tpos,1>();
        }
        Vec<Tpos,2> dm_data_get_field( const std::string field_name, StructForType<Vec<Tpos,2> >, bool disp = true ) const {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            return Vec<Tpos,2>();
        }
        Vec<Tpos,3> dm_data_get_field( const std::string field_name, StructForType<Vec<Tpos,3> >, bool disp = true ) const {
            if ( field_name == "f_vol" ) { return f_vol; }
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            return Vec<Tpos,3>();
        }
        Vec<Tpos,4> dm_data_get_field( const std::string field_name, StructForType<Vec<Tpos,4> >, bool disp = true ) const {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            return Vec<Tpos,4>();
        }
        Vec<Tpos,5> dm_data_get_field( const std::string field_name, StructForType<Vec<Tpos,5> >, bool disp = true ) const {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            return Vec<Tpos,5>();
        }
        Vec<Tpos,6> dm_data_get_field( const std::string field_name, StructForType<Vec<Tpos,6> >, bool disp = true ) const {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            return Vec<Tpos,6>();
        }
        Mat<Tpos,Gen<1> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Gen<1,1> > >, bool disp = true ) const {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            return Mat<Tpos,Gen<1,1> >();
        }
        Mat<Tpos,Gen<2> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Gen<2,2> > >, bool disp = true ) const {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            return Mat<Tpos,Gen<2,2> >();
        }
        Mat<Tpos,Gen<3> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Gen<3,3> > >, bool disp = true ) const {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            return Mat<Tpos,Gen<3,3> >();
        }
        Mat<Tpos,Gen<4> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Gen<4,4> > >, bool disp = true ) const {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            return Mat<Tpos,Gen<4,4> >();
        }
        Mat<Tpos,Gen<5> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Gen<5,5> > >, bool disp = true ) const {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            return Mat<Tpos,Gen<5,5> >();
        }
        Mat<Tpos,Gen<6> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Gen<6,6> > >, bool disp = true ) const {
            if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            return Mat<Tpos,Gen<6,6> >();
        }
        Mat<Tpos,Sym<1> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Sym<1> > >, bool disp = true ) const {
            assert( 0 /*TODO*/ );
            return Mat<Tpos,Sym<1> >();
        }
        Mat<Tpos,Sym<2> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Sym<2> > >, bool disp = true ) const {
            assert( 0 /*TODO*/ );
            return Mat<Tpos,Sym<2> >();
        }
        Mat<Tpos,Sym<3> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Sym<3> > >, bool disp = true ) const {
            assert( 0 /*TODO*/ );
            return Mat<Tpos,Sym<3> >();
        }
    };
    typedef Node<dim,Tpos,NodalStaticData> TNode;
    typedef ElementAncestor<TNode> EA;
    template<unsigned nvi_to_subs,unsigned skin,unsigned num_sub_element,unsigned inner=0> struct ElementChoice { typedef void NE; typedef DefaultBehavior BE; typedef VoidDMSet TData; };
    template<unsigned skin,unsigned inner> struct ElementChoice<0,skin,0,inner> { typedef Hexa NE; typedef DefaultBehavior BE; 
        struct TData {
            typedef Vec<Vec<Tpos,6>,1> T1;
            typedef Vec<Tpos,1> T0;
            TData():tr_epsilon(0),epsilon(0),sigma(0) {}
            CARACDMEXTNAME( 0, T0, tr_epsilon, "1" );
            CARACDMEXTNAME( 1, T1, epsilon, "1" );
            CARACDMEXTNAME( 2, T1, sigma, "N/m^2" );
            static const unsigned nb_params = 3;
            void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) {
                if ( field_name == "tr_epsilon" ) { tr_epsilon[0] = value; return; } // hum
                if ( field_name == "epsilon" ) { epsilon[0] = value; return; } // hum
                if ( field_name == "sigma" ) { sigma[0] = value; return; } // hum
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            }
            void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            }
            void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            }
            void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            }
            void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            }
            void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            }
            void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) {
                if ( field_name == "epsilon" ) { epsilon[0] = value; return; } // hum
                if ( field_name == "sigma" ) { sigma[0] = value; return; } // hum
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            }
            void dm_data_set_field( const std::string field_name, const Mat<Tpos,Gen<1> > &value, bool disp = true ) {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            }
            void dm_data_set_field( const std::string field_name, const Mat<Tpos,Gen<2> > &value, bool disp = true ) {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            }
            void dm_data_set_field( const std::string field_name, const Mat<Tpos,Gen<3> > &value, bool disp = true ) {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            }
            void dm_data_set_field( const std::string field_name, const Mat<Tpos,Gen<4> > &value, bool disp = true ) {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            }
            void dm_data_set_field( const std::string field_name, const Mat<Tpos,Gen<5> > &value, bool disp = true ) {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            }
            void dm_data_set_field( const std::string field_name, const Mat<Tpos,Gen<6> > &value, bool disp = true ) {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            }
            Tpos dm_data_get_field( const std::string field_name, StructForType<Tpos>, bool disp = true ) const {
                if ( field_name == "tr_epsilon" ) { return tr_epsilon[0]; } // hum
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
                return Tpos(0);
            }
            Vec<Tpos,1> dm_data_get_field( const std::string field_name, StructForType<Vec<Tpos,1> >, bool disp = true ) const {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
                return Vec<Tpos,1>();
            }
            Vec<Tpos,2> dm_data_get_field( const std::string field_name, StructForType<Vec<Tpos,2> >, bool disp = true ) const {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
                return Vec<Tpos,2>();
            }
            Vec<Tpos,3> dm_data_get_field( const std::string field_name, StructForType<Vec<Tpos,3> >, bool disp = true ) const {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
                return Vec<Tpos,3>();
            }
            Vec<Tpos,4> dm_data_get_field( const std::string field_name, StructForType<Vec<Tpos,4> >, bool disp = true ) const {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
                return Vec<Tpos,4>();
            }
            Vec<Tpos,5> dm_data_get_field( const std::string field_name, StructForType<Vec<Tpos,5> >, bool disp = true ) const {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
                return Vec<Tpos,5>();
            }
            Vec<Tpos,6> dm_data_get_field( const std::string field_name, StructForType<Vec<Tpos,6> >, bool disp = true ) const {
                if ( field_name == "epsilon" ) { return epsilon[0]; } // hum
                if ( field_name == "sigma" ) { return sigma[0]; } // hum
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
                return Vec<Tpos,6>();
            }
            Mat<Tpos,Gen<1> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Gen<1,1> > >, bool disp = true ) const {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
                return Mat<Tpos,Gen<1,1> >();
            }
            Mat<Tpos,Gen<2> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Gen<2,2> > >, bool disp = true ) const {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
                return Mat<Tpos,Gen<2,2> >();
            }
            Mat<Tpos,Gen<3> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Gen<3,3> > >, bool disp = true ) const {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
                return Mat<Tpos,Gen<3,3> >();
            }
            Mat<Tpos,Gen<4> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Gen<4,4> > >, bool disp = true ) const {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
                return Mat<Tpos,Gen<4,4> >();
            }
            Mat<Tpos,Gen<5> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Gen<5,5> > >, bool disp = true ) const {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
                return Mat<Tpos,Gen<5,5> >();
            }
            Mat<Tpos,Gen<6> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Gen<6,6> > >, bool disp = true ) const {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
                return Mat<Tpos,Gen<6,6> >();
            }
            Mat<Tpos,Sym<1> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Sym<1> > >, bool disp = true ) const {
                assert( 0 /*TODO*/ );
                return Mat<Tpos,Sym<1> >();
            }
            Mat<Tpos,Sym<2> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Sym<2> > >, bool disp = true ) const {
                assert( 0 /*TODO*/ );
                return Mat<Tpos,Sym<2> >();
            }
            Mat<Tpos,Sym<3> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Sym<3> > >, bool disp = true ) const {
                assert( 0 /*TODO*/ );
                return Mat<Tpos,Sym<3> >();
            }
        };
    };
    template<unsigned skin,unsigned inner> struct ElementChoice<1,skin,0,inner> { typedef Quad NE; typedef DefaultBehavior BE; 
        struct TData {
            TData() {}
            VOIDDMSET;
            void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            }
            void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            }
            void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            }
            void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            }
            void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            }
            void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            }
            void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            }
            void dm_data_set_field( const std::string field_name, const Mat<Tpos,Gen<1> > &value, bool disp = true ) {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            }
            void dm_data_set_field( const std::string field_name, const Mat<Tpos,Gen<2> > &value, bool disp = true ) {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            }
            void dm_data_set_field( const std::string field_name, const Mat<Tpos,Gen<3> > &value, bool disp = true ) {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            }
            void dm_data_set_field( const std::string field_name, const Mat<Tpos,Gen<4> > &value, bool disp = true ) {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            }
            void dm_data_set_field( const std::string field_name, const Mat<Tpos,Gen<5> > &value, bool disp = true ) {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            }
            void dm_data_set_field( const std::string field_name, const Mat<Tpos,Gen<6> > &value, bool disp = true ) {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
            }
            Tpos dm_data_get_field( const std::string field_name, StructForType<Tpos>, bool disp = true ) const {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
                return Tpos(0);
            }
            Vec<Tpos,1> dm_data_get_field( const std::string field_name, StructForType<Vec<Tpos,1> >, bool disp = true ) const {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
                return Vec<Tpos,1>();
            }
            Vec<Tpos,2> dm_data_get_field( const std::string field_name, StructForType<Vec<Tpos,2> >, bool disp = true ) const {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
                return Vec<Tpos,2>();
            }
            Vec<Tpos,3> dm_data_get_field( const std::string field_name, StructForType<Vec<Tpos,3> >, bool disp = true ) const {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
                return Vec<Tpos,3>();
            }
            Vec<Tpos,4> dm_data_get_field( const std::string field_name, StructForType<Vec<Tpos,4> >, bool disp = true ) const {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
                return Vec<Tpos,4>();
            }
            Vec<Tpos,5> dm_data_get_field( const std::string field_name, StructForType<Vec<Tpos,5> >, bool disp = true ) const {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
                return Vec<Tpos,5>();
            }
            Vec<Tpos,6> dm_data_get_field( const std::string field_name, StructForType<Vec<Tpos,6> >, bool disp = true ) const {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
                return Vec<Tpos,6>();
            }
            Mat<Tpos,Gen<1> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Gen<1,1> > >, bool disp = true ) const {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
                return Mat<Tpos,Gen<1,1> >();
            }
            Mat<Tpos,Gen<2> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Gen<2,2> > >, bool disp = true ) const {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
                return Mat<Tpos,Gen<2,2> >();
            }
            Mat<Tpos,Gen<3> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Gen<3,3> > >, bool disp = true ) const {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
                return Mat<Tpos,Gen<3,3> >();
            }
            Mat<Tpos,Gen<4> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Gen<4,4> > >, bool disp = true ) const {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
                return Mat<Tpos,Gen<4,4> >();
            }
            Mat<Tpos,Gen<5> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Gen<5,5> > >, bool disp = true ) const {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
                return Mat<Tpos,Gen<5,5> >();
            }
            Mat<Tpos,Gen<6> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Gen<6,6> > >, bool disp = true ) const {
                if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }
                return Mat<Tpos,Gen<6,6> >();
            }
            Mat<Tpos,Sym<1> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Sym<1> > >, bool disp = true ) const {
                assert( 0 /*TODO*/ );
                return Mat<Tpos,Sym<1> >();
            }
            Mat<Tpos,Sym<2> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Sym<2> > >, bool disp = true ) const {
                assert( 0 /*TODO*/ );
                return Mat<Tpos,Sym<2> >();
            }
            Mat<Tpos,Sym<3> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Sym<3> > >, bool disp = true ) const {
                assert( 0 /*TODO*/ );
                return Mat<Tpos,Sym<3> >();
            }
        };
    };
    template<unsigned skin,unsigned inner> struct ElementChoice<2,skin,0,inner> { typedef Bar NE; typedef DefaultBehavior BE; typedef VoidDMSet TData;    };
    template<unsigned skin,unsigned inner> struct ElementChoice<3,skin,0,inner> { typedef NodalElement NE; typedef DefaultBehavior BE; typedef VoidDMSet TData;    };
};
} // namespace LMT
#endif // Mesh_carac_pb_elasticity_dep_static_HEADER

#include "formulation/formulation.h"
namespace LMT {
#ifndef ELASTICITY_DEP_STATIC_3_NUM_0_CARAC_H
#define ELASTICITY_DEP_STATIC_3_NUM_0_CARAC_H
#ifndef STRUCT_ELASTICITY_DEP_STATIC
#define STRUCT_ELASTICITY_DEP_STATIC
struct elasticity_dep_static {
  static const char *name() { return "elasticity_dep_static"; }
};
#endif // ELASTICITY_DEP_STATIC

template<class P_T>
class CaracFormulation<elasticity_dep_static,3,P_T>  {
public:
  typedef P_T T;
  static const char *name() { return "elasticity_dep_static"; }
  static const bool matrix_will_be_definite_positive=true;
  static const bool has_nodal_matrix = false;
  static const bool has_IS_contact_matrix=false;
  static const bool need_skin_assembly=false;
  typedef Norm1_is_inf Name_convergence_criterium;
  static const unsigned nb_vectors = 4;
  static const unsigned nb_matrices = 4;
  static const unsigned auto_contact = false;
  static const bool friction_coeff_is_a_nodal_variable = 0;
  static const unsigned offset_of_pos_unknown=3;
  static const unsigned pos_is_an_unknown = false;
  static const unsigned nb_der_var = 0;
  template<class TF> static void add_to_der_vars( TF &f, const Vec<T> &v ) {
  }
  static bool is_unknown(const std::string &s) { return (s=="dep"); }
  static unsigned num_in_vec_unknown(const std::string &s) { if ( s=="dep" )return 0; return 0;  }
  template<unsigned num_mat,unsigned inner=0> struct NodalMatricesCarac {
      static const bool symm = 1;
      static const bool herm = false;
      static const bool diag = false;
  };
  template<unsigned num_mat,unsigned inner=0> struct GlobalMatricesCarac {
      static const bool symm = 1;
      static const bool herm = false;
      static const bool diag = false;
  };
  
  static const unsigned nb_nodal_unknowns = 3;
  template<class TE,class TTs,class Tvec>
  inline static void set_nodal_unknowns(TE &node,const TTs &f,const Tvec &vecs,unsigned indice) {
    node.dep[0]=vecs[0][indice+0]; node.dep[1]=vecs[0][indice+1]; node.dep[2]=vecs[0][indice+2];
  }
  template<class TE,class TTs,class Tvec>
  inline static void set_nodal_unknowns(TE &node,const TTs &f,const Tvec &vecs,unsigned indice,T partial_ts) {
    node.dep[0]=vecs[0][indice+0]; node.dep[1]=vecs[0][indice+1]; node.dep[2]=vecs[0][indice+2];
  }
  template<class TE,class TTs,class Tvec>
  inline static void get_nodal_initial_conditions(const TE &node,const TTs &f,Tvec &vecs,unsigned indice) {
    vecs[0][indice+0]=node.dep[0]; vecs[1][indice+0]=node.dep[0]; vecs[2][indice+0]=node.dep[0]; vecs[3][indice+0]=node.dep[0]; vecs[4][indice+0]=node.dep[0];
    vecs[0][indice+2]=node.dep[2]; vecs[1][indice+2]=node.dep[2]; vecs[2][indice+2]=node.dep[2]; vecs[3][indice+2]=node.dep[2]; vecs[4][indice+2]=node.dep[2];
    vecs[0][indice+1]=node.dep[1]; vecs[1][indice+1]=node.dep[1]; vecs[2][indice+1]=node.dep[1]; vecs[3][indice+1]=node.dep[1]; vecs[4][indice+1]=node.dep[1];
  }
  template<class TE,class TTs,class Tvec>
  inline static T max_nodal_error(const TE &node,const TTs &f,const Tvec &vecs,int indice) {
    T reg0=vecs[1][indice+0]-vecs[0][indice+0]; T reg1=vecs[1][indice+1]-vecs[0][indice+1]; reg0=abs(reg0); reg1=abs(reg1); T reg2=vecs[1][indice+2]-vecs[0][indice+2];
    reg0=max(reg1,reg0); reg2=abs(reg2); return max(reg2,reg0);
  }
  template<class TE,class TTs,class Tvecs,class Tvec>
  inline static void set_old_vec_nodal(const TE &node,const TTs &f,const Tvecs &vecs,Tvec &old_vec,int indice) {
    old_vec[indice+0]=vecs[1][indice+0]; old_vec[indice+1]=vecs[1][indice+1]; old_vec[indice+2]=vecs[1][indice+2];
  }
  
  static const unsigned nb_global_unknowns = 0;
  template<class TE,class TTs,class Tvec>
  inline static void set_global_unknowns(TE &mesh,const TTs &f,const Tvec &vecs,unsigned indice) {
  }
  template<class TE,class TTs,class Tvec>
  inline static void set_global_unknowns(TE &mesh,const TTs &f,const Tvec &vecs,unsigned indice,T partial_ts) {
  }
  template<class TE,class TTs,class Tvec>
  inline static void get_global_initial_conditions(const TE &mesh,const TTs &f,Tvec &vecs,unsigned indice) {
  }
  template<class TE,class TTs,class Tvec>
  inline static T max_global_error(const TE &mesh,const TTs &f,const Tvec &vecs,int indice) {
    return 0;
  }
  template<class TE,class TTs,class Tvecs,class Tvec>
  inline static void set_old_vec_global(const TE &mesh,const TTs &f,const Tvecs &vecs,Tvec &old_vec,int indice) {
  }
  
  static const unsigned nb_skin_nodal_unknowns = 0;
  template<class TE,class TTs,class Tvec>
  inline static void set_skin_nodal_unknowns(TE &node,const TTs &f,const Tvec &vecs,unsigned indice) {
  }
  template<class TE,class TTs,class Tvec>
  inline static void set_skin_nodal_unknowns(TE &node,const TTs &f,const Tvec &vecs,unsigned indice,T partial_ts) {
  }
  template<class TE,class TTs,class Tvec>
  inline static void get_skin_nodal_initial_conditions(const TE &node,const TTs &f,Tvec &vecs,unsigned indice) {
  }
  template<class TE,class TTs,class Tvec>
  inline static T max_skin_nodal_error(const TE &node,const TTs &f,const Tvec &vecs,int indice) {
    return 0;
  }
  template<class TE,class TTs,class Tvecs,class Tvec>
  inline static void set_old_vec_skin_nodal(const TE &node,const TTs &f,const Tvecs &vecs,Tvec &old_vec,int indice) {
  }
  
  static const unsigned nb_skin_global_unknowns = 0;
  template<class TE,class TTs,class Tvec>
  inline static void set_skin_global_unknowns(TE &mesh,const TTs &f,const Tvec &vecs,unsigned indice) {
  }
  template<class TE,class TTs,class Tvec>
  inline static void set_skin_global_unknowns(TE &mesh,const TTs &f,const Tvec &vecs,unsigned indice,T partial_ts) {
  }
  template<class TE,class TTs,class Tvec>
  inline static void get_skin_global_initial_conditions(const TE &mesh,const TTs &f,Tvec &vecs,unsigned indice) {
  }
  template<class TE,class TTs,class Tvec>
  inline static T max_skin_global_error(const TE &mesh,const TTs &f,const Tvec &vecs,int indice) {
    return 0;
  }
  template<class TE,class TTs,class Tvecs,class Tvec>
  inline static void set_old_vec_skin_global(const TE &mesh,const TTs &f,const Tvecs &vecs,Tvec &old_vec,int indice) {
  }
};
#endif // ELASTICITY_DEP_STATIC_3_NUM_0_CARAC_H
extern "C" void apply_on_elements_after_solve_0_elasticity_dep_static_Hexa_0( double * );
extern "C" void apply_on_elements_after_solve_1_elasticity_dep_static_Hexa_1( double * );
extern "C" void apply_on_elements_after_solve_2_elasticity_dep_static_Hexa_2( double * );
extern "C" void apply_on_elements_after_solve_3_elasticity_dep_static_Hexa_3( double * );
extern "C" void apply_on_elements_after_solve_4_elasticity_dep_static_Hexa_4( double * );
extern "C" void apply_on_elements_after_solve_5_elasticity_dep_static_Hexa_5( double * );
extern "C" void apply_on_elements_after_solve_6_elasticity_dep_static_Hexa_6( double * );
extern "C" void apply_on_elements_after_solve_7_elasticity_dep_static_Hexa_7( double * );
extern "C" void apply_on_elements_after_solve_8_elasticity_dep_static_Hexa_8( double * );
extern "C" void apply_on_elements_after_solve_9_elasticity_dep_static_Hexa_9( double * );
extern "C" void apply_on_elements_after_solve_10_elasticity_dep_static_Hexa_10( double * );
extern "C" void apply_on_elements_after_solve_11_elasticity_dep_static_Hexa_11( double * );
extern "C" void apply_on_elements_after_solve_12_elasticity_dep_static_Hexa_12( double * );
extern "C" void apply_on_elements_after_solve_13_elasticity_dep_static_Hexa_13( double * );
extern "C" void apply_on_elements_after_solve_14_elasticity_dep_static_Hexa_14( double * );
class Hexa;
template<unsigned A,class B,class C> class Node;
template<class A,class B,class C,class D,unsigned E> class Element;

// Carac for ...
template<class P_T_pos,class P_ND,class TED,unsigned nim,class TM,class T>
class CaracFormulationForElement<elasticity_dep_static,Element<Hexa,DefaultBehavior,Node<3,P_T_pos,P_ND>,TED,nim>,TM,T> {
public:
    template<unsigned num_mat,unsigned inner=0> struct ElemMatricesCarac {
        static const bool symm = true;
        static const bool herm = false;
        static const bool diag = false;
        static const bool linear = true;
    };
    static const unsigned order_integration = 2;
    static const bool has_elementary_matrix = false;
    static const bool has_skin_elementary_matrix = false;
    template<class TE,class TF, class TVEVE> static void after_solve(TE &elem,TF &f,TVEVE &vectors,const unsigned *indices) {
      #define PNODE(N) (*elem.node(N))
    T reg0=0.25*elem.pos(0)[1]; T reg1=0.25*elem.pos(1)[2]; T reg2=0.25*elem.pos(1)[1]; T reg3=0.25*elem.pos(0)[2]; T reg4=reg2-reg0;
    reg0=reg2+reg0; reg2=reg3+reg1; T reg5=0.25*elem.pos(2)[1]; T reg6=0.25*elem.pos(2)[2]; reg3=reg1-reg3;
    reg1=reg5+reg0; T reg7=0.25*elem.pos(3)[1]; T reg8=0.25*elem.pos(3)[2]; T reg9=0.25*elem.pos(0)[0]; reg4=reg4+reg5;
    T reg10=0.25*elem.pos(1)[0]; T reg11=reg2+reg6; reg2=reg6-reg2; reg3=reg6+reg3; reg0=reg5-reg0;
    reg5=0.25*elem.pos(4)[2]; reg6=0.25*elem.pos(4)[1]; reg4=reg4-reg7; T reg12=reg9+reg10; reg2=reg8+reg2;
    reg0=reg7+reg0; reg3=reg3-reg8; T reg13=0.25*elem.pos(2)[0]; reg9=reg10-reg9; reg1=reg7+reg1;
    reg8=reg11+reg8; reg3=reg3-reg5; reg7=0.25*elem.pos(5)[2]; reg8=reg5-reg8; reg10=0.25*elem.pos(5)[1];
    reg11=reg13-reg12; reg5=reg2-reg5; reg4=reg4-reg6; reg0=reg0-reg6; reg1=reg6-reg1;
    reg2=0.25*elem.pos(3)[0]; reg9=reg9+reg13; reg8=reg8+reg7; reg6=0.25*elem.pos(6)[2]; reg3=reg7+reg3;
    reg12=reg13+reg12; reg4=reg4+reg10; reg13=0.25*elem.pos(6)[1]; reg7=reg5-reg7; reg0=reg0-reg10;
    reg1=reg10+reg1; reg11=reg2+reg11; reg9=reg9-reg2; reg5=0.25*elem.pos(4)[0]; reg12=reg2+reg12;
    reg11=reg11-reg5; reg2=0.25*vectors[0][indices[0]+1]; reg10=0.25*vectors[0][indices[1]+1]; reg0=reg13+reg0; reg3=reg6+reg3;
    T reg14=0.25*elem.pos(7)[2]; reg8=reg8+reg6; T reg15=1+(*f.m).poisson_ratio; T reg16=0.25*vectors[0][indices[1]+0]; T reg17=0.25*vectors[0][indices[0]+0];
    T reg18=0.25*vectors[0][indices[1]+2]; T reg19=0.25*vectors[0][indices[0]+2]; reg1=reg13+reg1; reg9=reg9-reg5; reg13=reg4+reg13;
    reg4=0.25*elem.pos(7)[1]; T reg20=0.25*elem.pos(5)[0]; reg7=reg6+reg7; reg6=reg10-reg2; T reg21=reg19+reg18;
    reg15=reg15/(*f.m).elastic_modulus; reg9=reg9+reg20; T reg22=0.25*elem.pos(6)[0]; reg8=reg8+reg14; reg0=reg4+reg0;
    reg3=reg3-reg14; reg7=reg14+reg7; reg12=reg5-reg12; reg13=reg13-reg4; reg10=reg2+reg10;
    reg1=reg4+reg1; reg2=0.25*vectors[0][indices[2]+1]; reg19=reg18-reg19; reg11=reg11-reg20; reg4=reg16-reg17;
    reg5=0.25*vectors[0][indices[2]+2]; reg16=reg17+reg16; reg14=0.25*vectors[0][indices[2]+0]; reg12=reg20+reg12; reg17=0.25*vectors[0][indices[3]+0];
    reg11=reg22+reg11; reg18=reg8*reg13; reg19=reg5+reg19; reg20=pow(reg15,2); T reg23=reg3*reg1;
    reg6=reg2+reg6; T reg24=reg2-reg10; T reg25=0.25*vectors[0][indices[3]+1]; T reg26=reg5-reg21; T reg27=reg7*reg1;
    reg4=reg14+reg4; T reg28=reg14-reg16; T reg29=reg8*reg0; T reg30=0.25*elem.pos(7)[0]; reg9=reg9+reg22;
    T reg31=0.25*vectors[0][indices[3]+2]; reg24=reg24+reg25; reg15=reg15*reg20; T reg32=0.25*vectors[0][indices[4]+2]; reg5=reg21+reg5;
    reg21=reg13*reg7; reg14=reg16+reg14; reg28=reg17+reg28; reg16=reg3*reg0; reg23=reg18-reg23;
    reg19=reg19-reg31; reg4=reg4-reg17; reg11=reg30+reg11; reg27=reg29-reg27; reg18=1.0/(*f.m).elastic_modulus;
    reg29=0.25*vectors[0][indices[4]+1]; T reg33=0.25*vectors[0][indices[4]+0]; reg9=reg9-reg30; T reg34=(*f.m).poisson_ratio/(*f.m).elastic_modulus; reg10=reg2+reg10;
    reg26=reg31+reg26; reg12=reg22+reg12; reg6=reg6-reg25; reg2=0.25*vectors[0][indices[5]+0]; reg31=reg5+reg31;
    reg5=reg9*reg27; reg22=0.25*vectors[0][indices[5]+2]; reg4=reg4-reg33; reg16=reg21-reg16; reg26=reg26-reg32;
    reg28=reg28-reg33; reg17=reg14+reg17; reg10=reg25+reg10; reg14=reg34*reg15; reg6=reg6-reg29;
    reg19=reg19-reg32; reg12=reg30+reg12; reg21=reg11*reg23; reg15=reg18*reg15; reg25=0.25*vectors[0][indices[5]+1];
    reg24=reg24-reg29; reg28=reg28-reg2; reg30=0.25*vectors[0][indices[6]+2]; T reg35=reg12*reg16; reg4=reg2+reg4;
    reg31=reg32-reg31; reg32=0.25*vectors[0][indices[6]+0]; reg17=reg33-reg17; reg10=reg29-reg10; reg6=reg25+reg6;
    reg21=reg5-reg21; reg5=reg12*reg7; reg29=reg8*reg11; reg33=0.25*vectors[0][indices[6]+1]; reg24=reg24-reg25;
    T reg36=reg9*reg1; T reg37=reg12*reg13; T reg38=reg3*reg12; reg19=reg22+reg19; reg8=reg9*reg8;
    reg26=reg26-reg22; T reg39=reg18*reg15; T reg40=reg34*reg14; reg15=reg34*reg15; reg12=reg12*reg0;
    reg1=reg1*reg11; reg37=reg36-reg37; reg39=reg39-reg40; reg24=reg24+reg33; reg36=0.25*vectors[0][indices[7]+1];
    T reg41=reg18*reg20; reg5=reg29-reg5; reg38=reg8-reg38; reg15=reg40+reg15; reg13=reg13*reg11;
    reg6=reg33+reg6; reg7=reg9*reg7; reg22=reg31+reg22; reg2=reg17+reg2; reg10=reg25+reg10;
    reg0=reg9*reg0; reg14=reg18*reg14; reg28=reg32+reg28; reg4=reg32+reg4; reg11=reg3*reg11;
    reg3=0.25*vectors[0][indices[7]+0]; reg26=reg30+reg26; reg21=reg35+reg21; reg20=reg34*reg20; reg19=reg30+reg19;
    reg12=reg1-reg12; reg1=0.25*vectors[0][indices[7]+2]; reg32=reg2+reg32; reg11=reg7-reg11; reg37=reg37/reg21;
    reg2=reg18*reg41; reg7=reg18*reg39; reg28=reg3+reg28; reg12=reg12/reg21; reg8=reg34*reg15;
    reg9=reg20*reg34; reg14=reg40+reg14; reg10=reg33+reg10; reg27=reg27/reg21; reg26=reg1+reg26;
    reg23=reg23/reg21; reg41=reg34*reg41; reg4=reg4-reg3; reg5=reg5/reg21; reg19=reg19-reg1;
    reg6=reg6-reg36; reg13=reg0-reg13; reg24=reg24+reg36; reg38=reg38/reg21; reg30=reg22+reg30;
    reg2=reg2-reg9; reg0=reg23*reg26; reg17=reg5*reg4; reg22=reg38*reg28; reg13=reg13/reg21;
    reg3=reg32+reg3; reg25=reg38*reg26; reg38=reg38*reg24; reg41=reg9+reg41; reg29=reg27*reg4;
    reg31=reg24*reg37; reg32=reg6*reg12; reg8=reg7-reg8; reg1=reg30+reg1; reg20=reg20*reg18;
    reg7=reg5*reg6; reg11=reg11/reg21; reg30=reg34*reg14; reg33=reg28*reg37; reg4=reg4*reg12;
    reg10=reg36+reg10; reg28=reg23*reg28; reg21=reg16/reg21; reg5=reg5*reg19; reg23=reg24*reg23;
    reg6=reg6*reg27; reg27=reg27*reg19; reg28=reg29-reg28; reg30=reg8-reg30; reg8=reg10*reg13;
    reg2=reg18*reg2; reg16=reg3*reg13; reg31=reg32-reg31; reg18=reg21*reg1; reg0=reg27-reg0;
    reg5=reg25-reg5; reg7=reg38-reg7; reg33=reg4-reg33; reg4=reg11*reg1; reg24=reg11*reg10;
    reg19=reg12*reg19; reg10=reg10*reg21; reg23=reg6-reg23; reg11=reg11*reg3; reg41=reg34*reg41;
    reg17=reg22-reg17; reg6=reg9+reg20; reg3=reg21*reg3; reg26=reg37*reg26; reg39=reg39/reg30;
    reg1=reg13*reg1; reg26=reg19-reg26; reg15=reg15/reg30; reg33=reg16+reg33; reg10=reg23+reg10;
    reg11=reg17-reg11; reg18=reg0+reg18; reg31=reg8+reg31; reg4=reg5-reg4; reg6=reg34*reg6;
    reg14=reg14/reg30; reg24=reg7-reg24; elem.epsilon[0][1]=reg24; reg41=reg2-reg41; reg28=reg3+reg28;
    elem.epsilon[0][0]=reg28; reg6=reg41-reg6; reg0=reg39*reg28; reg4=reg31+reg4; reg18=reg33+reg18;
    reg26=reg1+reg26; elem.epsilon[0][2]=reg26; reg10=reg11+reg10; reg1=reg15*reg24; reg2=reg39*reg24;
    reg3=reg15*reg28; reg5=reg14*reg24; reg7=reg39*reg26; reg30=reg6/reg30; reg2=reg2+reg3;
    reg0=reg1+reg0; reg10=0.5*reg10; elem.epsilon[0][3]=reg10; reg4=0.5*reg4; elem.epsilon[0][5]=reg4;
    reg28=reg24+reg28; reg1=reg14*reg26; reg18=0.5*reg18; elem.epsilon[0][4]=reg18; reg5=reg3+reg5;
    elem.sigma[0][5]=reg4*reg30; elem.sigma[0][0]=reg0+reg1; elem.tr_epsilon=reg28+reg26; elem.sigma[0][1]=reg2+reg1; elem.sigma[0][4]=reg18*reg30;
    elem.sigma[0][2]=reg7+reg5; elem.sigma[0][3]=reg10*reg30;
      #undef PNODE
    }
    template<class TE,class TF, class TVEVE> static void after_solve_2(TE &elem,TF &f,TVEVE &vectors,const unsigned *indices) {
      #define PNODE(N) (*elem.node(N))
      #undef PNODE
    }
    template<class TE,class TF, class TVEVE> static void after_solve_3(TE &elem,TF &f,TVEVE &vectors,const unsigned *indices) {
      #define PNODE(N) (*elem.node(N))
      #undef PNODE
    }
    template<class TE,class TF, class TVEVE> static void after_solve_4(TE &elem,TF &f,TVEVE &vectors,const unsigned *indices) {
      #define PNODE(N) (*elem.node(N))
      #undef PNODE
    }
    template<class TE,class TF, class TVEVE> static void after_solve_5(TE &elem,TF &f,TVEVE &vectors,const unsigned *indices) {
      #define PNODE(N) (*elem.node(N))
      #undef PNODE
    }
    template<class TE,class TF, class TVEVE> static void after_solve_6(TE &elem,TF &f,TVEVE &vectors,const unsigned *indices) {
      #define PNODE(N) (*elem.node(N))
      #undef PNODE
    }
    template<class TE,class TF, class TVEVE> static void after_solve_7(TE &elem,TF &f,TVEVE &vectors,const unsigned *indices) {
      #define PNODE(N) (*elem.node(N))
      #undef PNODE
    }
    template<class TE,class TF, class TVEVE> static void after_solve_8(TE &elem,TF &f,TVEVE &vectors,const unsigned *indices) {
      #define PNODE(N) (*elem.node(N))
      #undef PNODE
    }
    template<class TE,class TF, class TVEVE> static void after_solve_9(TE &elem,TF &f,TVEVE &vectors,const unsigned *indices) {
      #define PNODE(N) (*elem.node(N))
      #undef PNODE
    }
    template<class TE,class TF, class TVEVE> static void after_solve_10(TE &elem,TF &f,TVEVE &vectors,const unsigned *indices) {
      #define PNODE(N) (*elem.node(N))
      #undef PNODE
    }
    template<class TE,class TF, class TVEVE> static void after_solve_11(TE &elem,TF &f,TVEVE &vectors,const unsigned *indices) {
      #define PNODE(N) (*elem.node(N))
      #undef PNODE
    }
    template<class TE,class TF, class TVEVE> static void after_solve_12(TE &elem,TF &f,TVEVE &vectors,const unsigned *indices) {
      #define PNODE(N) (*elem.node(N))
      #undef PNODE
    }
    template<class TE,class TF, class TVEVE> static void after_solve_13(TE &elem,TF &f,TVEVE &vectors,const unsigned *indices) {
      #define PNODE(N) (*elem.node(N))
      #undef PNODE
    }
    template<class TE,class TF, class TVEVE> static void after_solve_14(TE &elem,TF &f,TVEVE &vectors,const unsigned *indices) {
      #define PNODE(N) (*elem.node(N))
      #undef PNODE
    }
    template<class TE,class TF, class TVEVE> static void after_solve_15(TE &elem,TF &f,TVEVE &vectors,const unsigned *indices) {
      #define PNODE(N) (*elem.node(N))
      #undef PNODE
    }
  
  static const unsigned nb_elementary_unknowns = 0;
  template<class TE,class TTs,class Tvec>
  inline static void set_elementary_unknowns(TE &elem,const TTs &f,const Tvec &vecs,unsigned indice) {
  }
  template<class TE,class TTs,class Tvec>
  inline static void set_elementary_unknowns(TE &elem,const TTs &f,const Tvec &vecs,unsigned indice,T partial_ts) {
  }
  template<class TE,class TTs,class Tvec>
  inline static void get_elementary_initial_conditions(const TE &elem,const TTs &f,Tvec &vecs,unsigned indice) {
  }
  template<class TE,class TTs,class Tvec>
  inline static T max_elementary_error(const TE &elem,const TTs &f,const Tvec &vecs,int indice) {
    return 0;
  }
  template<class TE,class TTs,class Tvecs,class Tvec>
  inline static void set_old_vec_elementary(const TE &elem,const TTs &f,const Tvecs &vecs,Tvec &old_vec,int indice) {
  }
  
  static const unsigned nb_skin_elementary_unknowns = 0;
  template<class TE,class TTs,class Tvec>
  inline static void set_skin_elementary_unknowns(TE &elem,const TTs &f,const Tvec &vecs,unsigned indice) {
  }
  template<class TE,class TTs,class Tvec>
  inline static void set_skin_elementary_unknowns(TE &elem,const TTs &f,const Tvec &vecs,unsigned indice,T partial_ts) {
  }
  template<class TE,class TTs,class Tvec>
  inline static void get_skin_elementary_initial_conditions(const TE &elem,const TTs &f,Tvec &vecs,unsigned indice) {
  }
  template<class TE,class TTs,class Tvec>
  inline static T max_skin_elementary_error(const TE &elem,const TTs &f,const Tvec &vecs,int indice) {
    return 0;
  }
  template<class TE,class TTs,class Tvecs,class Tvec>
  inline static void set_old_vec_skin_elementary(const TE &elem,const TTs &f,const Tvecs &vecs,Tvec &old_vec,int indice) {
  }
};

// 
template<class T,class TM,bool wont_add_nz,class TMA,class TVE,class TVEVE,class T_pos,class ND,class ED,unsigned nim>
void add_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<false> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<true> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const unsigned *indices){ 
  #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class T,class TM,bool wont_add_nz,class TMA,class TVE,class TVEVE,class T_pos,class ND,class ED,unsigned nim>
void add_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<true> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<true> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const unsigned *indices){ 
  #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class T,class TM,bool wont_add_nz,class TMA,class TVE,class TVEVE,class T_pos,class ND,class ED,unsigned nim>
void add_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<false> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<false> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const unsigned *indices){ 
  #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class T,class TM,bool wont_add_nz,class TMA,class TVE,class TVEVE,class T_pos,class ND,class ED,unsigned nim>
void add_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<true> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<false> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const unsigned *indices){ 
  #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class T,class TM,bool wont_add_nz,class TMA,class TVE,class TVEVE,class T_pos,class ND,class ED,unsigned nim,unsigned symmetric_version>
void add_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<symmetric_version> &matrix_is_sym,
      const Number<false> &assemble_mat,
      const Number<true> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const unsigned *indices){ 
  #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class T,class TM,bool wont_add_nz,class TMA,class TVE,class TVEVE,class T_pos,class ND,class ED,unsigned nim,unsigned symmetric_version>
void add_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<symmetric_version> &matrix_is_sym,
      const Number<false> &assemble_mat,
      const Number<false> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const unsigned *indices){ 
  #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class T,class TM,bool wont_add_nz,class TVE,class TVEVE,class T_pos,class ND,class ED,unsigned nim>
void add_elem_residual(
      const Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TVE &sollicitation,
      TVEVE &vectors,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const unsigned *indices ) { 
  #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
#ifndef ADD_NODAL_MATRIX_elasticity_dep_static_false_true_true
#define ADD_NODAL_MATRIX_elasticity_dep_static_false_true_true
template<class TM,class T,bool wont_add_nz,class TMA,class TVE,class TVEVE >
void add_nodal_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<false> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<true> &assemble_vec,
      const typename TM::TNode &node,
      const unsigned *indices){ 
  #define PNODE(N) node
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
#endif
// 
#ifndef ADD_NODAL_MATRIX_elasticity_dep_static_true_true_true
#define ADD_NODAL_MATRIX_elasticity_dep_static_true_true_true
template<class TM,class T,bool wont_add_nz,class TMA,class TVE,class TVEVE >
void add_nodal_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<true> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<true> &assemble_vec,
      const typename TM::TNode &node,
      const unsigned *indices){ 
  #define PNODE(N) node
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
#endif
// 
#ifndef ADD_NODAL_MATRIX_elasticity_dep_static_false_true_false
#define ADD_NODAL_MATRIX_elasticity_dep_static_false_true_false
template<class TM,class T,bool wont_add_nz,class TMA,class TVE,class TVEVE >
void add_nodal_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<false> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<false> &assemble_vec,
      const typename TM::TNode &node,
      const unsigned *indices){ 
  #define PNODE(N) node
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
#endif
// 
#ifndef ADD_NODAL_MATRIX_elasticity_dep_static_true_true_false
#define ADD_NODAL_MATRIX_elasticity_dep_static_true_true_false
template<class TM,class T,bool wont_add_nz,class TMA,class TVE,class TVEVE >
void add_nodal_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<true> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<false> &assemble_vec,
      const typename TM::TNode &node,
      const unsigned *indices){ 
  #define PNODE(N) node
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
#endif
// 
#ifndef ADD_NODAL_MATRIX_elasticity_dep_static_symmetric_version_false_true
#define ADD_NODAL_MATRIX_elasticity_dep_static_symmetric_version_false_true
template<class TM,class T,bool wont_add_nz,class TMA,class TVE,class TVEVE ,unsigned symmetric_version>
void add_nodal_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<symmetric_version> &matrix_is_sym,
      const Number<false> &assemble_mat,
      const Number<true> &assemble_vec,
      const typename TM::TNode &node,
      const unsigned *indices){ 
  #define PNODE(N) node
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
#endif
// 
#ifndef ADD_NODAL_MATRIX_elasticity_dep_static_symmetric_version_false_false
#define ADD_NODAL_MATRIX_elasticity_dep_static_symmetric_version_false_false
template<class TM,class T,bool wont_add_nz,class TMA,class TVE,class TVEVE ,unsigned symmetric_version>
void add_nodal_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<symmetric_version> &matrix_is_sym,
      const Number<false> &assemble_mat,
      const Number<false> &assemble_vec,
      const typename TM::TNode &node,
      const unsigned *indices){ 
  #define PNODE(N) node
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
#endif
// 
#ifndef ADD_NODAL_RESIDUAL_elasticity_dep_static
#define ADD_NODAL_RESIDUAL_elasticity_dep_static
template<class TM,class T,bool wont_add_nz,class TVE,class TVEVE>
void add_nodal_residual(
      const Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TVE &sollicitation,
      TVEVE &vectors,
      const typename TM::TNode &node,
      const unsigned *indices ) { 
  #define PNODE(N) node
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
#endif
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<false> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<true> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<0> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<true> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<true> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<0> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<false> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<false> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<0> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<true> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<false> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<0> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2,unsigned symmetric_version>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<symmetric_version> &matrix_is_sym,
      const Number<false> &assemble_mat,
      const Number<true> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<0> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2,unsigned symmetric_version>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<symmetric_version> &matrix_is_sym,
      const Number<false> &assemble_mat,
      const Number<false> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<0> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_residual(
      const Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TVE &sollicitation,
      TVEVE &vectors,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<0> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<false> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<true> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<1> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<true> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<true> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<1> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<false> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<false> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<1> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<true> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<false> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<1> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2,unsigned symmetric_version>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<symmetric_version> &matrix_is_sym,
      const Number<false> &assemble_mat,
      const Number<true> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<1> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2,unsigned symmetric_version>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<symmetric_version> &matrix_is_sym,
      const Number<false> &assemble_mat,
      const Number<false> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<1> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_residual(
      const Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TVE &sollicitation,
      TVEVE &vectors,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<1> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<false> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<true> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<2> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<true> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<true> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<2> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<false> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<false> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<2> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<true> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<false> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<2> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2,unsigned symmetric_version>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<symmetric_version> &matrix_is_sym,
      const Number<false> &assemble_mat,
      const Number<true> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<2> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2,unsigned symmetric_version>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<symmetric_version> &matrix_is_sym,
      const Number<false> &assemble_mat,
      const Number<false> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<2> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_residual(
      const Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TVE &sollicitation,
      TVEVE &vectors,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<2> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<false> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<true> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<3> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<true> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<true> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<3> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<false> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<false> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<3> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<true> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<false> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<3> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2,unsigned symmetric_version>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<symmetric_version> &matrix_is_sym,
      const Number<false> &assemble_mat,
      const Number<true> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<3> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2,unsigned symmetric_version>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<symmetric_version> &matrix_is_sym,
      const Number<false> &assemble_mat,
      const Number<false> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<3> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_residual(
      const Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TVE &sollicitation,
      TVEVE &vectors,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<3> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<false> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<true> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<4> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<true> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<true> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<4> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<false> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<false> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<4> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<true> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<false> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<4> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2,unsigned symmetric_version>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<symmetric_version> &matrix_is_sym,
      const Number<false> &assemble_mat,
      const Number<true> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<4> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2,unsigned symmetric_version>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<symmetric_version> &matrix_is_sym,
      const Number<false> &assemble_mat,
      const Number<false> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<4> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_residual(
      const Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TVE &sollicitation,
      TVEVE &vectors,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<4> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<false> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<true> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<5> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<true> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<true> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<5> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<false> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<false> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<5> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<true> &matrix_is_sym,
      const Number<true> &assemble_mat,
      const Number<false> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<5> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2,unsigned symmetric_version>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<symmetric_version> &matrix_is_sym,
      const Number<false> &assemble_mat,
      const Number<true> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<5> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TMA, class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2,unsigned symmetric_version>
void add_skin_elem_matrix(
      Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TMA &matrix,
      TVE &sollicitation,
      TVEVE &vectors,
      const Number<symmetric_version> &matrix_is_sym,
      const Number<false> &assemble_mat,
      const Number<false> &assemble_vec,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<5> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}
// 
template<class TM,class T,bool wont_add_nz,class TVE,class TVEVE, class T_pos, class ND,class ED, unsigned nim,class ED2,unsigned nim2>
void add_skin_elem_residual(
      const Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f,
      TVE &sollicitation,
      TVEVE &vectors,
      const Element<Hexa,DefaultBehavior,Node<3,T_pos,ND>,ED,nim> &elem,
      const Element<Quad,DefaultBehavior,Node<3,T_pos,ND>,ED2,nim2> &skin_elem,
      const Number<5> &num_child,
      const unsigned *indices){
   #define PNODE(N) (*elem.node(N))
    pthread_mutex_lock( &( f.mutex_assemble_matrix ) );
    pthread_mutex_unlock( &( f.mutex_assemble_matrix ) );
  #undef PNODE
}

#ifndef elasticity_dep_static_read_material_to_mesh
#define elasticity_dep_static_read_material_to_mesh
template<class TM, class T, bool wont_add_nz>
void read_material_to_mesh_(const XmlNode &n, Formulation<TM,elasticity_dep_static,DefaultBehavior,T,wont_add_nz> &f){ 
    if(n.has_attribute("elastic_modulus"))  
        n.get_attribute("elastic_modulus", f.m->elastic_modulus ); 
    else  
        std::cerr << "Warning using default value of elastic_modulus : " << f.m->elastic_modulus << std::endl; 

    if(n.has_attribute("density"))  
        n.get_attribute("density", f.m->density ); 
    else  
        std::cerr << "Warning using default value of density : " << f.m->density << std::endl; 

    if(n.has_attribute("poisson_ratio"))  
        n.get_attribute("poisson_ratio", f.m->poisson_ratio ); 
    else  
        std::cerr << "Warning using default value of poisson_ratio : " << f.m->poisson_ratio << std::endl; 

    if(n.has_attribute("f_vol"))  
        n.get_attribute("f_vol", f.m->f_vol ); 
    else  
        std::cerr << "Warning using default value of f_vol : " << f.m->f_vol << std::endl; 

  };
#endif // elasticity_dep_static_read_material_to_mesh
} // namespace LMT



#endif // PROBLEM_pb_elasticity_dep_static
