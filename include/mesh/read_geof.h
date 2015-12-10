#ifndef READ_GEOF_H
#define READ_GEOF_H

#include "Bar.h"
#include "Triangle.h"
#include "Triangle_6.h"
#include "Quad.h"
#include "Tetra.h"
#include "Hexa.h"
#include "Wedge.h"
#include <stdexcept>
#include <map>
using namespace std;

namespace LMT {
/// put geof mesh from zebulon
template<class TM>
void read_geof(TM &mesh, std::istream &is) throw(std::runtime_error) {

    //TM mesh;
    typedef typename TM::Tpos T;
    typedef typename TM::Pvec Pvec;
    typedef typename TM::TNode TNode;
    static const int dim = TM::dim;
    map<int,TNode *> map_num_node;

    //is.tellg();
    // correspondance between number in file -> ref in mesh

    // obtaining nbnode, nbelem
    string str;
    getline(is,str);//***geometry
    getline(is,str);//**node
    getline(is,str);// nbnode et dimension
    istringstream s(str);

    int nbnode;
    s >> nbnode;
    int dim1;
    s >> dim1;

    if (dim1 != dim) {
        std::cout << "Dimension du maillage � lire ("<<dim1<<") incompatible avec celle du maillage pass� ("<<dim<<")" << endl;
        assert(0);
    }

    // reading nodes coordinates
    int nb=0;
    Vec<int> NodeNumber;
    NodeNumber.resize(nbnode);
    while(nb<nbnode) {
        string str;
        getline(is,str);
        istringstream s(str);
        int number;
        s >> number;
        //s=tokenize(str);

        Vec<double,dim> vec;
        for(unsigned di=0;di<(unsigned)dim;di++)
            s >> vec[di];

        // we add the node and we retain is ref
        map_num_node[number] = mesh.add_node(vec);

        nb=nb+1;
    }
    //PRINT( generate( mesh.node_list, ExtractDM<pos_DM>() ) );

    //reading elem
    getline(is,str);//**element
    getline(is,str);//nbelem
    istringstream s1(str);

    int nbelem;
    s1 >> nbelem;

    nb=0;
    //int nbnodeelem;
    int nnode_elem;
    string type_elem;
    while(nb<nbelem) {
        string str;
        getline(is,str);
        istringstream s(str);
        int number;
        s >> number; // numero de l'element

        s >> type_elem;//type d'element

        if (type_elem=="line") {
            nnode_elem=2;
            Vec<TNode *> vn;
            vn.resize(nnode_elem);
            for(int i=0;i<nnode_elem;i++) {
                s >> number;
                vn[i] = map_num_node[number];
            }
            //permutation_if_jac_neg(Bar(),vn.ptr());
            mesh.add_element(Bar(),DefaultBehavior(),&vn[0]);
        } else if (type_elem=="tri") {
            Vec<TNode *> vn;
            while ( true ) {
                unsigned number;
                s >> number;
                if ( !s ) break;
                vn.push_back( map_num_node[number] );
            }
            if ( vn.size()==3 ) {
                if ( dim==2 ) {
                    T o = ((*vn[1]).pos[0]-(*vn[0]).pos[0])*((*vn[2]).pos[1]-(*vn[0]).pos[1]) - ((*vn[1]).pos[1]-(*vn[0]).pos[1])*((*vn[2]).pos[0]-(*vn[0]).pos[0]);
                    if ( o < 0 )
                        swap( vn[0], vn[1] );
                }
                permutation_if_jac_neg(Triangle(),vn.ptr());
                mesh.add_element(Triangle(),DefaultBehavior(),&vn[0]);
            }
            else if ( vn.size()==6 ) {
               permutation_if_jac_neg(Triangle_6(),vn.ptr());
               mesh.add_element(Triangle_6(),DefaultBehavior(),&vn[0]);
            }
            else throw std::runtime_error("Unknown element...");
            
        } else if (type_elem=="c2d4") {
            nnode_elem=4;
            Vec<TNode *> vn;
            vn.resize(nnode_elem);
            for(int i=0;i<nnode_elem;i++) {
                s >> number;
                vn[i] = map_num_node[number];
            }
            permutation_if_jac_neg(Quad(),vn.ptr());
            mesh.add_element(Quad(),DefaultBehavior(),&vn[0]);
        } else if (type_elem=="tet") {
            nnode_elem=4;
            Vec<TNode *> vn;
            vn.resize(nnode_elem);
            for(int i=0;i<nnode_elem;i++) {
                s >> number;
                vn[i] = map_num_node[number];
            }
            permutation_if_jac_neg(Tetra(),vn.ptr());
            mesh.add_element(Tetra(),DefaultBehavior(),&vn[0]);
        } else if (type_elem=="cub8" or type_elem=="hex") {
            nnode_elem=8;
            Vec<TNode *> vn;
            vn.resize(nnode_elem);
            for(int i=0;i<nnode_elem;i++) {
                s >> number;
                vn[i] = map_num_node[number];
            }
            permutation_if_jac_neg(Hexa(),vn.ptr());
            mesh.add_element(Hexa(),DefaultBehavior(),&vn[0]);
        } else if (type_elem=="prism") {
            nnode_elem=6;
            Vec<TNode *> vn;
            vn.resize(nnode_elem);
            for(int i=0;i<nnode_elem;i++) {
                s >> number;
                vn[i] = map_num_node[number];
            }
            permutation_if_jac_neg(Wedge(),vn.ptr());
            mesh.add_element(Wedge(),DefaultBehavior(),&vn[0]);
        } else {
            std::cout << "Erreur - type d'element non lu" <<endl;
            assert(0);
        }
        nb+=1;
    }
    mesh.sub_mesh(Number<1>()).elem_list.change_hash_size( mesh, mesh.elem_list.size() /2 +1);
    mesh.sub_mesh(Number<2>()).elem_list.change_hash_size( mesh, mesh.elem_list.size() /2 +1);
//     mesh.sub_mesh(Number<0>()).elem_list.change_hash_size( mesh, mesh.elem_list.size() );
}

/*!
put avs mesh castem in m
\keyword Maillage/Import
*/
template<class TM>
void read_geof(TM &m,const std::string &fic_name) throw(std::runtime_error) {
    // ouverture du fichier
    std::ifstream my_file( fic_name.c_str() );
    if ( ! my_file.is_open() )
        throw std::runtime_error( "opening of "+fic_name+" has failed.");
    return read_geof(m,my_file);
}

};
#endif //READ_GEOF_H
