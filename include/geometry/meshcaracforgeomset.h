#ifndef LMT___LMT_INCLUDE_GEOMETRY_MESHCARACFORGEOMSET_MESHCARAC_PY
#define LMT___LMT_INCLUDE_GEOMETRY_MESHCARACFORGEOMSET_MESHCARAC_PY

#include "mesh/nodalelement.h"
#include "mesh/triangle.h"
#include "mesh/bar.h"
#include "mesh/tetra.h"
namespace LMT {

template<unsigned nb_dim,unsigned nvi,class T> class Meshcaracforgeomset;


#ifndef IFNDEF_local_coords_DM
#define IFNDEF_local_coords_DM
    struct local_coords_DM {};
#endif // IFNDEF_local_coords_DM

#ifndef IFNDEF_der_coords_DM
#define IFNDEF_der_coords_DM
    struct der_coords_DM {};
#endif // IFNDEF_der_coords_DM

#ifndef IFNDEF_pos_DM
#define IFNDEF_pos_DM
    struct pos_DM {};
#endif // IFNDEF_pos_DM

#ifndef IFNDEF_curvature_DM
#define IFNDEF_curvature_DM
    struct curvature_DM {};
#endif // IFNDEF_curvature_DM

template<class T>
class Meshcaracforgeomset<1,0,T> { 
public:
    static const unsigned dim = 1;
    typedef T Tpos;
    typedef Vec<T,1> Pvec;
    struct NodalStaticData {
        NodalStaticData():local_coords(0.0),der_coords(0.0),curvature(1,1,0.0) {}
        typedef Vec<T,1> T0;
        CARACDMEXTNAME(0,T0,local_coords,"m");
        typedef Vec<Vec<T,1>,1> T1;
        CARACDMEXTNAME(1,T1,der_coords,"m");
        typedef Mat<T,Sym<1> > T2;
        CARACDMEXTNAME(2,T2,curvature,"1/m");
        CARACDMEXTNAME(3,Pvec,pos,"m");
        static const unsigned nb_params = 4;
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
    template<unsigned inner> struct ElementChoice<0,0,0,inner> {
        typedef NodalElement NE;
        typedef DefaultBehavior BE;
        typedef VoidDMSet TData;
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
class Meshcaracforgeomset<2,0,T> { 
public:
    static const unsigned dim = 2;
    typedef T Tpos;
    typedef Vec<T,2> Pvec;
    struct NodalStaticData {
        NodalStaticData():local_coords(0.0),der_coords(0.0),curvature(1,1,0.0) {}
        typedef Vec<T,1> T0;
        CARACDMEXTNAME(0,T0,local_coords,"m");
        typedef Vec<Vec<T,2>,2> T1;
        CARACDMEXTNAME(1,T1,der_coords,"m");
        typedef Mat<T,Sym<1> > T2;
        CARACDMEXTNAME(2,T2,curvature,"1/m");
        CARACDMEXTNAME(3,Pvec,pos,"m");
        static const unsigned nb_params = 4;
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
    template<unsigned inner> struct ElementChoice<0,0,0,inner> {
        typedef NodalElement NE;
        typedef DefaultBehavior BE;
        typedef VoidDMSet TData;
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
class Meshcaracforgeomset<3,0,T> { 
public:
    static const unsigned dim = 3;
    typedef T Tpos;
    typedef Vec<T,3> Pvec;
    struct NodalStaticData {
        NodalStaticData():local_coords(0.0),der_coords(0.0),curvature(1,1,0.0) {}
        typedef Vec<T,1> T0;
        CARACDMEXTNAME(0,T0,local_coords,"m");
        typedef Vec<Vec<T,3>,3> T1;
        CARACDMEXTNAME(1,T1,der_coords,"m");
        typedef Mat<T,Sym<1> > T2;
        CARACDMEXTNAME(2,T2,curvature,"1/m");
        CARACDMEXTNAME(3,Pvec,pos,"m");
        static const unsigned nb_params = 4;
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
    template<unsigned inner> struct ElementChoice<0,0,0,inner> {
        typedef NodalElement NE;
        typedef DefaultBehavior BE;
        typedef VoidDMSet TData;
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
class Meshcaracforgeomset<1,1,T> { 
public:
    static const unsigned dim = 1;
    typedef T Tpos;
    typedef Vec<T,1> Pvec;
    struct NodalStaticData {
        NodalStaticData():local_coords(0.0),der_coords(0.0),curvature(1,1,0.0) {}
        typedef Vec<T,1> T0;
        CARACDMEXTNAME(0,T0,local_coords,"m");
        typedef Vec<Vec<T,1>,1> T1;
        CARACDMEXTNAME(1,T1,der_coords,"m");
        typedef Mat<T,Sym<1> > T2;
        CARACDMEXTNAME(2,T2,curvature,"1/m");
        CARACDMEXTNAME(3,Pvec,pos,"m");
        static const unsigned nb_params = 4;
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
    template<unsigned inner> struct ElementChoice<0,0,0,inner> {
        typedef Bar NE;
        typedef DefaultBehavior BE;
        typedef VoidDMSet TData;
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
        typedef VoidDMSet TData;
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
        typedef VoidDMSet TData;
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
class Meshcaracforgeomset<2,1,T> { 
public:
    static const unsigned dim = 2;
    typedef T Tpos;
    typedef Vec<T,2> Pvec;
    struct NodalStaticData {
        NodalStaticData():local_coords(0.0),der_coords(0.0),curvature(1,1,0.0) {}
        typedef Vec<T,1> T0;
        CARACDMEXTNAME(0,T0,local_coords,"m");
        typedef Vec<Vec<T,2>,2> T1;
        CARACDMEXTNAME(1,T1,der_coords,"m");
        typedef Mat<T,Sym<1> > T2;
        CARACDMEXTNAME(2,T2,curvature,"1/m");
        CARACDMEXTNAME(3,Pvec,pos,"m");
        static const unsigned nb_params = 4;
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
    template<unsigned inner> struct ElementChoice<0,0,0,inner> {
        typedef Bar NE;
        typedef DefaultBehavior BE;
        typedef VoidDMSet TData;
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
        typedef VoidDMSet TData;
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
        typedef VoidDMSet TData;
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
class Meshcaracforgeomset<3,1,T> { 
public:
    static const unsigned dim = 3;
    typedef T Tpos;
    typedef Vec<T,3> Pvec;
    struct NodalStaticData {
        NodalStaticData():local_coords(0.0),der_coords(0.0),curvature(1,1,0.0) {}
        typedef Vec<T,1> T0;
        CARACDMEXTNAME(0,T0,local_coords,"m");
        typedef Vec<Vec<T,3>,3> T1;
        CARACDMEXTNAME(1,T1,der_coords,"m");
        typedef Mat<T,Sym<1> > T2;
        CARACDMEXTNAME(2,T2,curvature,"1/m");
        CARACDMEXTNAME(3,Pvec,pos,"m");
        static const unsigned nb_params = 4;
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
    template<unsigned inner> struct ElementChoice<0,0,0,inner> {
        typedef Bar NE;
        typedef DefaultBehavior BE;
        typedef VoidDMSet TData;
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
        typedef VoidDMSet TData;
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
        typedef VoidDMSet TData;
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
class Meshcaracforgeomset<2,2,T> { 
public:
    static const unsigned dim = 2;
    typedef T Tpos;
    typedef Vec<T,2> Pvec;
    struct NodalStaticData {
        NodalStaticData():local_coords(0.0),der_coords(0.0),curvature(2,2,0.0) {}
        typedef Vec<T,2> T0;
        CARACDMEXTNAME(0,T0,local_coords,"m");
        typedef Vec<Vec<T,2>,2> T1;
        CARACDMEXTNAME(1,T1,der_coords,"m");
        typedef Mat<T,Sym<2> > T2;
        CARACDMEXTNAME(2,T2,curvature,"1/m");
        CARACDMEXTNAME(3,Pvec,pos,"m");
        static const unsigned nb_params = 4;
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
    template<unsigned inner> struct ElementChoice<0,0,0,inner> {
        typedef Triangle NE;
        typedef DefaultBehavior BE;
        typedef VoidDMSet TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<1,0,0,inner> {
        typedef Bar NE;
        typedef DefaultBehavior BE;
        typedef VoidDMSet TData;
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
        typedef VoidDMSet TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<1,1,0,inner> {
        typedef Bar NE;
        typedef DefaultBehavior BE;
        typedef VoidDMSet TData;
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
        typedef VoidDMSet TData;
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
class Meshcaracforgeomset<3,2,T> { 
public:
    static const unsigned dim = 3;
    typedef T Tpos;
    typedef Vec<T,3> Pvec;
    struct NodalStaticData {
        NodalStaticData():local_coords(0.0),der_coords(0.0),curvature(2,2,0.0) {}
        typedef Vec<T,2> T0;
        CARACDMEXTNAME(0,T0,local_coords,"m");
        typedef Vec<Vec<T,3>,3> T1;
        CARACDMEXTNAME(1,T1,der_coords,"m");
        typedef Mat<T,Sym<2> > T2;
        CARACDMEXTNAME(2,T2,curvature,"1/m");
        CARACDMEXTNAME(3,Pvec,pos,"m");
        static const unsigned nb_params = 4;
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
    template<unsigned inner> struct ElementChoice<0,0,0,inner> {
        typedef Triangle NE;
        typedef DefaultBehavior BE;
        typedef VoidDMSet TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<1,0,0,inner> {
        typedef Bar NE;
        typedef DefaultBehavior BE;
        typedef VoidDMSet TData;
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
        typedef VoidDMSet TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<1,1,0,inner> {
        typedef Bar NE;
        typedef DefaultBehavior BE;
        typedef VoidDMSet TData;
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
        typedef VoidDMSet TData;
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
class Meshcaracforgeomset<3,3,T> { 
public:
    static const unsigned dim = 3;
    typedef T Tpos;
    typedef Vec<T,3> Pvec;
    struct NodalStaticData {
        NodalStaticData():local_coords(0.0),der_coords(0.0),curvature(3,3,0.0) {}
        typedef Vec<T,3> T0;
        CARACDMEXTNAME(0,T0,local_coords,"m");
        typedef Vec<Vec<T,3>,3> T1;
        CARACDMEXTNAME(1,T1,der_coords,"m");
        typedef Mat<T,Sym<3> > T2;
        CARACDMEXTNAME(2,T2,curvature,"1/m");
        CARACDMEXTNAME(3,Pvec,pos,"m");
        static const unsigned nb_params = 4;
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
    template<unsigned inner> struct ElementChoice<0,0,0,inner> {
        typedef Tetra NE;
        typedef DefaultBehavior BE;
        typedef VoidDMSet TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<1,0,0,inner> {
        typedef Triangle NE;
        typedef DefaultBehavior BE;
        typedef VoidDMSet TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<2,0,0,inner> {
        typedef Bar NE;
        typedef DefaultBehavior BE;
        typedef VoidDMSet TData;
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
        typedef VoidDMSet TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<1,1,0,inner> {
        typedef Triangle NE;
        typedef DefaultBehavior BE;
        typedef VoidDMSet TData;
        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }
        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }
    };
    template<unsigned inner> struct ElementChoice<2,1,0,inner> {
        typedef Bar NE;
        typedef DefaultBehavior BE;
        typedef VoidDMSet TData;
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
        typedef VoidDMSet TData;
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

#endif // LMT___LMT_INCLUDE_GEOMETRY_MESHCARACFORGEOMSET_MESHCARAC_PY
