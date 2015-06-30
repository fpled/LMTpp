#include "mesh.h"
#include "meshcaracstd.h"
#include "make_rect.h"
#include "displayparaview.h"
#include "../util/combinatorialiterator.h"

#include "MonMeshCarac.h"

using namespace LMT;
using namespace std;

bool semblable( Vec<int> &c, Vec<int> &c2 ) {

    if ( ( c.size() == 0 ) and ( c2.size() == 0 ) )
        return true;

    if ( c.size() != c2.size() )
        return false;
    
    unsigned i;
    for( i = 0; i < c.size(); i += 2 ) {
        unsigned j;
        for( j = 0; j < c2.size(); j += 2 )
            if ( ( c[i] == c2[j] ) and ( c[i+1] == c2[j+1] ) )
                break;
        if ( j == c2.size() )
            return false;
    }        
    
    if ( i == c.size() )
        return true;
    else
        return false;
}

/// version dont les permutations vont de la référence vers le tétra courant. 
void genere_config() {
    Piterator<4> pi;
    Vec< Vec<int>, 11 > config_list;
    unsigned c = 0;
    //ofstream sortie( "config_id.h" );
    //sortie << "unsigned config_id[ 64 ] = { ";
    ofstream sortie2( "divide_element_Tetra.h" );
    
    config_list[1].push_back( 2 ); 
    config_list[1].push_back( 3 );
    
    config_list[2].push_back( 0 );
    config_list[2].push_back( 3 );
    config_list[2].push_back( 2 );
    config_list[2].push_back( 3 );
    
    config_list[3].push_back( 0 );
    config_list[3].push_back( 1 );
    config_list[3].push_back( 2 );
    config_list[3].push_back( 3 );
        
    config_list[4].push_back( 0 );
    config_list[4].push_back( 3 );
    config_list[4].push_back( 1 );
    config_list[4].push_back( 3 );
    config_list[4].push_back( 2 );
    config_list[4].push_back( 3 );
        
    config_list[5].push_back( 0 );
    config_list[5].push_back( 3 );
    config_list[5].push_back( 0 );
    config_list[5].push_back( 2 );
    config_list[5].push_back( 2 );
    config_list[5].push_back( 3 ); 

    config_list[6].push_back( 0 );
    config_list[6].push_back( 3 );
    config_list[6].push_back( 1 );
    config_list[6].push_back( 2 );
    config_list[6].push_back( 2 );
    config_list[6].push_back( 3 );
    
    config_list[7].push_back( 0 );
    config_list[7].push_back( 3 );
    config_list[7].push_back( 0 );
    config_list[7].push_back( 1 );
    config_list[7].push_back( 1 );
    config_list[7].push_back( 3 );
    config_list[7].push_back( 2 );
    config_list[7].push_back( 3 );
    
    config_list[8].push_back( 0 );
    config_list[8].push_back( 3 );
    config_list[8].push_back( 0 );
    config_list[8].push_back( 1 );
    config_list[8].push_back( 1 );
    config_list[8].push_back( 2 );
    config_list[8].push_back( 2 );
    config_list[8].push_back( 3 );    
    
    config_list[9].push_back( 0 );
    config_list[9].push_back( 3 );
    config_list[9].push_back( 0 );
    config_list[9].push_back( 1 );
    config_list[9].push_back( 1 );
    config_list[9].push_back( 3 );
    config_list[9].push_back( 1 );
    config_list[9].push_back( 2 ); 
    config_list[9].push_back( 2 );
    config_list[9].push_back( 3 ); 
    
    config_list[10].push_back( 0 );
    config_list[10].push_back( 3 );
    config_list[10].push_back( 1 );
    config_list[10].push_back( 3 );
    config_list[10].push_back( 2 );
    config_list[10].push_back( 3 );
    config_list[10].push_back( 0 );
    config_list[10].push_back( 1 ); 
    config_list[10].push_back( 1 );
    config_list[10].push_back( 2 );     
    config_list[10].push_back( 0 );
    config_list[10].push_back( 2 ); 
    
    sortie2 << "template<class TN,class TNG,class TD,unsigned NET,class TM>" << endl;
    sortie2 << "bool divide_element( Element< Tetra, TN, TNG, TD, NET> &e, TM &m, TNG **nodes ) {" << endl;
    sortie2 << "    typedef typename TNG::T T;" << endl;
    sortie2 << "    unsigned index = bool(nodes[0]) + bool(nodes[1])*2 + bool(nodes[2])*4 + bool(nodes[3])*8 + bool(nodes[4])*16 + bool(nodes[5])*32;" << endl << endl;
    sortie2 << "    TNG *nn[4];" << endl << endl;
    sortie2 << "    switch( index ) {" << endl;
    
    for( unsigned i1 = 0; i1 < 2; ++i1 )
    for( unsigned i2 = 0; i2 < 2; ++i2 )
    for( unsigned i3 = 0; i3 < 2; ++i3 )
    for( unsigned i4 = 0; i4 < 2; ++i4 )
    for( unsigned i5 = 0; i5 < 2; ++i5 )
    for( unsigned i6 = 0; i6 < 2; ++i6 ) {
        Vec<int> config, bar_list;
        unsigned nb_cut = i1 + i2 + i3 + i4 + i5 +i6;
        unsigned a_cut, b_cut, config_id = 1000000;
        Vec<unsigned,4> permutation;
        
        switch( nb_cut ) {
            case 0 : {
                a_cut = 0;
                b_cut = 1;
            } break;
            case 1 : {
                a_cut = 1;
                b_cut = 2;
            } break;
            case 2 : { 
                a_cut = 2;
                b_cut = 4;
            } break;                
            case 3 : {
                a_cut = 4;
                b_cut = 7;
            } break;
            case 4 : {
                a_cut = 7;
                b_cut = 9;
            } break;
            case 5 : {
                a_cut = 9;
                b_cut = 10;
            } break;
            case 6 : {
                a_cut = 10;
                b_cut = 11;
            } break;
        }
        
        if ( i6 ) {
            config.push_back( 0 ); config.push_back( 1 ); bar_list.push_back( 0 );
        }
        if ( i5 ) {
            config.push_back( 0 ); config.push_back( 2 ); bar_list.push_back( 1 );
        }
        if ( i4 ) { 
            config.push_back( 0 ); config.push_back( 3 ); bar_list.push_back( 2 );
        }
        if ( i3 ) {
            config.push_back( 1 ); config.push_back( 2 ); bar_list.push_back( 3 );
        }
        if ( i2 ) {
            config.push_back( 1 ); config.push_back( 3 ); bar_list.push_back( 4 );
        }
        if ( i1 ) {
            config.push_back( 2 ); config.push_back( 3 ); bar_list.push_back( 5 );
        }
        
        /// détermination de la configuration number et de la permutation associées
        bool found = false;
        for( unsigned i = a_cut; ( i < b_cut ) and not( found ); ++i ) {
            pi.init();
            while( true ) {
                //PRINT( pi.pos );
                Vec<int> new_config = config_list[ i ];
                
                for( unsigned j = 0; j < new_config.size(); ++j )
                    new_config[ j ] = pi.pos[ config_list[ i ][ j ] ]; /// on déplace le tétra de référence pour retrouver la configuration number courante
                
                for( unsigned j = 0; j < new_config.size(); j += 2 )
                    if ( new_config[ j ] > new_config[ j + 1 ] )
                        swap( new_config[ j ], new_config[ j + 1 ] ); 
                
                if ( semblable( new_config, config ) ) {
                    config_id = i;
                    permutation = pi.pos;
                    found = true;
                    break;
                }
                   
                if ( not( pi ) )
                    break;
                ++pi;
            }            
        }
        
        cout << c << " | config_id = " << config_id << " | " << config << " | ";
        if ( config_id < 11 )
            cout << "permutation = " << permutation;
        cout << " (bar) " << bar_list << endl;
        
        sortie2 << "        case " << c << " : { /// configuration number " << config_id << endl; 
        switch( config_id ) {
            case 0 : {
                sortie2 << "            return false;" << endl;
            } break;
            case 1 : {
                sortie2 << "            nn[0] = e.node( " << pi.pos[ 0 ] << " );" << endl;
                sortie2 << "            nn[1] = e.node( " << pi.pos[ 1 ] << " );" << endl;
                sortie2 << "            nn[2] = e.node( " << pi.pos[ 2 ] << " );" << endl;
                sortie2 << "            nn[3] = nodes[ " << bar_list[ 0 ] << " ];" << endl;
                sortie2 << "            permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;
                sortie2 << "            nn[0] = e.node( " << pi.pos[ 0 ] << " );" << endl;
                sortie2 << "            nn[1] = e.node( " << pi.pos[ 1 ] << " );" << endl;
                sortie2 << "            nn[2] = e.node( " << pi.pos[ 3 ] << " );" << endl;
                sortie2 << "            nn[3] = nodes[ " << bar_list[ 0 ] << " ];" << endl;
                sortie2 << "            permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;
            } break;
            case 2 : {
                sortie2 << "            bool AF;" << endl;
                sortie2 << "            T l_AF = length( e.pos( " << pi.pos[ 0 ] << " ) - nodes[ " << bar_list[ 1 ] << " ]->pos );" << endl;
                sortie2 << "            T l_CE = length( e.pos( " << pi.pos[ 2 ] << " ) - nodes[ " << bar_list[ 0 ] << " ]->pos );" << endl;
                sortie2 << "            if ( l_AF < l_CE ) {" << endl;
                sortie2 << "                AF = true;" << endl;
                sortie2 << "            } else {" << endl;
                sortie2 << "                if ( l_AF > l_CE )" << endl; 
                sortie2 << "                    AF = false;" << endl; 
                sortie2 << "                else {" << endl;
                sortie2 << "                    if ( e.node( " << pi.pos[ 0 ] << " )->number < e.node( " << pi.pos[ 2 ] << " )->number ) AF = true; else AF = false; " << endl;
                sortie2 << "                }" << endl;
                sortie2 << "            }" << endl;
                sortie2 << "            if ( AF ) {" << endl;
                sortie2 << "                nn[0] = e.node( " << pi.pos[ 1 ] << " );" << endl;
                sortie2 << "                nn[1] = e.node( " << pi.pos[ 3 ] << " );" << endl;
                sortie2 << "                nn[2] = nodes[ " << bar_list[ 0 ] << " ];" << endl;
                sortie2 << "                nn[3] = nodes[ " << bar_list[ 1 ] << " ];" << endl;
                sortie2 << "                permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;
                sortie2 << "                nn[0] = e.node( " << pi.pos[ 0 ] << " );" << endl;
                sortie2 << "                nn[1] = e.node( " << pi.pos[ 1 ] << " );" << endl;
                sortie2 << "                nn[2] = nodes[ " << bar_list[ 0 ] << " ];" << endl;
                sortie2 << "                nn[3] = nodes[ " << bar_list[ 1 ] << " ];" << endl;
                sortie2 << "                permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;
                sortie2 << "                nn[0] = e.node( " << pi.pos[ 0 ] << " );" << endl;
                sortie2 << "                nn[1] = e.node( " << pi.pos[ 1 ] << " );" << endl;
                sortie2 << "                nn[2] = e.node( " << pi.pos[ 2 ] << " );" << endl;
                sortie2 << "                nn[3] = nodes[ " << bar_list[ 1 ] << " ];" << endl;
                sortie2 << "                permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;
                sortie2 << "            } else {" << endl;
                sortie2 << "                nn[0] = e.node( " << pi.pos[ 1 ] << " );" << endl;
                sortie2 << "                nn[1] = e.node( " << pi.pos[ 3 ] << " );" << endl;
                sortie2 << "                nn[2] = nodes[ " << bar_list[ 0 ] << " ];" << endl;
                sortie2 << "                nn[3] = nodes[ " << bar_list[ 1 ] << " ];" << endl;
                sortie2 << "                permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;
                sortie2 << "                nn[0] = e.node( " << pi.pos[ 1 ] << " );" << endl;
                sortie2 << "                nn[1] = e.node( " << pi.pos[ 2 ] << " );" << endl;
                sortie2 << "                nn[2] = nodes[ " << bar_list[ 0 ] << " ];" << endl;
                sortie2 << "                nn[3] = nodes[ " << bar_list[ 1 ] << " ];" << endl;
                sortie2 << "                permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;
                sortie2 << "                nn[0] = e.node( " << pi.pos[ 0 ] << " );" << endl;
                sortie2 << "                nn[1] = e.node( " << pi.pos[ 1 ] << " );" << endl;
                sortie2 << "                nn[2] = e.node( " << pi.pos[ 2 ] << " );" << endl;
                sortie2 << "                nn[3] = nodes[ " << bar_list[ 0 ] << " ];" << endl;
                sortie2 << "                permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;
                sortie2 << "            }" << endl;
            } break;
            case 3 : {
                sortie2 << "            nn[0] = e.node( " << pi.pos[ 0 ] << " );" << endl;
                sortie2 << "            nn[1] = nodes[ " << bar_list[ 0 ] << " ];" << endl;
                sortie2 << "            nn[2] = nodes[ " << bar_list[ 1 ] << " ];" << endl;
                sortie2 << "            nn[3] = e.node( " << pi.pos[ 3 ] << " );" << endl;
                sortie2 << "            permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;
                sortie2 << "            nn[0] = e.node( " << pi.pos[ 0 ] << " );" << endl;
                sortie2 << "            nn[1] = nodes[ " << bar_list[ 0 ] << " ];" << endl;
                sortie2 << "            nn[2] = nodes[ " << bar_list[ 1 ] << " ];" << endl;
                sortie2 << "            nn[3] = e.node( " << pi.pos[ 2 ] << " );" << endl;
                sortie2 << "            permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;                
                sortie2 << "            nn[0] = e.node( " << pi.pos[ 1 ] << " );" << endl;
                sortie2 << "            nn[1] = nodes[ " << bar_list[ 0 ] << " ];" << endl;
                sortie2 << "            nn[2] = nodes[ " << bar_list[ 1 ] << " ];" << endl;
                sortie2 << "            nn[3] = e.node( " << pi.pos[ 3 ] << " );" << endl;
                sortie2 << "            permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;                
                sortie2 << "            nn[0] = e.node( " << pi.pos[ 1 ] << " );" << endl;
                sortie2 << "            nn[1] = nodes[ " << bar_list[ 0 ] << " ];" << endl;
                sortie2 << "            nn[2] = nodes[ " << bar_list[ 1 ] << " ];" << endl;
                sortie2 << "            nn[3] = e.node( " << pi.pos[ 2 ] << " );" << endl;
                sortie2 << "            permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;                
            } break;
            case 4 : {
                sortie2 << "            std::cerr << \"more complicated configuration number " << c << " to do\" << std::endl;" << endl;
                sortie2 << "            assert( 0 );" << endl;
            } break;
            case 5 : {
                sortie2 << "            nn[0] = e.node( " << pi.pos[ 0 ] << " );" << endl;
                sortie2 << "            nn[1] = e.node( " << pi.pos[ 1 ] << " );" << endl;
                sortie2 << "            nn[2] = nodes[ " << bar_list[ 0 ] << " ];" << endl;
                sortie2 << "            nn[3] = nodes[ " << bar_list[ 1 ] << " ];" << endl;
                sortie2 << "            permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;
                sortie2 << "            nn[0] = e.node( " << pi.pos[ 1 ] << " );" << endl;
                sortie2 << "            nn[1] = e.node( " << pi.pos[ 2 ] << " );" << endl;
                sortie2 << "            nn[2] = nodes[ " << bar_list[ 0 ] << " ];" << endl;
                sortie2 << "            nn[3] = nodes[ " << bar_list[ 2 ] << " ];" << endl;
                sortie2 << "            permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;
                sortie2 << "            nn[0] = e.node( " << pi.pos[ 1 ] << " );" << endl;
                sortie2 << "            nn[1] = nodes[ " << bar_list[ 0 ] << " ];" << endl;
                sortie2 << "            nn[2] = nodes[ " << bar_list[ 1 ] << " ];" << endl;
                sortie2 << "            nn[3] = nodes[ " << bar_list[ 2 ] << " ];" << endl;
                sortie2 << "            permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;
                sortie2 << "            nn[0] = e.node( " << pi.pos[ 1 ] << " );" << endl;
                sortie2 << "            nn[1] = e.node( " << pi.pos[ 3 ] << " );" << endl;
                sortie2 << "            nn[2] = nodes[ " << bar_list[ 1 ] << " ];" << endl;
                sortie2 << "            nn[3] = nodes[ " << bar_list[ 2 ] << " ];" << endl;
                sortie2 << "            permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;
            } break;
            case 6 : {
                sortie2 << "            std::cerr << \"more complicated configuration number " << c << " to do\" << std::endl;" << endl;
                sortie2 << "            assert( 0 );" << endl;
            } break;
            case 7 : {
                sortie2 << "            std::cerr << \"more complicated configuration number " << c << " to do\" << std::endl;" << endl;
                sortie2 << "            assert( 0 );" << endl;
            } break;
            case 8 : {
                sortie2 << "            std::cerr << \"more complicated configuration number " << c << " to do\" << std::endl;" << endl;
                sortie2 << "            assert( 0 );" << endl;
            } break;
            case 9 : {
                sortie2 << "            std::cerr << \"more complicated configuration number " << c << " to do\" << std::endl;" << endl;
                sortie2 << "            assert( 0 );" << endl;
            } break;
            case 10 : {
                sortie2 << "            nn[0] = e.node( 0 );" << endl;
                sortie2 << "            nn[1] = nodes[ 0 ];" << endl;
                sortie2 << "            nn[2] = nodes[ 1 ];" << endl;
                sortie2 << "            nn[3] = nodes[ 2 ];" << endl;
                sortie2 << "            permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;
                sortie2 << "            nn[0] = e.node( 1 );" << endl;
                sortie2 << "            nn[1] = nodes[ 0 ];" << endl;
                sortie2 << "            nn[2] = nodes[ 3 ];" << endl;
                sortie2 << "            nn[3] = nodes[ 4 ];" << endl;
                sortie2 << "            permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;
                sortie2 << "            nn[0] = e.node( 2 );" << endl;
                sortie2 << "            nn[1] = nodes[ 1 ];" << endl;
                sortie2 << "            nn[2] = nodes[ 3 ];" << endl;
                sortie2 << "            nn[3] = nodes[ 5 ];" << endl;
                sortie2 << "            permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;
                sortie2 << "            nn[0] = e.node( 3 );" << endl;
                sortie2 << "            nn[1] = nodes[ 2 ];" << endl;
                sortie2 << "            nn[2] = nodes[ 4 ];" << endl;
                sortie2 << "            nn[3] = nodes[ 5 ];" << endl;
                sortie2 << "            permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "            DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;
                sortie2 << "            /// octaèdre interne" << endl;
                sortie2 << "            int smaller;" << endl;
                sortie2 << "            T l_EJ = length( nodes[ 0 ]->pos - nodes[ 5 ]->pos );" << endl;
                sortie2 << "            T l_GH = length( nodes[ 2 ]->pos - nodes[ 3 ]->pos );" << endl;
                sortie2 << "            T l_FI = length( nodes[ 1 ]->pos - nodes[ 4 ]->pos );" << endl;
                sortie2 << "            if ( l_EJ < l_GH ) {" << endl;
                sortie2 << "                if ( l_FI < l_EJ ) smaller = 2; else smaller = 0;" << endl;
                sortie2 << "            } else {" << endl;
                sortie2 << "                if ( l_FI < l_GH ) smaller = 2; else smaller = 1;" << endl;                
                sortie2 << "            }" << endl;
                sortie2 << "            if ( smaller == 0 ) {" << endl;
                sortie2 << "                nn[0] = nodes[ 0 ];" << endl;
                sortie2 << "                nn[1] = nodes[ 5 ];" << endl;
                sortie2 << "                nn[2] = nodes[ 2 ];" << endl;
                sortie2 << "                nn[3] = nodes[ 4 ];" << endl;
                sortie2 << "                permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;
                sortie2 << "                nn[0] = nodes[ 0 ];" << endl;
                sortie2 << "                nn[1] = nodes[ 5 ];" << endl;
                sortie2 << "                nn[2] = nodes[ 3 ];" << endl;
                sortie2 << "                nn[3] = nodes[ 4 ];" << endl;
                sortie2 << "                permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;                               
                sortie2 << "                nn[0] = nodes[ 0 ];" << endl;
                sortie2 << "                nn[1] = nodes[ 5 ];" << endl;
                sortie2 << "                nn[2] = nodes[ 2 ];" << endl;
                sortie2 << "                nn[3] = nodes[ 1 ];" << endl;
                sortie2 << "                permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;
                sortie2 << "                nn[0] = nodes[ 0 ];" << endl;
                sortie2 << "                nn[1] = nodes[ 5 ];" << endl;
                sortie2 << "                nn[2] = nodes[ 3 ];" << endl;
                sortie2 << "                nn[3] = nodes[ 1 ];" << endl;
                sortie2 << "                permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "                DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;                               
                sortie2 << "            } else {" << endl;
                sortie2 << "                if ( smaller == 1 ) {" << endl;
                sortie2 << "                    nn[0] = nodes[ 2 ];" << endl;
                sortie2 << "                    nn[1] = nodes[ 3 ];" << endl;
                sortie2 << "                    nn[2] = nodes[ 4 ];" << endl;
                sortie2 << "                    nn[3] = nodes[ 5 ];" << endl;
                sortie2 << "                    permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "                    DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;
                sortie2 << "                    nn[0] = nodes[ 2 ];" << endl;
                sortie2 << "                    nn[1] = nodes[ 3 ];" << endl;
                sortie2 << "                    nn[2] = nodes[ 0 ];" << endl;
                sortie2 << "                    nn[3] = nodes[ 4 ];" << endl;
                sortie2 << "                    permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "                    DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;
                sortie2 << "                    nn[0] = nodes[ 2 ];" << endl;
                sortie2 << "                    nn[1] = nodes[ 3 ];" << endl;
                sortie2 << "                    nn[2] = nodes[ 1 ];" << endl;
                sortie2 << "                    nn[3] = nodes[ 5 ];" << endl;
                sortie2 << "                    permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "                    DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;
                sortie2 << "                    nn[0] = nodes[ 2 ];" << endl;
                sortie2 << "                    nn[1] = nodes[ 3 ];" << endl;
                sortie2 << "                    nn[2] = nodes[ 0 ];" << endl;
                sortie2 << "                    nn[3] = nodes[ 1 ];" << endl;
                sortie2 << "                    permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "                    DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;
                sortie2 << "                } else {" << endl;
                sortie2 << "                    nn[0] = nodes[ 1 ];" << endl;
                sortie2 << "                    nn[1] = nodes[ 4 ];" << endl;
                sortie2 << "                    nn[2] = nodes[ 0 ];" << endl;
                sortie2 << "                    nn[3] = nodes[ 3 ];" << endl;
                sortie2 << "                    permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "                    DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;
                sortie2 << "                    nn[0] = nodes[ 1 ];" << endl;
                sortie2 << "                    nn[1] = nodes[ 4 ];" << endl;
                sortie2 << "                    nn[2] = nodes[ 3 ];" << endl;
                sortie2 << "                    nn[3] = nodes[ 5 ];" << endl;
                sortie2 << "                    permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "                    DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;
                sortie2 << "                    nn[0] = nodes[ 1 ];" << endl;
                sortie2 << "                    nn[1] = nodes[ 4 ];" << endl;
                sortie2 << "                    nn[2] = nodes[ 5 ];" << endl;
                sortie2 << "                    nn[3] = nodes[ 2 ];" << endl;
                sortie2 << "                    permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "                    DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;
                sortie2 << "                    nn[0] = nodes[ 1 ];" << endl;
                sortie2 << "                    nn[1] = nodes[ 4 ];" << endl;
                sortie2 << "                    nn[2] = nodes[ 2 ];" << endl;
                sortie2 << "                    nn[3] = nodes[ 0 ];" << endl;
                sortie2 << "                    permutation_if_jac_neg ( Tetra(), nn );" << endl;
                sortie2 << "                    DM::copy( e, *m.add_element( Tetra(), TN(), nn ) );" << endl;
                sortie2 << "                }" << endl;
                sortie2 << "            }" << endl;
            } break;
            default : ;
        }
        sortie2 << "        } break;" << endl;
        
        //sortie << config_id;
        //if ( c < 63 )
        //    sortie << ", ";
        //else
        //    sortie << " };" << endl;
            
        c++;
    }

    sortie2 << "        default : ;" << endl;
    sortie2 << "    }" << endl << endl;
    sortie2 << "    return true;" << endl;
    sortie2 << "}" << endl;
    sortie2.close();
    //sortie.close();
}



int main( int argc, const char* argv[] ) {

    genere_config();
    return 0;
}



