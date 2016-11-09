#ifndef HDF_H
#define HDF_H

#pragma cpp_flag -DH5Acreate_vers=1
#pragma cpp_flag -DH5Gcreate_vers=1
#pragma cpp_flag -DH5Dcreate_vers=1
#pragma cpp_flag -DH5Dopen_vers=1
#pragma cpp_flag -DH5Gopen_vers=1

#pragma gpu_flag -DH5Acreate_vers=1
#pragma gpu_flag -DH5Gcreate_vers=1
#pragma gpu_flag -DH5Dcreate_vers=1
#pragma gpu_flag -DH5Dopen_vers=1
#pragma gpu_flag -DH5Gopen_vers=1

#pragma inc_path /usr/lib/openmpi/include
#pragma inc_path /usr/include/hdf5/serial

#include <hdf5.h>
#include <map>
#include <string.h>
#include <cstdlib>

#include "EnableIf.h"
#include "TensorOrder.h"

using namespace std;
using namespace LMT;

template<class T> struct H5_type {};

template<> struct H5_type<  signed char > { static hid_t res() { return H5T_NATIVE_SCHAR  ; } };
template<> struct H5_type<unsigned char > { static hid_t res() { return H5T_NATIVE_UCHAR  ; } };
template<> struct H5_type<  signed short> { static hid_t res() { return H5T_NATIVE_SHORT  ; } };
template<> struct H5_type<unsigned short> { static hid_t res() { return H5T_NATIVE_USHORT ; } };
template<> struct H5_type<  signed   int> { static hid_t res() { return H5T_NATIVE_INT    ; } };
template<> struct H5_type<unsigned   int> { static hid_t res() { return H5T_NATIVE_UINT   ; } };
template<> struct H5_type<  signed  long> { static hid_t res() { return H5T_NATIVE_LONG   ; } };
template<> struct H5_type<unsigned  long> { static hid_t res() { return H5T_NATIVE_ULONG  ; } };
template<> struct H5_type<     long long> { static hid_t res() { return H5T_NATIVE_LLONG  ; } };
template<> struct H5_type<         float> { static hid_t res() { return H5T_NATIVE_FLOAT  ; } };
template<> struct H5_type<        double> { static hid_t res() { return H5T_NATIVE_DOUBLE ; } };
template<> struct H5_type<   long double> { static hid_t res() { return H5T_NATIVE_LDOUBLE; } };


/**
  Hdf hdf( "my_file.h5" );
  vec.write_to( hdf, "/path/in/the/hdf/file" );

*/
class Hdf {
public:
    Hdf();
    Hdf( const std::string &filename, bool clear_old = false, bool read_only = false );
    ~Hdf();

    void open( const std::string &filename, bool clear_old = false, bool read_only = false );
    void close();
    Vec<std::string> list_dir( const std::string &dir ) const;

    // write tensorial data
    template<class T,class TV>
    void write_data( const std::string &name, const T *data, TV size, TV rese ) {
        check_grp( name );
        if ( H5Lexists( h5_file, name.c_str(), H5P_DEFAULT ) )
            H5Gunlink( h5_file, name.c_str() );

        int _dim = size.size();
        Vec<hsize_t,TV::static_size> _size;
        _size.resize( _dim );
        Vec<hsize_t,TV::static_size> _rese;
        _rese.resize( _dim );
        for( int d = 0; d < _dim; ++d ) {
            _size[ _dim - 1 - d ] = size[ d ];
            _rese[ _dim - 1 - d ] = rese[ d ];
        }

        hid_t dataspace = H5Screate_simple( _dim, _size.ptr(), _rese.ptr() );
        hid_t datatype  = H5Tcopy( H5_type<T>::res() );
        hid_t dataset   = H5Dcreate1( h5_file, name.c_str(), datatype, dataspace, H5P_DEFAULT );

        H5Dwrite( dataset, H5_type<T>::res(), H5S_ALL, H5S_ALL, H5P_DEFAULT, data );

        H5Sclose( dataspace );
        H5Tclose( datatype  );
        H5Dclose( dataset   );
    }

    // write tensorial data
    template<class T,class TV>
    void write_data( const std::string &name, T *data, TV size ) {
        write_data( name, data, size, size );
    }

    // write tag
    void write_tag( const std::string &name, const std::string &tag, const char *tag_value ) {
        hid_t dataset = H5Gopen1( h5_file, name.c_str() );
        hid_t aid     = H5Screate( H5S_SCALAR );
        hid_t atype   = H5Tcopy( H5T_C_S1 );

        // variables size
        H5Tset_size( atype, H5T_VARIABLE );
        hid_t attr = H5Acreate1( dataset, tag.c_str(), atype, aid, H5P_DEFAULT );

        H5Awrite( attr, atype, &tag_value );

        H5Sclose( aid     );
        H5Aclose( attr    );
        H5Gclose( dataset );
    }

    // write tag
    void write_tag( const std::string &name, const std::string &tag, const std::string &tag_value ) {
        write_tag( name, tag, tag_value.c_str() );
    }

