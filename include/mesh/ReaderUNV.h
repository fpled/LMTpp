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
#include <stdexcept>

#include "mesh.h"
#include "Bar.h"
#include "Bar_3.h"
#include "Triangle.h"
#include "Triangle_6.h"
#include "Quad.h"
#include "Quad_8.h"
#include "Tetra.h"
#include "Tetra_10.h"
#include "Hexa.h"
#include "Hexa_20.h"
#include "Wedge.h"
#include "Wedge_15.h"
#include "../containers/algo.h"

namespace LMT {

/*!

    ref : TODO
*/
template<class TM> 
struct ReaderUNV {    
    typedef typename TM::Pvec Pvec;
    typedef typename TM::Tpos Tpos;
    typedef typename TM::TNode TNode;
    typedef typename TM::TNode::T T;
    typedef typename TM::MA::EA EA;
    static const unsigned dim = TM::dim;
    
    ReaderUNV() {}
    
    ReaderUNV( TM &m,const std::string &fic_name ) throw(std::runtime_error) : m( m ) , fic_name( fic_name) {
        // ouverture du fichier
        my_file.open( fic_name.c_str() );
        if ( ! my_file.is_open() )
            throw std::runtime_error( "opening of "+fic_name+" has failed.");
        parse_unv();
    }
    
