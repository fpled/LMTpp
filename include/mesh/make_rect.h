#ifndef LMT_mesh_make_rect_HEADER
#define LMT_mesh_make_rect_HEADER

#include "../util/rectilinear_iterator.h"
#include "../containers/mat.h"

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
struct Wedge;
struct Wedge_15;
struct Hexa;
struct Hexa_20;

template<class TM>
struct BestialNodeAdder {
    typedef typename TM::TNode TN;
    typedef typename TM::Pvec Pvec;
    typedef typename TM::Tpos T;
    TN *get_node( const Pvec &pos ) {
        for(unsigned i=0;i<m->node_list.size();++i)
            if ( length( m->node_list[i].pos - pos ) < prec )
                return &m->node_list[i];
        return m->add_node( pos );
    }
    TM *m;
    T prec;
};

}

/*!
 * Bar
 * @param m 
 * @param t 
 * @param extent 
 * @param nb_x 
 * @param nb_y 
 */

/*!
\generic_comment make_rect

    Cette fonction permet de générer un maillage de forme pavé (oblique) défini par les coordonnées cartésiennes X0 et X1 de sommets opposés, le type d'élément (Hexa, Quad_8, etc...) et le nombre de points par dimension.
    Voici un exemple de code en dimension 3 générique :
    \code C/C++
        #include "mesh/make_rect.h"
        using namespace LMT;
        int main(int argc,char* argv[]) {
            typedef Mesh<MeshCarac<3,double> > TM;
            typedef TM::Pvec Pvec;
        
            TM m;
            make_rect( m, Hexa(), Pvec( 0., 0., 0. ), Pvec( 2., 1., 1. ), Pvec( 100, 50, 50 ) );

            return 0;
        }
    \friend samir.amrouche@lmt.ens-cachan.fr
    \friend hugo.leclerc@lmt.ens-cachan.fr
    \keyword Maillage/Elément/Opération
*/

template<class TM>
void make_rect(TM &m,const Bar &t,typename TM::Pvec X0_,typename TM::Pvec X1_,typename TM::Pvec nb_points_) {
    typedef typename TM::Pvec Pvec;
    typedef typename TM::Tpos Tpos;
    Pvec X0 = min(X0_,X1_);
    Pvec X1 = max(X0_,X1_);
    Vec<unsigned,TM::dim> nb_points(nb_points_); 

    unsigned nb_x = (unsigned)nb_points[0], beg = m.node_list.size();
    Pvec incr = (X1-X0)/(nb_points-1);
    for(Rectilinear_iterator<Tpos,TM::dim> iter(X0,X1+(incr+(incr==0))*0.5,incr+(incr==0));iter;++iter)
        m.add_node( iter.pos );
    for(unsigned i=0;i<nb_x-1;++i) {
        m.add_element( Bar(), DefaultBehavior(), beg+i, beg+i+1 );
    }
}

/*!
 * Bar_3

 */
template<class TM>
void make_rect(TM &m,const Bar_3 &t,typename TM::Pvec X0,typename TM::Pvec X1,typename TM::Pvec nb_points_) {
    typedef typename TM::Pvec Pvec;
    typedef typename TM::Tpos Tpos;
    unsigned nb_points = 2 * unsigned( nb_points_[ 0 ] ) - 1;

    for( unsigned i=0; i<nb_points; ++i )
        m.add_node( X0 + i * ( X1 - X0 ) / ( nb_points - 1.0 ) );
    for( unsigned i=0; i<nb_points-2; i += 2 )
        m.add_element( Bar_3(), DefaultBehavior(), i, i+2, i+1 );
}

/*!
 * Triangle

 */
template<class TM>
void make_rect(TM &m,const Triangle &t,typename TM::Pvec X0_,typename TM::Pvec X1_,Vec<unsigned,2> nb_points_) {
    typedef typename TM::Pvec Pvec;
    typedef typename TM::Tpos Tpos;
    
    Pvec X0 = min(X0_,X1_);
    Pvec X1 = max(X0_,X1_);
    Vec<unsigned,TM::dim> nb_points(nb_points_); 

    unsigned nb_x = (unsigned)nb_points[0], nb_y = (unsigned)nb_points[1], beg = m.node_list.size();
    Pvec P( 0.0 );
    for(P[1]=0;P[1]<nb_y;++P[1])
        for(P[0]=0;P[0]<nb_x;++P[0])
            m.add_node( X0 + (X1-X0) / (nb_points-1) * P );
    //
    for(unsigned i=0;i<nb_y-1;++i) {
        for(unsigned j=0;j<nb_x-1;++j) {
            /**
             1 3
             0 2
             */
            unsigned pts[] = { i*nb_x+j, i*nb_x+j+1, i*nb_x+j+nb_x, i*nb_x+j+1+nb_x, };
            m.add_element( Triangle(), DefaultBehavior(), beg+pts[1], beg+pts[2], beg+pts[0] );
            m.add_element( Triangle(), DefaultBehavior(), beg+pts[1], beg+pts[3], beg+pts[2] );
        }
    }
}

