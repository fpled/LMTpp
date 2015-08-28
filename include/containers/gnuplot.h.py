# -*- coding: utf-8 -*-
from string import *

print """//
// C++ Interface: gnuplot
//
// Description: 
//
//
// Author: Hugo LECLERC <hugo.leclerc@lmt.ens-cachan.fr>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMT_gnuplot_HEADER
#define LMT_gnuplot_HEADER

#include "mat.h"
#include <sstream>
#include <string>

namespace LMT {

/*!
    classe servant d'interface avec le celebre programme gnuplot.
    \\author Hugo Leclerc
    \\author Camille Gouttebroze
    \\author Florent Pled
    \\friend hugo.leclerc@lmt.ens-cachan.fr
    \\friend camille.gouttebroze@lmt.ens-cachan.fr
    \\friend florent.pled@univ-paris-est.fr
    \\keyword Visualisation/2D
*/
class GnuPlot {
public:

    struct HD {

        HD() {}

        HD( Vec<> x_, std::string p_ ) { params = p_; x = x_;}

        template <class OPY>
        HD( Vec<> x_, OPY y_, std::string p_ ) {
            params = p_;
            x = x_;
            y.resize(x.size());
            for (int i=0;i<y.size();i++)
                y[i] = y_(x[i]);
        }

        HD( Vec<> x_, Vec<> y_, std::string p_ ) { params = p_; x = x_; y = y_; }

        template <class OPZ>
        HD( Vec<> x_, Vec<> y_, OPZ z_, std::string p_ ) {
            params = p_;
            x = x_;
            y = y_;
            z.resize(x.size());
            for (int i=0;i<z.size();i++)
                z[i] = z_(x[i],y[i]);
        }

        HD( Vec<> x_, Vec<> y_, Vec<> z_, std::string p_ ) { params = p_; x = x_; y = y_; z = z_; }

        std::string params;
        Vec<> x,y,z;

    };

    GnuPlot() {
        hold = false;
        if ( (tube=popen("gnuplot","w")) == NULL ) {
            std::cerr << "gnuplot impossible a ouvrir" << std::endl;
            exit(1);
        }
    }

    ~GnuPlot() {
        pclose(tube);
    }

    /// Print
    void print(const char *str = "") const {
        fprintf(tube,"%s",str);
        fflush(tube);
    }
"""

lst = [ '', 'terminal', 'terminal epslatex', 'output', 'format', 'title', 'colorsequence', 'xlabel', 'ylabel', 'zlabel', 'xrange', 'yrange', 'zrange', 'label', 'key' ]

for i in lst :
    print '    void set'+(len(i)>0)*'_'+i.replace(' ','_')+'(const char *str = "") const { std::stringstream s; s << \"set'+(len(i)>0)*' '+i+' \" << str << \"\\n\"; print( s.str().c_str() ); } '
    print '    void unset'+(len(i)>0)*'_'+i.replace(' ','_')+'(const char *str = "") const { std::stringstream s; s << \"unset'+(len(i)>0)*' '+i+' \" << str << \"\\n\"; print( s.str().c_str() ); } '

