# -*- coding: utf-8 -*-
import string
from vecgenhelp import *



#print 'namespace LMT {'
print """

namespace LMT {

/*!
\generic_comment apply_wi

    Cette fonction agit sur un vecteur comme \\a apply mais en tenant compte de l'indice de l'élément (d'où le wi pour with index).
    Par conséquent la classe-fonction doit définir correctment operator() de la façon suivante :
    \code 
        template <class Telement> struct operateur {
            void operator()( const Telement& e, int i ) const { bla bla }
        };

    Cette fonction permet donc d'appliquer un opérateur, disons op, sur l'ensemble des éléments d'un conteneur (Vec en général). La syntaxe générale est du genre :
    \code 
        apply_wi( conteneur, op, param );
    où param sont les éventuels parametres de l'opérateur.

    A FAIRE : apply_wi a d'autres syntaxes utilisées en interne. J'en parlerai plus tard.

    \\keyword Fonctionalité/Transformer
    \\friend samir.amrouche@lmt.ens-cachan.fr
    \\friend hugo.leclerc@lmt.ens-cachan.fr
    \\author Hugo Leclerc
*/

/*!
\generic_comment apply_on_number

    Cette fonction sert à appliquer un opérateur sur un seul élément d'un vecteur. Elle est surtout intéressante pour les vecteurs hétérogènes ( \\a Vec<Heterogeneous<Carac,nt,TT>,static_size_,int> ). 
    Sa syntaxe générale est :
    \code
        apply_on_number( vecteur, indice, opérateur, paramètres faculatatifs de l'opérateur ) 
    Pour avoir un exemple de code, voir \\a [Exemples d'utilisation des vecteurs hétérogènes].
    Remarque : 
        * Pour un vecteur hétérogène, on n'a pas accès à l'indice d'un élément particulier. Donc on utilisera cette fonction avec \\a find_with_index pour récupérer l'indice de l'élément.
        * Excepté pour un vecteur hétérogène, il est préférable d'utiliser les crochets [] .
    
    \\relates apply
    \\relates find
    \\relates Vec
    \\keyword Fonctionalité/Transformer
    \\friend hugo.leclerc@lmt.ens-cachan.fr
    \\friend samir.amrouche@lmt.ens-cachan.fr
*/

/*!
\generic_comment find

    Cette fonction permet de savoir si un élément d'un vecteur vérifie une certaine propriété en renvoyant un booléen.
    La syntaxe générale est 
    \code
         booléen find( vecteur, opérateur, paramètres faculatatifs de l'opérateur ) 

    <strong> Remarque : L'opérateur doit renvoyer un booléen.</strong>
    Remarque : la fonction find() arrête sa recherche dès qu'elle a trouvé un élément qui convient.
    \\keyword Fonctionalité/Extraire
    \\relates Vec 
    \\friend hugo.leclerc@lmt.ens-cachan.fr
    \\friend samir.amrouche@lmt.ens-cachan.fr
*/

/*!
\generic_comment apply_range

    Cette fonction est une variante de \\a apply . 
    La syntaxe générale est 
    \code
         booléen aply_range( vecteur, début, fin, opérateur, paramètres faculatatifs de l'opérateur ) 
         
    Elle permet d'appliquer l'opérateur sur les indices de l'intervalle [début;fin[ (<strong> Attention : fin n'est pas compris </strong>).

    \\keyword Fonctionalité/Transformer
    \\relates Vec 
    \\friend hugo.leclerc@lmt.ens-cachan.fr
    \\friend samir.amrouche@lmt.ens-cachan.fr
*/

/*!
\\generic_comment remove_if

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

    \\keyword Fonctionalité/Transformer
    \\relates Vec 
    \\relates apply
    \\friend hugo.leclerc@lmt.ens-cachan.fr
    \\friend samir.amrouche@lmt.ens-cachan.fr
*/

/*!
\\generic_comment find_elem

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

    \\keyword Fonctionalité/Extraire
    \\relates Vec 
    \\relates apply
    \\friend hugo.leclerc@lmt.ens-cachan.fr
    \\friend samir.amrouche@lmt.ens-cachan.fr
*/

"""


