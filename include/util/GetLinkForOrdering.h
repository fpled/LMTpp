#ifndef GETLINKFORORDERING_H
#define GETLINKFORORDERING_H

#include "../containers/vec.h"
#include <algorithm>

namespace LMT {
    namespace LMT_PRIVATE {
        struct GetLink {
            template<class TE,class TM> void operator()(const TE &e,const TM &m,Vec<Vec<unsigned> > &v) const {
                for(unsigned i=0;i<TE::nb_nodes;++i) {
                    unsigned a = m.node_list.number(*e.node(i));
                    for(unsigned j=0;j<TE::nb_nodes;++j) {
                        unsigned b = m.node_list.number(*e.node(j));
                        if ( std::find(v[a].begin(),v[a].end(),b)==v[a].end() )
                            v[a].push_back( b );
                    }
                }
            }
        };
    }

    /**
            perform amd ordering on nodes of m (return node numbers)
        */
    template<class TM>
    void get_link_for_ordering( Vec<Vec<unsigned> > &ind, const TM &m ) {
        ind.resize( m.node_list.size() );
        apply( m.elem_list, LMT_PRIVATE::GetLink(), m, ind );
        for(unsigned i=0;i<ind.size();++i)
            std::sort( ind[i].begin(), ind[i].end() );
    }
}

#endif // GETLINKFORORDERING_H
