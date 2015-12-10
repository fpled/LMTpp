//
// C++ Interface: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMT_READ_GID_HEADER
#define LMT_READ_GID_HEADER

#include "mesh.h"
#include "Triangle.h"
#include "Triangle_6.h"
#include "Tetra.h"

#include <fstream>
#include <map>
#include <sstream>
#include <vector>
#include <stdexcept>

namespace LMT {

/// put gid mesh in m
template<class TM>
void read_gid(TM &m,std::istream &is) throw(std::runtime_error) {
    using namespace std;
    typedef typename TM::Pvec Pvec;
    typedef typename TM::TNode TNode;
    static const int dim = TM::dim;

    //
    int ctxte=0;

    // correspondance between number in file -> ref in mesh
    map<int,TNode *> map_num_node;
    string type_element;
    int nnode_elem;
    //
    while (1) {
        if (! is )
            throw std::runtime_error("gid file is corrupted : mark 'end elements' is absent");

        string str;
        getline(is,str);

        // �aluation du contexte
        if (str.find("end coordinates")<str.size()) {
            ctxte=0;
            continue;
        }
        if (str.find("end elements")<str.size())
            break;
        if (str.find("Coordinates")<str.size()) {
            ctxte=1;
            continue;
        }
        if (str.find("Elements")<str.size()) {
            ctxte=2;
            continue;
        }

        // utilisation du contexte
        if (ctxte==1) {
            istringstream s(str);
            int number;
            s >> number;
            Pvec vec;
            for(int di=0;di<dim;di++)
                s >> vec[di];
            // we add the node and we retain is ref
            map_num_node[number] = m.add_node(vec);
            continue;
        }
        if (ctxte==2) {
            istringstream s(str);
            int number;
            s >> number;
            std::vector<TNode *> vn(nnode_elem);
            for(int i=0;i<nnode_elem;i++) {
                s >> number;
                vn[i] = map_num_node[number];
            }
            if (type_element=="Triangle") {
                if ( nnode_elem==3 )      m.add_element(Triangle(),DefaultBehavior(),&vn[0]);
                else if ( nnode_elem==6 ) m.add_element(Triangle_6(),DefaultBehavior(),&vn[0]);
                else {
                    std::ostringstream os;
                    os << "I don't know any triangle elements with " << nnode_elem << " nodes.";
                    throw std::runtime_error( os.str() );
                }
            }
            else if (type_element=="Tetrahedra")
                m.add_element(Tetra(),DefaultBehavior(),&vn[0]);
            else
                assert(0);

            continue;
        }
        // 
        istringstream s(str);
        s >> str;
        if (str=="MESH") {
            while ( s ) {
                s >> str;
                if (str=="ElemType")
                    s >> type_element;
                if (str=="Nnode") {
                    s >> nnode_elem;
                    break;
                }
            }
        }
    }
}

/*!
 put gid mesh in m
\keyword Maillage/Import
*/
template<class TM>
void read_gid(TM &m,const std::string &fic_name) throw(std::runtime_error) {
    // ouverture du fichier
    std::ifstream my_file( fic_name.c_str() );
    if ( ! my_file.is_open() )
        throw std::runtime_error( "opening of "+fic_name+" has failed.");
    return read_gid(m,my_file);
}


};

#endif // LMT_READ_GID_HEADER