/*!
 * Triangle_6

 */
template<class TM>
void make_rect(TM &m,const Triangle_6 &t,typename TM::Pvec X0_,typename TM::Pvec X1_,typename TM::Pvec nb_points_) {
    typedef typename TM::Pvec Pvec;
    typedef typename TM::Tpos Tpos;
    Pvec X0 = min(X0_,X1_);
    Pvec X1 = max(X0_,X1_);
    Vec<unsigned,TM::dim> nb_points(nb_points_);

    unsigned nb_x = (unsigned)nb_points[0], nb_y = (unsigned)nb_points[1], beg = m.node_list.size();
    Pvec incr = (X1-X0)/(nb_points-1)*.5;
    for(Rectilinear_iterator<Tpos,TM::dim> iter(X0,X1+(incr+(incr==0))*0.5,incr+(incr==0));iter;++iter)
        m.add_node( iter.pos );

    for(unsigned i=0;i<nb_y-1;++i) {
        for(unsigned j=0;j<nb_x-1;++j) {
            /**
            2 5 8
            1 4 7
            0 3 6
            */
            unsigned pts[] = {
                                 2*i*(2*nb_x-1)+2*j, 2*i*(2*nb_x-1)+2*j+1, 2*i*(2*nb_x-1)+2*j+2,
                                 2*i*(2*nb_x-1)+2*j+2*nb_x-1, 2*i*(2*nb_x-1)+2*j+1+2*nb_x-1, 2*i*(2*nb_x-1)+2*j+2+2*nb_x-1,
                                 2*i*(2*nb_x-1)+2*j+4*nb_x-2, 2*i*(2*nb_x-1)+2*j+1+4*nb_x-2, 2*i*(2*nb_x-1)+2*j+2+4*nb_x-2,
                             };
             m.add_element( Triangle_6(), DefaultBehavior(), beg+pts[0], beg+pts[2], beg+pts[6], beg+pts[1], beg+pts[4], beg+pts[3] );
             m.add_element( Triangle_6(), DefaultBehavior(), beg+pts[2], beg+pts[8], beg+pts[6], beg+pts[5], beg+pts[7], beg+pts[4] );
        }
    }
}

/*!
 * Quad

 */
template<class TM>
void make_rect(TM &m,const Quad &t,typename TM::Pvec X0_,typename TM::Pvec X1_,typename TM::Pvec nb_points_) {
    typedef typename TM::Pvec Pvec;
    typedef typename TM::Tpos Tpos;
    Pvec X0 = min(X0_,X1_);
    Pvec X1 = max(X0_,X1_);
    Vec<unsigned,TM::dim> nb_points(nb_points_); 

    unsigned nb_x = (unsigned)nb_points[0], nb_y = (unsigned)nb_points[1], beg = m.node_list.size();
    Pvec incr = (X1-X0)/(nb_points-1);
    for(Rectilinear_iterator<Tpos,TM::dim> iter(X0,X1+(incr+(incr==0))*0.5,incr+(incr==0));iter;++iter)
        m.add_node( iter.pos );

    if ( incr[0] * incr[1] > 0 ) {
        for(unsigned i=0;i<nb_y-1;++i)
            for(unsigned j=0;j<nb_x-1;++j)
                m.add_element( Quad(), DefaultBehavior(), beg+i*nb_x+j, beg+i*nb_x+j+1, beg+i*nb_x+j+1+nb_x, beg+i*nb_x+j+nb_x );
    }
    else {
        for(unsigned i=0;i<nb_y-1;++i)
            for(unsigned j=0;j<nb_x-1;++j)
                m.add_element( Quad(), DefaultBehavior(), beg+i*nb_x+j+nb_x, beg+i*nb_x+j+1+nb_x, beg+i*nb_x+j+1, beg+i*nb_x+j );
    }
}

/*!
 * Quad_8

 */
