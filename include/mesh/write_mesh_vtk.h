//
// C++ Interface: write_mesh_vtk
//
// Description:
//
//
// Author: Hugo LECLERC <leclerc@lmt.ens-cachan.fr>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMT_WRITEMESHVTK_HEADER
#define LMT_WRITEMESHVTK_HEADER

#include <fstream>
#include <map>
#include <string.h>
#include "mesh.h"
#include "zlib.h"

namespace LMT {

template<class TE,class TM>
void set_vtk_cell_type_and_offsets( const TE &elem, Vec<unsigned> &connectivity, Vec<unsigned> &offsets, Vec<unsigned> &cell_types, TM *m ) {
    unsigned cell_type, nb_points;
    if ( strcmp(TE::can_directly_be_represented_by(),"NodalElement")==0 ) {
        cell_type = 1;
        nb_points = 1;
    } else if ( strcmp(TE::can_directly_be_represented_by(),"Bar")==0 ) {
        cell_type = 3;
        nb_points = 2;
    } else if ( strcmp(TE::can_directly_be_represented_by(),"Triangle")==0 ) {
        cell_type = 5;
        nb_points = 3;
    } else if ( strcmp(TE::can_directly_be_represented_by(),"Quad")==0 ) {
        cell_type = 9;
        nb_points = 4;
    } else if ( strcmp(TE::can_directly_be_represented_by(),"Tetra")==0 ) {
        cell_type = 10;
        nb_points = 4;
    } else if ( strcmp(TE::can_directly_be_represented_by(),"Hexa")==0 ) {
        cell_type = 12;
        nb_points = 8;
    } else if ( strcmp(TE::can_directly_be_represented_by(),"Wedge")==0 ) {
        cell_type = 13;
        nb_points = 6;
    } else if ( strcmp(TE::can_directly_be_represented_by(),"Pyramid")==0 ) {
        cell_type = 14;
        nb_points = 5;
    } else if ( strcmp(TE::can_directly_be_represented_by(),"Bar_3")==0 ) {
        cell_type = 21;
        nb_points = 3;
    } else if ( strcmp(TE::can_directly_be_represented_by(),"Triangle_6")==0 ) {
        cell_type = 22;
        nb_points = 6;
    } else if ( strcmp(TE::can_directly_be_represented_by(),"Quad_8")==0 ) {
        cell_type = 23;
        nb_points = 8;
    } else if ( strcmp(TE::can_directly_be_represented_by(),"Quad_9")==0 ) {
        cell_type = 23;
        nb_points = 8;
    } else if ( strcmp(TE::can_directly_be_represented_by(),"Tetra_10")==0 ) {
        cell_type = 24;
        nb_points = 10;
    } else if ( strcmp(TE::can_directly_be_represented_by(),"Hexa_20")==0 ) {
        cell_type = 25;
        nb_points = 20;
    } else if ( strcmp(TE::can_directly_be_represented_by(),"Wedge_15")==0 ) {
        cell_type = 26;
        nb_points = 15;
    } else {
        cell_type = 0;
        nb_points = 0;
    }

    for(unsigned i=0;i<nb_points;++i)
        connectivity.push_back( m->node_list.number(*elem.node(i)) );
    offsets.push_back( connectivity.size() );
    cell_types.push_back( cell_type );
}

template<class TM>
struct Elem_vtk_extract {
    template<class TE>
    void operator()( const TE &elem ) {
        set_vtk_cell_type_and_offsets( elem, connectivity, offsets, cell_types, m );
    }
    Vec<unsigned> connectivity,offsets,cell_types;
    const TM *m;
};

template<unsigned nb_data,bool binary>
struct Data_vtk_extract {

    template<class T>
    void operator()( unsigned i, T data ) {
        //std::cout << "### Data_vtk_extract.operator( scalar ) et i = " << i << std::endl;
        if ( binary ) {
            os[i].write( (char *)&data, sizeof(T) );
            //os[i].write( " ", sizeof(char) );
        } else {
            os[i] << data;
            if ( nb_comp[i]==2 )
                os[i] << " 0";
            os[i] << " ";
        }
    }
    
    template<class T>
    void operator()( unsigned i, std::complex<T> data ) {
        //std::cout << "### Data_vtk_extract.operator( complex ) et i = " << i << std::endl;
        if ( binary ) {
            os[i].write( (char *)&data, sizeof(T)*2 );
            T v = 0;
            os[i].write( (char *)&v, sizeof(T) );
        } else {
            os[i] << data << " 0 ";
        }
    }
    
    template<class T,int s>
    void operator()( unsigned i, Vec<T,s> data ) {
        //std::cout << "### Data_vtk_extract.operator( Vec ) et i = " << i << std::endl;
        if ( binary ) {
            os[i].write( (char *)&data, sizeof(T)*data.size() );
            if ( data.size()==2 ) {
                T v(0);
                os[i].write( (char *)&v, sizeof(T) );
            }
        } else {
            os[i] << data;
            if ( data.size()==2 )
                os[i] << " 0";
            os[i] << " ";
        }
    }
    
