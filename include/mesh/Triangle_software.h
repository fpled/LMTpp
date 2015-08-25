#ifndef TRIANGLE_SOFTWARE_H
#define TRIANGLE_SOFTWARE_H

#ifdef METIL_COMP_DIRECTIVE
#pragma inc_path /usr/local/triangle/
#pragma lib_path /usr/local/triangle/
#pragma lnk_flag /usr/local/triangle/triangle.o
#endif /// METIL_COMP_DIRECTIVE

#include <list>
#include <limits>
#include <map>
#include "sstream"
extern "C" {
    #define REAL double
    #define VOID void
    #include "/usr/local/triangle/triangle.h"
}

using namespace LMT;
 
/// debug
void display( const triangulateio &tri ) {

    PRINT( tri.pointlist                  );
    PRINT( tri.pointattributelist         );
    PRINT( tri.pointmarkerlist            );
    PRINT( tri.numberofpoints             );
    PRINT( tri.numberofpointattributes    );
    PRINT( tri.trianglelist               );
    PRINT( tri.triangleattributelist      );
    PRINT( tri.trianglearealist           );
    PRINT( tri.neighborlist               );
    PRINT( tri.numberoftriangles          );
    PRINT( tri.numberofcorners            );
    PRINT( tri.numberoftriangleattributes );      
    PRINT( tri.segmentlist                );
    PRINT( tri.segmentmarkerlist          );
    PRINT( tri.numberofsegments           );
    PRINT( tri.holelist                   );
    PRINT( tri.numberofholes              );
    PRINT( tri.regionlist                 );
    PRINT( tri.numberofregions            );
}

template<class T,int s>
inline typename FloatType<typename TypeReduction<Multiplies,Vec<T,s> >::T>::T relative_distance(const Vec<T,s> &c, const Vec<T,s> &d) {
    typedef typename FloatType<typename TypeReduction<Multiplies,Vec<T,s> >::T>::T TR;
    Vec<T,s> tmp = c - d;
    TR lc = length( c );
    TR ld = length( d );
    return length( tmp ) / ( lc + ld + ( lc == 0. ) * ( ld == 0. ) );
}

