#ifndef LMT_extrude_HEADER
#define LMT_extrude_HEADER

#include <map>
#include "make_rect.h"
#include "triangle.h"
#include "quad.h"

namespace LMT {

struct DefaultBehavior;
struct Bar;
struct Bar_3;
struct Triangle;
struct Triangle_6;
struct Quad;
struct Quad_8;
struct Quad_9;
struct Quad_6;
struct Quad_42;
struct Tetra;
struct Tetra_10;
struct Hexa;
struct Hexa_20;
struct Wedge;
struct Wedge_15;

template< class Pvec_, class TNode_ >
struct GetNormal {
    typedef Pvec_ Pvec;
    typedef std::map< TNode_*, Pvec_ > MapPvec;
    
    template< class TE >
    void operator() ( const TE & e) {
        Pvec nor = sample_normal( e );
        typename MapPvec::iterator it;
        //PRINT( nor );
        for( unsigned i = 0; i < TE::nb_nodes; ++i ) {
            it = map_normal.find( e.nodes[ i ] );
            if ( it != map_normal.end() )
                it->second += nor;
            else
                map_normal[ e.nodes[ i ] ] = nor;
        } 
    }
    
    void normalize() {
        typename MapPvec::iterator it;
        for( it = map_normal.begin(); it != map_normal.end(); it++ )
            if ( length( it->second ) >  0 )
                it->second /= length( it->second );
    }
    
    void display_normal() const {
        typename MapPvec::const_iterator it;
        for( it = map_normal.begin(); it != map_normal.end(); it++ )
            PRINT( it->second );    
    }

    MapPvec map_normal;
};

template< class TM, class MV3, class T, bool sameDirection >
struct Extruder {
    typedef typename TM::Pvec Pvec;
    typedef Vec< typename TM::TNode * > VN;
    
    Extruder( TM &m_, MV3& map_normal_, T thickness_, unsigned nb_layer_, T prec_ = 1e-5 ) : m( m_), map_normal( map_normal_ ), nb_layer( nb_layer_ ) { 
        ba.prec = prec_;
        ba.m = &m_;
        thickness_by_layer = thickness_ / nb_layer_;
    }
    
    template < class TE >
    void init_vnode( const TE &e, VN &vn ) {
        vn.resize( ( nb_layer + 1 ) * TE::nb_nodes );
        Vec< Pvec > vp;
        vp.resize( TE::nb_nodes );
        vp.set( 0 );/// si le maillage est défini dans le plan, on s'assure que la composante en z vaut zéro.
        for( unsigned i = 0; i < TE::nb_nodes; ++i ) {
            vp[ i ] = e.pos( i );
            vn[ i ] = ba.get_node( vp[ i ] );
        }
        for( unsigned z = 1; z <= nb_layer; ++z )
            for( unsigned i = 0; i < TE::nb_nodes; ++i )
                if ( sameDirection )
                    vn[ z * TE::nb_nodes + i ] = ba.get_node( vp[ i ] + z * thickness_by_layer * map_normal[ NULL ] ); 
                else
                    vn[ z * TE::nb_nodes + i ] = ba.get_node( vp[ i ] + z * thickness_by_layer * map_normal[ e.nodes[ i ] ] ); 
    }
    
    /// Triangle
    template< class NameBehavior, class TNode_, class TData, unsigned num_in_elem_list_ >
    void operator() ( const Element< Triangle, NameBehavior, TNode_, TData, num_in_elem_list_ > &e ) {
    
        typedef Element< Triangle, NameBehavior, TNode_, TData, num_in_elem_list_ > TE;
        VN vn;
        
        init_vnode( e, vn );
        for( unsigned z = 0; z < nb_layer; ++z ) {
            permutation_if_jac_neg( Wedge(), vn.ptr() + z * TE::nb_nodes );
            m.add_element( Wedge(), DefaultBehavior(), vn.ptr() + z * TE::nb_nodes );
        }
    }
    
    /// Quad
    template< class NameBehavior, class TNode_, class TData, unsigned num_in_elem_list_ >
    void operator() ( const Element< Quad, NameBehavior, TNode_, TData, num_in_elem_list_ > &e ) {
    
        typedef Element< Quad, NameBehavior, TNode_, TData, num_in_elem_list_ > TE;
        VN vn;
        
        init_vnode( e, vn );
        for( unsigned z = 0; z < nb_layer; ++z ) {
            permutation_if_jac_neg( Hexa(), vn.ptr() + z * TE::nb_nodes );
            m.add_element( Hexa(), DefaultBehavior(), vn.ptr() + z * TE::nb_nodes );
        }
    }
    
    template< class TE >
    void operator() ( const TE &e ) {
        std::cerr << "Error struct Extruder : " <<  e.name() << " is not extruded. TODO. " << std::endl;
    }
    
