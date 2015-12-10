//
// Description: Lecture d'un fichier bukdata genere par le logiciel gmsh en utilisant l'option FREEFIELDS ou LONGFIELDS
//
//
// Author: %{Violeau} <%{EMAIL}>, (C) %{06/09}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMT_READ_BDF_HEADER
#define LMT_READ_BDF_HEADER

#include "Bar.h"
#include "Triangle.h"
#include "Tetra.h"
#include "Hexa.h"
#include "Wedge.h"

#include <fstream>
#include <map>
#include <sstream>
#include <vector>
#include <stdexcept>

using namespace std;

namespace LMT {

inline void assign_num_group( void *e, const int tag) {

}

template<class TE> void assign_num_group( TE *e, const int tag) {
        (*e).num_group=tag;
}

/// lire un fichier bdf (bulkdata) genere par gmsh (option freefield ou longfield) dans un maillage LMT note m 
template<class TM>
void read_bdf(TM &m,std::istream &is) throw(std::runtime_error) {
    using namespace std;
    typedef typename TM::Pvec Pvec;
    typedef typename TM::TNode TNode;
    static const int dim = TM::dim;

    int ctxte=0;
    bool longfields=0;
    // correspondance entre le numero du noeud dans le fichier et la reference dans le maillage
    map<int,TNode *> map_num_node;
    string type_element;

    while (1) {
        if (! is )
            throw std::runtime_error("bdf file is corrupted ");

        string str2;
        getline(is,str2);

        // Evaluation du contexte
        if (str2.find("$")<str2.size() ) {
            ctxte=0;//ligne de commentaire
            continue;
        }
        if (str2.find("ENDDATA")<str2.size()){
            break;//fin du fichier
        }
        else if(str2.find("GRID*")<str2.size()) {
            ctxte=3;//noeud option longfield
            //modification de la ligne si option LONGFIELDS
            size_t found=str2.find("*N");;
            if(found<str2.size()){
                string rep=str2.substr(found);//extration de *Ni et effacement
                str2.replace(found,str2.length()," ");
                string str3;//recherche dans la ligne suivante
                getline(is,str3);
                //verification si *Ni est bien dans la ligne suivante
                if(str3.find(rep)<str3.size()){
                    string extract=str3.substr (rep.size(),str3.size());//extraction de ce qu'il y a apres *Ni
                    str2.append(extract);
                }
            }
            
        }
        else if(str2.find("GRID")<str2.size()) {
            ctxte=1;//noeud option freefields
            rm_commas(str2);
        }
        
	else
	{
            ctxte=2;//element
            rm_commas(str2);
	}
	
        
        //Separation des cas en fonction du contexte
        if (ctxte==1 or ctxte==3) {
            istringstream s(str2);
            string name;
            s >>name;
            int number;
            s >> number;
            int type;
            s >> type;
            Pvec vec;
            for(unsigned i=0;i<dim;++i)
                s >> vec[i];
            map_num_node[number] = m.add_node(vec);
            continue;
        }
        if(ctxte==2){
            istringstream s(str2);
            //lecture du type d'element, de son numero, de sa caracteristique materiau, de ses noeuds
            string type;
            s>>type;
            int num;
            s >> num;
            int carac;
            s>>carac;
            if(type=="CBAR"){
                LMT::Vec<TNode *> vn; vn.resize(2);
                for(int i=0;i<2;i++) {
                int number;
                s >> number;
                vn[i] = map_num_node[number];
                }
                //assign_num_group(m.add_element(Bar(),DefaultBehavior(),vn.ptr()),carac);
            }
            else if(type=="CTRIA3"){
                LMT::Vec<TNode *> vn; vn.resize(3);
                for(int i=0;i<3;i++) {
                        int number;
                        s >> number;
                        vn[i] = map_num_node[number];
                }
                permutation_if_jac_neg(Triangle(),vn.ptr());
                //assign_num_group(m.add_element(Triangle(),DefaultBehavior(),vn.ptr()),carac);
            }
            else if(type=="CTETRA"){
                LMT::Vec<TNode *> vn; vn.resize(4);
                for(int i=0;i<4;i++) {
                    int number;
                    s >> number;
                    vn[i] = map_num_node[number];
                }
                permutation_if_jac_neg(Tetra(),vn.ptr());
                //assign_num_group(m.add_element(Tetra(),DefaultBehavior(),vn.ptr()),carac);
            }
            continue;
        }
    }
}


/// put bdf mesh in m
template<class TM>
void read_bdf(TM &m,const std::string &fic_name) throw(std::runtime_error) {
    // ouverture du fichier
    std::ifstream my_file( fic_name.c_str() );

    if ( ! my_file.is_open() )
        throw std::runtime_error( "opening of "+fic_name+" has failed.");
    return read_bdf(m,my_file);
}


};

#endif // LMT_READ_BDF_HEADER

