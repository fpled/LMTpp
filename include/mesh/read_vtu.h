#ifndef LMT_READ_VTU_H
#define LMT_READ_VTU_H

#include <sstream>
#include <map>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <string.h> /// pour strcmp

#include "write_mesh_vtk.h"
#include "triangle.h"
#include "triangle_6.h"
#include "quad.h"
#include "quad_8.h"
#include "tetra.h"
#include "hexa.h"
#include "pyramid.h"
#include "wedge.h"
#include "../io/xmlnode.h"

namespace LMT {

/*!
enlève les caractères de la liste l dans la chaîne s

\keyword Utilitaires
*/
void cut_char( std::string& s, const char* l) {
    int i = 0;
    while ( i < s.size() )
        if ( strchr( l, s[ i ] ) )
            s.erase( i, 1 );
        else
            ++i;
    
}

/*!
L'objectif de cet objet est de connaître les noms des champs d'un élément ainsi que le nombre de composantes par champs.

*/
template<class TE>
struct GetListNamefieldAndCo {
    GetListNamefieldAndCo() {
        names.resize( TE::nb_params );
        DM::get_names<TE>( &names[0] );
        nb_comp.resize( TE::nb_params );
        DM::get_nb_comp<TE>( &nb_comp[0] );
    }
    
    std::vector<const char*> names;
    std::vector<unsigned> nb_comp;
};

/*!
    Ce foncteur permet, via l'appel à DM::apply_with_names(), d' assigner une valeur T ou un vecteur Vec<T,s> à un champ nommé field_name.
    index_node est l'indice du noeud courant et nb_comp est la taille du vecteur des composantes ( nb_comp == s à priori).
*/
template<class TDATA>
struct SetFieldByName {
    SetFieldByName( const std::string& name_, int nb_comp_) : field_name(name_), nb_comp(nb_comp_) {}
    //     template<class T>
    //     void operator()(unsigned num,const char *name,T &val) const {
    //     }

    template<class T>
    void operator()( unsigned num, const char *name, T &val ) {
        if ( name == field_name ) {
            //std::cout << "======== scalaire a " << nb_comp << " composantes"<< std::endl;
            val = T( data[ index_elt ] ); /// nb_comp == 1
        }
    }

    template<class T,int s>
    void operator()( unsigned num, const char *name, Vec<T,s> &val ) {
        if ( name == field_name ) {
            //std::cout << "======== vecteur a " << nb_comp << " composantes et s = "<< s << std::endl;
            for( int i = 0; i < s; ++i)
                val[ i ] = T( data[ index_elt * nb_comp + i ] );
        }
    }
    
    template<class T,int s1,int s2>
    void operator()( unsigned num, const char *name, Vec<Vec<T,s1>,s2> &val ) {
        if ( name == field_name ) {
            //std::cout << "======== vecteur a " << nb_comp << " composantes et s = "<< s << std::endl;
            for( int i = 0, c = 0; i < s2; ++i )
                for( int j = 0; j < s1; ++j, ++c )
                    val[ i ][ j ] = T( data[ index_elt * nb_comp + c ] );
        }
    }

    //     template<class T, class Structure, class O, class OP>
    //     void operator()( unsigned num, const char *name, Mat<T, Structure, O, OP > &val ) {
    //         PRINT( name ); PRINT( nb_comp );
    //         operator() ( num, name, val.data );
    //     }

    template<class T,int s >
    void operator()( unsigned num, const char *name, Mat<T, Sym<s>, Dense<Col> > &val ) {
        if ( name == field_name ) {
            for( int j = 0; j < val.nb_cols(); ++j ) {
                for( int i = 0; i <= j; ++i ) {
                    val( i, j ) = T( data[ index_elt * nb_comp + j * ( j + 1 ) / 2 + i ] );
                }
            }
        }
    }

    template<class T, int sr, int sc>
    void operator()( unsigned num,const char *name, Mat<T, Gen<sr, sc>, Dense<Col> > &val ) {
        if ( name == field_name ) {
            for( int i = 0; i < val.nb_rows(); ++i) {
                for( int j = 0; j < val.nb_cols(); ++j) {
                    val( i, j ) = T( data[ index_elt * nb_comp + i * val.nb_cols() + j ] );
                }
            }
        }
    }
    
