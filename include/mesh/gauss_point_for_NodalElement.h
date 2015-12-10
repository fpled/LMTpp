#ifndef LMT_gauss_point_for_simplex_NodalElement_H
#define LMT_gauss_point_for_simplex_NodalElement_H

#include "NodalElement.h"

namespace LMT {

inline void gauss_points( const NodalElement &elem, unsigned order, Vec<double> &poids, Vec< Vec<double, NodalElement::nb_var_inter > > &valeurs ) { 
    return;
}

} /// LMT
#endif /// LMT_gauss_point_for_simplex_NodalElement