/*!
    Cette classe encapsule certaines fonctionnalités du logiciel [[http://www.cs.cmu.edu/~quake/triangle.h|Triange]] .
    Elle permet de générer un maillage de triangles à partir de points et de segments dans le plan. Certains de ces segments servant à définir la peau du maillage.
    On peut aussi introduire un maillage en plus de points et de segments grâce à la méthode append_mesh() qui prend en paramètre 
        * un maillage 2D ( de \a Quad ou de \a Triangle ).
        * un mode d'insertion 
            * All_mode : toutes les barres et tous les noeuds
            * Only_skin_mode : seulement la peau
            * Skin_and_node_mode : la peau et les noeuds à l'intérieur du maillage
             
    Pour la génération, on uilise la méthode get_triangulation()
        * Le premier paramètre sera le maillage résultat et 
        * le second sont les paramètres à passer au programme Triangle.
            Ce dernier a de nombreuses possibilités de paramètres:
            Les plus utiles sont :
                * q?? où ?? représente un angle minimal à respecter. Prendre un valeur comprise entre 23 et 33. Au delà de 33 le programme risque d'échouer.
                * a?? où ?? est un nombre réel représentant l'aire maximale des triangles.
          
        * le dernier paramètre ( facultatif ) est du type <strong> triunsuitable_criterium_t </strong> qui est un synonyme de <strong> int (*)( double *ori, double *dest, double *apex, double area, void *param ) </strong> . C'est donc un pointeur de fonction qui dont les paramètres <strong> ori </strong> , <strong> dest </strong> et <strong> apex </strong> sont les coordonnées des sommets d'un triangle, <strong> area </strong> son aire et <strong> param </strong> un pointeur générique sur un autre paramètre ( si nécessaire ).
            Cette fonction doit renvoyer 1 si le triangle passé en paramètre est trop grand et 0 sinon. Cette fonction permet par exemple de mailler plus finement une zone.
            
    Remarque : les paramètres du programme Triangle zpDQ sont mis par défaut.
    
    Voici un exemple de code. 
    On souhaite créer un maillage rectangulaire avec des triangles de plus en plus petits au fur et à mesure que l'on s'approche de l'axe des abscisses.
    Une idée de créer un maillage avec \a make_rect() puis de déplacer les points pour qu'ils se raprochent de l'axe des abscisses.
    Ensuite on ajoute deux points et un segment sur cet axe des x.
    Enfin on se sert d'une fonction <strong> densite </strong> qui permet de mailler plus finement au voisinage de zéro. 
    
    \code C/C++
        #include "mesh/meshcaracstd.h"
        #include "mesh/make_rect.h"
        #include "mesh/displayparaview.h"
        #include "mesh/read_msh_2.h"
        #include "mesh/Triangle_software.h"
        
        int densite( double *ori, double *dest, double *apex, double area, void* p ) {
        
            double center[ 2 ];
            
            center[ 0 ] = (1./3) * ( ori[ 0 ] + dest[ 0 ] + apex[ 0 ] );
            center[ 1 ] = (1./3) * ( ori[ 1 ] + dest[ 1 ] + apex[ 1 ] );
            
            double density = 0.0001 + 0.01 * sqrt( center[ 0 ] * center[ 0 ] + center[ 1 ] * center[ 1 ] );
            
            if ( area > density )
                return 1;
            else
                return 0;
        }
        
        int main( int argc, const char* argv[] ) {
        
            typedef Mesh< MeshCaracStd< 2, 2, 1, double, 1  > > TMS; /// Quad dans le plan
            
            typedef Mesh< MeshCaracStd< 2, 2, 0, double, 1  > > TM; /// Triangle dans le plan
            typedef TM::Pvec Pvec;
        
            TMS m_source;
            make_rect( m_source, Quad(), Pvec( -2.5, -1 ), Pvec( 2.5, 1 ), Pvec( 11, 9 ) );
            for( unsigned i = 0; i < m_source.node_list.size(); ++i ) {
                double y = pow( abs( m_source.node_list[ i ].pos[ 1 ] ), 2 );
                if ( m_source.node_list[ i ].pos[ 1 ] > 0 )
                    m_source.node_list[ i ].pos[ 1 ] = y;
                else
                    m_source.node_list[ i ].pos[ 1 ] = -y;
            }
            
            display_mesh( m_source );
            
            Triangle_software tri( m_source, Triangle_software::Skin_and_node_mode );
            
            int p1 = tri.append_point( -2.5, 0. );
            int p2 = tri.append_point(  2.5, 0. );
            tri.append_segment( p1, p2 );
            
            TM m;
            tri.get_triangulation( m, "q31", densite );
            
            display_mesh( m );
            
            return 0;
        }          
     

     Les utilisateurs de metil_comp compile ce code sans problème en tapant par exemple :
     \code 
        metil_comp -DANSI_DECLARATORS main.cpp
     
     Pour ceux qui utilisent scons, inspirez-vous de cet exemple :
     \code Python
        from LMT import *
        
        
        libs_Triangle_software = [
            '/usr/local/triangle/triangle.o'
        ]
        
        env = Environment(
            CPPPATH = [ '#LMT/include' , '/usr/local' ],
            LIBS = [ 'pthread' ],
            CPPFLAGS = cppflags( ['xml2-config'] ) + " -O3 -g -DANSI_DECLARATORS ",
            LINKFLAGS = linkflags( ['xml2-config'] ) ,
            AS = 'yasm -f Elf32 -g Dwarf2'
        )
        make_dep_py(env)
        
        env.BuildDir( 'build/LMT', 'LMT/include', duplicate=0 )
        libs = SConscript( 'LMT/include/SConscript', exports='env', build_dir='build/LMT' )
        
        env.Program( "main", ["main.cpp"] + libs + libs_Triangle_software, build_dir='build/LMT' ) 

*/
struct Triangle_software {
    typedef double T;
    typedef Vec<T, 2 > Pvec;

    typedef enum {
        All_mode = 0,
        Only_skin_mode = 1,
        Skin_and_node_mode = 2
    } Mode_append;

    template <class _Pvec >
    struct MultiLine {
        typedef _Pvec Pvec;
    
        MultiLine() {}
    
        /// TE doit être de type \a Bar
        template <class TE>
        MultiLine( const TE &e ) {
            for( unsigned i = 0; i < TE::nb_nodes; ++i )
                l.push_back( e.pos( i ) );
        }
    