template<class TM>
void make_rect(TM &m,const Quad_8 &t,typename TM::Pvec X0_,typename TM::Pvec X1_,typename TM::Pvec nb_points_) {
    typedef typename TM::Pvec Pvec;
    typedef typename TM::Tpos Tpos;
    Vec<unsigned,TM::dim> nb_points(nb_points_);
    Pvec X0 = min(X0_,X1_);
    Pvec X1 = max(X0_,X1_);

    // nodes
    unsigned nb_x = (unsigned)nb_points[0], nb_y = (unsigned)nb_points[1];
    unsigned nnx = 2*nb_x-1;
    for(unsigned i=0;i<nnx;++i) // first line
        m.add_node( X0 + Pvec(i*(X1[0]-X0[0])/(nnx-1),0.0) );
    for(unsigned j=0;j<nb_y-1;++j) {
        for(unsigned i=0;i<nb_x;++i) //
            m.add_node( X0 + Pvec(i*(X1[0]-X0[0])/(nb_x-1),(j+0.5)*(X1[1]-X0[1])/(nb_y-1)) );
        for(unsigned i=0;i<nnx;++i) //
            m.add_node( X0 + Pvec(i*(X1[0]-X0[0])/(nnx-1),(j+1)*(X1[1]-X0[1])/(nb_y-1)) );
    }

    // elements
    unsigned inc_x = nnx + nb_x;
    for(unsigned j=0;j<nb_y-1;++j) {
        for(unsigned i=0;i<nb_x-1;++i) {
            m.add_element( Quad_8(), DefaultBehavior(),
                           (2*i  ) + (j  )*inc_x,
                           (2*i+2) + (j  )*inc_x,
                           (2*i+2) + (j+1)*inc_x,
                           (2*i  ) + (j+1)*inc_x,
                           (2*i+1) + (j  )*inc_x,
                           (  i+1) + nnx+j*inc_x,
                           (2*i+1) + (j+1)*inc_x,
                           (  i  ) + nnx+j*inc_x
                         );
        }
    }
}

/*!
 * Quad_9

 */
template<class TM>
void make_rect(TM &m,const Quad_9 &t,typename TM::Pvec X0_,typename TM::Pvec X1_,typename TM::Pvec nb_points_) {
    typedef typename TM::Pvec Pvec;
    typedef typename TM::Tpos Tpos;
    Vec<unsigned,TM::dim> nb_points(nb_points_);

    Pvec X0 = min(X0_,X1_);
    Pvec X1 = max(X0_,X1_);
    // nodes
    unsigned nb_x = (unsigned)nb_points[0] * 2 - 1, nb_y = (unsigned)nb_points[1] * 2 - 1;
    for(unsigned j=0;j<nb_y;++j)
        for(unsigned i=0;i<nb_x;++i)
            m.add_node( X0 + Pvec( i * (X1[0]-X0[0]) / (nb_x-1), j * (X1[1]-X0[1]) / (nb_y-1) ) );

    // #  3    6    2
    // #  x----x----x
    // #  |         |
    // # 7|    x8   x5
    // #  |         |
    // #  x----x----x
    // #  0    4    1
    // elements
    for(unsigned j=0;j<nb_y-2;j+=2)
        for(unsigned i=0;i<nb_x-2;i+=2)
            m.add_element( Quad_9(), DefaultBehavior(),
                           (i  ) + (j  )*nb_x,
                           (i+2) + (j  )*nb_x,
                           (i+2) + (j+2)*nb_x,
                           (i  ) + (j+2)*nb_x,

                           (i+1) + (j  )*nb_x,
                           (i+2) + (j+1)*nb_x,
                           (i+1) + (j+2)*nb_x,
                           (i  ) + (j+1)*nb_x,

                           (i+1) + (j+1)*nb_x
                         );
}

/*!
 * Quad_6

 */
template<class TM> void make_rect(TM &m,const Quad_6 &t,typename TM::Pvec X0_,typename TM::Pvec X1_,typename TM::Pvec nb_points_) {
    typedef typename TM::Pvec Pvec;
    typedef typename TM::Tpos Tpos;
    Vec<unsigned,TM::dim> nb_points(nb_points_);

    Pvec X0 = min(X0_,X1_);
    Pvec X1 = max(X0_,X1_);

    unsigned nb_x = (unsigned)nb_points[0], nb_y = (unsigned)nb_points[1];
    unsigned inc_x = 2*nb_x - 1, beg = m.node_list.size();

    Pvec incr = X1 - X0;
    incr[0] /= 2 * ( nb_points[0] - 1 );
    incr[1] /= nb_points[1] - 1;
    for(Rectilinear_iterator<Tpos,TM::dim> iter(X0,X1+(incr+(incr==0))*0.25,incr+(incr==0));iter;++iter)
        m.add_node( iter.pos );

    if ( incr[0] * incr[1] > 0 ) {
        for(unsigned i=0;i<nb_y-1;++i)
            for(unsigned j=0;j<nb_x-1;++j)
                m.add_element( Quad_6(), DefaultBehavior(),
                               beg + (i  )*inc_x + (2*j  ),
                               beg + (i  )*inc_x + (2*j+2),
                               beg + (i+1)*inc_x + (2*j+2),
                               beg + (i+1)*inc_x + (2*j  ),
                               beg + (i  )*inc_x + (2*j+1),
                               beg + (i+1)*inc_x + (2*j+1)
                             );
    }
    else {
        for(unsigned i=0;i<nb_y-1;++i)
            for(unsigned j=0;j<nb_x-1;++j)
                m.add_element( Quad_6(), DefaultBehavior(),
                               beg + (i+1)*inc_x + (2*j+1),
                               beg + (i  )*inc_x + (2*j+1),
                               beg + (i+1)*inc_x + (2*j  ),
                               beg + (i+1)*inc_x + (2*j+2),
                               beg + (i  )*inc_x + (2*j+2),
                               beg + (i  )*inc_x + (2*j  )
                             );
    }
}