    unsigned index_elt;
    std::string field_name;
    int nb_comp;
    Vec< TDATA > data;

};

/*!
    Ce double foncteur permet, via l'appel apply(Vec< VecElemList<...> > ) puis à DM::appl_with_names(), d' assigner une valeur T ou un vecteur Vec<T,s> à un champ nommé field_name dans un élément.
    index_elt est l'indice de l'élément courant et nb_comp est la taille du vecteur des composantes ( nb_comp == s à priori).

    On fait un foncteur différent de SetNodeFieldByName car m.elem_list est un vecteur hétérogène.

    Concrètement TDynamicData sera du type DynamicData<int, TM::ElemList::nb_elem_type >
    Je ne sait pas à quoi sert ce type dans l'appel de apply(Vec< VecElemList<...> > ) ...
 */
template<class TDATA, class TDynamicData, class TM>
struct SetElemFieldByName : public SetFieldByName<TDATA> {
    typedef SetFieldByName<TDATA> TP;
    SetElemFieldByName( const std::string& name_, int nb_comp_, TDynamicData* dd_, TM* m_) : SetFieldByName<TDATA>( name_, nb_comp_), dd(dd_), m(m_)  {}

    template<class T,int s>
    void operator()(unsigned num,const char *name,Vec<T,s> &val) {
        TP::operator()( num, name, val );
    }
    
    template<class T>
    void operator()(unsigned num,const char *name,T &val) {
        TP::operator()( num, name, val );
    }
    
    template<class TE>
    void operator()(TE &e) {
        this->index_elt = m->elem_list.get_data( *dd, e);
        DM::apply_with_names( e, *this );
    }

