//
// C++ Interface: meshancestor
//
// Description: 
//
//
// Author: Hugo LECLERC <leclerc@lmt.ens-cachan.fr>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMT_meshancestor_HEADER
#define LMT_meshancestor_HEADER

#include "../containers/vec.h"
#include "../containers/packedvectorofvector.h"
#include "element.h"
#include "meshdata.h"
#include "node.h"
#include "dynamicdata.h"
#include "nodelist.h"
#include "meshcarac.h"
#include "elemlist.h"
#include <algorithm>

/*!
    \brief Classes et Fonctions utiliser pour définir ou travailler sur des maillages
    \author Hugo LECLERC
    
    Pour utiliser la classe maillage il est nécessaire d'inclure le fichier mesh/mesh.h lors de la déclaration d'un maillage.
    \code C/C++
        #include "mesh/mesh.h" 
    \keyword 
*/

/*!
    \brief Classe générique pour la déclaration d'un maillage.
    
    La classe Mesh est paramétrable par une classe de caractéristiques définie et généré à partir du fichier SConstruct.
    
    Dans tous les exemples le maillage est noté m.
    Supposons que la classe Mesh_carac_pb_elast existe, on définira le maillage par :
    \code C/C++
        Mesh<Mesh_carac_pb_elast<2,double> > m;
    On pourra poser :
    \code C/C++
        typedef Mesh<Mesh_carac_pb_elast<2,double> > TM ;
        TM m;
    \keyword Maillage
*/

namespace LMT {


template<class Carac_,unsigned nvi_to_subs,unsigned skin>
class MeshAncestor : public Carac_::GlobalStaticData {
public:
    typedef Carac_ Carac;
    static const unsigned dim = Carac::dim;
    typedef typename Carac::Tpos Tpos;
    typedef Vec<Tpos,dim> Pvec;
    typedef typename Carac::NodalStaticData NodalStaticData;
    typedef typename Carac::GlobalStaticData GlobalStaticData;
    typedef Node<dim,Tpos,NodalStaticData> TNode;
    typedef ElementAncestor<TNode> EA;
    typedef Vec<VecNodeList<TNode,skin> > TNodeList;
    typedef Vec<VecElemList<Carac,TNode,nvi_to_subs,skin> > TElemList;
    typedef typename TElemList::TListPtr TElemListPtr;
    template<class NE,class BE=DefaultBehavior> struct TElem { typedef typename TElemList::template TElem<NE,BE>::TE TE; };
    static const unsigned nvi = TElemList::nvi;
    /*!
    Type utilisé seulement pour add_element().
    Il ne fait pas que transformer TE en TE*. lorsque l'utilisateur de add_element() souhaite ajouter un élément dont ne nom n'est pas dans le MeshCarac du maillage, add_element() ne renvoie pas un void* mais un ElementAncestor<TNode>* ( mais la valeur du pointeur reste NULL ).
    */
    template<class NE,class BE=DefaultBehavior> struct TElemPtr { 
        typedef typename TElem<NE,BE>::TE TE_;
        typedef typename AlternativeOnType< AreSameType<void, TE_>::res, TE_, EA >::T* TE;
    };
    
    MeshAncestor() {
        date_last_connectivity_change = 1;
        date_last_node_neighbours_update = 0;
        date_last_node_parents_update = 0;
        date_last_elem_neighbours_update = 0;
        date_last_absolute_number_update = 0;
        for(unsigned i=0;i<TElemList::nb_elem_type;++i) cpt_elem[i] = 0;
        wanted_hash_size = 1024;
    }

    /// 
    void update_node_neighbours();
    /// 
    void clear_node_neighbours() { node_neighbours.free(); --date_last_node_neighbours_update; }
    /// get nodes connected through elements to num_node. Result is sorted in ascending order.
    SimpleConstIterator<TNode *> get_node_neighbours(unsigned num_node) { return node_neighbours[num_node]; }
    SimpleConstIterator<const TNode *>  get_node_neighbours(unsigned num_node) const { return node_neighbours[num_node]; }
    /// 
    void update_node_parents();
    /// 
    void clear_node_parents() { node_parents.free(); --date_last_node_parents_update; }
    /// get elements connected to node
    SimpleConstIterator<EA *> get_node_parents(unsigned num_node) { return node_parents[num_node]; }
    SimpleConstIterator<const EA *> get_node_parents(unsigned num_node) const { return node_parents[num_node]; }
    /// 
    void update_elem_neighbours();
    /// 
    void clear_elem_neighbours() { for(unsigned i=0;i<TElemList::nb_elem_type;++i) elem_neighbours[i].free(); --date_last_elem_neighbours_update; }
    /// get elements connected (directly) to element
    template<class TE> SimpleConstIterator<EA *> get_elem_neighbours(const TE &e) { return elem_neighbours[TE::num_in_elem_list][e.number]; }
    template<class TE> SimpleConstIterator<const EA *> get_elem_neighbours(const TE &e) const { return elem_neighbours[TE::num_in_elem_list][e.number]; }
    SimpleConstIterator<EA *> get_elem_neighbours_EA(const EA *e) { return elem_neighbours[e->num_in_elem_list_virtual()][e->number]; }
    SimpleConstIterator<const EA *> get_elem_neighbours_EA(const EA *e) const { return elem_neighbours[e->num_in_elem_list_virtual()][e->number]; }
    
