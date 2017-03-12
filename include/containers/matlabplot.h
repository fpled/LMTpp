//
// C++ Interface: matlabplot
//
// Description: 
//
//
// Author: Hugo LECLERC <hugo.leclerc@lmt.ens-cachan.fr>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMT_matlabplot_HEADER
#define LMT_matlabplot_HEADER

#include "mat.h"
#include <sstream>
#include <string>
#include <stdio.h>
#include <unistd.h>

namespace LMT {

/*!
    classe servant d'interface avec le celebre programme matlab.
    \author Hugo Leclerc
    \author Florent Pled
    \friend hugo.leclerc@lmt.ens-cachan.fr
    \friend florent.pled@univ-paris-est.fr
    \keyword Visualisation/2D
*/
class MatlabPlot {
public:

    MatlabPlot() {
        display = true;
        if ( (tube=popen("matlab -nodesktop -nosplash","w")) == NULL ) {
            std::cerr << "matlab impossible à ouvrir" << std::endl;
            exit(1);
        }
    }

    ~MatlabPlot() {
        pclose(tube);
    }

    /// Print
    void print(const char *str = "") const {
        fprintf(tube,"%s",str);
        fflush(tube);
    }

    void addpath(const char *str = "") const { std::stringstream s; s << "addpath(" << str << ")\n"; print( s.str().c_str() ); }
    void addpath_genpath(const char *str = "") const { std::stringstream s; s << "addpath(genpath(" << str << "))\n"; print( s.str().c_str() ); }
    void set_gcf(const char *str = "") const { std::stringstream s; s << "set(gcf," << str << ")\n"; print( s.str().c_str() ); }
    void set_gca(const char *str = "") const { std::stringstream s; s << "set(gca," << str << ")\n"; print( s.str().c_str() ); }
    void set_name(const char *str = "") const { std::stringstream s; s << "set(gcf,'Name'," << str << ")\n"; print( s.str().c_str() ); }
    void set_paperpositionmode(const char *str = "") const { std::stringstream s; s << "set(gcf,'PaperPositionMode'," << str << ")\n"; print( s.str().c_str() ); }
    void set_renderer(const char *str = "") const { std::stringstream s; s << "set(gcf,'Renderer'," << str << ")\n"; print( s.str().c_str() ); }
    template<class T>
    void set_fontsize(const T &val) const { std::stringstream s; s << "set(gca,'FontSize'," << to_string( val ) << ")\n"; print( s.str().c_str() ); }
    void set_title(const char *str = "") const { std::stringstream s; s << "title(" << str << ")\n"; print( s.str().c_str() ); }
    void axis(const char *str = "") const { std::stringstream s; s << "axis " << str << "\n"; print( s.str().c_str() ); }
    void caxis(const char *str = "") const { std::stringstream s; s << "caxis " << str << "\n"; print( s.str().c_str() ); }
    void set_axis(const char *str = "") const { std::stringstream s; s << "axis(" << str << ")\n"; print( s.str().c_str() ); }
    void set_caxis(const char *str = "") const { std::stringstream s; s << "caxis(" << str << ")\n"; print( s.str().c_str() ); }
    void set_xlabel(const char *str = "") const { std::stringstream s; s << "xlabel(" << str << ")\n"; print( s.str().c_str() ); }
    void set_ylabel(const char *str = "") const { std::stringstream s; s << "ylabel(" << str << ")\n"; print( s.str().c_str() ); }
    void set_zlabel(const char *str = "") const { std::stringstream s; s << "zlabel(" << str << ")\n"; print( s.str().c_str() ); }
    void set_xlabel_interpreter(const char *str = "", const char *interpreter = "") const { std::stringstream s; s << "xlabel(" << str << ",'Interpreter'," << interpreter << ")\n"; print( s.str().c_str() ); }
    void set_ylabel_interpreter(const char *str = "", const char *interpreter = "") const { std::stringstream s; s << "ylabel(" << str << ",'Interpreter'," << interpreter << ")\n"; print( s.str().c_str() ); }
    void set_zlabel_interpreter(const char *str = "", const char *interpreter = "") const { std::stringstream s; s << "zlabel(" << str << ",'Interpreter'," << interpreter << ")\n"; print( s.str().c_str() ); }
    void set_xlim(const char *str = "") const { std::stringstream s; s << "xlim(" << str << ")\n"; print( s.str().c_str() ); }
    void set_ylim(const char *str = "") const { std::stringstream s; s << "ylim(" << str << ")\n"; print( s.str().c_str() ); }
    void set_zlim(const char *str = "") const { std::stringstream s; s << "zlim(" << str << ")\n"; print( s.str().c_str() ); }
    void set_grid(const char *str = "") const { std::stringstream s; s << "grid " << str << "\n"; print( s.str().c_str() ); }
    void set_box(const char *str = "") const { std::stringstream s; s << "box " << str << "\n"; print( s.str().c_str() ); }
    void set_view(const char *str = "") const { std::stringstream s; s << "view(" << str << ")\n"; print( s.str().c_str() ); }
    void set_legend(const char *str = "") const { std::stringstream s; s << "legend(" << str << ")\n"; print( s.str().c_str() ); }
    void set_legend_interpreter(const char *str = "", const char *interpreter = "") const { std::stringstream s; s << "l = legend(" << str << ");\n set(l,'Interpreter'," << interpreter << ")\n"; print( s.str().c_str() ); }