print """
    /// Set all graph-related options to their default values
    void reset() { print("reset\\n\\n"); }

    /// Stop and wait until the carriage return is pressed
    void wait() {
        print("pause -1\\n\\n");
        std::cin.get();
    }

    struct Disp {
        template<class TX>
        void operator() (const TX &x, FILE *tube) const { fprintf(tube,"%e\\n",double(x)); }
        template<class TX,class TY>
        void operator() (const TX &x, unsigned i, const TY &y, FILE *tube) const { fprintf(tube,"%e %e\\n",double(x),double(y(x))); }
        template<class TX, class T, int s, class O>
        void operator() (const TX &x, unsigned i, const Vec<T,s,O> &y, FILE *tube) const { fprintf(tube,"%e %e\\n",double(x),double(y[i])); }
        template<class TX, class TY, int sy, class OY, class TZ>
        void operator() (const TX &x, unsigned i, const Vec<TY,sy,OY> &y, const TZ &z, FILE *tube) const { fprintf(tube,"%e %e %e\\n",double(x),double(y[i]),double(z(x,y[i]))); }
        template<class TX, class TY, int sy, class OY, class TZ, int sz, class OZ>
        void operator() (const TX &x, unsigned i, const Vec<TY,sy,OY> &y, const Vec<TZ,sz,OZ> &z, FILE *tube) const { fprintf(tube,"%e %e %e\\n",double(x),double(y[i]),double(z[i])); }
    };
    
    /*!
        Cette méthode permet de représenter une surface discrétisée définie "au dessus" d'un rectangle.
        Entrées :
            * mat est une matrice de trois colonnes : la premiere colonne regroupe les abscisses, la deuxième les ordonnées et la troisième les côtes. Il y a autant de lignes que de points.
            * mins[ 0 ] est le minimum des abscisses
            * mins[ 1 ] est le minimum des ordonnées 
            * maxs[ 0 ] est le maximum des abscisses
            * maxs[ 1 ] est le maximum des ordonnées
            * nb[ 0 ] est le nombre points suivant l'axe des abscisses
            * nb[ 1 ] est le nombre points suivant l'axe des ordonnées
            
        Remarque : le nombre de lignes de mat est donc nb[ 0 ] * nb[ 1 ]
        ATTENTION : pour éviter un affichage bizarre, éviter de vous tromper sur les nb[ 0 ] et nb[ 1 ].
           
    */
    template<class T,class STR,class STO>
    void surface( const Mat<T,STR,STO> &mat, Vec<T,2> mins, Vec<T,2> maxs, Vec<int,2> nb, const char *params="" ) {
        std::ostringstream oss;
        oss << "set xrange [" << mins[ 0 ] << ":" << maxs[ 0 ] << "];";
        oss << "set yrange [" << mins[ 1 ] << ":" << maxs[ 1 ] << "];";
        oss << "set dgrid3d " << nb[ 0 ] - 1 << "," <<  nb[ 1 ] - 1 << ";";
        oss << "set hidden3d;"; 
        fprintf(tube,"%s\\n",oss.str().c_str());
        fprintf(tube,"splot '-' using 1:2:3 %s with lines \\n", params);
        std::ostringstream ss; ss << mat;
        fprintf(tube,"%s\\ne\\n",ss.str().c_str());
        fflush(tube);
    }
    
    /*!
        Cette fonction affiche un champ de vecteurs (vx,vy) discrétisé, défini aux points (x,y)

        mat doit etre une matrice de 4 colonnes au moins tel que :
            * la premiere colonne contient les x
            * la deuxieme colonne contient les y
            * la troisieme contient vx la premiere composante du vecteur
            * la quatrieme contient vy

        \\friend pasquier@lmt.ens-cachan.fr
        \\friend witz@lmt.ens-cachan.fr
    */
    template<class T,class STR,class STO>
    void plot_field(const Mat<T,STR,STO> &mat,const char *params="") {
        //fprintf(tube,"plot '-' using 1:2:(0.01*$3/sqrt($3*$3+$4*$4)):(0.01*$4/sqrt($3*$3+$4*$4)) title \\"%s\\" with vectors\\n",params); // ne marche pas encore...
        fprintf(tube,"plot '-' using 1:2:3:4 title \\"%s\\" with vectors\\n",params);
        std::ostringstream ss; ss << mat;
        fprintf(tube,"%s\\ne\\n",ss.str().c_str());
        fflush(tube);
    }"""

nb_par_max = 6

template = ['class T0']
arguments = [' const T0 &data0']

for nb_par in range(1,nb_par_max) :
    template += [' class T'+str(nb_par)]
    arguments += [' const T'+str(nb_par)+' &data'+str(nb_par)]
    for plot in [0,1] :
        print """
    template<"""+join(template,',')+plot *(', class T'+str(nb_par+1))+""">
    void """+plot*'s'+"""plot("""+join(arguments,',')+plot*(', const T'+str(nb_par+1)+' &data'+str(nb_par+1) )+""", const char *params="", bool jump_lines = false ) {
        if ( hold ) {"""
        for i in range(nb_par) :
            print '            holded_data.push_back( HD( data0,'+plot*' data1,'+' data'+str(i+1+plot)+', params ) );'
        print '        }'
        print '        else {'
        print '            fprintf(tube,"'+plot*'s'+'plot' + (nb_par-1)*' \'-\' %s,' + ' \'-\' %s\\n",' + (nb_par-1)*'params,' + 'params);'
        print '            if (jump_lines and data0.size()) {'
        for i in range(nb_par) :
            print '                fprintf_jumping_lines( data0,'+plot*' data1,'+' data'+str(i+1+plot)+' );'
            print '                fprintf(tube,"e\\n");'
        print '            }'
        print '            else {'
        for i in range(nb_par) :
            print '                apply_wi( data0, Disp(), '+plot*' data1,'+' data'+str(i+1+plot)+', tube );'
            print '                fprintf(tube,"e\\n");'
        print """            }
            fflush(tube);
        }
    }"""

