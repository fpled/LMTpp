#ifndef REPLACE_WEDGE_BY_TETRA_H
#define REPLACE_WEDGE_BY_TETRA_H

#include "mesh.h"
#include "Tetra.h"
#include "Wedge.h"

namespace LMT {
 
struct OrderNumber {

    typedef unsigned Ret;

    template<class TNode>
    Ret operator()( const TNode *n ) {
        return n->number;
    }

};

template<class Pvec> 
struct Pos {
    Pos() : data( 0 ) {}
    Pos( const Pvec &p ) : data( p ) {} 
    
    bool operator<( const Pos<Pvec> &p2 ) const {
    
        const unsigned char *a = reinterpret_cast<const unsigned char*>( &data );
        const unsigned char *b = reinterpret_cast<const unsigned char*>( &p2.data );
        unsigned carry = 0;
        for( unsigned i = 0; i < sizeof( Pvec ); ++i ) {
            unsigned ca = a[ i ] + carry;
            unsigned cb = b[ i ];
            if ( cb < ca )
                carry = 1;
            else 
                carry = 0; 
        }

        return ( carry != 1 );
    }
    
    bool operator==( const Pos<Pvec> &p2 ) const {
    
        const unsigned char *a = reinterpret_cast<const unsigned char*>( &data );
        const unsigned char *b = reinterpret_cast<const unsigned char*>( &p2.data );
        for( unsigned i = 0; i < sizeof( Pvec ); ++i )
            if ( a[ i ] != b[ i ] )
                return false;
        return true;
    }
    
    Pvec data;
};

template<class Pvec>
struct OrderPosition {

    typedef Pos<Pvec> Ret;

    template<class TNode>
    Ret operator()( const TNode *n ) {
        return Ret( n->pos );
    }

};

/*!
    L'opérateur \a Replace_Wedge_by_Tetra est conçu pour la fonction \a replace_Wedge_by_Tetra .
*/
template<class TM, class Order >
struct Replace_Wedge_by_Tetra {
    Replace_Wedge_by_Tetra( TM &m, const Order &order ) : ptr_m( &m ), impossible( false ) , o( order ) {} 
    
