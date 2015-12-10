/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef READ_PAT_H
#define READ_PAT_H

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

#include "Triangle.h"
#include "Tetra.h"
#include "Hexa.h"
//#include "mesh/extract_scalar.h"

/// read elements
template<class TM>
void read_pat_elements(TM &m,istream &is,int nb_elem, std::map<int,int> &map_num_node) {
  typedef typename TM::Pvec Pvec;
  typedef typename TM::T_pos T_pos;
  using namespace std;

  int n_nodes;
  string str;

  getline(is,str);

  for (int num_elem =0;num_elem< nb_elem; num_elem++)
  {
    getline(is,str);
    getline(is,str);
    istringstream s(str);
    s >> n_nodes;

    getline(is,str);
    s.clear();
    s.str(str);
    
    std::vector<int> vn(n_nodes);
    for(int i=0;i<n_nodes;i++) {
      int number;
      s >> number;
      vn[i]=map_num_node[number];
    }
    if (!is) {
      std::cerr << "Pat file is corrupted while reading element data." << std::endl;
      return;
    }

    // ATTETION DANS MOLDFLOW SEULS LES TRIANGLES ET THETRAHEDRES SONT UTILISES
    if (n_nodes == 3)
      m.add_element(Triangle(),&vn[0]);
    else if (n_nodes == 4)
      m.add_element(Tetra(),&vn[0]);
    else
      std::cerr << "Elements with " << n_nodes << " nodes are not managed in read_pat()." << std::endl;
  }
  return;
}

/// read nodes
template<class TM>
void read_pat_nodes(TM &m,istream &is,int nb_node, std::map<int,int> &map_num_node) {
  typedef typename TM::Pvec Pvec;
  typedef typename TM::T_pos T_pos;
  static const int dim=TM::dim;
  using namespace std;
  
  int node_lab;
  string str;
  for (int num_node=0; num_node < nb_node; num_node++)
  {
    for (int i=0;i<2;i++)
      getline(is,str);
    istringstream s(str);
    s >> node_lab;
    s >> node_lab;

    getline(is,str);
    s.clear();
    s.str(str);

    Pvec vec;
    for(int di=0;di<dim;di++)
       s >> vec[di];
    map_num_node[node_lab]=m.add_node(vec);
  }
  return;
}


/// put pat mesh from MOLDFLOW in m
template<class TM>
void read_pat(TM &m,istream &is) {
  using namespace std;
  typedef typename TM::Pvec Pvec;
  typedef typename TM::T_pos T_pos;
  
  // correspondance between number in file -> ref in mesh
  map<int,int> map_num_node;
  //
  int nb_node, nb_elem;
  int news;

cout << " dans read_pat" <<endl;

  string str;
  for (int i=0;i<3;i++)
    getline(is,str);

  istringstream s(str);

  for (int i=0;i<4;i++)
      s >> news;
  s >> nb_node;
  s >> nb_elem;

  read_pat_nodes(m,is,nb_node,map_num_node);
  read_pat_elements(m,is,nb_elem,map_num_node);

  return;
}

/*!
 put pat mesh from Moldflow Sotware in m
\keyword Maillage/Import
*/
template<class TM>
void read_pat(TM &m,const char *fic_name) {
  // open file
  ifstream my_file( fic_name );
  if ( ! my_file.is_open() ) {
    std::cerr << "error while opening " << fic_name << std::endl;
    throw (std::runtime_error( "error while opening "+std::string(fic_name) ));
  }
  read_pat(m,my_file);
}



#endif // READ_unv_H
