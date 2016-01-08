//
// C++ Interface: mesh_transformation
//
// Description: 
//
//
// Author: Florent PLED <pled@lmt.ens-cachan.fr>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef LMT_MESH_TRANSFORMATION_HEADER
#define LMT_MESH_TRANSFORMATION_HEADER

namespace LMT {

/*!
    Cette fonction translate la position des noeuds d'un maillage
    \keyword Maillage
*/
template<class TM,class Pvec> void translate_mesh( TM &m, const Pvec &trans ) {
    for (unsigned i=0;i<m.node_list.size();i++)
        m.node_list[i].pos+=trans;
}
template<class TM,class Pvec> void translate_mesh( TM &m, Pvec &trans ) {
    for (unsigned i=0;i<m.node_list.size();i++)
        m.node_list[i].pos+=trans;
}

/*!
    Cette fonction dilate/contracte la position des noeuds d'un maillage
    \keyword Maillage
*/
template<class TM,class Pvec> void scale_mesh( TM &m, const Pvec &scale_factor ) {
    static const unsigned dim = TM::dim;
    for (unsigned i=0;i<m.node_list.size();i++) {
        for (unsigned d=0;d<dim;d++)
            m.node_list[i].pos[d]*=scale_factor[d];
    }
}
template<class TM,class Pvec> void scale_mesh( TM &m, Pvec &scale_factor ) {
    static const unsigned dim = TM::dim;
    for (unsigned i=0;i<m.node_list.size();i++) {
        for (unsigned d=0;d<dim;d++)
            m.node_list[i].pos[d]*=scale_factor[d];
    }
}

}

#endif // LMT_MESH_TRANSFORMATION_HEADER
