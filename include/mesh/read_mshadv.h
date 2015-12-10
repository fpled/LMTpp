#include "Triangle.h"
#include "Triangle_6.h"
#include "Tetra.h"
#include "Hexa.h"
#include "Wedge.h"
#include <stdexcept>
#include <map>
using namespace std;

namespace LMT {
/// put pantheon mesh from the adventure project in mesh and only tetra...
template<class TM>
void read_mshadv(TM &mesh, std::istream &is) throw(std::runtime_error) {

    //TM mesh;
    typedef typename TM::Tpos T;
    typedef typename TM::Pvec Pvec;
    typedef typename TM::TNode TNode;
    static const int dim = TM::dim;
    map<int,TNode *> map_num_node;


    // obtaining nbelem
    string str;
    getline(is,str);
    istringstream s1(str);
    int nbelem;
    s1 >> nbelem;
//     cout << nbelem << " elements" << endl;

    //boucle dans le vide
    for( unsigned i=0;i<(unsigned)nbelem ;i++ ){
      getline(is,str);
    }
    
    //obtaining nbnode
    getline(is,str);
    istringstream s2(str);
    int nbnode;
    s2 >> nbnode;
//     cout << nbnode << " noeuds" << endl;
    
    is.seekg (0, ios::beg);
    getline(is,str);//on relit la 1ere ligne ! donc le nbelem

//     cout << "lecture des nodes utiles" << endl;
    // reading elem
    Vec<bool> NodeList;
    NodeList.resize(nbnode);
    NodeList.set(0);
    for( unsigned i=0;i<(unsigned)nbelem ;i++ ){
      getline(is,str);
      istringstream s(str);
      int number;
      for(int j=0;j<4;j++) {
        s >> number;
        NodeList[number]=1;
      }
    }
//     cout << "lecture des nodes" << endl;
    getline(is,str);//on relit le nbnode
    // reading nodes coordinates
    int nb=0;
    while(nb<nbnode and mesh.node_list.size() < NodeList.size()) {
        string str;
        getline(is,str);
        istringstream s(str);
        //int number;
        //s >> number;
        //s=tokenize(str);
        if (NodeList[nb]==1){
        Vec<double,dim> vec;
        for(unsigned di=0;di<(unsigned)dim;di++)
            s >> vec[di];

        // we add the node and we retain is ref
        map_num_node[nb] = mesh.add_node(vec);
        }
        nb=nb+1;
    }
//     cout << "on add les elements" << endl;
    is.seekg (0, ios::beg);
    getline(is,str);//on relit la 1ere ligne ! donc le nbelem
    nb=0;
    //int nbnodeelem;
    //int nnode_elem;
    //string type_elem;
    while(nb<nbelem) {
        string str;
        getline(is,str);
        istringstream s(str);
        int number;
        Vec<TNode *> vn;
        vn.resize(4);
        for(int i=0;i<4;i++) {
            s >> number;
            vn[i] = map_num_node[number];
        }
        permutation_if_jac_neg(Tetra(),vn.ptr());
        mesh.add_element(Tetra(),DefaultBehavior(),&vn[0]);
        nb+=1;
    }
    NodeList.free();
    mesh.sub_mesh(Number<1>()).elem_list.change_hash_size( mesh, mesh.elem_list.size() /2 );
    mesh.sub_mesh(Number<2>()).elem_list.change_hash_size( mesh, mesh.elem_list.size() /2 );
//     cout << "fin mail, nb noeud vrai : " << mesh.node_list.size() << endl;
//     mesh.update_skin();
//     cout << "fin update_skin" << endl;
//     DisplayParaview dp;
//     dp.add_mesh(mesh.skin,"./tmp/paraviewskin",Vec<string>(""));
//     dp.add_mesh(mesh,"./tmp/paraviewcompletv2",Vec<string>(""));
//     dp.exec();
}
/*!
\keyword Maillage/Import
*/
template<class TM>
void read_mshadv(TM &m,const std::string &fic_name) throw(std::runtime_error) {
    // ouverture du fichier
    std::ifstream my_file( fic_name.c_str() );
    if ( ! my_file.is_open() )
        throw std::runtime_error( "opening of "+fic_name+" has failed.");
    return read_mshadv(m,my_file);
}

};

