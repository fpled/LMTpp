/***************************************************************************
                          read_ply.h  -  description
                             -------------------
    begin                : lun oct 28 2002
    copyright            : (C) 2002 by hleclerc
    email                : hleclerc@lmamef23
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef READ_PLY_H
#define READ_PLY_H

#include <fstream>
#include <string.h>
#include <map>
#include <sstream>

#include "Triangle.h"
//#include "element_Triangle.h"

/// put gid mesh in m
template<class TM>
void read_ply(TM &m,istream &is) {
  typedef typename TM::node_ref node_ref;
  typedef typename TM::Pvec Pvec;
  typedef typename TM::T_pos T_pos;
  static const int dim=TM::dim;
  
  int nb_vertex;
  int nb_faces;
  // ref of nodes
  vector<node_ref> lst_n;
  // format of data (binary_big_endian, ascii, ...)
  string format;
  // version of format
  string format_version;
  // size of binary properties
  vector<int> size_prop;
  // name of properties
  vector<string> name_prop;

  map<string,int> size_types;
  size_types["int8"] = 1;
  size_types["int16"] = 2;
  size_types["int32"] = 4;
  size_types["uint8"] = 1;
  size_types["uint16"] = 2;
  size_types["uint32"] = 4;
  size_types["float32"] = 4;
  size_types["float64"] = 8;
  size_types["char"] = 1;
  size_types["short"] = 2;
  size_types["int"] = 4;
  size_types["uchar"] = 1;
  size_types["ushort"] = 2;
  size_types["uint"] = 4;
  size_types["float"] = 4;
  size_types["double"] = 8;
 
  string str;
  do {
    getline(is,str);
    if (!is)
        throw (std::runtime_error( "header ply file corrputed in read_ply" ));
    if (str.find("element vertex")<str.size()) {
      istringstream ss(str);
      ss >> str >> str >> nb_vertex;
    }
    else if (str.find("element face")<str.size()) {
      istringstream ss(str);
      ss >> str >> str >> nb_faces;
    }
    else if (str.find("format")<str.size()) {
      istringstream ss(str);
      ss >> str >> format >> format_version;
    }
    else if (str.find("property")<str.size()) {
      istringstream ss(str);
      string type,name;
      ss >> str >> type >> name;
      size_prop.push_back(size_types[type]);
      name_prop.push_back(name);
    }
  } while (str.find("end_header")!=0);

  if (format=="ascii") {
    while (nb_vertex--) {
      Pvec vec;
      getline(is,str);
      if (!is)
        throw (std::runtime_error( "ply file corrupted in read_ply" ));
      istringstream ss(str);
      int di;
      for(di=0;di<dim;++di)
        ss >> vec(di);
      for(;di<name_prop.size();++di)
        ss >> str;
      lst_n.push_back(m.add_node(vec));
    }
    vector<node_ref> lst_n_in_elem;
    while (nb_faces--) {
      getline(is,str);
      if (!is)
        throw (std::runtime_error( "ply file corrupted in read_ply" ));
      istringstream ss(str);
      int nb_nodes;
      ss >> nb_nodes;
      lst_n_in_elem.resize(nb_nodes);
      for(int i=0;i<nb_nodes;++i)
        ss >> lst_n_in_elem[i];
      for(int i=0;i<nb_nodes;++i)
        lst_n_in_elem[i] = lst_n[lst_n_in_elem[i]];
      m.add_element(Triangle(),&lst_n_in_elem[0]);
    }
  }
  else
    throw (std::runtime_error( "format "+format+" not recognized" ));

//  else if (format=="binary_big_endian") {
//    while (nb_vertex--) {
//      Pvec vec;
//      getline(is,str);
//      istringstream ss(str);
//      for(int di=0;di<dim;di++) {
//        float val;
//        ss.read((char *)&val,size_prop[di]);
//        vec(di) = val;
//      }
//      for(int di=dim-1;di<name_prop.size();di++)
//        ss.read((char *)&vec(di),size_prop[di]);
//      lst_n.push_back(m.add_node(vec));
//    }
//    vector<node_ref> lst_n_in_elem;
//    while (nb_faces--) {
//      int nb_nodes;
//      ss.read((char *)&val,size_prop[di]);
//      ss >> nb_nodes;
//      lst_n_in_elem.resize(nb_nodes);
//      for(int i=0;i<nb_nodes;++i)
//        ss >> lst_n_in_elem[ lst_n[i] ];
//      m.add_element(Triangle(),&lst_n_in_elem[0]);
//    }
//  }
    
}

/*!
 put gid mesh in m
\keyword Maillage/Import
*/
template<class TM>
void read_ply(TM &m,const char *fic_name) {
  // open file
  ifstream my_file( fic_name );
  if ( ! my_file.is_open() )
    throw (std::runtime_error( "error while opening "+std::string(fic_name) ));
  read_ply(m,my_file);
}



#endif // READ_PLY_H
