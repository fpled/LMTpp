

namespace LMT {

/*!
\generic_comment apply_wi

    Cette fonction agit sur un vecteur comme \a apply mais en tenant compte de l'indice de l'élément (d'où le wi pour with index).
    Par conséquent la classe-fonction doit définir correctement operator() de la façon suivante :
    \code 
        template <class Telement> struct operateur {
            void operator()( const Telement& e, int i ) const { bla bla }
        };

    Cette fonction permet donc d'appliquer un opérateur, disons op, sur l'ensemble des éléments d'un conteneur (Vec en général). La syntaxe générale est du genre :
    \code 
        apply_wi( conteneur, op, param );
    où param sont les éventuels parametres de l'opérateur.

    A FAIRE : apply_wi a d'autres syntaxes utilisées en interne. J'en parlerai plus tard.

    \keyword Fonctionalité/Transformer
    \friend samir.amrouche@lmt.ens-cachan.fr
    \friend hugo.leclerc@lmt.ens-cachan.fr
    \author Hugo Leclerc
*/

/*!
\generic_comment apply_on_number

    Cette fonction sert à appliquer un opérateur sur un seul élément d'un vecteur. Elle est surtout intéressante pour les vecteurs hétérogènes ( \a Vec<Heterogeneous<Carac,nt,TT>,static_size_,int> ). 
    Sa syntaxe générale est :
    \code
        apply_on_number( vecteur, indice, opérateur, paramètres faculatatifs de l'opérateur ) 
    Pour avoir un exemple de code, voir \a [Exemples d'utilisation des vecteurs hétérogènes].
    Remarque : 
        * Pour un vecteur hétérogène, on n'a pas accès à l'indice d'un élément particulier. Donc on utilisera cette fonction avec \a find_with_index pour récupérer l'indice de l'élément.
        * Excepté pour un vecteur hétérogène, il est préférable d'utiliser les crochets [] .
    
    \relates apply
    \relates find
    \relates Vec
    \keyword Fonctionalité/Transformer
    \friend hugo.leclerc@lmt.ens-cachan.fr
    \friend samir.amrouche@lmt.ens-cachan.fr
*/

/*!
\generic_comment find

    Cette fonction permet de savoir si un élément d'un vecteur vérifie une certaine propriété en renvoyant un booléen.
    La syntaxe générale est 
    \code
         booléen find( vecteur, opérateur, paramètres faculatatifs de l'opérateur ) 

    <strong> Remarque : L'opérateur doit renvoyer un booléen.</strong>
    Remarque : la fonction find() arrête sa recherche dès qu'elle a trouvé un élément qui convient.
    \keyword Fonctionalité/Extraire
    \relates Vec 
    \friend hugo.leclerc@lmt.ens-cachan.fr
    \friend samir.amrouche@lmt.ens-cachan.fr
*/

/*!
\generic_comment apply_range

    Cette fonction est une variante de \a apply . 
    La syntaxe générale est 
    \code
         booléen aply_range( vecteur, début, fin, opérateur, paramètres faculatatifs de l'opérateur ) 
         
    Elle permet d'appliquer l'opérateur sur les indices de l'intervalle [début;fin[ (<strong> Attention : fin n'est pas compris </strong>).

    \keyword Fonctionalité/Transformer
    \relates Vec 
    \friend hugo.leclerc@lmt.ens-cachan.fr
    \friend samir.amrouche@lmt.ens-cachan.fr
*/

/*!
\generic_comment remove_if

    Cette fonction supprime tous les éléments d'un vecteur vérifiant une certaine condition. 
    La syntaxe générale est 
    \code
         remove_if( vecteur, opérateur, paramètres faculatatifs de l'opérateur ) 
         
    C'est l'opérateur qui contient la condition. Voici un exemple qui supprime tous les entiers supérieurs à 100  d'un vecteur d'entier.
    \code C/C++
        template<class T> struct MonOp {
        
            bool operator() (T t,int f)  { return (t>f) ? true : false ; }
        } ;
        
        int main() {
        
            Vec<int> v = range(200); // crée un vecteur de taille 200 d'entiers variant de 0 à 199.
            MonOp<int> op ;
            
            PRINTN(v);
            remove_if(v,op,100);
            PRINTN(v);
        }

    Juste quelques remarques : 
        * on peut faire plus simple dans le cas précédent en utilisant les fonctions lambda : cad on écrirait 
            \code C/C++
                remove_if(v,_1>_2,100);
          _1 représente l'élément courant et _2 le premier paramètre i.e. 100.
        * l'opérateur parenthèse, operator(), doit retourner un booléen.

    \keyword Fonctionalité/Transformer
    \relates Vec 
    \relates apply
    \friend hugo.leclerc@lmt.ens-cachan.fr
    \friend samir.amrouche@lmt.ens-cachan.fr
*/

/*!
\generic_comment find_elem

    Cette fonction renvoie un pointeur sur le premier élément d'un vecteur vérifiant une certaine propriété. 
    La syntaxe générale est 
    \code
         find_elem( vecteur, opérateur, paramètres faculatatifs de l'opérateur ) 
         
    C'est l'opérateur qui contient la condition. Voici un exemple où on récupère un pointeur sur le premier élément égal à 7  d'un vecteur d'entier.
    \code C/C++
        template<class T,class U> struct MonOp {
        
            bool operator() (T t,U u)  { return (t.u==u) ? true : false ; }
        } ;
        
        struct A {
        
            int u ;
            char c ;
        } ;
        
        int main() {
        
            A a;
            Vec<A> v ;
            A* pi = NULL ;
            MonOp<A,int> op ;
        
            a.u = 10 ; a.c = 'e' ;
            v.push_back(a);
            a.u = 11 ; a.c = 'e' ;
            v.push_back(a);
            a.u = 7 ; a.c = 'e';
            v.push_back(a);
            a.u = 7 ; a.c = 'f' ;
            v.push_back(a);
        
            pi = find_elem(v,op,7);
            PRINTN(pi->u);
            PRINTN(pi->c); // affiche e
        }

    
    <strong> Remarque importante : la fonction ne marche pas avec les vecteurs hétérogènes (à moins que le type des éléments dérivent d'un type ancêtre).</strong>

    \keyword Fonctionalité/Extraire
    \relates Vec 
    \relates apply
    \friend hugo.leclerc@lmt.ens-cachan.fr
    \friend samir.amrouche@lmt.ens-cachan.fr
*/