        /*!
            Objectif :
                Ajouter la \a Bar e si elle peut se coller au début de la ligne brisée.
                
            Rem : on aurait pu (dû ) traiter ce cas dans le cas général append_if_start( MultiLine<Pvec> ) mais pour des raisons de performance, j'ai préféré écrire le code directement. 
        */ 
        template< class TE>
        bool append_if_start( const TE &e ) {
            if ( l.size() == 0 )
                return false;
                
            Pvec p = l[ 0 ];
            
            Pvec pe = e.pos( 0 );
                
            if ( relative_distance( p , pe ) < epsi ) {
                unsigned s = l.size();
                l.resize( l.size() + TE::nb_nodes - 1 );
                for( unsigned i = 0; i < s; ++i )
                    l[ l.size() - 1 - i ] = l[ s - 1 - i ];
                for( unsigned i = 1; i < TE::nb_nodes; ++i )
                    l[ TE::nb_nodes - 1 - i ] = e.pos( i );
                return true;
            }
            
            pe = e.pos( TE::nb_nodes - 1 );
            
            if ( relative_distance( p , pe ) < epsi ) {
                unsigned s = l.size();
                l.resize( l.size() + TE::nb_nodes - 1 );
                for( unsigned i = 0; i < s; ++i )
                    l[ l.size() - 1 - i ] = l[ s - 1 - i ];
                for( unsigned i = 0; i < TE::nb_nodes - 1; ++i )
                    l[ i ] = e.pos( i );
                return true;
            }
            
            return false;
        }
        
        bool append_if_start( const MultiLine<Pvec> &line ) {
            if ( l.size() == 0 )
                return false;
                
            Pvec p  = l[ 0 ];
            
            Pvec pe = line.l[ 0 ];
            
            if ( relative_distance( p , pe ) < epsi ) {
                unsigned s = l.size();
                l.resize( l.size() + line.l.size() );
                for( unsigned i = 0; i < s; ++i )
                    l[ l.size() - 1 - i ] = l[ s - 1 - i ];
                for( unsigned i = 1; i < line.l.size(); ++i )
                    l[ line.l.size() - 1 - i ] = line.l[ i ];
                return true;                
            }
            
            pe = line.l.back();
            
            if ( relative_distance( p , pe ) < epsi ) {
                unsigned s = l.size();
                l.resize( l.size() + line.l.size() - 1 );
                for( unsigned i = 0; i < s; ++i )
                    l[ l.size() - 1 - i ] = l[ s - 1 - i ];
                for( unsigned i = 0; i < line.l.size() - 1; ++i )
                    l[ i ] = line.l[ i ];
                return true;
            }
            
            return false;
        }
        
        template< class TE>
        bool append_if_end( const TE &e ) {
            if ( l.size() == 0 )
                return false;
                
            Pvec p  = l.back();
            
            Pvec pe = e.pos( 0 );
                
            if ( relative_distance( p , pe ) < epsi ) {
                unsigned s = l.size();
                l.resize( l.size() + TE::nb_nodes - 1 );
                for( unsigned i = 1; i < TE::nb_nodes; ++i )
                    l[ i + s - 1 ] = e.pos( i );
                return true;
            }
            
            pe = e.pos( TE::nb_nodes - 1 );
            
            if ( relative_distance( p , pe ) < epsi ) {
                unsigned s = l.size();
                l.resize( l.size() + TE::nb_nodes - 1 );
                for( unsigned i = 1; i < TE::nb_nodes; ++i )
                    l[ i + s - 1 ] = e.pos( TE::nb_nodes - 1 - i );
                return true;
            }            
            
            return false;
        }
    
        bool append_if_end( const MultiLine<Pvec> &line ) {
            if ( l.size() == 0 )
                return false;
                
            Pvec p  = l.back();
            
            Pvec pe = line.l[ 0 ];
            
            if ( relative_distance( p , pe ) < epsi ) {
                unsigned s = l.size();
                l.resize( l.size() + line.l.size() - 1 );
                for( unsigned i = 1; i < line.l.size(); ++i )
                    l[ i + s - 1 ] = line.l[ i ];
                return true;
            }            
            
            pe = line.l.back();
            
            if ( relative_distance( p , pe ) < epsi ) {
                unsigned s = l.size();
                l.resize( l.size() + line.l.size() - 1 );
                for( unsigned i = 1; i < line.l.size(); ++i )
                    l[ i + s - 1 ] = line.l[ line.l.size() - 1 - i ];
                return true;
            }           
            
            return false;
        }
    
