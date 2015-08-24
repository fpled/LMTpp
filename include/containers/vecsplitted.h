
namespace LMT {


/*!
    Avec cette structure, on alloue des petits blocs de mémoire séparés. Avantage : on peut ajouter des éléments sans réallouer.
    \friend hugo.leclerc@lmt.ens-cachan.fr
*/

template<class TT,unsigned atomic_size> struct Splitted {};

template<class TT,unsigned atomic_size> struct IsVecOp<Splitted<TT,atomic_size> > { typedef int T; };

/*!
 stores blocks as T[atomic_size].
 The main property is that push_back, resize and so on... do not change adress of stored data
 (which is not the case with std::vector or LMT::Vec<T,-1>).
 <strong> all operations that implies a walk through data assume that size is not modified. Example : if op in apply(op) add elements, new elements won't be used. </strong>

    \relates Vec
    \relates apply_wi_ptr
    \relates apply
*/
template<class TT,unsigned atomic_size>
class Vec<Splitted<TT,atomic_size>,-1,int> {
public:
    template<unsigned n> struct SubType { typedef TT T; };
    static const unsigned nb_sub_type = 1;
    
    typedef TT T;
    static const int sparsity_rate = 0;
    struct Atom {
        T data[atomic_size];
    };
    /// 
    Vec():size_last_atom(0) { owning=true; }
    ~Vec() {
        if ( owning )
            for(unsigned i=0;i<atoms.size();++i) {
                #ifdef PRINT_ALLOC
                total_allocated[ typeid(Atom).name() ] -= sizeof(Atom);
                #endif
                delete atoms[i];
            }
    }
    
    Vec(const Vec &v) {
        owning=true;
        atoms.resize(v.atoms.size());
        for(unsigned i=0;i<v.atoms.size();++i) {
            #ifdef PRINT_ALLOC
            total_allocated[ typeid(Atom).name() ] += sizeof(Atom);
            #endif
            atoms[i] = new Atom;
            *atoms[i] = *v.atoms[i];
        }
        size_last_atom = v.size_last_atom;
        if ( atoms.size() )
            last_atom = atoms.back();
    }
    Vec &operator=(const Vec &v) {
        free();
        atoms.resize(v.atoms.size());
        for(unsigned i=0;i<v.atoms.size();++i) {
            #ifdef PRINT_ALLOC
            total_allocated[ typeid(Atom).name() ] += sizeof(Atom);
            #endif
            atoms[i] = new Atom;
            *atoms[i] = *v.atoms[i];
        }
        size_last_atom = v.size_last_atom;
        if ( atoms.size() )
            last_atom = atoms.back();
        return *this;
    }
    ///
    void is_a_ref_of(Vec &v) {
        if ( owning ) {
            for(unsigned i=0;i<atoms.size();++i) {
                #ifdef PRINT_ALLOC
                total_allocated[ typeid(Atom).name() ] -= sizeof(Atom);
                #endif
                delete atoms[i];
            }
        }
        owning=false;
        atoms=v.atoms;
        size_last_atom=v.size_last_atom;
        last_atom = v.last_atom;
    }
    