/*!
 * Quad_42

 */
template<class TM> void make_rect(TM &m,const Quad_42 &t,typename TM::Pvec X0_,typename TM::Pvec X1_,typename TM::Pvec nb_points_) {
    typedef typename TM::Pvec Pvec;
    typedef typename TM::Tpos Tpos;
    Vec<unsigned,TM::dim> nb_points(nb_points_);
    Pvec X0 = min(X0_,X1_);
    Pvec X1 = max(X0_,X1_);

    unsigned nb_x = (unsigned)nb_points[0], nb_y = (unsigned)nb_points[1];
    unsigned inc_x = 3*nb_x - 2, beg = m.node_list.size();

    Pvec incr = X1 - X0;
    incr[0] /= 3 * ( nb_points[0] - 1 );
    incr[1] /= nb_points[1] - 1;
    for(Rectilinear_iterator<Tpos,TM::dim> iter(X0,X1+(incr+(incr==0))*0.05,incr+(incr==0));iter;++iter)
        m.add_node( iter.pos );

    if ( incr[0] * incr[1] > 0 ) {
        for(unsigned i=0;i<nb_y-1;++i)
            for(unsigned j=0;j<nb_x-1;++j)
                m.add_element( Quad_42(), DefaultBehavior(),
                               beg + (i  )*inc_x + (3*j  ),
                               beg + (i  )*inc_x + (3*j+3),
                               beg + (i+1)*inc_x + (3*j+3),
                               beg + (i+1)*inc_x + (3*j  ),

                               beg + (i  )*inc_x + (3*j+1),
                               beg + (i  )*inc_x + (3*j+2),
                               beg + (i+1)*inc_x + (3*j+2),
                               beg + (i+1)*inc_x + (3*j+1)
                             );
    }
    else {
        for(unsigned i=0;i<nb_y-1;++i)
            for(unsigned j=0;j<nb_x-1;++j)
                m.add_element( Quad_42(), DefaultBehavior(),
                               beg + (i+1)*inc_x + (3*j+1),
                               beg + (i  )*inc_x + (3*j+1),
                               beg + (i+1)*inc_x + (3*j  ),
                               beg + (i+1)*inc_x + (3*j+2),

                               beg + (i+1)*inc_x + (3*j+1),
                               beg + (i+1)*inc_x + (3*j+2),
                               beg + (i  )*inc_x + (3*j+2),
                               beg + (i  )*inc_x + (3*j+1)
                             );
    }
}

/*!
 * Tetra

 */
template<class TM>
void make_rect(TM &m,const Tetra &t,typename TM::Pvec X0_,typename TM::Pvec X1_,typename TM::Pvec nb_points_) {
    typedef typename TM::Pvec Pvec;
    typedef typename TM::Tpos Tpos;
    Pvec X0 = min(X0_,X1_);
    Pvec X1 = max(X0_,X1_);
    Vec<unsigned,TM::dim> nb_points(nb_points_); 

    Pvec incr = (X1-X0)/(nb_points-1);
    unsigned nb_x = (unsigned)nb_points[0], nb_y = (unsigned)nb_points[1], nb_z = (unsigned)nb_points[2], beg = m.node_list.size();
    for(Rectilinear_iterator<Tpos,3> iter(X0,X1+(incr+(incr==0))*0.5,incr+(incr==0));iter;++iter)
        m.add_node( iter.pos );
    for(unsigned i=0;i<nb_z-1;++i) {
        for(unsigned j=0;j<nb_y-1;++j) {
            for(unsigned k=0;k<nb_x-1;++k) {
//                 int lst[] = {
//                                 3,6,5,7,
//                                 0,4,5,6,
//                                 0,5,3,6,
//                                 0,1,3,5,
//                                 0,3,2,6,
//                             };
                int lst[] = {
                    0,1,2,4,
                    1,2,3,5,
                    1,2,4,5,
                    2,4,5,6,
                    2,3,5,6,
                    3,5,6,7,
                };
                for(unsigned n=0;n<6;++n) {
                    typename TM::TNode *vn[4];
                    for(unsigned p=0;p<4;++p)
                        vn[p] = &m.node_list[ beg+(i+(lst[n*4+p]&4)/4)*nb_x*nb_y+(j+(lst[n*4+p]&2)/2)*nb_x+(k+(lst[n*4+p]&1)/1) ];
                    permutation_if_jac_neg( Tetra(), vn );
                    m.add_element( Tetra(), DefaultBehavior(), vn );
                }
            }
        }
    }
    
}

