#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <string.h> /// pour strcmp

using namespace std;
#include "token.h"
#include "op.h"
#include "comment.h"
#include "commentitem.h"
#include "commentitemkeyword.h"
#include "commentitemtxt.h"
#include "commentitemcode.h"
#include "commentitemverbatim.h"
#include "commentitemsection.h"
#include "commentitemtable.h"
#include "commentitemlist.h"
#include "commentitemwebpage.h"
#include "commentitemgenericcomment.h"
#include "commentitemexample.h"
#include "commentitemtutorial.h"
#include "commentitemlatex.h"
#include "visitorcommentitem_tohtml.h"
#include "visitorcommentitem_listtarget.h"

//#define INDEX_CARACTERISTIC_TXT 15
//#define INDEX_TO 18

#define SIZE_TABLE_KEYWORD 22

// i.e. dans le tableau ci-dessous à partir de brief ce sont des mot-clés qui caractérisent le commentaire
//#define INDEX_CARACTERISTIC 8

const char* Comment::keyword[SIZE_TABLE_KEYWORD] = {
                                         "relates",  // ancre
                                         "friend",
                                         "struct",
                                         "latex", // fonction
                                         "generic_comment",
                                         "author", // 5
                                         "example",
                                         "tutorial",
                                         "brief", // 8
                                         "property",
                                         "mainclass", // 10
                                         "keyword", // 11
                                         "param",
                                         "return",
                                         "internal",
                                         "code",  // 15
                                         "table",
                                         "webpage", // 17
                                         "main",  // 18
                                         "verbatim", // 19
                                         "a", // 20
                                         "anchor" // 21
                                     };
// ----------------------------------------------------
struct LangRef {
    virtual void print( std::ostream &os ) const = 0;
    virtual ~LangRef() {}
}
;

struct LangRefFunction : public LangRef {
    virtual void print( std::ostream &os ) const {}
    virtual ~LangRefFunction() {}
}
;

Comment::~Comment() {

    for( int i = 0; i < items.size(); i++ ) 
        delete items[ i ];
}


//cette fonction ajoute à la liste pointée par ptr_lt les exemples, tutorials et commentaires sur les fonctions et classes génériques contenus dans cet objet Comment.
void Comment::add_tutorial_example_genericComment_webpage_image( ListTarget* ptr_lt  ) {

    int n,i;
    VisitorCommentItem_listTarget visi( ptr_lt );

    n = items.size();
    for(i=0;i<n;i++)
        items[i]->execute_function( &visi );
}


void Comment::apply_on_related_to( const std::string &s, Op *op ) {

    // for(unsigned i=0;i<items.size();++i)
    //            items[i]->apply_on_related_to( s, op );
}

int Comment::findKeyword( int* position,int* positionAfter, string& s,int end,int start) {

    string token;
    int pos,pos2,j;
    char delim;
    
    j = -1;
    pos = -1;
    pos2 = -1;
    
    //cout << "line = {" << s.substr(start,end-start) << "}" << endl;
    
    if (chercher_motif( s,"\\",&pos,end,start )) {
        pos = extraire_token( &pos2,&delim,"[]{}<>&*/:;()=\n \t",s,end,pos+1 );/// après l'appel pos2 pointe sur le caractère qui suit le token
        if ( pos < s.size() and pos2 <= s.size() ) {
            token = s.substr(pos,pos2-pos);
            //cout << " token = ||" << token << "||" << endl;
            for(j=0;j<SIZE_TABLE_KEYWORD;j++)
                if (token == Comment::keyword[j])
                    break;
        }
    }
    *position = pos;
    *positionAfter = pos2;
    return j;
}

int findKeyword_( int* position,int* positionAfter, const char* s,int end,int start) {
    int pos,pos2,j;
    char delim;

    j = -1;
    pos = -1;
    pos2 = -1;
    //cout << " vrai début |" << s[start] << "| = " << (int) s[start] << " start= " << start << endl;
    if (chercher_motif( s,"\\",&pos,end,start )) {
        //cout << " début |" << s[pos] << "| = " << (int) s[pos] << " pos = " << pos << endl;
        pos = extraire_token( &pos2,&delim,"[]{}<>&*/:;()=\n \t",s,end,pos+1 );/// après l'appel pos2 pointe sur le caractère qui suit le token
        //cout << " ooooooooooooooooooooo |"; cout.write(s+pos,pos2-pos); cout << "|" <<endl;
        for(j=0;j<SIZE_TABLE_KEYWORD;j++) 
            if (not(strncmp(s+pos,Comment::keyword[j],strlen(Comment::keyword[j]))))
                break;
    } //else
        //cout << " pppppppppppppppppppppppppppppppppppppppppppp "<< endl;
    *position = pos;
    *positionAfter = pos2;
    if (j == SIZE_TABLE_KEYWORD)
        j = -1;
    return j;
}

