//
// C++ Interface: sollicitation
//
// Description: 
//
//
// Author: Hugo LECLERC <hugo.leclerc@lmt.ens-cachan.fr>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMT_sollicitation_HEADER
#define LMT_sollicitation_HEADER

namespace LMT {

/**
 * sollicitation in formulation. Can represent a penalty or a lagrangian constraint.
 */
template<class TF>
class Sollicitation {
public:
    typedef typename TF::ScalarType T;
    static const int type_var_nodal = -1;
    static const int type_var_global = -2;
    
    Sollicitation():val(0.0) { }
    Sollicitation(int type_var_,const std::string &txt,unsigned nb_in_type_,unsigned num_in_vec_) {
        type_var = type_var_;
        nb_in_type = nb_in_type_;
        num_in_vec = num_in_vec_;
        using namespace Codegen;
        try {
            symbols.clear();
            val = read_ex( txt, symbols );
        } catch(const Read_ex_error &e) { std::cerr << "Error while reading sollicitation " << txt << " : " << e.what() << std::endl; }
    }
    
    unsigned num_in_fmat(const TF &f) const {
        if ( type_var==-2 ) {
            return *f.indice_glob + num_in_vec;
        }
        else if ( type_var==-1 ) {
            return (*f.indice_noda)[ nb_in_type ] + num_in_vec;
        }
        else {
            return f.indice_elem[ type_var ][ nb_in_type ] + num_in_vec;
        }
    }
    
    Codegen::Ex val; /// sollicitation
    std::vector<Codegen::Ex> symbols;
    int type_var; /// -1 -> nodal, -2 -> global, n -> elementary (elem type n)
    unsigned nb_in_type, num_in_vec; /// num node if type nodal
};


};

#endif // LMT_sollicitation_HEADER