    void convert(const char *str = "") const { std::stringstream s; s << "convert " << str << ".jpg -compress jpeg eps2:" << str << ".eps &"; system( s.str().c_str() ); }
    void pdfcrop(const char *str = "") const { std::stringstream s; s << "pdfcrop " << str << "..pdf &"; system( s.str().c_str() ); }

    /// Go to the current working directory
    void cd_cwd() const {
        char *cwd;
        cwd = (char*) malloc( FILENAME_MAX * sizeof(char) );
        std::stringstream s; s << "cd " << getcwd(cwd,FILENAME_MAX) << "\n"; print( s.str().c_str() );
    }

    /// Get filename without extension from output (filename with extension)
    std::string get_filename(const char *output = "") {
        std::stringstream s; s << output;
        std::string filename( s.str() );
        size_t lastdot = filename.find_last_of(".");
        if ( lastdot == std::string::npos )
            return filename;
        return filename.substr(0,lastdot) + "'";
    }

    /// Get format (extension) from output (filename with extension)
    std::string get_format(const char *output = "") {
        std::stringstream s; s << output;
        std::string filename( s.str() );
        size_t lastdot = filename.find_last_of(".");
        if ( lastdot == std::string::npos )
            return "";
        return "'" + filename.substr(lastdot+1);
    }

    /// Save current figure as LaTeX file
    void matlab2tikz(const char *output = "", const char *params = "") const { std::stringstream s; s << "matlab2tikz(" << output << ",'encoding','UTF-8','parseStrings',false,'showInfo',false" << params << ");\n"; print( s.str().c_str() ); }

    /// Save current figure
    void saveas(const char *filename = "", const char *format = "'fig'") const { std::stringstream s; s << "saveas(gcf," << filename << "," << format << ")\n"; print( s.str().c_str() ); }

    /// Save current figure
    void mysaveas(const char *filename = "", const char *format = "'fig'", const char *renderer = "") {
        set_paperpositionmode("'auto'");
        if ( strlen(renderer) != 0 )
            set_renderer(renderer);
        std::string filename_, format_;
        if ( strlen(format) == 0 ) {
            filename_ = get_filename(filename);
            format_ = get_format(filename);
        }
        else {
            std::stringstream s; s << filename;
            filename_ = s.str();
            std::stringstream ss; ss << format;
            format_ = ss.str();
        }
        if ( format_ == "'jpeg'")
            convert(filename_.c_str());
        if ( format_ == "'pdf'" )
            pdfcrop(filename_.c_str());
        saveas(filename_.c_str(),format_.c_str());
    }

    /// Clear command window
    void clc() { print("clc\n"); }

    /// Clear current figure window
    void clf() { print("clf\n"); }