        bool is_loop() const {
            if ( l.size() == 0 )
                return false;
                
            return relative_distance( l[ 0 ] , l.back() ) < epsi;
        }
        
        /*!
            retourne l'indice d'un point par rapport au segment semi_ouvert [A;B[
        */
        static int indice_for_segment( const Pvec &M, const Pvec &A, const Pvec &B ) {
        
            typedef typename Pvec::template SubType<0,0>::T T;
        
            if ( ( ( M[ 1 ] <= A[ 1 ] ) and ( M[ 1 ] > B[ 1 ] ) ) or ( ( M[ 1 ] >= A[ 1 ] ) and ( M[ 1 ] < B[ 1 ] ) ) ) {
                /// l'ordonnée de M est entre celles de A et B donc M se projete sur [A;B[ suivant la direction (Oy).
                T d = A[ 0 ] - M[ 0 ] + ( M[ 1 ] - A[ 1 ] ) / ( B[ 1 ] - A[ 1 ] ) * ( B[ 0 ] - A[ 0 ] ); /// d = P[ 0 ] - M[ 0 ] où P est la projection de M sur [A;B[.
                if ( d > 0. ) {
                    if ( A[ 1 ] > B[ 1 ] ) return -1; else return 1;
                } else {
                    if ( d < 0. ) {
                        if ( A[ 1 ] > B[ 1 ] ) return 1; else return -1;
                    } else {
                        ///cout << "Probleme : M appartient au segment!" << endl; 
                        return POINT_IN_SEGMENT; /// ?
                    }
                }
            } else
                return 0;
        }
        
        /*!
        retourne l'indice d'un lacet de segments.
        l doit contenir 2 * n valeurs telles que l[ 0 ] est l'abscisse du 1er point, l[ 1 ] son ordonnée, etc... 
        Rem : le dernier segment est ( l[ 2 * n - 2 ], l[ 2 * n - 1 ] )--( l[ 0 ], l[ 1 ] ) 
        
        */
        int indice_of( const Pvec &M ) {
        
            int sum = 0, ind;
            for( unsigned i = 0; i < l.size() - 1; ++i ) {
                ind = indice_for_segment( M, l[ i ], l[ i + 1 ] );
                if ( ind != POINT_IN_SEGMENT )
                    sum += ind;
                else
                    return POINT_IN_SEGMENT;
                
            }
        
            return sum / 2;
        }

        
        /// debug
        void display() const {
            std::cout << "---MultiLine---" << std::endl;
            if ( l.size() == 0 )
                return;
            std::cout << l[ 0 ];
            for( unsigned i = 1; i < l.size() ; ++i )
                std::cout << " --- " << l[ i ];
            std::cout << std::endl;
        }
    
        Vec<Pvec> l;
        static const int POINT_IN_SEGMENT = 2147483647;
    };
    
    /*!
        Ce foncteur a pour mission de déterminer la frontière d'un maillage avec ces éventuels trous.
        
        Algorithme :
            on parcourt m.skin.elem_list.
            Soit e l' élément courant. Si on peut le mettre au bout d'une file , on le fait sinon on crée une nouvelle file avec e.
             
            Après on rassemble toutes les files avec la méthode assemble_line().
    
    */
    template <class _Pvec, class T2 >
    struct Get_topology {
    
        Get_topology() : indice_of_wrapping( std::numeric_limits<unsigned>::max() ) {}
        
        template <class TE>
        void operator() ( const TE &e ) {
            for( unsigned i = 0; i < line_list.size(); ++i ) {
                if ( line_list[ i ].append_if_start( e ) )
                    return;
                
                if ( line_list[ i ].append_if_end( e ) )
                    return;
            }
            
            line_list.push_back( MultiLine< Pvec >( e ) );
        }
        
