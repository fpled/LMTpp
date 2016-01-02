//
// C++ Interface: Triangle
//
// Description:
//
//
// Author: Hugo LECLERC <leclerc@lmt.ens-cachan.fr>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMTTRIANGLE_H
#define LMTTRIANGLE_H

#include "../containers/staticassert.h"
#include "../containers/basicops.h"
#include "Bar.h"

namespace LMT {

// --------------------------------------------------------------------------------------------------------
/*!
    Triangle à 3 noeuds
    \verbatim
    .                    2
    .                    |\
    .                    | \
    .                    |  \
    .                    |   \
    .                    0----1
    \relates Mesh
    \relates Element
    \keyword Maillage/Elément
    \author Hugo LECLERC
    \friend hugo.leclerc@lmt.ens-cachan.fr
    \friend samir.amrouche@lmt.ens-cachan.fr
*/
struct Triangle {
    static const unsigned nb_var_inter = 2;
    static const unsigned nb_nodes = 3;
    static const char *name() { return "Triangle"; }
    static const char *can_directly_be_represented_by() { return "Triangle"; }
};

// --------------------------------------------------------------------------------------------------------
template<> struct NbChildrenElement<Triangle,0> { static const unsigned res = 1; };
template<> struct NbChildrenElement<Triangle,1> { static const unsigned res = 3; };
template<> struct NbChildrenElement<Triangle,2> { static const unsigned res = 3; };

template<unsigned n> struct TypeChildrenElement<Triangle,0,n> { typedef Triangle T; };
template<unsigned n> struct TypeChildrenElement<Triangle,1,n> { typedef Bar T; };
template<unsigned n> struct TypeChildrenElement<Triangle,2,n> { typedef NodalElement T; };

template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Triangle,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<0> nvi_to_subs) {
   het.add_element(e,ch,Triangle(),e.node(0),e.node(1),e.node(2));
}
template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Triangle,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<1> nvi_to_subs) {
    het.add_element(e,ch,Bar(),e.node(0),e.node(1));
    het.add_element(e,ch,Bar(),e.node(1),e.node(2));
    het.add_element(e,ch,Bar(),e.node(2),e.node(0));
}
template<class TN,class TNG,class TD,unsigned NET,class TC,class HET>
void append_skin_elements(Element<Triangle,TN,TNG,TD,NET> &e,TC &ch,HET &het,Number<2> nvi_to_subs) {
    het.add_element(e,ch,NodalElement(),e.node(0));
    het.add_element(e,ch,NodalElement(),e.node(1));
    het.add_element(e,ch,NodalElement(),e.node(2));
}

// --------------------------------------------------------------------------------------------------------
template<class TN,class TNG,class TD,unsigned NET,class Pvec,class T>
void update_circum_center(const Element<Triangle,TN,TNG,TD,NET> &e,Pvec &C,T &R) {
    C = getCenterOfCircumCircle( e.node(0)->pos, e.node(1)->pos, e.node(2)->pos );
    R = length( e.node(0)->pos - C );
}

template<class TN,class TNG,class TD,unsigned NET,class Pvec,class T>
void update_in_center(const Element<Triangle,TN,TNG,TD,NET> &e,Pvec &C,T &R) {
    C = getCenterOfInCircle( e.node(0)->pos, e.node(1)->pos, e.node(2)->pos );
    R = 2 * measure(e) / ( length( e.node(1)->pos - e.node(0)->pos ) + length( e.node(2)->pos - e.node(0)->pos ) + length( e.node(2)->pos - e.node(1)->pos ) );
}

template<class TN,class TNG,class TD,unsigned NET,class T>
void update_radius_ratio(const Element<Triangle,TN,TNG,TD,NET> &e,T &radius_ratio) {
    Vec<T> C_circum, C_in;
    T R_circum, R_in;
    update_circum_center( e, C_circum, R_circum );
    update_in_center( e, C_in, R_in );
    radius_ratio = R_in / R_circum;
}

template<class TN,class TNG,class TD,unsigned NET,class TM,class T>
void update_edge_ratio(const Element<Triangle,TN,TNG,TD,NET> &e,TM &m,T &edge_ratio) {
    T edge_length_0 = (m.get_children_of( e, Number<1>() )[ 0 ])->measure_virtual();
    T edge_length_1 = (m.get_children_of( e, Number<1>() )[ 1 ])->measure_virtual();
    T edge_length_2 = (m.get_children_of( e, Number<1>() )[ 2 ])->measure_virtual();
    edge_ratio = min( edge_length_0, edge_length_1, edge_length_2 ) / max( edge_length_0, edge_length_1, edge_length_2 );
}

