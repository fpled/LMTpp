//
// C++ Interface: node_reorder
//
// Description: 
//
//
// Author: Hugo LECLERC <hugo.leclerc@lmt.ens-cachan.fr>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMT_node_reorder_HEADER
#define LMT_node_reorder_HEADER

#include "../util/symrcm.h"

namespace LMT {

namespace LMTPRIVATE {
    struct FillIndRcmMesh {
        template<class TE> void operator()(const TE &e,Vec<Vec<unsigned> > &ind) const {
            for(unsigned i=0;i<TE::nb_nodes;++i) {
                unsigned n1 = e.node(i)->number_in_original_mesh();
                for(unsigned j=0;j<TE::nb_nodes;++j) {
                    unsigned n2 = e.node(j)->number_in_original_mesh();
                    if ( std::find( ind[ n1 ].begin(), ind[ n1 ].end(), n2 ) == ind[ n1 ].end() )
                        ind[ n1 ].push_back( n2 );
                }
            }
        }
    };
}

/**
    Reorder nodes in mesh such as factorization of matrix for pb based on nodal unknowns should be faster
*/
template<class TM>
void node_reorder(TM &m) {
    Vec<Vec<unsigned> > ind; ind.resize(m.node_list.size());
    apply( m.elem_list, LMTPRIVATE::FillIndRcmMesh(), ind );
    for(unsigned i=0;i<ind.size();++i)
        std::sort( ind[i].begin(), ind[i].end() );

    Vec<unsigned> perm = symrcm( ind );

    Vec<typename TM::TNode *> cor; cor.resize( m.node_list.size() );
    for(unsigned i=0;i<m.node_list.size();++i)
        cor[i] = &m.node_list[ perm[i] ];
    m.change_node_ptr_in_elements(cor.begin());
        
    Vec<typename TM::TNode> new_nodes; new_nodes.resize(m.node_list.size());
    for(unsigned i=0;i<m.node_list.size();++i)
        new_nodes[i] = m.node_list[ perm[i] ];
    for(unsigned i=0;i<m.node_list.size();++i) {
        m.node_list[i] = new_nodes[i];
        m.node_list[i].number = i;
    }
    
    m.signal_connectivity_change();
}

};

#endif // LMT_node_reorder_HEADER
