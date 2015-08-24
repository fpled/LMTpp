#ifndef LMTTRIANGLE10_H
#define LMTTRIANGLE10_H

#include "../containers/staticassert.h"
#include "../containers/basicops.h"
#include "bar_4.h"

namespace LMT {

/*!
    \verbatim
            2
            | \
       faux |  \
            5   4
            |    \
            |     \
            0__3___1


    \relates Mesh
    \keyword Maillage/Elément 
    \author Hugo LECLERC
    \friend rapahel.pasquier@lmt.ens-cachan.fr
    \friend hugo.leclerc@lmt.ens-cachan.fr
*/

// --------------------------------------------------------------------------------------------------------
struct Triangle_10 {
    static const unsigned nb_var_inter = 2;
    static const unsigned nb_nodes = 10;
    static const char *name() { return "Triangle_10"; }
    static const char *can_directly_be_represented_by() { return "Triangle_10"; }
};

// --------------------------------------------------------------------------------------------------------
template<> struct NbChildrenElement<Triangle_10,0> { static const unsigned res = 1; };
template<> struct NbChildrenElement<Triangle_10,1> { static const unsigned res = 3; };
template<> struct NbChildrenElement<Triangle_10,2> { static const unsigned res = 10; };

template<unsigned n> struct TypeChildrenElement<Triangle_10,0,n> { typedef Triangle_10 T; };
template<unsigned n> struct TypeChildrenElement<Triangle_10,1,n> { typedef Bar_4 T; };
template<unsigned n> struct TypeChildrenElement<Triangle_10,2,n> { typedef NodalElement T; };

template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Triangle_10,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<0> nvi_to_subs) {
    assert( 0 );
    // TODO
    het.add_element(e,ch,Triangle_10(),e.node(0),e.node(1),e.node(2),e.node(3),e.node(4),e.node(5));
}

template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Triangle_10,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<1> nvi_to_subs) {
    assert( 0 );
    // TODO
    het.add_element(e,ch,Bar_3(),e.node(0),e.node(1),e.node(3));
    het.add_element(e,ch,Bar_3(),e.node(1),e.node(2),e.node(4));
    het.add_element(e,ch,Bar_3(),e.node(2),e.node(0),e.node(5));
}
template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Triangle_10,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<2> nvi_to_subs) {
    assert( 0 );
    // TODO
    het.add_element(e,ch,NodalElement(),e.node(0));
    het.add_element(e,ch,NodalElement(),e.node(1));
    het.add_element(e,ch,NodalElement(),e.node(2));
    het.add_element(e,ch,NodalElement(),e.node(3));
    het.add_element(e,ch,NodalElement(),e.node(4));
    het.add_element(e,ch,NodalElement(),e.node(5));
}

template<class TN,class TNG,class TD,unsigned NET,class TM>
bool divide_element(Element<Triangle_10,TN,TNG,TD,NET> &e,TM &m,TNG **nnodes) {
   std::cout << "divide_element not implemented for Triangle_10" << std::endl;
   assert(0);
   return false;
}

template<class TN,class TNG,class TD,unsigned NET,class TM>
bool divide_element_using_elem_children(Element<Triangle_10,TN,TNG,TD,NET> &e,TM &m,TNG **nnodes) {
    return divide_element(e,m,nnodes);
}

// --------------------------------------------------------------------------------------------------------
template<class TN,class TNG,class TD,unsigned NET>
typename TNG::Pvec sample_normal(const Element<Triangle_10,TN,TNG,TD,NET> &e) {
    assert( 0 );
    // TODO
    DEBUGASSERT( (TNG::dim==3) );
    typename TNG::Pvec res = vect_prod( e.node(1)->pos-e.node(0)->pos, e.node(2)->pos-e.node(0)->pos );
    return res / length( res );
}
template<class TN,class TNG,class TD,unsigned NET>
typename TypePromote<Abs,typename TNG::T>::T measure( const Element<Triangle_10,TN,TNG,TD,NET> &e ) {
    assert( 0 );
    // TODO
    typename TNG::Pvec P0 = e.node(0)->pos, P1 = e.node(1)->pos, P2 = e.node(2)->pos, N = normalized(P1-P0);
    P2 -= dot(P2-P0,N)*N;
    return abs( 0.5 * length( P1-P0 ) * length( P2-P0 ) );
}

/*!
    Cette fonction retourne l'isobarycentre des sommets du triangle.

    \friend raphael.pasquier@lmt.ens-cachan.fr
    \friend hugo.leclerc@lmt.ens-cachan.fr
*/
template<class TN,class TNG,class TD,unsigned NET>
typename TNG::Pvec center(const Element<Triangle_10,TN,TNG,TD,NET> &e) {
    assert( 0 );
    // TODO
    typename TNG::Pvec res = 0.0;
    for(unsigned i=0;i<3;++i)
        res += e.pos(i);
    return res / 3;
}

template<class TV,class T>
bool var_inter_is_inside( const Triangle_10 &e, const TV &var_inter, T tol = 0 ) {
    assert( 0 );
    // TODO
    return heaviside( var_inter[0] + tol ) * heaviside( var_inter[1] + tol ) * heaviside( 1 - var_inter[0] - var_inter[1] + tol );
}

/// >= 0 -> inside, < 0 -> outside
template<class T,class TV>
T var_inter_insideness( const Triangle_10 &e, const TV &var_inter ) {
    assert( 0 );
    // TODO
    return min( min( var_inter[0], var_inter[1] ), 1 - var_inter[0] - var_inter[1] );
}

/*!
    Objectif :
        la fonction renvoie vrai si pos est dans le triangle formé par les trois sommets et faux sinon ( sous la condition que le \a Triangle_10 est dans le plan ).
        
    paramètres :
        Triangle_10 : le type d'élément
        pos_nodes : le position des sommets dans le plan
        pos : la position du point dans le plan

*/
template< class PosNodes, class Pvec > 
bool is_inside_linear( const Triangle_10 &elem, const PosNodes &pos_nodes, const Pvec &pos ) {
    assert( 0 );
    // TODO
    typedef typename Pvec::template SubType<0>::T T;
    
    Pvec AB = pos_nodes[ 1 ] - pos_nodes[ 0 ];
    Pvec BC = pos_nodes[ 2 ] - pos_nodes[ 1 ];
    Pvec XM = pos - pos_nodes[ 0 ];
    
    T det1 = AB[ 0 ] * XM[ 1 ] - AB[ 1 ] * XM[ 0 ];
    XM = pos - pos_nodes[ 1 ];
    T det2 = BC[ 0 ] * XM[ 1 ] - BC[ 1 ] * XM[ 0 ];
    
    if ( ( det1 * det2 ) >= 0 ) {
        Pvec CA = pos_nodes[ 0 ] - pos_nodes[ 2 ];
        XM = pos - pos_nodes[ 2 ];
        T det3 = CA[ 0 ] * XM[ 1 ] - CA[ 1 ] * XM[ 0 ];
        if ( ( det1 * det3 ) >= 0 and ( det2 * det3 ) >= 0 )
            return true;
        else
            return false;
    } else
        return false;
}


};
#include "element_Triangle_10.h"

#endif
