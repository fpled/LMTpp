//
// C++ Interface: points_repartition_sphere
//
// Description: 
//
//
// Author: Hugo LECLERC <hugo.leclerc@lmt.ens-cachan.fr>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMT_points_repartition_sphere_HEADER
#define LMT_points_repartition_sphere_HEADER

#include "../containers/vec.h"

namespace LMT {

class SphereRepartition {
public:
    typedef Vec<double,3> Pvec;
    typedef std::pair<unsigned,unsigned> L;
    
    SphereRepartition() {}
    SphereRepartition(unsigned nb_points) {
        const T z = -1.0/3.0, y = sqrt(3.0)/2.0, x = sqrt(1-z*z);
        points.push_back(Pvec( 0,      0, 1));
        points.push_back(Pvec( x,      0, z));
        points.push_back(Pvec(-x/2.0,  y, z));
        points.push_back(Pvec(-x/2.0, -y, z));
    
        lines.push_back(L(0,1));
        lines.push_back(L(0,2));
        lines.push_back(L(0,3));
        lines.push_back(L(1,2));
        lines.push_back(L(1,3));
        lines.push_back(L(2,3));
    }
    void add_points() {
    
    }

    Vec<Pvec> points;
    Vec<L> lines;
};

/** get nb_points equally (or nearly) repartited on a unit sphere
*/
template<class T> points_repartition_sphere(Vec<Vec<T,3> > &res,unsigned nb_points) {
    
    res.clear();
    
}

};

#endif // LMT_points_repartition_sphere_HEADER
