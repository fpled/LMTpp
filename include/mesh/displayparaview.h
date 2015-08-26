//
// C++ Interface: displayparaview
//
// Description: 
//
//
// Author: Hugo LECLERC <leclerc@lmt.ens-cachan.fr>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMTDISPLAYPARAVIEW_H
#define LMTDISPLAYPARAVIEW_H

#include "write_mesh_vtk.h"
#include "../implicitgeometry/shapedeclaration.h"
#include "../containers/algo.h"

#include <string>
#include <fstream>
#include <sstream>

#ifdef METIL_COMP_DIRECTIVE
#pragma src_file displayparaview.cpp
#endif


namespace LMT {

template<class Carac, class X, class Y>   /// modif 22/06/2009
struct SubStructuredProblem;

/*!
    Objectif :
        Cette classe permet de stocker dans des fichiers lisibles par paraview le contenu de maillages mais avec des contraintes sur le type des attributs du maillage car paraview ne gère pas tous les types d'attributs ( e.g. les points de Gauss ).
        
    Exemple classique d'utilisation :
    \code C/C++
            DisplayParaview dp;
            Vec<std::string> lp;
            lp.push_back( "dep" );
            lp.push_back( "tag" );

            TM m; // un maillage
                    
            for( unsigned i = 0; ; ++i ) {

                /// le maillage ou son contenu varie
                
                dp.add_mesh( m, "tmp/ref", lp, i + 1 );
            }
             
            dp.exec( "sauvegarde.pvd" );

    Explication du code et quelques commentaires:
    
    Avant de commencer à exécuter le code, créer un répertoire tmp dans le répertoire de votre programme.
    
    Le vecteur de <strong> string </strong> doit contenir le nom des attributs que vous souhaitez enregistrer. Par défaut la géométrie est toujours enregistrée donc inutile d'ajouter "pos". Si vous souhaitez enregistrer tous les attributs, il suffit de créer un vecteur de <strong> string </strong> qui contient juste la chaîne de caractère "all", i.e. :
    \code C/C++
        Vec<std::string> lp( "all" );   

    Ensuite pour enregistrer un maillage, utilisez la méthode add_mesh(). Le dernier paramètre est le numéro du maillage. Cela peut être aussi une date de type double à la place d'un entier.

    Pour enregistrer et lancer paraview , on utilise la méthode exec(). Si vous exécuter exec() sans paramètre, vous aurez nom de fichier par défaut ( paraview.pvd ).

    \author Hugo LECLERC
*/
class DisplayParaview {
public:
    enum TypeField {Nodal,Elementary};
    DisplayParaview() {
        background_color = Vec<double,3>(0.2,0.2,0.3);
        init_xminmax = false;
    }
    ~DisplayParaview() {
    }
    
    template<class TM> std::string add_mesh( const TM &m, const std::string &prefix = "paraview", const Vec<std::string> &display_fields = Vec<std::string>("all"), double time_step = 0, bool write_mesh = true ) {
        std::string pvu_name = prefix;
        //if ( prefix.rfind(".vtu") != prefix.size() - 4 )
        pvu_name += "_" + to_string( time_step ) + "_" + to_string( pvu_files[time_step].size() ) + ".vtu";
        // std::cout << pvu_name << std::endl;

        pvu_files[ time_step ].push_back( pvu_name );
        
        if (write_mesh) {
            std::ofstream f( pvu_name.c_str() );
            write_mesh_vtk<true>( f, m, display_fields );
        }

        typename TM::Pvec xmi,xma;
        get_min_max( m.node_list, ExtractDM<pos_DM>(), xmi, xma );
        if ( m.node_list.size() )
            app_xminmax(prefix,xmi,xma);
        return pvu_name;
    }