        void assemble_line() {
            typedef typename std::list< MultiLine< Pvec > >::iterator ITER;
        
            std::list< MultiLine< Pvec > > loop_list;
            std::list< MultiLine< Pvec > > real_line_list;
            
            for( unsigned i = 0; i < line_list.size(); ++i ) {
                if ( line_list[ i ].is_loop() )
                    loop_list.push_back( line_list[ i ] );
                else
                    real_line_list.push_back( line_list[ i ] );
            }
            
            //PRINT( loop_list.size() );
            //PRINT( real_line_list.size() );
            
            while( real_line_list.size() ) {
                //PRINT( real_line_list.size() );
                while( not real_line_list.front().is_loop() ) {
                    ITER i = real_line_list.begin();
                    ++i; /// on saute le premier élément
                    for( ; i != real_line_list.end(); ++i ) {
                        //std::cout << "@" << std::endl;
                        if ( real_line_list.front().append_if_start( *i ) ) {
                            //std::cout << "B" << std::endl;
                            real_line_list.erase( i );
                            //std::cout << "C" << std::endl;
                            break;
                        } else {
                            //std::cout << "A" << std::endl;
                            if ( real_line_list.front().append_if_end( *i ) ) {
                                real_line_list.erase( i );
                                break;
                            }
                        }   
                    }
                    //std::cout << "D" << std::endl;
                    //real_line_list.front().display();
                }
                
                loop_list.push_back( real_line_list.front() );
                real_line_list.erase( real_line_list.begin() );
            }
            
            line_list.resize( 0 );
            for( ITER i = loop_list.begin(); i != loop_list.end(); ++i )
                line_list.push_back( *i );
               
            /// détermination de la topologie
            if ( line_list.size() == 1 ) 
                indice_of_wrapping = 0;
            else {
                bool loop = true;
                for( unsigned i = 0; ( i < line_list.size() - 1 ) and loop; ++i )
                    for( unsigned j = i + 1; ( j < line_list.size() ) and loop; ++j ) {
                        int ind = std::abs( line_list[ i ].indice_of( line_list[ j ].l[ 0 ] ) );
                        if ( ind == 1 ) {
                            indice_of_wrapping = i;
                            loop = false;
                        }
                        
                        ind = std::abs( line_list[ j ].indice_of( line_list[ i ].l[ 0 ] ) );
                        if ( ind == 1 ) {
                            indice_of_wrapping = j;
                            loop = false;
                        }
                    }
            }
            
            /// recherche d'un point dans chaque trou
            hole_list.resize( line_list.size() );
            for( unsigned i = 0; i < line_list.size(); ++i )
                if ( i != indice_of_wrapping ) {
                    bool loop = true;
                    for( unsigned j = 0; ( j < line_list[ i ].l.size() - 1 ) and loop; ++j ) {
                        Pvec middle = 0.5 * ( line_list[ i ].l[ j ] + line_list[ i ].l[ j + 1 ] );
                        Pvec u = line_list[ i ].l[ j + 1 ] - line_list[ i ].l[ j ];
                        Pvec n( -u[ 1 ], u[ 0 ] );
                        n = normalized( n ); /// normale au segment
                        T2 d = 1.;
                        while( d > 1e-5 ) {
                            Pvec p = middle + d * n;
                            if ( std::abs( line_list[ i ].indice_of( p ) ) == 1 ) {
                                hole_list[ i ] = p;
                                loop = false;
                                break;
                            }
                            p = middle - d * n;
                            if ( std::abs( line_list[ i ].indice_of( p ) ) == 1 ) {
                                hole_list[ i ] = p;
                                loop = false;
                                break;
                            }                            
                            p *= 0.5;
                        }
                    }
                } else
                    hole_list[ i ] = std::numeric_limits<T2>::max();
        }
        
        Vec< MultiLine< _Pvec > > line_list;
        Vec< _Pvec> hole_list;
        unsigned indice_of_wrapping;
    };

    /*!
        Ce foncteur a pour objectif d'ajouter les segments d'un maillage aux structures de Triangle_software.

    */
    template< class TNode>
    struct AppendAllMesh {
        
        AppendAllMesh( std::map< const TNode*, int> &ltt, Vec<int> &_segment_list ) : lmt_to_Triangle( ltt ), segment_list( _segment_list ) {}
        
        template< class TE >
        void linear_or_bilinear( const TE &e ) {
        
            for( unsigned i = 0; i < TE::nb_nodes; ++i ) {
                int seg[ 2 ];
                seg[ 0 ] = lmt_to_Triangle[ e.node( i ) ];
                seg[ 1 ] = lmt_to_Triangle[ e.node( ( i + 1 ) % TE::nb_nodes ) ];
                unsigned j = 0;
                for( ; j < segment_list.size(); j += 2 )
                    if ( ( ( seg[ 0 ] == segment_list[ 0 ] ) and ( seg[ 1 ] == segment_list[ 1 ] ) ) or 
                         ( ( seg[ 1 ] == segment_list[ 0 ] ) and ( seg[ 0 ] == segment_list[ 1 ] ) ) )
                        break;
                if ( j >= segment_list.size() ) {
                    segment_list.push_back( seg[ 0 ] );
                    segment_list.push_back( seg[ 1 ] );
                }
            }
        }        
        
