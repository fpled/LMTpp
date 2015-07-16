#ifndef CRACK_CUT_H
#define CRACK_CUT_H

#include <mesh/refinement.h>
#include <mesh/sep_mesh.h>

namespace LMT {

struct ProjGrad {
    template<class TE>
    double operator()( const TE &e, Vec<double> &vg, Vec<double> &sg ) const {
        for( int i = 0; i < e.nb_nodes; ++i ) {
            vg[ e.node( i )->number ] += length( e.epsilon[ 0 ] );
            sg[ e.node( i )->number ] += 1;
        }
    }
};

template<class Pvec>
struct MyRefinement {
    template<class TE>
    double operator()( TE &e ) {
        int n0 = e.node( 0 )->number;
        int n1 = e.node( 1 )->number;
        Pvec d0 = e.node( 0 )->dep_sv;
        Pvec d1 = e.node( 1 )->dep_sv;
        Pvec dm = dir; //  = d0 - d1; // direction moyenne
        double s0 = dot( d0, dm );
        double s1 = dot( d1, dm );
        double gr = 0.5 * ( (*vg)[ n0 ] + (*vg)[ n1 ] );
        if ( gr < lim or s0 * s1 > 0 )
            return -1;


        // noeuds trop proches des coupes potentielles
        double r = s0 / ( s0 - s1 );
        Pvec cut = ( 1 - r ) * e.node( 0 )->pos + ( 0 + r ) * e.node( 1 )->pos;

        if ( r < 0.1 ) {
            if ( e.node( 0 )->number < allowed_to_move.size() and allowed_to_move[ e.node( 0 )->number ] )
                e.node( 0 )->pos = cut;
            allowed_to_move[ e.node( 0 )->number ] = false;
            cuts << e.node( 0 )->pos;
            return -1;
        }
        if ( r > 0.9 ) {
            if ( e.node( 1 )->number < allowed_to_move.size() and allowed_to_move[ e.node( 1 )->number ] )
                e.node( 1 )->pos = cut;
            allowed_to_move[ e.node( 1 )->number ] = false;
            cuts << e.node( 1 )->pos;
            return -1;
        }

        cuts << cut;
        return r;
    }
    Vec<double> allowed_to_move;
    Vec<double> *vg;
    Vec<Pvec> cuts;
    double lim;
    Pvec dir;
};

template<class Pvec,class TM>
bool elem_has_a_fully_cut_child( Pvec &pos, Pvec &normal, TM &m, const typename TM::EA *e, const Vec<bool> &cut ) {
    const typename TM::EA * const *l = m.get_children_of_EA( e, Number<1>() );
    int nb_ok = 0;
    for( int n = 0; n < e->nb_children_virtual( 1 ); ++n ) {
        for( int m = 0; ; ++m ) {
            if ( m == l[ n ]->nb_nodes_virtual() ) {
                Pvec N = l[ n ]->sample_normal_virtual();
                normal += N * ( dot( N, center( *e ) - center( *l[ n ] ) ) > 0 ? 1 : -1 );
                pos += center( *l[ n ] );
                ++nb_ok;
                break;
            }
            if ( not cut[ l[ n ]->node_virtual( m )->number ] )
                break;
        }
    }
    normal /= nb_ok;
    pos /= nb_ok;
    return nb_ok;
}

template<class EA>
bool elem_has_a_cut( const EA *e, const Vec<bool> &cut ) {
    for( int n = 0; n < e->nb_nodes_virtual(); ++n )
        if ( cut[ e->node_virtual( n )->number ] )
            return true;
    return false;
}

template<class EA>
bool fully_cut( const EA *e, const Vec<bool> &cut ) {
    for( int n = 0; n < e->nb_nodes_virtual(); ++n )
        if ( not cut[ e->node_virtual( n )->number ] )
            return false;
    return true;
}

template<class EA>
bool elem_has_a_cut_not_in_border( const EA *e, const Vec<bool> &cut, const Vec<bool> &bor ) {
    for( int n = 0; n < e->nb_nodes_virtual(); ++n )
        if ( cut[ e->node_virtual( n )->number ] and not bor[ e->node_virtual( n )->number ] )
            return true;
    return false;
}

struct GetImmediateParentsInCut {
    template<class TE,class TM> void operator()( TE &e, TM &m, const Vec<bool> &cut ) {
        if ( fully_cut( &e, cut ) )
            append_skin_elements( e, m, *this, Number<1>() );
    }
    template<class TE,class TM,class NE>
    void add_element( TE &e, TM &m, NE ne, typename TM::TNode *n0 = 0, typename TM::TNode *n1 = 0, typename TM::TNode *n2 = 0, typename TM::TNode *n3 = 0 ) {
        typename TM::TNode *n[] = { n0, n1, n2, n3 };
        typename TM::EA *c = m.sub_mesh( Number<2>() ).elem_list.find( ne, DefaultBehavior(), m.sub_mesh( Number<2>() ), n );
        ++(*sub_sub_in_cut)[ c->absolute_number ];
    }
    Vec<int> *sub_sub_in_cut;
};

struct UpdateBor {
    template<class TE,class TM> void operator()( const TE &e, TM &m, Vec<bool> &bor, const Vec<bool> &cut ) const {
        if ( fully_cut( &e, cut ) and sub_sub_in_cut[ e.absolute_number ] == 1 )
            for( int var = 0; var < e.nb_nodes; ++var )
                bor[ e.node( var )->number ] = true;
    }
    Vec<int> sub_sub_in_cut;
};

template<class TM,class Pvec>
void cut_mesh( TM &m, const Vec<Pvec> &cuts ) {
    // node number -> cut or not
    Vec<bool> cut; cut.resize( m.node_list.size(), false );
    for( int n = 0; n < m.node_list.size(); ++n ) {
        Pvec pos = m.node_list[ n ].pos;
        for( int i = 0; i < cuts.size(); ++i ) {
            if ( length( cuts[ i ] - pos ) < 1e-6 ) {
                cut[ n ] = true;
                break;
            }
        }
    }

    for( int o = 0; o < m.node_list.size(); ++o )
        m.node_list[ o ].temperature = cut[ o ];

    // get bor list
    m.update_skin();
    m.update_elem_parents( Number<2>() );
    for( int n = 0; n < m.sub_mesh( Number<2>() ).elem_list.size(); ++n )
        m.sub_mesh( Number<2>() ).elem_list[ n ]->absolute_number = n;

    UpdateBor ud;
    GetImmediateParentsInCut gipic;
    gipic.sub_sub_in_cut = &ud.sub_sub_in_cut;
    ud.sub_sub_in_cut.resize( m.sub_mesh( Number<2>() ).elem_list.size(), 0 );
    apply( m.sub_mesh( Number<1>() ).elem_list, gipic, m, cut );

    Vec<bool> bor; bor.resize( m.node_list.size(), false );
    apply( m.sub_mesh( Number<2>() ).elem_list, ud, m, bor, cut );
    for( int n = 0; n < m.skin.node_list.size(); ++n )
        bor[ m.skin.node_list[ n ].number ] = false;


    // propagation
    Vec<bool> seen_elems; seen_elems.resize( m.elem_list.size(), false );
    m.update_elem_parents( Number<1>() );
    for( int n = 0; n < m.elem_list.size(); ++n )
        m.elem_list[ n ]->absolute_number = n;
    while ( true ) {
        // get a first element to fill the front
        Vec<typename TM::EA *> front;
        for( int n = 0; n < m.elem_list.size(); ++n ) {
            if ( seen_elems[ m.elem_list[ n ]->absolute_number ] )
                continue;
            Pvec pos = 0, normal = 0;
            if ( elem_has_a_fully_cut_child( pos, normal, m, m.elem_list[ n ], cut ) ) {
                front << m.elem_list[ n ];
                seen_elems[ m.elem_list[ n ]->absolute_number ] = true;
                break;
            }
        }
        PRINT( front.size() );
        if ( not front.size() )
            break;

        // propagation
        Vec<typename TM::TNode *> new_nodes; new_nodes.resize( m.node_list.size(), 0 );
        while ( front.size() ) {
            typename TM::EA *e = front.back();
            front.pop_back();

            // change the nodes
            for( int n = 0; n < e->nb_nodes_virtual(); ++n ) {
                int no = e->node_virtual( n )->number;
                if ( cut[ no ] and not bor[ no ] ) {
                    if ( not new_nodes[ no ] ) {
                        new_nodes[ no ] = m.add_node();
                        DM::copy( *e->node_virtual( n ), *new_nodes[ no ] );
                        new_nodes[ no ]->pos = e->node_virtual( n )->pos;
                    }
                    e->set_node_virtual( n, new_nodes[ no ] );
                }
            }

            //static double t = 0;
            //e->set_field( "tr_epsilon", ++t + 10 );
            //if ( ++t == 10 )
            //    return;

            // neighbors
            const typename TM::EA * const *ls = m.get_children_of_EA( e, Number<1>() );
            for( int ns = 0; ns < e->nb_children_virtual( 1 ); ++ns ) {
                // through the surface ?
                if ( fully_cut( ls[ ns ], cut )  ) {
                    for( SimpleConstIterator<typename TM::EA *> iter = m.sub_mesh( Number<1>() ).get_parents_of_EA( ls[ ns ] ); iter; ++iter )
                        seen_elems[ (*iter)->absolute_number ] = true;
                    continue;
                }
                for( SimpleConstIterator<typename TM::EA *> iter = m.sub_mesh( Number<1>() ).get_parents_of_EA( ls[ ns ] ); iter; ++iter ) {
                    if ( seen_elems[ (*iter)->absolute_number ] )
                        continue;
                    seen_elems[ (*iter)->absolute_number ] = true;

                    if ( elem_has_a_cut_not_in_border( *iter, cut, bor ) )
                        front << *iter;
                }
            }
        }
    }
}

template<class TM>
void crack_cut( TM &m, double lim = 0.5, typename TM::Pvec dir = typename TM::Pvec( 1 ) ) {
    MyRefinement<typename TM::Pvec> mr;

    // rafinement
    for( int var = 0; ; ++var ) {
        // moyenne de epsilon
        Vec<double> vg, sg; vg.resize( m.node_list.size(), 0 ); sg.resize( m.node_list.size(), 0 );
        apply( m.elem_list, ProjGrad(), vg, sg );
        vg /= sg;

        for( int o = 0; o < m.node_list.size(); ++o )
            m.node_list[ o ].is_on_skin = vg[ o ];

        mr.allowed_to_move.resize( m.node_list.size(), var == 0 );
        mr.lim = lim * max( vg );
        mr.dir = dir;
        mr.vg = &vg;

        if ( not refinement( m, mr ) )
            break;
    }

    // separation des noeuds
    // display_mesh( m );
    cut_mesh( m, mr.cuts );
    // display_mesh( m );
}

}

#endif // CRACK_CUT_H