    template<class TM> std::string add_mesh_iter( const TM &m, const std::string &prefix = "paraview", const Vec<std::string> &display_fields = Vec<std::string>("all"), double iter = 0, bool write_mesh = true ) {
        std::string pvu_name = prefix;
        //if ( prefix.rfind(".vtu") != prefix.size() - 4 )
        pvu_name += "_" + to_string( iter ) + ".vtu";
        // std::cout << pvu_name << std::endl;

        pvu_files[ iter ].push_back( pvu_name );
        
        if (write_mesh) {
            std::ofstream f( pvu_name.c_str() );
            write_mesh_vtk<true>( f, m, display_fields );
        }

        typename TM::Pvec xmi,xma;
        get_min_max( m.node_list, ExtractDM<pos_DM>(), xmi, xma );
        if ( m.node_list.size() )
            app_xminmax(prefix,xmi,xma);
        return pvu_name;
    }

    template<class TM> std::string set_mesh( const TM &m, const std::string &prefix = "paraview", const Vec<std::string> &display_fields = Vec<std::string>("all"), bool write_mesh = true ) {
        std::ostringstream ss;
        ss << prefix ;//<< pvu_files[prefix].size() << ".vtu";
        std::string pvu_name( ss.str() );
        if ( prefix.rfind(".vtu") != prefix.size() - 4 )
            pvu_name += ".vtu";

        pvu_files[ 0 ].push_back( pvu_name );

        if (write_mesh) {
            std::ofstream f( pvu_name.c_str() );
            write_mesh_vtk<true>( f, m, display_fields );
        }

        typename TM::Pvec xmi,xma;
        get_min_max( m.node_list, ExtractDM<pos_DM>(), xmi, xma );
        if ( m.node_list.size() )
            app_xminmax(prefix,xmi,xma);
        return pvu_name;
    }

    template<class TS> std::string add_shape( const Shape<2,TS> &shape, unsigned grid_size, const std::string &prefix = "paraview" ) {
        typedef typename Shape<2,TS>::Pvec Pvec;
        typedef typename Shape<2,TS>::TPen TPen;
        Pvec tdim_min,tdim_max;
        shape.get_dim_min_max(tdim_min,tdim_max);
        tdim_min -= 2.0/grid_size*(tdim_max-tdim_min);
        tdim_max += 2.0/grid_size*(tdim_max-tdim_min);
        app_xminmax(prefix,tdim_min,tdim_max);

        Vec<Pvec> points(grid_size*grid_size);
        Vec<TPen> pem(grid_size*grid_size);
        Pvec spacing = (tdim_max-tdim_min)/(grid_size-1);
        for (unsigned i=0,offset=0;i<grid_size;++i)
            for (unsigned j=0;j<grid_size;++j,++offset)
                points[offset] = tdim_min + Pvec(j,i) * spacing;
        shape.get_penetration( points.begin(), pem.begin(), points.size() );
        
        std::ostringstream ss;
        ss << prefix << "_" << vti_files.size() << ".vti";
        std::string vti_name( ss.str() );
        vti_files.push_back(vti_name);
        std::ofstream f(vti_name.c_str());
        
        f << "<?xml version='1.0'?>" << std::endl;
        f << "<VTKFile type='ImageData' version='0.1' byte_order='LittleEndian'>" << std::endl;
        f << "  <ImageData WholeExtent='0 " << grid_size-1 << " 0 " << grid_size-1
          << " 0 0' Origin='" << tdim_min << " 0' Spacing='" << spacing << " 0.01'>" << std::endl;
        f << "    <Piece Extent='0 " << grid_size-1 << " 0 " << grid_size-1 << " 0 0'>" << std::endl;
        f << "      <PointData Scalars='offset'>" << std::endl;
        f << "        <DataArray type='Float64' Name='offset' format='ascii' NumberOfComponents='1'>" << std::endl;
        for(unsigned i=0;i<pem.size();++i)
            f << pem[i].dist << ' ';
        //f.write((char *)scalars.begin(),scalars.size()*sizeof(float));
        f << std::endl;
        f << "        </DataArray>" << std::endl;
        f << "        <DataArray type='Float64' Name='normals' format='ascii' NumberOfComponents='3'>" << std::endl;
        for(unsigned i=0;i<pem.size();++i)
            f << pem[i].normal[0] << ' ' << pem[i].normal[1] << " 0 ";
        //f.write((char *)scalars.begin(),scalars.size()*sizeof(float));
        f << std::endl;
        f << "        </DataArray>" << std::endl;
        f << "      </PointData>" << std::endl;
        f << "      <CellData>" << std::endl;
        f << "      </CellData>" << std::endl;
        f << "    </Piece>" << std::endl;
        f << "  </ImageData>" << std::endl;
        f << "</VTKFile>" << std::endl;
        return vti_name;
    }
    void set_field_to_display(const std::string &name,TypeField type) {
        field_to_display = name;
        type_field_to_display = type;
    }