    /// read elements
    void read_unv_elements(unsigned nvi=0) {
        int element_lab,fe_descriptor_id,phys_prop_tab_num,mat_prop_tab_num,color,n_nodes;
        std::string str;
        
        if ( not nvi )
            nvi = TM::nvi;
        
        while (true) {
            std::getline(my_file,str);
            std::istringstream s(str);
            s >> element_lab;
            if (element_lab==-1)
            return;
                
            s >> fe_descriptor_id        // read FE descriptor id
            >> phys_prop_tab_num       // (not supported yet)
            >> mat_prop_tab_num        // (not supported yet)
            >> color                   // (not supported yet)
            >> n_nodes;                // read number of nodes on element
            
            std::getline(my_file,str);
            if(fe_descriptor_id==22)//bar_3, on saute une ligne
                std::getline(my_file,str);
            
            s.clear();
            s.str(str);
            
            Vec<TNode *> vn;
            vn.resize(n_nodes);
            for(int i=0;i<n_nodes;i++) {
                if(s.eof()){//pour sauter des lignes (fortran)
                    std::getline(my_file,str);
                    s.clear();
                    s.str(str);
                }
                int number;
                s >> number;
                vn[i]=map_num_node[number];
            }
            if (!my_file) {
            std::cerr << "Unv file is corrupted while reading element data." << std::endl;
            return;
            }

            if (fe_descriptor_id==11){//bar
                if( nvi==1 ){
                    m.add_element(Bar(),DefaultBehavior(),&vn[0]);
                }
            }
            else if (fe_descriptor_id==22){//bar_3
                if( nvi==1 ){
                    swap( vn[1], vn[2] );
                    m.add_element(Bar_3(),DefaultBehavior(),&vn[0]);
                }
            }
            else if (fe_descriptor_id==41){//triangle
                if( nvi==2 ){
                    permutation_if_jac_neg(Triangle(),vn.ptr());
                    m.add_element(Triangle(),DefaultBehavior(),&vn[0]);
                }
            }
            else if (fe_descriptor_id==42){//triangle_6
                if( nvi==2 ){
                    Vec<TNode *> vn2 = vn;
                    vn[ 1 ] = vn2[ 2 ];
                    vn[ 2 ] = vn2[ 4 ];
                    vn[ 3 ] = vn2[ 1 ];
                    vn[ 4 ] = vn2[ 3 ];
                    permutation_if_jac_neg(Triangle_6(),vn.ptr());
                    m.add_element(Triangle_6(),DefaultBehavior(),&vn[0]);
                }
            }
            else if (fe_descriptor_id==44){//quad
                if( nvi==2 ){
                    permutation_if_jac_neg(Quad(),vn.ptr());
                    m.add_element(Quad(),DefaultBehavior(),&vn[0]);
                }
            }
            else if (fe_descriptor_id==45){//quad_8
                if( nvi==2 ){
                    Vec<TNode *> vn2 = vn;
                    vn[ 1 ] = vn2[ 2 ];
                    vn[ 2 ] = vn2[ 4 ];
                    vn[ 3 ] = vn2[ 6 ];
                    vn[ 4 ] = vn2[ 1 ];
                    vn[ 5 ] = vn2[ 3 ];
                    vn[ 6 ] = vn2[ 5 ];
                    vn[ 7 ] = vn2[ 7 ];
                    permutation_if_jac_neg(Quad_8(),vn.ptr());
                    m.add_element(Quad_8(),DefaultBehavior(),&vn[0]);
                }
            }
            else if (fe_descriptor_id==111){//Tetra
                if( nvi==3 ){
                    permutation_if_jac_neg(Tetra(),vn.ptr());
                    m.add_element(Tetra(),DefaultBehavior(),&vn[0]);
                }
            }
            else if (fe_descriptor_id==112){//Wedge
                if( nvi==3 ){
                    permutation_if_jac_neg(Wedge(),vn.ptr());
                    m.add_element(Wedge(),DefaultBehavior(),&vn[0]);
                }
            }
            else if (fe_descriptor_id==113){//Wedge_15
                if( nvi==3 ){
                    Vec<TNode *> vn2 = vn;
                    vn[ 1 ] = vn2[ 2 ];
                    vn[ 2 ] = vn2[ 4 ];
                    vn[ 3 ] = vn2[ 9 ];
                    vn[ 4 ] = vn2[ 11 ];
                    vn[ 5 ] = vn2[ 13 ];
                    vn[ 6 ] = vn2[ 1 ];
                    vn[ 7 ] = vn2[ 5 ];
                    vn[ 8 ] = vn2[ 6 ];
                    vn[ 9 ] = vn2[ 3 ];
                    vn[ 10 ] = vn2[ 7 ];
                    vn[ 11 ] = vn2[ 8 ];
                    vn[ 12 ] = vn2[ 10 ];
                    vn[ 13 ] = vn2[ 14 ];
                    vn[ 14 ] = vn2[ 13 ];
                    permutation_if_jac_neg(Wedge_15(),vn.ptr());
                    m.add_element(Wedge_15(),DefaultBehavior(),&vn[0]);
                }
            }
            else if (fe_descriptor_id==115){//hexa
                if( nvi==3 ){
                    permutation_if_jac_neg(Hexa(),vn.ptr());
                    m.add_element(Hexa(),DefaultBehavior(),&vn[0]);
                }
            }
            else if (fe_descriptor_id==116){//hexa_20
                if( nvi==3 ){
                    Vec<TNode *> vn2 = vn;
                    vn[ 1 ] = vn2[ 2 ];
                    vn[ 2 ] = vn2[ 4 ];
                    vn[ 3 ] = vn2[ 6 ];
                    vn[ 4 ] = vn2[ 12 ];
                    vn[ 5 ] = vn2[ 14 ];
                    vn[ 6 ] = vn2[ 16 ];
                    vn[ 7 ] = vn2[ 18 ];
                    vn[ 8 ] = vn2[ 1 ];
                    vn[ 9 ] = vn2[ 7 ];
                    vn[ 10 ] = vn2[ 8 ];
                    vn[ 11 ] = vn2[ 3 ];
                    vn[ 12 ] = vn2[ 9 ];
                    vn[ 13 ] = vn2[ 5 ];
                    vn[ 14 ] = vn2[ 10 ];
                    vn[ 15 ] = vn2[ 11 ];
                    vn[ 16 ] = vn2[ 13 ];
                    vn[ 17 ] = vn2[ 19 ];
                    vn[ 18 ] = vn2[ 15 ];
                    vn[ 19 ] = vn2[ 17 ];
                    permutation_if_jac_neg(Hexa_20(),vn.ptr());
                    m.add_element(Hexa_20(),DefaultBehavior(),&vn[0]);
                }
            }
            else if (fe_descriptor_id==118){//tetra_10
                if( nvi==3 ){
                    Vec<TNode *> vn2 = vn;
                    vn[ 1 ] = vn2[ 2 ];
                    vn[ 2 ] = vn2[ 4 ];
                    vn[ 3 ] = vn2[ 9 ];
                    vn[ 4 ] = vn2[ 1 ];
                    vn[ 5 ] = vn2[ 3 ];
                    vn[ 6 ] = vn2[ 5 ];
                    vn[ 7 ] = vn2[ 6 ];
                    vn[ 8 ] = vn2[ 7 ];
                    vn[ 9 ] = vn2[ 8 ];
                    permutation_if_jac_neg(Tetra_10(),vn.ptr());
                    m.add_element(Tetra_10(),DefaultBehavior(),&vn[0]);
                }
            }
            else
            std::cerr << "Elements with " << n_nodes << " nodes are not managed in read_unv()." << std::endl;
        }
        return;
    }

