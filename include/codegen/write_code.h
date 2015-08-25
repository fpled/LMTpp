#ifndef WRITE_CODE_H
#define WRITE_CODE_H

#include "ex.h"

#ifdef METIL_COMP_DIRECTIVE
#pragma src_file write_code.cpp
#endif

namespace Codegen {

class Write_code_language;

/*!

La classe Write_code permet de générer le code C++ du corps d'une fonction. On ajoute du code à l'aide de la fonction add() puis à la fin on appelle la fonction to_string() qui renvoie le code dans une chaine de caractères. 
Voici un exemple de code :
\code C/C++
    #include <containers/mat.h>
    #include <codegen/ex.h>
    #include <codegen/write_code.h>
    using namespace LMT;
    using namespace Codegen;
    int main() {
        Ex R = symbol("R"), S(0);
        for(int i=0;i<2;i++) {
            R /= 2;
            S += M_PI * R * R;
        }
        PRINT( S );
        //  
        Write_code wc("float");
        wc.add( S,"",Write_code::Return);
        std::cout << wc.to_string() << std::endl;
    
        return 0;
    }

avec le fichier SConstruct :
\code Python
    from LMT import *
    env = Environment(
        CPPPATH = [ '#LMT/include' ],
        LIBS = [ 'pthread' ],
        CPPFLAGS = cppflags( ['xml2-config'] ),
        LINKFLAGS = linkflags( ['xml2-config'] )
    )
    make_dep_py(env)

    env.BuildDir( 'build/LMT', 'LMT/include', duplicate=0 )
    libs = SConscript( 'LMT/include/SConscript', exports='env', build_dir='build/LMT' )
    
    env.Program( "test", ["test.cpp"] + libs )

et le fichier Makefile :
\code
    all: codegen
        scons
        ./test
    codegen:
        cd LMT/include/codegen; scons

<strong> REMARQUE : </strong> n'oubliez pas la ligne wc.add(tmp,"R",Write_code.Return); pour le retour de la fonction.

Les deux fonctions utiles sont add() pour ajouter du code. Son premier paramètre est l'expression, le deuxième est le nom de la variable et le troisième est le type d'"affectation". On  a le choix entre :
    * <strong> Declare </strong> pour une déclaration,
    * <strong> Set </strong> pour une affectation,
    * <strong> Add </strong> pour l'opération +=,
    * <strong> Sub </strong> pour l'opération -=,
    * <strong> Return </strong> pour le retour de la fonction ( return ).  


\friend samir.amrouche@lmt.ens-cachan.fr
\friend hugo.leclerc@lmt.ens-cachan.fr
\relates Ex
*/
class Write_code {
public:
    ///
    typedef enum {Declare,Set,Add,Sub,Return} Method;
    ///
    struct Var {
        Ex ex;
        std::string name;
        Method method;
    };
    /// beware : destructor of Write_code will delete cl.
    Write_code(const char *default_type="T",unsigned nb_spaces=4);
    ~Write_code();
    /// add ex to var that has to be calculated.
    /// update nodes and leaves.
    Write_code &add( const Ex &ex, const std::string &name, Method method=Declare );
    std::string to_string();    
    unsigned node_count() const;
    
    std::string to_asm( std::string function_name="" );
    std::string asm_caller( std::string asm_function_name );
    
    std::vector<Var> lst_var;
    
    void write_graphviz(std::ostream &os);
    int display_graphviz();

private:
    struct Node {
        Node():reg(-2) {}
        Ex ex;
        unsigned nb_times_to_be_used;
        unsigned depth;
        int reg;
    };
    struct SortNode {
        bool operator()(const Node *a,const Node *b) const { return a->depth > b->depth; }
    };
    typedef std::map<Ex,Node,Ex::ExMapCmp> Tnodes;
    typedef std::set<Node *> Tleaves;
    Tnodes nodes;
    Tleaves leaves;
    
    std::vector<Node *> node_order;
    
    Write_code_language *wcl;
    
    const Op *set_depth_rec(const Ex &ex,unsigned depth); /// find max depth for each sub nodes ( results in nodes ). Only used via add()
     
    Tleaves::iterator get_next_node();
    std::string ex_to_string(Node &node) const; /// use node_order to get output
    void set_node_order();
};

void write_graphviz(const Op *op,std::ostream &os);

};

#endif // WRITE_CODE_H