/*!
 * Tetra_10
 * \verbatim
        .                            3
        .                           /|\
        .                          / |9\
        .                        7/  |  \8
        .                        /  /2\  \
        .                       /  /6 5\  \
        .                     0/--- 4------\1
 */
template<class TM>
void make_rect(TM &m,const Tetra_10 &t,typename TM::Pvec X0_,typename TM::Pvec X1_,typename TM::Pvec nb_points_ ) {
    typedef typename TM::Pvec Pvec;
    typedef typename TM::Tpos Tpos;

    Pvec X0 = min(X0_,X1_);
    Pvec X1 = max(X0_,X1_);
    Vec< Pvec, 10 > rela;
    Vec< Pvec,  3 > basis;
    Mat< double, Gen<3,3> > chgt;
    Vec< typename TM::TNode*, 10 > vn;
    Pvec O, tr, incr;
    Tpos dist_ref = distance( X0, X1 );

    if ( dist_ref / ( 1. + length( X0 ) + length( X1 ) ) < 16 * std::numeric_limits<Tpos>::epsilon() ) {
        std::cerr << "error make_rect() : vacuum space " << std::endl;
        return;
    }

    BestialNodeAdder<TM> ban; ban.m = &m; ban.prec = 16 * dist_ref * std::numeric_limits<Tpos>::epsilon();

    Vec<unsigned, 3 > nb_elem( nb_points_ ); nb_elem -= 1;

    incr = ( X1 - X0 ) / nb_elem;

    rela[ 0 ] = Pvec(0.0,0.0,0.0);
    rela[ 1 ] = Pvec(1.0,0.0,0.0);
    rela[ 2 ] = Pvec(0.0,1.0,0.0);
    rela[ 3 ] = Pvec(0.0,0.0,1.0);

    rela[ 4 ] = Pvec(0.5,0.0,0.0);
    rela[ 5 ] = Pvec(0.5,0.5,0.0);
    rela[ 6 ] = Pvec(0.0,0.5,0.0);
    rela[ 7 ] = Pvec(0.0,0.0,0.5);
    rela[ 8 ] = Pvec(0.5,0.0,0.5);
    rela[ 9 ] = Pvec(0.0,0.5,0.5);

    for( unsigned i = 0; i < nb_elem[ 2 ]; ++i )
        for( unsigned j = 0; j < nb_elem[ 1 ]; ++j )
            for( unsigned k = 0; k < nb_elem[ 0 ]; ++k ) {
                O = X0 + Pvec( k * incr[ 0 ], j * incr[ 1 ], i * incr[ 2 ] );
                /// Tetra 0
                basis[ 0 ] = Pvec(1.0,0.0,0.0);
                basis[ 1 ] = Pvec(0.0,1.0,0.0);
                basis[ 2 ] = Pvec(0.0,0.0,1.0);
                chgt.col( 0 ) = basis[ 0 ]; chgt.col( 1 ) = basis[ 1 ]; chgt.col( 2 ) = basis[ 2 ];
                for( unsigned i = 0; i < 10; ++i )
                    vn[ i ] = ban.get_node( O + incr * ( chgt * rela[ i ] ) );
                permutation_if_jac_neg( Tetra_10(), vn.ptr() );
                m.add_element( Tetra_10(), DefaultBehavior(), vn.ptr() );
                /// Tetra 1
                tr = Pvec( incr[ 0 ], incr[ 1 ], 0.0 );
                basis[ 0 ] = Pvec(-1.0,0.0,0.0);
                basis[ 1 ] = Pvec(0.0,-1.0,0.0);
                basis[ 2 ] = Pvec(0.0,-1.0,1.0);
                chgt.col( 0 ) = basis[ 0 ]; chgt.col( 1 ) = basis[ 1 ]; chgt.col( 2 ) = basis[ 2 ];
                for( unsigned i = 0; i < 10; ++i )
                    vn[ i ] = ban.get_node( O + tr + incr * ( chgt * rela[ i ] ) );
                permutation_if_jac_neg( Tetra_10(), vn.ptr() );
                m.add_element( Tetra_10(), DefaultBehavior(), vn.ptr() );
                /// Tetra 2
                tr = Pvec( incr[ 0 ], 0.0, 0.0 );
                basis[ 0 ] = Pvec(-1.0,0.0,1.0);
                basis[ 1 ] = Pvec(-1.0,1.0,0.0);
                basis[ 2 ] = Pvec(0.0,0.0,1.0);
                chgt.col( 0 ) = basis[ 0 ]; chgt.col( 1 ) = basis[ 1 ]; chgt.col( 2 ) = basis[ 2 ];
                for( unsigned i = 0; i < 10; ++i )
                    vn[ i ] = ban.get_node( O + tr + incr * ( chgt * rela[ i ] ) );
                permutation_if_jac_neg( Tetra_10(), vn.ptr() );
                m.add_element( Tetra_10(), DefaultBehavior(), vn.ptr() );
                /// Tetra 3
                tr = Pvec( incr[ 0 ], 0.0, incr[ 2 ] );
                basis[ 0 ] = Pvec(-1.0,0.0,0.0);
                basis[ 1 ] = Pvec(-1.0,1.0,0.0);
                basis[ 2 ] = Pvec(-1.0,1.0,-1.0);
                chgt.col( 0 ) = basis[ 0 ]; chgt.col( 1 ) = basis[ 1 ]; chgt.col( 2 ) = basis[ 2 ];
                for( unsigned i = 0; i < 10; ++i )
                    vn[ i ] = ban.get_node( O + tr + incr * ( chgt * rela[ i ] ) );
                permutation_if_jac_neg( Tetra_10(), vn.ptr() );
                m.add_element( Tetra_10(), DefaultBehavior(), vn.ptr() );
                /// Tetra 4
                tr = Pvec( incr[ 0 ], incr[ 1 ], 0.0 );
                basis[ 0 ] = Pvec(-1.0,0.0,0.0);
                basis[ 1 ] = Pvec(-1.0,0.0,1.0);
                basis[ 2 ] = Pvec(0.0,-1.0,1.0);
                chgt.col( 0 ) = basis[ 0 ]; chgt.col( 1 ) = basis[ 1 ]; chgt.col( 2 ) = basis[ 2 ];
                for( unsigned i = 0; i < 10; ++i )
                    vn[ i ] = ban.get_node( O + tr + incr * ( chgt * rela[ i ] ) );
                permutation_if_jac_neg( Tetra_10(), vn.ptr() );
                m.add_element( Tetra_10(), DefaultBehavior(), vn.ptr() );
                /// Tetra 5
                tr = Pvec( incr[ 0 ], incr[ 1 ], incr[ 2 ] );
                basis[ 0 ] = Pvec(-1.0,0.0,0.0);
                basis[ 1 ] = Pvec(0.0,-1.0,0.0);
                basis[ 2 ] = Pvec(0.0,0.0,-1.0);
                chgt.col( 0 ) = basis[ 0 ]; chgt.col( 1 ) = basis[ 1 ]; chgt.col( 2 ) = basis[ 2 ];
                for( unsigned i = 0; i < 10; ++i )
                    vn[ i ] = ban.get_node( O + tr + incr * ( chgt * rela[ i ] ) );
                permutation_if_jac_neg( Tetra_10(), vn.ptr() );
                m.add_element( Tetra_10(), DefaultBehavior(), vn.ptr() );
            }
}