    TDynamicData* dd;
    TM* m;
};

/*!
Ce code est récupéré du fichier vtkBase64Utilities.cxx du programme vtk-5.4.2.
*/
const unsigned char vtkBase64UtilitiesDecodeTable[256] = {
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0x3E,0xFF,0xFF,0xFF,0x3F,
    0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,
    0x3C,0x3D,0xFF,0xFF,0xFF,0x00,0xFF,0xFF,
    0xFF,0x00,0x01,0x02,0x03,0x04,0x05,0x06,
    0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,
    0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16,
    0x17,0x18,0x19,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,
    0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,
    0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,0x30,
    0x31,0x32,0x33,0xFF,0xFF,0xFF,0xFF,0xFF,
    //-------------------------------------
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};

//----------------------------------------------------------------------------
/*!
Ce code est récupéré du fichier vtkBase64Utilities.cxx du programme vtk-5.4.2.
*/
inline unsigned char vtkBase64UtilitiesDecodeChar(unsigned char c) {
    return vtkBase64UtilitiesDecodeTable[c];
}

/*!
  Description:
  Decode 4 bytes into 3 bytes.
  Return the number of bytes actually decoded
  
  Ce code est récupéré du fichier vtkBase64Utilities.h du programme vtk-5.4.2.

*/
int decodeTriplet(unsigned char i0, unsigned char i1, unsigned char i2, unsigned char i3, unsigned char *o0, unsigned char *o1, unsigned char *o2) {
    unsigned char d0, d1, d2, d3;

    d0 = vtkBase64UtilitiesDecodeChar(i0);
    d1 = vtkBase64UtilitiesDecodeChar(i1);
    d2 = vtkBase64UtilitiesDecodeChar(i2);
    d3 = vtkBase64UtilitiesDecodeChar(i3);

    /// Make sure all characters were valid

    if (d0 == 0xFF || d1 == 0xFF || d2 == 0xFF || d3 == 0xFF) return 0;

    /// Decode the 3 bytes

    *o0 = ((d0 << 2) & 0xFC) | ((d1 >> 4) & 0x03);
    *o1 = ((d1 << 4) & 0xF0) | ((d2 >> 2) & 0x0F);
    *o2 = ((d2 << 6) & 0xC0) | ((d3 >> 0) & 0x3F);

    /// Return the number of bytes actually decoded

    if (i2 == '=') return 1;
    if (i3 == '=') return 2;
    return 3;
}

/*!
    Ce code est récupéré du fichier vtkBase64Utilities.cxx du programme vtk-5.4.2.

  // Description:
  // Decode bytes from the input buffer and store the decoded stream
  // into the output buffer until 'length' bytes have been decoded.
  // Return the real length of the decoded stream (which should be equal to
  // 'length'). Note that the output buffer must be allocated by the caller.
  // If 'max_input_length' is not null, then it specifies the number of
  // encoded bytes that should be at most read from the input buffer. In
  // that case the 'length' parameter is ignored. This enables the caller
  // to decode a stream without actually knowing how much decoded data to
  // expect (of course, the buffer must be large enough).


*/
// unsigned int decode(const unsigned char *input,  unsigned long length, unsigned char *output, unsigned long max_input_length) {
inline unsigned int decode(const std::string &input, unsigned int offset,  unsigned int ilength, std::string &output ) {
    //     unsigned char optr[9];
    typedef unsigned char* puc;
    unsigned char *ptr = puc(input.c_str()) + offset;
    unsigned int len, i, j, nb_decoded=0;
    unsigned char temp, temp2;
    //     unsigned char *optr = output;

    //ilength = min( ilength, input.size()-offset);
    output.clear();
    output.resize(3./4*double(ilength)+5.);
    
    /// on décode les 12 premiers octets en base 64 pour récupérer les 9 premiers octets, sachant que les 4 ou 8 premiers sont le nombre d'octets en base 256 encodés...
    for( i=0, j=0; i < ilength; i += 4, j+=3) {
        len = decodeTriplet(ptr[i], ptr[i+1], ptr[i+2], ptr[i+3], (puc) &output[j], (puc) &output[j+1], (puc) &output[j+2]);
        nb_decoded += len;
        if (len < 3)
            return nb_decoded; /// sortie anormale
    }
    /// Decode the last triplet
    ilength = ilength+4-i;
    j-=3;
    if (ilength == 3) {
        len = decodeTriplet(ptr[0], ptr[1], ptr[2], ptr[3], (puc) &output[j], (puc) &output[j+1], (puc) &output[j+2] );
    } else if (ilength == 2) {
        len = decodeTriplet(ptr[0], ptr[1], ptr[2], ptr[3], (puc) &output[j], (puc) &output[j+1], &temp);
    } else if (ilength == 1) {
        len = decodeTriplet(ptr[0], ptr[1], ptr[2], ptr[3], (puc) &output[j], &temp, &temp2);
    }
    return nb_decoded + len;
}

/// problème : dépendance l'OS peut-être ???
unsigned sizeTypeVtk (const std::string& type) {
    if (type == "Float64")
        return 8;
    else if (type == "Float32")
        return 4;
    else if (type == "UInt64")
        return 8;
    else if (type == "Int64")
        return 8;
    else if (type == "UInt32")
        return 4;
    else if (type == "Int32")
        return 4;
    else if (type == "UInt16")
        return 2;
    else if (type == "Int16")
        return 2;
    else if (type == "UInt8")
        return 1;
    else if (type == "Int8")
        return 1;
    else
        return 0;
}

struct DataArray {
    DataArray() : offset(0), numberOfComponents(0), format(0) {}
    DataArray( const std::string name_, const std::string type_, int format_, int nb_comp, unsigned int offset_, std::string& content_) : name(name_), type(type_), format(format_), numberOfComponents(nb_comp), offset(offset_), content(content_) {
        if (format > 1) {
            const char* delchar = "_\n ";
            cut_char( content, delchar );
        }
    }
    
    /**
    Entrée : offset est un entier de la liste *ptr_offsets et *ptr_offsets est une liste d'entiers strictement croissante.

    si offset n'est pas dans la liste *ptr_offsets, la fonction renvoie 0 sinon elle renvoie la différence avec l'offset supérieur s'il existe sinon elle renvoie 0
     */
    unsigned get_length( unsigned int offset ) {
        int i;

        for(i=0; i < ptr_offsets->size(); ++i)
            if ((*ptr_offsets)[i] == offset)
                break;
        if (i == ptr_offsets->size())
            return 0;
        if (offset == ptr_offsets->back())
            return 0;
        return (*ptr_offsets)[i+1] - offset;
    }
    