    template< class NameBehavior, class TNode_, class TData, unsigned num_in_elem_list_ >
    bool operator() ( Element< Wedge, NameBehavior, TNode_, TData, num_in_elem_list_ > &e ) {
         
        typename Element< Wedge, NameBehavior, TNode_, TData, num_in_elem_list_ >::TNode* nn[4];

        if ( impossible ) return false;
        
        typedef typename Element< Wedge, NameBehavior, TNode_, TData, num_in_elem_list_ >::TNode::T T;
    
        T l1, l2;
        typename Order::Ret min1, min2;
        unsigned BF, BD, CD;
        
        l1 = length( e.pos( 1 ) - e.pos( 5 ) );
        l2 = length( e.pos( 2 ) - e.pos( 4 ) );
        if ( l1 < l2 ) 
            BF = 1;
        else {
            if ( l2 > l1 )
                BF = 0;
            else {
                min1 = min( o( e.node( 1 ) ), o( e.node( 5 ) ) );
                min2 = min( o( e.node( 2 ) ), o( e.node( 4 ) ) );
                if ( min1 < min2 ) BF = 1; else BF = 0;
            }
        }
    
        l1 = length( e.pos( 1 ) - e.pos( 3 ) );
        l2 = length( e.pos( 0 ) - e.pos( 4 ) );
        if ( l1 < l2 ) 
            BD = 1;
        else {
            if ( l2 > l1 )
                BD = 0;
            else {
                min1 = min( o( e.node( 1 ) ), o( e.node( 3 ) ) );
                min2 = min( o( e.node( 0 ) ), o( e.node( 4 ) ) );
                if ( min1 < min2 ) BD = 1; else BD = 0;
            }
        }
    
        l1 = length( e.pos( 2 ) - e.pos( 3 ) );
        l2 = length( e.pos( 0 ) - e.pos( 5 ) );
        if ( l1 < l2 ) 
            CD = 1;
        else {
            if ( l2 > l1 )
                CD = 0;
            else {
                min1 = min( o( e.node( 2 ) ), o( e.node( 3 ) ) );
                min2 = min( o( e.node( 0 ) ), o( e.node( 5 ) ) );
                if ( min1 < min2 ) CD = 1; else CD = 0;
            }
        }
        
        unsigned index = BF + 2 * BD + 4 * CD;
        
        switch( index ) {
            case 0 : /// 000
                nn[0] = e.node( 0 );
                nn[1] = e.node( 1 );
                nn[2] = e.node( 2 );
                nn[3] = e.node( 4 );
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *( ptr_m->add_element( Tetra(), NameBehavior(), nn ) ) );
                nn[0] = e.node( 3 );
                nn[1] = e.node( 4 );
                nn[2] = e.node( 5 );
                nn[3] = e.node( 0 );
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *( ptr_m->add_element( Tetra(), NameBehavior(), nn ) ) );
                nn[0] = e.node( 0 );
                nn[1] = e.node( 2 );
                nn[2] = e.node( 5 );
                nn[3] = e.node( 4 );
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *( ptr_m->add_element( Tetra(), NameBehavior(), nn ) ) );
                break;
            case 1 : /// 001
                nn[0] = e.node( 0 );
                nn[1] = e.node( 1 );
                nn[2] = e.node( 2 );
                nn[3] = e.node( 5 );
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *( ptr_m->add_element( Tetra(), NameBehavior(), nn ) ) );
                nn[0] = e.node( 0 );
                nn[1] = e.node( 1 );
                nn[2] = e.node( 4 );
                nn[3] = e.node( 5 );
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *( ptr_m->add_element( Tetra(), NameBehavior(), nn ) ) );
                nn[0] = e.node( 0 );
                nn[1] = e.node( 3 );
                nn[2] = e.node( 5 );
                nn[3] = e.node( 4 );
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *( ptr_m->add_element( Tetra(), NameBehavior(), nn ) ) );
                break;                
            case 2 : /// 010
                impossible = true;
                std::cerr << "Error Replace_Wedge_by_Tetra : case 2" << std::endl;
                return false;
                break;
            case 3 : /// 011
                nn[0] = e.node( 0 );
                nn[1] = e.node( 1 );
                nn[2] = e.node( 2 );
                nn[3] = e.node( 5 );
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *( ptr_m->add_element( Tetra(), NameBehavior(), nn ) ) );
                nn[0] = e.node( 1 );
                nn[1] = e.node( 3 );
                nn[2] = e.node( 4 );
                nn[3] = e.node( 5 );
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *( ptr_m->add_element( Tetra(), NameBehavior(), nn ) ) );
                nn[0] = e.node( 0 );
                nn[1] = e.node( 1 );
                nn[2] = e.node( 3 );
                nn[3] = e.node( 5 );
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *( ptr_m->add_element( Tetra(), NameBehavior(), nn ) ) );
                break;
            case 4 : /// 100
                nn[0] = e.node( 3 );
                nn[1] = e.node( 4 );
                nn[2] = e.node( 5 );
                nn[3] = e.node( 2 );
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *( ptr_m->add_element( Tetra(), NameBehavior(), nn ) ) );
                nn[0] = e.node( 0 );
                nn[1] = e.node( 1 );
                nn[2] = e.node( 2 );
                nn[3] = e.node( 4 );
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *( ptr_m->add_element( Tetra(), NameBehavior(), nn ) ) );
                nn[0] = e.node( 0 );
                nn[1] = e.node( 3 );
                nn[2] = e.node( 4 );
                nn[3] = e.node( 2 );
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *( ptr_m->add_element( Tetra(), NameBehavior(), nn ) ) );
                break;
            case 5 : /// 101
                impossible = true;
                std::cerr << "Error Replace_Wedge_by_Tetra : case 5" << std::endl;
                return false;
                break;
            case 6 : /// 110
                nn[0] = e.node( 3 );
                nn[1] = e.node( 4 );
                nn[2] = e.node( 5 );
                nn[3] = e.node( 2 );
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *( ptr_m->add_element( Tetra(), NameBehavior(), nn ) ) );
                nn[0] = e.node( 1 );
                nn[1] = e.node( 3 );
                nn[2] = e.node( 4 );
                nn[3] = e.node( 2 );
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *( ptr_m->add_element( Tetra(), NameBehavior(), nn ) ) );
                nn[0] = e.node( 0 );
                nn[1] = e.node( 1 );
                nn[2] = e.node( 3 );
                nn[3] = e.node( 2 );
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *( ptr_m->add_element( Tetra(), NameBehavior(), nn ) ) );
                break;
            case 7 : /// 111
                nn[0] = e.node( 0 );
                nn[1] = e.node( 1 );
                nn[2] = e.node( 2 );
                nn[3] = e.node( 3 );
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *( ptr_m->add_element( Tetra(), NameBehavior(), nn ) ) );
                nn[0] = e.node( 1 );
                nn[1] = e.node( 3 );
                nn[2] = e.node( 4 );
                nn[3] = e.node( 5 );
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *( ptr_m->add_element( Tetra(), NameBehavior(), nn ) ) );
                nn[0] = e.node( 1 );
                nn[1] = e.node( 2 );
                nn[2] = e.node( 5 );
                nn[3] = e.node( 3 );
                permutation_if_jac_neg ( Tetra(), nn );
                DM::copy( e, *( ptr_m->add_element( Tetra(), NameBehavior(), nn ) ) );
                break;
            default : ;
        }
        return true;    
    }
    
    /// pour tous les autres éléments autres que \a Wedge , on renvoie faux
    template<class TE> 
    bool operator() ( TE &e ) { 
        return false;
    }

    TM* ptr_m;
    bool impossible;
    Order o;
};