    struct GetNodeAdresses { template<class N> void operator()(N &n,TNode** &res) const { *(res++) = &n; } };
    void get_nodes_adresses(TNode **res) { apply( this->node_list, GetNodeAdresses(), res ); } /// fill vec with adresses of nodes
    struct ChangeNodePointerInElems {
        template<class TE> void operator()(TE &e,TNode **cor) const {
             for(unsigned i=0;i<TE::nb_nodes;++i) e.nodes[i] = cor[e.nodes[i]->number_in_original_mesh()];
        }
    }; ///
    void change_node_ptr_in_elements(TNode **correspondance) {
        apply( this->elem_list, ChangeNodePointerInElems(), correspondance );
        //for(unsigned i=0;i<this->node_list.size();++i)
        //    this->node_list[i].number = i;
    } /// used by remove_double (as example)
    
    struct GetTypeElements {
        template<class TE> void operator()(const StructForType<TE> &st,Vec<std::string> &res) const {
            res.push_back( TE::name() );
        }
    };
    Vec<std::string> type_elements() const {
        Vec<std::string> res;
        elem_list.apply_static( GetTypeElements(), res );
        return res;
    }

    /// return number of node at pos $pos ( in fact the first point in the sphere around pos, with radius tolerance )
    int poin( const Pvec &pos, Tpos tolerance = 1e-6 ) const {
        for(unsigned k=0; k<node_list.size(); ++k)
            if ( norm_2( node_list[k].pos - pos ) < tolerance )
                return k;
        return -1;
    }
    
    ///
    void free() {
        this->clear_elements();
        this->clear_node_neighbours();
        this->clear_node_parents();
        this->clear_elem_neighbours();
    }
    
    /// without update
    unsigned get_absolute_number( const EA *e ) const {
        unsigned r = 0;
        for(unsigned i=0;i<e->num_in_elem_list_virtual();++i)
            r += cpt_elem[ i ];
        return r + e->number;
    }


private:
    struct AbsoluteNumberUpdate { template<class TE> void operator()( TE &e, unsigned &n ) const { e.absolute_number = n++; } };
   
    struct FindMinNodeDist {
        template<class TE> void operator()( const TE &e, Tpos &min_node_dist ) const {
            for(unsigned i=0;i<TE::nb_nodes;++i)
                for(unsigned j=i+1;j<TE::nb_nodes;++j)
                    min_node_dist = min( min_node_dist, norm_2( e.pos( i ) - e.pos( j ) ) );
        }
    };
    
public:
    
    /// min dist between nodes
    Tpos min_node_dist() const {
        Tpos min_node_dist = std::numeric_limits<Tpos>::max();
        apply( elem_list, FindMinNodeDist(), min_node_dist );
        return min_node_dist;
    }
    
    /// 
    void absolute_number_update() {
        if ( date_last_absolute_number_update == date_last_connectivity_change )
            return;
        date_last_absolute_number_update = date_last_connectivity_change;
        unsigned cpt = 0;
        apply( elem_list, AbsoluteNumberUpdate(), cpt );
    }

    /// surdefined by children
    void update_skin() {}