    ///
    T *new_elem() {
        DEBUGASSERT(owning);
        if ( atoms.size()==0 ) {
            #ifdef PRINT_ALLOC
            total_allocated[ typeid(Atom).name() ] += sizeof(Atom);
            #endif
            last_atom = new Atom;
            atoms.push_back(last_atom); 
            size_last_atom = 0;
        }
        else if ( size_last_atom>=atomic_size ) {
            #ifdef PRINT_ALLOC
            total_allocated[ typeid(Atom).name() ] += sizeof(Atom);
            #endif
            last_atom = new Atom;
            atoms.push_back(last_atom);
            size_last_atom = 1;
            return last_atom->data;
        }
        return &last_atom->data[size_last_atom++];
    }
    ///
    template<class T2> T *push_back(const T2 &val) { T *res = new_elem(); *res = val; return res; }
    ///
    void resize(unsigned n) {
        if ( n==0 ) {
            free();
            return;
        }
        unsigned nb_to_append = n-size();
        if ( not nb_to_append )
            return;
        if ( not atoms.size() )
            atoms.push_back(new Atom);
        //
        if ( nb_to_append + size_last_atom < atomic_size ) {
            size_last_atom += nb_to_append;
        }
        else {
            nb_to_append -= atomic_size - size_last_atom;
                
            unsigned nb_atoms = nb_to_append / atomic_size;
            nb_to_append -= nb_atoms * atomic_size;
            for(unsigned i=0;i<nb_atoms;++i) {
                #ifdef PRINT_ALLOC
                total_allocated[ typeid(Atom).name() ] += sizeof(Atom);
                #endif
                atoms.push_back(new Atom);
            }
            #ifdef PRINT_ALLOC
            total_allocated[ typeid(Atom).name() ] += sizeof(Atom);
            #endif
            last_atom = new Atom;
            atoms.push_back(last_atom);
            
            size_last_atom = nb_to_append;
        }
    }
    ///
    template<class T2> void append_vec(const T2 *b,const T2 *e) {
        DEBUGASSERT(owning);
        unsigned nb_to_append = e-b;
        if ( nb_to_append + size_last_atom < atomic_size ) {
            while(--nb_to_append)
                last_atom->data[size_last_atom++] = *(b++);
        }
        else {
            nb_to_append -= atomic_size - size_last_atom;
            while( size_last_atom < atomic_size )
                last_atom->data[size_last_atom++] = *(b++);
                
            unsigned nb_atoms = nb_to_append / atomic_size;
            nb_to_append -= nb_atoms * atomic_size;
            
            for(unsigned i=0;i<nb_atoms;++i) {
                #ifdef PRINT_ALLOC
                total_allocated[ typeid(Atom).name() ] += sizeof(Atom);
                #endif
                Atom *n = new Atom;
                atoms.push_back(n);
                for(unsigned j=0;j<atomic_size;++j)
                    n->data[j] = *(b++);
            }
            
            #ifdef PRINT_ALLOC
            total_allocated[ typeid(Atom).name() ] += sizeof(Atom);
            #endif
            last_atom = new Atom;
            atoms.push_back(last_atom);
            
            size_last_atom = nb_to_append;
            for(unsigned j=0;j<nb_to_append;++j)
                last_atom->data[j] = *(b++);
        }
    }
    ///
    template<class TV> void append( const TV &c ) {
        apply( c, PushBack(), *this );
    }
    ///
    unsigned size() const { return nb_full_atoms()*atomic_size + size_last_atom; }
    ///
    inline unsigned nb_full_atoms() const { return max((int)atoms.size(),1)-1; }
    ///
    T &back() { return last_atom->data[size_last_atom-1]; }
    ///
    const T &back() const { return last_atom->data[size_last_atom-1]; }
    ///
    void pop_back() {
        #ifdef DEBUG
            assert(owning);
        #endif
        --size_last_atom;
        if ( size_last_atom == 0 ) {
            rm_last_atom();
        }
    }
    /// remove n elements from the end
    void pop_back(unsigned n) {
        #ifdef DEBUG
            assert(owning);
        #endif
        if ( n <= size_last_atom ) {
            size_last_atom -= n;
            n = 0;
            if ( size_last_atom == 0 )
                rm_last_atom();
        }
        else {
            n -= size_last_atom;
            rm_last_atom();
            for ( ; n>atomic_size ; n -= atomic_size )
                rm_last_atom();
            size_last_atom = atomic_size - n;
        }
    }
    ///
    void free() {
        if ( owning ) {
            for(unsigned i=0;i<atoms.size();++i) {
                #ifdef PRINT_ALLOC
                total_allocated[ typeid(Atom).name() ] -= sizeof(Atom);
                #endif
                delete atoms[i];
            }
        }
        atoms.free();
        
        size_last_atom = 0;
        owning = true;
    }


    /// allows to use a simple for. Conduct to SLOW code. ASSUMES that nb elements of the list is not modified while
    ///  walking through it.
    class iterator {
    public:
        iterator(Vec &s):i(0),j(0),sv(&s) {}
        operator bool() const { return ( i<sv->nb_full_atoms() || j<sv->size_last_atom ); }
        T &operator*() {
            if ( i<sv->nb_full_atoms() ) return sv->atoms[i]->data[j];
            return sv->last_atom->data[j];
        }
        void operator++() { if (++j>=atomic_size) { j=0; ++i; } }
    private:
        unsigned i,j;
        Vec *sv;
    };
    /// allows to use a simple for. Conduct to SLOW code. ASSUMES that nb elements of the list is not modified while
    ///  walking through it.
    class const_iterator {
    public:
        const_iterator(const Vec &s):i(0),j(0),sv(&s) {}
        operator bool() const { return ( i<sv->nb_full_atoms() || j<sv->size_last_atom ); }
        const T &operator*() const {
            if ( i<sv->nb_full_atoms() ) return sv->atoms[i]->data[j];
            return sv->last_atom->data[j];
        }
        void operator++() { if (++j>=atomic_size) { j=0; ++i; } }
    private:
        unsigned i,j;
        const Vec *sv;
    };
    
   
    T &operator[](unsigned i) { return atoms[i/atomic_size]->data[i%atomic_size]; } /// slower than apply (but allows walk through a list while modifying it)...
    const T &operator[](unsigned i) const { return atoms[i/atomic_size]->data[i%atomic_size]; } /// slower than apply (but allows walk through a list while modifying it)...
    
