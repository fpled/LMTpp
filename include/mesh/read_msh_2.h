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
#ifndef LMT_READ_GID2_HEADER
#define LMT_READ_GID2_HEADER

#include "bar.h"
#include "bar_3.h"
#include "triangle.h"
#include "triangle_6.h"
#include "tetra.h"
#include "tetra_10.h"
#include "hexa.h"
#include "wedge.h"
#include "../containers/indexof.h"

#include <fstream>
#include <map>
#include <sstream>
#include <vector>
#include <stdexcept>

namespace LMT {

/*!
\internal
    Fonction interne

*/
template<class TE,class T,class VarTag,unsigned n,unsigned m>
void assign_tag_values( TE *e, const Vec<T> &tag_values, const VarTag &vt, Number<n>, Number<m> ) {
    ExtractDM<typename VarTag::template SubType<n>::T> ev;
    ev( *e ) = tag_values[ n ];
    assign_tag_values( e, tag_values, vt, Number<n+1>(), Number<m>() );
}
template<class TE,class T,class VarTag,unsigned n           > void assign_tag_values( TE   *e, const Vec<T> &tag_values, const VarTag &vt, Number<n>, Number<n> ) { e->set_field("tag", tag_values[0], false); }
// template<class TE,class T,class VarTag,unsigned n           > void assign_tag_values( TE   *e, const Vec<T> &tag_values, const VarTag &vt, Number<n>, Number<n> ) { Vec<double, 2> tags = tag_values; e->set_field("tags", tags, false); }
// template<class TE,class T,class VarTag,unsigned n           > void assign_tag_values( TE   *e, const Vec<T> &tag_values, const VarTag &vt, Number<n>, Number<n> ) { }
template<         class T, class VarTag,unsigned n,unsigned m> void assign_tag_values( void *e, const Vec<T> &tag_values, const VarTag &vt, Number<n>, Number<m> ) { }
template<         class T, class VarTag,unsigned n           > void assign_tag_values( void *e, const Vec<T> &tag_values, const VarTag &vt, Number<n>, Number<n> ) { }

/// put gmsh mesh in m
template<class TM,class VarTag>
void read_msh_2( TM &m, std::istream &is, unsigned nvi, const VarTag &vt ) throw ( std::runtime_error ) {
    using namespace std;
    typedef typename TM::Pvec Pvec;
    typedef typename TM::TNode TNode;
    static const int dim = TM::dim;
    if ( not nvi )
        nvi = TM::nvi;
    int ctxte = 0;

    // correspondance between number in file -> ref in mesh
    map<int,TNode *> map_num_node;
    string type_element;
    Vec<typename TM::Tpos> tag_values;
    //
    while ( 1 ) {
        if ( ! is )
            throw std::runtime_error ( "msh file is corrupted : mark 'end elements' is absent" );

        string str;
        getline ( is,str );

        // évaluation du contexte
        if ( str.find ( "$EndNodes" ) != string::npos ) {
            ctxte=0;
            continue;
        }
        if ( str.find ( "$EndElements" ) != string::npos ) {
            break;
        }
        if ( str.find ( "$Nodes" ) != string::npos ) {
            getline( is,str );
            ctxte = 1;
            continue;
        }
        if ( str.find ( "$Elements" ) != string::npos ) {
            getline( is,str );
            ctxte = 2;
            continue;
        }

        // utilisation du contexte
        if ( ctxte==1 ) {
            istringstream s( str );
            int number;
            s >> number;
            Pvec vec;
            for ( int di=0;di<dim;di++ )
                s >> vec[di];
            // we add the node and we retain is ref
            map_num_node[number] = m.add_node( vec );
            continue;
        }
        if ( ctxte==2 ) {
            istringstream s ( str );
            unsigned number;
            s >> number;
            unsigned type_elem;
            s >> type_elem;
            unsigned nb_nodes_elem[] = { 2, 3, 4, 4, 8, 6, 5, 3, 6, 9, 10, 27, 18, 14, 1, 8, 20, 15, 13 };
            unsigned nb_nodes;
            nb_nodes = nb_nodes_elem[type_elem-1];
            unsigned nb_tags;
            s >> nb_tags;
            tag_values.resize( nb_tags );
            for ( unsigned k = 0; k<nb_tags; k++ )
                s >> tag_values[k];

            LMT::Vec<TNode *> vn;
            vn.resize ( nb_nodes );
            for ( unsigned i=0;i<nb_nodes;i++ ) {
                s >> number;
                vn[i] = map_num_node[number];
            }

            if ( type_elem == 1 ) { //TODO
                if ( nvi == 1 )
                    assign_tag_values( m.add_element( Bar(),DefaultBehavior(),vn.ptr() ), tag_values, vt, Number<0>(), Number<NbSubTypes<VarTag>::res>() );
            }
            else if ( type_elem == 2 ) { //TODO
                if ( nvi == 2 ) {
                    permutation_if_jac_neg ( Triangle(), vn.ptr() );
                    assign_tag_values( m.add_element( Triangle(),DefaultBehavior(),vn.ptr() ), tag_values, vt, Number<0>(), Number<NbSubTypes<VarTag>::res>() );
                }
            }
            else if ( type_elem == 3 ) { //TODO
                if ( nvi == 2 ) {
                    permutation_if_jac_neg ( Quad(), vn.ptr() );
                    assign_tag_values( m.add_element( Quad(),DefaultBehavior(),vn.ptr() ), tag_values, vt, Number<0>(), Number<NbSubTypes<VarTag>::res>() );
                }
            }
            else if ( type_elem == 4 ) { //TODO
                if ( nvi == 3 ) {
                    permutation_if_jac_neg ( Tetra(), vn.ptr() );
                    assign_tag_values( m.add_element( Tetra(),DefaultBehavior(),vn.ptr() ), tag_values, vt, Number<0>(), Number<NbSubTypes<VarTag>::res>() );
                }
            }
            else if ( type_elem == 5 ) { //TODO
                if ( nvi == 3 ) {
                    permutation_if_jac_neg ( Hexa(), vn.ptr() );
                    assign_tag_values( m.add_element( Hexa(),DefaultBehavior(),vn.ptr() ), tag_values, vt, Number<0>(), Number<NbSubTypes<VarTag>::res>() );
                }
            }
            else if ( type_elem == 6 ) { //TODO
                if ( nvi == 3 ) {
                    permutation_if_jac_neg ( Wedge(), vn.ptr() );
                    assign_tag_values( m.add_element( Wedge(),DefaultBehavior(),vn.ptr() ), tag_values, vt, Number<0>(), Number<NbSubTypes<VarTag>::res>() );
                }
            }
            else if ( type_elem == 8 ) { //TODO
                if ( nvi == 1 ) {
                    assign_tag_values( m.add_element( Bar_3(),DefaultBehavior(),vn.ptr() ), tag_values, vt, Number<0>(), Number<NbSubTypes<VarTag>::res>() );
                }
            }
            else if ( type_elem == 9 ) { //TODO
                if ( nvi == 2 ) {
                    permutation_if_jac_neg ( Triangle_6(), vn.ptr() );
                    assign_tag_values( m.add_element( Triangle_6(),DefaultBehavior(),vn.ptr() ), tag_values, vt, Number<0>(), Number<NbSubTypes<VarTag>::res>() );
                }
            }
            else if ( type_elem == 11 ) { //TODO
                if ( nvi == 3 ) {
                    swap( vn[8], vn[9] );
                    permutation_if_jac_neg ( Tetra_10(), vn.ptr() );
                    assign_tag_values( m.add_element( Tetra_10(),DefaultBehavior(),vn.ptr() ), tag_values, vt, Number<0>(), Number<NbSubTypes<VarTag>::res>() );
                }
            }
            else if ( type_elem == 15 ) { //TODO
            }
            else {
                std::cout << "unknown type elem in read_msh_2 -> " << type_elem << std::endl;
            }

            continue;
        }
    }
}


/*!

    \code
        read_msh_2_tags_for_nvi( m, "toto.msh", Number<1>(), HeteroExplPack<toto_DM,tata_DM>() )

    Soit un maillage de tetra (niveau Number<0> pour rappel).
    Supposons que soit défini pour chaque sous élément triangle, deux atttributs toto, tata
     -> on aura dans les pour chaque element elem
        elem.toto = le tag 1
        elem.tata = le tag 2
      (e.g. densité temperature), la fonction assigne les tags numéros 0 et 1 dans les champs toto et tata (en supposant bien sûr que gmsh à mis au moins 2 tags dans sa liste des sous éléments triangle).

     Exemple: pour lire les tags des triangle dans un maillage de triangles (Number<0> parce que ne les veux pas dans des sous éléments)
        read_msh_2_tags_for_nvi( m, "toto.msh", Number<0>(), HeteroExplPack<toto_DM,tata_DM>() )
 */
// put gid mesh in m
// corresponding sub mesh must be updated. nvi is nb var inter to subs

template<class TM,unsigned nvi_to_subs,class VarTag>
void read_msh_2_tags_for_nvi( TM &m, const std::string &fic_name, Number<nvi_to_subs>, const VarTag &vt ) throw ( std::runtime_error ) {
    // ouverture du fichier
    std::ifstream is( fic_name.c_str() );
    if ( ! is.is_open() )
        throw std::runtime_error( "opening of "+fic_name+" has failed." );
    int mesh_nvi = TM::nvi;
    int wanted_nvi = mesh_nvi - nvi_to_subs;
    // $MeshFormat
    std::string str;
    getline( is, str );
    // 2 x x
    int major;
    is >> major; // 2
    getline( is, str ); // mezzo, minor
    assert( major >= 2 );
    // $EndMeshFormat, $Nodes
    getline( is, str ); // $EndMeshFormat
    getline( is, str ); // $Nodes
    // nodes
    unsigned nb_nodes = 0;
    is >> nb_nodes; //
    Vec<unsigned> corr_nodes; corr_nodes.resize( nb_nodes+1, 0 );
    unsigned offset_nodes = m.node_list.size() - nb_nodes;
    for(unsigned i=0;i<nb_nodes;++i) {
        unsigned num_node = 0;
        is >> num_node;
        getline( is, str );
        //
        if ( num_node >= corr_nodes.size() )
            corr_nodes.resize( num_node + 16, 0 );
        corr_nodes[ num_node ] = offset_nodes + i;
    }
    // $EndNodes
    getline( is, str );
    if ( str != "$EndNodes" )
        throw std::runtime_error( "msh file is corrupted : mark '$EndNodes' is absent" );
    // $Elements
    getline( is, str );
    if ( str != "$Elements" )
        throw std::runtime_error( "msh file is corrupted : mark '$Elements' is absent" );
    // elements
    unsigned nb_elems = 0;
    is >> nb_elems; //
    getline( is, str );
    Vec<typename TM::Tpos> tag_values;
    for(unsigned i=0;i<nb_elems;++i) {
        static unsigned nb_nodes_elem[] = { 2, 3, 4, 4, 8, 6, 5, 3, 6, 9, 10, 27, 18, 14, 1, 8, 20, 15, 13 };
        static unsigned nvi_elem[] = { 0, 1, 2, 2, 3, 3, 3 };
        //
        getline( is, str );
        std::istringstream s( str );
        unsigned number = 0, type_elem = 0, nb_tags = 0;
        s >> number >> type_elem >> nb_tags;
        //         if ( type_elem > 6 )
        //             throw std::runtime_error( "TODO : type_elem > 6..." );
        if ( nvi_elem[ type_elem ] == wanted_nvi ) {
            unsigned nb_nodes = nb_nodes_elem[type_elem-1];
            tag_values.resize( nb_tags );
            for (unsigned k=0;k<nb_tags;++k)
                s >> tag_values[k];
            Vec<typename TM::TNode *> v; v.resize( nb_nodes );
            for(unsigned i=0;i<nb_nodes;++i) {
                unsigned nn;
                s >> nn;
                v[ i ] = &m.node_list[ corr_nodes[ nn ] ];
            }
            if ( type_elem == 1 )
                assign_tag_values( m.sub_mesh( Number<nvi_to_subs>() ).elem_list.find( Bar()     , DefaultBehavior(), m, v.ptr() ), tag_values, vt, Number<0>(), Number<NbSubTypes<VarTag>::res>() );
            else if ( type_elem == 2 )
                assign_tag_values( m.sub_mesh( Number<nvi_to_subs>() ).elem_list.find( Triangle(), DefaultBehavior(), m, v.ptr() ), tag_values, vt, Number<0>(), Number<NbSubTypes<VarTag>::res>() );
            else if ( type_elem == 3 )
                assign_tag_values( m.sub_mesh( Number<nvi_to_subs>() ).elem_list.find( Quad()    , DefaultBehavior(), m, v.ptr() ), tag_values, vt, Number<0>(), Number<NbSubTypes<VarTag>::res>() );
            else if ( type_elem == 4 )
                assign_tag_values( m.sub_mesh( Number<nvi_to_subs>() ).elem_list.find( Tetra()   , DefaultBehavior(), m, v.ptr() ), tag_values, vt, Number<0>(), Number<NbSubTypes<VarTag>::res>() );
            else if ( type_elem == 5 )
                assign_tag_values( m.sub_mesh( Number<nvi_to_subs>() ).elem_list.find( Hexa()    , DefaultBehavior(), m, v.ptr() ), tag_values, vt, Number<0>(), Number<NbSubTypes<VarTag>::res>() );
            else if ( type_elem == 6 )
                assign_tag_values( m.sub_mesh( Number<nvi_to_subs>() ).elem_list.find( Wedge()   , DefaultBehavior(), m, v.ptr() ), tag_values, vt, Number<0>(), Number<NbSubTypes<VarTag>::res>() );
            else {
                std::cout << "unknown type elem." << std::endl;
                assert ( 0 );
            }
        }
    }
}

/// put gid mesh in m
template<class TM,class VarTag>
void read_msh_2_tags_on_skin( TM &m, const std::string &fic_name, const VarTag &vt ) throw ( std::runtime_error ) {
    m.update_skin();
    read_msh_2_tags_for_nvi( m, fic_name, Number<1>(), vt );
}


/// put gid mesh in m
/*!
Attention: importe seulement les Bar(), les Triangle(), les Quad(), les Hexa(), les Tetra() et les Wedge().
*/
template<class TM>
void read_msh_2( TM &m, const std::string &fic_name, unsigned nvi = 0 ) throw ( std::runtime_error ) {
    // ouverture du fichier
    std::ifstream my_file( fic_name.c_str() );
    if ( ! my_file.is_open() )
        throw std::runtime_error( "opening of "+fic_name+" has failed." );
    read_msh_2( m, my_file, nvi, HeteroExplPack<>() );
}

/*!
put gid mesh in m
Attention: importe seulement les Bar(), les Triangle(), les Quad(), les Hexa(), les Tetra() et les Wedge().

read_msh_2( m, "toto.msh", dim, HeteroExplPack<toto_DM,tata_DM>() )

\keyword Maillage/Import
*/
template<class TM,class VarTag>
void read_msh_2( TM &m, const std::string &fic_name, unsigned nvi, const VarTag &vt ) throw ( std::runtime_error ) {
    // ouverture du fichier
    std::ifstream my_file( fic_name.c_str() );
    if ( ! my_file.is_open() )
        throw std::runtime_error( "opening of "+fic_name+" has failed." );
    read_msh_2( m, my_file, nvi, vt );
}


};

#endif // LMT_READ_GID_HEADER