    void make_pvd_file( const std::string &prefix = "paraview" ) const {
        std::string filename = prefix;
        if ( filename.rfind(".pvd") != filename.size() - 4 )
           filename += ".pvd";
        std::ofstream f( filename.c_str() );
        //
        f << "<?xml version='1.0'?>" << std::endl;
        f << "<VTKFile type='Collection' version='0.1'>" << std::endl;
        f << "    <Collection>" << std::endl;
        unsigned cpt = 0;
        for( std::map<double,Vec<std::string> >::const_iterator iter = pvu_files.begin(); iter != pvu_files.end(); ++iter, ++cpt )
            for(unsigned i=0;i<iter->second.size();++i)
                f << "        <DataSet timestep='" << iter->first << "' part='" << i << "' file='" << iter->second[i] << "'/>" << std::endl;
        f << "    </Collection>" << std::endl;
        f << "</VTKFile>" << std::endl;
   }

   int exec( const std::string &prefix = "paraview" ) {
       std::string filename = prefix;
       if ( filename.rfind(".pvd") != filename.size() - 4 )
           filename += ".pvd";
       make_pvd_file( filename );

       if ( pvsm_file.size() ) {
           std::string t = "paraview --state=" + pvsm_file;
           return system( t.c_str() );
       }

       std::string t = "paraview --data=" + filename;
       return system( t.c_str() );
    
       /*
       std::string tmp_file = prefix + ".pvs";
       std::ofstream pvs( tmp_file.c_str() );
       pvs << "" << std::endl;
       pvs << "# ParaView State Version 1.8" << std::endl;
       pvs << "" << std::endl;
       pvs << "set kw(vtkApplication) [$Application GetMainWindow]" << std::endl;
       pvs << "set kw(vtkMainWin) [$kw(vtkApplication) GetMainView]" << std::endl;
       // pvs << "set kw(vtkTemp322) [$kw(vtkApplication) GetAnimationInterface]" << std::endl;
       //pvs << "[$kw(vtkApplication) GetRotateCameraButton] SetState 1" << std::endl;
       //pvs << "$kw(vtkApplication) ChangeInteractorStyle 1" << std::endl;
       for( std::map<std::string,Vec<std::string> >::const_iterator iter = pvu_files.begin(); iter != pvu_files.end(); ++iter ) {
           for(unsigned i=0;i<( all_mesh ? iter->second.size() : min(iter->second.size(),(unsigned)1) );++i) {
               pvs << "set kw(vtkgr" << i << ") [$kw(vtkApplication) InitializeReadCustom \"XMLUnstructuredGridReader\" \""
                   << iter->second[i] << "\"]" << std::endl;
               pvs << "$kw(vtkApplication) ReadFileInformation $kw(vtkgr" << i << ") \"" << iter->second[i] << "\"" << std::endl;
               pvs << "$kw(vtkApplication) FinalizeRead $kw(vtkgr" << i << ") \"" << iter->second[i] << "\"" << std::endl;
               pvs << "set kw(vtkTemp534) [$kw(vtkgr" << i << ") GetPVWidget {Filename}]" << std::endl;
               pvs << "$kw(vtkTemp534) SetValue \"" << iter->second[i] << "\"" << std::endl;
               pvs << "$kw(vtkgr" << i << ") AcceptCallback" << std::endl;
               if ( field_to_display.size() ) {
                   if ( type_field_to_display == Nodal )
                       pvs << "[$kw(vtkgr"<<i<<") GetPVOutput] ColorByPointField {" << field_to_display << "} 1" << std::endl;
                   else
                       pvs << "[$kw(vtkgr"<<i<<") GetPVOutput] ColorByCellField {" << field_to_display << "} 1" << std::endl;
               }
               pvs << "" << std::endl;
           }
       }
       for(unsigned i=0;i<vti_files.size();++i) {
           pvs << "set kw(vtkTemp525) [$kw(vtkApplication) InitializeReadCustom \"XMLImageDataReader\" \""
               << vti_files[i] << "\"]" << std::endl;
           pvs << "$kw(vtkApplication) ReadFileInformation $kw(vtkTemp525) \"" << vti_files[i] << "\"" << std::endl;
           pvs << "$kw(vtkApplication) FinalizeRead $kw(vtkTemp525) \"" << vti_files[i] << "\"" << std::endl;
           pvs << "set kw(vtkTemp534) [$kw(vtkTemp525) GetPVWidget {Filename}]" << std::endl;
           pvs << "$kw(vtkTemp534) SetValue \"" << vti_files[i] << "\"" << std::endl;
           pvs << "set kw(vtkTemp529) [$kw(vtkTemp525) GetPVWidget {PointSelection}]" << std::endl;
           pvs << "$kw(vtkTemp529) SetArrayStatus {offset} 1" << std::endl;
           pvs << "set kw(vtkTemp533) [$kw(vtkTemp525) GetPVWidget {CellSelection}]" << std::endl;
           pvs << "$kw(vtkTemp525) AcceptCallback" << std::endl;
           pvs << "set pvDisp(vtkTemp525) [$kw(vtkTemp525) GetPartDisplay]" << std::endl;
           pvs << "$pvDisp(vtkTemp525) SetOpacity 0.3" << std::endl;
           pvs << "[$kw(vtkTemp525) GetPVOutput] ColorByPointField {offset} 1" << std::endl;
           pvs << "set kw(vtkTemp602) [$kw(vtkApplication) CreatePVSource Contour]" << std::endl;
           pvs << "set kw(vtkTemp608) [$kw(vtkTemp602) GetPVWidget {Contour Values}]" << std::endl;
           pvs << "$kw(vtkTemp608) AddValue 0" << std::endl;
           pvs << "$kw(vtkTemp602) AcceptCallback" << std::endl;
       }
       if ( field_to_display.size() ) {
           pvs << "set kw(vtkTemp571) [$kw(vtkApplication) GetPVColorMap {" << field_to_display << "} 1]" << std::endl;
           // pvs << "$kw(vtkTemp571) SetScalarRange 0 11" << std::endl;
           pvs << "$kw(vtkTemp571) SetStartHSV 0.6667 1 1" << std::endl;
           pvs << "$kw(vtkTemp571) SetEndHSV 0 1 1" << std::endl;
           pvs << "$kw(vtkTemp571) VectorModeMagnitudeCallback" << std::endl;
           pvs << "$kw(vtkTemp571) SetScalarBarVisibility 1" << std::endl;
       }
       pvs << "$kw(vtkMainWin) SetRendererBackgroundColor "
           << background_color[0] << " "
           << background_color[1] << " "
           << background_color[2] << std::endl;
       pvs << "$kw(vtkMainWin) ParallelProjectionOff" << std::endl;

       pvs << "$kw(vtkApplication) SetCenterOfRotation "
           << (xmin[0]+xmax[0])/2.0 << " "
           << (xmin[1]+xmax[1])/2.0 << " "
           << (xmin[2]+xmax[2])/2.0 << std::endl;
       pvs << "$kw(vtkApplication) ResetCameraCallback" << std::endl;
       //    pvs << "$kw(vtkMainWin) Render" << std::endl;
       pvs.close();

       system( ("paraview --data="+tmp_file).c_str() );
       */
    }