        /// Triangle
        template< class NameBehavior,class TNode_,class TData,unsigned num_in_elem_list_>
        void operator() ( const Element< Triangle, NameBehavior, TNode_, TData, num_in_elem_list_> &e ) {

            linear_or_bilinear( e );
        }
        
        /// Quad
        template< class NameBehavior,class TNode_,class TData,unsigned num_in_elem_list_>
        void operator() ( const Element< Quad, NameBehavior, TNode_, TData, num_in_elem_list_> &e ) {

            linear_or_bilinear( e );
        }        
        
        template< class TE >
        void operator() ( const TE &e ) {
            /// TODO
            std::cerr << "Warning AppendAllMesh.operator() : " << e.name() << " is not appended." << std::endl;
        }
        
        std::map< const TNode*, int> &lmt_to_Triangle;
        Vec<int> &segment_list;
    };

    Triangle_software() : nb_point_by_triangle( 3 ) {  }
    
    template<class TM>
    Triangle_software( TM &m, const Mode_append mode = All_mode ) : nb_point_by_triangle( 3 ) {
        append_mesh( m, mode );
    }
    
    int find_point( const T x, const T y ) {
        return find_point( Pvec( x, y ) );
    }
    
    int find_point( const Pvec &p ) {
        
        int j = 0;
        for( ; j < point_list.size(); j += 2 )
            if ( relative_distance( p , Pvec( point_list[ j ], point_list[ j + 1 ] ) ) < epsi )
                break;
        return j;
    }
    
    int find_segment( const int p1, const int p2 ) {
        return find_segment( Vec<int,2>( p1, p2 ) );
    }
    
    template<int s>
    int find_segment( const Vec<int,s> &p ) {
        
        int j = 0;
        for( ; j < segment_list.size(); j += 2 )
            if ( ( p[ 0 ] == segment_list[ j ] ) and ( p[ 1 ] == segment_list[ j + 1 ] )  )
                break;
        return j;
    }
    
    template< class T2 >
    int append_point( const T2 x, const T2 y ) {
        return append_point( Pvec( x, y ) );
    }
    
    template< class T2, int s >
    int append_point( const Vec<T2, s> &p ) {
        int index = find_point( p[ 0 ], p[ 1 ] );
        if ( index == point_list.size() ) {
            point_list.push_back( p[ 0 ] );
            point_list.push_back( p[ 1 ] );
        }
        return index / 2;
    }
    
    void append_segment( int p1, int p2 ) {
        append_segment( Vec<int,2>( p1, p2 ) ); 
    }
    
    template< int s >
    void append_segment( const Vec<int, s> &p ) {
    
        int index = find_segment( p );
        
        if ( index < segment_list.size() )
            return;
    
        if ( ( p[ 0 ] < point_list.size() ) and ( p[ 1 ] < point_list.size() ) ) {
            segment_list.push_back( p[ 0 ] );
            segment_list.push_back( p[ 1 ] );
        }
    }
    
