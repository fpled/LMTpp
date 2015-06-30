#ifndef READ_AVS_H
#define READ_AVS_H

#include "mesh.h"
#include "bar.h"
#include "triangle.h"
#include "triangle_6.h"
#include "quad.h"
#include "quad_8.h"
#include "quad_9.h"
#include "tetra.h"
#include "tetra_10.h"
#include "hexa.h"
#include "hexa_20.h"
#include "wedge.h"
#include "../io/normalize_end_line.h"
#include <stdexcept>
#include <map>
using namespace std;

namespace LMT {
/// put avs castem mesh in mesh



struct Totito {
    //     template<class T>
    //     void operator()(unsigned num,const char *name,T &val) const {
    //     }
    template<class T,int s>
    void operator()(unsigned num,const char *name,Vec<T,s> &val) const {
        for(unsigned i=0;i<fname.size();++i)
            if ( name == fname[i] ) {
                val = data[number][i];
            }
    }
    template<class T>
    void operator()(unsigned num,const char *name,T &val) const {
        for(unsigned i=0;i<fname.size();++i)
            if (name==fname[i]) {
                val = T (data[number][i][0]);
            }
    }

    template<class TE, class DD>
    void operator()(TE &e, DD & dd) {
        number = e.number;
        DM::apply_with_names( e, *this );
    }

    int number;
    Vec<string> fname;
    Vec<int> fsize;
    Vec<Vec<Vec<double> > > data;

};
template <class dinamiquedata, class TM >
struct Totito_ELEM {
    Totito_ELEM(dinamiquedata &dd_, TM &mesh_){
        dd = &dd_;
        mesh = &mesh_;
    };
    template<class T>
    void operator()(unsigned num,const char *name,T &val) const {
        for(unsigned i=0;i<fname.size();++i) {
            if ( name == fname[i] ) {
                std::cerr << "Pour l'instant pas codé lecteur de types autre que Tpos, Vec, Vec<Vec> dans read_avs."  << std::endl;
                assert( 0 );
            }
        }
    }
    template<class T,int s1, int s2>
    void operator()(unsigned num,const char *name,Vec<Vec<T,s2>,s1> &val) const {
        for(unsigned i=0;i<fname.size();++i)
            if ( name == fname[i] ) {
                unsigned cpt =0;
                for(unsigned j=0;j<val.size();j++)
                    for(unsigned k=0;k<val[j].size();k++){
                        val[j][k] = data[number][i][cpt];
                        cpt++;
                    }
            }
    }

    template<class T,int s>
    void operator()(unsigned num,const char *name,Vec<T,s> &val) const {
        for(unsigned i=0;i<fname.size();++i)
            if ( name == fname[i] ) {
                val = data[number][i];
                //cout << " changing information of node number " << number << " data "<< name << " value : " << val << endl ;
            }
    }

