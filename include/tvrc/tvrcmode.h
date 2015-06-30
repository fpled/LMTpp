//
// C++ Interface: %{MODULE}
//
// Description: 
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TVRCMODE_H
#define TVRCMODE_H

#include "../containers/mat.h"
#include <string>

namespace LMT {

/** Convenience class.

 It is not neccessary to make Modes depends on TvrcMode<>. But it is a way to share typedefs.

 nb_var_inter = 1 -> line.
 
 nb_var_inter = 2 -> surface.
 
 nb_var_inter = 3 -> volume.
 
 @author Hugo LECLERC
 */
template<unsigned dim,unsigned nb_var_inter,class T>
class TvrcMode {
public:
    typedef std::complex<T> TC;
    typedef Vec<T,dim> Pvec;
    typedef Vec<T,dim-(dim==2)> Rvec;
    typedef Vec<TC,dim> PvecC;
    typedef Vec<TC,dim-(dim==2)> RvecC;
    typedef Vec<T,nb_var_inter> Tvi;
    
    typedef Vec<Pvec,dim> DerCoords;
    typedef Mat<T,Gen<nb_var_inter,nb_var_inter> > MNVI;
    
    TvrcMode() { amplitude = 1.0; }
    
    TC amplitude; /// a
};


/// approximation of a tension mode
template<unsigned dim,class T>
class TvrcModeRigidBody : public TvrcMode<dim,2,T> {
public:
    typedef typename TvrcMode<dim,2,T>::Pvec Pvec;
    typedef typename TvrcMode<dim,2,T>::PvecC PvecC;
    typedef typename TvrcMode<dim,2,T>::Tvi Tvi;
    
    template<class TMat> void set_param_for_w(double w,const TMat &mat) {}
    T get_min_wave_length() const { return 1e40; }
    template<class TN> PvecC get_value(const TN &node) const {
        return dir_deplacement;
    }
    static std::string name() { return "rigid_body"; }
    template<class HV> static void push_dist(HV &hv,const XmlNode &sn) {
        for(unsigned i=0;i<dim+dim-(dim==2);++i) {
            TvrcModeRigidBody tv;
            tv.dir_deplacement = 0.0;
            tv.dir_deplacement[dim] = 1.0;
            hv.push_back(tv);
        }
    }
    
    MaterialPoint<T,dim> dir_deplacement;
};

/** default procedure for integration of deplacement between too geometric entities
    \relates TvrcMode
*/
template<class TM1,class TM2,class TE1,class TE2,class T>
std::complex<T> dot_integration_on_elem(const TM1 &m1,const TM2 &m2,const TE1 &ep1,const TE2 &ep2,T min_wl ) {
    Vec<typename TE1::TNode> new_nodes1;
    Vec<typename TE2::TNode> new_nodes2;
    subdivision_element(ep1,new_nodes1,min_wl/20.0);
    subdivision_element(ep2,new_nodes2,min_wl/20.0);

    std::complex<T> res = 0.0;
    for(unsigned i=0;i<new_nodes1.size();++i)
        res += dot( m1.get_value(new_nodes1[i]) , m2.get_value(new_nodes2[i]) );
    return res / (T)new_nodes1.size();
}

/** default procedure for integration of deplacement between too geometric entities
    \relates TvrcMode
*/
template<class TM1,class TM2,class TConstraint,class TE1,class TE2,class T>
std::complex<T> dot_integration_with_constraint_dir_on_elem(const TM1 &m1,const TM2 &m2,const TConstraint &constraint, const TE1 &ep1,const TE2 &ep2,T min_wl ) {
    Vec<typename TE1::TNode> new_nodes1;
    Vec<typename TE2::TNode> new_nodes2;
    subdivision_element(ep1,new_nodes1,min_wl/20.0);
    subdivision_element(ep2,new_nodes2,min_wl/20.0);
    
    std::complex<T> res = 0.0;
    for(unsigned i=0;i<new_nodes1.size();++i)
        res += dot( m1.get_value(new_nodes1[i]), constraint.get_dir(new_nodes1[i]) ) * 
               dot( m2.get_value(new_nodes2[i]), constraint.get_dir(new_nodes2[i]) );
    return res / (T)new_nodes1.size();
}


};

#endif
