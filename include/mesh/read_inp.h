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
#ifndef LMT_READ_INP_HEADER
#define LMT_READ_INP_HEADER

#include "tetra.h"
#include "tetra_10.h"
#include "hexa.h"
#include "wedge.h"
#include "../containers/indexof.h"

#include <fstream>
#include <map>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>

namespace LMT {

/*!
    Cette fonction lit un fichier ABAQUS puis construit le maillage m.
    <strong> ATTENTION </strong> : pour l'instant on ne traite que les noeuds et les éléments de type C3D4.
    \keyword Maillage/Import
*/
template<class TM> void read_inp( TM &m, const std::string &fic_name ) {
    typedef typename TM::Tpos T;
    typedef int S;
    static const int dim = TM::dim;
    
    typedef enum {
        ReadingNothing,
        ReadingNode,
        ReadingHexa,
        ReadingWedge,
        ReadingTetra
    } ReadingCtxt;
    
    std::ifstream is( fic_name.c_str() );
    
    std::string line;
    ReadingCtxt ctxt = ReadingNothing;
    int nb_nodes = 0;
    int num_sub_elem_lst = -1;
    std::map<S,S> map_num_node;
    Vec<unsigned> con;
    //
    while ( true ) {
        std::getline( is, line );
        if ( not is )
            break;
        // context change ?
        if ( line.size() > 1 and line[0] == '*' ) {
            if ( line.find( "Instance" ) == 1 or line.find( "INSTANCE" ) == 1 ) {
                // m = meshes.new_elem();
                ctxt = ReadingNothing;
            } else if ( line.find( "Node" ) == 1 or line.find( "NODE" ) == 1 ) {
                ctxt = ReadingNode;
                //if ( not m )
                //    m = meshes.new_elem();
            } else if ( line.find( "Element" ) == 1 or line.find( "ELEMENT" ) == 1) {
                //if ( not m )
                //    m = meshes.new_elem();
                //
                int type_pos = line.find( "type=" );
                if ( type_pos < 0 )
                    type_pos = line.find( "TYPE=" );
                if ( type_pos < 0 )
                    throw "Pb inp";
                std::string type_elem = read_until_sp( line, type_pos + 5 );
                //
                if ( int( type_elem.find( "C3D8" ) ) >= 0 ) {
                    ctxt = ReadingHexa;
                } else if ( int( type_elem.find( "C3D6" ) ) >= 0 ) {
                    ctxt = ReadingWedge;
                } else if ( int( type_elem.find( "C3D4" ) ) >= 0 ) {
                    ctxt = ReadingTetra;
                } else {
                    std::cerr << "in InpReader, unmanaged type " + type_elem << std::endl;
                    ctxt = ReadingNothing;
                }
            } else if ( line[1] == '*' ) {
                // comment
            } else
                ctxt = ReadingNothing;
        } else if ( ctxt == ReadingNode ) {
            rm_commas( line );
            std::istringstream ss( line );
            int n;
            Vec<T,dim> pos;
            ss >> n >> pos;
            assert( ss );
            map_num_node[ n ] = m.node_list.size();
            m.add_node( pos );
        } else if ( ctxt == ReadingTetra ) {
            rm_commas( line ); std::istringstream ss( line ); int n;
            con.resize( 0 ); ss >> n >> con;
            for(unsigned i=0;i<con.size();++i) con[i] = map_num_node[ con[i] ];
            m.add_element( Tetra(), DefaultBehavior(), con.ptr() );
        } else if ( ctxt == ReadingWedge ) {
            rm_commas( line ); std::istringstream ss( line ); int n;
            con.resize( 0 ); ss >> n >> con;
            for(unsigned i=0;i<con.size();++i) con[i] = map_num_node[ con[i] ];
            m.add_element( Wedge(), DefaultBehavior(), con.ptr() );
        } else if ( ctxt == ReadingHexa ) {
            rm_commas( line ); std::istringstream ss( line ); int n;
            con.resize( 0 ); ss >> n >> con;
            for(unsigned i=0;i<con.size();++i) con[i] = map_num_node[ con[i] ];
            m.add_element( Hexa(), DefaultBehavior(), con.ptr() );
        }
    }
//     using namespace std;
//     typedef typename TM::Pvec Pvec;
//     typedef typename TM::TNode TNode;
//     static const int dim = TM::dim;
//     string str;
//     int number,nb_nodes;
//     Pvec vec;
//     LMT::Vec<TNode *> vn;
//     // correspondance between number in file -> ref in mesh
//     map<int,TNode *> map_num_node;
// 
//     str.reserve(512);
//     // ouverture du fichier
//     std::ifstream is( fic_name.c_str() );
//     if ( ! is.is_open() )
//         throw std::runtime_error( "opening of "+fic_name+" has failed." );
// 
//     // état de l'automate
//     int ctxte=0;
// 
//     while ( true ) {
// 
//         str.clear();
//         getline ( is,str );
//         if ( !is.good() )
//             break;
// 
//         // évaluation du contexte
//         if (str[0] == '*') {
//             if ( str.find ( "Node" ) < str.size() ) {
//                 ctxte = 1;
//                 continue;
//             }
//             if ( str.find ( "Element" ) < str.size() ) {
//                 ctxte = 2;
//                 nb_nodes = 4; /// ATTENTION : pour l'instant on ne parse que les éléments de type C3D4 i.e. probablement de type tétraèdre
//                 vn.resize ( nb_nodes );
//                 continue;
//             } else {
//                 ctxte = 0; // on ignorera ce bloc
//                 continue;
//             }
//         }
// 
//         // utilisation du contexte
//         if ( ctxte==1 ) { /// node
//             replace(str,',',' ');
//             istringstream s( str );
//             s >> number;
//             for ( int di=0;di<dim;di++ )
//                 s >> vec[di];
//             // we add the node and we retain is ref
//             map_num_node[number] = m.add_node( vec );
//             continue;
//         }
//         if ( ctxte==2 ) { /// seulement les éléments de type C3D4
//             replace(str,',',' ');
//             istringstream s( str );
//             s >> number;
//             for ( unsigned i=0;i<nb_nodes;i++ ) {
//                 s >> number;
//                 vn[i] = map_num_node[number];
//             }
//             permutation_if_jac_neg ( Tetra(), vn.ptr() );
//             m.add_element( Tetra(),DefaultBehavior(),vn.ptr() );
//             continue;
//         }
//     }
}

}
#endif // LMT_READ_INP_HEADER

