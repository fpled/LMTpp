#ifndef READ_VTK_H
#define READ_VTK_H

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
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace LMT {

template<class T> void swap_byte_order( T &d ) {
    char *data = (char *)&d;
    for( unsigned i=0; i<sizeof(T)/2; ++i )
        std::swap( data[ i ], data[ sizeof(T) - i - 1 ] );
}

struct GetVtkFields {
    template<class TE> void operator()(TE &e,std::istream &is,const std::string &name,unsigned d) const {
        double tmp[120];
        for(unsigned i=0;i<d;++i) {
            is.read( (char *)(tmp+i), sizeof(double) );
            swap_byte_order( tmp[i] );
        }
        //if ( name == "num" )
        //    e.num = tmp[0];
    }
};


/// put vtk mesh in mesh
template<class TM>
void read_vtk(TM &mesh, std::istream &is) throw(std::runtime_error) {
    using namespace std;
    
    //TM mesh;
    typedef typename TM::Tpos T;
    typedef typename TM::Pvec Pvec;
    typedef typename TM::TNode TNode;
    static const int dim = TM::dim;
    unsigned num_first_node = mesh.node_list.size(), nb_cells = 0;
    Vec<int> cell_connectivity_data;
    
    while ( true ) {
        string str, tmp_string;
        getline( is, str );
        if ( not is ) {
            std::cerr << "Apparemment, ce n'est pas un fichier .vtk que tu m'as donné à bouffer." << std::endl;
            return;
        }
        // PRINT(str);
        if ( str.find("POINTS") < str.size() ) {
            std::istringstream ss( str );
            unsigned nb_points;
            ss >> tmp_string >> nb_points >> tmp_string;
            if ( tmp_string != "double" )
                std::cerr << "TODO : type != double. " << __FILE__ << " " << __LINE__ << std::endl;
            for(unsigned i=0;i<nb_points;++i) {
                Pvec pos;
                double tmp_double;
                for(int d=0;d<dim;++d) { is.read( (char *)&tmp_double, sizeof(double) ); pos[d] = tmp_double; }
                for(int d=dim;d<3;++d) is.read( (char *)&tmp_double, sizeof(double) );
                for(int d=0;d<dim;++d)
                    swap_byte_order( pos[d] );
                mesh.add_node( pos );
                // PRINT( pos );
            }
        }
        else if ( str.find("CELLS") < str.size() ) {
            std::istringstream ss( str );
            unsigned nb_data;
            ss >> tmp_string >> nb_cells >> nb_data;
            cell_connectivity_data.resize( nb_data );
            is.read( (char *)cell_connectivity_data.ptr(), sizeof(int) * nb_data );
            for(unsigned i=0;i<nb_data;++i)
                swap_byte_order( cell_connectivity_data[i] );
            // PRINT( cell_connectivity_data );
        }
        else if ( str.find("CELL_TYPES") < str.size() ) {
            for(unsigned i=0,cpt=0;i<nb_cells;++i) {
                int type_cell;
                is.read( (char *)&type_cell, sizeof(int) );
                swap_byte_order( type_cell );
                // PRINT( type_cell );
                unsigned nb_points_in_element = cell_connectivity_data[cpt++];
                for(unsigned j=0;j<nb_points_in_element;++j)
                    cell_connectivity_data[cpt+j] += num_first_node;
                if ( type_cell == 10 ) { // Tetra
                    assert( nb_points_in_element == 4 );
                    mesh.add_element( Tetra(), DefaultBehavior(), (unsigned *)cell_connectivity_data.ptr() + cpt );
                }
                else if ( type_cell == 24 ) { // Tetra_10
                    assert( nb_points_in_element == 10 );
                    mesh.add_element( Tetra_10(), DefaultBehavior(), (unsigned *)cell_connectivity_data.ptr() + cpt );
                }
                else if ( type_cell == 5 ) { // Triangle
                    assert( nb_points_in_element == 3 );
                    mesh.add_element( Triangle(), DefaultBehavior(), (unsigned *)cell_connectivity_data.ptr() + cpt );
                }
                else if ( type_cell == 22 ) { // Triangle_6
                    assert( nb_points_in_element == 6 );
                    mesh.add_element( Triangle_6(), DefaultBehavior(), (unsigned *)cell_connectivity_data.ptr() + cpt );
                }
                else if ( type_cell == 3 ) { // Bar
                    assert( nb_points_in_element == 2 );
                    mesh.add_element( Bar(), DefaultBehavior(), (unsigned *)cell_connectivity_data.ptr() + cpt );
                }
                else if ( type_cell == 21 ) { // Bar_3
                    assert( nb_points_in_element == 3 );
                    mesh.add_element( Bar_3(), DefaultBehavior(), (unsigned *)cell_connectivity_data.ptr() + cpt );
                }
                else if ( type_cell == 9 ) { // Quad
                    assert( nb_points_in_element == 4 );
                    mesh.add_element( Quad(), DefaultBehavior(), (unsigned *)cell_connectivity_data.ptr() + cpt );
                }
                else if ( type_cell == 23 ) { // Quad_8
                    assert( nb_points_in_element == 8 );
                    mesh.add_element( Quad_8(), DefaultBehavior(), (unsigned *)cell_connectivity_data.ptr() + cpt );
                }
                else if ( type_cell == 12 ) { // Hexa
                    assert( nb_points_in_element == 8 );
                    mesh.add_element( Hexa(), DefaultBehavior(), (unsigned *)cell_connectivity_data.ptr() + cpt );
                }
                else if ( type_cell == 25 ) { // Hexa_20
                    assert( nb_points_in_element == 20 );
                    mesh.add_element( Hexa_20(), DefaultBehavior(), (unsigned *)cell_connectivity_data.ptr() + cpt );
                }
                else if ( type_cell == 13 ) { // Wedge
                    assert( nb_points_in_element == 8 );
                    mesh.add_element( Wedge(), DefaultBehavior(), (unsigned *)cell_connectivity_data.ptr() + cpt );
                }
                else if ( type_cell == 26 ) { // Wedge_15
                    assert( nb_points_in_element == 15 );
                    mesh.add_element( Wedge_15(), DefaultBehavior(), (unsigned *)cell_connectivity_data.ptr() + cpt );
                }
                else {
                    std::cerr << "TODO : type cell " << __FILE__ << " " << __LINE__ << std::endl;
                }
                cpt += nb_points_in_element;
            }
        }
        else if ( str.find("CELL_DATA") < str.size() ) {
            getline( is, str );
            std::istringstream ss( str );
            unsigned nb_fields;
            ss >> tmp_string >> tmp_string >> nb_fields;
            for(unsigned num_field=0;num_field<nb_fields;++num_field) {
                //
                getline( is, str );
                std::istringstream ss( str );
                unsigned d;
                std::string field_name;
                ss >> field_name >> d;
                apply( mesh.elem_list, GetVtkFields(), is, field_name, d );
            }
            break;
        }
    }
}
/*!
 put vtk mesh in m
\keyword Maillage/Import
*/
template<class TM>
void read_vtk(TM &m,const std::string &fic_name) throw(std::runtime_error) {
    // ouverture du fichier
    std::ifstream my_file( fic_name.c_str() );
    if ( ! my_file.is_open() )
        throw std::runtime_error( "opening of "+fic_name+" has failed.");
    return read_vtk(m,my_file);
}

}

#endif // READ_VTK_H