/*!
 * Wedge

 */
template<class TM> void make_rect(TM &m,const Wedge &t,typename TM::Pvec X0_,typename TM::Pvec X1_,typename TM::Pvec nb_points_) {
    typedef typename TM::Pvec Pvec;
    typedef typename TM::Tpos Tpos;
    Vec<unsigned,TM::dim> nb_points(nb_points_); 
    
    Pvec X0 = min(X0_,X1_);
    Pvec X1 = max(X0_,X1_);

    unsigned nb_x = (unsigned)nb_points[0], nb_y = (unsigned)nb_points[1], nb_z = (unsigned)nb_points[2], beg = m.node_list.size();
    Pvec incr = (X1-X0)/(nb_points-1);
    for(Rectilinear_iterator<Tpos,3> iter(X0,X1+(incr+(incr==0))*0.5,incr+(incr==0));iter;++iter)
        m.add_node( iter.pos );
    for(unsigned i=0;i<nb_z-1;++i) {
        for(unsigned j=0;j<nb_y-1;++j) {
            for(unsigned k=0;k<nb_x-1;++k) {
                unsigned pts[] = { i*nb_x*nb_y+j*nb_x+k,
                    i*nb_x*nb_y+j*nb_x+k+1,
                    i*nb_x*nb_y+j*nb_x+k+1+nb_x,
                    i*nb_x*nb_y+j*nb_x+k+nb_x,
                    (i+1)*nb_x*nb_y+j*nb_x+k,
                    (i+1)*nb_x*nb_y+j*nb_x+k+1,
                    (i+1)*nb_x*nb_y+j*nb_x+k+1+nb_x,
                    (i+1)*nb_x*nb_y+j*nb_x+k+nb_x,
                };
                m.add_element( Wedge(), DefaultBehavior(), beg+pts[0], beg+pts[2], beg+pts[3], beg+pts[4], beg+pts[6], beg+pts[7] );
                m.add_element( Wedge(), DefaultBehavior(), beg+pts[0], beg+pts[1], beg+pts[2], beg+pts[4], beg+pts[5], beg+pts[6] );
            }
        }
    }
}

