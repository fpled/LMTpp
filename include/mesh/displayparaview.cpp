//
// C++ Implementation: displayparaview
//
// Description: 
//
//
// Author: Hugo LECLERC <leclerc@lmt.ens-cachan.fr>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "nodalelement.h"
#include "bar.h"
#include "bar_3.h"
#include "triangle.h"
#include "triangle_6.h"
#include "quad.h"
#include "quad_8.h"
#include "tetra.h"
#include "tetra_10.h"
#include "hexa.h"
#include "hexa_20.h"
#include "wedge.h"
#include "wedge_15.h"

namespace LMT {

const unsigned NodalElement::nb_nodes;
const unsigned Bar::nb_nodes;
const unsigned Bar_3::nb_nodes;
const unsigned Triangle::nb_nodes;
const unsigned Triangle_6::nb_nodes;
const unsigned Quad::nb_nodes;
const unsigned Quad_8::nb_nodes;
const unsigned Tetra::nb_nodes;
const unsigned Tetra_10::nb_nodes;
const unsigned Wedge::nb_nodes;
const unsigned Wedge_15::nb_nodes;
const unsigned Hexa::nb_nodes;
const unsigned Hexa_20::nb_nodes;

#ifdef PRINT_ALLOC
std::map<std::string,long long> total_allocated;
#endif // PRINT_ALLOC

};
