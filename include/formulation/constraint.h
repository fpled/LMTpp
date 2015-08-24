//
// C++ Interface: constraint
//
// Description: 
//
//
// Author: Hugo LECLERC <hugo.leclerc@lmt.ens-cachan.fr>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMT_constraint_HEADER
#define LMT_constraint_HEADER

#include "../codegen/codegen.h"

namespace LMT {

/**
 * constraint in formulation. Can represent a penalty or a lagrangian constraint.
 */
template<class TF>
class Constraint {
public:
    typedef typename TF::ScalarType T;
    static const int type_var_nodal = -1;
    static const int type_var_global = -2;
    struct Coeff {
        Coeff():num_in_vec(0) {}
        Coeff(Codegen::Ex val_,int type_var_,unsigned num_,unsigned num_in_vec_=0):type_var(type_var_),num(num_),num_in_vec(num_in_vec_),val(val_) {}
        int type_var; /// -1 -> nodal, -2 -> global, n -> elementary (elem type n)
        unsigned num;  /// num of node or element
        unsigned num_in_vec;  /// in case of the unknown is a vector or formulation contain several unknowns
        Codegen::Ex val; /// coeff
        Codegen::Ex sym; /// symbol
        unsigned num_in_fmat(const TF &f) const {
            if ( type_var==-2 )
                return ( *f.indice_glob ) + num_in_vec;
            if ( type_var==-1 )
                return ( *f.indice_noda )[ num ] + num_in_vec;
            return f.indice_elem[ type_var ][ num ] + num_in_vec;
        }
    };
    
    Constraint():res(0.0) { }
    Constraint( const std::string &txt_, const T &_penalty_value, const TF &f ) {
        penalty_value = _penalty_value;

        std::string txt;
        for( int i = 0; i < txt_.size(); ++i )
           if ( txt_[ i ] != ' ' )
               txt += txt_[ i ];

        using namespace Codegen;
        try {
            symbols.clear();
            Ex ex = read_ex( txt, symbols );
            for(unsigned i=0;i<symbols.size();++i) {
                std::string str = symbols[ i ].to_string();
                Vec<std::string> split = tokenize( str, '.' );
                
                std::string name_unk_ns = split[split.size()-1];
                Vec<std::string> split_nu = tokenize( name_unk_ns, '[' );
                std::string name_unk = split_nu[0];
                
                if ( f.carac.is_unknown( name_unk ) ) {
                    Coeff c;
                    c.sym = symbols[ i ];
                    if ( split.size()==0 )
                        c.type_var = -2;
                    else if ( split[0][0]=='n' ) {
                        c.type_var = -1;
                        Vec<std::string> splitpar = tokenize( split[0], '[' );
                        if ( splitpar.size()!=2 or splitpar[1][splitpar[1].size()-1]!=']' )
                            throw Read_ex_error("syntax for node data is node[0].data.",0);
                        c.num = (unsigned)atoi( std::string(splitpar[1].begin(),splitpar[1].begin()+splitpar[1].size()-1).c_str() );
                    } else {
                        c.type_var = 0;
                    }
                    c.val = ex.diff( symbols[i] );
                    if ( split_nu.size() != 1 )
                        c.num_in_vec = (unsigned)atoi( std::string(split_nu[1].begin(),split_nu[1].begin()+split_nu[1].size()-1).c_str() );
                    //
                    c.num_in_vec += f.carac.num_in_vec_unknown( name_unk );

                    coeffs.push_back( c );
                    
                    ex = ex.subs( symbols[i], 0 );
                }
            }
            res = - ex;
        } catch(const Read_ex_error &e) { std::cerr << "Error while reading constraint " << txt << " : " << e.what() << std::endl; }
    }
    
    T penalty_value;
    Codegen::Ex res; /// sollicitation
    Vec<Coeff> coeffs;
    std::vector<Codegen::Ex> symbols;
};

template<class TF> 
std::ostream &operator<<( std::ostream &s, const Constraint<TF> &c ) {
    for(unsigned i=0;i<c.coeffs.size();++i)
        s << c.coeffs[i].val << " * unk[" << c.coeffs[i].type_var << "," << c.coeffs[i].num << "," << c.coeffs[i].num_in_vec << "]"
          << (i<c.coeffs.size()-1 ? " + " : "" );
    s << " == " << c.res;
    return s;
}

};

#endif // LMT_constraint_HEADER