    /// Remove items from workspace, freeing up system memory
    void clear(const char *str = "") const { std::stringstream s; s << "clear " << str << "\n"; print( s.str().c_str() ); }

    /// Remove specified figure
    void close(const char *str = "") const { std::stringstream s; s << "close " << str << "\n"; print( s.str().c_str() ); }

    /// Remove all variables, globals, functions and MEX links
    void clear_all() { clear("'all'"); }

    /// Remove all figures
    void close_all() { close("'all'"); }

    /// Remove all variables, globals, functions and MEX links
    /// Remove all figures
    void reset() {
        clear_all();
        close_all();
    }

    /// Stop and wait for you to press any key before continuing
    void wait() {
        print("pause\n");
        std::cin.get();
    }

    struct Disp {
        template<class TX>
        void operator() (const TX &x, FILE *tube) const { fprintf(tube,"%g,\n",double(x)); }
        template<class TX>
        void operator()(const TX &x, unsigned i, FILE *tube) const { fprintf(tube,"%g, %g,\n",double(i),double(x)); }
        template<class TX,class TY>
        void operator() (const TX &x, unsigned i, const TY &y, FILE *tube) const { fprintf(tube,"%g, %g,\n",double(x),double(y(x))); }
        template<class TX, class T, int s, class O>
        void operator() (const TX &x, unsigned i, const Vec<T,s,O> &y, FILE *tube) const { fprintf(tube,"%g, %g,\n",double(x),double(y[i])); }
        template<class TX, class TY, int sy, class OY, class TZ>
        void operator() (const TX &x, unsigned i, const Vec<TY,sy,OY> &y, const TZ &z, FILE *tube) const { fprintf(tube,"%g, %g, %g,\n",double(x),double(y[i]),double(z(x,y[i]))); }
        template<class TX, class TY, int sy, class OY, class TZ, int sz, class OZ>
        void operator() (const TX &x, unsigned i, const Vec<TY,sy,OY> &y, const Vec<TZ,sz,OZ> &z, FILE *tube) const { fprintf(tube,"%g, %g, %g,\n",double(x),double(y[i]),double(z[i])); }
    };

    /// Plot vector vec versus its index vector
    template<class T,int s,class O>
    void plot( const Vec<T,s,O> &vec, const char *params="" ) {
        fprintf(tube,"v = [\n");
        apply_wi( vec, Disp(), tube );
        fprintf(tube,"];\n");
        
        fprintf(tube,"plot( v(:,1), v(:,2)%s );\n",params);
        fflush(tube);
    }

    /// Plot vector x versus vector y
    template<class TX,int sx,class OX,class TY,int sy,class OY>
    void plot( const Vec<TX,sx,OX> &x, const Vec<TY,sy,OY> &y, const char *params="" ) {
        fprintf(tube,"v = [ \n");
        apply_wi( x, Disp(), y, tube );
        fprintf(tube," ];\n");
        
        fprintf(tube,"plot( v(:,1), v(:,2)%s );\n",params);
        fflush(tube);
    }

    /// Plot vector x versus vectors y1 and y2
    template<class TX,int sx,class OX,class TY,int sy,class OY>
    void plot( const Vec<TX,sx,OX> &x, const Vec<TY,sy,OY> &y1, const Vec<TY,sy,OY> &y2, const char *params="" ) {
        //         if ( hold ) {
        //             holded_data.push_back( HD( x, y1, params ) );
        //             holded_data.push_back( HD( x, y2, params ) );
        //         }
        //         else {
        //             fprintf(tube,"plot '-' %s, '-' %s\n",params,params);
        //             apply_wi( x, Disp(), y1, tube );
        //             fprintf(tube,"e\n");
        //             apply_wi( x, Disp(), y2, tube );
        //             fprintf(tube,"e\n");
        //             fflush(tube);
        //         }
        assert(0);
    }