    TM &m;
    MV3 &map_normal;
    T thickness_by_layer;
    unsigned nb_layer;
    BestialNodeAdder<TM> ba;
};

/*!
    Objectif :
        Extruder un maillage surfacique défini dans le plan ou dans l'espace ou bien le maillage peau d'un maillage 3D.
         
    Paramètres :
        * res est le maillage résultat
        * m est le maillage que l'on veut extruder.
        * thickness est l'épaisseur de l'extrusion
        * nb_layer est le nombre d'éléments qui se superposeront dans le sens de l'extrusion
          
    Exemple de code :
    \code C/C++
        #include "mesh/meshcaracstd.h"
        #include "mesh/mesh.h"
        #include "mesh/read_msh.h"
        
        #include "mesh/displayparaview.h"
        #include "mesh/extrude.h"
        
        using namespace LMT;
        using namespace std;

        int main(int argc,char **argv) {
            typedef double T;
            typedef Mesh< Mesh_carac_test_extrude<T,2> > TM_2D; /// maillage 2D
            typedef Mesh< Mesh_carac_test_extrude<T,3> > TM_3D; /// maillage 3D
            typedef Mesh< MeshCaracStd< 3, 2, 1, T  > > TM_s; /// avec des Quad dans l'espace
            typedef Mesh< MeshCaracStd< 3, 2, 0  > > TM_s_2; /// avec des Triangle dans l'espace
            typedef Mesh< MeshCaracStd< 3, 2, 0, T, 2  > > TM_s_3; /// avec des Triangle_6
            typedef TM_3D::TNode TNode;
            typedef TM_3D::Pvec Pvec;

            Pvec mini = -1, maxi = 1;
            TM_s m;
            
            /// création du maillage
            make_rect( m, Quad(), mini, maxi, 20 );
            
            for( unsigned i = 0; i < m.node_list.size(); ++i ) {
                T tmp = m.node_list[ i ].pos[ 0 ] * m.node_list[ i ].pos[ 0 ] + m.node_list[ i ].pos[ 1 ] * m.node_list[ i ].pos[ 1 ];
                m.node_list[ i ].pos[ 2 ] = 1. - std::sin( tmp );    
            }
            
            m.update_skin();
            display_mesh( m );
            
            TM_3D res;
        
            extrude( res, m, 0.25, 1 );

            display_mesh( res );
        
            return 0;
        }


    Pour générer Mesh_carac_test_extrude , vous aurez besoin des fichiers formulation_test_extrude.py :
    \code Python
        # -*- coding: utf-8 -*-

        deter = Variable( interpolation='elementary', default_value='0.0', unit='' )
        #interface_n = Variable( default_value='0.0', unit='' )
        #interface = Variable( interpolation='elementary', default_value='0.0', unit='' )
        #normal = Variable( nb_dim=[3], default_value='0.0', unit='' )
        #dep  = Variable( default_value='0.0', nb_dim=[dim], unit='m' )
        #ener = Variable( interpolation='elementary', default_value='0.0', unit='W/m^2/K' )
        #numsst = Variable( interpolation='global', default_value='0.0', unit='' )
        #attribut_mortel = Variable( T='Mat<double,Gen<>,SparseUMFPACK,void>', unit='Jojo' )
        # T précise le type exact de l'attribut
        
        
        #
        def formulation():
            return 0
    
    et MeshCarac_test_extrude.h.py :
    \code Python
        # -*- coding: utf-8 -*-
        from LMT.formal_lf import *
        
        write_pb(
            name = 'test_extrude',
            formulations = ['test_extrude'],
            #elements = [ 'Bar', 'Triangle', 'Tetra', 'Quad', 'Hexa', 'Wedge' ],
            #elements = ['Triangle', 'Wedge' ],
            #elements = ['Quad','Hexa'],
            elements = ['Triangle','Triangle_6','Quad','Tetra','Hexa','Wedge'],
        )
 
*/
template < class TM_RES, class TM_SCR, class T >
void extrude( TM_RES &res, const TM_SCR &m, T thickness, unsigned nb_layer = 1 ) {
    typedef typename TM_RES::MA::Pvec Pvec;
    typedef GetNormal< Pvec, typename TM_SCR::TNode > GN;

    GN gn;

    /// maillage 2D
    if ( TM_SCR::MA::dim == 2 ) {
        /// la directon d'extrusion est toujours (Oz) => 1 vecteur
        //PRINT( "coucou" );
        gn.map_normal[ NULL ] = Pvec( 0., 0., 1. );
        Extruder< TM_RES, typename GN::MapPvec, T, true > extruder( res, gn.map_normal, thickness, nb_layer );
        apply( m.elem_list, extruder );
        return;
    }
    
    /// maillage 3D
    if ( TM_SCR::MA::dim == 3 ) {
        /// maillage surfacique
        if ( TM_SCR::MA::nvi == 2 ) {
            /// on calcule la somme des normales des éléments adjacents à chaque noeud
            apply( m.elem_list, gn );
            /// on normalise les normales
            gn.normalize();
            /// on extrude suivant la direction des normales
            Extruder< TM_RES, typename GN::MapPvec, T, false > extruder( res, gn.map_normal, thickness, nb_layer );
            apply( m.elem_list, extruder );
            return;
        }
        /// maillage volumique dont la peau sera extrudée
        if ( TM_SCR::MA::nvi == 3 ) {
            /// on calcule la somme des normales des éléments adjacents à chaque noeud
            apply( m.skin.elem_list, gn );
            /// on normalise les normales
            gn.normalize();
            /// on extrude suivant la direction des normales
            Extruder< TM_RES, typename GN::MapPvec, T, false > extruder( res, gn.map_normal, thickness, nb_layer );
            apply( m.skin.elem_list, extruder );
            return;
        }
    }
    
    std::cerr << "Error extrude() : nothing has been done." << std::endl;
}

}

#endif // LMT_extrude_HEADER