  template<class TT,int s,class Op> inline void apply(const Vec<TT,s,void> &v,const Op& op) { for(unsigned i=0;i<v.size();++i) op(v[i]); }
  template<class TT,int s,class Op> inline void apply(Vec<TT,s,void> &v,const Op& op) { for(unsigned i=0;i<v.size();++i) op(v[i]); }
  template<class TT,int s,class Op> inline void apply(const Vec<TT,s,void> &v,Op& op) { for(unsigned i=0;i<v.size();++i) op(v[i]); }
  template<class TT,int s,class Op> inline void apply(Vec<TT,s,void> &v,Op& op) { for(unsigned i=0;i<v.size();++i) op(v[i]); }
  template<class TT,int s,class Op,class TP0> inline void apply(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply(Vec<TT,s,void> &v,const Op& op,const TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply(const Vec<TT,s,void> &v,Op& op,const TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply(Vec<TT,s,void> &v,Op& op,const TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply(const Vec<TT,s,void> &v,const Op& op,TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply(Vec<TT,s,void> &v,const Op& op,TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply(const Vec<TT,s,void> &v,Op& op,TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply(Vec<TT,s,void> &v,Op& op,TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(const Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(const Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }

  template<class TT,int s,class Op> inline void apply_nz(const Vec<TT,s,void> &v,const Op& op) { apply(v,op); }
  template<class TT,int s,class Op> inline void apply_nz(Vec<TT,s,void> &v,const Op& op) { apply(v,op); }
  template<class TT,int s,class Op> inline void apply_nz(const Vec<TT,s,void> &v,Op& op) { apply(v,op); }
  template<class TT,int s,class Op> inline void apply_nz(Vec<TT,s,void> &v,Op& op) { apply(v,op); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0) { apply(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz(Vec<TT,s,void> &v,const Op& op,const TP0 &p0) { apply(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz(const Vec<TT,s,void> &v,Op& op,const TP0 &p0) { apply(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz(Vec<TT,s,void> &v,Op& op,const TP0 &p0) { apply(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz(const Vec<TT,s,void> &v,const Op& op,TP0 &p0) { apply(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz(Vec<TT,s,void> &v,const Op& op,TP0 &p0) { apply(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz(const Vec<TT,s,void> &v,Op& op,TP0 &p0) { apply(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz(Vec<TT,s,void> &v,Op& op,TP0 &p0) { apply(v,op,p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(const Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(const Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }

  template<class TT,int s,class Op> inline void apply_nz_unique(const Vec<TT,s,void> &v,const Op& op) { apply_nz(v,op); }
  template<class TT,int s,class Op> inline void apply_nz_unique(Vec<TT,s,void> &v,const Op& op) { apply_nz(v,op); }
  template<class TT,int s,class Op> inline void apply_nz_unique(const Vec<TT,s,void> &v,Op& op) { apply_nz(v,op); }
  template<class TT,int s,class Op> inline void apply_nz_unique(Vec<TT,s,void> &v,Op& op) { apply_nz(v,op); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_unique(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0) { apply_nz(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_unique(Vec<TT,s,void> &v,const Op& op,const TP0 &p0) { apply_nz(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_unique(const Vec<TT,s,void> &v,Op& op,const TP0 &p0) { apply_nz(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_unique(Vec<TT,s,void> &v,Op& op,const TP0 &p0) { apply_nz(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_unique(const Vec<TT,s,void> &v,const Op& op,TP0 &p0) { apply_nz(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_unique(Vec<TT,s,void> &v,const Op& op,TP0 &p0) { apply_nz(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_unique(const Vec<TT,s,void> &v,Op& op,TP0 &p0) { apply_nz(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_unique(Vec<TT,s,void> &v,Op& op,TP0 &p0) { apply_nz(v,op,p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(const Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(const Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }

  template<class TT,int s,class Op> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,const Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from]); }
  template<class TT,int s,class Op> inline void apply_range(Vec<TT,s,void> &v,int from,int to,const Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from]); }
  template<class TT,int s,class Op> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from]); }
  template<class TT,int s,class Op> inline void apply_range(Vec<TT,s,void> &v,int from,int to,Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from]); }
  template<class TT,int s,class Op,class TP0> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,const Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range(Vec<TT,s,void> &v,int from,int to,const Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range(Vec<TT,s,void> &v,int from,int to,Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,const Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range(Vec<TT,s,void> &v,int from,int to,const Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range(Vec<TT,s,void> &v,int from,int to,Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(Vec<TT,s,void> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(Vec<TT,s,void> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(Vec<TT,s,void> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(Vec<TT,s,void> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(Vec<TT,s,void> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(Vec<TT,s,void> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(Vec<TT,s,void> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(Vec<TT,s,void> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,void> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,void> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,void> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,void> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,void> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,void> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,void> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,void> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,void> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,void> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,void> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,void> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,void> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,void> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,void> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,void> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,void> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }

  template<class TT,int s,class Op> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from]); }
  template<class TT,int s,class Op> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from]); }
  template<class TT,int s,class Op> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from]); }
  template<class TT,int s,class Op> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from]); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,void> &v,int from,int to,int inc,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }

  template<class TT,int s,class Op> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op) { for(;from<to-0;from+=1) op(v[from+0]);  }
  template<class TT,int s,class Op> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op) { for(;from<to-0;from+=1) op(v[from+0]);  }
  template<class TT,int s,class Op> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op) { for(;from<to-0;from+=1) op(v[from+0]);  }
  template<class TT,int s,class Op> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op) { for(;from<to-0;from+=1) op(v[from+0]);  }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op,const TP0 &p0) { for(;from<to-0;from+=1) op(v[from+0],p0);  }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op,const TP0 &p0) { for(;from<to-0;from+=1) op(v[from+0],p0);  }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op,const TP0 &p0) { for(;from<to-0;from+=1) op(v[from+0],p0);  }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op,const TP0 &p0) { for(;from<to-0;from+=1) op(v[from+0],p0);  }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op,TP0 &p0) { for(;from<to-0;from+=1) op(v[from+0],p0);  }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op,TP0 &p0) { for(;from<to-0;from+=1) op(v[from+0],p0);  }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op,TP0 &p0) { for(;from<to-0;from+=1) op(v[from+0],p0);  }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op,TP0 &p0) { for(;from<to-0;from+=1) op(v[from+0],p0);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op,const TP0 &p0,const TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op,const TP0 &p0,const TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op,TP0 &p0,const TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op,TP0 &p0,const TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op,TP0 &p0,const TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op,TP0 &p0,const TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op,const TP0 &p0,TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op,const TP0 &p0,TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op,const TP0 &p0,TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op,const TP0 &p0,TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op,TP0 &p0,TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op,TP0 &p0,TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op,TP0 &p0,TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op,TP0 &p0,TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<1> &n_inc,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }

  template<class TT,int s,class Op> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op) { for(;from<to-1;from+=2) op(v[from+0],v[from+1]); for(;from<to;++from) op(v[from]); }
  template<class TT,int s,class Op> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op) { for(;from<to-1;from+=2) op(v[from+0],v[from+1]); for(;from<to;++from) op(v[from]); }
  template<class TT,int s,class Op> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op) { for(;from<to-1;from+=2) op(v[from+0],v[from+1]); for(;from<to;++from) op(v[from]); }
  template<class TT,int s,class Op> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op) { for(;from<to-1;from+=2) op(v[from+0],v[from+1]); for(;from<to;++from) op(v[from]); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op,const TP0 &p0) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op,const TP0 &p0) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op,const TP0 &p0) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op,const TP0 &p0) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op,TP0 &p0) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op,TP0 &p0) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op,TP0 &p0) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op,TP0 &p0) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op,const TP0 &p0,const TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op,const TP0 &p0,const TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op,TP0 &p0,const TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op,TP0 &p0,const TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op,TP0 &p0,const TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op,TP0 &p0,const TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op,const TP0 &p0,TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op,const TP0 &p0,TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op,const TP0 &p0,TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op,const TP0 &p0,TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op,TP0 &p0,TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op,TP0 &p0,TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op,TP0 &p0,TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op,TP0 &p0,TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<2> &n_inc,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }

  template<class TT,int s,class Op> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3]); for(;from<to;++from) op(v[from]); }
  template<class TT,int s,class Op> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3]); for(;from<to;++from) op(v[from]); }
  template<class TT,int s,class Op> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3]); for(;from<to;++from) op(v[from]); }
  template<class TT,int s,class Op> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3]); for(;from<to;++from) op(v[from]); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op,const TP0 &p0) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op,const TP0 &p0) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op,const TP0 &p0) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op,const TP0 &p0) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op,TP0 &p0) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op,TP0 &p0) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op,TP0 &p0) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op,TP0 &p0) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op,const TP0 &p0,const TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op,const TP0 &p0,const TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op,TP0 &p0,const TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op,TP0 &p0,const TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op,TP0 &p0,const TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op,TP0 &p0,const TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op,const TP0 &p0,TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op,const TP0 &p0,TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op,const TP0 &p0,TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op,const TP0 &p0,TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op,TP0 &p0,TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op,TP0 &p0,TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op,TP0 &p0,TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op,TP0 &p0,TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,void> &v,int from,int to,const Number<4> &n_inc,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }

  template<class TT,int s,class Op> inline void apply_on_number(const Vec<TT,s,void> &v,int num,const Op& op) { op(v[num]); }
  template<class TT,int s,class Op> inline void apply_on_number(Vec<TT,s,void> &v,int num,const Op& op) { op(v[num]); }
  template<class TT,int s,class Op> inline void apply_on_number(const Vec<TT,s,void> &v,int num,Op& op) { op(v[num]); }
  template<class TT,int s,class Op> inline void apply_on_number(Vec<TT,s,void> &v,int num,Op& op) { op(v[num]); }
  template<class TT,int s,class Op,class TP0> inline void apply_on_number(const Vec<TT,s,void> &v,int num,const Op& op,const TP0 &p0) { op(v[num],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_on_number(Vec<TT,s,void> &v,int num,const Op& op,const TP0 &p0) { op(v[num],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_on_number(const Vec<TT,s,void> &v,int num,Op& op,const TP0 &p0) { op(v[num],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_on_number(Vec<TT,s,void> &v,int num,Op& op,const TP0 &p0) { op(v[num],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_on_number(const Vec<TT,s,void> &v,int num,const Op& op,TP0 &p0) { op(v[num],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_on_number(Vec<TT,s,void> &v,int num,const Op& op,TP0 &p0) { op(v[num],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_on_number(const Vec<TT,s,void> &v,int num,Op& op,TP0 &p0) { op(v[num],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_on_number(Vec<TT,s,void> &v,int num,Op& op,TP0 &p0) { op(v[num],p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(const Vec<TT,s,void> &v,int num,const Op& op,const TP0 &p0,const TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(Vec<TT,s,void> &v,int num,const Op& op,const TP0 &p0,const TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(const Vec<TT,s,void> &v,int num,Op& op,const TP0 &p0,const TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(Vec<TT,s,void> &v,int num,Op& op,const TP0 &p0,const TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(const Vec<TT,s,void> &v,int num,const Op& op,TP0 &p0,const TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(Vec<TT,s,void> &v,int num,const Op& op,TP0 &p0,const TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(const Vec<TT,s,void> &v,int num,Op& op,TP0 &p0,const TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(Vec<TT,s,void> &v,int num,Op& op,TP0 &p0,const TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(const Vec<TT,s,void> &v,int num,const Op& op,const TP0 &p0,TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(Vec<TT,s,void> &v,int num,const Op& op,const TP0 &p0,TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(const Vec<TT,s,void> &v,int num,Op& op,const TP0 &p0,TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(Vec<TT,s,void> &v,int num,Op& op,const TP0 &p0,TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(const Vec<TT,s,void> &v,int num,const Op& op,TP0 &p0,TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(Vec<TT,s,void> &v,int num,const Op& op,TP0 &p0,TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(const Vec<TT,s,void> &v,int num,Op& op,TP0 &p0,TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(Vec<TT,s,void> &v,int num,Op& op,TP0 &p0,TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,void> &v,int num,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,void> &v,int num,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,void> &v,int num,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,void> &v,int num,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,void> &v,int num,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,void> &v,int num,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,void> &v,int num,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,void> &v,int num,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,void> &v,int num,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,void> &v,int num,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,void> &v,int num,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,void> &v,int num,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,void> &v,int num,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,void> &v,int num,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,void> &v,int num,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,void> &v,int num,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,void> &v,int num,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,void> &v,int num,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,void> &v,int num,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,void> &v,int num,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,void> &v,int num,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,void> &v,int num,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,void> &v,int num,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,void> &v,int num,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,void> &v,int num,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,void> &v,int num,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,void> &v,int num,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,void> &v,int num,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,void> &v,int num,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,void> &v,int num,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,void> &v,int num,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,void> &v,int num,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }

  template<class TT,int s,class Op> inline void apply_wi(const Vec<TT,s,void> &v,const Op& op) { for(unsigned i=0;i<v.size();++i) op(v[i],i); }
  template<class TT,int s,class Op> inline void apply_wi(Vec<TT,s,void> &v,const Op& op) { for(unsigned i=0;i<v.size();++i) op(v[i],i); }
  template<class TT,int s,class Op> inline void apply_wi(const Vec<TT,s,void> &v,Op& op) { for(unsigned i=0;i<v.size();++i) op(v[i],i); }
  template<class TT,int s,class Op> inline void apply_wi(Vec<TT,s,void> &v,Op& op) { for(unsigned i=0;i<v.size();++i) op(v[i],i); }
  template<class TT,int s,class Op,class TP0> inline void apply_wi(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_wi(Vec<TT,s,void> &v,const Op& op,const TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_wi(const Vec<TT,s,void> &v,Op& op,const TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_wi(Vec<TT,s,void> &v,Op& op,const TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_wi(const Vec<TT,s,void> &v,const Op& op,TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_wi(Vec<TT,s,void> &v,const Op& op,TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_wi(const Vec<TT,s,void> &v,Op& op,TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_wi(Vec<TT,s,void> &v,Op& op,TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(const Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(const Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }

  template<class TT,int s,class Op> inline void apply_nz_wi(const Vec<TT,s,void> &v,const Op& op) { apply_wi(v,op); }
  template<class TT,int s,class Op> inline void apply_nz_wi(Vec<TT,s,void> &v,const Op& op) { apply_wi(v,op); }
  template<class TT,int s,class Op> inline void apply_nz_wi(const Vec<TT,s,void> &v,Op& op) { apply_wi(v,op); }
  template<class TT,int s,class Op> inline void apply_nz_wi(Vec<TT,s,void> &v,Op& op) { apply_wi(v,op); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_wi(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_wi(Vec<TT,s,void> &v,const Op& op,const TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_wi(const Vec<TT,s,void> &v,Op& op,const TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_wi(Vec<TT,s,void> &v,Op& op,const TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_wi(const Vec<TT,s,void> &v,const Op& op,TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_wi(Vec<TT,s,void> &v,const Op& op,TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_wi(const Vec<TT,s,void> &v,Op& op,TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_wi(Vec<TT,s,void> &v,Op& op,TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(const Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(const Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }

  template<class TT,int s,class Op> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,const Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from); }
  template<class TT,int s,class Op> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,const Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from); }
  template<class TT,int s,class Op> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from); }
  template<class TT,int s,class Op> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,const Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,const Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,const Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,const Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,void> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,void> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }

  template<class TT,int s,class Op> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from); }
  template<class TT,int s,class Op> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from); }
  template<class TT,int s,class Op> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from); }
  template<class TT,int s,class Op> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,void> &v,int from,int to,int inc,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,void> &v,int from,int to,int inc,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }

  template<class TT,int s,class Op> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,const Op& op) { return reduction( generate(v,op), Or() ); }
  template<class TT,int s,class Op> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,const Op& op) { return reduction( generate(v,op), Or() ); }
  template<class TT,int s,class Op> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,Op& op) { return reduction( generate(v,op), Or() ); }
  template<class TT,int s,class Op> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,Op& op) { return reduction( generate(v,op), Or() ); }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,const Op& op,const TP0 &p0) { return reduction( generate(v,op,p0), Or() ); }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,const Op& op,const TP0 &p0) { return reduction( generate(v,op,p0), Or() ); }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,Op& op,const TP0 &p0) { return reduction( generate(v,op,p0), Or() ); }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,Op& op,const TP0 &p0) { return reduction( generate(v,op,p0), Or() ); }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,const Op& op,TP0 &p0) { return reduction( generate(v,op,p0), Or() ); }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,const Op& op,TP0 &p0) { return reduction( generate(v,op,p0), Or() ); }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,Op& op,TP0 &p0) { return reduction( generate(v,op,p0), Or() ); }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,Op& op,TP0 &p0) { return reduction( generate(v,op,p0), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,const Op& op,const TP0 &p0,const TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,const Op& op,const TP0 &p0,const TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,Op& op,const TP0 &p0,const TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,Op& op,const TP0 &p0,const TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,const Op& op,TP0 &p0,const TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,const Op& op,TP0 &p0,const TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,Op& op,TP0 &p0,const TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,Op& op,TP0 &p0,const TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,const Op& op,const TP0 &p0,TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,const Op& op,const TP0 &p0,TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,Op& op,const TP0 &p0,TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,Op& op,const TP0 &p0,TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,const Op& op,TP0 &p0,TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,const Op& op,TP0 &p0,TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,Op& op,TP0 &p0,TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,Op& op,TP0 &p0,TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<0> &ni,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<0> &ni,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }

  template<class TT,int s,class Op> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,const Op& op) { for(unsigned i=0;i<v.size();++i) if (op(v[i])) return true; return false; }
  template<class TT,int s,class Op> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,const Op& op) { for(unsigned i=0;i<v.size();++i) if (op(v[i])) return true; return false; }
  template<class TT,int s,class Op> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,Op& op) { for(unsigned i=0;i<v.size();++i) if (op(v[i])) return true; return false; }
  template<class TT,int s,class Op> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,Op& op) { for(unsigned i=0;i<v.size();++i) if (op(v[i])) return true; return false; }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,const Op& op,const TP0 &p0) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return true; return false; }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,const Op& op,const TP0 &p0) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return true; return false; }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,Op& op,const TP0 &p0) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return true; return false; }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,Op& op,const TP0 &p0) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return true; return false; }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,const Op& op,TP0 &p0) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return true; return false; }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,const Op& op,TP0 &p0) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return true; return false; }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,Op& op,TP0 &p0) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return true; return false; }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,Op& op,TP0 &p0) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,const Op& op,const TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,const Op& op,const TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,Op& op,const TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,Op& op,const TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,const Op& op,TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,const Op& op,TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,Op& op,TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,Op& op,TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,const Op& op,const TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,const Op& op,const TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,Op& op,const TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,Op& op,const TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,const Op& op,TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,const Op& op,TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,Op& op,TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,Op& op,TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,void> &v,const Number<1> &ni,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,void> &v,const Number<1> &ni,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }

  template<class TT,int s,class Op> inline bool find(const Vec<TT,s,void> &v,const Op& op) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op); }
  template<class TT,int s,class Op> inline bool find(Vec<TT,s,void> &v,const Op& op) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op); }
  template<class TT,int s,class Op> inline bool find(const Vec<TT,s,void> &v,Op& op) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op); }
  template<class TT,int s,class Op> inline bool find(Vec<TT,s,void> &v,Op& op) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op); }
  template<class TT,int s,class Op,class TP0> inline bool find(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline bool find(Vec<TT,s,void> &v,const Op& op,const TP0 &p0) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline bool find(const Vec<TT,s,void> &v,Op& op,const TP0 &p0) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline bool find(Vec<TT,s,void> &v,Op& op,const TP0 &p0) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline bool find(const Vec<TT,s,void> &v,const Op& op,TP0 &p0) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline bool find(Vec<TT,s,void> &v,const Op& op,TP0 &p0) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline bool find(const Vec<TT,s,void> &v,Op& op,TP0 &p0) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline bool find(Vec<TT,s,void> &v,Op& op,TP0 &p0) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(const Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(const Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }

  template<class TT,int s,class Op> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,const Op& op) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i])) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,const Op& op) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i])) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,Op& op) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i])) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,Op& op) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i])) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,const Op& op,const TP0 &p0) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,Op& op,const TP0 &p0) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,Op& op,const TP0 &p0) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,const Op& op,TP0 &p0) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,const Op& op,TP0 &p0) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,Op& op,TP0 &p0) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,Op& op,TP0 &p0) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,void>::template SubType<0>::T * find_elem(const Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,void>::template SubType<0>::T * find_elem(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,void>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,void>::template SubType<0>::T *)NULL; }

  namespace LMTPRIVATE {
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op) { apply(v,op); }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op) { apply(v,op); }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op) { apply(v,op); }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op) { apply(v,op); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0) { apply(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0) { apply(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0) { apply(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0) { apply(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0) { apply(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0) { apply(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0) { apply(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0) { apply(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  } // namespace LMTPRIVATE

  namespace LMTPRIVATE {
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE));
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE));
        for(;i<(int)v.size();++i) op(v[i]);
     }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE));
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE));
        for(;i<(int)v.size();++i) op(v[i]);
     }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE));
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE));
        for(;i<(int)v.size();++i) op(v[i]);
     }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE));
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE));
        for(;i<(int)v.size();++i) op(v[i]);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  } // namespace LMTPRIVATE

  namespace LMTPRIVATE {
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE));
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE));
        for(;i<(int)v.size();++i) op(v[i]);
     }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE));
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE));
        for(;i<(int)v.size();++i) op(v[i]);
     }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE));
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE));
        for(;i<(int)v.size();++i) op(v[i]);
     }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE));
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE));
        for(;i<(int)v.size();++i) op(v[i]);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  } // namespace LMTPRIVATE

  template<class TT,int s,class Op> inline void apply_simd(const Vec<TT,s,void> &v,const Op& op) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op); }
  template<class TT,int s,class Op> inline void apply_simd(Vec<TT,s,void> &v,const Op& op) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op); }
  template<class TT,int s,class Op> inline void apply_simd(const Vec<TT,s,void> &v,Op& op) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op); }
  template<class TT,int s,class Op> inline void apply_simd(Vec<TT,s,void> &v,Op& op) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd(Vec<TT,s,void> &v,const Op& op,const TP0 &p0) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd(const Vec<TT,s,void> &v,Op& op,const TP0 &p0) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd(Vec<TT,s,void> &v,Op& op,const TP0 &p0) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd(const Vec<TT,s,void> &v,const Op& op,TP0 &p0) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd(Vec<TT,s,void> &v,const Op& op,TP0 &p0) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd(const Vec<TT,s,void> &v,Op& op,TP0 &p0) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd(Vec<TT,s,void> &v,Op& op,TP0 &p0) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(const Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(const Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }

  namespace LMTPRIVATE {
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op) { apply_wi(v,op); }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op) { apply_wi(v,op); }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op) { apply_wi(v,op); }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op) { apply_wi(v,op); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  } // namespace LMTPRIVATE

  namespace LMTPRIVATE {
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i);
        for(;i<(int)v.size();++i) op(v[i],i);
     }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i);
        for(;i<(int)v.size();++i) op(v[i],i);
     }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i);
        for(;i<(int)v.size();++i) op(v[i],i);
     }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i);
        for(;i<(int)v.size();++i) op(v[i],i);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  } // namespace LMTPRIVATE

  namespace LMTPRIVATE {
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i);
        for(;i<(int)v.size();++i) op(v[i],i);
     }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i);
        for(;i<(int)v.size();++i) op(v[i],i);
     }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i);
        for(;i<(int)v.size();++i) op(v[i],i);
     }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i);
        for(;i<(int)v.size();++i) op(v[i],i);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,void> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  } // namespace LMTPRIVATE

  template<class TT,int s,class Op> inline void apply_simd_wi(const Vec<TT,s,void> &v,const Op& op) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op); }
  template<class TT,int s,class Op> inline void apply_simd_wi(Vec<TT,s,void> &v,const Op& op) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op); }
  template<class TT,int s,class Op> inline void apply_simd_wi(const Vec<TT,s,void> &v,Op& op) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op); }
  template<class TT,int s,class Op> inline void apply_simd_wi(Vec<TT,s,void> &v,Op& op) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd_wi(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd_wi(Vec<TT,s,void> &v,const Op& op,const TP0 &p0) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd_wi(const Vec<TT,s,void> &v,Op& op,const TP0 &p0) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd_wi(Vec<TT,s,void> &v,Op& op,const TP0 &p0) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd_wi(const Vec<TT,s,void> &v,const Op& op,TP0 &p0) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd_wi(Vec<TT,s,void> &v,const Op& op,TP0 &p0) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd_wi(const Vec<TT,s,void> &v,Op& op,TP0 &p0) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd_wi(Vec<TT,s,void> &v,Op& op,TP0 &p0) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(const Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(const Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,void> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,void> >::res>(),op,p0,p1,p2); }

  template<class TT,int s,class Op> inline void remove_if(Vec<TT,s,void> &v,const Op& op) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i]) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op> inline void remove_if(Vec<TT,s,void> &v,Op& op) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i]) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0> inline void remove_if(Vec<TT,s,void> &v,const Op& op,const TP0 &p0) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0> inline void remove_if(Vec<TT,s,void> &v,Op& op,const TP0 &p0) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0> inline void remove_if(Vec<TT,s,void> &v,const Op& op,TP0 &p0) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0> inline void remove_if(Vec<TT,s,void> &v,Op& op,TP0 &p0) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1> inline void remove_if(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1> inline void remove_if(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1> inline void remove_if(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1> inline void remove_if(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1> inline void remove_if(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1> inline void remove_if(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1> inline void remove_if(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1> inline void remove_if(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,void> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,void> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,void> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,void> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,void> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,void> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,void> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }

  template<class TT,int s,class Op> inline void apply(const Vec<TT,s,int> &v,const Op& op) { for(unsigned i=0;i<v.size();++i) op(v[i]); }
  template<class TT,int s,class Op> inline void apply(Vec<TT,s,int> &v,const Op& op) { for(unsigned i=0;i<v.size();++i) op(v[i]); }
  template<class TT,int s,class Op> inline void apply(const Vec<TT,s,int> &v,Op& op) { for(unsigned i=0;i<v.size();++i) op(v[i]); }
  template<class TT,int s,class Op> inline void apply(Vec<TT,s,int> &v,Op& op) { for(unsigned i=0;i<v.size();++i) op(v[i]); }
  template<class TT,int s,class Op,class TP0> inline void apply(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply(Vec<TT,s,int> &v,const Op& op,const TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply(const Vec<TT,s,int> &v,Op& op,const TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply(Vec<TT,s,int> &v,Op& op,const TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply(const Vec<TT,s,int> &v,const Op& op,TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply(Vec<TT,s,int> &v,const Op& op,TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply(const Vec<TT,s,int> &v,Op& op,TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply(Vec<TT,s,int> &v,Op& op,TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(const Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(const Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(const Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],p0,p1,p2); }

  template<class TT,int s,class Op> inline void apply_nz(const Vec<TT,s,int> &v,const Op& op) { apply(v,op); }
  template<class TT,int s,class Op> inline void apply_nz(Vec<TT,s,int> &v,const Op& op) { apply(v,op); }
  template<class TT,int s,class Op> inline void apply_nz(const Vec<TT,s,int> &v,Op& op) { apply(v,op); }
  template<class TT,int s,class Op> inline void apply_nz(Vec<TT,s,int> &v,Op& op) { apply(v,op); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0) { apply(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz(Vec<TT,s,int> &v,const Op& op,const TP0 &p0) { apply(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz(const Vec<TT,s,int> &v,Op& op,const TP0 &p0) { apply(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz(Vec<TT,s,int> &v,Op& op,const TP0 &p0) { apply(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz(const Vec<TT,s,int> &v,const Op& op,TP0 &p0) { apply(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz(Vec<TT,s,int> &v,const Op& op,TP0 &p0) { apply(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz(const Vec<TT,s,int> &v,Op& op,TP0 &p0) { apply(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz(Vec<TT,s,int> &v,Op& op,TP0 &p0) { apply(v,op,p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(const Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(const Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(const Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }

  template<class TT,int s,class Op> inline void apply_nz_unique(const Vec<TT,s,int> &v,const Op& op) { apply_nz(v,op); }
  template<class TT,int s,class Op> inline void apply_nz_unique(Vec<TT,s,int> &v,const Op& op) { apply_nz(v,op); }
  template<class TT,int s,class Op> inline void apply_nz_unique(const Vec<TT,s,int> &v,Op& op) { apply_nz(v,op); }
  template<class TT,int s,class Op> inline void apply_nz_unique(Vec<TT,s,int> &v,Op& op) { apply_nz(v,op); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_unique(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0) { apply_nz(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_unique(Vec<TT,s,int> &v,const Op& op,const TP0 &p0) { apply_nz(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_unique(const Vec<TT,s,int> &v,Op& op,const TP0 &p0) { apply_nz(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_unique(Vec<TT,s,int> &v,Op& op,const TP0 &p0) { apply_nz(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_unique(const Vec<TT,s,int> &v,const Op& op,TP0 &p0) { apply_nz(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_unique(Vec<TT,s,int> &v,const Op& op,TP0 &p0) { apply_nz(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_unique(const Vec<TT,s,int> &v,Op& op,TP0 &p0) { apply_nz(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_unique(Vec<TT,s,int> &v,Op& op,TP0 &p0) { apply_nz(v,op,p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(const Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(const Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_unique(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { apply_nz(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(const Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_unique(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_nz(v,op,p0,p1,p2); }

  template<class TT,int s,class Op> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,const Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from]); }
  template<class TT,int s,class Op> inline void apply_range(Vec<TT,s,int> &v,int from,int to,const Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from]); }
  template<class TT,int s,class Op> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from]); }
  template<class TT,int s,class Op> inline void apply_range(Vec<TT,s,int> &v,int from,int to,Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from]); }
  template<class TT,int s,class Op,class TP0> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,const Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range(Vec<TT,s,int> &v,int from,int to,const Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range(Vec<TT,s,int> &v,int from,int to,Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,const Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range(Vec<TT,s,int> &v,int from,int to,const Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range(Vec<TT,s,int> &v,int from,int to,Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(Vec<TT,s,int> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(Vec<TT,s,int> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(Vec<TT,s,int> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(Vec<TT,s,int> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(Vec<TT,s,int> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(Vec<TT,s,int> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(Vec<TT,s,int> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range(Vec<TT,s,int> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,int> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,int> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,int> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,int> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,int> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,int> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,int> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,int> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,int> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,int> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,int> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,int> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,int> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,int> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,int> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(const Vec<TT,s,int> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range(Vec<TT,s,int> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],p0,p1,p2); }

  template<class TT,int s,class Op> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from]); }
  template<class TT,int s,class Op> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from]); }
  template<class TT,int s,class Op> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from]); }
  template<class TT,int s,class Op> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from]); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride(Vec<TT,s,int> &v,int from,int to,int inc,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],p0,p1,p2); }

  template<class TT,int s,class Op> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op) { for(;from<to-0;from+=1) op(v[from+0]);  }
  template<class TT,int s,class Op> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op) { for(;from<to-0;from+=1) op(v[from+0]);  }
  template<class TT,int s,class Op> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op) { for(;from<to-0;from+=1) op(v[from+0]);  }
  template<class TT,int s,class Op> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op) { for(;from<to-0;from+=1) op(v[from+0]);  }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op,const TP0 &p0) { for(;from<to-0;from+=1) op(v[from+0],p0);  }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op,const TP0 &p0) { for(;from<to-0;from+=1) op(v[from+0],p0);  }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op,const TP0 &p0) { for(;from<to-0;from+=1) op(v[from+0],p0);  }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op,const TP0 &p0) { for(;from<to-0;from+=1) op(v[from+0],p0);  }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op,TP0 &p0) { for(;from<to-0;from+=1) op(v[from+0],p0);  }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op,TP0 &p0) { for(;from<to-0;from+=1) op(v[from+0],p0);  }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op,TP0 &p0) { for(;from<to-0;from+=1) op(v[from+0],p0);  }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op,TP0 &p0) { for(;from<to-0;from+=1) op(v[from+0],p0);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op,const TP0 &p0,const TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op,const TP0 &p0,const TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op,TP0 &p0,const TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op,TP0 &p0,const TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op,TP0 &p0,const TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op,TP0 &p0,const TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op,const TP0 &p0,TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op,const TP0 &p0,TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op,const TP0 &p0,TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op,const TP0 &p0,TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op,TP0 &p0,TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op,TP0 &p0,TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op,TP0 &p0,TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op,TP0 &p0,TP1 &p1) { for(;from<to-0;from+=1) op(v[from+0],p0,p1);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<1> &n_inc,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-0;from+=1) op(v[from+0],p0,p1,p2);  }

  template<class TT,int s,class Op> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op) { for(;from<to-1;from+=2) op(v[from+0],v[from+1]); for(;from<to;++from) op(v[from]); }
  template<class TT,int s,class Op> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op) { for(;from<to-1;from+=2) op(v[from+0],v[from+1]); for(;from<to;++from) op(v[from]); }
  template<class TT,int s,class Op> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op) { for(;from<to-1;from+=2) op(v[from+0],v[from+1]); for(;from<to;++from) op(v[from]); }
  template<class TT,int s,class Op> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op) { for(;from<to-1;from+=2) op(v[from+0],v[from+1]); for(;from<to;++from) op(v[from]); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op,const TP0 &p0) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op,const TP0 &p0) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op,const TP0 &p0) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op,const TP0 &p0) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op,TP0 &p0) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op,TP0 &p0) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op,TP0 &p0) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op,TP0 &p0) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op,const TP0 &p0,const TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op,const TP0 &p0,const TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op,TP0 &p0,const TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op,TP0 &p0,const TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op,TP0 &p0,const TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op,TP0 &p0,const TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op,const TP0 &p0,TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op,const TP0 &p0,TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op,const TP0 &p0,TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op,const TP0 &p0,TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op,TP0 &p0,TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op,TP0 &p0,TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op,TP0 &p0,TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op,TP0 &p0,TP1 &p1) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<2> &n_inc,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-1;from+=2) op(v[from+0],v[from+1],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }

  template<class TT,int s,class Op> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3]); for(;from<to;++from) op(v[from]); }
  template<class TT,int s,class Op> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3]); for(;from<to;++from) op(v[from]); }
  template<class TT,int s,class Op> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3]); for(;from<to;++from) op(v[from]); }
  template<class TT,int s,class Op> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3]); for(;from<to;++from) op(v[from]); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op,const TP0 &p0) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op,const TP0 &p0) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op,const TP0 &p0) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op,const TP0 &p0) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op,TP0 &p0) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op,TP0 &p0) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op,TP0 &p0) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op,TP0 &p0) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0); for(;from<to;++from) op(v[from],p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op,const TP0 &p0,const TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op,const TP0 &p0,const TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op,TP0 &p0,const TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op,TP0 &p0,const TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op,TP0 &p0,const TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op,TP0 &p0,const TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op,const TP0 &p0,TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op,const TP0 &p0,TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op,const TP0 &p0,TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op,const TP0 &p0,TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op,TP0 &p0,TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op,TP0 &p0,TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op,TP0 &p0,TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op,TP0 &p0,TP1 &p1) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1); for(;from<to;++from) op(v[from],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(const Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_by_n(Vec<TT,s,int> &v,int from,int to,const Number<4> &n_inc,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(;from<to-3;from+=4) op(v[from+0],v[from+1],v[from+2],v[from+3],p0,p1,p2); for(;from<to;++from) op(v[from],p0,p1,p2); }

  template<class TT,int s,class Op> inline void apply_on_number(const Vec<TT,s,int> &v,int num,const Op& op) { op(v[num]); }
  template<class TT,int s,class Op> inline void apply_on_number(Vec<TT,s,int> &v,int num,const Op& op) { op(v[num]); }
  template<class TT,int s,class Op> inline void apply_on_number(const Vec<TT,s,int> &v,int num,Op& op) { op(v[num]); }
  template<class TT,int s,class Op> inline void apply_on_number(Vec<TT,s,int> &v,int num,Op& op) { op(v[num]); }
  template<class TT,int s,class Op,class TP0> inline void apply_on_number(const Vec<TT,s,int> &v,int num,const Op& op,const TP0 &p0) { op(v[num],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_on_number(Vec<TT,s,int> &v,int num,const Op& op,const TP0 &p0) { op(v[num],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_on_number(const Vec<TT,s,int> &v,int num,Op& op,const TP0 &p0) { op(v[num],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_on_number(Vec<TT,s,int> &v,int num,Op& op,const TP0 &p0) { op(v[num],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_on_number(const Vec<TT,s,int> &v,int num,const Op& op,TP0 &p0) { op(v[num],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_on_number(Vec<TT,s,int> &v,int num,const Op& op,TP0 &p0) { op(v[num],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_on_number(const Vec<TT,s,int> &v,int num,Op& op,TP0 &p0) { op(v[num],p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_on_number(Vec<TT,s,int> &v,int num,Op& op,TP0 &p0) { op(v[num],p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(const Vec<TT,s,int> &v,int num,const Op& op,const TP0 &p0,const TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(Vec<TT,s,int> &v,int num,const Op& op,const TP0 &p0,const TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(const Vec<TT,s,int> &v,int num,Op& op,const TP0 &p0,const TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(Vec<TT,s,int> &v,int num,Op& op,const TP0 &p0,const TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(const Vec<TT,s,int> &v,int num,const Op& op,TP0 &p0,const TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(Vec<TT,s,int> &v,int num,const Op& op,TP0 &p0,const TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(const Vec<TT,s,int> &v,int num,Op& op,TP0 &p0,const TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(Vec<TT,s,int> &v,int num,Op& op,TP0 &p0,const TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(const Vec<TT,s,int> &v,int num,const Op& op,const TP0 &p0,TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(Vec<TT,s,int> &v,int num,const Op& op,const TP0 &p0,TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(const Vec<TT,s,int> &v,int num,Op& op,const TP0 &p0,TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(Vec<TT,s,int> &v,int num,Op& op,const TP0 &p0,TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(const Vec<TT,s,int> &v,int num,const Op& op,TP0 &p0,TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(Vec<TT,s,int> &v,int num,const Op& op,TP0 &p0,TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(const Vec<TT,s,int> &v,int num,Op& op,TP0 &p0,TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_on_number(Vec<TT,s,int> &v,int num,Op& op,TP0 &p0,TP1 &p1) { op(v[num],p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,int> &v,int num,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,int> &v,int num,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,int> &v,int num,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,int> &v,int num,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,int> &v,int num,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,int> &v,int num,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,int> &v,int num,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,int> &v,int num,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,int> &v,int num,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,int> &v,int num,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,int> &v,int num,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,int> &v,int num,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,int> &v,int num,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,int> &v,int num,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,int> &v,int num,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,int> &v,int num,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,int> &v,int num,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,int> &v,int num,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,int> &v,int num,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,int> &v,int num,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,int> &v,int num,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,int> &v,int num,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,int> &v,int num,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,int> &v,int num,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,int> &v,int num,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,int> &v,int num,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,int> &v,int num,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,int> &v,int num,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,int> &v,int num,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,int> &v,int num,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(const Vec<TT,s,int> &v,int num,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_on_number(Vec<TT,s,int> &v,int num,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { op(v[num],p0,p1,p2); }

  template<class TT,int s,class Op> inline void apply_wi(const Vec<TT,s,int> &v,const Op& op) { for(unsigned i=0;i<v.size();++i) op(v[i],i); }
  template<class TT,int s,class Op> inline void apply_wi(Vec<TT,s,int> &v,const Op& op) { for(unsigned i=0;i<v.size();++i) op(v[i],i); }
  template<class TT,int s,class Op> inline void apply_wi(const Vec<TT,s,int> &v,Op& op) { for(unsigned i=0;i<v.size();++i) op(v[i],i); }
  template<class TT,int s,class Op> inline void apply_wi(Vec<TT,s,int> &v,Op& op) { for(unsigned i=0;i<v.size();++i) op(v[i],i); }
  template<class TT,int s,class Op,class TP0> inline void apply_wi(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_wi(Vec<TT,s,int> &v,const Op& op,const TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_wi(const Vec<TT,s,int> &v,Op& op,const TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_wi(Vec<TT,s,int> &v,Op& op,const TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_wi(const Vec<TT,s,int> &v,const Op& op,TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_wi(Vec<TT,s,int> &v,const Op& op,TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_wi(const Vec<TT,s,int> &v,Op& op,TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_wi(Vec<TT,s,int> &v,Op& op,TP0 &p0) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(const Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(const Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_wi(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(const Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_wi(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) op(v[i],i,p0,p1,p2); }

  template<class TT,int s,class Op> inline void apply_nz_wi(const Vec<TT,s,int> &v,const Op& op) { apply_wi(v,op); }
  template<class TT,int s,class Op> inline void apply_nz_wi(Vec<TT,s,int> &v,const Op& op) { apply_wi(v,op); }
  template<class TT,int s,class Op> inline void apply_nz_wi(const Vec<TT,s,int> &v,Op& op) { apply_wi(v,op); }
  template<class TT,int s,class Op> inline void apply_nz_wi(Vec<TT,s,int> &v,Op& op) { apply_wi(v,op); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_wi(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_wi(Vec<TT,s,int> &v,const Op& op,const TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_wi(const Vec<TT,s,int> &v,Op& op,const TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_wi(Vec<TT,s,int> &v,Op& op,const TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_wi(const Vec<TT,s,int> &v,const Op& op,TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_wi(Vec<TT,s,int> &v,const Op& op,TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_wi(const Vec<TT,s,int> &v,Op& op,TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_nz_wi(Vec<TT,s,int> &v,Op& op,TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(const Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(const Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_nz_wi(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(const Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_nz_wi(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }

  template<class TT,int s,class Op> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,const Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from); }
  template<class TT,int s,class Op> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,const Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from); }
  template<class TT,int s,class Op> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from); }
  template<class TT,int s,class Op> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,const Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,const Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,const Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,const Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(const Vec<TT,s,int> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_wi(Vec<TT,s,int> &v,int from,int to,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,p0,p1,p2); }

  template<class TT,int s,class Op> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from); }
  template<class TT,int s,class Op> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from); }
  template<class TT,int s,class Op> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from); }
  template<class TT,int s,class Op> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,Op& op) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,Op& op,const TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,Op& op,TP0 &p0) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,Op& op,const TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,Op& op,TP0 &p0,const TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,Op& op,const TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,Op& op,TP0 &p0,TP1 &p1) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(const Vec<TT,s,int> &v,int from,int to,int inc,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_range_stride_wi(Vec<TT,s,int> &v,int from,int to,int inc,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,p0,p1,p2); }

  template<class TT,int s,class Op> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,const Op& op) { return reduction( generate(v,op), Or() ); }
  template<class TT,int s,class Op> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,const Op& op) { return reduction( generate(v,op), Or() ); }
  template<class TT,int s,class Op> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,Op& op) { return reduction( generate(v,op), Or() ); }
  template<class TT,int s,class Op> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,Op& op) { return reduction( generate(v,op), Or() ); }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,const Op& op,const TP0 &p0) { return reduction( generate(v,op,p0), Or() ); }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,const Op& op,const TP0 &p0) { return reduction( generate(v,op,p0), Or() ); }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,Op& op,const TP0 &p0) { return reduction( generate(v,op,p0), Or() ); }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,Op& op,const TP0 &p0) { return reduction( generate(v,op,p0), Or() ); }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,const Op& op,TP0 &p0) { return reduction( generate(v,op,p0), Or() ); }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,const Op& op,TP0 &p0) { return reduction( generate(v,op,p0), Or() ); }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,Op& op,TP0 &p0) { return reduction( generate(v,op,p0), Or() ); }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,Op& op,TP0 &p0) { return reduction( generate(v,op,p0), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,const Op& op,const TP0 &p0,const TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,const Op& op,const TP0 &p0,const TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,Op& op,const TP0 &p0,const TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,Op& op,const TP0 &p0,const TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,const Op& op,TP0 &p0,const TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,const Op& op,TP0 &p0,const TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,Op& op,TP0 &p0,const TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,Op& op,TP0 &p0,const TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,const Op& op,const TP0 &p0,TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,const Op& op,const TP0 &p0,TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,Op& op,const TP0 &p0,TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,Op& op,const TP0 &p0,TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,const Op& op,TP0 &p0,TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,const Op& op,TP0 &p0,TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,Op& op,TP0 &p0,TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,Op& op,TP0 &p0,TP1 &p1) { return reduction( generate(v,op,p0,p1), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<0> &ni,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<0> &ni,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { return reduction( generate(v,op,p0,p1,p2), Or() ); }

  template<class TT,int s,class Op> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,const Op& op) { for(unsigned i=0;i<v.size();++i) if (op(v[i])) return true; return false; }
  template<class TT,int s,class Op> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,const Op& op) { for(unsigned i=0;i<v.size();++i) if (op(v[i])) return true; return false; }
  template<class TT,int s,class Op> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,Op& op) { for(unsigned i=0;i<v.size();++i) if (op(v[i])) return true; return false; }
  template<class TT,int s,class Op> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,Op& op) { for(unsigned i=0;i<v.size();++i) if (op(v[i])) return true; return false; }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,const Op& op,const TP0 &p0) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return true; return false; }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,const Op& op,const TP0 &p0) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return true; return false; }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,Op& op,const TP0 &p0) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return true; return false; }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,Op& op,const TP0 &p0) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return true; return false; }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,const Op& op,TP0 &p0) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return true; return false; }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,const Op& op,TP0 &p0) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return true; return false; }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,Op& op,TP0 &p0) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return true; return false; }
  template<class TT,int s,class Op,class TP0> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,Op& op,TP0 &p0) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,const Op& op,const TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,const Op& op,const TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,Op& op,const TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,Op& op,const TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,const Op& op,TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,const Op& op,TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,Op& op,TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,Op& op,TP0 &p0,const TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,const Op& op,const TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,const Op& op,const TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,Op& op,const TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,Op& op,const TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,const Op& op,TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,const Op& op,TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,Op& op,TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,Op& op,TP0 &p0,TP1 &p1) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(const Vec<TT,s,int> &v,const Number<1> &ni,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find_w_wo_ind(Vec<TT,s,int> &v,const Number<1> &ni,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return true; return false; }

  template<class TT,int s,class Op> inline bool find(const Vec<TT,s,int> &v,const Op& op) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op); }
  template<class TT,int s,class Op> inline bool find(Vec<TT,s,int> &v,const Op& op) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op); }
  template<class TT,int s,class Op> inline bool find(const Vec<TT,s,int> &v,Op& op) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op); }
  template<class TT,int s,class Op> inline bool find(Vec<TT,s,int> &v,Op& op) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op); }
  template<class TT,int s,class Op,class TP0> inline bool find(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline bool find(Vec<TT,s,int> &v,const Op& op,const TP0 &p0) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline bool find(const Vec<TT,s,int> &v,Op& op,const TP0 &p0) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline bool find(Vec<TT,s,int> &v,Op& op,const TP0 &p0) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline bool find(const Vec<TT,s,int> &v,const Op& op,TP0 &p0) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline bool find(Vec<TT,s,int> &v,const Op& op,TP0 &p0) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline bool find(const Vec<TT,s,int> &v,Op& op,TP0 &p0) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline bool find(Vec<TT,s,int> &v,Op& op,TP0 &p0) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(const Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(const Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline bool find(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(const Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline bool find(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,p0,p1,p2); }

  template<class TT,int s,class Op> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,const Op& op) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i])) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,const Op& op) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i])) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,Op& op) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i])) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,Op& op) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i])) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,const Op& op,const TP0 &p0) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,Op& op,const TP0 &p0) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,Op& op,const TP0 &p0) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,const Op& op,TP0 &p0) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,const Op& op,TP0 &p0) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,Op& op,TP0 &p0) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,Op& op,TP0 &p0) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0)) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1)) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline const typename Vec<TT,s,int>::template SubType<0>::T * find_elem(const Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (const typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline typename Vec<TT,s,int>::template SubType<0>::T * find_elem(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { STATICASSERT((Vec<TT,s,int>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],p0,p1,p2)) return &v[i]; return (typename Vec<TT,s,int>::template SubType<0>::T *)NULL; }

  namespace LMTPRIVATE {
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op) { apply(v,op); }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op) { apply(v,op); }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op) { apply(v,op); }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op) { apply(v,op); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0) { apply(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0) { apply(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0) { apply(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0) { apply(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0) { apply(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0) { apply(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0) { apply(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0) { apply(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1) { apply(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply(v,op,p0,p1,p2); }
  } // namespace LMTPRIVATE

  namespace LMTPRIVATE {
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE));
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE));
        for(;i<(int)v.size();++i) op(v[i]);
     }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE));
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE));
        for(;i<(int)v.size();++i) op(v[i]);
     }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE));
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE));
        for(;i<(int)v.size();++i) op(v[i]);
     }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE));
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE));
        for(;i<(int)v.size();++i) op(v[i]);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  } // namespace LMTPRIVATE

  namespace LMTPRIVATE {
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE));
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE));
        for(;i<(int)v.size();++i) op(v[i]);
     }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE));
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE));
        for(;i<(int)v.size();++i) op(v[i]);
     }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE));
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE));
        for(;i<(int)v.size();++i) op(v[i]);
     }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE));
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE));
        for(;i<(int)v.size();++i) op(v[i]);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0);
        for(;i<(int)v.size();++i) op(v[i],p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1);
        for(;i<(int)v.size();++i) op(v[i],p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],p0,p1,p2);
     }
  } // namespace LMTPRIVATE

  template<class TT,int s,class Op> inline void apply_simd(const Vec<TT,s,int> &v,const Op& op) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op); }
  template<class TT,int s,class Op> inline void apply_simd(Vec<TT,s,int> &v,const Op& op) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op); }
  template<class TT,int s,class Op> inline void apply_simd(const Vec<TT,s,int> &v,Op& op) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op); }
  template<class TT,int s,class Op> inline void apply_simd(Vec<TT,s,int> &v,Op& op) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd(Vec<TT,s,int> &v,const Op& op,const TP0 &p0) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd(const Vec<TT,s,int> &v,Op& op,const TP0 &p0) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd(Vec<TT,s,int> &v,Op& op,const TP0 &p0) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd(const Vec<TT,s,int> &v,const Op& op,TP0 &p0) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd(Vec<TT,s,int> &v,const Op& op,TP0 &p0) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd(const Vec<TT,s,int> &v,Op& op,TP0 &p0) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd(Vec<TT,s,int> &v,Op& op,TP0 &p0) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(const Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(const Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(const Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }

  namespace LMTPRIVATE {
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op) { apply_wi(v,op); }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op) { apply_wi(v,op); }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op) { apply_wi(v,op); }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op) { apply_wi(v,op); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0) { apply_wi(v,op,p0); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1) { apply_wi(v,op,p0,p1); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<0> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { apply_wi(v,op,p0,p1,p2); }
  } // namespace LMTPRIVATE

  namespace LMTPRIVATE {
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i);
        for(;i<(int)v.size();++i) op(v[i],i);
     }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i);
        for(;i<(int)v.size();++i) op(v[i],i);
     }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i);
        for(;i<(int)v.size();++i) op(v[i],i);
     }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i);
        for(;i<(int)v.size();++i) op(v[i],i);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<1> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  } // namespace LMTPRIVATE

  namespace LMTPRIVATE {
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i);
        for(;i<(int)v.size();++i) op(v[i],i);
     }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i);
        for(;i<(int)v.size();++i) op(v[i],i);
     }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i);
        for(;i<(int)v.size();++i) op(v[i],i);
     }
  template<class TT,unsigned ne,int s,class Op> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i);
        for(;i<(int)v.size();++i) op(v[i],i);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0);
        for(;i<(int)v.size();++i) op(v[i],i,p0);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(const Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  template<class TT,unsigned ne,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi_w_wo_simd_method(Vec<TT,s,int> &v,const Number<2> &,const Number<ne> &NE,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,p0,p1,p2);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,p0,p1,p2);
        for(;i<(int)v.size();++i) op(v[i],i,p0,p1,p2);
     }
  } // namespace LMTPRIVATE

  template<class TT,int s,class Op> inline void apply_simd_wi(const Vec<TT,s,int> &v,const Op& op) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op); }
  template<class TT,int s,class Op> inline void apply_simd_wi(Vec<TT,s,int> &v,const Op& op) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op); }
  template<class TT,int s,class Op> inline void apply_simd_wi(const Vec<TT,s,int> &v,Op& op) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op); }
  template<class TT,int s,class Op> inline void apply_simd_wi(Vec<TT,s,int> &v,Op& op) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd_wi(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd_wi(Vec<TT,s,int> &v,const Op& op,const TP0 &p0) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd_wi(const Vec<TT,s,int> &v,Op& op,const TP0 &p0) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd_wi(Vec<TT,s,int> &v,Op& op,const TP0 &p0) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd_wi(const Vec<TT,s,int> &v,const Op& op,TP0 &p0) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd_wi(Vec<TT,s,int> &v,const Op& op,TP0 &p0) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd_wi(const Vec<TT,s,int> &v,Op& op,TP0 &p0) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0> inline void apply_simd_wi(Vec<TT,s,int> &v,Op& op,TP0 &p0) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(const Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(const Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1> inline void apply_simd_wi(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(const Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void apply_simd_wi(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<Vec<TT,s,int> >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<Vec<TT,s,int> >::res>(),op,p0,p1,p2); }

  template<class TT,int s,class Op> inline void remove_if(Vec<TT,s,int> &v,const Op& op) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i]) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op> inline void remove_if(Vec<TT,s,int> &v,Op& op) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i]) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0> inline void remove_if(Vec<TT,s,int> &v,const Op& op,const TP0 &p0) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0> inline void remove_if(Vec<TT,s,int> &v,Op& op,const TP0 &p0) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0> inline void remove_if(Vec<TT,s,int> &v,const Op& op,TP0 &p0) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0> inline void remove_if(Vec<TT,s,int> &v,Op& op,TP0 &p0) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1> inline void remove_if(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1> inline void remove_if(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1> inline void remove_if(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1> inline void remove_if(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1> inline void remove_if(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1> inline void remove_if(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1> inline void remove_if(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1> inline void remove_if(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,const TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,const TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,const TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,int> &v,Op& op,const TP0 &p0,const TP1 &p1,TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,int> &v,const Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,int> &v,Op& op,TP0 &p0,const TP1 &p1,TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,int> &v,const Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,int> &v,Op& op,const TP0 &p0,TP1 &p1,TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,int> &v,const Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }
  template<class TT,int s,class Op,class TP0,class TP1,class TP2> inline void remove_if(Vec<TT,s,int> &v,Op& op,TP0 &p0,TP1 &p1,TP2 &p2) { 
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],p0,p1,p2) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
     }

}