bool Comment::chercher_section( int* nb, string& s, int end,int start  ) {
    int p;
 
    *nb = -1;
    for(p=start;p<end;p++) if (s[p] != ' ') break;
    if (p == end) return false;
    if (s[p] == '=') {
        *nb = p;
        return true;
    } else 
        return false;
}

bool chercher_section_( int* nb, const char* s, int end, int start  ) {
    int p;
 
    *nb = -1;
    for(p=start;p<end;p++) if (s[p] != ' ') break;
    if (p == end) return false;
    if (s[p] == '=') {
        *nb = p;
        return true;
    } else 
        return false;
}

bool Comment::chercher_liste( int* nb, char* type, string& s,int end,int start ) {
    int p;
    char c;
 
    *nb = -1;
    for(p=start;p<end;p++) if (s[p] != ' ') break;
    if (p == end) return false;
    c = s[p];
    if ((s[p] == '*') || (s[p] == '#') || (s[p] == '-')) {
        *nb = p;
        *type = c;
        return true;
    }
    return false;
}

bool chercher_liste_( int* nb, char* type, const char* s, int end, int start ) {
    int p;
    char c;
 
    *nb = -1;
    for(p=start;p<end;p++) if (s[p] != ' ') break;
    if (p == end) return false;
    c = s[p];
    if ((s[p] == '*') || (s[p] == '#') || (s[p] == '-')) {
        *nb = p;
        *type = c;
        return true;
    }
    return false;
}

// void Comment::parse( vector<CommentItem*>& listItems, const char* s, int size ) {
// 
//     string cmal;
// 
//     cmal.append(s,size);
//     parse(listItems,cmal,cmal.size(),0);
// }