    /// Log-log scale plot vector vec versus its index vector
    template<class T,int s,class O>
    void loglog( const Vec<T,s,O> &vec, const char *params="" ) {
        fprintf(tube,"v = [\n");
        apply_wi( vec, Disp(), tube );
        fprintf(tube,"];\n");

        fprintf(tube,"loglog( v(:,1), v(:,2)%s );\n",params);
        fflush(tube);
    }

    /// Log-log scale plot vector x versus vector y
    template<class TX,int sx,class OX,class TY,int sy,class OY>
    void loglog( const Vec<TX,sx,OX> &x, const Vec<TY,sy,OY> &y, const char *params="" ) {
        fprintf(tube,"v = [ \n");
        apply_wi( x, Disp(), y, tube );
        fprintf(tube," ];\n");

        fprintf(tube,"loglog( v(:,1), v(:,2)%s );\n",params);
        fflush(tube);
    }

    /// Log-log scale plot vector x versus vectors y1 and y2
    template<class TX,int sx,class OX,class TY,int sy,class OY>
    void loglog( const Vec<TX,sx,OX> &x, const Vec<TY,sy,OY> &y1, const Vec<TY,sy,OY> &y2, const char *params="" ) {
        //         if ( hold ) {
        //             holded_data.push_back( HD( x, y1, params ) );
        //             holded_data.push_back( HD( x, y2, params ) );
        //         }
        //         else {
        //             fprintf(tube,"loglog '-' %s, '-' %s\n",params,params);
        //             apply_wi( x, Disp(), y1, tube );
        //             fprintf(tube,"e\n");
        //             apply_wi( x, Disp(), y2, tube );
        //             fprintf(tube,"e\n");
        //             fflush(tube);
        //         }
        assert(0);
    }

    /// Semi-log scale plot vector vec versus its index vector
    template<class T,int s,class O>
    void semilogx( const Vec<T,s,O> &vec, const char *params="" ) {
        fprintf(tube,"v = [\n");
        apply_wi( vec, Disp(), tube );
        fprintf(tube,"];\n");

        fprintf(tube,"semilogx( v(:,1), v(:,2)%s );\n",params);
        fflush(tube);
    }

    /// Semi-log scale plot vector x versus vector y
    template<class TX,int sx,class OX,class TY,int sy,class OY>
    void semilogx( const Vec<TX,sx,OX> &x, const Vec<TY,sy,OY> &y, const char *params="" ) {
        fprintf(tube,"v = [ \n");
        apply_wi( x, Disp(), y, tube );
        fprintf(tube," ];\n");

        fprintf(tube,"semilogx( v(:,1), v(:,2)%s );\n",params);
        fflush(tube);
    }

    /// Semi-log scale plot vector x versus vectors y1 and y2
    template<class TX,int sx,class OX,class TY,int sy,class OY>
    void semilogx( const Vec<TX,sx,OX> &x, const Vec<TY,sy,OY> &y1, const Vec<TY,sy,OY> &y2, const char *params="" ) {
        //         if ( hold ) {
        //             holded_data.push_back( HD( x, y1, params ) );
        //             holded_data.push_back( HD( x, y2, params ) );
        //         }
        //         else {
        //             fprintf(tube,"semilogx '-' %s, '-' %s\n",params,params);
        //             apply_wi( x, Disp(), y1, tube );
        //             fprintf(tube,"e\n");
        //             apply_wi( x, Disp(), y2, tube );
        //             fprintf(tube,"e\n");
        //             fflush(tube);
        //         }
        assert(0);
    }

    /// Semi-log scale plot vector vec versus its index vector
    template<class T,int s,class O>
    void semilogy( const Vec<T,s,O> &vec, const char *params="" ) {
        fprintf(tube,"v = [\n");
        apply_wi( vec, Disp(), tube );
        fprintf(tube,"];\n");

        fprintf(tube,"semilogy( v(:,1), v(:,2)%s );\n",params);
        fflush(tube);
    }

    /// Semi-log scale plot vector x versus vector y
    template<class TX,int sx,class OX,class TY,int sy,class OY>
    void semilogy( const Vec<TX,sx,OX> &x, const Vec<TY,sy,OY> &y, const char *params="" ) {
        fprintf(tube,"v = [ \n");
        apply_wi( x, Disp(), y, tube );
        fprintf(tube," ];\n");

        fprintf(tube,"semilogy( v(:,1), v(:,2)%s );\n",params);
        fflush(tube);
    }

