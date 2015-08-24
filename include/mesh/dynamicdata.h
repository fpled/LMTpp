//
// C++ Interface: %{MODULE}
//
// Description: 
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMT_DYNAMIC_DATA_HEADER
#define LMT_DYNAMIC_DATA_HEADER

#include "element.h"
#include "node.h"

namespace LMT {

///
class DynamicDataAncestor {
public:
    DynamicDataAncestor(const std::string &n):name(n) {}
    virtual ~DynamicDataAncestor() {}
    
    
    const std::string &get_name() const { return name; }
    virtual unsigned nb_comp() const = 0;
    virtual void write_data(std::ostream &os) const = 0;
    virtual void pop_back(unsigned n) = 0;
protected:
    virtual void resize(unsigned num_type,unsigned i) = 0;
    virtual void free() = 0;
    virtual void pop_back_and_copy_to(unsigned num_type,unsigned i) = 0;
    virtual void push_back(unsigned num_type) = 0;
    std::string name;
    
    template<class T_,int s_,class o_> friend class Vec;
};


/*!
Liste de type T associée à des noeuds.

Ça permet de faire comme un nouvel attribut par élément qui sera géré à l'affichage et pendant les sauvegardes (parce que si vous faites votre vecteur dans votre coin, rien ne dit aux procédures comme write_vtk() qu'il faudra l'enregistrer).

Exemple d'utilisation :

\code C/C++
    TM mesh;
    DynamicData<double> data( "nom de liste pour paraview ou pour l'affichage en général" );
    m.node_list.reg_dyn( data ); // pour donner la bonne taille aux vecteurs
    data[ node ] = 10;

    display( mesh ); // sauve aussi votre vecteur

Si vous voulez enlever le lien m.elem_list.unreg_dyn( data );
*/
template<class T,unsigned nb_elem_type=0>
class DynamicData : public DynamicDataAncestor {
public:
    DynamicData(const std::string &n) : DynamicDataAncestor(n) {}
    ///    
    T &get_data(unsigned num_type,unsigned i) { return vec[num_type][i]; }
    const T &get_data(unsigned num_type,unsigned i) const { return vec[num_type][i]; }
    ///
    //template<class TNode> T &get_data(const ElementAncestor<TNode> &e) { return vec[e.num_in_elem_list_virtual()][e.number]; }
    //template<class TNode> const T &get_data(const ElementAncestor<TNode> &e) const { return vec[e.num_in_elem_list_virtual()][e.number]; }
    ///
    //template<class NE,class NB,class TN,class TD,unsigned nel> T &get_data(const Element<NE,NB,TN,TD,nel> &e) { return vec[nel][e.number]; };
    //template<class NE,class NB,class TN,class TD,unsigned nel> const T &get_data(const Element<NE,NB,TN,TD,nel> &e) const { return vec[nel][e.number]; };
    
    void set_values( const T &v ) {
        for(unsigned i=0;i<nb_elem_type;++i)
            vec[i].set( v );
    }
    
    unsigned size(unsigned num_type) const { return vec[num_type].size(); }
    virtual unsigned nb_comp() const { return DM::NbComponents<T>::n; }
    virtual void write_data(std::ostream &os) const {
        for(unsigned i=0;i<nb_elem_type;++i)
            for(unsigned j=0;j<vec[i].size();++j)
                os << vec[i][j] << " ";
    }
    virtual void pop_back(unsigned n) { assert(0); }
protected:
    virtual void resize(unsigned num_type,unsigned i) { vec[num_type].resize(i); }
    virtual void free() { for(unsigned i=0;i<nb_elem_type;++i) vec[i].free(); }
    virtual void pop_back_and_copy_to(unsigned num_type,unsigned i) { vec[num_type][i]=vec[num_type].back(); vec[num_type].pop_back(); }
    virtual void push_back(unsigned num_type) { vec[num_type].resize(vec[num_type].size()+1); }
    Vec<T> vec[nb_elem_type];
};

/*!
Liste de type T associée à des éléments.

Ça sert de (nouveau) vecteur pour les éléments sauf qu'on ne peut pas faire vec[ elem.number ] parce que number c'est dans la sous-liste d'élément...

Ça permet de faire comme un nouvel attribut par élément.

Exemple d'utilisation :

\code C/C++
    TM mesh;
    DynamicData<double,TM::TElemList::nb_elem_type> data( "nom de liste pour paraview ou pour l'affichage en général" );
    ... avec elem qui est un élément du maillage
    m.elem_list.reg_dyn( data ); // pour donner la bonne taille aux vecteurs
    data[ elem ] = 10;

Si vous voulez enlever le lien m.elem_list.unreg_dyn( data );
*/
template<class T>
class DynamicData<T,0> : public DynamicDataAncestor {
public:
    DynamicData(const std::string &n) : DynamicDataAncestor(n) {}
    /// assumes that nb_elem_type==1
    T &get_data(unsigned i) { return vec[i]; }
    const T &get_data(unsigned i) const { return vec[i]; }
    ///
    //template<unsigned d,class TN,class TND> T &get_data(const Node<d,TN,TND> &n) { return vec[n.number]; }
    //template<unsigned d,class TN,class TND> const T &get_data(const Node<d,TN,TND> &n) const { return vec[n.number]; }
    
    unsigned size() const { return vec.size(); }
    virtual unsigned nb_comp() const { return DM::NbComponents<T>::n; }
    virtual void write_data(std::ostream &os) const {
        for(unsigned i=0;i<vec.size();++i)
            os << vec[i] << " ";
    }
    virtual void pop_back(unsigned n) { vec.pop_back(n); }
protected:
    virtual void resize(unsigned num_type,unsigned i) { vec.resize(i); }
    virtual void free() { vec.free(); }
    virtual void pop_back_and_copy_to(unsigned num_type,unsigned i) { vec[i]=vec.back(); vec.pop_back(); }
    virtual void push_back(unsigned num_type) { vec.resize(vec.size()+1); }
    Vec<T> vec;
};

inline std::ostream &operator<<(std::ostream &os,const DynamicDataAncestor &dd) {
    dd.write_data(os);
    return os;
}

};

#endif // LMT_DYNAMIC_DATA_HEADER