    /*!
        il aurait été tentant de copier en binaire le contenu du vecteur data.data mais pour des matrices de taille varaible, le contenu et la taille de data.data est optimisé pour les instructions SIMD et donc ne correspond pas forcément à ce qu'on peut penser comme stockage de données.
    */
    template<class T, int sr, int sc, class STO>
    void operator()( unsigned i, Mat<T, Gen<sr, sc>, STO > data ) {
        //std::cout << "### Data_vtk_extract.operator( Mat ) et i = " << i << std::endl;
        if ( binary ) {
            T v;
            for( unsigned ii = 0; ii < data.nb_rows(); ++ii )
                for( unsigned j = 0; j < data.nb_cols(); ++j ) {
                    v = data( ii, j );
                    os[i].write( (char *)&v,  sizeof(T) ); 
                }
        } else {
            os[i] << data << " 0 ";
        }
    }
    
    /*!
        il aurait été tentant de copier en binaire le contenu du vecteur data.data mais pour des matrices de taille varaible, le contenu et la taille de data.data est optimisé pour les instructions SIMD et donc ne correspond pas forcément à ce qu'on peut penser comme stockage de données.
    */
    template<class T, int s, class STO>
    void operator()( unsigned i, Mat<T, Sym<s>, STO > data ) {
        //std::cout << "### Data_vtk_extract.operator( Mat Sym ) et i = " << i << std::endl;
        if ( binary ) {
            T v;
            for( unsigned ii = 0; ii < data.nb_rows(); ++ii )
                for( unsigned j = 0; j <= ii; ++j ) {
                    v = data( ii, j );
                    os[i].write( (char *)&v,  sizeof(T) ); 
                }
        } else {
            os[i] << data << " 0 ";
        }
    }
    
    std::ostringstream os[nb_data+(nb_data==0)];

    unsigned nb_comp[nb_data+(nb_data==0)];
};

template<unsigned nb_params_>
struct GetDynamicSize {
    GetDynamicSize() {
        for(unsigned i=0;i<nb_params_;++i) {
            dynamic_size[i] = 0;
            nb_comp[ i ] = 0;
        }
    }
    template<class T> void operator()( unsigned n, const T &d ) { }
    template<class T> void operator()( unsigned n, const Vec<T,-1> &d ) {
        dynamic_size[n] = max( d.size(), dynamic_size[n] );
        nb_comp[n] = DM::NbComponents<T>::n;
    }
    