        void operator()(unsigned num,const char *name,typename TM::Tpos &val) const {
            for(unsigned i=0;i<fname.size();++i)
                if (name==fname[i]) {
                    val = typename TM::Tpos (data[number][i][0]);
                    // cout << " changing info of node number " << number << " data "<< name << " value : " << val << endl ;
                }
        }
            template<class TE>
            void operator()(TE &e) {
                number = mesh->elem_list.get_data(*dd, e);
                DM::apply_with_names( e, *this);
            }
            dinamiquedata *dd;
            TM *mesh;
            int number;
            Vec<string> fname;
            Vec<int> fsize;
            Vec<Vec<Vec<double> > > data;
};

/*!
référence sur le format UCD : 
    http://people.sc.fsu.edu/~jburkardt/data/ucd/ucd.html
    http://people.sc.fsu.edu/~jburkardt/html/ucd_format.html

Rem : L'élément \a Tetra_10 n'est pas spécifié dans le format.
*/
template<class TM>
void read_avs(TM &mesh, std::istream &is) throw(std::runtime_error) {

    //TM mesh;
    typedef typename TM::Tpos T;
    typedef typename TM::Pvec Pvec;
    typedef typename TM::TNode TNode;
    typedef typename TM::EA EA;
    typedef DynamicData<int,TM::TElemList::nb_elem_type> CDC;
    static const int dim = TM::dim;
    map<int,TNode *> map_num_node;

    //is.tellg();
    // correspondance between number in file -> ref in mesh

    // obtaining nbnode, nbelem
    unsigned nbnode;
    unsigned nbelem;
    unsigned nbnode_data;
    unsigned nbelem_data;
    unsigned nbglobal_data;
    {
        string str;
        getline(is,str);
        istringstream s(str);
        s >> nbnode >> nbelem >> nbnode_data >> nbelem_data >> nbglobal_data;
    }
    // reading nodes coordinates
    unsigned nb = 0;
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

    nb=0;
    //int nbnodeelem;
    int nnode_elem;
    string type_elem;
    CDC dd("global_number");
    while(nb<nbelem) {
        string str;
        getline(is,str);
        normalize_end_line( str );
        istringstream s( str );
        int number, number2;
        s >> number; // numero de l'element

        s >> number2; // 1 ou 2

        s >> type_elem;//type dl'element

        if ( type_elem == "line" ) {
            nnode_elem=2;
            Vec<TNode *> vn;
            vn.resize(nnode_elem);
            for(int i=0;i<nnode_elem;i++) {
                s >> number;
                vn[i] = map_num_node[number];
            }
            typename TM::EA *ne = reinterpret_cast<typename TM::EA *>(mesh.add_element(Bar(),DefaultBehavior(),&vn[0]));
            ne->group = number2;
            if(nbelem_data) mesh.elem_list.synchronize_dyn(&dd);
            if(nbelem_data) mesh.elem_list.get_data(dd, *ne) = nb;
        } else if ( type_elem == "tri" ) {
            Vec<TNode *> vn;
            while ( true ) {
                unsigned number;
                s >> number;
                if ( !s )
                    break;
                vn.push_back( map_num_node[number] );
            }
            if ( vn.size()==3 ) {
                if ( dim==2 ) {
                    T o = ((*vn[1]).pos[0]-(*vn[0]).pos[0])*((*vn[2]).pos[1]-(*vn[0]).pos[1]) - ((*vn[1]).pos[1]-(*vn[0]).pos[1])*((*vn[2]).pos[0]-(*vn[0]).pos[0]);
                    if ( o < 0 )
                        swap( vn[0], vn[1] );
                }
                permutation_if_jac_neg(Triangle(),vn.ptr());
                typename TM::EA *ne = reinterpret_cast<typename TM::EA *>(mesh.add_element(Triangle(),DefaultBehavior(),&vn[0]));
                ne->group = number2;
                if(nbelem_data) mesh.elem_list.synchronize_dyn(&dd);
                if(nbelem_data) mesh.elem_list.get_data(dd, *ne) = nb;
            } else if ( vn.size()==6 ) {
                permutation_if_jac_neg(Triangle_6(),vn.ptr());
                typename TM::EA *ne = reinterpret_cast<typename TM::EA *>(mesh.add_element(Triangle_6(),DefaultBehavior(),&vn[0]));
                ne->group = number2;
                if(nbelem_data) mesh.elem_list.synchronize_dyn(&dd);
                if(nbelem_data) mesh.elem_list.get_data(dd, *ne) = nb;
            } else
                throw std::runtime_error("Unknown element...");

        } else if ( type_elem == "quad" ) {
            nnode_elem=4;
            Vec<TNode *> vn;
            vn.resize(nnode_elem);
            for(int i=0;i<nnode_elem;i++) {
                s >> number;
                vn[i] = map_num_node[number];
            }
            permutation_if_jac_neg(Quad(),vn.ptr());
            typename TM::EA *ne = reinterpret_cast<typename TM::EA *>(mesh.add_element(Quad(),DefaultBehavior(),&vn[0]));
            ne->group = number2;
            if(nbelem_data) mesh.elem_list.synchronize_dyn(&dd);
            if(nbelem_data) mesh.elem_list.get_data(dd, *ne) = nb;
        } else if (type_elem=="quad8") {
            nnode_elem=8;
            Vec<TNode *> vn;
            vn.resize(nnode_elem);
            for(int i=0;i<nnode_elem;i++) {
                s >> number;
                vn[i] = map_num_node[number];
            }
            permutation_if_jac_neg(Quad_8(),vn.ptr());
            typename TM::EA *ne = reinterpret_cast<typename TM::EA *>(mesh.add_element(Quad_8(),DefaultBehavior(),&vn[0]));
            ne->group = number2;
            if(nbelem_data) mesh.elem_list.synchronize_dyn(&dd);
            if(nbelem_data) mesh.elem_list.get_data(dd, *ne) = nb;
        } else if (type_elem=="quad9") {
            nnode_elem=9;
            Vec<TNode *> vn;
            vn.resize(nnode_elem);
            for(int i=0;i<nnode_elem;i++) {
                s >> number;
                vn[i] = map_num_node[number];
            }
            permutation_if_jac_neg(Quad_9(),vn.ptr());
            typename TM::EA *ne = reinterpret_cast<typename TM::EA *>(mesh.add_element(Quad_9(),DefaultBehavior(),&vn[0]));
            ne->group = number2;
            if(nbelem_data) mesh.elem_list.synchronize_dyn(&dd);
            if(nbelem_data) mesh.elem_list.get_data(dd, *ne) = nb;
        } else if (type_elem=="tet") {
            EA *ne;
            Vec<TNode *> vn;
            /// lecture de l'indice des noeuds
            while ( not( s.eof() ) ) {
                s >> number;
                vn.push_back( map_num_node[ number ] );
            }
            
            switch( vn.size() ) {
                case 4 : { /// Tetra
                    permutation_if_jac_neg( Tetra(), vn.ptr() );
                    ne = reinterpret_cast<EA *>( mesh.add_element( Tetra(), DefaultBehavior(), &vn[0] ) );
                    break;
                }
                case 10 : { /// Tetra_10
                    Vec<TNode *> vn2 = vn;
                    vn[ 2 ] = vn2[ 9 ];
                    vn[ 3 ] = vn2[ 5 ];
                    vn[ 4 ] = vn2[ 7 ];
                    vn[ 5 ] = vn2[ 3 ];
                    vn[ 6 ] = vn2[ 2 ];
                    vn[ 7 ] = vn2[ 4 ];
                    vn[ 8 ] = vn2[ 6 ];
                    vn[ 9 ] = vn2[ 8 ];       
                    permutation_if_jac_neg( Tetra_10(), vn.ptr() );
                    ne = reinterpret_cast<EA *>( mesh.add_element( Tetra_10(), DefaultBehavior(), &vn[0] ) );                
                    break;
                }
                default : throw std::runtime_error( "Unknown tet element..." );
            }
            //typename TM::template TElem<Tetra,DefaultBehavior>::TE *ne = mesh.add_element(Tetra(),DefaultBehavior(),&vn[0]);
            ne->group = number2;
            if( nbelem_data ) { 
                mesh.elem_list.synchronize_dyn( &dd );
                mesh.elem_list.get_data(dd, *ne) = nb;
            }
        } else if (type_elem=="cub8" or type_elem=="hex") {
            nnode_elem=8;
            Vec<TNode *> vn;
            vn.resize(nnode_elem);
            for(int i=0;i<nnode_elem;i++) {
                s >> number;
                vn[i] = map_num_node[number];
            }
            permutation_if_jac_neg(Hexa(),vn.ptr());
            //typename TM::template TElem<Hexa,DefaultBehavior>::TE *ne = mesh.add_element(Hexa(),DefaultBehavior(),&vn[0]);
            typename TM::EA *ne = reinterpret_cast<typename TM::EA *>(mesh.add_element(Hexa(),DefaultBehavior(),&vn[0]));
            ne->group = number2;
            if(nbelem_data) mesh.elem_list.synchronize_dyn(&dd);
            if(nbelem_data) mesh.elem_list.get_data(dd, *ne) = nb;
        } else if (type_elem=="prism") {
            nnode_elem=6;
            Vec<TNode *> vn;
            vn.resize(nnode_elem);
            for(int i=0;i<nnode_elem;i++) {
                s >> number;
                vn[i] = map_num_node[number];
            }
            permutation_if_jac_neg(Wedge(),vn.ptr());
            //typename TM::template TElem<Wedge,DefaultBehavior>::TE *ne = mesh.add_element(Wedge(),DefaultBehavior(),&vn[0]);
            typename TM::EA *ne = reinterpret_cast<typename TM::EA *>(mesh.add_element(Wedge(),DefaultBehavior(),&vn[0]));
            ne->group = number2;
            if(nbelem_data) mesh.elem_list.synchronize_dyn(&dd);
            if(nbelem_data) mesh.elem_list.get_data(dd, *ne) = nb;
        } else if (type_elem=="cu20") {
            nnode_elem=20;
            Vec<TNode *> vn;
            vn.resize(nnode_elem);
            for(int i=0;i<nnode_elem;i++) {
                s >> number;
                vn[i] = map_num_node[number];
            }
            permutation_if_jac_neg(Hexa_20(),vn.ptr());
            //typename TM::template TElem<Wedge,DefaultBehavior>::TE *ne = mesh.add_element(Wedge(),DefaultBehavior(),&vn[0]);
            typename TM::EA *ne = reinterpret_cast<typename TM::EA *>(mesh.add_element(Hexa_20(),DefaultBehavior(),&vn[0]));
            if(nbelem_data) mesh.elem_list.synchronize_dyn(&dd);
            if(nbelem_data) mesh.elem_list.get_data(dd, *ne) = nb; 
        } else {
            std::cout << "Erreur - type d'element non lu" <<endl;
            assert(0);
        }
        nb+=1;
    }
    mesh.sub_mesh(Number<1>()).elem_list.change_hash_size( mesh, mesh.elem_list.size() / 2 + 1 );
    //mesh.sub_mesh(Number<2>()).elem_list.change_hash_size( mesh, mesh.elem_list.size() /2 +1);



    if(nbnode_data){    //reading node fields
        string str;
        getline(is,str);
        istringstream s(str);

        nb=0;
        unsigned nb_of_node_fields;
        s >> nb_of_node_fields;
//         cout << "nb_of_node_fields " << nb_of_node_fields << endl;
        Vec<unsigned> nb_of_comp;
        nb_of_comp.resize(nb_of_node_fields);
        while(nb<nb_of_node_fields) {
            s >> nb_of_comp[nb];
            ++nb;
        }
//         cout << "nb of comp " << nb_of_comp << endl;
        nb=0;
        Vec<string> node_fields_names;
        Vec<string> node_fields_units;
        node_fields_names.resize(nb_of_node_fields);
        node_fields_units.resize(nb_of_node_fields);

        while(nb<nb_of_node_fields) {
            string str;
            getline(is,str);
            istringstream s(str);
            string buf;
            s >> buf;
            Vec<std::string> split = tokenize( str, ',' );
            node_fields_names[nb] = split[0];
            node_fields_units[nb] = split[1];
            ++nb;
        }
//         cout << "names " << node_fields_names << " " <<  node_fields_units << endl;

        nb=0;
        int number;

        Totito mmmm;
        mmmm.fname = node_fields_names;
        mmmm.data.resize(nbnode);
        for(unsigned i=0;i<nbnode;++i)
            mmmm.data[i].resize(nb_of_node_fields);
        for(unsigned i=0;i<nbnode;++i)
            for(unsigned ii=0;ii<nb_of_node_fields;++ii)
                mmmm.data[i][ii].resize(nb_of_comp[ii]);

        while(nb<nbnode) {
            string str;
            getline(is,str);
            istringstream s(str);
            s >> number;
            int nb2=0;
            Vec<double> val;
            while(nb2<nb_of_node_fields) {
                val.resize(nb_of_comp[nb2]);
                for(unsigned i=0;i<nb_of_comp[nb2];++i) {
                    s >> mmmm.data[nb][nb2][i];
                    //                debug << mmmm.data[nb][nb2][i] << " " ;
                }
                ++nb2;
            }
            ++nb;
        }

        for(unsigned i=0;i<nbnode;++i) {
            mmmm.number = i;
            DM::apply_with_names( mesh.node_list[i], mmmm);
        }
    }


    if(nbelem_data){//reading elem fields
        string str;
        getline(is,str);
        istringstream s(str);

        nb=0;
        int nb_of_elem_fields;
        s >> nb_of_elem_fields;
//         cout << "nb_of_elem_fields " << nb_of_elem_fields << endl;
        Vec<int> nb_of_comp;
        nb_of_comp.resize(nb_of_elem_fields);
        while(nb<nb_of_elem_fields) {
            s >> nb_of_comp[nb];
            ++nb;
        }
        //debug << "nb of comp " << nb_of_comp << endl;
        nb=0;
        Vec<string> elem_fields_names;
        Vec<string> elem_fields_units;
        elem_fields_names.resize(nb_of_elem_fields);
        elem_fields_units.resize(nb_of_elem_fields);

        while(nb<nb_of_elem_fields) {
            string str;
            getline(is,str);
            istringstream s(str);
            string buf;
            s >> buf;
            Vec<std::string> split = tokenize( str, ',' );
            elem_fields_names[nb] = split[0];
            elem_fields_units[nb] = split[1];
            ++nb;
        }
//         cout << "names " << elem_fields_names << " " <<  elem_fields_units << endl;

        nb=0;
        int number;
        Totito_ELEM<CDC,TM> mmmm(dd,mesh);
        mmmm.fname = elem_fields_names;
        mmmm.data.resize(nbelem);
        for(unsigned i=0;i<nbelem;++i)
            mmmm.data[i].resize(nb_of_elem_fields);
        for(unsigned i=0;i<nbelem;++i)
            for(unsigned ii=0;ii<nb_of_elem_fields;++ii)
                mmmm.data[i][ii].resize(nb_of_comp[ii]);

        while(nb<nbelem) {
            string str;
            getline(is,str);
            istringstream s(str);
            s >> number;
            int nb2=0;
            Vec<double> val;
            while(nb2<nb_of_elem_fields) {
                val.resize(nb_of_comp[nb2]);
                for(unsigned i=0;i<nb_of_comp[nb2];++i) {
                    s >> mmmm.data[nb][nb2][i];
                    //                debug << mmmm.data[nb][nb2][i] << endl;
                }
                ++nb2;
            }
            ++nb;
        }

        apply( mesh.elem_list, mmmm);
    }

    if(nbglobal_data){//reading globals fields
        ///nbglobal_data
        string str;
        getline(is,str);
        istringstream s(str);

        nb=0;
        int nb_of_global_fields;
        s >> nb_of_global_fields;
//         cout << "nb_of_global_fields " << nb_of_global_fields << endl;
        Vec<int> nb_of_comp;
        nb_of_comp.resize(nb_of_global_fields);
        while(nb<nb_of_global_fields) {
            s >> nb_of_comp[nb];
            ++nb;
        }
//         cout << "nb of comp " << nb_of_comp << endl;
        nb=0;
        Vec<string> global_fields_names;
        Vec<string> global_fields_units;
        global_fields_names.resize(nb_of_global_fields);
        global_fields_units.resize(nb_of_global_fields);

        while(nb<nb_of_global_fields) {
            string str;
            getline(is,str);
            istringstream s(str);
            string buf;
            s >> buf;
            Vec<std::string> split = tokenize( str, ',' );
            global_fields_names[nb] = split[0];
            global_fields_units[nb] = split[1];
            ++nb;
        }
//         debug << "names " << global_fields_names << " " <<  global_fields_units << endl;

        nb=0;
        int number;

        Totito mmmm;
        mmmm.fname = global_fields_names;
        mmmm.data.resize(nbelem);
        for(unsigned i=0;i<1;++i)
            mmmm.data[i].resize(nb_of_global_fields);
        for(unsigned i=0;i<1;++i)
            for(unsigned ii=0;ii<nb_of_global_fields;++ii)
                mmmm.data[i][ii].resize(nb_of_comp[ii]);

        while(nb<nb_of_global_fields) {
            string str;
            getline(is,str);
            istringstream s(str);
            //val.resize(nb_of_comp[nb2]);
            for(unsigned i=0;i<nb_of_comp[nb];++i) {
                s >> mmmm.data[0][nb][i];
                //                debug << mmmm.data[nb][nb2][i] << endl;
            }
            ++nb;
        }
        mmmm.number=0;
        DM::apply_with_names(mesh, mmmm);
    }
}

/*!
 put avs mesh castem in m
\keyword Maillage/Import
*/
template<class TM>
void read_avs(TM &m,const std::string &fic_name) throw(std::runtime_error) {
    //PRINT( fic_name );
    // ouverture du fichier
    std::ifstream my_file( fic_name.c_str() );
    if ( ! my_file.is_open() )
        throw std::runtime_error( "opening of "+fic_name+" has failed.");
    return read_avs(m,my_file);
}

};
#endif //READ_AVS_H