template<class TN,class TNG,class TD,unsigned NET,class Pvec,class T>
void intersection(const Element<Triangle,TN,TNG,TD,NET> &e,Pvec P1,Pvec P2,T &numP1P2,T &dist_ext) {
    DEBUGASSERT( (TNG::dim==3) );

    Pvec T0(e.node(0)->pos), T1(e.node(1)->pos), T2(e.node(2)->pos);
    Pvec N = vect_prod(T1-T0,T2-T0);
    numP1P2 = dot(T0-P1,N) / dot(P2-P1,N);

    Pvec pres = P1 + (P2-P1) * numP1P2;
    T dist0 = dot(vect_prod(T0-T1,T2-T1),vect_prod(pres-T1,T2-T1));
    T dist1 = dot(vect_prod(T1-T2,T0-T2),vect_prod(pres-T2,T0-T2));
    T dist2 = dot(vect_prod(T2-T0,T1-T0),vect_prod(pres-T0,T1-T0));
    if ( dist0>=0 && dist1>=0 && dist2>=0 ) // interior
        dist_ext = -min(min(fabs(dist0),fabs(dist1)),fabs(dist2));
    else  // exterior
        dist_ext = min(min(fabs(dist0),fabs(dist1)),fabs(dist2));
}

template<class TN,class TNG,class TD,unsigned NET>
typename TypePromote<Abs,typename TNG::T>::T measure( const Element<Triangle,TN,TNG,TD,NET> &e ) {
    typename TNG::Pvec P0 = e.node(0)->pos, P1 = e.node(1)->pos, P2 = e.node(2)->pos, N = normalized(P1-P0);
    P2 -= dot(P2-P0,N)*N;
    return abs( 0.5 * length( P1-P0 ) * length( P2-P0 ) );
}

template<class PV>
typename PV::T sample_normal( Triangle, const PV &pos ) {
    typename PV::T res = vect_prod( Pvec( pos[ 1 ] - pos[ 0 ] ), Pvec( pos[ 2 ] - pos[ 0 ] ) );
    return res / length( res );
}

template<class TN,class TNG,class TD,unsigned NET>
typename TNG::Pvec sample_normal(const Element<Triangle,TN,TNG,TD,NET> &e) {
    DEBUGASSERT( (TNG::dim==3) );
    typedef typename TNG::Pvec Pvec;
    Pvec res = vect_prod( Pvec( e.node(1)->pos-e.node(0)->pos ), Pvec( e.node(2)->pos-e.node(0)->pos ) );
    return res / length( res );
}

