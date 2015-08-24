//
// C++ Interface: surface
//
// Description: 
//
//
// Author: Hugo LECLERC <leclerc@lmt.ens-cachan.fr>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMT_surface_HEADER
#define LMT_surface_HEADER

#include "curve.h"
#include "../mesh/meshcaracallnvi.h"
#include "../mesh/delaunay.h"
#include "../mesh/make_proj_periodic.h"

namespace LMT {

template<class Carac>
class Surface : public GeometricEntity<Carac,2> {
public:
    typedef typename GeometricEntity<Carac,2>::T T;
    typedef typename GeometricEntity<Carac,2>::Pvec Pvec;
    typedef typename GeometricEntity<Carac,2>::MNVI MNVI;
    typedef typename GeometricEntity<Carac,2>::TNode TNode;
    static const unsigned dim = Carac::dim;
 
    /** p is an input/output parameter */
    virtual void get_2d_proj(Pvec &p) const = 0;
    /** p is an input/output parameter */
    virtual void set_2d_proj_in_metric_space(Pvec &p) const {}
    /** p is an input/output parameter */
    virtual void get_nd_proj(Pvec &p) const = 0;
    /** */
    virtual bool x_periodic() const { return false; }
    /** */
    virtual bool y_periodic() const { return false; }
    /**  */
    virtual void get_curvature_and_axes(const Pvec &p,MNVI &curvature,LMT::Vec<LMT::Vec<T,dim>,dim> &der_coords) const = 0;
private:
    struct Proj2D { template<class TN> void operator()(TN &node,Surface *s) const { s->get_2d_proj(node.pos); } };
    struct ProjND { template<class TN> void operator()(TN &node,Surface *s) const {
        Pvec lc = node.pos;
        s->set_2d_proj_in_metric_space(lc);
        node.local_coords[0] = lc[0];
        node.local_coords[1] = lc[1];
        s->get_nd_proj(node.pos);
    } };
protected:
    // Pvec pos_child(unsigned num_child) const { return static_cast<const Point<Carac> *>(this->template get_child<1>(num_child).ge)->pos; }
    virtual void update_repr_(T ang_prec) {
        // children data
        Mesh<MeshCaracAllNvi<dim,T> > skin;

        for(unsigned i=0;i<this->template nb_children<1>();++i) skin.append( this->template get_child<1>(i).ge->repr );
        for(unsigned i=0;i<this->template nb_children<2>();++i) skin.append( this->template get_child<2>(i).ge->repr );
        apply( skin.node_list, Proj2D(), this );
        if ( x_periodic() ) make_proj_periodic(skin,0);
        if ( y_periodic() ) make_proj_periodic(skin,1);
        
        // find children
        
        //         DisplayParaview dp;
        //         dp.add_mesh(skin);
        //         dp.exec();
        
        // delaunay
        Vec<TNode *> new_nodes;
        make_delaunay_triangulation<2>( this->repr, skin, new_nodes );
        apply( this->repr.node_list, ProjND(), this );
        // curvatures
        for(unsigned i=0;i<this->repr.node_list.size();++i)
            get_curvature_and_axes( this->repr.node_list[i].pos, this->repr.node_list[i].curvature, this->repr.node_list[i].der_coords );

        // node correspondance    
        unsigned cpt = 0;
        this->template make_node_correspondance<1>(cpt,new_nodes);
        this->template make_node_correspondance<2>(cpt,new_nodes);
    }
};


};

#endif // LMT_surface_HEADER