    static const unsigned nb_params = nb_params_; 
    unsigned dynamic_size[nb_params_+(nb_params_==0)];
    unsigned nb_comp[nb_params_+(nb_params_==0)];
};

struct GetDynamicSizeOs {
    template<class T>
    void operator()( unsigned n, const T &d ) { }
    template<class T>
    void operator()( unsigned n, const Vec<T,-1> &d ) {
        if ( n==n_wanted ) {
            if ( pos_in_vec_wanted<d.size() )
                os << d[pos_in_vec_wanted] << " ";
            else {
                for(unsigned i=0;i<DM::NbComponents<T>::n;++i)
                    os << "0 ";
            }
        }
    }
    std::ostream &os;
    unsigned n_wanted;
    unsigned pos_in_vec_wanted;
};

template<class T> struct GetVtkType { };
template<> struct GetVtkType<double> { static const char *res() { return "Float64"; } };
template<> struct GetVtkType<float> { static const char *res() { return "Float32"; } };
template<> struct GetVtkType<int> { static const char *res() { return "Int32"; } };
template<> struct GetVtkType<unsigned> { static const char *res() { return "UInt32"; } };
template<> struct GetVtkType<short> { static const char *res() { return "Int16"; } };
template<> struct GetVtkType<unsigned short> { static const char *res() { return "UInt16"; } };
template<> struct GetVtkType<char> { static const char *res() { return "Int8"; } };
template<> struct GetVtkType<unsigned char> { static const char *res() { return "UInt8"; } };
template<> struct GetVtkType<signed long> { static const char *res() { return "Int64"; } };
template<> struct GetVtkType<unsigned long> { static const char *res() { return "UInt64"; } };

template<class T> struct GetVtkTypeSize { static const unsigned n = sizeof(T); };
template<class T,int a> struct GetVtkTypeSize<Vec<T,a> > { static const unsigned n = sizeof(T); };
template<class T,int a,int m> struct GetVtkTypeSize<Vec<Vec<T,a>,m> > { static const unsigned n = sizeof(T); };
template<class T,class TO,class ST> struct GetVtkTypeSize<Mat<T,TO,ST> > { static const unsigned n = sizeof(T); };
template<class T,class TO,class ST> struct GetVtkTypeSize< Vec< Mat<T,TO,ST> > > { static const unsigned n = sizeof(T); };
template<class T> struct GetVtkTypeSize<std::complex<T> > { static const unsigned n = sizeof(T); };

struct GetVtkTypes {
    /// scalar
    template<unsigned n,class T> void operator()( const Number<n> &nn, const StructForType<T> &st ) { res.push_back( GetVtkType<T>::res() ); }
    /// complex
    template<unsigned n,class T> void operator()( const Number<n> &nn, const StructForType<std::complex<T> > &st ) { res.push_back( GetVtkType<T>::res() ); }
    /// vector
    template<unsigned n,class T,int s> void operator()( const Number<n> &nn, const StructForType<Vec<T,s> > &st ) { res.push_back( GetVtkType<T>::res() ); }
    /// vector of vector
    template<unsigned n,class T,int s,int s2> void operator()( const Number<n> &nn, const StructForType<Vec<Vec<T,s>,s2 > > &st ) { res.push_back( GetVtkType<T>::res() ); }
    /// vector of vector of vector
    template<unsigned n,class T,int s,int s2,int s3> void operator()( const Number<n> &nn, const StructForType<Vec<Vec<Vec<T,s>,s2>,s3> > &st ){ res.push_back( GetVtkType<T>::res() ); }
    /// vector of vector of vector of vector
    template<unsigned n,class T,int s,int s2,int s3,int s4> void operator()( const Number<n> &nn, const StructForType<Vec<Vec<Vec<Vec<T,s>,s2>,s3>,s4> > &st ){ res.push_back( GetVtkType<T>::res() ); }
    /// generic matrix
    template<unsigned n,class T,int s,int s2> void operator()( const Number<n> &nn, const StructForType<Mat<T,Gen<s,s2> > > &st ) { res.push_back( GetVtkType<T>::res() ); }
    /// vector of generic matrix
    template<unsigned n,class T,int s,int s2,int s3> void operator()( const Number<n> &nn, const StructForType<Vec<Mat<T,Gen<s2,s3> >,s > > &st ) { res.push_back( GetVtkType<T>::res() ); }
    /// matrice symetrique
    template<unsigned n,class T,int s> void operator()( const Number<n> &nn, const StructForType<Mat<T,Sym<s> > > &st ) { res.push_back( GetVtkType<T>::res() ); }
    /// vecteur de matrice symetrique
    template<unsigned n,class T,int s,int s1> void operator()( const Number<n> &nn, const StructForType<Vec<Mat<T,Sym<s> >,s1> > &st ){ res.push_back( GetVtkType<T>::res() ); }
    /// vecteur de vecteur de matrice symetrique
    template<unsigned n,class T,int s,int s1,int s2> void operator()( const Number<n> &nn, const StructForType<Vec<Vec<Mat<T,Sym<s> >,s1>,s2> > &st ){ res.push_back( GetVtkType<T>::res() ); }
    Vec<const char *> res;
};

template<bool binary>
struct Data_vtk_extract_elem {
    struct DataForOneElemType {
        DataForOneElemType() : nb_comp(0) {}
        unsigned nb_comp;
        const char *vtk_type;
        std::string os;
        bool appended;
    };
    typedef std::map<std::string,DataForOneElemType> Map;
    template<class TE>
    void operator()( const StructForType<TE> &s ) {
        const char *names[TE::nb_params+(TE::nb_params==0)];
        DM::get_names<TE>( names );
        unsigned nb_comp[TE::nb_params+(TE::nb_params==0)];
        DM::get_nb_comp<TE>( nb_comp );
        GetVtkTypes get_vtk_types;
        DM::get_types<TE>( get_vtk_types );

        for(unsigned i=0;i<TE::nb_params;++i) {
            mapd[names[i]].nb_comp = max( mapd[names[i]].nb_comp, nb_comp[i] );
            mapd[names[i]].vtk_type = get_vtk_types.res[i];
        }
    }
    
    template<class TE>
    void operator()( const TE &elem, const Vec<std::string> &display_fields = Vec<std::string>("all") ) {
        for(typename Map::iterator iter=mapd.begin();iter!=mapd.end();++iter)
            iter->second.appended = false;
        DM::apply_with_names_up_to(elem,*this,Number<TE::nb_params>());
        for(typename Map::iterator iter=mapd.begin();iter!=mapd.end();++iter)
            if ( iter->second.appended == false )
                for(unsigned i=0;i<iter->second.nb_comp;++i)
                    iter->second.os += "0 ";

        GetDynamicSize<TE::nb_params> gds;
        DM::apply( elem, gds );
        const char *names[TE::nb_params+(TE::nb_params==0)];
        DM::get_names<TE>( names );
        unsigned nb_comp[TE::nb_params+(TE::nb_params==0)];
        DM::get_nb_comp<TE>( nb_comp );
        for(unsigned i=0;i<TE::nb_params;++i) {
//            if ( std::find(display_fields.begin(),display_fields.end(),names[i])!=display_fields.end() )
//                continue;
//            for(unsigned j=0;j<gds.dynamic_size[i];++j) {
//                std::ostringstream name;
//                name << names[i] << "_" << j;
//                mapd[ name.str() ].nb_comp = gds.nb_comp[i];
                
//                std::ostringstream os;
//                GetDynamicSizeOs gdsos = { os, i, j };
//                DM::apply(elem,gdsos);
//                os << " ";
//                mapd[ name.str() ].os += os.str();
//            }
            if ( std::find(display_fields.begin(),display_fields.end(),names[i])!=display_fields.end() or (display_fields.size()>=1 and display_fields[0]=="all") ) {
                for(unsigned j=0;j<gds.dynamic_size[i];++j) {
                    std::ostringstream name;
                    name << names[i] << "_" << j;
                    mapd[ name.str() ].nb_comp = gds.nb_comp[i];

                    std::ostringstream os;
                    GetDynamicSizeOs gdsos = { os, i, j };
                    DM::apply(elem,gdsos);
                    os << " ";
                    mapd[ name.str() ].os += os.str();
                }

            }
        }
    }
    
