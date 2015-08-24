//
// C++ Interface: tvrcmodebeam
//
// Description: 
//
//
// Author: Hugo LECLERC <leclerc@lmt.ens-cachan.fr>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMT_tvrcmodebeam_HEADER
#define LMT_tvrcmodebeam_HEADER

#include "../util/materialpoint.h"

namespace LMT {

/** omly for beams
    \todo right value in k
 */
template<unsigned dim,class T>
class TvrcModeBeamFlexion : public TvrcMode<dim,1,T> {
public:
    typedef TvrcMode<dim,1,T> TP;
    typedef MaterialPoint<typename TP::TC,dim> TD;
    /// return min wave length
    T get_min_wave_length() const { return fabs(2.0*M_PI/k.imag()); }
    ///
    template<class TMat,class GeomEnt> void set_param_for_w(double w,const TMat &mat,const GeomEnt &geoment) {
        k = typename TP::TC(
            0,
            10.0 * 2.0 * M_PI * w // TODO right values
        );
        this->w = w;
    }
    ///
    template<class HV> static void push_dist(HV &hv,const XmlNode &sn) {
        TvrcModeBeamFlexion tv;
        for(unsigned i=0;i<dim;++i) {
            tv.dir = 0.0;
            for(unsigned d=0;d<dim;++d) tv.dir[d] = (d==i);
            hv.push_back( tv );
        }
    }
    
    /// return displacement and rotation
    template<class TN> TD get_value(const TN &node) const {
        typename TP::Rvec R( T(dim==2) );
        if ( dim==3 ) for(unsigned j=0;j<dim;++j) R += node.der_coords[0] * dir[1] - node.der_coords[1] * dir[0];
        R /= length(R,1e-40);
        typename TP::Pvec D = 0.0;
        for(unsigned j=0;j<dim;++j) D += node.der_coords[j] * dir[j];
        
        typename TP::TC k2 = k;
        if ( dir[dim-1] ) k2 = 0.0; // rigid body
        return TD(
            exp( k2 * node.local_coords[0] ) * D,
            k2 * exp( k2 * node.local_coords[0] ) * R / (10.0*w)
        );
    }
    ///
    static std::string name() { return "beam_flexion"; }
    ///
    typename TP::TC k;
    Vec<T,dim> dir;
    T w;
};

/** omly for beams in 3D
    \todo right value in k
 */
template<unsigned dim,class T>
class TvrcModeBeamTorsion : public TvrcMode<dim,1,T> {
public:
    typedef TvrcMode<dim,1,T> TP;
    typedef MaterialPoint<typename TP::TC,dim> TD;
    /// return min wave length
    T get_min_wave_length() const { return fabs(2.0*M_PI/k.imag()); }
    ///
    template<class TMat,class GeomEnt> void set_param_for_w(double w,const TMat &mat,const GeomEnt &geoment) {
        k = typename TP::TC(
            0,
            2.0 * M_PI * w
        );
        this->w = w;
    }
    ///
    template<class HV> static void push_dist(HV &hv,const XmlNode &sn) {
         hv.push_back( TvrcModeBeamTorsion() );
    }
    
    /// return displacement and rotation
    template<class TN> TD get_value(const TN &node) const {
        return TD(
            typename TP::Pvec(0.0),
            k * exp( k * node.local_coords[0] ) * node.der_coords[dim-1] / (10.0*w) * 0.
        );
    }
    ///
    static std::string name() { return "beam_torsion"; }
    ///
    typename TP::TC k;
    T w;
};



};

#endif // LMT_tvrcmodebeam_HEADER