    template <class TM>
    void append_mesh( TM &m, const Mode_append mode = All_mode ) {
        
        m.update_skin();
        /// détermination de la topologie
        //PRINT( m.skin.elem_list.size() );
        Get_topology<Pvec, T> gt;
        apply( m.skin.elem_list, gt );
        gt.assemble_line();
        //PRINT( gt.line_list.size() );
        //for( unsigned i = 0; i < gt.line_list.size(); ++i ) {
        //    gt.line_list[ i ].display();
        //    PRINT( gt.hole_list[ i ] );
        //}
        //PRINT( gt.indice_of_wrapping );
                
        /// insertion des trous
        if ( gt.hole_list.size() >= 2 ) {
            unsigned s = hole_list.size();
            hole_list.resize( s + 2 * ( gt.hole_list.size() - 1 ) );
            for( unsigned i = 0, p = 0; i < gt.hole_list.size() ; ++i )
                if ( i != gt.indice_of_wrapping ) {
                    hole_list[ s + 2 * p     ] = gt.hole_list[ i ][ 0 ];
                    hole_list[ s + 2 * p + 1 ] = gt.hole_list[ i ][ 1 ];
                    ++p;
                }
        }
        
        switch( mode ) {
            case All_mode : {
            
                std::map< const typename TM::TNode*, int> lmt_to_Triangle;
                int ipointStart  = point_list.size() / 2;
                for( unsigned i = 0; i < m.node_list.size(); ++i ) {
                    Pvec p( m.node_list[ i ].pos[ 0 ], m.node_list[ i ].pos[ 1 ] );
                    /// vérification que p n'est pas déjà dans la liste
                    int j = find_point( p );
                    if ( j < point_list.size() )
                        lmt_to_Triangle[ &m.node_list[ i ] ] = j / 2;
                    else {
                        point_list.push_back( p[ 0 ] );
                        point_list.push_back( p[ 1 ] );
                        lmt_to_Triangle[ &m.node_list[ i ] ] = ipointStart;
                        ipointStart++;
                    }
                }
                
                AppendAllMesh< typename TM::TNode > aam( lmt_to_Triangle, segment_list );
                apply( m.elem_list, aam );
                
            } break;
            case Only_skin_mode : case Skin_and_node_mode : {
            
                /// insertion des points  du maillage
                for( unsigned i = 0; i < gt.line_list.size(); ++i ) {
                    unsigned s = point_list.size();
                    int ipointStart = point_list.size() / 2;
                    point_list.resize( s + 2 * ( gt.line_list[ i ].l.size() - 1 ) );
                    for( unsigned j = 0; j < gt.line_list[ i ].l.size() - 1; ++j ) {
                        point_list[ s + 2 * j     ] = gt.line_list[ i ].l[ j ][ 0 ];
                        point_list[ s + 2 * j + 1 ] = gt.line_list[ i ].l[ j ][ 1 ];
                    }
                    
                    s = segment_list.size();
                    segment_list.resize( s + 2 * ( gt.line_list[ i ].l.size() - 1 ) );
                    for( unsigned j = 0; j < gt.line_list[ i ].l.size() - 2; ++j ) {
                        segment_list[ s + 2 * j     ] = ipointStart + j;
                        segment_list[ s + 2 * j + 1 ] = ipointStart + j + 1 ;
                    }
                    
                    segment_list[ segment_list.size() - 2 ] = ipointStart + gt.line_list[ i ].l.size() - 2;
                    segment_list[ segment_list.size() - 1 ] = ipointStart;
                }

                if ( mode == Skin_and_node_mode ) {
                    /// NAIF
                    for( unsigned i = 0; i < m.node_list.size(); ++ i ) {
                        bool appe = true;
                        for( unsigned j = 0; j < m.skin.node_list.size(); ++j ) {
                            if ( relative_distance( m.node_list[ i ].pos , m.skin.node_list[ j ].pos ) < epsi ) {
                                appe = false;
                                break;
                            }
                        }
                        if ( appe ) {
                            point_list.push_back( m.node_list[ i ].pos[ 0 ] );
                            point_list.push_back( m.node_list[ i ].pos[ 1 ] );
                        }
                    }
                }
                
            } break;

            default :
                std::cerr << "Error in Triangle_software.append_mesh() : unknown Mode_append" << std::endl;
        }
    }
    
    void clear() {
        point_list.free();
        segment_list.free();
    }
    
