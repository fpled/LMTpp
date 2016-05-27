#include "Hdf.h"

#ifdef METIL_COMP_DIRECTIVE
#pragma lib_path /usr/lib/x86_64-linux-gnu/hdf5/serial
#pragma lib_name hdf5
#endif
// #pragma lib_name mpi
// #pragma lib_name mpi++

using namespace std;
using namespace LMT;

Hdf::Hdf() : h5_file( 0 ) {
}

Hdf::Hdf( const std::string &filename, bool clear_old, bool read_only ) : h5_file( 0 ) {
    open( filename, clear_old );
}

Hdf::~Hdf() {
    if ( h5_file )
        H5Fclose( h5_file );
}

void Hdf::open( const std::string &filename, bool clear_old, bool read_only ) {
    if ( h5_file )
        H5Fclose( h5_file );
    // create or read a previously created file
    if ( clear_old )
        h5_file = H5Fcreate( filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    else
        h5_file = H5Fopen( filename.c_str(), read_only ? H5F_ACC_RDONLY : ( H5F_ACC_RDWR | H5F_ACC_CREAT ), H5P_DEFAULT );
}

static herr_t file_info( hid_t, const char *name, void *op_data ) {
    *reinterpret_cast<Vec<std::string> *>( op_data ) << std::string( name );
    return 0;
}

Vec<std::string> Hdf::list_dir( const std::string &dir ) const {
    Vec<std::string> res;
    H5Giterate( h5_file, dir.c_str(), NULL, file_info, &res );
    return res;
}

void Hdf::read_tag( const std::string &name, const std::string &tag, std::string &tag_value, bool group ) const {
    hid_t dataset = group ? H5Gopen1( h5_file, name.c_str() ) : H5Dopen1( h5_file, name.c_str() );
    hid_t attr    = H5Aopen_name( dataset, tag.c_str() );
    hid_t ftype   = H5Aget_type( attr );
    hid_t atype   = H5Tget_native_type( ftype, H5T_DIR_ASCEND );

    if ( H5Tget_strpad( ftype ) == H5T_STR_NULLTERM ) {
        char *string_attr;
        H5Aread( attr, atype, &string_attr );
        if ( string_attr )
            tag_value = string_attr;
        else
            tag_value = std::string();
        ::free( string_attr );
    } else {
        size_t size   = H5Tget_size( ftype );

        char *string_attr = (char *)std::malloc( size );
        H5Aread( attr, atype, string_attr );
        tag_value = string_attr;
        std::free( string_attr );
    }

    H5Aclose( attr    );
    H5Tclose( atype   );
    if ( group )
        H5Gclose( dataset );
    else
        H5Dclose( dataset );
}

void Hdf::read_group_size( const std::string &name, int &size ) const {
    hid_t dataset = H5Gopen1( h5_file, name.c_str() );

    H5G_info_t group_info;
    H5Gget_info( dataset, &group_info );
    size = group_info.nlinks;

    H5Gclose(dataset);
}

void Hdf::read_size( const std::string &name, int &size ) const {
    Vec<int> size_vec;
    read_size( name, size_vec );
    size = size_vec[ 0 ];
}