    /// read nodes
    void read_unv_nodes( ) {
        int exp_coord_sys_num,disp_coord_sys_num,color,node_lab;
        std::string str;
        while (true) {
            std::getline(my_file,str);
            std::istringstream s(str);
            s >> node_lab;
            if (node_lab==-1)
            break;
            
            s >> exp_coord_sys_num       // (not supported yet)
            >> disp_coord_sys_num      // (not supported yet)
            >> color;                   // (not supported yet)

            std::getline(my_file,str);

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

    /// read group
    void read_unv_groups( ) {
        int type,label,num_in,id_group,tmp0,tmp1,tmp2,tmp3,tmp4,tmp5,id;
        std::string name_group;
        std::string str;
        
        while(true){
            std::getline(my_file,str);
            std::istringstream s(str);
            s >> id_group ;
            if (id_group==-1)          
                return;
            
            s >> tmp0 >> tmp1 >> tmp2 >> tmp3 >> tmp4 >> tmp5 >> num_in; //
            
            map_idGroup_idNode.resize(id_group);
            map_idGroup_idNode[id_group-1].resize(num_in);
            
            std::getline(my_file,str);
            s.clear();
            s.str(str);
            s >> name_group;
            
            name_groups.resize(id_group);
            name_groups[id_group-1]=name_group;

            id=-1;
            while (true) {//attention, 2 elements par ligne
                id+=1;
                if(id>=num_in) break;
                std::getline(my_file,str);
                s.clear();
                s.str(str);
                s >> type;
                s >> map_idGroup_idNode[id_group-1][id] >> tmp0 >> tmp1;
                id+=1;
                if(id>=num_in) break;
                s >> type;
                s >> map_idGroup_idNode[id_group-1][id];
            }
        }
    }
    
    /// put unv mesh in m, read groups
    void parse_unv( ) {
        //
        int old,news=0;
        while (true) {
            std::string str;
            do {
            old = news;
            std::getline(my_file,str);
            if ( !my_file )
                return;
            std::istringstream s(str);
            s >> news;
            } while ((news!=-1));
            std::getline(my_file,str);
            std::istringstream s(str);
            s >> news;
            switch (news) {
                case (2411):
                    std::cout<<"\tLecture des noeuds"<<std::endl;
                    read_unv_nodes( );
                    break;
                case (2412):
                    std::cout<<"\tLecture des elements"<<std::endl;
                    read_unv_elements( );
                    break;
                case (2467):
                    std::cout<<"\tLecture des groupes"<<std::endl;
                    read_unv_groups( );
                    break;
    //       case (55):
    //         read_unv_results(m,is,map_num_node);
    //         break;
            }
            news = -1;
        }
    }
    
    ///Get LMT ids of node in group
    int get_index_node_of_node_set(Vec<unsigned> &id_node_lmt,const std::string &name_group){
        int id_group=-1;
        for(int i=0;i<name_groups.size();i++){
            if(name_groups[i]==name_group)
                id_group=i;
        }
        if(id_group==-1) return 1;
        
        for(unsigned i=0;i<map_idGroup_idNode[id_group].size();++i){
            id_node_lmt.push_back(map_num_node[map_idGroup_idNode[id_group][i]]->number);
        }
        return 0;
    }
    
    /*!
    Cette méthode retourne les element skin du maillage "mesh" qui appartiennent à la surface nommé "nameSurface".
    */
    template<unsigned nvi_to_subs>
    int get_element_skin_of_surface( Vec<EA* > &list_el,const std::string &name_group, Number<nvi_to_subs> ) {
        
        
        unsigned nvi = TM::nvi;
        int element_lab,fe_descriptor_id,phys_prop_tab_num,mat_prop_tab_num,color,n_nodes;
        std::string str;
        
        int id_group=-1;
        for(int i=0;i<name_groups.size();i++){
            if(name_groups[i]==name_group)
                id_group=i;
        }
        if(id_group==-1) return 0;
        
        my_file.clear();
        my_file.seekg(0,std::ios::beg);
        
        //il faut reparcourir le fichier jusqu'au element
        int old,news=0;
        bool init=true;
        while (true) {
            std::string str;
            do {
                old = news;
                std::getline(my_file,str);
                if ( !my_file ){
                    return 0;
                }
                std::istringstream s(str);
                s >> news;
            } while ((news!=-1));
            std::getline(my_file,str);
            if(not init)
                std::getline(my_file,str);
            std::istringstream s(str);
            s >> news;
            if(news==2412)
                break;
            news = -1;
            init=false;
        }
        
        Vec<int> id_el = map_idGroup_idNode[id_group];
        sort( id_el );
        
        //on parcourt la liste des elements taggé 
        //(attention hypothese range par ordre croissant) 
        for(unsigned i=0;i<id_el.size();++i){
            while (true) {
                std::getline(my_file,str);
                std::istringstream s(str);
                s >> element_lab;
                if (element_lab==-1)
                return 1;
                    
                s >> fe_descriptor_id        // read FE descriptor id
                >> phys_prop_tab_num       // (not supported yet)
                >> mat_prop_tab_num        // (not supported yet)
                >> color                   // (not supported yet)
                >> n_nodes;                // read number of nodes on element
                
                std::getline(my_file,str);
                if(fe_descriptor_id==22)//bar_3, on saute une ligne
                    std::getline(my_file,str);
                
                if( element_lab == id_el[i] ){
                
                    s.clear();
                    s.str(str);
                
                    //on vient juste lire les numéros de noeuds et on transpose en noeud LMT 
                    Vec<TNode *> vn;
                    vn.resize(n_nodes);
                    for(int i=0;i<n_nodes;i++) {
                        if(s.eof()){//pour sauter des lignes (fortran)
                            std::getline(my_file,str);
                            s.clear();
                            s.str(str);
                        }
                        int number;
                        s >> number;
                        vn[i]=map_num_node[number];
                    }
                    
                    //on recherche l'element de surface du maillage qui correspond
                    if (fe_descriptor_id==11){//bar
                        if( ( nvi - nvi_to_subs ) == 1 ){
                            list_el.push_back(reinterpret_cast<EA*>(m.sub_mesh( Number<nvi_to_subs>() ).elem_list.find( Bar() , DefaultBehavior(), m, &vn[0] )));
                        }
                    }
                    else if (fe_descriptor_id==22){//bar_3
                        if( ( nvi - nvi_to_subs ) == 1 ){
                            swap( vn[1], vn[2] );
                            list_el.push_back(reinterpret_cast<EA*>(m.sub_mesh( Number<nvi_to_subs>() ).elem_list.find( Bar_3() , DefaultBehavior(), m, &vn[0] )));
                        }
                    }
                    else if (fe_descriptor_id==41){//triangle
                        if( ( nvi - nvi_to_subs )==2 ){
                            list_el.push_back(reinterpret_cast<EA*>(m.sub_mesh( Number<nvi_to_subs>() ).elem_list.find( Triangle() , DefaultBehavior(), m, &vn[0] )));
                        }
                    }
                    else if (fe_descriptor_id==42){//triangle_6
                        if( ( nvi - nvi_to_subs )==2 ){
                            Vec<TNode *> vn2 = vn;
                            vn[ 1 ] = vn2[ 2 ];
                            vn[ 2 ] = vn2[ 4 ];
                            vn[ 3 ] = vn2[ 1 ];
                            vn[ 4 ] = vn2[ 3 ];
                            list_el.push_back(reinterpret_cast<EA*>(m.sub_mesh( Number<nvi_to_subs>() ).elem_list.find( Triangle_6() , DefaultBehavior(), m, &vn[0] )));
                        }
                    }
                    else if (fe_descriptor_id==44){//quad
                        if( ( nvi - nvi_to_subs )==2 ){
                            list_el.push_back(reinterpret_cast<EA*>(m.sub_mesh( Number<nvi_to_subs>() ).elem_list.find( Quad() , DefaultBehavior(), m, &vn[0] )));
                        }
                    }
                    else if (fe_descriptor_id==45){//quad_8
                        if( ( nvi - nvi_to_subs )==2 ){
                            Vec<TNode *> vn2 = vn;
                            vn[ 1 ] = vn2[ 2 ];
                            vn[ 2 ] = vn2[ 4 ];
                            vn[ 3 ] = vn2[ 6 ];
                            vn[ 4 ] = vn2[ 1 ];
                            vn[ 5 ] = vn2[ 3 ];
                            vn[ 6 ] = vn2[ 5 ];
                            vn[ 7 ] = vn2[ 7 ];
                            list_el.push_back(reinterpret_cast<EA*>(m.sub_mesh( Number<nvi_to_subs>() ).elem_list.find( Quad_8() , DefaultBehavior(), m, &vn[0] )));
                        }
                    }else if (fe_descriptor_id==111){//Tetra
                        if( ( nvi - nvi_to_subs )==3 ){
                            list_el.push_back(reinterpret_cast<EA*>(m.sub_mesh( Number<nvi_to_subs>() ).elem_list.find( Tetra() , DefaultBehavior(), m, &vn[0] )));
                        }
                    }
                    else if (fe_descriptor_id==112){//Wedge
                        if( ( nvi - nvi_to_subs )==3 ){
                            list_el.push_back(reinterpret_cast<EA*>(m.sub_mesh( Number<nvi_to_subs>() ).elem_list.find( Wedge() , DefaultBehavior(), m, &vn[0] )));
                        }
                    }
                    else if (fe_descriptor_id==113){//Wedge_15
                        if( ( nvi - nvi_to_subs )==3 ){
                            Vec<TNode *> vn2 = vn;
                            vn[ 1 ] = vn2[ 2 ];
                            vn[ 2 ] = vn2[ 4 ];
                            vn[ 3 ] = vn2[ 9 ];
                            vn[ 4 ] = vn2[ 11 ];
                            vn[ 5 ] = vn2[ 13 ];
                            vn[ 6 ] = vn2[ 1 ];
                            vn[ 7 ] = vn2[ 5 ];
                            vn[ 8 ] = vn2[ 6 ];
                            vn[ 9 ] = vn2[ 3 ];
                            vn[ 10 ] = vn2[ 7 ];
                            vn[ 11 ] = vn2[ 8 ];
                            vn[ 12 ] = vn2[ 10 ];
                            vn[ 13 ] = vn2[ 14 ];
                            vn[ 14 ] = vn2[ 13 ];
                            list_el.push_back(reinterpret_cast<EA*>(m.sub_mesh( Number<nvi_to_subs>() ).elem_list.find( Wedge_15() , DefaultBehavior(), m, &vn[0] )));
                        }
                    }
                    else if (fe_descriptor_id==115){//hexa
                        if( ( nvi - nvi_to_subs )==3 ){
                            list_el.push_back(reinterpret_cast<EA*>(m.sub_mesh( Number<nvi_to_subs>() ).elem_list.find( Hexa() , DefaultBehavior(), m, &vn[0] )));
                        }
                    }
                    else if (fe_descriptor_id==116){//hexa_20
                        if( ( nvi - nvi_to_subs )==3 ){
                            Vec<TNode *> vn2 = vn;
                            vn[ 1 ] = vn2[ 2 ];
                            vn[ 2 ] = vn2[ 4 ];
                            vn[ 3 ] = vn2[ 6 ];
                            vn[ 4 ] = vn2[ 12 ];
                            vn[ 5 ] = vn2[ 14 ];
                            vn[ 6 ] = vn2[ 16 ];
                            vn[ 7 ] = vn2[ 18 ];
                            vn[ 8 ] = vn2[ 1 ];
                            vn[ 9 ] = vn2[ 7 ];
                            vn[ 10 ] = vn2[ 8 ];
                            vn[ 11 ] = vn2[ 3 ];
                            vn[ 12 ] = vn2[ 9 ];
                            vn[ 13 ] = vn2[ 5 ];
                            vn[ 14 ] = vn2[ 10 ];
                            vn[ 15 ] = vn2[ 11 ];
                            vn[ 16 ] = vn2[ 13 ];
                            vn[ 17 ] = vn2[ 19 ];
                            vn[ 18 ] = vn2[ 15 ];
                            vn[ 19 ] = vn2[ 17 ];
                            list_el.push_back(reinterpret_cast<EA*>(m.sub_mesh( Number<nvi_to_subs>() ).elem_list.find( Hexa_20() , DefaultBehavior(), m, &vn[0] )));
                        }
                    }
                    else if (fe_descriptor_id==118){//tetra_10
                        if( ( nvi - nvi_to_subs )==3 ){
                            Vec<TNode *> vn2 = vn;
                            vn[ 1 ] = vn2[ 2 ];
                            vn[ 2 ] = vn2[ 4 ];
                            vn[ 3 ] = vn2[ 9 ];
                            vn[ 4 ] = vn2[ 1 ];
                            vn[ 5 ] = vn2[ 3 ];
                            vn[ 6 ] = vn2[ 5 ];
                            vn[ 7 ] = vn2[ 6 ];
                            vn[ 8 ] = vn2[ 7 ];
                            vn[ 9 ] = vn2[ 8 ];
                            list_el.push_back(reinterpret_cast<EA*>(m.sub_mesh( Number<nvi_to_subs>() ).elem_list.find( Tetra_10() , DefaultBehavior(), m, &vn[0] )));
                        }
                    }
                    break;
                }
                if (!my_file) {
                std::cerr << "Unv file is corrupted while reading element data." << std::endl;
                return 0;
                }
            }
        }
        return list_el.size();
    }
    
    /// attributs :
    TM &m;
    const std::string &fic_name;
    std::ifstream my_file;
    // correspondance between number in file -> ref in mesh
    std::map<int,TNode *> map_num_node;
    Vec<Vec<int> > map_idGroup_idNode;
    Vec<std::string> name_groups;
};
    
} //namespace LMT

#endif // READ_unv_H