/*!
 * Wedge_15
 * \verbatim
    .            5
    .          /|\
    .        11 | 10
    .        /  14  \
    .        3___9___4
    .        |   |   |
    .        |   2   |
    .       12  / \  13
    .        | 8   7 |
    .        |/     \|
    .        0___6__ 1
*/
template<class TM>
void make_rect(TM &m,const Wedge_15 &t,typename TM::Pvec X0_,typename TM::Pvec X1_,typename TM::Pvec nb_elements) {
    typedef typename TM::Pvec Pvec;
    typedef typename TM::Tpos Tpos;

    Pvec X0 = min(X0_,X1_);
    Pvec X1 = max(X0_,X1_);

    BestialNodeAdder<TM> ban; ban.m = &m; ban.prec = min( (X1-X0)/nb_elements ) * 1e-6;

    Pvec step = (X1-X0) / nb_elements;
    for(Rectilinear_iterator<Tpos,TM::dim> iter( X0, X1 - 0.1 * step, step ); iter; ++iter ) {
        typename TM::TNode *n[] = {
            ban.get_node( iter.pos + step * Pvec(0.0,0.0,0.0) ),
            ban.get_node( iter.pos + step * Pvec(1.0,0.0,0.0) ),
            ban.get_node( iter.pos + step * Pvec(1.0,1.0,0.0) ),
            ban.get_node( iter.pos + step * Pvec(0.0,1.0,0.0) ),
            ban.get_node( iter.pos + step * Pvec(0.0,0.0,1.0) ),
            ban.get_node( iter.pos + step * Pvec(1.0,0.0,1.0) ),
            ban.get_node( iter.pos + step * Pvec(1.0,1.0,1.0) ),
            ban.get_node( iter.pos + step * Pvec(0.0,1.0,1.0) ),

            ban.get_node( iter.pos + step * Pvec(0.5,0.0,0.0) ),
            ban.get_node( iter.pos + step * Pvec(1.0,0.5,0.0) ),
            ban.get_node( iter.pos + step * Pvec(0.5,1.0,0.0) ),
            ban.get_node( iter.pos + step * Pvec(0.0,0.5,0.0) ),
            ban.get_node( iter.pos + step * Pvec(0.5,0.0,1.0) ),
            ban.get_node( iter.pos + step * Pvec(1.0,0.5,1.0) ),
            ban.get_node( iter.pos + step * Pvec(0.5,1.0,1.0) ),
            ban.get_node( iter.pos + step * Pvec(0.0,0.5,1.0) ),

            ban.get_node( iter.pos + step * Pvec(0.0,0.0,0.5) ),
            ban.get_node( iter.pos + step * Pvec(1.0,0.0,0.5) ),
            ban.get_node( iter.pos + step * Pvec(1.0,1.0,0.5) ),
            ban.get_node( iter.pos + step * Pvec(0.0,1.0,0.5) ),

            ban.get_node( iter.pos + step * Pvec(0.5,0.5,0.0) ),
            ban.get_node( iter.pos + step * Pvec(0.5,0.5,1.0) ),
        };
        typename TM::TNode *n1[15] = { n[0],n[2],n[3],n[4],n[6],n[7],
                                         n[20],n[10],n[11],n[21],n[14],n[15],
                                         n[16],n[18],n[19] };
        m.add_element( Wedge_15(), DefaultBehavior(), n1 );

        typename TM::TNode *n2[15] = { n[0],n[1],n[2],n[4],n[5],n[6],
                                         n[8],n[9],n[20],n[12],n[13],n[21],
                                         n[16],n[17],n[18] };
        m.add_element( Wedge_15(), DefaultBehavior(), n2 );


    }
}

/*!
 * Hexa

 */
