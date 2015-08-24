/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef READ_unv_H
#define READ_unv_H

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

#include "bar.h"
#include "triangle.h"
#include "quad.h"
#include "tetra.h"
#include "hexa.h"

/// read nodes
template<class TM>
void read_unv_elements(TM &m,std::istream &is,std::map<int,typename TM::TNode *> &map_num_node) {
  typedef typename TM::Pvec Pvec;
  typedef typename TM::Tpos Tpos;
  typedef  typename TM::TNode TNode;
  using namespace std;

  int element_lab,fe_descriptor_id,phys_prop_tab_num,mat_prop_tab_num,color,n_nodes;
  string str;
  while (true) {
    getline(is,str);
    istringstream s(str);
    s >> element_lab;
    if (element_lab==-1)
      return;
          
    s >> fe_descriptor_id        // read FE descriptor id
      >> phys_prop_tab_num       // (not supported yet)
      >> mat_prop_tab_num        // (not supported yet)
      >> color                   // (not supported yet)
      >> n_nodes;                // read number of nodes on element
      
    getline(is,str);

    s.clear();
    s.str(str);

    Vec<TNode *> vn;
    vn.resize(n_nodes);
    for(int i=0;i<n_nodes;i++) {
      int number;
      s >> number;
      vn[i]=map_num_node[number];
    }
    if (!is) {
      std::cerr << "Unv file is corrupted while reading element data." << std::endl;
      return;
    }

    if (n_nodes==2)
      m.add_element(Bar(),DefaultBehavior(),&vn[0]);
    else if (n_nodes==3)
      m.add_element(Triangle(),DefaultBehavior(),&vn[0]);
    else if (n_nodes==4){
      if (TM::dim == 2)
         m.add_element(Quad(),DefaultBehavior(),&vn[0]);
      else if (TM::dim == 3)
         m.add_element(Tetra(),DefaultBehavior(),&vn[0]);
    }
    else if (n_nodes==8)
      m.add_element(Hexa(),DefaultBehavior(),&vn[0]);
    else
      std::cerr << "Elements with " << n_nodes << " nodes are not managed in read_unv()." << std::endl;
  }
  return;
}

/// read nodes
template<class TM>
void read_unv_nodes(TM &m,std::istream &is,std::map<int,typename TM::TNode *> &map_num_node) {
  typedef typename TM::Pvec Pvec;
  typedef typename TM::Tpos Tpos;
  static const int dim=TM::dim;
  using namespace std;

  int exp_coord_sys_num,disp_coord_sys_num,color,node_lab;
  string str;
  while (true) {
    getline(is,str);
    istringstream s(str);
    s >> node_lab;
    if (node_lab==-1)
      break;
      
    s >> exp_coord_sys_num       // (not supported yet)
      >> disp_coord_sys_num      // (not supported yet)
      >> color;                   // (not supported yet)

    getline(is,str);

    // on remplace le 'D' par 'e' dans le format scientifique....
    for(unsigned i=0;i<str.size();++i){
       if (str[i]=='D' or str[i]=='d') str[i]='e';
    }

    s.clear();
    s.str(str);
    Vec<double,dim> vec;
    for(int di=0;di<dim;di++)
       s >> vec[di];
    map_num_node[node_lab]=m.add_node(vec);
  }
}

/// read result
// TODO lecture de champs resultats

// template<class TM>
// void read_unv_results(TM &m,std::istream &is,std::map<int,typename TM::TNode *> &map_num_node) {
//   typedef typename TM::Pvec Pvec;
//   typedef typename TM::Tpos Tpos;
//   using namespace std;
//   
//   std::string keyword,str;
//   getline(is,str);
//   istringstream s(str);
//   s >> keyword;
//   for(int i=0;i<5;++i)
//     getline(is,str);
//   s.clear();
//   s.str(str);
//   int nb_dim;
//   s >> nb_dim;
// 
//   std::vector<std::vector<Tpos> > res(nb_dim);
//   for(int di=0;di<nb_dim;di++)
//     res[di].resize(m.node_list.size());
//     
//   getline(is,str);
//   getline(is,str);
// 
//   int node_lab;
//   while (true) {
//     getline(is,str);
//     if (!is) {
//       cerr << "UNV file not corrupted while reading results." << endl;
//       return;
//     }
//     istringstream s(str);
//     s >> node_lab;
//     if (node_lab==-1)
//       break;
//       
//     getline(is,str);
//     s.clear();
//     s.str(str);
//     for(int di=0;di<nb_dim;di++) {
//       Tpos val;
//       s >> val;
//       res[di][m.node_list.number(map_num_node[node_lab])] = val;
//     }
// 
//   }
//   for(int di=0;di<nb_dim;di++) {
//     ostringstream s;
//     s << keyword << "(" << di << ")";
//     write_scalars_to_mesh(m,s.str().c_str(),&res[di][0]);
//   }
// }

/// put unv mesh in m
template<class TM>
void read_unv(TM &m,std::istream &is) {
  using namespace std;
  typedef typename TM::Pvec Pvec;
  typedef typename TM::Tpos Tpos;
  typedef  typename TM::TNode TNode;

  // correspondance between number in file -> ref in mesh
  map<int,TNode *> map_num_node;
  //
  int old,news=0;
  while (true) {
    string str;
    do {
      old = news;
      getline(is,str);
      if ( !is )
        return;
      istringstream s(str);
      s >> news;
    } while ((news!=-1));
    getline(is,str);
    istringstream s(str);
    s >> news;
    switch (news) {
      case (2411):
        cout<<"\tLecture des noeuds"<<endl;
        read_unv_nodes(m,is,map_num_node);
        break;
      case (2412):
        cout<<"\tLecture des elements"<<endl;
        read_unv_elements(m,is,map_num_node);
        break;
//       case (55):
//         read_unv_results(m,is,map_num_node);
//         break;
    }
    news = -1;
  }
}

/*!
 put unv mesh in m
\keyword Maillage/Import
*/
template<class TM>
void read_unv(TM &m,const std::string &fic_name) throw(std::runtime_error) {
    // ouverture du fichier
    std::ifstream my_file( fic_name.c_str() );
    if ( ! my_file.is_open() )
        throw std::runtime_error( "opening of "+fic_name+" has failed.");
    return read_unv(m,my_file);
}

#endif // READ_unv_H