void Comment::parse( vector<CommentItem*>& listItems, const char* s, int size ) {

    int n,i,pos,j,c,nb_espaces_reference,pos2,nb,next,decalage_ref,start_tuto_example, start;
    vector<int> order_section;
    string token;
    string token2;
    CommentItemKeyword*        ptr_CommentItemKeyword;
    CommentItemTxt*            ptr_CommentItemTxt;
    CommentItemCode*           ptr_CommentItemCode;
    CommentItemVerbatim*       ptr_CommentItemVerbatim;
    CommentItemSection*        ptr_CommentItemSection;
    CommentItemTable*          ptr_CommentItemTable;
    CommentItemList*           ptr_CommentItemList;
    CommentItemWebPage*        ptr_CommentItemWebPage;
    CommentItemGenericComment* ptr_CommentItemGenericComment;
    CommentItemExample*        ptr_CommentItemExample;
    CommentItemTutorial*       ptr_CommentItemTutorial;
    CommentItemLaTex*          ptr_CommentItemLaTex;
    char caractere;

    start = 0;
    while(start<size) {
        //cout << " ::::::::::::::: BOUCLE ::::::::::::::::::::::::::::::::" << endl;
        if (!chercher_motif( s,"\n",&next,size,start )) next = size;
        j = findKeyword_(&pos,&pos2,s,next,start ); ///après l'appel pos contient l'indice du premier caractère du mot clé et pos2 celui du premier caractère après le token
        //cout << "--j = " << j << endl;
        //cout << " chaine |"; cout.write(s+start,next-start);cout << "| fin de chaine"<< endl;
        //cout << " start = " << start << "   ,   next = " <<  next << endl;

        switch(j) {
            case 0 : case 1 : case 2 : case 5 : case 8 : case 9 : case 10 : case 11 : case 12 : case 13 : case 14 : case 18 :
                ptr_CommentItemKeyword = new CommentItemKeyword( keyword[j] );
                listItems.push_back( ptr_CommentItemKeyword );   //addCommentItem( ptr_CommentItemKeyword );
                token.assign( s + pos2, next - pos2 );//token = file.substr(pos2,next-pos2 );
                cut_space( token );
                ptr_CommentItemKeyword->addParameter( token );
                //cout << " plout " << endl;
                start = next + 1; /// + 1 pour sauter le \n
                break;
            case 3 :    /// pour un commentaire au format LaTex
                //token.assign( s + pos2, next - pos2 ); // récupération du header
                //cut_space( token );
                ptr_CommentItemLaTex = new CommentItemLaTex( token );
                ptr_CommentItemLaTex->source_file = source_file;
                listItems.push_back( ptr_CommentItemLaTex );  //addCommentItem( ptr_CommentItemCode );
                decalage_ref = pos - start - 1;// pos contient la position de \latex, start celle du début de ligne
                start = next + 1; // on passe à la ligne suivante 
                start_tuto_example = start;
                while (start<size)  {
                    if (!chercher_motif( s,"\n",&next,size,start )) next = size;
                    c = indentation( &pos2, s, start ); 
                    if ((c != LINE_VACUUM) && (c <= decalage_ref)) break; // problème potentiel --- pour distinguer la fin d'un code
                //ptr_CommentItemLaTex->addTxt( file,next,pos2);
                    start = next + 1; // + 1 pour sauter le \n
                }
                ptr_CommentItemLaTex->addTxt( s + start_tuto_example, start - start_tuto_example );
                break;

            case 4 : /// pour un commentaire du type generic_comment
                token.assign( s + pos2, next - pos2 );//token = file.substr(pos2,next-pos2 ); // récupération de l'éventuelle référence
                cut_space( token );
                ptr_CommentItemGenericComment = new CommentItemGenericComment( token );
                ptr_CommentItemGenericComment->source_file = source_file;
                listItems.push_back( ptr_CommentItemGenericComment ); //addCommentItem( ptr_CommentItemGenericComment );
                decalage_ref = pos - start - 1;
                start = next + 1; // on passe à la ligne suivante 
                start_tuto_example = start;
                while (start<size)  {
                    if (!chercher_motif( s,"\n",&next,size,start )) next = size;
                    c = indentation( &pos2,s,start ); // pos contient la position de \code, start celle du début de ligne
                    if ((c != LINE_VACUUM) && (c <= decalage_ref)) break; // problème potentiel --- pour distinguer la fin d'un exemple ou d'un tutoriel
                    start = next + 1; // + 1 pour sauter le \n
            //cout << " ~~~~~~~~~~~~" << endl;
                }
                parse( ptr_CommentItemGenericComment->items, s + start_tuto_example, start - start_tuto_example );
                break;

            case 6 : case 7 : /// on a le début d'un exemple ou d'un tutoriel
                /// on détermine les lignes qui correspondent à l'exemple via l'indentation
                decalage_ref = pos - start - 1;// pos contient la position de \example, start celle du début de ligne
                token.assign( s + pos2, next -pos2 ); // on enregistre la référence ou le titre de l'exemple ou du tutoriel
                cut_space( token );
                start = next + 1; // on passe à la ligne suivante 
                start_tuto_example = start;
                while (start<size)  {
                    if (!chercher_motif( s, "\n", &next, size, start ) ) next = size;
                    c = indentation( &pos2, s, start ); // pos contient la position de \code, start celle du début de ligne
                    if ( (c != LINE_VACUUM ) && ( c <= decalage_ref ) ) break; // problème potentiel --- pour distinguer la fin d'un exemple ou d'un tutoriel
                    start = next + 1; // + 1 pour sauter le \n
                    //cout << " ~~~~~~~~~~~~" << endl;
                }
                //cout << " start = " << start << endl;
                //cout << " start_tuto_example = " << start_tuto_example << endl;
                if (j==6) {
                    /// c'est un exemple
                    ptr_CommentItemExample = new CommentItemExample( token );
                    ptr_CommentItemExample->source_file = source_file;
                    listItems.push_back( ptr_CommentItemExample ); //addCommentItem( ptr_CommentItemExample );
                    //cout << " ~~~~~~~~~~~~  AVANT LE PARSING RECURSIF  ~~~~~~~~~~~~~~~~~~~~~" << endl;
                    parse( ptr_CommentItemExample->items,s+start_tuto_example,start-start_tuto_example );
                    //cout << " ~~~~~~~~~~~~  JE PASSE LE PARSING RECURSIF  ~~~~~~~~~~~~~~~~~~~~~" << endl;
                } else {
                    /// c'est un tutoriel
                    ptr_CommentItemTutorial = new CommentItemTutorial( token );
                    ptr_CommentItemTutorial->source_file = source_file;
                    listItems.push_back( ptr_CommentItemTutorial ); //addCommentItem( ptr_CommentItemTutorial );
                    parse( ptr_CommentItemTutorial->items,s+start_tuto_example,start-start_tuto_example );
                }
                break;
            case 15 : /// pour un code
                token.assign(s+pos2,next-pos2 ); // récupération du langage
                cut_space( token );
                ptr_CommentItemCode = new CommentItemCode( token );
                listItems.push_back( ptr_CommentItemCode );  //addCommentItem( ptr_CommentItemCode );
                decalage_ref = pos - start - 1;// pos contient la position de \code, start celle du début de ligne de \code
                start = next + 1; // on passe à la ligne suivante 
                nb_espaces_reference = 0;
                while (start<size)  {
                    if (!chercher_motif( s,"\n",&next,size,start )) next = size;
                    c = indentation( &pos2,s,start ); 
                    if ((c != LINE_VACUUM) && (c <= decalage_ref)) break; // problème potentiel --- pour distinguer la fin d'un code
                    if ((nb_espaces_reference == 0) && (c != LINE_VACUUM)) nb_espaces_reference = c - decalage_ref; 
                    token.assign(s+pos2,next-pos2 );
                    if (c != LINE_VACUUM)
                        c = (c-decalage_ref)/nb_espaces_reference - 1;
                    else
                        c = 0;
                    ptr_CommentItemCode->addTabulation_String( c, token);
                    start = next + 1; // + 1 pour sauter le \n
                }
                break;
            case 19 : /// pour un mode verbatim
                ptr_CommentItemVerbatim = new CommentItemVerbatim();
                listItems.push_back( ptr_CommentItemVerbatim );  //addCommentItem( ptr_CommentItemCode );
                decalage_ref = pos - start - 1;// pos contient la position de \code, start celle du début de ligne
                start = next + 1; // on passe à la ligne suivante 
                nb_espaces_reference = 0;
                while (start<size)  {
                    if (!chercher_motif( s,"\n",&next,size,start )) next = size;
                    c = indentation( &pos2,s,start ); 
                    if ((c != LINE_VACUUM) && (c <= decalage_ref)) break; // problème potentiel --- pour distinguer la fin d'un code
                    if ((nb_espaces_reference == 0) && (c != LINE_VACUUM)) nb_espaces_reference = c - decalage_ref;
                    token.assign(s+pos2,next-pos2 );
                    if (c != LINE_VACUUM)
                        c = (c-decalage_ref)/nb_espaces_reference - 1;
                    else
                        c = 0;
                    ptr_CommentItemVerbatim->addTabulation_String( c, token);
                    start = next + 1; // + 1 pour sauter le \n
                }
                break;
            case 16 : /// pour un tableau
                token.assign( s + pos2, next - pos2 ); // récupération du header
                cut_space( token );
                ptr_CommentItemTable = new CommentItemTable( token );
                listItems.push_back( ptr_CommentItemTable ); // addCommentItem( ptr_CommentItemTable );
                decalage_ref = pos - start - 1;// pos contient la position de \code, start celle du début de ligne
                start = next + 1; // on passe à la ligne suivante 
                nb_espaces_reference = 0;
                while (start<size)  {
                    if (!chercher_motif( s,"\n",&next,size,start )) next = size;
                    c = indentation( &pos2,s,start ); // pos contient la position de \table
                    if (nb_espaces_reference == 0) nb_espaces_reference = c; 
                    if (c <= decalage_ref) 
                        break;// problème potentiel pour distinguer la fin d'un tableau
                    token.assign(s+pos2,next-pos2 );
                    ptr_CommentItemTable->addLine( token );
                    start = next + 1;
                }
                break;
            case 17 : /// pour une page web
                decalage_ref = pos - start - 1;// pos contient la position de \webpage, start celle du début de ligne
                token.assign(s+pos2,next-pos2 ); // récupération des paramètres
                cut_space( token );
                start = next + 1; // on passe à la ligne suivante 
                start_tuto_example = start;
                while (start<size)  {
                    if (!chercher_motif( s,"\n",&next,size,start )) next = size;
                    c = indentation( &pos2,s,start ); // pos contient la position de \code, start celle du début de ligne
                    if ((c != LINE_VACUUM) && (c <= decalage_ref)) break; // problème potentiel --- pour distinguer la fin d'un exemple ou d'un tutoriel
                    start = next + 1; // + 1 pour sauter le \n
            //cout << " ~~~~~~~~~~~~" << endl;
                }
                //cout << "++++++++++++++++++++++  token web page +++++++++++++++++++++++" << endl;
                //cout << " token2 page web =" << token2 << "|||" << endl;
                ptr_CommentItemWebPage = new CommentItemWebPage( token );
                ptr_CommentItemWebPage->source_file = source_file;
                listItems.push_back( ptr_CommentItemWebPage );  //addCommentItem( ptr_CommentItemWebPage );
                parse( ptr_CommentItemWebPage->items,s+start_tuto_example,start-start_tuto_example );
                break;
            case SIZE_TABLE_KEYWORD : /// token inconnu
                //cout << "--------------------  token inconnu ---------------------" << endl;
                //cout << " token =" << token << "|||" << endl;
                //cout << " j=" << j << endl;
                ptr_CommentItemTxt = new CommentItemTxt();
                listItems.push_back( ptr_CommentItemTxt );  // addCommentItem( ptr_CommentItemTxt );
                token.assign(s+start,next-start );
                cut_space( token );
                if (token.size() > 0)
                    ptr_CommentItemTxt->addString( token );
                start = next + 1;
                break;
            case 20 : case 21 :
            default : /// pas de token trouvé ou \a ou \anchor
                    /// c'est du texte ou une section ou bien une liste
                if ( chercher_section_(&pos,s,next,start) ) { /// c'est un titre de section
                    //cout << "--------- SECTION -------------" << endl;
                    token.assign(s+pos+1,next-pos-1 );
                    cut_space( token );
                    j = pos-start;
                    for(i=0;i<order_section.size();i++) {
                        if (j == order_section[i])
                            break;
                    }
                    if (i==order_section.size()) {
                        order_section.push_back(j);
                    }
                    ptr_CommentItemSection = new CommentItemSection( token,i+1 );
                    listItems.push_back( ptr_CommentItemSection );  // addCommentItem( ptr_CommentItemSection );
                    start = next + 1;
                } else {
                    if (chercher_liste_(&pos,&caractere,s,next,start)) {/// c'est une liste
                        //cout << "--------- liste -------------" << endl;
                        token.assign(s+pos+1,next-pos-1 );
                        cut_space( token );
                        ptr_CommentItemList = new CommentItemList( );
                        listItems.push_back( ptr_CommentItemList ); //addCommentItem( ptr_CommentItemList );
                        ptr_CommentItemList->addCaracteristic( token,pos-start,caractere );
                        start = next + 1;
                        while (start<size) {
                            if (!chercher_motif( s,"\n",&next,size,start )) next = size;
                            if (chercher_liste_(&pos,&caractere,s,next,start )) {
                                token.assign(s+pos+1,next-pos-1 );
                                cut_space( token );
                                ptr_CommentItemList->addCaracteristic( token,pos-start,caractere );
                            } else 
                                break;
                                start = next + 1; //i++;
                        }
                    } else {/// c'est du simple texte
                        ptr_CommentItemTxt = new CommentItemTxt();
                        listItems.push_back( ptr_CommentItemTxt ); // addCommentItem( ptr_CommentItemTxt );
                        token.assign(s+start,next-start);
                        cut_space( token );
                        if (token.size()>0)
                            ptr_CommentItemTxt->addString( token );
                        //cout << " boucle ttxt ,,, start = " << start << "   ,,, next = " << next << endl;
                        start = next + 1;
                        while (start<size) {
                            if (!chercher_motif( s,"\n",&next,size,start )) next = size;
                            if (chercher_liste_(&pos,&caractere,s,next,start) || chercher_section_(&pos,s,next,start )) break;
                            j = findKeyword_( &pos,&pos2,s,next,start );
                            //cout << "~~~~~~~~~~~~~~ boucle ~~~~~~~~~~~~~~~~ j = " << j << " next = " << next << endl;
                            if (j>=0) break; //if ((j>=0) && (j<INDEX_TO)) break; // un token intéressant fut trouvé
                            token.assign(s+start,next-start);
                            cut_space( token );
                            if (token.size() > 0)
                                ptr_CommentItemTxt->addString( token );
                            start = next + 1; 
                        }
                    }
                }
            }
        //start = next + 1; // GROSSE ERREUR de modifier le start ici
    }
}