    bool is_owning() const { return owning; }

    void erase_elem_nb(unsigned i) { operator[](i)=back(); pop_back(); } /// erase element number i. This procedure does not maintain the order of elements

//private:
    void rm_last_atom() {
        #ifdef PRINT_ALLOC
        total_allocated[ typeid(Atom).name() ] -= sizeof(Atom);
        #endif
        delete last_atom;
        atoms.pop_back();
        last_atom = atoms.size() ? atoms.back() : 0;
        if ( atoms.size() )
            size_last_atom = atomic_size;
        else
            size_last_atom = 0;
    }

    Vec<Atom *> atoms;
    Atom *last_atom;
    unsigned size_last_atom;
    bool owning;
};

/*!
\generic_comment apply_wi_ptr

    Cette fonction réservée aux vecteurs  Vec<Splitted<TT,atomic_size>,-1,int> (cad un  Vec spécialisé par  Splitted) agit sur un vecteur comme  apply mais en tenant compte de l'indice de l'élément (d'où le wi pour with index) et l'accés à cet élément se fait par référence.
    Par conséquent la classe-fonction doit définir correctement operator() de cette façon :
    \code 
        template <class Telement> struct operateur {
            void operator()( const Telement* ptr_e, int i ) const { bla bla }
        };
          
    ptr_e est l'adresse de l'élément dans la mémoire de l'ordinateur.
    *ptr_e est l'élément lui-même (remarquer l'astérisque).

    riend samir.amrouche@lmt.ens-cachan.fr
*/