    /// Semi-log scale plot vector x versus vectors y1 and y2
    template<class TX,int sx,class OX,class TY,int sy,class OY>
    void semilogy( const Vec<TX,sx,OX> &x, const Vec<TY,sy,OY> &y1, const Vec<TY,sy,OY> &y2, const char *params="" ) {
        //         if ( hold ) {
        //             holded_data.push_back( HD( x, y1, params ) );
        //             holded_data.push_back( HD( x, y2, params ) );
        //         }
        //         else {
        //             fprintf(tube,"semilogy '-' %s, '-' %s\n",params,params);
        //             apply_wi( x, Disp(), y1, tube );
        //             fprintf(tube,"e\n");
        //             apply_wi( x, Disp(), y2, tube );
        //             fprintf(tube,"e\n");
        //             fflush(tube);
        //         }
        assert(0);
    }
    
    /// Create new figure window
    void figure() { print("figure\n"); }

    /// Make the figure h current, visible, and displayed on top of other figures
    /// If figure h does not exist, and h is an integer, a new figure is created with handle h
    template<class T>
    void figure(const T &h) const { std::stringstream s; s << "figure(" << to_string( h ) << ")\n"; print( s.str().c_str() ); }

    /// Make the figure h current, but do not change its visibility or stacking with respect to other figures
    template<class T>
    void set_current_figure(const T &h) { std::stringstream s; s << "set(0,'CurrentFigure'," << to_string( h ) << ");\n"; print( s.str().c_str() ); }

    /// Retain the current graph and add another graph to it
    void hold_on() { print("hold on\n"); }

    /// Reset hold state to the default behavior, in which MATLAB clears the existing graph and resets axes properties to their defaults before drawing new plots
    void hold_off() { print("hold off\n"); }

    /// Add major grid lines to the current axes
    void grid_on() { print("grid on\n"); }

    /// Remove all grid lines from the current axes
    void grid_off() { print("grid off\n"); }

    /// Display the boundary of the current axes
    void box_on() { print("box on\n"); }

    /// Do not display the boundary of the current axes
    void box_off() { print("box off\n"); }
    
    /// Enable the display of figure windows
    void display_on() { print("set(0,'DefaultFigureVisible','on');\n"); }

    /// Disable the display of figure windows
    void display_off() { print("set(0,'DefaultFigureVisible','off');\n"); }