    ///    
    TNodeList node_list;
    ///    
    TElemList elem_list;
    ///
    unsigned wanted_hash_size;
protected:
    ///
    template<class NE,class BE,unsigned num_index>
    typename TElemPtr<NE,BE>::TE add_element_(const NE &ne,const BE &be,TNode **n,const Number<num_index> &nn) {
    //typename TElem<NE,BE>::TE *add_element_(const NE &ne,const BE &be,TNode **n,const Number<num_index> &nn) {
        typedef typename TElem<NE,BE>::TE TE;
        TE *res = elem_list.hp.new_elem(StructForType<TE>()); // template 
        for(unsigned i=0;i<NE::nb_nodes;++i) res->nodes[i] = n[i];
        res->number = cpt_elem[ TE::num_in_elem_list ]++;
        elem_list.push_back_dyn(TE::num_in_elem_list);
        return res;
    }
    ///
    template<class NE,class BE>
    typename TElemPtr<NE,BE>::TE add_element_(const NE &ne,const BE &be,TNode **n,const Number<IndexOfMagicValue::res> &nn) {
    //typename TElem<NE,BE>::TE *add_element_(const NE &ne,const BE &be,TNode **n,const Number<IndexOfMagicValue::res> &nn) {
        std::cerr << NE::name() << " with behavior cannot be included in elem_list." << std::endl;
        return (typename TElemPtr<NE,BE>::TE )NULL;
    }
    ///
    template<class NE,class BE>
    typename TElemPtr<NE,BE>::TE add_element_(const NE &ne,const BE &be,TNode **n) {
//     typename TElem<NE,BE>::TE *add_element_(const NE &ne,const BE &be,TNode **n) {
        return add_element_(ne,be,n,LMT::Number<TElemList::template IndexOf<NE,BE>::res>());
    }
//     template<class NE,class BE,unsigned num_index>
//     typename TElem<NE,BE>::TE *add_element_(const NE &ne,const BE &be,TNode **n,const Number<num_index> &nn) {
//         typedef typename TElem<NE,BE>::TE TE;
//         TE *res = elem_list.hp.new_elem(StructForType<TE>()); // template 
//         for(unsigned i=0;i<NE::nb_nodes;++i) res->nodes[i] = n[i];
//         res->number = cpt_elem[ TE::num_in_elem_list ]++;
//         elem_list.push_back_dyn(TE::num_in_elem_list);
//         return res;
//     }
//     ///
//     template<class NE,class BE>
//     typename TElem<NE,BE>::TE *add_element_(const NE &ne,const BE &be,TNode **n,const Number<IndexOfMagicValue::res> &nn) {
//         std::cerr << NE::name() << " with behavior cannot be included in elem_list." << std::endl;
//         return (typename TElem<NE,BE>::TE *)NULL;
//     }
//     ///
//     template<class NE,class BE> typename TElem<NE,BE>::TE *add_element_(const NE &ne,const BE &be,TNode **n) {
//         return add_element_(ne,be,n,LMT::Number<TElemList::template IndexOf<NE,BE>::res>());
//     }
    ///
    void clear_elements() {
        this->elem_list.free();
        for(unsigned i=0;i<TElemList::nb_elem_type;++i)
            cpt_elem[i] = 0;
    }
    ///
    unsigned date_last_connectivity_change, 
             date_last_node_neighbours_update, 
             date_last_node_parents_update, 
             date_last_elem_neighbours_update,
             date_last_absolute_number_update;

    ///    
    PackedVectorOfVector<TNode *> node_neighbours;
    ///    
    PackedVectorOfVector<EA *> node_parents;
    ///    
    PackedVectorOfVector<EA *> elem_neighbours[TElemList::nb_elem_type+(TElemList::nb_elem_type==0)];
    ///
    unsigned cpt_elem[TElemList::nb_elem_type+(TElemList::nb_elem_type==0)];
    