  template<class TT,unsigned atomic_size,int s,class Op> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j]);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j]);
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j]);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j]);
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j]);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j]);
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j]);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j]);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],p0,p1,p2);
     }

  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++);
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++);
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++);
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(v.last_atom->data[j],cpt++,p0,p1,p2);
     }

  template<class TT,unsigned atomic_size,int s,class Op> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j])) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j])) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j])) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j])) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j])) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j])) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j])) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j])) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }

  template<class TT,unsigned atomic_size,int s,class Op> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j]);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j]);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j]);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j]);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }

  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb]);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb]);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb]);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb]);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb]);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb]);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb]);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb]);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb]);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb]);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb]);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb]);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,const TP0 &p0) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,const TP0 &p0) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,const TP0 &p0) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,const TP0 &p0) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,TP0 &p0) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,TP0 &p0) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,TP0 &p0) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,TP0 &p0) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op( v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op( v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op( v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }

  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt);
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt);
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt);
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(v.last_atom->data[j],cpt,p0,p1,p2);
     }

  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op) { apply_nz(op); }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op) { apply_nz(op); }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op) { apply_nz(op); }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op) { apply_nz(op); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0) { apply_nz(op,p0); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0) { apply_nz(op,p0); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0) { apply_nz(op,p0); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0) { apply_nz(op,p0); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0) { apply_nz(op,p0); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0) { apply_nz(op,p0); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0) { apply_nz(op,p0); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0) { apply_nz(op,p0); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { apply_nz(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { apply_nz(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { apply_nz(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { apply_nz(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { apply_nz(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { apply_nz(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { apply_nz(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { apply_nz(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { apply_nz(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { apply_nz(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { apply_nz(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { apply_nz(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { apply_nz(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { apply_nz(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { apply_nz(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { apply_nz(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz(op,p0,p1,p2); }

  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j]);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j]);
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j]);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j]);
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j]);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j]);
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j]);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j]);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],p0,p1,p2);
     }

  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++);
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++);
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++);
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt=0;
            for(unsigned i=0;i<nf;++i) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt++,p0,p1,p2);
            for(unsigned j=0;j<sl;++j) op(*v.last_atom->data[j],cpt++,p0,p1,p2);
     }

  template<class TT,unsigned atomic_size,int s,class Op> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j])) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j])) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j])) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j])) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j])) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j])) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j])) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j])) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline bool find_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(*v.atoms[i]->data[j],p0,p1,p2)) return true;
            for(unsigned j=0;j<sl;++j)
                if (op(*v.last_atom->data[j],p0,p1,p2)) return true;
            return false;
     }

  template<class TT,unsigned atomic_size,int s,class Op> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j]);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j]);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j]);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j]);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned new_in_end = 0, to_be_filtered = v.size();
            if ( to_be_filtered == 0 ) return;
            unsigned j=atomic_size;
            for(unsigned i=0;i<v.atoms.size();++i) {
                for(j=(j==0);j<atomic_size;++j) {
                    unsigned os = v.size();
                    bool res = op(*v.atoms[i]->data[j],p0,p1,p2);
                    new_in_end += v.size() - os;
                    if ( res ) {
                        v.atoms[i]->data[j] = v.back();
                        v.pop_back();
                        if ( new_in_end ) --new_in_end;
                        else              --j;
                    }
                    if ( --to_be_filtered == 0 ) return;
                }
            }
         }

  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb]);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb]);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb]);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb]);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb]);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb]);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb]);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb]);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb]);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb]);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb]);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb]);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,const TP0 &p0) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,const TP0 &p0) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,const TP0 &p0) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,const TP0 &p0) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,TP0 &p0) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,TP0 &p0) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,TP0 &p0) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,TP0 &p0) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            DEBUGASSERT( to <= v.size() );
            unsigned ie = to / atomic_size;
            unsigned je = to % atomic_size;
            unsigned ib = from / atomic_size;
            unsigned jb = from % atomic_size;
            if ( ib == ie ) {
                for(;jb<je;++jb)
                    op(* v.atoms[ib]->data[jb],p0,p1,p2);
            }
            else {
                for(;ib<ie;++ib) {
                    for(;jb<atomic_size;++jb)
                        op(* v.atoms[ib]->data[jb],p0,p1,p2);
                    jb=0;
                }
                for(;jb<je;++jb)
                    op(* v.atoms[ie]->data[jb],p0,p1,p2);
            }
     }

  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt);
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt);
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt);
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom, cpt = 0;
            for(unsigned i=0;i<nf;++i,++cpt) for(unsigned j=0;j<atomic_size;++j) op(*v.atoms[i]->data[j],cpt,p0,p1,p2);
            for(unsigned j=0;j<sl;++j,++cpt) op(*v.last_atom->data[j],cpt,p0,p1,p2);
     }

  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op) { apply_nz_ptr(op); }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op) { apply_nz_ptr(op); }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op) { apply_nz_ptr(op); }
  template<class TT,unsigned atomic_size,int s,class Op> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op) { apply_nz_ptr(op); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0) { apply_nz_ptr(op,p0); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0) { apply_nz_ptr(op,p0); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0) { apply_nz_ptr(op,p0); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0) { apply_nz_ptr(op,p0); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0) { apply_nz_ptr(op,p0); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0) { apply_nz_ptr(op,p0); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0) { apply_nz_ptr(op,p0); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0) { apply_nz_ptr(op,p0); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { apply_nz_ptr(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { apply_nz_ptr(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { apply_nz_ptr(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { apply_nz_ptr(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { apply_nz_ptr(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { apply_nz_ptr(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { apply_nz_ptr(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { apply_nz_ptr(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { apply_nz_ptr(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { apply_nz_ptr(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { apply_nz_ptr(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { apply_nz_ptr(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { apply_nz_ptr(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { apply_nz_ptr(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { apply_nz_ptr(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { apply_nz_ptr(op,p0,p1); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique_ptr(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz_ptr(op,p0,p1,p2); }

  template<class TT,unsigned atomic_size,int s,class Op> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j])) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j])) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j])) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j])) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline TT * find_elem(Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }

  template<class TT,unsigned atomic_size,int s,class Op> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j])) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j])) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j])) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j])) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }
  template<class TT,unsigned atomic_size,int s,class Op,class TP0,class TP1,class TP2> inline const TT * find_elem(const Vec<Splitted<TT,atomic_size>,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
            unsigned nf = v.nb_full_atoms(), sl = v.size_last_atom;
            for(unsigned i=0;i<nf;++i)
                for(unsigned j=0;j<atomic_size;++j)
                    if (op(v.atoms[i]->data[j],p0,p1,p2)) return &v.atoms[i]->data[j];
            for(unsigned j=0;j<sl;++j)
                if (op(v.last_atom->data[j],p0,p1,p2)) return &v.last_atom->data[j];
            return NULL;
     }


}