    // write tag
    template<class TTV>
    void write_tag( const std::string &name, const std::string &tag, TTV tag_value ) {
        hid_t dataset = H5Gopen1( h5_file, name.c_str() );
        hid_t aid     = H5Screate( H5S_SCALAR );
        hid_t attr    = H5Acreate1( dataset, tag.c_str(), H5_type<TTV>::res(), aid, H5P_DEFAULT );

        H5Awrite( attr, H5_type<TTV>::res(), &tag_value );

        H5Sclose( aid     );
        H5Aclose( attr    );
        H5Gclose( dataset );
    }

//    // write tag
//    template<class TS, class TTV>
//    void write_tag( const std::string &name, TS &tag, TTV tag_value ) {
//        hid_t dataset = H5Gopen1( h5_file, name.c_str() );
//        hid_t aid     = H5Screate( H5S_SCALAR );
//        hid_t attr    = H5Acreate1( dataset, tag, H5_type<TTV>::res(), aid, H5P_DEFAULT );

//        H5Awrite( attr, H5_type<TTV>::res(), &tag_value );

//        H5Sclose( aid     );
//        H5Aclose( attr    );
//        H5Gclose( dataset );
//    }

    // read tag
    template<class TTV>
    void read_tag( const std::string &name, const std::string &tag, TTV &tag_value, bool group = true ) const {
        hid_t dataset = group ? H5Gopen1( h5_file, name.c_str() ) : H5Dopen1( h5_file, name.c_str() );
        hid_t attr    = H5Aopen_name( dataset, tag.c_str() );

        H5Aread( attr, H5_type<TTV>::res(), &tag_value );

        H5Aclose( attr    );
        if ( group )
            H5Gclose( dataset );
        else
            H5Dclose( dataset );
    }

//    // read tag
//    template<class TS, class TTV>
//    void read_tag( const std::string &name, TS &tag, TTV &tag_value, bool group = true ) {
//        hid_t dataset = group ? H5Gopen1( h5_file, name.c_str() ) : H5Dopen1( h5_file, name.c_str() );
//        hid_t attr    = H5Aopen_name( dataset, tag );

//        H5Aread( attr, H5_type<TTV>::res(), &tag_value );

//        H5Aclose( attr    );
//        if ( group )
//            H5Gclose( dataset );
//        else
//            H5Dclose( dataset );
//    }

    void read_tag( const std::string &name, const std::string &tag, std::string &tag_value, bool group = true ) const;
    void read_group_size( const std::string &name, int &size ) const;
    void read_size( const std::string &name, int &size ) const;

    // read size
    template<class TV>
    void read_size( const std::string &name, TV &size_ ) const {
        hid_t dataset = H5Dopen1( h5_file, name.c_str() );
        hid_t dataspace = H5Dget_space( dataset );
        //
        Vec<hsize_t> tmp;
        tmp.resize( H5Sget_simple_extent_ndims( dataspace ) );
        H5Sget_simple_extent_dims( dataspace, tmp.ptr(), NULL );
        size_.resize( tmp.size() );
        for( int d = 0; d < tmp.size(); ++d )
            size_[ tmp.size() - 1 - d ] = tmp[ d ];
        //
        H5Dclose( dataset   );
        H5Sclose( dataspace );
    }

    // read tensorial data
    template<class T,class TV>
    void read_data( const std::string &name, T *data, const TV &size, const TV &rese ) const {
        // filespace
        hid_t dataset = H5Dopen1( h5_file, name.c_str() );
        hid_t filespace = H5Dget_space( dataset );

        // memspace
        int _dim = size.size();
        Vec<hsize_t,TV::static_size> _size;
        _size.resize( _dim );
        Vec<hsize_t,TV::static_size> _rese;
        _rese.resize( _dim );
        for( int d = 0; d < _dim; ++d ) {
            _size[ _dim - 1 - d ] = size[ d ];
            _rese[ _dim - 1 - d ] = rese[ d ];
        }
        hid_t memspace = H5Screate_simple( _dim, _size.ptr(), _rese.ptr() );

        // read
        H5Dread( dataset, H5_type<T>::res(), memspace, filespace, H5P_DEFAULT, data );

        // close
        H5Sclose( memspace );
        H5Sclose( filespace );
        H5Dclose( dataset );
    }

    // read tensorial data
    template<class T,class TV>
    void read_data( const std::string &name, T *data, TV size ) {
        read_data( name, data, size, size );
    }

    // read
    template<class TV>
    typename EnableIf<(TensorOrder<TV>::res==1)>::T read( const std::string &name, TV &data ) const {
        Vec<int,1> size;
        read_size( name, size );
        data.resize( size[0] );
        read_data( name, data.ptr(), size, size );
    }

    // read
    template<class TV>
    typename EnableIf<(TensorOrder<TV>::res!=1)>::T read( const std::string &name, TV &data ) const {
        Vec<int> size;
        read_size( name, size );
        data.resize( size );
        read_data( name, data.ptr(), size, size );
    }

    // check if dataset exists
    bool dataset_exist( std::string &name ){
        bool exist = false;
        if ( H5Lexists( h5_file , name.c_str(), H5P_DEFAULT ) ){
           exist = true;
        }
        return exist;
    }
    
    
private:
    void check_grp( const std::string &name ) {
        // find grp
        int off = name.rfind( '/' );
        if ( off <= 0 )
            return;
        const std::string &grp = name.substr( 0, off );
        check_grp( grp );
        //
        if ( not H5Lexists( h5_file, grp.c_str(), H5P_DEFAULT ) )
            H5Gclose( H5Gcreate1( h5_file, grp.c_str(), 0 ) );
    }

    hid_t h5_file;
    
};

#endif // HDF_H