    template<class T,class ST,class OT>
    void operator()( unsigned i, const char *name, const Mat<T,ST,OT> &data ) {
        operator()( i, name, data.data );
    }
    template<class T>
    void operator()( unsigned i, const char *name, T data ) {
        if ( binary )
            mapd[ name ].os += std::string( (char *)&data, (char *)&data + sizeof(T) );
        else
            mapd[ name ].os += to_string( data ) + " ";

        // nb_comp complement
        const unsigned real_nb_comp = DM::NbComponents<T>::n;
        const unsigned want_nb_comp = mapd[ name ].nb_comp;
        static T tmp;
        if ( binary )
            for(unsigned i=real_nb_comp;i<want_nb_comp;++i)
                mapd[ name ].os += std::string( (char *)&tmp, (char *)&tmp + GetVtkTypeSize<T>::n );
        else
            for(unsigned i=real_nb_comp;i<want_nb_comp;++i)
                mapd[ name ].os += "0 ";

        //
        mapd[ name ].appended = true;
    }
    Map mapd;
};

static const unsigned char vtkBase64UtilitiesEncodeTable[65] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

//----------------------------------------------------------------------------
inline static unsigned char vtkBase64UtilitiesEncodeChar(unsigned char c) {
    return vtkBase64UtilitiesEncodeTable[c];
}

//----------------------------------------------------------------------------
inline void EncodeTriplet(unsigned char i0,
                          unsigned char i1,
                          unsigned char i2,
                          char *o0,
                          char *o1,
                          char *o2,
                          char *o3) {
    *o0 = vtkBase64UtilitiesEncodeChar((i0 >> 2) & 0x3F);
    *o1 = vtkBase64UtilitiesEncodeChar(((i0 << 4) & 0x30)|((i1 >> 4) & 0x0F));
    *o2 = vtkBase64UtilitiesEncodeChar(((i1 << 2) & 0x3C)|((i2 >> 6) & 0x03));
    *o3 = vtkBase64UtilitiesEncodeChar(i2 & 0x3F);
}
inline void EncodePair(unsigned char i0,
                       unsigned char i1,
                       char *o0,
                       char *o1,
                       char *o2,
                       char *o3) {
    *o0 = vtkBase64UtilitiesEncodeChar((i0 >> 2) & 0x3F);
    *o1 = vtkBase64UtilitiesEncodeChar(((i0 << 4) & 0x30)|((i1 >> 4) & 0x0F));
    *o2 = vtkBase64UtilitiesEncodeChar(((i1 << 2) & 0x3C));
    *o3 = '=';
}

//----------------------------------------------------------------------------
inline void EncodeSingle(unsigned char i0,
                         char *o0,
                         char *o1,
                         char *o2,
                         char *o3) {
    *o0 = vtkBase64UtilitiesEncodeChar((i0 >> 2) & 0x3F);
    *o1 = vtkBase64UtilitiesEncodeChar(((i0 << 4) & 0x30));
    *o2 = '=';
    *o3 = '=';
}

/*!
Cette fonction transforme une suite de nombres binaires en une suite de nombres codés en base 64.
Fontion utilisée par \a write_mesh_vtk .
*/
inline void add_encoded( std::string s, std::string &appended ) {
    unsigned s_size = s.size();
    s = std::string( (char *)&s_size, (char *)&s_size + sizeof(s_size) ) + s;

    char o0,o1,o2,o3;
    unsigned k;
    for(k=0;k<s.size()-2;k+=3) {
        EncodeTriplet(s[k+0],s[k+1],s[k+2],&o0,&o1,&o2,&o3);
        appended += o0; appended += o1; appended += o2; appended += o3;
    }
    for(;k<s.size()-1;k+=2) {
        EncodePair(s[k+0],s[k+1],&o0,&o1,&o2,&o3);
        appended += o0; appended += o1; appended += o2; appended += o3;
    }
    for(;k<s.size();++k) {
        EncodeSingle(s[k+0],&o0,&o1,&o2,&o3);
        appended += o0; appended += o1; appended += o2; appended += o3;
    }

}

template<bool binary,class TM>
void write_mesh_vtk( std::ostream &os, const TM &m, const Vec<std::string> &display_fields = Vec<std::string>("all") ) {
    using namespace std;

    std::string appended;
    os << "<?xml version='1.0' ?>" << endl;
    os << "<VTKFile type='UnstructuredGrid' byte_order='LittleEndian'>" << endl;
    os << "    <UnstructuredGrid>" << endl;
    os << "        <Piece NumberOfPoints='" << m.node_list.size() << "' NumberOfCells='" << m.elem_list.size() << "'>" << endl;
    
    // PointData
    os << "            <PointData>" << endl;
    if ( m.node_list.size() ) {
        // static
        const char *names[TM::TNode::nb_params+(TM::TNode::nb_params==0)];
        DM::get_names<typename TM::TNode>( names );
        unsigned nb_comp[TM::TNode::nb_params+(TM::TNode::nb_params==0)];
        DM::get_nb_comp<typename TM::TNode>( nb_comp );
        for(unsigned i=0;i<TM::TNode::nb_params;++i)
            nb_comp[i] += (nb_comp[i]==2);
        GetDynamicSize<TM::TNode::nb_params> gds;
        for(unsigned i=0;i<m.node_list.size();++i)
            DM::apply( m.node_list[i], gds );

        Data_vtk_extract<TM::TNode::nb_params,binary> dve;
        DM::get_nb_comp<typename TM::TNode>( dve.nb_comp );
        for(unsigned i=0;i<m.node_list.size();++i)
            DM::apply(m.node_list[i],dve);

        GetVtkTypes get_vtk_types;
        DM::get_types<typename TM::TNode>( get_vtk_types );

        for(unsigned i=0;i<TM::TNode::nb_params;++i) {
            if ( std::find(display_fields.begin(),display_fields.end(),std::string(names[i]))!=display_fields.end() or (display_fields.size()>=1 and display_fields[0]=="all") ) {    //continue;
                if ( names[i]!=const_cast<char *>("pos") and nb_comp[i] ) {
                    os << "                <DataArray Name='" << names[i]
                          << "' NumberOfComponents='" << nb_comp[i] << "' type='" << get_vtk_types.res[i] << "' format='" << ( binary ? "appended" : "ascii" );
                    if ( binary )
                        os << "' offset='" << appended.size();
                    os << "'>" << endl;
                    if ( binary )
                        add_encoded( dve.os[i].str(), appended );
                    else
                        os << dve.os[i].str() << endl;
                    os << "                </DataArray>" << endl;
                }
            }
        }
        // static with dynamic size ( vectors )
        for(unsigned i=0;i<TM::TNode::nb_params;++i) {
            if ( std::find(display_fields.begin(),display_fields.end(),std::string(names[i]))!=display_fields.end() or (display_fields.size()>=1 and display_fields[0]=="all") ) {    //continue;
                for(unsigned j=0;j<gds.dynamic_size[i];++j) {
                    os << "                <DataArray Name='" << names[i]  << "_" << j
                    << "' NumberOfComponents='" << gds.nb_comp[i] << "' type='Float64' format='ascii'>" << endl;
                    GetDynamicSizeOs gdsos = { os, i, j };
                    for(unsigned i=0;i<m.node_list.size();++i)
                        DM::apply(m.node_list[i],gdsos);
                    os << endl;
                    os << "                </DataArray>" << endl;
                }
            }
        }

        // dynamic
        for(unsigned i=0;i<m.node_list.nb_dyn_data();++i) {
            const DynamicDataAncestor *dd = m.node_list.get_dyn_data(i);
            if ( std::find(display_fields.begin(),display_fields.end(),std::string(dd->get_name()))!=display_fields.end() or (display_fields.size()>=1 and display_fields[0]=="all") ) {    //continue;
                if ( dd->nb_comp()==0 )
                    continue;
                os << "                <DataArray Name='" << dd->get_name()
                << "' NumberOfComponents='" << dd->nb_comp() << "' type='Float64' format='ascii'>" << endl;
                dd->write_data(os);
                os << endl;
                os << "                </DataArray>" << endl;
            }
        }
    }
    os << "            </PointData> " << endl;

    // CellData
    os << "            <CellData>" << endl;
    if ( m.elem_list.size() ) {
        // static
        Data_vtk_extract_elem<binary> dve;
        m.elem_list.apply_static(dve);
        apply( m.elem_list, dve, display_fields );
        for(typename Data_vtk_extract_elem<binary>::Map::const_iterator iter=dve.mapd.begin();iter!=dve.mapd.end();++iter) {
            if ( std::find(display_fields.begin(),display_fields.end(),iter->first)!=display_fields.end() or (display_fields.size()>=1 and display_fields[0]=="all") ) {    //continue;
                if ( iter->second.nb_comp ) {
                    os << "                <DataArray Name='" << iter->first
                       << "' NumberOfComponents='" << iter->second.nb_comp << "' type='" << iter->second.vtk_type << "' format='" << ( binary ? "appended" : "ascii" );
                    if ( binary )
                        os << "' offset='" << appended.size();
                    os << "'>" << endl;
                    if ( binary )
                        add_encoded( iter->second.os, appended );
                    else
                        os << iter->second.os << endl;
                    os << "                </DataArray>" << endl;
                }
            }
        }
        // dynamic
        for(unsigned i=0;i<m.elem_list.nb_dyn_data();++i) {
            const DynamicDataAncestor *dd = m.elem_list.get_dyn_data(i);
            if ( std::find(display_fields.begin(),display_fields.end(),dd->get_name())!=display_fields.end() or (display_fields.size()>=1 and display_fields[0]=="all") ) {    //continue;
                if ( dd->nb_comp()==0 )
                    continue;
                os << "                <DataArray Name='" << dd->get_name()
                        << "' NumberOfComponents='" << dd->nb_comp() << "' type='Float64' format='ascii'>" << endl;
                dd->write_data(os);
                os << endl;
                os << "                </DataArray>" << endl;
            }
        }
        // ElementAncestor.group
        if ( std::find(display_fields.begin(),display_fields.end(),"group")!=display_fields.end() or (display_fields.size()>=1 and display_fields[0]=="all") )
        {
            os << "                <DataArray Name='group' NumberOfComponents='1' type='UInt32' format='ascii'>" << endl;
            for(unsigned k_elem=0; k_elem<m.elem_list.size(); k_elem++)
                os << m.elem_list[k_elem]->group << " ";
            os << endl;
            os << "                </DataArray>" << endl;
        }
    }
    // global data
    const char *names[TM::nb_params+(TM::nb_params==0)];
    DM::get_names<TM>( names );
    unsigned nb_comp[TM::nb_params+(TM::nb_params==0)];
    DM::get_nb_comp<TM>( nb_comp );
    for(unsigned i=0;i<TM::nb_params;++i)
        nb_comp[i] += (nb_comp[i]==2);

    Data_vtk_extract<TM::nb_params,binary> dvem;
    DM::get_nb_comp<TM>( dvem.nb_comp );
    DM::apply( m, dvem );
    for(unsigned i=0;i<TM::nb_params;++i) {
        if ( std::find(display_fields.begin(),display_fields.end(),names[i])!=display_fields.end() or (display_fields.size()>=1 and display_fields[0]=="all") ) {    //continue;
            if ( nb_comp[i]==0 )
                continue;
            os << "                <DataArray Name='" << names[i]
                    << "' NumberOfComponents='" << nb_comp[i] << "' type='Float64' format='" << ( binary ? "appended" : "ascii" );
            if ( binary )
                os << "' offset='" << appended.size();
            os << "'>" << endl;
            std::string s; s.reserve( dvem.os[i].str().size() * m.elem_list.size() );
            for(unsigned k=0;k<m.elem_list.size();++k)
                s += dvem.os[i].str();
            if ( binary )
                add_encoded( s, appended );
            else
                os << s << endl;
            os << "                </DataArray>" << endl;
        }
    }
    // end CellData
    os << "            </CellData> " << endl;
    
    
    // Points
    os << "            <Points>" << endl;
    os << "                <DataArray NumberOfComponents='3' type='Float64' format='" << ( binary ? "appended" : "ascii" );
    if ( binary )
        os << "' offset='" << appended.size();
    os << "'>" << endl;
    if ( binary ) {
        std::string s; s.reserve( sizeof(double)*m.node_list.size()*3 );
        for(unsigned i=0;i<m.node_list.size();++i) {
            for(unsigned d=0;d<3;++d) {
                double v = m.node_list[i].pos[d*(TM::dim>d)]*(TM::dim>d);
                s += std::string( (char *)&v, (char *)&v+sizeof(double) );
            }
        }
        add_encoded( s, appended );
    }
    else {
        for(unsigned i=0;i<m.node_list.size();++i)
            os << m.node_list[i].pos[0] << " " <<
                    m.node_list[i].pos[1*(TM::dim>1)]*(TM::dim>1) << " " <<
                    m.node_list[i].pos[2*(TM::dim>2)]*(TM::dim>2) << endl;
    }
    os << "                </DataArray>" << endl;
    os << "            </Points> " << endl;

    // Cells
    Elem_vtk_extract<TM> es;
    es.m = &m;
    es.offsets.reserve( m.elem_list.size() );
    es.cell_types.reserve( m.elem_list.size() );
    apply( m.elem_list, es );
    os << "            <Cells>" << endl;
    os << "                <DataArray Name='connectivity' type='Int32' format='ascii'>" << endl;
    for(unsigned i=0;i<es.connectivity.size();++i)
        os << es.connectivity[i] << ( (i%8)==7 || i==es.connectivity.size()-1 ? '\n' : ' ' );
    os << "                </DataArray>" << endl;
    os << "                <DataArray Name='offsets' type='Int32' format='ascii'>" << endl;
    for(unsigned i=0;i<es.offsets.size();++i)
        os << es.offsets[i] << ( (i%8)==7 || i==es.offsets.size()-1 ? '\n' : ' ' );
    os << "                </DataArray>" << endl;
    os << "                <DataArray Name='types' type='Int32' format='ascii'>" << endl;
    for(unsigned i=0;i<es.cell_types.size();++i)
        os << es.cell_types[i] << ( (i%8)==7 || i==es.cell_types.size()-1 ? '\n' : ' ' );
    os << "                </DataArray>" << endl;
    os << "            </Cells>" << endl;
    
    os << "        </Piece>" << endl;
    os << "    </UnstructuredGrid>" << endl;
    
    // AppendedData
    os << "    <AppendedData encoding='base64'>" << endl;
    os << "        _";

    //std::string s; s.resize( 12 + 2 * appended.size() );
    //unsigned long l_s = s.size();
    //compress( (Bytef *)&s[0], &l_s, (Bytef *)&appended[0], appended.size() );
    //s.resize( l_s );
    os << appended;

    os << "\n    </AppendedData>" << endl;

    os << "</VTKFile>" << endl;

}

template<bool binary,class TM>
void write_mesh_vtk_v2( std::ostream &os, const TM &m, const Vec<std::string> &display_fields = Vec<std::string>("all") ) {
    using namespace std;

    os << "# vtk DataFile Version 3.0" << endl;
    os << "vtk output" << endl;
    os << "ASCII" << endl;
    os << "DATASET UNSTRUCTURED_GRID" << endl;
    
    // POINTS
    os << "POINTS " << m.node_list.size() <<" float " << endl;
    for(unsigned i=0;i<m.node_list.size();++i)
        os << m.node_list[i].pos[0] << " " <<
        m.node_list[i].pos[1*(TM::dim>1)]*(TM::dim>1) << " " <<
        m.node_list[i].pos[2*(TM::dim>2)]*(TM::dim>2) << endl;

    // CELLS
    Elem_vtk_extract<TM> es;
    es.m = &m;
    es.cell_types.reserve( m.elem_list.size() );
    apply( m.elem_list, es );

    // total number of nodes per element + number of elements
    unsigned nbtot=0;
    for(unsigned i=0;i<m.elem_list.size();++i)
        nbtot+=m.elem_list[i]->nb_nodes_virtual();
    nbtot+=m.elem_list.size();
    
    os << "CELLS " << m.elem_list.size() << " " << nbtot << endl;
    for(unsigned i=0;i<m.elem_list.size();++i) {
        os << m.elem_list[i]->nb_nodes_virtual() << " ";
        for(unsigned j=0;j<m.elem_list[i]->nb_nodes_virtual();++j)
            os << m.elem_list[i]->node_virtual(j)->number_in_original_mesh() << " ";
        os << endl;
    }

    // CELL_TYPES
    os << "CELL_TYPES " << m.elem_list.size()  << endl;
    for(unsigned i=0;i<es.cell_types.size();++i)
        os << es.cell_types[i] << endl;

    // CELL_DATA
    os << "CELL_DATA " << m.elem_list.size() << endl;
    if ( m.elem_list.size() ) {
        // static
        Data_vtk_extract_elem<binary> dve;
        m.elem_list.apply_static(dve);
        apply( m.elem_list, dve, display_fields );
        unsigned nb_field=0;
        // 1ere passe pour reperer les scalaires et vecteurs
        for(typename Data_vtk_extract_elem<binary>::Map::const_iterator iter=dve.mapd.begin();iter!=dve.mapd.end();++iter) {
            if ( std::find(display_fields.begin(),display_fields.end(),iter->first)!=display_fields.end() or (display_fields.size()>=1 and display_fields[0]=="all") ) {
                nb_field+=1;
                //std::cout << iter->first << endl;
            }
        }
        //std::cout << nb_field << endl;
        os << "FIELD FieldData " << nb_field << endl;
        // 2ere passe
        for(typename Data_vtk_extract_elem<binary>::Map::const_iterator iter=dve.mapd.begin();iter!=dve.mapd.end();++iter) {
            if ( std::find(display_fields.begin(),display_fields.end(),iter->first)!=display_fields.end() or (display_fields.size()>=1 and display_fields[0]=="all") ) {    //continue;
                if (iter->second.nb_comp>1) {
                    os << iter->first << " " << iter->second.nb_comp << " " << m.elem_list.size() << " float " << endl;
                    os << iter->second.os << endl;
                    os << endl;
                }
            }
        }

        // dynamic
    }
    
    // POINT_DATA
    os << "POINT_DATA " << m.node_list.size() << endl;
    if ( m.node_list.size() ) {
        // static
        const char *names[TM::TNode::nb_params+(TM::TNode::nb_params==0)];
        DM::get_names<typename TM::TNode>( names );
        unsigned nb_comp[TM::TNode::nb_params+(TM::TNode::nb_params==0)];
        DM::get_nb_comp<typename TM::TNode>( nb_comp );
        for(unsigned i=0;i<TM::TNode::nb_params;++i)
            nb_comp[i] += (nb_comp[i]==2);
        GetDynamicSize<TM::TNode::nb_params> gds;
        for(unsigned i=0;i<m.node_list.size();++i)
            DM::apply( m.node_list[i], gds );

        Data_vtk_extract<TM::TNode::nb_params,binary> dve;
        DM::get_nb_comp<typename TM::TNode>( dve.nb_comp );
        for(unsigned i=0;i<m.node_list.size();++i)
            DM::apply(m.node_list[i],dve);

        os << "FIELD FieldData " << TM::TNode::nb_params << endl;

        for(unsigned i=0;i<TM::TNode::nb_params;++i) {
            if ( std::find(display_fields.begin(),display_fields.end(),std::string(names[i]))!=display_fields.end() or (display_fields.size()>=1 and display_fields[0]=="all") ) {    //continue;
                if ( names[i]!=const_cast<char *>("pos") and nb_comp[i] ) {
                    os << names[i] << " " << nb_comp[i] << " " << m.node_list.size() << " float " << endl;
                    os << dve.os[i].str();
                    os << endl;
                }
            }
        }

        // static with dynamic size ( vectors )
        for(unsigned i=0;i<TM::TNode::nb_params;++i) {
            if ( std::find(display_fields.begin(),display_fields.end(),std::string(names[i]))!=display_fields.end() or (display_fields.size()>=1 and display_fields[0]=="all") ) {    //continue;
                for(unsigned j=0;j<gds.dynamic_size[i];++j) {
                    os << names[i] << " " << gds.nb_comp[i] << " " << m.node_list.size() << " float " << endl;
                    GetDynamicSizeOs gdsos = { os, i, j };
                    for(unsigned i=0;i<m.node_list.size();++i)
                        DM::apply(m.node_list[i],gdsos);
                    os << endl;
                }
            }
        }

//        // dynamic
//        for(unsigned i=0;i<m.node_list.nb_dyn_data();++i) {
//            const DynamicDataAncestor *dd = m.node_list.get_dyn_data(i);
//            if ( std::find(display_fields.begin(),display_fields.end(),std::string(dd->get_name()))!=display_fields.end() or (display_fields.size()>=1 and display_fields[0]=="all") )
//            {    //continue;
//                if ( dd->nb_comp()==0 ) continue;
//                os << "                <DataArray Name='" << dd->get_name()
//                   << "' NumberOfComponents='" << dd->nb_comp() << "' type='Float64' format='ascii'>" << endl;
//                dd->write_data(os);
//                os << endl;
//                os << "                </DataArray>" << endl;
//            }
//        }
    }

//    // global data
//    const char *names[TM::nb_params+(TM::nb_params==0)];
//    DM::get_names<TM>( names );
//    unsigned nb_comp[TM::nb_params+(TM::nb_params==0)];
//    DM::get_nb_comp<TM>( nb_comp );
//    for(unsigned i=0;i<TM::nb_params;++i)
//        nb_comp[i] += (nb_comp[i]==2);
    
//    Data_vtk_extract<TM::nb_params,binary> dvem;
//    DM::get_nb_comp<TM>( dvem.nb_comp );
//    DM::apply( m, dvem );
//    for(unsigned i=0;i<TM::nb_params;++i) {
//        if ( std::find(display_fields.begin(),display_fields.end(),names[i])!=display_fields.end() or (display_fields.size()>=1 and display_fields[0]=="all") )
//        {    //continue;
//            if ( nb_comp[i]==0 ) continue;
//            os << "                <DataArray Name='" << names[i]
//                  << "' NumberOfComponents='" << nb_comp[i] << "' type='Float64' format='" << ( binary ? "binary" : "ascii" ) << "'>";
//            for(unsigned k=0;k<m.elem_list.size();++k) {
//                if ( binary ) {
////                    os << "_";
////                    std::string s = dvem.os[i].str();
////                    char o0,o1,o2,o3;
////                    unsigned k;
////                    for(k=0;k<s.size()-2;k+=3) { EncodeTriplet(s[k+0],s[k+1],s[k+2],&o0,&o1,&o2,&o3); os.write(&o0,1); os.write(&o1,1); os.write(&o2,1); os.write(&o3,1); }
////                    for(;k<s.size()-1;k+=2) { EncodePair(s[k+0],s[k+1],&o0,&o1,&o2,&o3); os.write(&o0,1); os.write(&o1,1); os.write(&o2,1); os.write(&o3,1); }
////                    for(;k<s.size();++k) { EncodeSingle(s[k+0],&o0,&o1,&o2,&o3); os.write(&o0,1); os.write(&o1,1); os.write(&o2,1); os.write(&o3,1); }
//                    os << "_";
//                    os << *dvem.osvtk[i];
//                }
//                else os << dvem.os[i].str();
//            }
//            os << "                </DataArray>" << endl;
//        }
//    }
    

}

template<class TM>
void write_mesh_vtk( std::string filename, const TM &m, const Vec<std::string> &display_fields = Vec<std::string>("all") ) {
    std::ofstream os( filename.c_str() );
    write_mesh_vtk<true>( os, m, display_fields );
}

};

#endif // LMT_WRITEMESHVTK_HEADER