/*!
    Objectif :
        La fonction \a replace_Wedge_by_Tetra enlève tous les \a Wedge du maillage m et les remplace par des \a Tetra . Le maillage m est donc __modifié__ .

    Paramètre :
        * m : est un maillage de la plateforme qui sera modifié.
        * order : est un opérateur qui doit pouvoir ordonner les noeuds de l'espace, i.e. si on a deux noeuds n1 et n2, order( n1 ) doit renvoyer une classe pour laquelle l'opérateur < a un sens. L'opérateur le plus simple est celui qui renvoie l'indice du noeud dans le maillage. Il s'appelle \a OrderNumber .
        
    Lorsqu'on manipule plusieurs maillages qui ont des noeuds superposés au niveau de la peau, on peut souhaiter que la découpe des \a Wedge se fasse de façon conforme ( i.e. ne pas avoir des \a Tetra dont les arêtes se croisent ). On peut alors utiliser la position des noeuds pour les identifier avec la classe \a OrderPosition .  

    Problème : si l'opération échoue, un message d'erreur est affiché et le maillage contient des \a Wedge et des \a Tetra avec très certainement des faces de Tetra et de Wedge en non-conformité. Il faut donc ne pas utiliser le maillage!

    Exemple de code pour remplacer les \a Wedge par des \a Tetra dans un maillage :
    \code C/C++

        #include "mesh/make_rect.h"
        #include "mesh/displayparaview.h"

        // inclusion du code de notre MeshCarac
        #include "MonMeshCarac.h"

        int main( int argc, char **argv ) {
            typedef Mesh< Mesh_carac_MonMeshCarac<double,3> > TM;
            typedef TM::Pvec Pvec;

            TM m;
            make_rect( m, Wedge(), Pvec( 0., 0., 0. ), Pvec( 1., 1., 1. ), Pvec( 2, 2, 2 ) );
   
            display_mesh( m );
    
            replace_Wedge_by_Tetra( m, OrderNumber() );
            // replace_Wedge_by_Tetra( m, OrderPosition<Pvec>() );
        
            display_mesh( m );

            return 0;
        }

    \relates replace_Wedge_by_Tetra
    \relates replace_Hexa_by_Tetra
    \relates replace_Quad_by_Triangle
    \keyword Maillage/Opération
    \friend lecler@lmt.ens-cachan.fr
*/
template <class TM, class Order >
void replace_Wedge_by_Tetra( TM &m, const Order &order ) {

    Replace_Wedge_by_Tetra<TM, Order> rw( m, order );
    
    m.remove_elements_if( rw );
    
    if ( rw.impossible ) 
        std::cerr << "Error replace_Wedge_by_Tetra() : impossible to replace all wedges. " << std::endl;
}

}
#endif  /// REPLACE_WEDGE_BY_TETRA
 
