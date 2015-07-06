#ifndef LMT___LMT_INCLUDE_TVRC_MESHCARACTVRC_MESHCARAC_PY
#define LMT___LMT_INCLUDE_TVRC_MESHCARACTVRC_MESHCARAC_PY

#include "mesh/nodalelement.h"
#include "mesh/triangle.h"
#include "mesh/bar.h"
#include "mesh/tetra.h"
namespace LMT {

template<unsigned nb_dim,class T> class Meshcaractvrc;


#ifndef IFNDEF_rot_norm_DM
#define IFNDEF_rot_norm_DM
    struct rot_norm_DM {};
#endif // IFNDEF_rot_norm_DM

#ifndef IFNDEF_rot_real_DM
#define IFNDEF_rot_real_DM
    struct rot_real_DM {};
#endif // IFNDEF_rot_real_DM

#ifndef IFNDEF_normal_DM
#define IFNDEF_normal_DM
    struct normal_DM {};
#endif // IFNDEF_normal_DM

#ifndef IFNDEF_amp_norm_DM
#define IFNDEF_amp_norm_DM
    struct amp_norm_DM {};
#endif // IFNDEF_amp_norm_DM

#ifndef IFNDEF_rot_imag_DM
#define IFNDEF_rot_imag_DM
    struct rot_imag_DM {};
#endif // IFNDEF_rot_imag_DM

#ifndef IFNDEF_pos_DM
#define IFNDEF_pos_DM
    struct pos_DM {};
#endif // IFNDEF_pos_DM

#ifndef IFNDEF_curvature_DM
#define IFNDEF_curvature_DM
    struct curvature_DM {};
#endif // IFNDEF_curvature_DM

#ifndef IFNDEF_amp_real_DM
#define IFNDEF_amp_real_DM
    struct amp_real_DM {};
#endif // IFNDEF_amp_real_DM

#ifndef IFNDEF_local_coords_DM
#define IFNDEF_local_coords_DM
    struct local_coords_DM {};
#endif // IFNDEF_local_coords_DM

#ifndef IFNDEF_amp_imag_DM
#define IFNDEF_amp_imag_DM
    struct amp_imag_DM {};
#endif // IFNDEF_amp_imag_DM

#ifndef IFNDEF_Y_DM
#define IFNDEF_Y_DM
    struct Y_DM {};
#endif // IFNDEF_Y_DM

#ifndef IFNDEF_X_DM
#define IFNDEF_X_DM
    struct X_DM {};
#endif // IFNDEF_X_DM

#ifndef IFNDEF_is_separation_DM
#define IFNDEF_is_separation_DM
    struct is_separation_DM {};
#endif // IFNDEF_is_separation_DM

#ifndef IFNDEF_Z_DM
#define IFNDEF_Z_DM
    struct Z_DM {};
#endif // IFNDEF_Z_DM

#ifndef IFNDEF_num_structure_DM
#define IFNDEF_num_structure_DM
    struct num_structure_DM {};
#endif // IFNDEF_num_structure_DM

#ifndef IFNDEF_has_local_coords_DM
#define IFNDEF_has_local_coords_DM
    struct has_local_coords_DM {};
#endif // IFNDEF_has_local_coords_DM

template<class T>
class Meshcaractvrc<1,T> { 
public:
    static const unsigned dim = 1;
    typedef T Tpos;
    typedef Vec<T,1> Pvec;
    struct NodalStaticData {
        NodalStaticData():normal(0.0),local_coords(0.0),X(0.0),Y(0.0),Z(0.0),curvature(1,1,0.0),has_local_coords(0),is_separation(0),amp_real(0.0),amp_imag(0.0),amp_norm(0.0),rot_real(0.0),rot_imag(0.0),rot_norm(0.0) {}
        typedef Vec<T,1> T0;
        CARACDMEXTNAME(0,T0,normal,"m");
        typedef Vec<T,1> T1;
        CARACDMEXTNAME(1,T1,local_coords,"m");
        typedef Vec<T,1> T2;
        CARACDMEXTNAME(2,T2,X,"1");
        typedef Vec<T,1> T3;
        CARACDMEXTNAME(3,T3,Y,"1");
        typedef Vec<T,1> T4;
        CARACDMEXTNAME(4,T4,Z,"1");
        typedef Mat<T,Sym<1> > T5;
        CARACDMEXTNAME(5,T5,curvature,"1/m");
        CARACDMEXTNAME(6,unsigned,has_local_coords,"1");
        CARACDMEXTNAME(7,unsigned,is_separation,"1");
        typedef Vec<T,1> T8;
        CARACDMEXTNAME(8,T8,amp_real,"m");
        typedef Vec<T,1> T9;
        CARACDMEXTNAME(9,T9,amp_imag,"m");
        typedef Vec<T,1> T10;
        CARACDMEXTNAME(10,T10,amp_norm,"m");
        typedef Vec<T,1> T11;
        CARACDMEXTNAME(11,T11,rot_real,"m");
        typedef Vec<T,1> T12;
        CARACDMEXTNAME(12,T12,rot_imag,"m");
        typedef Vec<T,1> T13;
        CARACDMEXTNAME(13,T13,rot_norm,"m");
        CARACDMEXTNAME(14,Pvec,pos,"m");
        static const unsigned nb_params = 15;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<0, 0>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<1, 1>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<2, 2>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<3, 3>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<4, 4>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<5, 5>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<6, 6>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        template<class __G__> __G__ dm_data_get_field( const std::string field_name, StructForType<__G__>, bool disp = true ) const { assert( 0 /*TODO*/ ); return __G__( 0.0 );  }
    };
    struct GlobalStaticData {
        VOIDDMSET;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<0, 0>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<1, 1>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<2, 2>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<3, 3>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<4, 4>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<5, 5>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<6, 6>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        template<class __G__> __G__ dm_data_get_field( const std::string field_name, StructForType<__G__>, bool disp = true ) const { assert( 0 /*TODO*/ ); return __G__( 0.0 );  }
    };
    template<unsigned nvi_to_subs,unsigned skin,unsigned num_sub_element,unsigned inner=0>
    struct ElementChoice {
        typedef void NE;
        typedef DefaultBehavior BE;
        typedef VoidDMSet TData;
    };
    struct TCDM0 {
        TCDM0():num_structure(0) {}
        CARACDMEXTNAME(0,unsigned,num_structure,"");
        static const unsigned nb_params = 1;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<0, 0>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<1, 1>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<2, 2>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<3, 3>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<4, 4>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<5, 5>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<6, 6>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        template<class __G__> __G__ dm_data_get_field( const std::string field_name, StructForType<__G__>, bool disp = true ) const { assert( 0 /*TODO*/ ); return __G__( 0.0 );  }
    };
    template<unsigned inner> struct ElementChoice<0,0,0,inner> {
        typedef NodalElement NE;
        typedef DefaultBehavior BE;
        typedef TCDM0 TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<0,0,1,inner> {
        typedef Bar NE;
        typedef DefaultBehavior BE;
        typedef TCDM0 TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<1,0,0,inner> {
        typedef NodalElement NE;
        typedef DefaultBehavior BE;
        typedef TCDM0 TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<1,1,0,inner> {
        typedef NodalElement NE;
        typedef DefaultBehavior BE;
        typedef TCDM0 TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
};

template<class T>
class Meshcaractvrc<2,T> { 
public:
    static const unsigned dim = 2;
    typedef T Tpos;
    typedef Vec<T,2> Pvec;
    struct NodalStaticData {
        NodalStaticData():normal(0.0),local_coords(0.0),X(0.0),Y(0.0),Z(0.0),curvature(2,2,0.0),has_local_coords(0),is_separation(0),amp_real(0.0),amp_imag(0.0),amp_norm(0.0),rot_real(0.0),rot_imag(0.0),rot_norm(0.0) {}
        typedef Vec<T,2> T0;
        CARACDMEXTNAME(0,T0,normal,"m");
        typedef Vec<T,2> T1;
        CARACDMEXTNAME(1,T1,local_coords,"m");
        typedef Vec<T,2> T2;
        CARACDMEXTNAME(2,T2,X,"1");
        typedef Vec<T,2> T3;
        CARACDMEXTNAME(3,T3,Y,"1");
        typedef Vec<T,2> T4;
        CARACDMEXTNAME(4,T4,Z,"1");
        typedef Mat<T,Sym<2> > T5;
        CARACDMEXTNAME(5,T5,curvature,"1/m");
        CARACDMEXTNAME(6,unsigned,has_local_coords,"1");
        CARACDMEXTNAME(7,unsigned,is_separation,"1");
        typedef Vec<T,2> T8;
        CARACDMEXTNAME(8,T8,amp_real,"m");
        typedef Vec<T,2> T9;
        CARACDMEXTNAME(9,T9,amp_imag,"m");
        typedef Vec<T,2> T10;
        CARACDMEXTNAME(10,T10,amp_norm,"m");
        typedef Vec<T,1> T11;
        CARACDMEXTNAME(11,T11,rot_real,"m");
        typedef Vec<T,1> T12;
        CARACDMEXTNAME(12,T12,rot_imag,"m");
        typedef Vec<T,1> T13;
        CARACDMEXTNAME(13,T13,rot_norm,"m");
        CARACDMEXTNAME(14,Pvec,pos,"m");
        static const unsigned nb_params = 15;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<0, 0>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<1, 1>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<2, 2>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<3, 3>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<4, 4>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<5, 5>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<6, 6>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        template<class __G__> __G__ dm_data_get_field( const std::string field_name, StructForType<__G__>, bool disp = true ) const { assert( 0 /*TODO*/ ); return __G__( 0.0 );  }
    };
    struct GlobalStaticData {
        VOIDDMSET;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<0, 0>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<1, 1>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<2, 2>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<3, 3>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<4, 4>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<5, 5>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<6, 6>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        template<class __G__> __G__ dm_data_get_field( const std::string field_name, StructForType<__G__>, bool disp = true ) const { assert( 0 /*TODO*/ ); return __G__( 0.0 );  }
    };
    template<unsigned nvi_to_subs,unsigned skin,unsigned num_sub_element,unsigned inner=0>
    struct ElementChoice {
        typedef void NE;
        typedef DefaultBehavior BE;
        typedef VoidDMSet TData;
    };
    struct TCDM0 {
        TCDM0():num_structure(0) {}
        CARACDMEXTNAME(0,unsigned,num_structure,"");
        static const unsigned nb_params = 1;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<0, 0>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<1, 1>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<2, 2>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<3, 3>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<4, 4>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<5, 5>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<6, 6>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        template<class __G__> __G__ dm_data_get_field( const std::string field_name, StructForType<__G__>, bool disp = true ) const { assert( 0 /*TODO*/ ); return __G__( 0.0 );  }
    };
    template<unsigned inner> struct ElementChoice<0,0,0,inner> {
        typedef NodalElement NE;
        typedef DefaultBehavior BE;
        typedef TCDM0 TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<0,0,1,inner> {
        typedef Bar NE;
        typedef DefaultBehavior BE;
        typedef TCDM0 TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<0,0,2,inner> {
        typedef Triangle NE;
        typedef DefaultBehavior BE;
        typedef TCDM0 TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<1,0,0,inner> {
        typedef NodalElement NE;
        typedef DefaultBehavior BE;
        typedef TCDM0 TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<1,0,1,inner> {
        typedef Bar NE;
        typedef DefaultBehavior BE;
        typedef TCDM0 TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<2,0,0,inner> {
        typedef NodalElement NE;
        typedef DefaultBehavior BE;
        typedef TCDM0 TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<1,1,0,inner> {
        typedef NodalElement NE;
        typedef DefaultBehavior BE;
        typedef TCDM0 TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<1,1,1,inner> {
        typedef Bar NE;
        typedef DefaultBehavior BE;
        typedef TCDM0 TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<2,1,0,inner> {
        typedef NodalElement NE;
        typedef DefaultBehavior BE;
        typedef TCDM0 TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
};

template<class T>
class Meshcaractvrc<3,T> { 
public:
    static const unsigned dim = 3;
    typedef T Tpos;
    typedef Vec<T,3> Pvec;
    struct NodalStaticData {
        NodalStaticData():normal(0.0),local_coords(0.0),X(0.0),Y(0.0),Z(0.0),curvature(3,3,0.0),has_local_coords(0),is_separation(0),amp_real(0.0),amp_imag(0.0),amp_norm(0.0),rot_real(0.0),rot_imag(0.0),rot_norm(0.0) {}
        typedef Vec<T,3> T0;
        CARACDMEXTNAME(0,T0,normal,"m");
        typedef Vec<T,3> T1;
        CARACDMEXTNAME(1,T1,local_coords,"m");
        typedef Vec<T,3> T2;
        CARACDMEXTNAME(2,T2,X,"1");
        typedef Vec<T,3> T3;
        CARACDMEXTNAME(3,T3,Y,"1");
        typedef Vec<T,3> T4;
        CARACDMEXTNAME(4,T4,Z,"1");
        typedef Mat<T,Sym<3> > T5;
        CARACDMEXTNAME(5,T5,curvature,"1/m");
        CARACDMEXTNAME(6,unsigned,has_local_coords,"1");
        CARACDMEXTNAME(7,unsigned,is_separation,"1");
        typedef Vec<T,3> T8;
        CARACDMEXTNAME(8,T8,amp_real,"m");
        typedef Vec<T,3> T9;
        CARACDMEXTNAME(9,T9,amp_imag,"m");
        typedef Vec<T,3> T10;
        CARACDMEXTNAME(10,T10,amp_norm,"m");
        typedef Vec<T,3> T11;
        CARACDMEXTNAME(11,T11,rot_real,"m");
        typedef Vec<T,3> T12;
        CARACDMEXTNAME(12,T12,rot_imag,"m");
        typedef Vec<T,3> T13;
        CARACDMEXTNAME(13,T13,rot_norm,"m");
        CARACDMEXTNAME(14,Pvec,pos,"m");
        static const unsigned nb_params = 15;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<0, 0>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<1, 1>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<2, 2>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<3, 3>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<4, 4>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<5, 5>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<6, 6>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        template<class __G__> __G__ dm_data_get_field( const std::string field_name, StructForType<__G__>, bool disp = true ) const { assert( 0 /*TODO*/ ); return __G__( 0.0 );  }
    };
    struct GlobalStaticData {
        VOIDDMSET;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<0, 0>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<1, 1>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<2, 2>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<3, 3>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<4, 4>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<5, 5>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<6, 6>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        template<class __G__> __G__ dm_data_get_field( const std::string field_name, StructForType<__G__>, bool disp = true ) const { assert( 0 /*TODO*/ ); return __G__( 0.0 );  }
    };
    template<unsigned nvi_to_subs,unsigned skin,unsigned num_sub_element,unsigned inner=0>
    struct ElementChoice {
        typedef void NE;
        typedef DefaultBehavior BE;
        typedef VoidDMSet TData;
    };
    struct TCDM0 {
        TCDM0():num_structure(0) {}
        CARACDMEXTNAME(0,unsigned,num_structure,"");
        static const unsigned nb_params = 1;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<0, 0>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<1, 1>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<2, 2>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<3, 3>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<4, 4>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<5, 5>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<6, 6>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }
        template<class __G__> __G__ dm_data_get_field( const std::string field_name, StructForType<__G__>, bool disp = true ) const { assert( 0 /*TODO*/ ); return __G__( 0.0 );  }
    };
    template<unsigned inner> struct ElementChoice<0,0,0,inner> {
        typedef NodalElement NE;
        typedef DefaultBehavior BE;
        typedef TCDM0 TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<0,0,1,inner> {
        typedef Bar NE;
        typedef DefaultBehavior BE;
        typedef TCDM0 TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<0,0,2,inner> {
        typedef Triangle NE;
        typedef DefaultBehavior BE;
        typedef TCDM0 TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<0,0,3,inner> {
        typedef Tetra NE;
        typedef DefaultBehavior BE;
        typedef TCDM0 TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<1,0,0,inner> {
        typedef NodalElement NE;
        typedef DefaultBehavior BE;
        typedef TCDM0 TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<1,0,1,inner> {
        typedef Bar NE;
        typedef DefaultBehavior BE;
        typedef TCDM0 TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<1,0,2,inner> {
        typedef Triangle NE;
        typedef DefaultBehavior BE;
        typedef TCDM0 TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<2,0,0,inner> {
        typedef NodalElement NE;
        typedef DefaultBehavior BE;
        typedef TCDM0 TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<2,0,1,inner> {
        typedef Bar NE;
        typedef DefaultBehavior BE;
        typedef TCDM0 TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<3,0,0,inner> {
        typedef NodalElement NE;
        typedef DefaultBehavior BE;
        typedef TCDM0 TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<1,1,0,inner> {
        typedef NodalElement NE;
        typedef DefaultBehavior BE;
        typedef TCDM0 TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<1,1,1,inner> {
        typedef Bar NE;
        typedef DefaultBehavior BE;
        typedef TCDM0 TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<1,1,2,inner> {
        typedef Triangle NE;
        typedef DefaultBehavior BE;
        typedef TCDM0 TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<2,1,0,inner> {
        typedef NodalElement NE;
        typedef DefaultBehavior BE;
        typedef TCDM0 TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<2,1,1,inner> {
        typedef Bar NE;
        typedef DefaultBehavior BE;
        typedef TCDM0 TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<3,1,0,inner> {
        typedef NodalElement NE;
        typedef DefaultBehavior BE;
        typedef TCDM0 TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
};



};

#endif // LMT___LMT_INCLUDE_TVRC_MESHCARACTVRC_MESHCARAC_PY
