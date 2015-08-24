//
// C++ Implementation: symamd
//
// Description:
//
//
// Author: Hugo LECLERC <hugo.leclerc@lmt.ens-cachan.fr>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "symamd.h"
#include "../amd/amd.h"

namespace LMT {

Vec<unsigned> symamd(const Vec<Vec<unsigned> > &ind) {
    Vec<int> lines; lines.resize( ind.size() + 1 );
    Vec<int> indices;
    
    unsigned nb_ind = 0;
    for(unsigned i=0;i<ind.size();++i)
        nb_ind += ind[i].size();
    indices.reserve( nb_ind ); 
    
    lines[0] = 0;
    for(unsigned i=0;i<ind.size();++i) {
        for(unsigned j=0;j<ind[i].size();++j)
            indices.push_back( ind[i][j] );
        lines[i+1] = indices.size();
    }
    Vec<unsigned> P; P.resize( ind.size() );
    amd_order( ind.size(), lines.begin(), indices.begin(), (int *)P.begin(), (double *)NULL, (double *)NULL );
    return P;
}


// /// max_col must be inferior to line
// unsigned nb_correspondance(const Vec<Vec<unsigned> > &indices,unsigned max_col,unsigned line) {
//     Vec<unsigned> new_indices; new_indices.reserve( indices.size() );
//     
//     for(unsigned col=0;col<=max_col;++col) {
//         const Vec<unsigned> &l1 = indices[col];
//         const Vec<unsigned> &l2 = indices[line];
//         unsigned p1=0, p2=0;
//         while( l1[p1]<=max_col or l2[p2]<=max_col ) {
//             if ( l1[p1]==l2[p2] ) {
//                 ++p1; ++p2;
//                 new_indices.push_back( l1[p1] );
//             }
//             else if ( l1[p1]<l2[p2] ) ++p1;
//             else                      ++p2;
//         }
//     }
//     
//     
//     return new_indices.size();
// }
// void apply_perm(Vec<Vec<unsigned> > &indices,unsigned old_emplacement, unsigned new_emplacement) {
//     for(unsigned i=new_emplacement;i<indices.size();++i)
//         for(unsigned j=0;indices[i][j] <= old_emplacement;++j)
//             if ( indices[i][j] == old_emplacement )
//                 indices[i][j] = new_emplacement;
//     for(unsigned i=new_emplacement;i<indices.size();++i)
//         std::sort( indices[i].begin(), indices[i].end() );
// }
// 
// unsigned cost_symamd(const Vec<Vec<unsigned> > indices) {
//     unsigned cost = 0;
//     for(unsigned i=0;i<indices.size();++i)
//         for(unsigned j=0;j<=i;++j)
//             cost += nb_correspondance( indices, j, i );
//     return cost;
// }
// 
// Vec<unsigned> symamd(const Vec<Vec<unsigned> > &indices_) {
//     Vec<Vec<unsigned> > indices = indices_;
//     unsigned nb_lines = indices_.size();
// 
//     Vec<unsigned> perm; perm.push_back(0);
//     unsigned cost = 0;
//     for(unsigned line=1;line<nb_lines;++line) {
//         unsigned next_perm = 0, nb_corr = nb_lines * nb_lines;
//         for(unsigned i=line+1;i<nb_lines;++i) {
//             unsigned res = nb_correspondance( indices, line, i );
//             if ( res < nb_corr ) {
//                 nb_corr = res;
//                 next_perm = i;
//             }
//         }
//         cost += nb_corr;
//         
//         perm.push_back( next_perm );
//         std::swap( indices[next_perm], indices[line] );
//         apply_perm( indices, next_perm, line );
//     }
//     
//     //
//     std::cout << "new -> " << cost << std::endl;
//     
//     return perm;
// }

}