    /*!
        ATTENTION : nb_val est le nombre de valeurs à extraire. Si les données sont au format ascii, il est facultatif. Par contre il est obligatoire pour les formats binary et append.
    */
    template <class T>
    bool extract_content(Vec<T>& v,  unsigned nb_val=0 ) {
        std::string exdata;
        unsigned size_val = sizeTypeVtk(type); // sizeof(T);
        unsigned nb_decoded;
        unsigned int size_encoded, delay; /// sizeof( unsigned long int ) == 4 or 8 depends on operating system and CPU. delay car sur les premiers octets il est enregistré le nombre d'octets encodés moins la taille de unsigned int qui contient cette taille. Or la taille de unsigned int du système qui a créait le fichier .vtu est inconnue...
        
        v.resize(0);
        if ( type.compare( GetVtkType<T>::res() ) ) {
            std::cerr << " warning : types of VTK file and mesh are different. VTK.type = " << type << " and GetVtkType<T>::res() = " << GetVtkType<T>::res() << ". Extraction may be has failures." << std::endl;
        }
        if (format == 0) {
            std::cerr << " error: unknow format : failure to extract datas of " << name << std::endl;
            return false;
        } else if (format == 1) { /// format ascii
            std::istringstream is( content );
            is >> v;
            //if ( !is ) throw IoException( "Error while reading content of : " + name );
        } else if (format == 2) { /// format binaire
            nb_decoded = decode( content, 0, content.size(), exdata );

            /// i suppose that it's on litteleEndian
            memcpy((void*) &size_encoded, (const void*) (exdata.c_str()), 4 );
            /// heuristique
            delay = 4;
            //             if (size_encoded <= nb_decoded + 4 )
            //                 delay = 4;
            //             else
            //                 delay = 8;

            size_encoded = size_val * nb_val + delay;
            if ((size_val) and (nb_decoded > size_encoded)) {
                T d;
                /**
                        Pourquoi delay ?
                        Les premiers octets décodés contiennent la taille en octet des données encodés. Seulement suivant le type de système d'exploitation ou CPU, cette taille peut être codée sur 4 ou 8 octets.
                        Comment savoir sur quelque système le fichier a-t-il été généré ?
                     */
                for (int i=delay; i< size_encoded; i += size_val ) {
                    memcpy((void*) &d, (const void*) (exdata.c_str()+i), size_val );
                    //PRINT( d ) ;
                    v.push_back( d );
                }
            }
        } else if (format == 3) { /// format "tas" ou "append"
            if ((ptr_append == NULL) or (ptr_offsets == NULL)) {
                std::cerr << " error : ptr_append == NULL or ptr_offsets == NULL " << std::endl;
                return false;
            }
            unsigned len = get_length( offset );
            if (len == 0) {
                std::cerr << " error : problem with the offset " << std::endl;
                return false;
            }
            nb_decoded = decode( *ptr_append, offset, len , exdata );
            if ( nb_decoded < 8) {
                std::cerr << " error : problem to decode. Number of decoded bytes = " << nb_decoded << std::endl;
                return false;
            }
            /// i suppose that it's on litteleEndian
            memcpy((void*) &size_encoded, (const void*) (exdata.c_str()), 4 );
            //PRINT( size_encoded );
            delay = 4;
            size_encoded = size_val * nb_val + delay;
            //PRINT( nb_decoded ); PRINT( size_encoded ); PRINT( size_val ); PRINT( nb_val );
            if ( ( size_val ) and ( nb_decoded >= size_encoded ) ) {
                T d;
                /**
                        Pourquoi delay ?
                        Les premiers octets décodés contiennent la taille en octet des données encodés. Seulement suivant le type de système d'exploitation ou CPU, cette taille peut être codée sur 4 ou 8 octets.
                        Comment savoir sur quelque système le fichier a-t-il été généré ?
                    */
                for ( int i = delay; i < size_encoded; i += size_val ) {
                    memcpy((void*) &d, (const void*) (exdata.c_str()+i), size_val );
                    //PRINT( d );
                    v.push_back( d );
                }
            }
        } else
            std::cerr << "error : unknow format; failure to extract datas of " << name << std::endl;

        if (v.size() == nb_val)
            return true;
        else
            return false;
    }
    
