//
// C++ Interface: problem
//
// Description: 
//
//
// Author: Hugo LECLERC <hugo.leclerc@lmt.ens-cachan.fr>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMT_problem_HEADER
#define LMT_problem_HEADER

#include "mesh.h"
#include "displayparaview.h"
#include "read_gid.h"
#include "read_stl.h"

namespace LMT {

/**

*/
template<class Carac>
class Problem {
public:
    typedef Vec<Heterogeneous<Carac> > TListMesh;
    static const unsigned dim = TListMesh::template SubType<0>::T::dim;
    
    ///
    Problem() {
        file_prefix = file_prefix;
    }
    ///
    void read_data(int argc,char **argv) throw (IoException) {
        if ( argc==1 ) {
            throw IoException( "No data to read (in arguments) !" );
            return;
        }
        for(unsigned i=1;i<argc;++i) {
            XmlNode n;
            n.parse_file( argv[i] );
            for(unsigned nm=0;nm<n.nb_elements("mesh");++nm) {
                XmlNode M( n.get_element("mesh",nm) );
                std::string file = M.get_attribute("file");
                if ( file.size()>4 and std::string(file.end()-4,file.end())==".msh" ) {
                    read_gid( *mesh_list.sub_vec(Number<0>()).new_elem(), file );
                }
                else if ( file.size()>4 and std::string(file.end()-4,file.end())==".stl" ) {
                    read_stl( *mesh_list.sub_vec(Number<0>()).new_elem(), file );
                }
                else {
                    throw IoException(file+" is in an unknown format");
                }
            }
        }
    }
private:
    struct DispMesh { template<class TM> void operator()(const TM &m,DisplayParaview &dp,const Vec<std::string> &avoid_fields) const { dp.add_mesh(m,file_prefix,avoid_fields); } };
    struct DispSubMesh { template<class TM> void operator()(const TM &m,DisplayParaview &dp,const Vec<std::string> &avoid_fields) const { dp.add_mesh(m.sub_mesh(Number<1>()),file_prefix,avoid_fields); } };
    struct AppendMesh { template<class TM,class TM2> void operator()(const TM &m,TM2 &mm) const { mm.append(m); } };
public:
    void display(bool display_sub_meshes=false,const Vec<std::string> &avoid_fields=Vec<std::string>()) {
        DisplayParaview dp;
        apply( mesh_list, DispMesh(), dp );
        if ( display_sub_meshes )
            apply( mesh_list, DispSubMesh(), dp );
        //dp.set_field_to_display("a",DisplayParaview::Nodal);
        //dp.set_field_to_display("v",DisplayParaview::Elementary);
        dp.exec();
    }
    /**
        Assumes that type mesh number 0 can contain all data
    */
    void display_in_unique_mesh( bool display_sub_meshes=false, const Vec<std::string> &avoid_fields=Vec<std::string>(), double time_step = 0 ) {
        DisplayParaview dp;
        typename TListMesh::template SubType<0>::T m;
        apply( mesh_list, AppendMesh(), m );
        dp.add_mesh( m, file_prefix, avoid_fields, time_step );
        dp.exec();
    }
    
    void save_in_unique_mesh( bool display_sub_meshes=false, const Vec<std::string> &avoid_fields=Vec<std::string>(), std::string filename = "", bool exec = false, double time_step = 0 ) {
        if ( not filename.size() )
            filename = file_prefix;
        //
        DisplayParaview dp;
        typename TListMesh::template SubType<0>::T m;
        apply( mesh_list, AppendMesh(), m );
        dp.add_mesh( m, filename, avoid_fields, time_step );
        if ( exec )
            dp.exec();
    }
   
    std::string file_prefix;
    TListMesh mesh_list;
};

};

#endif // LMT_problem_HEADER