print """
    template<class T,int s,class O>
    void plot( const Vec<T,s,O> &vec, const char *params="", bool jump_lines = false ) {
        if ( hold )
            holded_data.push_back( HD ( vec, params )  );
        else {
            fprintf(tube,"plot '-' %s\\n",params);
            apply( vec, Disp(), tube );
            fprintf(tube,"e\\n");
            fflush(tube);
        }
    }

    template<class T,class STR,class STO>
    void splot( const Mat<T,STR,STO> &mat, const char *params="", bool jump_lines = false ) {
        fprintf(tube,"splot '-' matrix %s\\n",params);
        std::ostringstream ss; ss << mat;
        fprintf(tube,"%s\\ne\\n",ss.str().c_str());
        fflush(tube);
    }

    void hold_on() { hold = true; }

    void hold_off( bool jump_lines = false ) {
        if ( holded_data.size()==0 ) return;

        if ( holded_data[0].z.size() )
            fprintf(tube,"splot ");
        else
            fprintf(tube,"plot ");
        for(unsigned i=0;i<holded_data.size();++i) {
            fprintf(tube," '-' %s",holded_data[i].params.c_str());
            fprintf(tube, ( i<holded_data.size()-1 ? "," : "\\n" ) );
        }

        for(unsigned i=0;i<holded_data.size();++i) {
            if ( jump_lines and holded_data[i].x.size() )
                fprintf_jumping_lines( holded_data[i].x, holded_data[i].y, holded_data[i].z );
            else if ( holded_data[i].z.size() )
                apply_wi( holded_data[i].x, Disp(), holded_data[i].y, holded_data[i].z, tube );
            else if ( holded_data[i].y.size() )
                apply_wi( holded_data[i].x, Disp(), holded_data[i].y, tube );
            else
                apply( holded_data[i].x, Disp(), tube );
            fprintf(tube,"e\\n");
        }
        fflush(tube);
        holded_data.resize( 0 );
        hold = false;
    }

    template<class TY>
    void fprintf_jumping_lines( const Vec<> &x, const TY &y ) {}

    template<class TZ>
    void fprintf_jumping_lines( const Vec<> &x, const Vec<> &y, const TZ &z ) {
        double old_x = x[0];
        for (unsigned i=0;i<x.size();i++) {
            if (x[i]!=old_x) {
                fprintf(tube,"\\n");
                old_x = x[i];
            }
            fprintf(tube,"%e %e %e\\n",x[i],y[i],z(x[i],y[i]));
        }
    }

    void fprintf_jumping_lines( const Vec<> &x, const Vec<> &y, const Vec<> &z ) {
        double old_x = x[0];
        for (unsigned i=0;i<x.size();i++) {
            if (x[i]!=old_x) {
                fprintf(tube,"\\n");
                old_x = x[i];
            }
            fprintf(tube,"%e %e %e\\n",x[i],y[i],z[i]);
        }
    }

    FILE *tube;
    bool hold;
    Vec<HD> holded_data;

};"""

template = []
arguments = []
data = []

for nb_par in range(nb_par_max) :
    template += ['class T'+str(nb_par)]
    arguments += [' const T'+str(nb_par)+' &data'+str(nb_par)]
    data += [' data'+str(nb_par)]
    for plot in ['plot','splot'] :
        print """
template<"""+join(template,',')+""">
void """+plot+"""("""+join(arguments,',')+""", const char *params="", bool jump_lines = false ) {
    GnuPlot gp;
    gp."""+plot+"""("""+join(data,',')+""", params, jump_lines );
    gp.wait();
}"""
    for plot in ['plot','splot'] :
        print """
template<"""+join(template,',')+""">
void save_"""+plot+"""("""+join(arguments,',')+""", const char *output="", const char *xlabel="", const char *ylabel="","""+(plot=='splot')*""" const char *zlabel="","""+""" const char *params="", bool jump_lines = false ) {
    GnuPlot gp;
    std::stringstream s; s << output;
    std::string filename( s.str() );
    if ( filename.rfind( ".tex'" ) == filename.size() - 5 ) {
        gp.set_terminal_epslatex( "color colortext" );
        gp.set_format("'$%g$'");
    }
    else if ( filename.rfind( ".png'" ) == filename.size() - 5 )
        gp.set_terminal( "pngcairo enhanced" );
    else if ( filename.rfind( ".svg'" ) == filename.size() - 5 )
        gp.set_terminal( "svg enhanced" );
    else if ( filename.rfind( ".pdf'" ) == filename.size() - 5 )
        gp.set_terminal( "pdfcairo enhanced color" );
    else
        gp.set_terminal( "postscript eps enhanced color colortext" );
    gp.set_output(output);
    gp.set_xlabel(xlabel);
    gp.set_ylabel(ylabel);"""+"""
    gp.set_zlabel(zlabel);"""*(plot=='splot')+"""
    gp."""+plot+"""("""+join(data,',')+""", params, jump_lines );
}"""
print """
}

#endif // LMT_gnuplot_HEADER"""
