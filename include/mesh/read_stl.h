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
#ifndef LMT_READ_STL_HEADER
#define LMT_READ_STL_HEADER

#include "mesh.h"
#include "Triangle.h"

#include <fstream>
#include <string.h>
#include <map>
#include <sstream>
#include <vector>
#include <stdexcept>

namespace LMT {

/// put gid mesh in m
template<class TM>
void read_binary_stl(TM &m,std::istream &is) throw(std::runtime_error) {
    typedef typename TM::Pvec Pvec;
    typedef typename TM::TNode TNode;
    static const int dim=TM::dim;
    using namespace std;

    char tmp[80];
    is.read(tmp,80);
    tmp[79]=0;

    int nb_elems;
    float ftmp;
    is.read((char *)&nb_elems,4);
    
    while(nb_elems--) {
        if (! is)
            throw std::runtime_error( "STL file is corrupted.");
        Pvec norm,vec[3];
        TNode *lst_n[3];
        // normal
        for(int di=0;di<3;di++) {
            is.read((char *)&ftmp,4);
            if (di<dim)
                norm[di]=ftmp;
        }
        // nodes
        for(int nn=0;nn<3;nn++) {
            for(int di=0;di<3;di++) {
                is.read((char *)&ftmp,4);
                if (di<dim)
                    vec[nn][di]=ftmp;
            }
            // if there's already an indentical node ...
            unsigned i;
            for(i=0; i!=m.node_list.size() && length(vec[nn]-m.node_list[i].pos)!=0.0 ;++i);
            if (i==m.node_list.size()) {
                lst_n[nn] = m.add_node(vec[nn]);
            }
            else
                lst_n[nn] = &m.node_list[i];
        }
        // element
        // we look if normal corresponds
        if (dot(vect_prod(vec[1]-vec[0], vec[2]-vec[0]), norm) < 0.0 )
            swap(lst_n[0],lst_n[1]);
        m.add_element(Triangle(),DefaultBehavior(),lst_n);

        is.read(tmp,2);
    }
}

/// put gid mesh in m
template<class TM>
void read_ascii_stl(TM &m,std::istream &is) throw(std::runtime_error) {
    typedef typename TM::Pvec Pvec;
    typedef typename TM::TNode TNode;
    static const int dim = TM::dim;
    using namespace std;

    //
    enum Contexte_lecture_fichier {VOID_CONTEXT,CONTEXT_POINTS,CONTEXT_ELEMENTS};

    Contexte_lecture_fichier ctxte=VOID_CONTEXT;

    //
    vector<TNode *> lst_n;
    vector<Pvec> lst_p;
    Pvec current_normal;
    while (1) {
        if (! is )
            throw std::runtime_error( "STL file is corrupted.");

        string str;
        getline(is,str);
        // �aluation du contexte
        if (str.find("outer loop")<str.size()) {
            ctxte=CONTEXT_POINTS;
            lst_n.clear();
            lst_p.clear();
            continue;
        }
        if (str.find("endloop")<str.size()) {
            ctxte=VOID_CONTEXT;
            if (lst_n.size()!=3) {
                cerr << "stl means 3 nodes per element" << endl;
                continue;
            }
            // we look if normal corresponds
            if (dot(vect_prod(lst_p[1]-lst_p[0], lst_p[2]-lst_p[0]), current_normal) < 0.0 )
                swap(lst_n[0],lst_n[1]);
            m.add_element(Triangle(),DefaultBehavior(),&lst_n[0]);
            continue;
        }
        if (str.find("endsolid")<str.size()) {
            break;
        }
        if (str.find("facet normal")<str.size()) {
            istringstream s(str);
            s >> str;
            s >> str;
            for(int di=0;di<dim;di++)
                s >> current_normal[di];
        }

        // utilisation du contexte
        if (ctxte==CONTEXT_POINTS) {
            istringstream s(str);
            s >> str;

            Pvec vec;
            for(int di=0;di<dim;di++)
                s >> vec[di];
            lst_p.push_back(vec);
            // if there's already an indentical node ...
            unsigned i;
            for(i=0; i!=m.node_list.size() && length(vec-m.node_list[i].pos)!=0.0 ;++i);
            if (i==m.node_list.size())
                lst_n.push_back(m.add_node(vec));
            else
                lst_n.push_back(&m.node_list[i]);
        }
    }
}

/// put gid mesh in m
template<class TM>
void read_stl(TM &m,std::istream &is) throw(std::runtime_error) {
    char car[6];
    is.read(car,5);
    is.seekg(0);
    if ((car[0]!='s')||(car[1]!='o')||(car[2]!='l')||(car[3]!='i')||(car[4]!='d'))
        read_binary_stl(m,is);
    else
        read_ascii_stl(m,is);
}


/*!
\keyword Maillage/Import
*/
template<class TM>
void read_stl(TM &m,const std::string &fic_name) throw(std::runtime_error) {
    // open file
    std::ifstream my_file( fic_name.c_str() );

    if ( ! my_file.is_open() )
        throw std::runtime_error( "opening of "+fic_name+" has failed." );
    read_stl(m,my_file);
}


};

#endif // LMT_READ_STL_HEADER