    template< class TMESH>
    void get_triangulation( TMESH &m, const std::string& triswitch, triunsuitable_criterium_t triunsuitable_criterium = NULL, void* param_triunsuitable = NULL ) const {
        triangulateio tri_in, tri_out;
        
        /// initialisation
        if ( point_list.size() ) 
            tri_in.pointlist              = const_cast<double*>( point_list.ptr() );
        else
            tri_in.pointlist              = NULL;
        tri_in.pointattributelist         = NULL;
        tri_in.pointmarkerlist            = NULL;
        tri_in.numberofpoints             = point_list.size() / 2;
        tri_in.numberofpointattributes    = 0;
        tri_in.trianglelist               = NULL;
        tri_in.triangleattributelist      = NULL;
        tri_in.trianglearealist           = NULL;
        tri_in.neighborlist               = NULL;
        tri_in.numberoftriangles          = 0;
        tri_in.numberofcorners            = 0;
        tri_in.numberoftriangleattributes = 0; 
        if ( segment_list.size() )     
            tri_in.segmentlist            = const_cast<int*>( segment_list.ptr() );
        else
            tri_in.segmentlist            = NULL;
        tri_in.segmentmarkerlist          = NULL;
        tri_in.numberofsegments           = segment_list.size() / 2;
        if ( hole_list.size() )
            tri_in.holelist               = const_cast<double*>( hole_list.ptr() );
        else
            tri_in.holelist               = NULL;
        tri_in.numberofholes              = hole_list.size() / 2;
        tri_in.regionlist                 = NULL;
        tri_in.numberofregions            = 0;
        tri_in.triunsuitable_criterium    = triunsuitable_criterium;
        tri_in.param_triunsuitable        = param_triunsuitable;
        
        
        tri_out.pointlist                  = NULL;
        tri_out.pointattributelist         = NULL;
        tri_out.pointmarkerlist            = NULL;
        tri_out.numberofpoints             = 0;
        tri_out.numberofpointattributes    = 0;
        tri_out.trianglelist               = NULL;
        tri_out.triangleattributelist      = NULL;
        tri_out.trianglearealist           = NULL;
        tri_out.neighborlist               = NULL;
        tri_out.numberoftriangles          = 0;
        tri_out.numberofcorners            = 0;
        tri_out.numberoftriangleattributes = 0;      
        tri_out.segmentlist                = NULL;
        tri_out.segmentmarkerlist          = NULL;
        tri_out.numberofsegments           = 0;
        tri_out.holelist                   = NULL;
        tri_out.numberofholes              = 0;
        tri_out.regionlist                 = NULL;
        tri_out.numberofregions            = 0;
        
        std::ostringstream oss;
        oss << "zpDQ";
        if ( triunsuitable_criterium != NULL )
            oss << "u";
        oss << triswitch; 
        
        triangulate( const_cast<char*>( oss.str().c_str() ), &tri_in, &tri_out, NULL );
        
        //display( tri_in );
        //std::cout << "***************************************************" << std::endl;
        //display( tri_out );
        
        m.free();
        
        if ( tri_out.numberoftriangles == 0 )
            return;
            
        typedef typename TMESH::TNode TNode;
        typedef typename TMESH::TNode::Pvec Pvec; 
        static const unsigned dim = TNode::dim;
        
        Vec< TNode* > vinode; vinode.resize( tri_out.numberofpoints );
            
        for( int i = 0; i < tri_out.numberofpoints; ++i ) {
            Pvec p;
            for( unsigned d = 0; d < dim; ++d )
                p[ d ] = tri_out.pointlist[ i * 2 + d ];
            vinode[ i ] = m.add_node( p );
        }
        
        Vec< TNode* > vn;
        int nb_corner = tri_out.numberofcorners;
        
        switch( nb_corner ) {
            case 3 : {
                vn.resize( nb_corner );
                for( int i = 0; i < tri_out.numberoftriangles; ++i ) {
                    vn[ 0 ] = vinode[ tri_out.trianglelist[ i * nb_corner     ] ];
                    vn[ 1 ] = vinode[ tri_out.trianglelist[ i * nb_corner + 1 ] ];
                    vn[ 2 ] = vinode[ tri_out.trianglelist[ i * nb_corner + 2 ] ];
                    permutation_if_jac_neg ( Triangle(), vn.ptr() );
                    m.add_element( Triangle(),DefaultBehavior(), vn.ptr() );
                }
            } break;
            case 6 : {
                /// TODO
            } break;
            
            default : ;
        }
        
        free( tri_out.pointlist             );
        free( tri_out.pointattributelist    );
        free( tri_out.pointmarkerlist       );
        free( tri_out.trianglelist          );
        free( tri_out.triangleattributelist );
        free( tri_out.trianglearealist      );
        free( tri_out.neighborlist          );
        free( tri_out.segmentlist           );
        free( tri_out.segmentmarkerlist     );
        free( tri_out.regionlist            );
        
    }
    
    int nb_point_by_triangle; /// nombre de points dans un Triangle
    Vec< T > point_list;
    Vec< T > hole_list;
    Vec< int > segment_list; /// an segment has 2 indexes in the list "point_list"
    //Vec< int > triangle_list; /// an segment has nb_point_by_triangle indexes in the list "point_list"
    static T epsi;
};

double Triangle_software::epsi = std::numeric_limits<double>::epsilon() * 64;

#endif /// TRIANGLE_SOFTWARE_H
