//
// C++ Interface: refinement
//
// Description: 
//
//
// Author: Hugo LECLERC <leclerc@lmt.ens-cachan.fr>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMT_refinementdelaunay_HEADER
#define LMT_refinementdelaunay_HEADER

#include "refinement.h"
#include "delaunay.h"

namespace LMT {

namespace LMTPRIVATE {
    template<class TM>
    struct RefinementDelaunay {
        typedef typename TM::Pvec Pvec;
        typedef typename TM::Tpos T;
        template<class TE> void operator()(const TE &e) {
            for(unsigned i=0;i<TE::nb_nodes;++i) {
                for(unsigned j=i+1;j<TE::nb_nodes;++j) {
                    T l = length(e.node(i)->pos-e.node(j)->pos);
                    if ( l > max_length ) {
                        Pvec C;
                        T R;
                        update_circum_center(e,C,R);
                        new_points.push_back( C );
                    }
                }
            }
        }
        Vec<Pvec> new_points;
        T max_length;
    };
};


/**
*/
template<unsigned nvi,class TM,class T>
bool refinementdelaunay_if_length_sup( TM &m, T max_length ) {
    LMTPRIVATE::RefinementDelaunay<TM> r;
    r.max_length = max_length;
    apply( m.elem_list, r );
    remove_doubles( r. new_points );
    add_delaunay_points<nvi>( m, r. new_points );
    return r.new_points.size();
}


}

#endif // LMT_refinementdelaunay_HEADER