    ///
    FILE *tube;
    bool display;

};

template<class T,int s,class O>
void ml_plot( const Vec<T,s,O> &vec, const char *params="" ) {
    MatlabPlot mp;
    mp.plot( vec, params );
    mp.wait();
}

template<class TX,int sx,class OX,class TY,int sy,class OY>
void ml_plot( const Vec<TX,sx,OX> &vecx, const Vec<TY,sy,OY> &vecy, const char *params="" ) {
    MatlabPlot mp;
    mp.plot( vecx, vecy, params );
    mp.wait();
}

template<class T,class STR,class STO>
void ml_plot( const Mat<T,STR,STO> &mat, const char *params="" ) {
    MatlabPlot mp;
    mp.plot( mat, params );
    mp.wait();
}

template<class T,int s,class O>
void ml_loglog( const Vec<T,s,O> &vec, const char *params="" ) {
    MatlabPlot mp;
    mp.loglog( vec, params );
    mp.wait();
}

template<class TX,int sx,class OX,class TY,int sy,class OY>
void ml_loglog( const Vec<TX,sx,OX> &vecx, const Vec<TY,sy,OY> &vecy, const char *params="" ) {
    MatlabPlot mp;
    mp.loglog( vecx, vecy, params );
    mp.wait();
}

template<class T,class STR,class STO>
void ml_loglog( const Mat<T,STR,STO> &mat, const char *params="" ) {
    MatlabPlot mp;
    mp.loglog( mat, params );
    mp.wait();
}

template<class T,int s,class O>
void ml_semilogx( const Vec<T,s,O> &vec, const char *params="" ) {
    MatlabPlot mp;
    mp.semilogx( vec, params );
    mp.wait();
}

template<class TX,int sx,class OX,class TY,int sy,class OY>
void ml_semilogx( const Vec<TX,sx,OX> &vecx, const Vec<TY,sy,OY> &vecy, const char *params="" ) {
    MatlabPlot mp;
    mp.semilogx( vecx, vecy, params );
    mp.wait();
}

template<class T,class STR,class STO>
void ml_semilogx( const Mat<T,STR,STO> &mat, const char *params="" ) {
    MatlabPlot mp;
    mp.semilogx( mat, params );
    mp.wait();
}

template<class T,int s,class O>
void ml_semilogy( const Vec<T,s,O> &vec, const char *params="" ) {
    MatlabPlot mp;
    mp.semilogx( vec, params );
    mp.wait();
}

template<class TX,int sx,class OX,class TY,int sy,class OY>
void ml_semilogy( const Vec<TX,sx,OX> &vecx, const Vec<TY,sy,OY> &vecy, const char *params="" ) {
    MatlabPlot mp;
    mp.semilogx( vecx, vecy, params );
    mp.wait();
}

template<class T,class STR,class STO>
void ml_semilogy( const Mat<T,STR,STO> &mat, const char *params="" ) {
    MatlabPlot mp;
    mp.semilogx( mat, params );
    mp.wait();
}

template<class T,int s,class O>
void save_ml_plot( const Vec<T,s,O> &vec, const char *output="", const char *xlabel="", const char *ylabel="", const char *params="", const char *interpreter="'latex'" ) {
    MatlabPlot mp;
    mp.cd_cwd();
    mp.display_off();
    mp.plot( vec, params );
    mp.grid_on();
    mp.box_on();
    mp.set_fontsize(16);
    mp.set_xlabel_interpreter(xlabel,interpreter);
    mp.set_ylabel_interpreter(ylabel,interpreter);
    std::string format = mp.get_format(output);
    if ( format == "'tex'" )
        mp.matlab2tikz(output,",'height','\\figureheight'");
    else {
        std::string filename = mp.get_filename(output);
        mp.mysaveas(filename.c_str(),format.c_str());
    }
    mp.close();
}

template<class TX,int sx,class OX,class TY,int sy,class OY>
void save_ml_plot( const Vec<TX,sx,OX> &vecx, const Vec<TY,sy,OY> &vecy, const char *output="", const char *xlabel="", const char *ylabel="", const char *params="", const char *interpreter="'latex'" ) {
    MatlabPlot mp;
    mp.cd_cwd();
    mp.display_off();
    mp.plot( vecx, vecy, params );
    mp.grid_on();
    mp.box_on();
    mp.set_fontsize(16);
    mp.set_xlabel_interpreter(xlabel,interpreter);
    mp.set_ylabel_interpreter(ylabel,interpreter);
    std::string format = mp.get_format(output);
    if ( format == "'tex'" )
        mp.matlab2tikz(output,",'height','\\figureheight'");
    else {
        std::string filename = mp.get_filename(output);
        mp.mysaveas(filename.c_str(),format.c_str());
    }
    mp.close();
}

template<class T,class STR,class STO>
void save_ml_plot( const Mat<T,STR,STO> &mat, const char *output="", const char *xlabel="", const char *ylabel="", const char *params="", const char *interpreter="'latex'" ) {
    MatlabPlot mp;
    mp.cd_cwd();
    mp.display_off();
    mp.plot( mat, params );
    mp.grid_on();
    mp.box_on();
    mp.set_fontsize(16);
    mp.set_xlabel_interpreter(xlabel,interpreter);
    mp.set_ylabel_interpreter(ylabel,interpreter);
    std::string format = mp.get_format(output);
    if ( format == "'tex'" )
        mp.matlab2tikz(output,",'height','\\figureheight'");
    else {
        std::string filename = mp.get_filename(output);
        mp.mysaveas(filename.c_str(),format.c_str());
    }
    mp.close();
}

template<class T,int s,class O>
void save_ml_loglog( const Vec<T,s,O> &vec, const char *output="", const char *xlabel="", const char *ylabel="", const char *params="", const char *interpreter="'latex'" ) {
    MatlabPlot mp;
    mp.cd_cwd();
    mp.display_off();
    mp.loglog( vec, params );
    mp.grid_on();
    mp.box_on();
    mp.set_fontsize(16);
    mp.set_xlabel_interpreter(xlabel,interpreter);
    mp.set_ylabel_interpreter(ylabel,interpreter);
    std::string format = mp.get_format(output);
    if ( format == "'tex'" )
        mp.matlab2tikz(output,",'height','\\figureheight'");
    else {
        std::string filename = mp.get_filename(output);
        mp.mysaveas(filename.c_str(),format.c_str());
    }
    mp.close();
}

template<class TX,int sx,class OX,class TY,int sy,class OY>
void save_ml_loglog( const Vec<TX,sx,OX> &vecx, const Vec<TY,sy,OY> &vecy, const char *output="", const char *xlabel="", const char *ylabel="", const char *params="", const char *interpreter="'latex'" ) {
    MatlabPlot mp;
    mp.cd_cwd();
    mp.display_off();
    mp.loglog( vecx, vecy, params );
    mp.grid_on();
    mp.box_on();
    mp.set_fontsize(16);
    mp.set_xlabel_interpreter(xlabel,interpreter);
    mp.set_ylabel_interpreter(ylabel,interpreter);
    std::string format = mp.get_format(output);
    if ( format == "'tex'" )
        mp.matlab2tikz(output,",'height','\\figureheight'");
    else {
        std::string filename = mp.get_filename(output);
        mp.mysaveas(filename.c_str(),format.c_str());
    }
    mp.close();
}

template<class T,class STR,class STO>
void save_ml_loglog( const Mat<T,STR,STO> &mat, const char *output="", const char *xlabel="", const char *ylabel="", const char *params="", const char *interpreter="'latex'" ) {
    MatlabPlot mp;
    mp.cd_cwd();
    mp.display_off();
    mp.loglog( mat, params );
    mp.grid_on();
    mp.box_on();
    mp.set_fontsize(16);
    mp.set_xlabel_interpreter(xlabel,interpreter);
    mp.set_ylabel_interpreter(ylabel,interpreter);
    std::string format = mp.get_format(output);
    if ( format == "'tex'" )
        mp.matlab2tikz(output,",'height','\\figureheight'");
    else {
        std::string filename = mp.get_filename(output);
        mp.mysaveas(filename.c_str(),format.c_str());
    }
    mp.close();
}

template<class T,int s,class O>
void save_ml_semilogx( const Vec<T,s,O> &vec, const char *output="", const char *xlabel="", const char *ylabel="", const char *params="", const char *interpreter="'latex'" ) {
    MatlabPlot mp;
    mp.cd_cwd();
    mp.display_off();
    mp.semilogx( vec, params );
    mp.grid_on();
    mp.box_on();
    mp.set_fontsize(16);
    mp.set_xlabel_interpreter(xlabel,interpreter);
    mp.set_ylabel_interpreter(ylabel,interpreter);
    std::string format = mp.get_format(output);
    if ( format == "'tex'" )
        mp.matlab2tikz(output,",'height','\\figureheight'");
    else {
        std::string filename = mp.get_filename(output);
        mp.mysaveas(filename.c_str(),format.c_str());
    }
    mp.close();
}

template<class TX,int sx,class OX,class TY,int sy,class OY>
void save_ml_semilogx( const Vec<TX,sx,OX> &vecx, const Vec<TY,sy,OY> &vecy, const char *output="", const char *xlabel="", const char *ylabel="", const char *params="", const char *interpreter="'latex'" ) {
    MatlabPlot mp;
    mp.cd_cwd();
    mp.display_off();
    mp.semilogx( vecx, vecy, params );
    mp.grid_on();
    mp.box_on();
    mp.set_fontsize(16);
    mp.set_xlabel_interpreter(xlabel,interpreter);
    mp.set_ylabel_interpreter(ylabel,interpreter);
    std::string format = mp.get_format(output);
    if ( format == "'tex'" )
        mp.matlab2tikz(output,",'height','\\figureheight'");
    else {
        std::string filename = mp.get_filename(output);
        mp.mysaveas(filename.c_str(),format.c_str());
    }
    mp.close();
}

template<class T,class STR,class STO>
void save_ml_semilogx( const Mat<T,STR,STO> &mat, const char *output="", const char *xlabel="", const char *ylabel="", const char *params="", const char *interpreter="'latex'" ) {
    MatlabPlot mp;
    mp.cd_cwd();
    mp.display_off();
    mp.semilogx( mat, params );
    mp.grid_on();
    mp.box_on();
    mp.set_fontsize(16);
    mp.set_xlabel_interpreter(xlabel,interpreter);
    mp.set_ylabel_interpreter(ylabel,interpreter);
    std::string format = mp.get_format(output);
    if ( format == "'tex'" )
        mp.matlab2tikz(output,",'height','\\figureheight'");
    else {
        std::string filename = mp.get_filename(output);
        mp.mysaveas(filename.c_str(),format.c_str());
    }
    mp.close();
}

template<class T,int s,class O>
void save_ml_semilogy( const Vec<T,s,O> &vec, const char *output="", const char *xlabel="", const char *ylabel="", const char *params="", const char *interpreter="'latex'" ) {
    MatlabPlot mp;
    mp.cd_cwd();
    mp.display_off();
    mp.semilogy( vec, params );
    mp.grid_on();
    mp.box_on();
    mp.set_fontsize(16);
    mp.set_xlabel_interpreter(xlabel,interpreter);
    mp.set_ylabel_interpreter(ylabel,interpreter);
    std::string format = mp.get_format(output);
    if ( format == "'tex'" )
        mp.matlab2tikz(output,",'height','\\figureheight'");
    else {
        std::string filename = mp.get_filename(output);
        mp.mysaveas(filename.c_str(),format.c_str());
    }
    mp.close();
}

template<class TX,int sx,class OX,class TY,int sy,class OY>
void save_ml_semilogy( const Vec<TX,sx,OX> &vecx, const Vec<TY,sy,OY> &vecy, const char *output="", const char *xlabel="", const char *ylabel="", const char *params="", const char *interpreter="'latex'" ) {
    MatlabPlot mp;
    mp.cd_cwd();
    mp.display_off();
    mp.semilogy( vecx, vecy, params );
    mp.grid_on();
    mp.box_on();
    mp.set_fontsize(16);
    mp.set_xlabel_interpreter(xlabel,interpreter);
    mp.set_ylabel_interpreter(ylabel,interpreter);
    std::string format = mp.get_format(output);
    if ( format == "'tex'" )
        mp.matlab2tikz(output,",'height','\\figureheight'");
    else {
        std::string filename = mp.get_filename(output);
        mp.mysaveas(filename.c_str(),format.c_str());
    }
    mp.close();
}

template<class T,class STR,class STO>
void save_ml_semilogy( const Mat<T,STR,STO> &mat, const char *output="", const char *xlabel="", const char *ylabel="", const char *params="", const char *interpreter="'latex'" ) {
    MatlabPlot mp;
    mp.cd_cwd();
    mp.display_off();
    mp.semilogy( mat, params );
    mp.grid_on();
    mp.box_on();
    mp.set_fontsize(16);
    mp.set_xlabel_interpreter(xlabel,interpreter);
    mp.set_ylabel_interpreter(ylabel,interpreter);
    std::string format = mp.get_format(output);
    if ( format == "'tex'" )
        mp.matlab2tikz(output,",'height','\\figureheight'");
    else {
        std::string filename = mp.get_filename(output);
        mp.mysaveas(filename.c_str(),format.c_str());
    }
    mp.close();
}

}

#endif // LMT_matlabplot_HEADER
