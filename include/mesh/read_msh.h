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

#include "Bar.h"
#include "Bar_3.h"
#include "Triangle.h"
#include "Triangle_6.h"
#include "Quad.h"
#include "Quad_8.h"
#include "Quad_9.h"
#include "Tetra.h"
#include "Tetra_10.h"
#include "Hexa.h"
#include "Hexa_20.h"
#include "Wedge.h"
#include "Wedge_15.h"
#include "Pyramid.h"

#include <fstream>
#include <map>
#include <sstream>
#include <vector>
#include <stdexcept>

namespace LMT {

/// put gmsh mesh in m
template<class TM>
void read_msh( TM &m, std::istream &is ) throw(std::runtime_error) {
    using namespace std;
    typedef typename TM::Pvec Pvec;
    typedef typename TM::TNode TNode;
    static const int dim = TM::dim;
    unsigned nb_nodes_elem[] = { 2, 3, 4, 4, 8, 6, 5, 3, 6, 9, 10, 27, 18, 14, 1, 8, 20, 15, 13 };
    int nvi = TM::nvi;
    int nb_args;
    int ctxte = 0;

    // correspondance between number in file -> ref in mesh
    map<int,TNode *> map_num_node;
    string type_element;
    //
    while ( 1 ) {
        if ( ! is )
            throw std::runtime_error("msh file is corrupted : mark 'end elements' is absent");

        string str;
        getline(is,str);

        // évaluation du contexte
        if ( str.find("$MeshFormat") != string::npos ) {
            ctxte=-1;
            continue;     
        }
        if ( str.find("$EndMeshFormat") != string::npos ) {
            ctxte=0;
            continue;     
        }     
        if ( (str.find("$ENDNOD") != string::npos) or (str.find("$EndNodes") != string::npos) ) {
            ctxte=0;
            continue;
        }
        if ( (str.find("$ENDELM") != string::npos) or (str.find("$EndElements") != string::npos) ) {
            break;
        }
        if ( (str.find("$NOD") != string::npos) or (str.find("$Nodes") != string::npos) ) {
            getline(is,str);
            ctxte=1;
            continue;
        }
        if ( (str.find("$ELM") != string::npos) or (str.find("$Elements") != string::npos) ) {
            getline(is,str);
            ctxte=2;
            continue;
        }

        // utilisation du contexte
        if ( ctxte==-1 ) {
            istringstream s(str);
            double format;
            s >> format;
            if (format < 2) {
                cerr << "ERROR read_msh : The format " << format << " is not supported." << endl;
                return;              
            }
        }
        if ( ctxte==1 ) {
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
        if ( ctxte==2 ) {
            istringstream s(str);
            unsigned int number;
            s >> number;
            unsigned int nb_nodes, type_elem;
            s >> type_elem;
            nb_nodes = nb_nodes_elem[type_elem-1];
            s >> nb_args;
            for(int k=0;k<nb_args;++k)
                s >> number;
            
            LMT::Vec<TNode *> vn; vn.resize(nb_nodes);
            for( unsigned i=0;i<nb_nodes;i++) {
                s >> number;
                vn[i] = map_num_node[number];
            }
      
            if ( type_elem == 1 ) { //TODO
                m.add_element( Bar(),DefaultBehavior(),vn.ptr() );
            }
            else if ( type_elem == 2 ) { //TODO
                if ( nvi == 2 ) {
                    permutation_if_jac_neg ( Triangle(), vn.ptr() );
                    m.add_element( Triangle(),DefaultBehavior(),vn.ptr() );
                }
            }
            else if ( type_elem == 3 ) { //TODO
                if ( nvi == 2 ) {
                    permutation_if_jac_neg ( Quad(), vn.ptr() );
                    m.add_element( Quad(),DefaultBehavior(),vn.ptr() );
                }
            }
            else if ( type_elem == 4 ) { //TODO
                if ( nvi == 3 ) {
                    permutation_if_jac_neg ( Tetra(), vn.ptr() );
                    m.add_element( Tetra(),DefaultBehavior(),vn.ptr() );
                }
            }
            else if ( type_elem == 5 ) { //TODO
                if ( nvi == 3 ) {
                    permutation_if_jac_neg ( Hexa(), vn.ptr() );
                    m.add_element( Hexa(),DefaultBehavior(),vn.ptr() );
                }
            }
            else if ( type_elem == 6 ) { //TODO
                if ( nvi == 3 ) {
                    permutation_if_jac_neg ( Wedge(), vn.ptr() );
                    m.add_element( Wedge(),DefaultBehavior(),vn.ptr() );
                }
            }
            else if ( type_elem == 7 ) { //TODO
                if ( nvi == 3 ) {
                    permutation_if_jac_neg ( Pyramid(), vn.ptr() );
                    m.add_element( Pyramid(),DefaultBehavior(),vn.ptr() );
                }
            }
            else if ( type_elem == 8 ) { //TODO
                if ( nvi == 1 ) {
                    m.add_element( Bar_3(),DefaultBehavior(),vn.ptr() );
                }
            }
            else if ( type_elem == 9 ) { //TODO
                if ( nvi == 2 ) {
                    permutation_if_jac_neg ( Triangle_6(), vn.ptr() );
                    m.add_element( Triangle_6(),DefaultBehavior(),vn.ptr() );
                }
            }
            else if ( type_elem == 10 ) { //TODO
                if ( nvi == 2 ) {
                    permutation_if_jac_neg ( Quad_9(), vn.ptr() );
                    m.add_element( Quad_9(),DefaultBehavior(),vn.ptr() );
                }
            }
            else if ( type_elem == 11 ) { //TODO
                if ( nvi == 3 ) {
                    swap( vn[8], vn[9] );
                    permutation_if_jac_neg ( Tetra_10(), vn.ptr() );
                    m.add_element( Tetra_10(),DefaultBehavior(),vn.ptr() );
                }
            }
//            else if ( type_elem == 12 ) { //TODO
//                if ( nvi == 3 ) {
//                    permutation_if_jac_neg ( Hexa_27(), vn.ptr() );
//                    m.add_element( Hexa_27(),DefaultBehavior(),vn.ptr() );
//                }
//            }
//            else if ( type_elem == 13 ) { //TODO
//                if ( nvi == 3 ) {
//                    permutation_if_jac_neg ( Wedge_18(), vn.ptr() );
//                    m.add_element( Wedge_18(),DefaultBehavior(),vn.ptr() );
//                }
//            }
//            else if ( type_elem == 14 ) { //TODO
//                if ( nvi == 3 ) {
//                    permutation_if_jac_neg ( Pyramid_14(), vn.ptr() );
//                    m.add_element( Pyramid_14(),DefaultBehavior(),vn.ptr() );
//                }
//            }
            else if ( type_elem == 15 ) { //TODO
            }
            else if ( type_elem == 16 ) { //TODO
                if ( nvi == 2 ) {
                    permutation_if_jac_neg ( Quad_8(), vn.ptr() );
                    m.add_element( Quad_8(),DefaultBehavior(),vn.ptr() );
                }
            }
            else if ( type_elem == 17 ) { //TODO
                if ( nvi == 3 ) {
                    permutation_if_jac_neg ( Hexa_20(), vn.ptr() );
                    m.add_element( Hexa_20(),DefaultBehavior(),vn.ptr() );
                }
            }
            else if ( type_elem == 18 ) { //TODO
                if ( nvi == 3 ) {
                    permutation_if_jac_neg ( Wedge_15(), vn.ptr() );
                    m.add_element( Wedge_15(),DefaultBehavior(),vn.ptr() );
                }
            }
//            else if ( type_elem == 19 ) { //TODO
//                if ( nvi == 3 ) {
//                    permutation_if_jac_neg ( Pyramid_13(), vn.ptr() );
//                    m.add_element( Pyramid_13(),DefaultBehavior(),vn.ptr() );
//                }
//            }
            else {
                std::cout << "unknown type elem in read_msh -> " << type_elem << std::endl;
            }            
//             if ( type_elem == 1 ) { // TODO
//                 if ( TM::dim == 1 )
//                     m.add_element(Bar(),DefaultBehavior(),vn.ptr());
//             }
//             else if ( type_elem == 2 ) {
//                 permutation_if_jac_neg(Triangle(),vn.ptr());
//                 m.add_element(Triangle(),DefaultBehavior(),vn.ptr());
//             }
//             else {
//                 std::cout << "unknown type elem." << std::endl;
//                 assert(0);
//             }

            continue;
        }
    }
}


/// put gid mesh in m
/*!
Attention: importe seulement les Bar() et les Triangle() 
\keyword Maillage/Import
*/
template<class TM>
void read_msh( TM &m, const std::string &fic_name ) throw(std::runtime_error) {
    // ouverture du fichier
    std::ifstream my_file( fic_name.c_str() );
    if ( ! my_file.is_open() )
        throw std::runtime_error( "opening of "+fic_name+" has failed.");
    return read_msh(m,my_file);
}


};

#endif // LMT_READ_GID_HEADER