template<class TN,class TNG,class TD,unsigned NET,class TM>
bool divide_element(Element<Triangle,TN,TNG,TD,NET> &e,TM &m,TNG **nnodes) {
    typedef Element<Triangle,TN,TNG,TD,NET> TE;
    typedef typename TNG::T T;
    if ( nnodes[0] && nnodes[1] && nnodes[2] ) {
        int sommet = -1;
        T l01 = length( e.node(0)->pos - e.node(1)->pos );
        T l12 = length( e.node(1)->pos - e.node(2)->pos );
        T l02 = length( e.node(0)->pos - e.node(2)->pos );
        T r = 1.43; /// pas trop d'idée... disons > sqrt(2)
        if ( l01 < l12 ) {
            if ( l02 > l12 ) {
                if ( l02 > r * l12 )
                    sommet = 1; 
            } else {
                if ( l02 > l01 ) {
                    if ( l12 > r * l02 )
                        sommet = 0;
                } else {
                    if ( l12 > r * l01 )
                        sommet = 0;
                }
            }
        } else {
            if ( l02 > l01 ) {
                if ( l02 > r * l01 )
                    sommet = 1; 
            } else {
                if ( l02 > l12 ) {
                    if ( l01 > r * l02 )
                        sommet = 2;
                } else {
                    if ( l01 > r * l12 )
                        sommet = 2;
                }
            }        
        }
        
        switch( sommet ) {
            case 0 :
                DM::copy( e, *m.add_element( Triangle(), TN(), e.node(0), nnodes[0], nnodes[1] ) );
                DM::copy( e, *m.add_element( Triangle(), TN(), e.node(0), nnodes[1], nnodes[2] ) );
                DM::copy( e, *m.add_element( Triangle(), TN(), nnodes[0], e.node(1), nnodes[1] ) );
                DM::copy( e, *m.add_element( Triangle(), TN(), e.node(2), nnodes[2], nnodes[1] ) );            
                break;
            case 1 :
                DM::copy( e, *m.add_element( Triangle(), TN(), e.node(1), nnodes[1], nnodes[2] ) );
                DM::copy( e, *m.add_element( Triangle(), TN(), e.node(1), nnodes[2], nnodes[0] ) );
                DM::copy( e, *m.add_element( Triangle(), TN(), nnodes[1], e.node(2), nnodes[2] ) );
                DM::copy( e, *m.add_element( Triangle(), TN(), e.node(0), nnodes[0], nnodes[2] ) ); 
                break;
            case 2 :
                DM::copy( e, *m.add_element( Triangle(), TN(), e.node(2), nnodes[2], nnodes[0] ) );
                DM::copy( e, *m.add_element( Triangle(), TN(), e.node(2), nnodes[0], nnodes[1] ) );
                DM::copy( e, *m.add_element( Triangle(), TN(), nnodes[2], e.node(0), nnodes[0] ) );
                DM::copy( e, *m.add_element( Triangle(), TN(), e.node(1), nnodes[1], nnodes[0] ) );
                break;        
            default :
                DM::copy( e, *m.add_element( Triangle(), TN(), e.node(0), nnodes[0], nnodes[2] ) );
                DM::copy( e, *m.add_element( Triangle(), TN(), nnodes[2], nnodes[0], nnodes[1] ) );
                DM::copy( e, *m.add_element( Triangle(), TN(), nnodes[1], nnodes[0], e.node(1) ) );
                DM::copy( e, *m.add_element( Triangle(), TN(), e.node(2), nnodes[2], nnodes[1] ) );
        }
    }
    else if ( nnodes[0] && nnodes[1] ) {
        T l_0_n1 = length( e.node(0)->pos - nnodes[1]->pos );
        T l_2_n0 = length( e.node(2)->pos - nnodes[0]->pos );
        if ( l_0_n1 > l_2_n0 ) {
            DM::copy( e, *m.add_element( Triangle(), TN(), e.node(0), nnodes[0], e.node(2) ) );
            DM::copy( e, *m.add_element( Triangle(), TN(), nnodes[0], nnodes[1], e.node(2) ) );
            DM::copy( e, *m.add_element( Triangle(), TN(), e.node(1), nnodes[1], nnodes[0] ) );
        } else {
            DM::copy( e, *m.add_element( Triangle(), TN(), e.node(0), nnodes[0], nnodes[1] ) );
            DM::copy( e, *m.add_element( Triangle(), TN(), nnodes[0], e.node(1), nnodes[1] ) );
            DM::copy( e, *m.add_element( Triangle(), TN(), e.node(0), nnodes[1], e.node(2) ) );        
        }
    }
    else if ( nnodes[0] && nnodes[2] ) {
        T l_1_n2 = length( e.node(1)->pos - nnodes[2]->pos );
        T l_2_n0 = length( e.node(2)->pos - nnodes[0]->pos );
        if ( l_1_n2 > l_2_n0 ) {
            DM::copy( e, *m.add_element( Triangle(), TN(), nnodes[2], e.node(0), nnodes[0] ) );
            DM::copy( e, *m.add_element( Triangle(), TN(), nnodes[2], nnodes[0], e.node(2) ) );
            DM::copy( e, *m.add_element( Triangle(), TN(), nnodes[0], e.node(1), e.node(2) ) );
        } else {
            DM::copy( e, *m.add_element( Triangle(), TN(), nnodes[2], e.node(0), nnodes[0] ) );
            DM::copy( e, *m.add_element( Triangle(), TN(), nnodes[2], nnodes[0], e.node(1) ) );
            DM::copy( e, *m.add_element( Triangle(), TN(), nnodes[2], e.node(1), e.node(2) ) );        
        }
    }
    else if ( nnodes[1] && nnodes[2] ) {
        T l_0_n1 = length( e.node(0)->pos - nnodes[1]->pos );
        T l_1_n2 = length( e.node(1)->pos - nnodes[2]->pos );
        if ( l_0_n1 > l_1_n2 ) {
            DM::copy( e, *m.add_element( Triangle(), TN(), nnodes[2], e.node(0), e.node(1) ) );
            DM::copy( e, *m.add_element( Triangle(), TN(), nnodes[2], e.node(1), nnodes[1] ) );
            DM::copy( e, *m.add_element( Triangle(), TN(), nnodes[2], nnodes[1], e.node(2) ) );
        } else {
            DM::copy( e, *m.add_element( Triangle(), TN(), nnodes[2], e.node(0), nnodes[1] ) );
            DM::copy( e, *m.add_element( Triangle(), TN(), nnodes[2], nnodes[1], e.node(2) ) );
            DM::copy( e, *m.add_element( Triangle(), TN(), e.node(0), e.node(1), nnodes[1] ) );        
        }
    }
    else if ( nnodes[0] ) {
        DM::copy( e, *m.add_element( Triangle(), TN(), e.node(0), nnodes[0], e.node(2) ) );
        DM::copy( e, *m.add_element( Triangle(), TN(), nnodes[0], e.node(1), e.node(2) ) );
    }
    else if ( nnodes[1] ) {
        DM::copy( e, *m.add_element( Triangle(), TN(), e.node(0), e.node(1), nnodes[1] ) );
        DM::copy( e, *m.add_element( Triangle(), TN(), e.node(0), nnodes[1], e.node(2) ) );
    }
    else if ( nnodes[2] ) {
        DM::copy( e, *m.add_element( Triangle(), TN(), e.node(0), e.node(1), nnodes[2] ) );
        DM::copy( e, *m.add_element( Triangle(), TN(), nnodes[2], e.node(1), e.node(2) ) );
    }

    return ( nnodes[0] || nnodes[1] || nnodes[2] );
}