    Vec<std::string> get_all_pvu_files() const {
        Vec<std::string> res;
        for( std::map<double,Vec<std::string> >::const_iterator iter = pvu_files.begin(); iter != pvu_files.end(); ++iter )
            for(unsigned i=0;i<iter->second.size();++i)
                res.push_back( iter->second[i] );
        return res;
    }

    /// need s.number and s.mesh
    struct AddSstMesh {
        template<class TS> void operator()(const TS &s,int i,DisplayParaview &dp,double time_step) const {
            disp( s, i, dp, Number<TS::has_mesh>(), time_step );
        }
        template<class TS> void disp(const TS &s,int i,DisplayParaview &dp,Number<0>,double time_step) const {
        }
        template<class TS> void disp(const TS &s,int i,DisplayParaview &dp,Number<1>,double time_step) const {
            DynamicData<double,TS::TM::TElemList::nb_elem_type> number("number");
            s.mesh.elem_list.reg_dyn( &number );
            number.set_values( s.number );
            dp.add_mesh( s.mesh, "paraview", Vec<std::string>("all"), time_step );
            s.mesh.elem_list.unreg_dyn( &number );
        }
    };
    
    std::string pvsm_file;
    
private:
    template<class PV> void app_xminmax(const std::string &prefix,const PV &xmi,const PV &xma) {
        if ( init_xminmax ) {
            for(unsigned i=0;i<min(xmi.size(),(unsigned)3);++i) {
                xmin[i] = min(xmin[i],xmi[i]); xmax[i] = max(xmax[i],xma[i]);
            }
        }
        else {
            xmin = 0.0; xmax = 0.0;
            for(unsigned i=0;i<min(xmi.size(),(unsigned)3);++i) {
                xmin[i] = xmi[i]; xmax[i] = xma[i];
            }
            init_xminmax = true;
        }
    }