    void print( std::ostream& os ) const {
        os << " name = " << name << " type = " << type << " format = " << format << " numberOfComponents = " << numberOfComponents << " offset = " << offset << " content = ||]" << content << "[||";

    }
    //static void push_back( unsigned i) { offsets.push_back( i ); }
    //static void assign_offsets( std::vector<unsigned>* p) { ptr_offsets = p; }
    //static void assign_append( std::string* p) { ptr_append = p; }
    
    std::string name;
    std::string type;
    std::string content;
    int format; /// 1 pour ascii, 2 pour binary, 3 pour appended et 0 pour un format inconnu
    int numberOfComponents;
    unsigned int offset;
    static std::string* ptr_append;
    //static std::string append;
    static std::vector<unsigned>* ptr_offsets;
    //static std::vector<unsigned> offsets;
};

std::string* DataArray::ptr_append = NULL;
std::vector<unsigned>* DataArray::ptr_offsets = NULL;

std::ostream& operator<< ( std::ostream& os, const DataArray& d ) {
    d.print(os);
    return os;
}


/*!
    Cette fonction récupère tous les DataArray de x.
 */
void extract_DataArray( std::map< std::string, DataArray > & v, const XmlNode& x) {

    std::string s, name;
    int f;
    unsigned int of;
    for(unsigned i=0;i<x.nb_elements();++i) {
        XmlNode xx = x.get_element("DataArray",i);
        s = xx.get_attribute( "format");
        if (s == "ascii")
            f = 1;
        else if (s == "binary")
            f = 2;
        else if (s == "appended")
            f = 3;
        else
            f = 0;
        if (xx.has_attribute("offset") ) {
            std::istringstream iss(xx.get_attribute( "offset" ));
            iss >> of;
        } else
            of = 0;
        s = xx.get_content();
        if (xx.has_attribute("Name") )
            name = xx.get_attribute( "Name" );
        else {
            std::ostringstream oss("Name__");
            oss << i;
            name = oss.str();
        }
        v[name] = DataArray( name,
                             (xx.has_attribute("type") ) ? xx.get_attribute( "type" ) : "",
                             f,
                             (xx.has_attribute("NumberOfComponents") ) ? atoi(xx.get_attribute( "NumberOfComponents" ).c_str()) : -1,
                             of,
                             s ) ;
    }
}

/*!
    Objectif:
        Lire un fichier au format vtu pour construire un maillage "m".
        
    Paramètres :
        * m est un maillage de la plate-forme. Ces champs doivent correspondre à ceux du fichier vtu. Les champs globaux, nodaux et par éléments sont gérés. ATTENTION : Les champs au points de Gauss ne sont pas validés.
        * filename est le nom du fichier.

    Exemple de code :
    \code C/C++
        read_vtu( m, "essai_0_0.vtu" );
        
    WARNING : elle ne marche que pour les champs scalaires ou vectoriels donc pas les matrices symétriques et autres structures plus complexes.

    \keyword Maillage/Import
    \friend samir.amrouche@lmt.ens-cachan.fr
*/
template<class TM>
void read_vtu(TM &m, const char* filename ) {
    typedef typename TM::Pvec Pvec;
    typedef typename TM::TNode TNode;
    typedef typename TM::TNode::T T;
    typedef typename TM::EA EA;
    typedef std::map<std::string, DataArray> MapDataArray;
    typedef std::map<std::string, DataArray>::iterator ItMapDataArray;
    typedef DynamicData< int, TM::TElemList::nb_elem_type> DD;
    DD dd("global_number");
    std::string append;
    MapDataArray pointdatas;
    MapDataArray celldatas;
    MapDataArray points;
    MapDataArray cells;
    std::vector<unsigned> offsets;
    
    /// TODO : vérifier que le suffix est vtu
    XmlNode xn( filename );
    if (strcmp( xn.get_name(), "VTKFile" ) != 0)
        throw (std::runtime_error( " Error : not VTKFile"));
    if (xn.has_attribute("type")) {
        if (strcmp(xn.get_attribute("type").c_str(),"UnstructuredGrid") != 0)
            throw (std::runtime_error( " only UnstructuredGrid type is supported" ));
    } else
        throw (std::runtime_error( " VTKFile file as no attribute type"));
    if (xn.has_attribute("byte_order")) {
        if (strcmp(xn.get_attribute("byte_order").c_str(),"LittleEndian") != 0)
            throw (std::runtime_error( " only LittleEndian format is supported"));
    } else
        throw (std::runtime_error( " VTKFile file as no attribute byte_order"));
    
    
    /// récupération de UnstructuredGrid
    XmlNode ug = xn.get_element("UnstructuredGrid");
    XmlNode piece = ug.get_element("Piece");
    int nb_nodes = atoi( piece.get_attribute( "NumberOfPoints" ).c_str() );
    int nb_elem  = atoi( piece.get_attribute( "NumberOfCells" ).c_str() );
    //PRINT( nb_nodes );
    //PRINT( nb_elem );
    /// extraction du contenu de PointData
    XmlNode pointdata = piece.get_element("PointData");
    extract_DataArray( pointdatas, pointdata );
    //PRINT( pointdatas );
    /// extraction du contenu de CellData
    XmlNode celldata = piece.get_element("CellData");
    extract_DataArray( celldatas, celldata );
    //PRINT( celldatas );
    /// extraction du contenu de Points
    XmlNode point = piece.get_element("Points");
    extract_DataArray( points, point );
    //PRINT( points );
    /// extraction du contenu de Cells
    XmlNode cell = piece.get_element("Cells");
    extract_DataArray( cells, cell );
    //PRINT( cells );
    /// récupération des offsets
    for( ItMapDataArray i=pointdatas.begin(); i != pointdatas.end() ;++i)
        if (i->second.format == 3)
            offsets.push_back(i->second.offset);
    for( ItMapDataArray i=celldatas.begin(); i != celldatas.end(); ++i)
        if (i->second.format == 3)
            offsets.push_back(i->second.offset);
    for( ItMapDataArray i=points.begin(); i != points.end(); ++i)
        if (i->second.format == 3)
            offsets.push_back(i->second.offset);
    for( ItMapDataArray i=cells.begin(); i != cells.end();++i)
        if (i->second.format == 3)
            offsets.push_back(i->second.offset);
    std::sort( offsets.begin(), offsets.end() );
    
    /// extraction de AppendedData
    XmlNode xappend;
    if (xn.nb_elements("AppendedData")) {
        xappend = xn.get_element("AppendedData");
        append = xappend.get_content();
        const char* delchar = "_\n ";
        cut_char( append, delchar );
    }
    offsets.push_back( append.size() );
    DataArray::ptr_offsets = &offsets;
    DataArray::ptr_append = &append;
    //for(int i=0;i<offsets.size();++i) std::cout <<  offsets[i] << std::endl ;

    /// construction du maillage
    if (( points.size() == 0) or (points.size() > 1))
        throw (std::runtime_error( " no point in VTK file or too much "));
    ItMapDataArray it = points.begin();
    if ( it->second.type != GetVtkType<T>::res() )
        throw (std::runtime_error( " types of VTK file and LMT mesh are different"));
    if ( it->second.numberOfComponents == 0)
        throw (std::runtime_error( " numberOfComponents == 0 in DataArray"));
    
    int n_comp = it->second.numberOfComponents;
    Vec<T> v;
    it->second.extract_content( v, n_comp*nb_nodes );
    //PRINT( nb_nodes ); PRINT( v.size() );
    
    if (v.size() != n_comp*nb_nodes)
        throw (std::runtime_error( " number of nodes is different of extracted points"));
    
    for( int i=0; i<v.size(); i+= n_comp ) {
        Pvec pos;
        for( int j=0; j < TM::dim; ++j )
            pos[j] = v[i+j];
        m.add_node( pos );
    }
    
    /// assignation des attributs des noeuds du maillage
    for ( it = pointdatas.begin(); it != pointdatas.end(); ++it) {
        //std::cout << "---------------" << std::endl;
        //PRINT( it->first ); PRINT( it->second.numberOfComponents );
        SetFieldByName<float> setfield_float( it->first, it->second.numberOfComponents );
        SetFieldByName<double> setfield_double ( it->first, it->second.numberOfComponents );
        unsigned taille_data = nb_nodes * it->second.numberOfComponents;
        //PRINT( taille_data );
        //PRINT( m.node_list.size() ); PRINT( setfield.data.size() ); PRINT( setfield.nb_comp ); PRINT( setfield.data );
        if (it->second.type == "Float32") {
            it->second.extract_content( setfield_float.data, taille_data );
            for( unsigned i = 0; i< m.node_list.size(); ++i ) {
                setfield_float.index_elt = i;
                DM::apply_with_names( m.node_list[i], setfield_float );
            }
        } else {
            it->second.extract_content( setfield_double.data, taille_data );
            //PRINT( setfield_double.data );
            for( unsigned i = 0; i < m.node_list.size(); ++i ) {
                setfield_double.index_elt = i;
                DM::apply_with_names( m.node_list[i], setfield_double );
            }
        }
    }
    /// récupération de la connectivité, des types et des offsets des éléments du maillage.
    Vec<int> connectivity, typeVtk, offset_elemVtk;
    cells["types"].extract_content( typeVtk, nb_elem );
    cells["offsets"].extract_content( offset_elemVtk, nb_elem );
    cells["connectivity"].extract_content( connectivity, offset_elemVtk.back() );

    /// génération des éléments
    Vec<TNode*> vn;
    EA* ne;
    int begin =0, end;
    for( int i=0; i<typeVtk.size(); ++i) {
        vn.resize(0);
        end = offset_elemVtk[i];
        for( int j=begin; j < end; ++j)
            vn.push_back( &(m.node_list[connectivity[j]]) );
        
        switch(typeVtk[i]) {
        case 5 :
            permutation_if_jac_neg ( Triangle(), vn.ptr() );
            ne = m.add_element( Triangle(),DefaultBehavior(),vn.ptr() );
            m.elem_list.synchronize_dyn(&dd);
            m.elem_list.get_data(dd, *ne) = i;
            break;
        case 8 : /// VTK_PIXEL
            permutation_if_jac_neg ( Quad(), vn.ptr() );
            ne =  (m.add_element( Quad(),DefaultBehavior(),vn.ptr() ) );
            m.elem_list.synchronize_dyn(&dd);
            m.elem_list.get_data(dd, *ne) = i;
            break;
        case 9 :
            permutation_if_jac_neg ( Quad(), vn.ptr() );
            ne = (m.add_element( Quad(),DefaultBehavior(),vn.ptr() ) );
            m.elem_list.synchronize_dyn(&dd);
            m.elem_list.get_data(dd, *ne) = i;
            break;
        case 10 :
            permutation_if_jac_neg ( Tetra(), vn.ptr() );
            ne =  (m.add_element( Tetra(),DefaultBehavior(),vn.ptr() ) );
            m.elem_list.synchronize_dyn(&dd);
            m.elem_list.get_data(dd, *ne) = i;
            break;
        case 11 : /// VTK_VOXEL
            permutation_if_jac_neg ( Hexa(), vn.ptr() );
            ne =  (m.add_element( Hexa(),DefaultBehavior(),vn.ptr() ) );
            m.elem_list.synchronize_dyn(&dd);
            m.elem_list.get_data(dd, *ne) = i;
            break;
        case 12 :
            permutation_if_jac_neg ( Hexa(), vn.ptr() );
            ne =  (m.add_element( Hexa(),DefaultBehavior(),vn.ptr() ) );
            m.elem_list.synchronize_dyn(&dd);
            m.elem_list.get_data(dd, *ne) = i;
            break;
        case 13 :
            permutation_if_jac_neg ( Wedge(), vn.ptr() );
            ne =  (m.add_element( Wedge(),DefaultBehavior(),vn.ptr() ) );
            m.elem_list.synchronize_dyn(&dd);
            m.elem_list.get_data(dd, *ne) = i;
            break;
        case 14 :
            //permutation_if_jac_neg ( Pyramid(), vn.ptr() );
            ne =  (m.add_element( Pyramid(),DefaultBehavior(),vn.ptr() ) );
            m.elem_list.synchronize_dyn(&dd);
            m.elem_list.get_data(dd, *ne) = i;
            break;
        case 22 :
            permutation_if_jac_neg ( Triangle_6(), vn.ptr() );
            ne =  ( m.add_element( Triangle_6(),DefaultBehavior(),vn.ptr() ) );
            m.elem_list.synchronize_dyn(&dd);
            m.elem_list.get_data(dd, *ne) = i;
            break;
        case 23 :
            permutation_if_jac_neg ( Quad_8(), vn.ptr() );
            ne =  ( m.add_element( Quad_8(),DefaultBehavior(),vn.ptr() ) );
            m.elem_list.synchronize_dyn(&dd);
            m.elem_list.get_data(dd, *ne) = i;
            break;
        default :
            std::cerr << "warning : the element number " << typeVtk[i] << " is not implemented." << std::endl;
        }
        begin = end;
    }
    
    /// assignation des attributs des éléments du maillage
    GetListNamefieldAndCo<typename TM::TElemList::template SubType<0>::T> gnf;
    int l;

    for ( it = celldatas.begin(); it != celldatas.end(); ++it) {
        //PRINT( it->first ); std::cout << "---------------" << std::endl;
        for( l=0;l < gnf.names.size(); ++l)
            if ( gnf.names[l] == it->first ) {
                //PRINT( it->first );
                SetElemFieldByName< float, DD, TM > setfield_float( it->first, it->second.numberOfComponents, &dd, &m );
                SetElemFieldByName< double, DD, TM > setfield_double( it->first, it->second.numberOfComponents, &dd, &m );
                unsigned taille_data = nb_elem * it->second.numberOfComponents;
                //PRINT( taille_data ); PRINT( it->second.type );
                if (it->second.type == "Float32") {
                    it->second.extract_content( setfield_float.data, taille_data );
                    //PRINT( m.elem_list.size() ); PRINT( setfield_float.data.size() ); PRINT( setfield_float.nb_comp ); PRINT( setfield_float.data );
                    apply( m.elem_list, setfield_float );

                } else {
                    it->second.extract_content( setfield_double.data, taille_data );
                    //PRINT( m.elem_list.size() ); PRINT( setfield_double.data.size() ); PRINT( setfield_double.nb_comp ); PRINT( setfield_double.data );
                    apply( m.elem_list, setfield_double );
                }
                break;
            }
        if (l == gnf.names.size()) { /// Ce champ n'est pas un champ d'élément
            /// si c'est un champ global, il sera assigné ici.
            //PRINT( it->first );
            SetFieldByName<float> setfield_float( it->first, it->second.numberOfComponents );
            SetFieldByName<double> setfield_double ( it->first, it->second.numberOfComponents );
            unsigned taille_data = it->second.numberOfComponents;
            //PRINT( taille_data ); PRINT( setfield.data.size() ); PRINT( setfield.nb_comp ); PRINT( setfield.data );
            if (it->second.type == "Float32") {
                it->second.extract_content( setfield_float.data, taille_data );
                //PRINT( m.elem_list.size() ); PRINT( setfield_float.data.size() ); PRINT( setfield_float.nb_comp ); PRINT( setfield_float.data );
                setfield_float.index_elt = 0;
                DM::apply_with_names( m, setfield_float );
            } else {
                it->second.extract_content( setfield_double.data, taille_data );
                //PRINT( m.elem_list.size() ); PRINT( setfield_double.data.size() ); PRINT( setfield_double.nb_comp ); PRINT( setfield_double.data );
                setfield_double.index_elt = 0;
                DM::apply_with_names( m, setfield_double );
            }
        }
    }
    
}

}

#endif /// LMT_READ_VTU_H