template<class TN,class TNG,class TD,unsigned NET,class TM>
bool divide_element_using_elem_children(Element<Triangle,TN,TNG,TD,NET> &e,TM &m,TNG **nnodes) {
    return divide_element(e,m,nnodes);
}

/** new_nodes are independant nodes created to make a fine grid for integration. Used in TvrcFormulation
    \relates Triangle
    \todo actually, we divide element n times, with n = max(edge length) / max_dist. For flat elements, it's far from optimallity
*/
template<class TN,class TNG,class TD,unsigned NET,class T>
bool subdivision_element(const Element<Triangle,TN,TNG,TD,NET> &e,Vec<TNG> &new_nodes,T max_dist) {
    T max_l = max( length( e.node(1)->pos - e.node(0)->pos ), length( e.node(2)->pos - e.node(1)->pos ), length( e.node(0)->pos - e.node(2)->pos ) );
    if ( max_l <= max_dist )
        return false;
    unsigned n = (unsigned)(max_l / max_dist);
    for(unsigned i=0;i<n;++i) {
        for(unsigned j=0;j<=i;++j) {
            TNG *nn = new_nodes.new_elem();
            T f1 = (i+1.0)/(n+1.0);
            T f2 = (j+1.0)/(n+1.0);
            T f3 = 1.0 - f1 - f2;
            std::pair<T,const TNG *> pond_list[] = {
                std::pair<T,const TNG *>( f1, e.node(0) ),
                std::pair<T,const TNG *>( f2, e.node(1) ),
                std::pair<T,const TNG *>( f3, e.node(2) )
            };
            DM::get_ponderation( pond_list, 3, *nn );
        }
    }
    return true;
}

template<class TV,class T>
bool var_inter_is_inside( const Triangle &e, const TV &var_inter, T tol = 0 ) {
    return heaviside( var_inter[0] + tol ) * heaviside( var_inter[1] + tol ) * heaviside( 1 - var_inter[0] - var_inter[1] + tol );
}

/// >= 0 -> inside, < 0 -> outside
template<class T,class TV>
T var_inter_insideness( const Triangle &e, const TV &var_inter ) {
    return min( min( var_inter[0], var_inter[1] ), 1 - var_inter[0] - var_inter[1] );
}

/*!
\generic_comment is_inside_linear
    objectif :
        la fonction renvoie vrai si pos est dans l'élément défini par les noeuds "sommets" et faux sinon. Pour les éléments linéaires ( i.e. d'ordre 1 ) cette fonction est équivalente à \a is_inside () mais pour pour des éléments non linéaires, ces fonctions peuvent être différentes. Par exemple pour l'élément non linéaire \a Triangle_6 la fonction renvoie vrai si pos est contenu dans le triangle défini par les sommets de \a Triangle_6 ( i.e. sans tenir compte des noeuds sur les côtés ).
        
    paramètre :
        le premier paramètre est le type d'élément
        pos_nodes : le position des sommets dans le plan de l'élément
        pos : la position du point dans le plan

*/

/*!
    objectif :
        la fonction renvoie vrai si pos est dans le triangle et faux sinon ( sous la condition que le \a Triangle est dans le plan ).
        
    paramètre :
        Triangle : le type d'élément
        pos_nodes : le position des sommets dans le plan
        pos : la position du point dans le plan

*/
template< class PosNodes, class Pvec > 
bool is_inside_linear( const Triangle &elem, const PosNodes &pos_nodes, const Pvec &pos ) {
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
        if ( ( det1 * det3 ) >= 0 and ( det2 * det3 ) >=0 )
            return true;
        else
            return false;
    } else
        return false;
}

inline unsigned vtk_num( StructForType<Triangle> ) { return 5; }

}

#include "element_Triangle.h"

#endif // LMTTRIANGLE_H
