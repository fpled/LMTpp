//
// C++ Interface: meshtransfert
//
// Description: 
//
//
// Author: Grover <hugo@grosnordi>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MESH_TRANSFERT_H
#define MESH_TRANSFERT_H

#include "diffuseapproximation.h"

namespace LMT {

/**
   methods to tranfer nodal fields of m1 to nodal fields of m2, with an optionnal position offset
 */
template<class TM1,class TM2>
class MeshTransfer {
public:
    typedef typename TM1::Pvec Pvec;
    MeshTransfer(TM1 *m_1,TM2 *m_2,const Pvec offset_=0.0):m1(m_1),m2(m_2),offset(offset_) {
        for(unsigned i=0;i<m1->node_list.size();++i)
            da.add_node( m1->node_list[i].pos + offset, 0.142 );
        da.calc_mat();
    }
    
    Vec<double> transfer( const Vec<double> &v ) {
        da.get_coeffs( v );
        Vec<double> res; res.resize( m2->node_list.size() );
        for(unsigned i=0;i<res.size();++i)
            res[i] = da.get_val( m2->node_list[i].pos );
        return res;
    }
    DiffuseApproximation<typename TM1::Tpos,TM1::dim> da;
    TM1 *m1;
    TM2 *m2;
    Pvec offset;
};

}

#endif // MESH_TRANSFERT_H