    std::map<double,Vec<std::string> > pvu_files;
    Vec<std::string> vti_files;
    std::string field_to_display;
    TypeField type_field_to_display;
    Vec<double,3> background_color;
    Vec<double,3> xmin,xmax;
    bool init_xminmax;
};

/*!
 * declare a DisplayParaview instance, push m and interact with user
 * @param m the mesh
 * @param nodal_field_to_display ...
 
    \relates DisplayParaview
    \keyword Maillage/Affichage
    \keyword Visualisation/3D
    \keyword Visualisation/2D
    
    \relates Exemple de création d'un maillage et utilisation de la fonction interpolation()
     
    \author Hugal
 
*/
template<class TM> void display_mesh( const TM &m, const char *nodal_field_to_display = "", DisplayParaview::TypeField type_field_to_display = DisplayParaview::Nodal, const std::string &prefix = "paraview" ) {
    DisplayParaview dp;
    dp.add_mesh( m, prefix );
    dp.set_field_to_display( nodal_field_to_display, type_field_to_display );
    dp.exec( prefix );
}

/*!
 * usefull to get several Windows (using apply_mt)
 */
struct DpExec { void operator()(DisplayParaview &dp,unsigned i) const { dp.exec( "conf"+to_string(i)+".pvs" ); } };

/*!
*/
template<class Carac,unsigned nvi_to_subs,unsigned skin>
int display( const MeshAncestor<Carac,nvi_to_subs,skin> &m, const std::string &prefix = "paraview", const std::string &pvsm_file = "" ) {
    DisplayParaview dp;
    std::string res = dp.set_mesh( m, prefix );
    
    if ( pvsm_file.size() ) {
        std::string t = "paraview --state=" + pvsm_file;
        return system( t.c_str() );
    }
    std::string t = "paraview --data=" + res;
    return system( t.c_str() );
}



/**

    A commenter

    \keyword Sous-structure/Affichage
    


    \author Hugal
*/
template<class Carac, class X, class Y>
int display( const SubStructuredProblem<Carac,X,Y> &sst_pb, const std::string &prefix = "paraview", const std::string &pvsm_file = "" ) {
    if ( prefix.rfind(".pvd") != prefix.size() - 4 )
        prefix += ".pvd";
    DisplayParaview dp;
    apply_wi( sst_pb.sub_structures, DisplayParaview::AddSstMesh(), dp, 0 );
    apply_wi( sst_pb.interfaces    , DisplayParaview::AddSstMesh(), dp, 0 );
    dp.make_pvd_file( prefix );
    
    if ( pvsm_file.size() ) {
        std::string t = "paraview --state=" + pvsm_file;
        return system( t.c_str() );
    }
    std::string t = "paraview --data=" + prefix;
    return system( t.c_str() );
}

};



#endif