# apply, find
for tt in ['void','int']:
    TT = ['class TT']
    TV = 'Vec<TT,s,'+tt+'>'
    print_apply_ext('apply'                ,TT,TV,'for(unsigned i=0;i<v.size();++i) op(v[i],PARALIST);')

    print_apply_ext('apply_nz'             ,TT,TV,'apply(v,op,PARALIST);')
    print_apply_ext('apply_nz_unique'      ,TT,TV,'apply_nz(v,op,PARALIST);')
    print_apply_ext('apply_range'          ,TT,TV,'DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],PARALIST);',suppar=['int from','int to'])
    print_apply_ext('apply_range_stride'   ,TT,TV,'DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],PARALIST);',suppar=['int from','int to','int inc'])
    for inc in [1,2,4]:
        print_apply_ext(
            'apply_range_by_n',TT,TV,
            'for(;from<to-'+str(inc-1)+';from+='+str(inc)+') op('+string.join(map(lambda x:'v[from+'+str(x)+']',range(inc)),",")+',PARALIST); '+'for(;from<to;++from) op(v[from],PARALIST);'*(inc>1),
            suppar=['int from','int to','const Number<'+str(inc)+'> &n_inc']
        )
    print_apply_ext(
        'apply_on_number',TT,TV,
        'op(v[num],PARALIST);',
        suppar=['int num']
    )

    print_apply_ext('apply_wi'             ,TT,TV,'for(unsigned i=0;i<v.size();++i) op(v[i],i,PARALIST);')
    print_apply_ext('apply_nz_wi'          ,TT,TV,'apply_wi(v,op,PARALIST);')
    print_apply_ext('apply_range_wi'       ,TT,TV,'DEBUGASSERT(to<=(int)v.size()); for(;from<to;++from) op(v[from],from,PARALIST);',suppar=['int from','int to'])
    print_apply_ext('apply_range_stride_wi',TT,TV,'DEBUGASSERT(to<=(int)v.size()); for(;from<to;from+=inc) op(v[from],from,PARALIST);',suppar=['int from','int to','int inc'])

    print_apply_ext('find_w_wo_ind'        ,TT,TV,'return reduction( generate(v,op,PARALIST), Or() );',ret='bool',suppar=['const Number<0> &ni'])
    print_apply_ext('find_w_wo_ind'        ,TT,TV,'for(unsigned i=0;i<v.size();++i) if (op(v[i],PARALIST)) return true; return false;',ret='bool',suppar=['const Number<1> &ni'])
    
    print_apply_ext('find'                 ,TT,TV,'return find_w_wo_ind(v,Number<HasIndexAccess<TT>::res>(),op,PARALIST);',ret='bool')
    
    RET = 'typename '+TV+'::template SubType<0>::T'
    print_apply_ext( 'find_elem', TT, TV,
        'STATICASSERT((Vec<TT,s,'+tt+'>::nb_sub_type==1)); for(unsigned i=0;i<v.size();++i) if (op(v[i],PARALIST)) return &v[i]; return (CONSTK '+RET+' *)NULL;',
        ret='CONSTK '+RET+' *'
    )

# simd
    print_apply_ext(
      'apply_simd_w_wo_simd_method',TT+['unsigned ne'],TV,
      'apply(v,op,PARALIST);',
      nsp='LMTPRIVATE',suppar=['const Number<0> &,const Number<ne> &NE']
    )
    print_apply_ext(
      'apply_simd_w_wo_simd_method',TT+['unsigned ne'],TV,"""
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),PARALIST);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),PARALIST);
        for(;i<(int)v.size();++i) op(v[i],PARALIST);
    """,
      nsp='LMTPRIVATE',suppar=['const Number<1> &,const Number<ne> &NE']
    )
    print_apply_ext(
      'apply_simd_w_wo_simd_method',TT+['unsigned ne'],TV,"""
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),PARALIST);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),PARALIST);
        for(;i<(int)v.size();++i) op(v[i],PARALIST);
    """,
      nsp='LMTPRIVATE',suppar=['const Number<2> &,const Number<ne> &NE']
    )
    print_apply_ext('apply_simd',TT,TV,
        'LMTPRIVATE::apply_simd_w_wo_simd_method(v,Number<(VecSupportSIMD<%s >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<%s >::res>(),op,PARALIST);'%(TV,TV)
    )

# sim_wi
    print_apply_ext(
      'apply_simd_wi_w_wo_simd_method',TT+['unsigned ne'],TV,
      'apply_wi(v,op,PARALIST);',
      nsp='LMTPRIVATE',suppar=['const Number<0> &,const Number<ne> &NE']
    )
    print_apply_ext(
      'apply_simd_wi_w_wo_simd_method',TT+['unsigned ne'],TV,"""
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(get_simd(v,i+3*ne,NE),get_simd(v,i+2*ne,NE),get_simd(v,i+ne,NE),get_simd(v,i,NE),i,PARALIST);
        for(;i<int(v.size()-ne+1);i+=ne) op(get_simd(v,i,NE),i,PARALIST);
        for(;i<(int)v.size();++i) op(v[i],i,PARALIST);
    """,
      nsp='LMTPRIVATE',suppar=['const Number<1> &,const Number<ne> &NE']
    )
    print_apply_ext(
      'apply_simd_wi_w_wo_simd_method',TT+['unsigned ne'],TV,"""
        int i;
        for(i=0;i<int(v.size()-4*ne+1);i+=4*ne) op(v.simd(i+3*ne,NE),v.simd(i+2*ne,NE),v.simd(i+ne,NE),v.simd(i,NE),i,PARALIST);
        for(;i<int(v.size()-ne+1);i+=ne) op(v.simd(i,NE),i,PARALIST);
        for(;i<(int)v.size();++i) op(v[i],i,PARALIST);
    """,
      nsp='LMTPRIVATE',suppar=['const Number<2> &,const Number<ne> &NE']
    )
    print_apply_ext('apply_simd_wi',TT,TV,
        'LMTPRIVATE::apply_simd_wi_w_wo_simd_method(v,Number<(VecSupportSIMD<%s >::res+1)*(s==-1||s>=16)>(),Number<SimdSizeForVec<%s >::res>(),op,PARALIST);'%(TV,TV)
    )

# remove_if
    print_apply_ext('remove_if',TT,TV,"""
        for(unsigned i=0;i<v.size();++i) {
            if ( op(v[i],PARALIST) ) {
                v[i--] = v.back();
                v.pop_back();
            }
        }
    """,onlyfornonconstvec=True)

print '}'
