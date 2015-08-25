//
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
#ifndef LMT_matlabplot_HEADER
#define LMT_matlabplot_HEADER

#include "mat.h"
#include <sstream>

namespace LMT {

class MatlabPlot {
public:
    MatlabPlot() {
        if ((tube=popen("matlab -nodesktop","w"))==NULL) {
            std::cerr << "matlab impossible à ouvrir" << std::endl;
            exit(1);
        }
    }
    ~MatlabPlot() {
        pclose(tube);
    }
    /// 
    void print(const char *str) {
        fprintf(tube,str);
        fflush(tube);
    }
    /// Pour effacer les courbes d'avant
    void reset() {
        print("clear all\n\n");
        print("close all\n\n");
    }
    /// Wait for a key
    void wait() {
        print("pause -1\n\n");
        std::cin.get();
    }
    struct Disp {
        template<class TX,class TY> void operator()(const TY &val,const TX &x,FILE *tube) const { fprintf(tube,"%10.6f, %10.6f,\n",(double)x,(double)val); }
        template<class TX,class TY> void operator()(const TX &val,unsigned i,const TY &y,FILE *tube) const { fprintf(tube,"%10.6f, %10.6f,\n",(double)val,(double)y[i]); }
    };
    /// Dans le cas où une seule variable est fournie. x=numéro. vec=ordonnée
    template<class T,int s,class O>
    void plot(const Vec<T,s,O> &vec,const char *params="") {
        fprintf(tube,"v = [\n");
        apply_wi( vec, Disp(), tube );
        fprintf(tube,"];\n");
        
        fprintf(tube,"plot( v(:,1), v(:,2)%s );\n",params);
        fflush(tube);
    }
    /// Dans le cas où une seule variable est fournie. x=numéro. vec=ordonnée
    template<class T1,int s1,class O1,class T2,int s2,class O2>
    void plot(const Vec<T1,s1,O1> &x,const Vec<T2,s2,O2> &y,const char *params="") {
        fprintf(tube,"v = [ \n");
        apply_wi( x, Disp(), y, tube );
        fprintf(tube," ];\n");
        
        fprintf(tube,"plot( v(:,1), v(:,2)%s );\n",params);
        fflush(tube);
    }
    /// Dans le cas où une seule variable est fournie. x=numéro. vec=ordonnée
    template<class T1,int s1,class O1,class T2,int s2,class O2>
    void plot(const Vec<T1,s1,O1> &x,const Vec<T2,s2,O2> &y1,const Vec<T2,s2,O2> &y2,const char *params="") {
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
    
    ///
    void hold_on () { fprintf(tube,"hold on;\n"); }
    ///
    void hold_off() { fprintf(tube,"hold off;\n"); }
    
    ///
    FILE *tube;
};
/** plot and wait
*/
template<class T,int s,class O>
void ml_plot(const Vec<T,s,O> &vec,const char *params="") {
    MatlabPlot gp;
    gp.plot(vec,params);
    gp.wait();
}

/** plot and wait
*/
template<class T,class STR,class STO>
void ml_plot(const Mat<T,STR,STO> &mat,const char *params="") {
    MatlabPlot gp;
    gp.plot(mat,params);
    gp.wait();
}

};

#endif // LMT_matlabplot_HEADER