template<class TM>
void make_rect(TM &m,const Hexa &t,typename TM::Pvec X0_,typename TM::Pvec X1_,typename TM::Pvec nb_points_) {
    typedef typename TM::Pvec Pvec;
    typedef typename TM::Tpos Tpos;
    Vec<unsigned,TM::dim> nb_points(nb_points_);

    Pvec X0 = min(X0_,X1_);
    Pvec X1 = max(X0_,X1_);

    unsigned nb_x = (unsigned)nb_points[0], nb_y = (unsigned)nb_points[1], nb_z = (unsigned)nb_points[2], beg = m.node_list.size();
    Pvec incr = (X1-X0)/(nb_points-1);
    for(Rectilinear_iterator<Tpos,3> iter(X0,X1+(incr+(incr==0))*0.5,incr+(incr==0));iter;++iter)
        m.add_node( iter.pos );
    for(unsigned i=0;i<nb_z-1;++i)
        for(unsigned j=0;j<nb_y-1;++j)
            for(unsigned k=0;k<nb_x-1;++k)
                m.add_element( Hexa(), DefaultBehavior(),
                               beg+i*nb_x*nb_y+j*nb_x+k,
                               beg+i*nb_x*nb_y+j*nb_x+k+1,
                               beg+i*nb_x*nb_y+j*nb_x+k+1+nb_x,
                               beg+i*nb_x*nb_y+j*nb_x+k+nb_x,
                               beg+(i+1)*nb_x*nb_y+j*nb_x+k,
                               beg+(i+1)*nb_x*nb_y+j*nb_x+k+1,
                               beg+(i+1)*nb_x*nb_y+j*nb_x+k+1+nb_x,
                               beg+(i+1)*nb_x*nb_y+j*nb_x+k+nb_x
                             );
}

/*!
 * Hexa_20
 \verbatim
        .                        7 ---14----6
        .                       /|         /|
        .                    15  |       13 |
        .                    /  19      /  18
        .                    4---|-12---5   |
        .                    |   |      |   |
        .                    |   3---10-|-- 2
        .                    16 /      17  /
        .                    |11        | 9
        .                    |/         |/
        .                    0-----8----1
 
 */
template<class TM>
void make_rect(TM &m,const Hexa_20 &t,typename TM::Pvec X0_,typename TM::Pvec X1_,typename TM::Pvec nb_elements) {
    typedef typename TM::Pvec Pvec;
    typedef typename TM::Tpos Tpos;
    //     Vec<unsigned,TM::dim> nb_points(nb_points_); 
    
    Pvec X0 = min(X0_,X1_);
    Pvec X1 = max(X0_,X1_);
    
    BestialNodeAdder<TM> ban; ban.m = &m; ban.prec = min( (X1-X0)/nb_elements ) * 1e-6;
    
    Pvec step = (X1-X0) / nb_elements;
    for(Rectilinear_iterator<Tpos,TM::dim> iter( X0, X1 - 0.1 * step, step ); iter; ++iter ) {
        typename TM::TNode *n[] = {
            ban.get_node( iter.pos + step * Pvec(0.0,0.0,0.0) ),
            ban.get_node( iter.pos + step * Pvec(1.0,0.0,0.0) ),
            ban.get_node( iter.pos + step * Pvec(1.0,1.0,0.0) ),
            ban.get_node( iter.pos + step * Pvec(0.0,1.0,0.0) ),
            ban.get_node( iter.pos + step * Pvec(0.0,0.0,1.0) ),
            ban.get_node( iter.pos + step * Pvec(1.0,0.0,1.0) ),
            ban.get_node( iter.pos + step * Pvec(1.0,1.0,1.0) ),
            ban.get_node( iter.pos + step * Pvec(0.0,1.0,1.0) ),
                       
            ban.get_node( iter.pos + step * Pvec(0.5,0.0,0.0) ),
            ban.get_node( iter.pos + step * Pvec(1.0,0.5,0.0) ),
            ban.get_node( iter.pos + step * Pvec(0.5,1.0,0.0) ),
            ban.get_node( iter.pos + step * Pvec(0.0,0.5,0.0) ),
            ban.get_node( iter.pos + step * Pvec(0.5,0.0,1.0) ),
            ban.get_node( iter.pos + step * Pvec(1.0,0.5,1.0) ),
            ban.get_node( iter.pos + step * Pvec(0.5,1.0,1.0) ),
            ban.get_node( iter.pos + step * Pvec(0.0,0.5,1.0) ),
            
            ban.get_node( iter.pos + step * Pvec(0.0,0.0,0.5) ),
            ban.get_node( iter.pos + step * Pvec(1.0,0.0,0.5) ),
            ban.get_node( iter.pos + step * Pvec(1.0,1.0,0.5) ),
            ban.get_node( iter.pos + step * Pvec(0.0,1.0,0.5) )
        };
        m.add_element( Hexa_20(), DefaultBehavior(), n );
    }
}

#endif // LMT_mesh_make_rect_HEADER