void Comment::parse( vector<CommentItem*>& listItems, string& file,int end, int start ) {

    int n,i,pos,j,c,nb_espaces_reference,pos2,nb,next,decalage_ref,start_tuto_example;
    vector<int> order_section;
    string token;
    string token2;
    CommentItemKeyword*        ptr_CommentItemKeyword;
    CommentItemTxt*            ptr_CommentItemTxt;
    CommentItemCode*           ptr_CommentItemCode;
    CommentItemVerbatim*       ptr_CommentItemVerbatim;
    CommentItemSection*        ptr_CommentItemSection;
    CommentItemTable*          ptr_CommentItemTable;
    CommentItemList*           ptr_CommentItemList;
    CommentItemWebPage*        ptr_CommentItemWebPage;
    CommentItemGenericComment* ptr_CommentItemGenericComment;
    CommentItemExample*        ptr_CommentItemExample;
    CommentItemTutorial*       ptr_CommentItemTutorial;
    CommentItemLaTex*          ptr_CommentItemLaTex;
    char caractere;

    //cout << "================== lecture des lignes =================================" << endl;
    //while(start<end) {
        //cout << " ::::::::::::::: BOUCLE ::::::::::::::::::::::::::::::::" << endl;
        //if (!chercher_motif( file,"\n",&next,end,start )) next = end;
        //cout << file.substr( start,next-start ) << endl;
        //start = next + 1;
    //}
    //cout << "=======================================================================" << endl;

    while(start<end) {
        //cout << " ::::::::::::::: BOUCLE ::::::::::::::::::::::::::::::::" << endl;
        if (!chercher_motif( file,"\n",&next,end,start )) next = end;
        j = findKeyword(&pos,&pos2,file,next,start ); //après l'appel pos contient l'indice du premier caractère du mot clé et pos2 celui du premier caractère après le token
        //cout << "--j = " << j << endl;
        //cout << " start = " << start << "   ,   next = " <<  next << endl;

        switch(j) {
            case 0 : case 1 : case 2 : case 5 : case 8 : case 9 : case 10 : case 11 : case 12 : case 13 : case 14 : case 18 :
                ptr_CommentItemKeyword = new CommentItemKeyword( keyword[j] );
                listItems.push_back( ptr_CommentItemKeyword );   //addCommentItem( ptr_CommentItemKeyword );
                token = file.substr( pos2, next - pos2 );
                cut_space( token );
                ptr_CommentItemKeyword->addParameter( token );
                //cout << " plout " << endl;
                start = next + 1; /// + 1 pour sauter le \n
                break;

            case 3 :    /// pour un commentaire au format LaTex
                token = file.substr( pos2, next - pos2 ); /// récupération de l'éventuelle option
                cut_space( token );
                ptr_CommentItemLaTex = new CommentItemLaTex( token );
                ptr_CommentItemLaTex->source_file = source_file;
                listItems.push_back( ptr_CommentItemLaTex );  //addCommentItem( ptr_CommentItemCode );
                decalage_ref = pos - start - 1; /// pos contient la position de \latex, start celle du début de ligne
                start = next + 1; /// on passe à la ligne suivante 
                start_tuto_example = start;
                while ( start < end ) {
                    if (!chercher_motif( file, "\n", &next, end, start ) ) next = end;
                    c = indentation( &pos2, file, start ); 
                    if ( ( c != LINE_VACUUM ) && ( c <= decalage_ref ) ) break; // problème potentiel --- pour distinguer la fin d'un code
                    //ptr_CommentItemLaTex->addTxt( file,next,pos2);
                    start = next + 1; // + 1 pour sauter le \n
                }
                ptr_CommentItemLaTex->addTxt( file, start, start_tuto_example );
                break;

            case 4 : // pour un commentaire du type generic_comment
                token = file.substr( pos2, next - pos2 ); // récupération de l'éventuelle référence
                cut_space( token );
                ptr_CommentItemGenericComment = new CommentItemGenericComment( token );
                ptr_CommentItemGenericComment->source_file = source_file;
                listItems.push_back( ptr_CommentItemGenericComment ); //addCommentItem( ptr_CommentItemGenericComment );
                decalage_ref = pos - start - 1;
                start = next + 1; /// on passe à la ligne suivante 
                start_tuto_example = start;
                while ( start < end ) {
                    if (!chercher_motif( file,"\n",&next,end,start )) next = end;
                    c = indentation( &pos2,file,start ); // pos contient la position de \code, start celle du début de ligne
                    if ((c != LINE_VACUUM) && (c <= decalage_ref)) break; // problème potentiel --- pour distinguer la fin d'un exemple ou d'un tutoriel
                    start = next + 1; // + 1 pour sauter le \n
                    //cout << " ~~~~~~~~~~~~" << endl;
                }
                parse( ptr_CommentItemGenericComment->items, file, start, start_tuto_example );
                break;

            case 6 : case 7 : /// on a le début d'un exemple ou d'un tutoriel
                /// on détermine les lignes qui correspondent à l'exemple via l'indentation
                decalage_ref = pos - start - 1;// pos contient la position de \example, start celle du début de ligne
                token = file.substr(pos2,next-pos2 ); // on enregistre la référence ou le titre de l'exemple ou du tutoriel
                cut_space( token );
                start = next + 1; // on passe à la ligne suivante 
                start_tuto_example = start;
                while (start<end)  {
                    if (!chercher_motif( file,"\n",&next,end,start )) next = end;
                    c = indentation( &pos2,file,start ); // pos contient la position de \code, start celle du début de ligne
                    if ((c != LINE_VACUUM) && (c <= decalage_ref)) break; // problème potentiel --- pour distinguer la fin d'un exemple ou d'un tutoriel
                    start = next + 1; // + 1 pour sauter le \n
                    //cout << " ~~~~~~~~~~~~" << endl;
                }
                //cout << " start = " << start << endl;
                //cout << " start_tuto_example = " << start_tuto_example << endl;
                if (j==6) {
                    // c'est un exemple
                    ptr_CommentItemExample = new CommentItemExample( token );
                    ptr_CommentItemExample->source_file = source_file;
                    listItems.push_back( ptr_CommentItemExample ); //addCommentItem( ptr_CommentItemExample );
                    //cout << " ~~~~~~~~~~~~  AVANT LE PARSING RECURSIF  ~~~~~~~~~~~~~~~~~~~~~" << endl;
                    parse( ptr_CommentItemExample->items,file,start,start_tuto_example );
                    //cout << " ~~~~~~~~~~~~  JE PASSE LE PARSING RECURSIF  ~~~~~~~~~~~~~~~~~~~~~" << endl;
                } else {
                    // c'est un tutoriel
                    ptr_CommentItemTutorial = new CommentItemTutorial( token );
                    ptr_CommentItemTutorial->source_file = source_file;
                    listItems.push_back( ptr_CommentItemTutorial ); //addCommentItem( ptr_CommentItemTutorial );
                    parse( ptr_CommentItemTutorial->items,file,start,start_tuto_example );
                }
                break;

            case 15 : // pour un code
                token = file.substr(pos2,next-pos2 ); // récupération du langage
                cut_space( token );
                ptr_CommentItemCode = new CommentItemCode( token );
                listItems.push_back( ptr_CommentItemCode );  //addCommentItem( ptr_CommentItemCode );
                decalage_ref = pos - start - 1;// pos contient la position de \code, start celle du début de ligne de \code
                start = next + 1; // on passe à la ligne suivante 
                nb_espaces_reference = 0;
                while (start<end)  {
                    if (!chercher_motif( file,"\n",&next,end,start )) next = end;
                    c = indentation( &pos2,file,start ); 
                    if ((c != LINE_VACUUM) && (c <= decalage_ref)) break; // problème potentiel --- pour distinguer la fin d'un code
                    if ((nb_espaces_reference == 0) && (c != LINE_VACUUM)) nb_espaces_reference = c - decalage_ref; 
                    token = file.substr(pos2,next-pos2 );
                    if (c != LINE_VACUUM)
                        c = (c-decalage_ref)/nb_espaces_reference - 1;
                    else
                        c = 0;
                    ptr_CommentItemCode->addTabulation_String( c, token);
                    start = next + 1; // + 1 pour sauter le \n
                }
                break;

            case 19 : // pour un mode verbatim
                ptr_CommentItemVerbatim = new CommentItemVerbatim();
                listItems.push_back( ptr_CommentItemVerbatim );  //addCommentItem( ptr_CommentItemCode );
                decalage_ref = pos - start - 1;// pos contient la position de \code, start celle du début de ligne
                start = next + 1; // on passe à la ligne suivante 
                nb_espaces_reference = 0;
                while (start<end)  {
                    if (!chercher_motif( file,"\n",&next,end,start )) next = end;
                    c = indentation( &pos2,file,start ); 
                    if ((c != LINE_VACUUM) && (c <= decalage_ref)) break; // problème potentiel --- pour distinguer la fin d'un code
                    if ((nb_espaces_reference == 0) && (c != LINE_VACUUM)) nb_espaces_reference = c - decalage_ref;
                    token = file.substr(pos2,next-pos2 );
                    if (c != LINE_VACUUM)
                        c = (c-decalage_ref)/nb_espaces_reference - 1;
                    else
                        c = 0;
                    ptr_CommentItemVerbatim->addTabulation_String( c, token);
                    start = next + 1; // + 1 pour sauter le \n
                }
                break;

            case 16 : // pour un tableau
                token = file.substr(pos2,next-pos2 ); // récupération du header
                cut_space( token );
                ptr_CommentItemTable = new CommentItemTable( token );
                listItems.push_back( ptr_CommentItemTable ); // addCommentItem( ptr_CommentItemTable );
                decalage_ref = pos - start - 1;// pos contient la position de \code, start celle du début de ligne
                start = next + 1; // on passe à la ligne suivante 
                nb_espaces_reference = 0;
                while (start<end)  {
                    if (!chercher_motif( file,"\n",&next,end,start )) next = end;
                    c = indentation( &pos2,file,start ); // pos contient la position de \table
                    if (nb_espaces_reference == 0) nb_espaces_reference = c; 
                    if (c <= decalage_ref) 
                        break;// problème potentiel pour distinguer la fin d'un tableau
                    token = file.substr(pos2,next-pos2 );
                    ptr_CommentItemTable->addLine( token );
                    start = next + 1;
                }
                break;

            case 17 : // pour une page web
                decalage_ref = pos - start - 1;// pos contient la position de \webpage, start celle du début de ligne
                token = file.substr(pos2,next-pos2 ); // récupération des paramètres
                cut_space( token );
                start = next + 1; // on passe à la ligne suivante 
                start_tuto_example = start;
                while (start<end)  {
                    if (!chercher_motif( file,"\n",&next,end,start )) next = end;
                    c = indentation( &pos2,file,start ); // pos contient la position de \code, start celle du début de ligne
                    if ((c != LINE_VACUUM) && (c <= decalage_ref)) break; // problème potentiel --- pour distinguer la fin d'un exemple ou d'un tutoriel
                    start = next + 1; // + 1 pour sauter le \n
                    //cout << " ~~~~~~~~~~~~" << endl;
                }
                //cout << "++++++++++++++++++++++  token web page +++++++++++++++++++++++" << endl;
                //cout << " token2 page web =" << token2 << "|||" << endl;
                ptr_CommentItemWebPage = new CommentItemWebPage( token );
                ptr_CommentItemWebPage->source_file = source_file;
                listItems.push_back( ptr_CommentItemWebPage );  //addCommentItem( ptr_CommentItemWebPage );
                parse( ptr_CommentItemWebPage->items,file,start,start_tuto_example );
                break;

            case SIZE_TABLE_KEYWORD : /// token inconnu
                //cout << "--------------------  token inconnu ---------------------" << endl;
                //cout << " token =" << token << "|||" << endl;
                //cout << " j=" << j << endl;
                ptr_CommentItemTxt = new CommentItemTxt();
                listItems.push_back( ptr_CommentItemTxt );  // addCommentItem( ptr_CommentItemTxt );
                token = file.substr( start,next-start );
                cut_space( token );
                if (token.size() > 0)
                    ptr_CommentItemTxt->addString( token );
                start = next + 1;
                break;

            case 20 : case 21 :
            default : // pas de token trouvé ou \a ou \anchor
                      // c'est du texte ou une section ou bien une liste
                    if ( chercher_section(&pos,file,next,start) ) { // c'est un titre de section
                        //cout << "--------- SECTION -------------" << endl;
                        token = file.substr(pos+1,next-pos-1 );
                        cut_space( token );
                        j = pos-start;
                        for(i=0;i<order_section.size();i++) {
                            if (j == order_section[i])
                                break;
                        }
                        if (i==order_section.size()) {
                            order_section.push_back(j);
                        }
                        ptr_CommentItemSection = new CommentItemSection( token,i+1 );
                        listItems.push_back( ptr_CommentItemSection );  // addCommentItem( ptr_CommentItemSection );
                        start = next + 1;
                    }
                    else {
                        if (chercher_liste(&pos,&caractere,file,next,start)) {// c'est une liste
                            //cout << "--------- liste -------------" << endl;
                            token = file.substr(pos+1,next-pos-1 );
                            cut_space( token );
                            ptr_CommentItemList = new CommentItemList( );
                            listItems.push_back( ptr_CommentItemList ); //addCommentItem( ptr_CommentItemList );
                            ptr_CommentItemList->addCaracteristic( token,pos-start,caractere );
                            start = next + 1;
                            while (start<end) {
                                if (!chercher_motif( file,"\n",&next,end,start )) next = end;
                                if (chercher_liste(&pos,&caractere,file,next,start )) {
                                    token = file.substr(pos+1,next-pos-1 );
                                    cut_space( token );
                                    ptr_CommentItemList->addCaracteristic( token,pos-start,caractere );
                                } else 
                                    break;
                                start = next + 1; //i++;
                            }
                        }
                        else {// c'est du simple texte
                            ptr_CommentItemTxt = new CommentItemTxt();
                            listItems.push_back( ptr_CommentItemTxt ); // addCommentItem( ptr_CommentItemTxt );
                            token = file.substr(start,next-start);
                            cut_space( token );
                            if (token.size()>0)
                                ptr_CommentItemTxt->addString( token );
                            //cout << " boucle ttxt ,,, start = " << start << "   ,,, next = " << next << endl;
                            start = next + 1;
                            while (start<end) {
                                if (!chercher_motif( file,"\n",&next,end,start )) next = end;
                                if (chercher_liste(&pos,&caractere,file,next,start) || chercher_section(&pos,file,next,start )) break;
                                j = findKeyword( &pos,&pos2,file,next,start );
                                //cout << "~~~~~~~~~~~~~~ boucle ~~~~~~~~~~~~~~~~ j = " << j << " next = " << next << endl;
                                if (j>=0) break; //if ((j>=0) && (j<INDEX_TO)) break; // un token intéressant fut trouvé
                                token = file.substr(start,next-start);
                                cut_space( token );
                                if (token.size() > 0)
                                    ptr_CommentItemTxt->addString( token );
                                start = next + 1; 
                            }
                        }
                    }
        }
        //start = next + 1; // GROSSE ERREUR de modifier le start ici
    }
}

// ----------------------------------------------------
std::ostream &operator<<( std::ostream &os, const Comment &c ) {
    int n = c.items.size();
    //os << "type de Comment = " << c.type << endl;
    for(int i=0;i<n;++i) os << *c.items[i] << endl;
    os << " fin de comment" << endl; 
    return os;
}

void Comment::generate_pageHTML( ofstream* ptr_ofstream, ListTarget* plt,Target* parent ){

    int i;
    int n = items.size();
    VisitorCommentItem_toHTML visitor_item_to_html(ptr_ofstream,plt,parent );


    for(i=0;i<n;++i) { 
        items[i]->execute_function( &visitor_item_to_html );
    }
}