    struct AppendNodeNodeCon;
    struct ResizeNodeConElem;
};



// ------------------------------------------------------------------------------------------
template<class Carac,unsigned nvi_to_subs,unsigned skin>
struct MeshAncestor<Carac,nvi_to_subs,skin>::AppendNodeNodeCon {
    template<class TE,class TM> void operator()(TE &e,const TM &m) {
        for(unsigned i=0;i<TE::nb_nodes;++i) {
            unsigned ni = m.node_list.number(*e.node(i));
            for(unsigned j=0;j<TE::nb_nodes;++j)
                if ( i!=j )
                    vv[ni].push_back_in_reserved( e.node(j) );
        }
    }
    Vec<Vec<TNode *> > vv;
};
template<class Carac,unsigned nvi_to_subs,unsigned skin>
struct MeshAncestor<Carac,nvi_to_subs,skin>::ResizeNodeConElem {
    template<class TE,class TM> void operator()(const TE &e,const TM &m) {
        for(unsigned i=0;i<TE::nb_nodes;++i) res[ m.node_list.number(*e.node(i)) ] += TE::nb_nodes-1;
    }
    Vec<unsigned> res;
};

template<class Carac,unsigned nvi_to_subs,unsigned skin>
void MeshAncestor<Carac,nvi_to_subs,skin>::update_node_neighbours() {
    if ( date_last_node_neighbours_update == date_last_connectivity_change )
        return;
    date_last_node_neighbours_update = date_last_connectivity_change;
    
    // get a first approx of nb nodes connected to each node
    ResizeNodeConElem gne;
    gne.res.resize( node_list.size(), (unsigned)0 );
    apply(elem_list, gne,*this);

    // get connections (neither sorted nor unique)
    AppendNodeNodeCon amc;
    amc.vv.resize( node_list.size() );
    for(unsigned i=0;i<node_list.size();++i)
        amc.vv[i].reserve( gne.res[i] );
    apply(elem_list, amc,*this);

    // update node_neighbours... tables
    // sort and make unique
    for(unsigned i=0;i<node_list.size();++i) {
        TNode **be = amc.vv[i].begin(), **en = amc.vv[i].end();
        std::sort( be, en, std::less<TNode *>() );
        TNode **new_end = std::unique( be, en );
        amc.vv[i].resize( new_end - be );
    }
    node_neighbours = amc.vv;
}

namespace LMTPRIVATE {
    template<class EA> struct AppendNodeParents {
        template<class TE,class TM> void operator()(TE &e,const TM &m) {
            for(unsigned i=0;i<TE::nb_nodes;++i)
                res[m.node_list.number(*e.node(i))].push_back( &e );
        }
        Vec<Vec<EA *> > res;
    };
};
template<class Carac,unsigned nvi_to_subs,unsigned skin>
void MeshAncestor<Carac,nvi_to_subs,skin>::update_node_parents() {
    if ( date_last_node_parents_update == date_last_connectivity_change )
        return;
    date_last_node_parents_update = date_last_connectivity_change;
    
    LMTPRIVATE::AppendNodeParents<EA> anb;
    anb.res.resize( node_list.size() );
    apply( elem_list, anb, *this );
    
    // sort and make unique
    for(unsigned i=0;i<node_list.size();++i) {
        EA **be = anb.res[i].begin(), **en = anb.res[i].end();
        std::sort( be, en, std::less<EA *>() );
        EA **new_end = std::unique( be, en );
        anb.res[i].resize( new_end - be );
    }
    node_parents = anb.res;
}

namespace LMTPRIVATE {
    template<class TEN,class EA> struct UpdateElemNeighbour {
        template<class TE> void operator()(const StructForType<TE> &st) {
            for(unsigned i=0;i<res[TE::num_in_elem_list].size();++i) {
                EA **be = res[TE::num_in_elem_list][i].begin(), **en = res[TE::num_in_elem_list][i].end();
                std::sort( be, en, std::less<EA *>() );
                EA **new_end = std::unique( be, en );
                res[TE::num_in_elem_list][i].resize( new_end - be );
            }
            elem_neighbours[TE::num_in_elem_list] = res[TE::num_in_elem_list];
        }
        Vec<Vec<EA *> > *res;
        TEN *elem_neighbours;
    };
};
template<class Carac,unsigned nvi_to_subs,unsigned skin>
void MeshAncestor<Carac,nvi_to_subs,skin>::update_elem_neighbours() {
    if ( date_last_elem_neighbours_update == date_last_connectivity_change )
        return;
    date_last_elem_neighbours_update = date_last_connectivity_change;

    //    
    Vec<Vec<EA *> > res[TElemList::nb_elem_type];
    unsigned s[TElemList::nb_elem_type];
    elem_list.get_sizes(s);
    for(unsigned i=0;i<TElemList::nb_elem_type;++i)
        res[i].resize( s[i] );
      
    //  
    update_node_parents();
    for(unsigned i=0;i<node_list.size();++i) {
        SimpleConstIterator<EA *> iter_par = get_node_parents(i);
        EA * const *b = iter_par.begin(), * const *e = iter_par.end();
        for(EA * const *e1=b;e1!=e;++e1)
            for(EA * const *e2=b;e2!=e;++e2)
                if ( e1 != e2 )
                    res[(*e1)->num_in_elem_list_virtual()][(*e1)->number].push_back( *e2 );
    }
    // sort and make unique
    LMTPRIVATE::UpdateElemNeighbour<PackedVectorOfVector<EA *>,EA> uen;
    uen.res = res;
    uen.elem_neighbours = elem_neighbours;
    TElemList::apply_static(uen);
}


// template<class TM,class Op>
// struct ApplyDS {
//     template<class TE,class TM> void operator()( TE &e, TM &m ) const {
//         
// //         (*op)( e,  );
//     }
//     Op *op;
// };

// template<class TM,class Op> void apply_dS( TM &m, const Op &op ) {
//     m.update_skin();
//     ApplyDS<TM,Op> ads;
//     ads.op = &op;
//     apply( m.sub_mesh(Number<1>()), ads, m );
// }

template<class Carac,unsigned nvi_to_subs,unsigned skin>
typename MeshAncestor<Carac,nvi_to_subs,skin>::Pvec center( const MeshAncestor<Carac,nvi_to_subs,skin> &m ) {
    typename MeshAncestor<Carac,nvi_to_subs,skin>::Pvec res( 0 );
    for(unsigned i=0;i<m.node_list.size();++i)
        res += m.node_list[i].pos;
    return res / m.node_list.size();
}

};

#endif // LMT_meshancestor_HEADER

