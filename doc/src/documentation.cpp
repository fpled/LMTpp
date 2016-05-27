#include<iostream>
#include<cstdlib>
#include<fstream>
#include<iomanip>
#include<cmath>
#include<string>
#include<vector>
#include<map>
#include<sstream>
#include <stdexcept>
using namespace std;
#include <string.h> /// pour strcmp
/// pour parcourir, manipuler les fichiers sur linux
//#include <unistd.h>
//#include <dirent.h>
//#include <sys/types.h>
//#include <sys/stat.h>   // pour avoir des infos sur les fichiers : par exemple le type
//#include <stdio.h>
//#include <cerrno>
#include <algorithm>
//#include <string.h>

#include "token.h"
#include "op.h"
#include "documentation.h"
#include "pagecomment.h"
#include "opallgenericcomment.h"
#include "listtargetcommentitem.h"
#include "listtarget.h"
#include "listtargetbytype.h"
#include "target.h"
#include "util2html.h"
#include "opwebpage_exists.h"
#include "opget_listkeyword.h"
#include "opinternal.h"

/// pour parcourir, manipuler les fichiers avec Boost
#include "boost/filesystem.hpp"    // includes all needed Boost.Filesystem declarations
using namespace boost::filesystem; //  a namespace alias is preferred practice in real code

Documentation::~Documentation() {

    for(int i=0;i<listPageComment.size(); i++)
        delete listPageComment[i];
}

void Documentation::add_source( const char* name_dir, vector<const char*>& list_excluded_directories ) {

    vector<string> list_source;
    string stmp, stmp2;
    int i,j,n,m;

    addDirSource( name_dir,list_source,&list_excluded_directories );
    n = list_source.size();
    m = listPageComment.size();
//         cerr << " lll <<<<<< " << list_source[i] << endl;

    for(i=0;i<n;i++) {
        stmp = enleve_suffix( list_source[i] );
        for(j=0;j<m;j++) {
            if (stmp == listPageComment[j]->nameWithoutSuffix) {
                stmp2 = suffix( list_source[i] );
                listPageComment[j]->suffix_source = stmp2;
                break;
            }
        }
    }
//     cerr << " >>>>>>>>>>>>> "  << endl;
}

void Documentation::addFileSource( const char* file ) {

    PageComment* ptr_pc = NULL;

    ptr_pc = new PageComment( file );
    if (ptr_pc != NULL ) {
        listPageComment.push_back( ptr_pc );
//         cerr << " on ajoute " << ptr_pc->nameFile << endl;
    }
}

void Documentation::addDirSource( const char* name_dir,vector<string>& listsource, vector<const char*>* ptr_list_excluded_directories ) {
    
    string stmp;
    //string path;
    string stmp2;
    bool on_le_parse;

    stmp = name_dir;
    path dir_path(stmp);
    if (not(exists( dir_path ))) {
        cerr << " Le répertoire n'existe pas." << endl;
        return;
    }
    directory_iterator end_itr; /// default construction yields past-the-end
    for ( directory_iterator itr( dir_path ); itr != end_itr; ++itr ) {
        if ( is_directory(itr->status()) ) {
            //cout << " itr->path().string().c_str() = |" << itr->path().string().c_str() << "|" << endl;
            /// on vérifie que le répertoire ne fait pas parti des répertoires exclus.
            if (ptr_list_excluded_directories != NULL) {
                on_le_parse = true;
                for(int i=0;i<(*ptr_list_excluded_directories).size();i++) {
                    //cout << " (*ptr_list_excluded_directories)[i] = |" << (*ptr_list_excluded_directories)[i] << "|" << endl;
                    if (strcmp(itr->path().string().c_str()+2,(*ptr_list_excluded_directories)[i]) == 0) { /// +2 pour sauter le ./
                        on_le_parse = false;
                        break;
                    }
                }
                if (on_le_parse)
                    addDirSource( itr->path().string().c_str(),listsource );      // addDirSource(dp->d_name);
            } else
                addDirSource( itr->path().string().c_str(),listsource );  
        } else {
            if (is_regular(itr->status())) {
                /// on vérifie que le fichier a le bon suffix
                stmp = suffix( itr->path().filename().string() );
                vector<string>::iterator it;
                if ((it = find(listHeaderSuffix.begin(),listHeaderSuffix.end(),stmp)) != listHeaderSuffix.end())
                    addFileSource(itr->path().string().c_str());
                if ((it = find(listBodySuffix.begin(),listBodySuffix.end(),stmp)) != listBodySuffix.end())
                    listsource.push_back( stmp );
                //op.analyse(itr->path().string());
            } else
                cerr << " le fichier " << itr->path().filename().string() << " n'est pas régulier " << endl;

        }
    }

}

// void Documentation::addDirSource( const char* name_dir,vector<string>& listsource, vector<const char*>* ptr_list_excluded_directories ) {
// 
//     string stmp;
//     string path;
//     string stmp2;
//     int err;
//     DIR* dir;
//     struct dirent* dp;
//     struct stat st;
// 
//     //cout << "------------- départ ----------------" << name_dir << endl;
//     dir = opendir(name_dir);
//     if (dir==NULL)
//         return;
//     //cout << "++++++++++++++++++++++++++++++++++++++ je parcours " << name_dir << endl;
//     path = name_dir ;
//     path += "/";
//     dp = readdir(dir);
//     while(dp != NULL) {
//         //cout << dp->d_name << endl; //<< " : " << dp->d_reclen << endl;
//         stmp = path + dp->d_name;
//         //cout << " new dp->d_name = " << stmp << endl; 
//         if ( (err = lstat(stmp.c_str(),&st)) != -1) {
//             if ((S_ISDIR(st.st_mode)) && (dp->d_name[0] != '.')) { 
//                 /// dp->d_name est le nom d'un répertoire qui ne commence pas un point
//                 //cout << " fichier = |" << dp->d_name << "| est un répertoire" << endl;
//                 /// on teste si ce répertoire n'est pas un répertoire à exclure.
//                 if (ptr_list_excluded_directories != NULL) {
//                     bool on_le_parse = true;
//                     for(int i=0;i<(*ptr_list_excluded_directories).size();i++) {
//                         //cout << " (*ptr_list_excluded_directories)[i] = |" << (*ptr_list_excluded_directories)[i] << "|" << endl;
//                         if (strcmp(dp->d_name,(*ptr_list_excluded_directories)[i]) == 0) {
//                             on_le_parse = false;
//                             break;
//                         }
//                     }
//                     if (on_le_parse)
//                         addDirSource( stmp.c_str(),listsource );      // addDirSource(dp->d_name);
//                 }else
//                     addDirSource( stmp.c_str(),listsource );      // addDirSource(dp->d_name);
//             } else
//             if (S_ISREG(st.st_mode)) {
//                 // c'est un fichier
//                 //stmp = dp->d_name;
//                 //cout << " fichier = |" << dp->d_name << "|";
//                 stmp2 = suffix( stmp );
//                 //cout << " suffix = |" << stmp2 << "|" << endl;
//                 if ((stmp2=="h") || (stmp2=="txt")) {
//                     addFileSource(stmp.c_str()); 
//                 //cout << "[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[ ajoutons " << stmp  << endl;//addFileSource(stmp.c_str());
//                 }
//                 if ((stmp2=="c") || (stmp2=="cpp") || (stmp2=="cc"))
//                     listsource.push_back( stmp );
//             } else
//             if (S_ISLNK(st.st_mode)) {
//                 //cout << " fichier = |" << stmp << "| est un lien" << endl;
//             } else
//             if (S_ISCHR(st.st_mode)) {
//                 //cout << " fichier = |" << stmp << "| est un fichier spécial caractère" << endl;
//             } else
//             if (S_ISBLK(st.st_mode)) {
//                 //cout << " fichier = |" << stmp << "| est un fichier spécial bloc" << endl;
//             } else
//             if (S_ISFIFO(st.st_mode)) {
//                 //cout << " fichier = |" << stmp << "| est un tube nommé" << endl;
//             } else
//             if (S_ISSOCK(st.st_mode)) {
//                 //cout << " fichier = |" << stmp << "| est un socket" << endl;
//             } else
//                ;//cout << " fichier = |" << stmp << "| est d'un format inconnu..." << endl;
// 
//         }
//         else {
//             cerr << "lstat échoue avec la réponse = " << err << "   errno = ";
//             switch(errno) {
//                 case EBADF : cout << "EBADF" << endl; break;
//                 case EFAULT : cout << "EFAULT" << endl; break;
//                 case ENAMETOOLONG : cout << "ENAMETOOLONG" << endl; break;
//                 case ENOENT : cout << "ENOENT" << endl; break;
//                 case ENOMEM : cout << "ENOMEM" << endl; break;
//                 case ENOTDIR : cout << "ENOTDIR" << endl; break;
//                 default : cout << "ERREUR INCONNUE" << endl; break;
//             }
//         }
//         dp = readdir(dir);
//     }
//     //cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++"  << endl;
//     closedir(dir);
// }

void Documentation::generate_ListTarget() {

    string stmp;
    int i,n,j,m;
    Target* pt;

    listTarget.clear();
    // on définit la liste des éléments pointables : cad les éléments qui héritent de la classe Target.
    m = listPageComment.size();
    for(j=0;j<m;j++) {
        n = listPageComment[j]->code.list_subType_function.size();
        for(i=0;i<n;i++)
            listTarget.push_back( listPageComment[j]->code.list_subType_function[i],true ); // vrai car il faut incrémenter l'ID du target listPageComment[j]->code.list_subType_function[i] et false car ce n'est pas un generic_comment
        n= listPageComment[j]->listComment.size();
        for(i=0;i<n;i++) {
            listPageComment[j]->listComment[i]->add_tutorial_example_genericComment_webpage_image( &listTarget );
        }
    }

    // on détermine le type référencé par les generic_comment déjà rencontré dans le code et dont les indices dans list sont stockés dans listPrincipalName


    // un certain nombre d'objets génériques n'ont pas de commentaires génériques (cad \generic_comment). On crée ici un target pour chacun d'eux dans la foncton suivante et on crée aussi une sous-liste les contenant :
    listTarget.generate_listPrincipalName( tree["generic_comment"] );

    // détermination des types commentés par les generic_comment
    listTarget.set_type_of_listPrincipalName();
}

void Documentation::display_ListTarget() {

    int i,n;

    cout << "============== liste de Targets ===========================" << endl;
    cout << listTarget << endl;
    cout << "============== fin de la liste de Targets =================" << endl;
}

void Documentation::display_ListPrincipalName() {

    int i,n;

    n = listTarget.listPrincipalName.size(); //  listPrincipalName.size();
    for(i=0;i<n;i++)
        cout << " nom principal = " << listTarget.list[listTarget.listPrincipalName[i]].pt->name.principalName << " avec un commentaire générique : " << listTarget.list[listTarget.listPrincipalName[i]].pt->ancestor << endl;
}

void Documentation::scan( Op& op ) {

    //for_each(listPageComment.begin(),listPageComment.end(), op );
    int i;
    for(i=0;i<listPageComment.size(); i++)
        op( listPageComment[i] );
}

void Documentation::parse() {

    //VisitorBloc_NameBaseClass visitorbloc_name_all_base_class( &listLink  );
    //VisitorBloc_NameBaseIsoledFunction visitorbloc_name_all_base_isoled_function( &listLink  );
    int i,m;

    m = listPageComment.size();
    for(i=0;i<m;i++) {
        listPageComment[i]->parse(language_id);
        cout << listPageComment[i]->nameFile <<" is parsing" << endl;
    }
}

void Documentation::generate_webpage_list( vector<Target*>& v, const string& name_of_path, const string& suffix,const string& nom, bool incrementID) {

    string stmp;
    string stmp2;
    int i,n,nb_col,m,j;
    bool genre_first_cell;
    Target* pt;

    pt = new Target();
    stmp = french2webID( nom );
    pt->name.principalName = stmp;
    pt->name.name = suffix + '_' + stmp;
    pt->path = tree[name_of_path];
    listTarget.push_back_RealTarget( pt,incrementID ); // utile pour pt->reference( ) : i.e. incrémente le nombre id
    stmp = pt->reference();
    //cout << " -------------->>>>>>>>>>>  CommentItemTutorial->reference() " << stmp << endl;
    std::ofstream pageWeb( stmp.c_str() ,ios::out);
    //VisitorCommentItem_toHTML vivi( &pageWeb, ptr_list_target );
    stmp = french2HTML( nom );

    //cout << "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIII" << pt->reference() << endl;
    //cout << "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIII" << tree["root"] << endl;

    generate_header( pt,tree["root"],pageWeb, stmp );


    pageWeb << "<p class=\"resume\">" << std::endl;

    /// on génère le tableau 
    nb_col = 3;
    n = v.size();
    if (n>0) {
        pageWeb << "<table cellspacing=\"2\">" << std::endl;
        for( i = 0; i < n; i+= nb_col) {
            if( ((i / nb_col ) % 2 ) == 0)
                genre_first_cell = true;
            else
                genre_first_cell = false;
            pageWeb << "<tr>" << std::endl;
            m = min(i+nb_col,n);
            for( j = i ; j < m; j++ ) {
                stmp = v[j]->name.principalName;
                stmp2 = linkHTML( pt->reference(), v[j]->reference(),stmp );
                if (genre_first_cell)
                    pageWeb << "    <td class=\"pair\">" << stmp2 << "</td>" << std::endl;
                else
                    pageWeb << "    <td>" << stmp2 << "</td>" << std::endl;
                genre_first_cell = !genre_first_cell;
            }
            pageWeb << "</tr>" << std::endl;
        }
        pageWeb << "</table>" << std::endl;
    }

    pageWeb << "</p>" << std::endl;
    feetPage(pageWeb);
}

typedef map<string,Target*>::const_iterator const_iter;

void display_multi_list( map<string,Target*>& ma, int level, const_iter first, const_iter end, std::ofstream& pageWeb, Target* pt ) {

    string stmp,stmp2,stmp_local_path;
    const_iter iter, iter2;

    /// liste vide
    if (first == end)
        return;
    pageWeb << "<ul>" << std::endl;
    for(iter = first;iter != end; ) {
        if (extract_name_of_path( stmp,iter->first,level)) {
            /// au niveau level, on a une feuille
            stmp_local_path = stmp;// on récupère le chemin courant
            pageWeb << "<li>" << linkHTML( pt->reference(), iter->second->reference(),stmp ) << "</li>" << std::endl;
            iter++;
        } else {
            /// au niveau level, on a un "sous-répertoire"
            if (stmp != stmp_local_path)
                pageWeb << "<li>" << french2HTML( stmp ) << "</li>" << std::endl;
            stmp_local_path.clear();
            for( iter2 = iter; iter2 != end; iter2++ ) {
                extract_name_of_path( stmp2,iter2->first,level);
                if (stmp2 != stmp)
                    break;
            }
            display_multi_list( ma,level+1,iter,iter2,pageWeb,pt );
            iter = iter2;
        }
    }
    pageWeb << "</ul>" << std::endl;
}

void Documentation::generate_webpage_multi_list( map<string,Target*>& ma, const string& name_of_path, const string& suffix,const string& nom, bool incrementID) {

    string cur_dir,stmp;
    string dir;
    deque<int> list_index;
    int i,j,ii,next,n;
    Target* pt;
    //map<string,Target*>::const_iterator iter;
    bool leaf;

    pt = new Target();
    stmp = french2webID( nom );
    pt->name.principalName = stmp;
    pt->name.name = suffix + '_' + stmp;
    pt->path = tree[name_of_path];
    listTarget.push_back_RealTarget( pt,incrementID ); // utile pour pt->reference( ) : i.e. incrémente le nombre id
    stmp = pt->reference();

    std::ofstream pageWeb( stmp.c_str() ,ios::out);
    stmp = french2HTML( nom );

    generate_header( pt,tree["root"],pageWeb, stmp );

    pageWeb << "<p class=\"resume\">" << std::endl;

    display_multi_list( ma,0,ma.begin(),ma.end(),pageWeb,pt);

    pageWeb << "</p>" << std::endl;
    feetPage(pageWeb);
}

Target* Documentation::generate_webpage_list_by_type( const ListTargetByType& li, const string& name_of_path, const string& suffix, const string& nom, bool incrementID) {

    string stmp;
    string stmp2;
    int i,n,m,j;
    bool ouvrant;
    Target* pt;

    pt = new Target();
    //stmp = replace_delimitateur( nom,'/','.');
    stmp = french2webID( nom );
    pt->name.principalName = stmp;
    //stmp = french2webID( replace_delimitateur( nom,'/','_') );
    pt->name.name = suffix + '_' + stmp;
    pt->path = tree[ name_of_path ];
    listTarget.push_back_RealTarget( pt, incrementID ); // utile pour pt->reference( ) : i.e. incrémente le nombre id
    stmp = pt->reference();
    //cout << " -------------->>>>>>>>>>>  CommentItemTutorial->reference() " << stmp << endl;
    std::ofstream pageWeb( stmp.c_str() ,ios::out);
    //VisitorCommentItem_toHTML vivi( &pageWeb, ptr_list_target );
    stmp = french2HTML( extract_last_name_of_path(nom) );

    //cout << "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIII" << pt->reference() << endl;
    //cout << "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIII" << tree["root"] << endl;
    //cout << "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIII" << tree[name_of_path] << endl;

    generate_header( pt,tree["root"],pageWeb, stmp );

    n = li.list_class_struct.size();
    if ( n > 0 ) {
        pageWeb << "<h2> Classes </h2>" << std::endl;
        pageWeb << "<ul>" << std::endl;
        for( i = 0; i < n; i++ ) {
            if ( li.list_class_struct[i]->of_class_or_struct() )
                stmp = li.list_class_struct[i]->name.name;
            else 
                stmp = li.list_class_struct[i]->name.principalName;
            stmp2 = linkHTML( pt->reference(), li.list_class_struct[i]->reference(), stmp );
            pageWeb << "<li>" << stmp2 << "</li>" << std::endl;
        }
        pageWeb << "</ul>" << std::endl;
    }

    n = li.list_function.size();
    if ( n > 0 ) {
        pageWeb << "<h2> Fonctions </h2>" << std::endl;
        pageWeb << "<ul>" << std::endl;
        for( i = 0; i < n; i++ ) {
            if (li.list_function[i]->of_function())
                stmp = li.list_function[i]->name.name;
            else 
                stmp = li.list_function[i]->name.principalName;
            stmp2 = linkHTML( pt->reference(), li.list_function[i]->reference(),stmp );
            pageWeb << "<li>" << stmp2 << "</li>" << std::endl;
        }
        pageWeb << "</ul>" << std::endl;
    }

    n = li.list_example.size();
    if ( n > 0 ) {
        pageWeb << "<h2> Exemples </h2>" << std::endl;
        pageWeb << "<ul>" << std::endl;
        for( i = 0; i < n; i++ ) {
            if ( li.list_example[i]->of_example() )
                stmp = li.list_example[i]->name.name;
            else
                stmp = li.list_example[i]->name.principalName;
            stmp2 = linkHTML( pt->reference(), li.list_example[i]->reference(), stmp );
            pageWeb << "<li>" << stmp2 << "</li>" << std::endl;
        }
        pageWeb << "</ul>" << std::endl;
    }

    n = li.list_tutorial.size();
    if ( n > 0 ) {
        pageWeb << "<h2> Tutoriels </h2>" << std::endl;
        pageWeb << "<ul>" << std::endl;
        for( i = 0; i < n; i++ ) {
            if (li.list_tutorial[i]->of_tutorial())
                stmp = li.list_tutorial[i]->name.name;
            else
                stmp = li.list_tutorial[i]->name.principalName;
            stmp2 = linkHTML( pt->reference(), li.list_tutorial[i]->reference(),stmp );
            pageWeb << "<li>" << stmp2 << "</li>" << std::endl;
        }
        pageWeb << "</ul>" << std::endl;
    }

    n = li.list_webpage.size();
    if ( n > 0 ) {
        pageWeb << "<h2> G&eacute;n&eacute;ral </h2>" << std::endl;
        pageWeb << "<ul>" << std::endl;
        for( i = 0; i < n; i++ ) {
            stmp = li.list_webpage[i]->name.principalName;
            stmp2 = linkHTML( pt->reference(), li.list_webpage[i]->reference(),stmp );
            pageWeb << "<li>" << stmp2 << "</li>" << std::endl;
        }
        pageWeb << "</ul>" << std::endl;
    }

    feetPage( pageWeb );

    return pt;
}

void Documentation::generate_webpage_vacuum( const string& name_of_path, const string& suffix,const string& nom, const string& titre,bool incrementID) {

    string stmp;
    string stmp2;
    int i,n,m,j;
    Target* pt;

    pt = new Target();
    stmp = french2webID( nom );
    //pt->name.principalName = stmp;
    pt->name.principalName = titre;
    pt->name.name = suffix + '_' + stmp;
    //pt->name.name = suffix + '_' + nom;
    pt->path = tree[name_of_path];
    listTarget.push_back_RealTarget( pt,incrementID ); // utile pour pt->reference( ) : i.e. incrémente le nombre id
    stmp = pt->reference();

    std::ofstream pageWeb( stmp.c_str() ,ios::out);

    stmp = french2HTML( titre );

    generate_header( pt,tree["root"],pageWeb, stmp );

    pageWeb << "<p>" << std::endl;
    pageWeb << "Compl&eacute;ter le fichier "<< nom << " dans le r&eacute;pertoire des sources du programme." << std::endl;
    pageWeb << "</p>" << std::endl;

    feetPage(pageWeb);
}

    /**
    génère la page web des objets génériques (i.e. lorsque plusieurs Target ont le même nom principal).
    Il y a deux cas à traités; Celui où l'objet générique a un commentaire générique et l'autre où il n' en a pas.
    s'il y a un commentaire générique, on parcourt la structure pour faire la page, sinon on fait directement la page !
    */
void Documentation::generate_webpage_of_summary() {

    string stmp;
    OpGet_listKeyword op_get_list_keyword;
    vector<Target*> list_example;
    vector<Target*> list_tutorial;
    vector<Target*> list_function;
    vector<Target*> list_struct_class;
    int i,n,end,pos;
    map<string,ListTargetByType>::const_iterator iter;
    map<string,Target*> map_keyword_Target;
    Target* pt;

     /**
     On génére les pages web de résumé de toutes les fonctions, toutes les structures, tous les tutoriaux, tous les exemples.
     On commence par récupérer les brief des fonctions, structures, classes, exemples, et exemples qui ne sont pas génériques.
     Puis avec ces données, on créera les pages web recapitulatives des structures/classes, des fonctions, des exemples et des tutoriaux.
    */
    // tout compte fait, on ne récupère pas les briefs !!!!!!!!!!!!!!!!!!!!

    n = listTarget.listPrincipalName.size();
    for(i=0;i<n;i++) {
        pt = listTarget.list[ listTarget.listPrincipalName[i] ].pt;
        if ( pt->of_generic_comment() || pt->of_type_without_generic_comment() ) {
            if ( pt->ancestor != NULL ) {
                if ( pt->ancestor->of_class_or_struct() )
                    list_struct_class.push_back(pt);
                if ( pt->ancestor->of_tutorial() )
                    list_tutorial.push_back(pt);
                if ( pt->ancestor->of_example() )
                    list_example.push_back(pt);
                if ( pt->ancestor->of_function() )
                    list_function.push_back(pt);
            } else {
                cerr << "**** ATTENION : ancestor == NULL !" << endl;
                cerr << *pt << endl;
                cerr << "****" << endl;
            }
        } else {
            if (pt->of_class_or_struct())
                list_struct_class.push_back( pt );
            if (pt->of_tutorial() )
                list_tutorial.push_back( pt );
            if (pt->of_example())
                list_example.push_back( pt );
            if (pt->of_function())
                list_function.push_back( pt );
        }
    }

    /// ensuite on trie les 4 listes.
    stable_sort( list_example.begin(),list_example.end(), inferior );
    stable_sort( list_tutorial.begin(),list_tutorial.end(), inferior );
    stable_sort( list_function.begin(),list_function.end(), inferior );
    stable_sort( list_struct_class.begin(),list_struct_class.end(), inferior );
    /// on crée les pages web.
    generate_webpage_list( list_example,"root","list", "Tous les exemples", false ); /// false signifie que l'id d target n'est pas incrémenté
    generate_webpage_list( list_tutorial,"root","list", "Tous les tutoriels", false ); /// false signifie que l'id d target n'est pas incrémenté
    generate_webpage_list( list_function,"root","list", "Toutes les  fonctions", false ); /// false signifie que l'id d target n'est pas incrémenté
    generate_webpage_list( list_struct_class,"root","list", "Toutes les classes", false ); /// false signifie que l'id d target n'est pas incrémenté

    /**
     On crée ensuite la page web affichant la liste des mots clés .
     on récupère tous les mots clés avec les pages web associées dans op_get_list_keyword.
    
    */
    scan( op_get_list_keyword );
    for( iter = op_get_list_keyword.list_keyword.begin(); iter != op_get_list_keyword.list_keyword.end(); ++iter ) {
        /// cette boucle sert à créer la page web de chaque feuille de l'arboresence correspondant aux mot-clés.
        stmp = extract_path( iter->first );
        stmp = "keyword_" + replace_delimitateur( stmp, '/', '_' );
        map_keyword_Target[ iter->first ] = generate_webpage_list_by_type( iter->second, "keyword", stmp, extract_last_name_of_path( iter->first ), false );
        //cerr << " iter->first = " << iter->first << endl;
        //cerr << " extract_last_name_of_path(iter->first) = " << extract_last_name_of_path(iter->first) << endl;
        //cerr << " replace_delimitateur(iter->first,'/','_') = " << replace_delimitateur(iter->first,'/','_') << endl;
    }
    /// cette fonction génère les pages web correspondant aux branches de l'arbre
    generate_webpage_multi_list( map_keyword_Target, "root", "list", "Tous les mot-clés", false ); // false signifie que l'id d target n'est pas incrémenté

}

void Documentation::generate_webpage_of_main_object() {

    int i,n;
    OpInternal opinternal;
    vector<Target*> list_function;
    vector<Target*> list_struct_class;
    Target* pt;


    listTarget.init_property();
    scan( opinternal );
    listTarget.update_property_of_generic_comment();

    n = listTarget.listPrincipalName.size();
    for(i=0;i<n;i++) {
        pt = listTarget.list[listTarget.listPrincipalName[i]].pt;
        if (pt->of_generic_comment() || pt->of_type_without_generic_comment() ) {
            if (pt->ancestor != NULL) {
                if (pt->ancestor->of_class_or_struct() && (pt->property == 0))
                    list_struct_class.push_back(pt);
                if (pt->ancestor->of_function() && (pt->property == 0))
                    list_function.push_back(pt);
            } else {
                cerr << "**** ATTENION : ancestor == NULL !" << endl;
                cerr << *pt << endl;
                cerr << "****" << endl;
            }
        } else {
            if (pt->of_class_or_struct() && (pt->property == 0))
                list_struct_class.push_back(pt);
            if (pt->of_function() && (pt->property == 0))
                list_function.push_back(pt);
        }
    }

    // ensuite on trie les 2 listes.
    stable_sort( list_function.begin(),list_function.end(), inferior );
    stable_sort( list_struct_class.begin(),list_struct_class.end(), inferior );
    // on crée les pages web.
    generate_webpage_list( list_function,"root","list", "Les fonctions principales", false ); // false signifie que l'id d target n'est pas incrémenté
    generate_webpage_list( list_struct_class,"root","list", "Les classes principales", false ); // false signifie que l'id d target n'est pas incrémenté
}

void Documentation::generate_file_for_webpage( const string& titre, const string& nom_web ) {

    string stmp;

    stmp = french2webID( titre ) + ".txt";
    std::ofstream page( stmp.c_str() ,ios::out);
    page << "/*!" << std::endl;
    page << "\\webpage " << nom_web <<  " " << titre << std::endl;
    page << std::endl;
    page << "    à compléter (ne pas effacer la ligne ci dessus et indenter le texte)." << std::endl;
    page << std::endl;
    page << std::endl;
    page << std::endl;
    page << std::endl;
    page << std::endl;
    page << std::endl;
    page << std::endl;
    page << std::endl;
    page << std::endl;
    page << std::endl;
    page << "*/" << std::endl;
}

// génère la page d'accueil du site (avec des a priori...)
void Documentation::generate_index() {

    OpWebPage_exists op_webpage_exists;
    vector<string> list_webpage;
    vector<string> list_titre_of_webpage;
    string stmp;
    string stmp2;
    int i,n;
    Target* pt;

    pt = new Target();
    pt->name.principalName = "index_software";
    pt->name.name = "index";
    pt->path = tree["root"];
    listTarget.push_back_RealTarget( pt,false ); // false signifie que l'id target n'est pas incrémenté donc le nom du fichier html ne sera pas indexé
    stmp = pt->reference();
    //cout << " -------------->>>>>>>>>>>  CommentItemTutorial->reference() " << stmp << endl;
    std::ofstream pageWeb( stmp.c_str() ,ios::out);

    //pageWeb<<"<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>"<<std::endl;
    pageWeb<<"<!DOCTYPE html"<<std::endl;
    pageWeb<<"   PUBLIC \"-//W3C//DTDXHTML 1.0 Strict//EN\" \"DTD/xhtml1-strict.dtd\">"<<std::endl;
    pageWeb<<"<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"fr\" lang=\"fr\">"<<std::endl;
    pageWeb<<"<head>"<<std::endl;
    pageWeb<<"<title> Documentation de " << name_software <<  " </title>"<<std::endl;
    pageWeb<<"<link href=\"classic.css\" rel=\"stylesheet\" type=\"text/css\" />"<<std::endl;
    pageWeb<<"</head>"<<std::endl;
    pageWeb<<"<body text=\"#000000\" bgcolor=\"#ffffff\" link=\"#0000cc\" vlink=\"#551a8b\"  >"<<std::endl;
    

    pageWeb<< "<h1 align=\"center\"> Documentation de " << name_software << "<br/><small></small></h1>" << std::endl;

    pageWeb<< "<table cellspacing=\"2\">" << std::endl;
    pageWeb<< " <tr>"  << std::endl;
    pageWeb<< "<td class=\"pseudo_en-tete_index\"> Pour commencer </td>"  << std::endl;
    pageWeb<< "<td class=\"pseudo_en-tete_index\"> Description </td>"  << std::endl;
    pageWeb<< "<td class=\"pseudo_en-tete_index\"> Outils </td>"  << std::endl;
    pageWeb<< " </tr>"  << std::endl;

    pageWeb<< " <tr>"  << std::endl;
    stmp = "introduction";
    list_webpage.push_back(stmp);
    list_titre_of_webpage.push_back("Introduction");
    pageWeb<< "<td><a href=\"" << "webpage_"+stmp <<".html\">Quest-ce que c'est ?</a></td>" << std::endl;
    pageWeb<< "<td><a href=\"list_touslesmot-cles.html\">Tous les MOT-CL&Eacute;S</a></td>" << std::endl;
    stmp = "git";
    list_webpage.push_back(stmp);
    stmp2 = "Git";
    list_titre_of_webpage.push_back(stmp2);
    pageWeb<< "<td><a href=\"" << "webpage_"+stmp <<".html\"> " << stmp2 << " </a></td>" << std::endl; // CODAGE en DUR   -> pas bien
    pageWeb<< " </tr>"  << std::endl;

    pageWeb<< " <tr>"  << std::endl;
    stmp = "installation";
    stmp2 = "Installation";
    list_webpage.push_back(stmp);
    list_titre_of_webpage.push_back(stmp2);
    pageWeb<< "<td><a href=\"" << "webpage_"+stmp <<".html\">" << stmp2 << " </a></td>" << std::endl;
    pageWeb<< "<td><a href=\"list_lesclassesprincipales.html\">Les classes principales</a></td>" << std::endl; // CODAGE en DUR   -> pas bien
    stmp = "scons";
    list_webpage.push_back(stmp);
    stmp2 = "Scons";
    list_titre_of_webpage.push_back(stmp2);
    pageWeb<< "<td><a href=\"" << "webpage_"+stmp <<".html\"> " << stmp2 << " </a></td>" << std::endl; // CODAGE en DUR   -> pas bien
    pageWeb<< " </tr>"  << std::endl;

    pageWeb<< " <tr>"  << std::endl;
    stmp = "quickstart";
    list_webpage.push_back(stmp);
    list_titre_of_webpage.push_back("Bien commencer avec la LMT++");
    pageWeb<< "<td><a href=\"" << "webpage_"+stmp <<".html\">D&eacute;part rapide</a> (quick start) </td>" << std::endl;
    pageWeb<< "<td><a href=\"list_lesfonctionsprincipales.html\">Les fonctions principales</a></td>" << std::endl;
    stmp = "python";
    list_webpage.push_back(stmp);
    stmp2 = "Python";
    list_titre_of_webpage.push_back(stmp2);
    pageWeb<< "<td><a href=\"" << "webpage_"+stmp <<".html\"> " << stmp2 << " </a></td>" << std::endl; // CODAGE en DUR   -> pas bien
    pageWeb<< " </tr>"  << std::endl;

    pageWeb<< " <tr>"  << std::endl;
    pageWeb<< "<td><a href=\"list_touslestutoriels.html\">Tutoriels</a></td>" << std::endl; // ********* CODAGE en DUR -> pas bien
    pageWeb<< "<td><a href=\"list_touteslesclasses.html\">Toutes les classes</a></td>" << std::endl; // CODAGE en DUR   -> pas bien
    stmp = "metil";
    list_webpage.push_back(stmp);
    stmp2 = "METIL";
    list_titre_of_webpage.push_back(stmp2);
    pageWeb<< "<td><a href=\"" << "webpage_"+stmp <<".html\"> " << stmp2 << " </a></td>" << std::endl; // CODAGE en DUR   -> pas bien
    pageWeb<< " </tr>"  << std::endl;

    pageWeb<< " <tr>"  << std::endl;
    pageWeb<< "<td><a href=\"list_touslesexemples.html\">Exemples</a></td>" << std::endl;
    pageWeb<< "<td><a href=\"list_touteslesfonctions.html\">Toutes les fonctions</a></td>" << std::endl;
    stmp = "kdevelop";
    list_webpage.push_back(stmp);
    stmp2 = "kdevelop";
    list_titre_of_webpage.push_back(stmp2);
    pageWeb<< "<td><a href=\"" << "webpage_"+stmp <<".html\"> " << stmp2 << " </a></td>" << std::endl; // CODAGE en DUR   -> pas bien
    pageWeb<< " </tr>"  << std::endl;

    pageWeb<< " <tr>"  << std::endl;
    stmp = "faq";
    list_webpage.push_back(stmp);
    stmp2 = "FAQ";
    list_titre_of_webpage.push_back(stmp2);
    pageWeb<< "<td><a href=\"" << "webpage_"+stmp <<".html\"> " << stmp2 << " </a></td>" << std::endl; // CODAGE en DUR   -> pas bien
    pageWeb<< "<td></td>" << std::endl;
    pageWeb<< "<td></td>" << std::endl;
    pageWeb<< " </tr>"  << std::endl;

    pageWeb<< " <tr>"  << std::endl;
    pageWeb<< "<td class=\"pseudo_en-tete_index\"> Liens externes </td>"  << std::endl;
    pageWeb<< "<td class=\"pseudo_en-tete_index\"> Communaut&eacute; </td>" << std::endl;
    pageWeb<< "<td class=\"pseudo_en-tete_index\"> G&eacute;n&eacute;ralit&eacute;s </td>" << std::endl;
    pageWeb<< " </tr>"  << std::endl;

    pageWeb<< " <tr>"  << std::endl;
    pageWeb<< "<td></td>" << std::endl;
    stmp = "contribution";
    list_webpage.push_back(stmp);
    stmp2 = "Comment contribuer";
    list_titre_of_webpage.push_back(stmp2);
    pageWeb<< "<td><a href=\"" << "webpage_"+stmp <<".html\"> " << stmp2 << " </a></td>" << std::endl; // CODAGE en DUR   -> pas bien
    stmp = "philosophie";
    list_webpage.push_back(stmp);
    stmp2 = "Philosophie de la librairie";
    list_titre_of_webpage.push_back(stmp2);
    pageWeb<< "<td><a href=\"" << "webpage_"+stmp <<".html\"> " << stmp2 << " </a></td>" << std::endl; // CODAGE en DUR   -> pas bien
    pageWeb<< " </tr>"  << std::endl;

    pageWeb<< " <tr>"  << std::endl;
    pageWeb<< "<td></td>" << std::endl;
    stmp = "rappportdebug";
    list_webpage.push_back(stmp);
    stmp2 = "Rapporter un bug";
    list_titre_of_webpage.push_back(stmp2);
    pageWeb<< "<td><a href=\"" << "webpage_"+stmp <<".html\"> " << stmp2 << " </a></td>" << std::endl; // CODAGE en DUR   -> pas bien
    stmp = "licence";
    list_webpage.push_back(stmp);
    stmp2 = "Licence de la librairie";
    list_titre_of_webpage.push_back(stmp2);
    pageWeb<< "<td><a href=\"" << "webpage_"+stmp <<".html\"> " << stmp2 << " </a></td>" << std::endl; // CODAGE en DUR   -> pas bien
    pageWeb<< " </tr>"  << std::endl;
    
    pageWeb<< " <tr>"  << std::endl;
    pageWeb<< "<td></td>" << std::endl;
    pageWeb<< "<td></td>" << std::endl;
    stmp = "report.html";
    pageWeb<< "<td><a href=\"" << stmp << "\" > Rapport des tests unitaires </a></td>" << std::endl; // CODAGE en DUR   -> pas bien
    pageWeb<< " </tr>"  << std::endl;    
    
    pageWeb<< " </table>"  << std::endl;
    
    pageWeb<< "<hr/>" << std::endl;
    pageWeb<< "<form action=\"search.php\" method=\"get\">" << std::endl;
    pageWeb<< "        <table cellspacing=\"0\" cellpadding=\"0\" border=\"0\">" << std::endl;
    pageWeb<< "          <tr>" << std::endl;
    pageWeb<< "            <td><label>&nbsp;<u>S</u>earch&nbsp;for&nbsp;</label></td>" << std::endl;
    pageWeb<< "            <td><input type=\"text\" name=\"query\" value=\"\" size=\"20\" accesskey=\"s\"/></td>" << std::endl;
    pageWeb<< "          </tr>" << std::endl;
    pageWeb<< "        </table>" << std::endl;
    pageWeb<< "      </form>" << std::endl;
    feetPage(pageWeb,true);

    /*
         cet endroit, il faut parcourir le répertoire du programme pour trouver les fichiers txt correspondants aux pages web intro, installation, 
    */
    n = list_webpage.size();
    for(i=0;i<n;i++) {
        op_webpage_exists.init("webpage_"+list_webpage[i]);
        scan( op_webpage_exists );
        if (!op_webpage_exists.exists) {
            generate_webpage_vacuum("root","webpage",list_webpage[i],list_titre_of_webpage[i],false ); // cela crée une page web vide : c'est mieux que rien
            generate_file_for_webpage( list_webpage[i],list_titre_of_webpage[i]);
        }
    }
}

void Documentation::setHeaderSuffix(const char* l[], int s){
    for(int i=0;i<s;i++)
        listHeaderSuffix.push_back(l[i]);
}

void Documentation::setBodySuffix(const char* l[], int s){
    for(int i=0;i<s;i++)
        listBodySuffix.push_back(l[i]);
}

void Documentation::generate_file_for_search_engine() { 

    string stmp;
    bool hiPriority;

    for(int i=0;i<listTarget.list.size();i++) {
        searchIndex.setCurrentDoc(listTarget.list[i].pt);
        if (listTarget.list[i].pt->name.name == listTarget.list[i].pt->name.principalName)
            hiPriority = true;
        else
            hiPriority = false;
        cerr << " ajoute " << listTarget.list[i].pt->name.name << endl;
        searchIndex.addWord(listTarget.list[i].pt->name.name,hiPriority);
    }

    stmp = tree["root"] + "search.idx";
    searchIndex.write(stmp.c_str());
//     ifstream entree("search_php_provisoire");
//     if (!entree.is_open()) {
//         cerr << " problème pour ouvrir search_php_provisoire" << endl;
//         return;
//     }

//     stmp = tree["root"] + "search_choupette_string";
//     ofstream sortie(stmp.c_str());
//     if (!sortie.is_open()) {
//         cerr << " problème pour ouvrir " << stmp << endl;
//         return;
//     }
//     sortie << "const char Documentation::text_search_php[] = \"";
//     while (true) {
//         stmp.clear();
//         getline(entree,stmp);
//         if (!entree.good())
//             break;
//         sortie << stmp << "\\n";
//     }
//     sortie << "\"" << endl;
//     sortie.close();



    stmp = tree["root"] + "search.php";
    std::ofstream sortie(stmp.c_str());
    if (!sortie.is_open()) {
        cerr << " problème pour ouvrir " << stmp << endl;
        return;
    }
    sortie << text_search_php << endl;
    sortie.close();
//     entree.close();
}

void Documentation::generate_file_css() {

    string stmp;

    stmp = tree["root"] + "classic.css";
    std::ofstream pageWeb( stmp.c_str() ,ios::out );

    pageWeb << std::endl << std::endl;
    pageWeb << "ul.navigation{"  << std::endl;
    pageWeb << "\tlist-style-image: url(Kurodai-icon.gif);"  << std::endl;
    pageWeb << "\tlist-style-type: none;"  << std::endl;
    pageWeb << "\tmargin: 4px;"  << std::endl;
    pageWeb << "\tpadding: 4px;"  << std::endl;
    pageWeb << "}"  << std::endl;
    pageWeb << std::endl;
    pageWeb << "ul.navigation li {" << std::endl;
    pageWeb << "\tdisplay: inline;" << std::endl;
    pageWeb << "}"  << std::endl;
    pageWeb << std::endl;
    pageWeb <<  "ul.navigation li a {" << std::endl;
    pageWeb <<  "\tpadding: 5px 20px;" << std::endl;
    pageWeb << "\tmargin: 0px 2px;" << std::endl;
    pageWeb << "\tborder: 1px solid #666;" << std::endl;
    pageWeb << "\tbackground-color: #9DC5D4;" << std::endl;
    pageWeb << "\ttext-decoration: none;" << std::endl;
    pageWeb << "\ttext-align: center;" << std::endl;
    pageWeb << "}" << std::endl;
    pageWeb << std::endl;
    pageWeb << "ul.en_tete_tableau_3colonnes {" << std::endl;
    pageWeb << "\tlist-style-image: url(Kurodai-icon.gif);" << std::endl;
    pageWeb << "\tpadding: 5px 5px;" << std::endl;
    pageWeb << "}" << std::endl;
    pageWeb << "td.pseudo_en-tete_index {\n\tbackground-color: #6fadea;\n\twidth: 400px;\n\tmargin: 1px;\n\ttext-align: center;\n\t}\n" << std::endl;
//     pageWeb << "ul.en_tete_tableau_3colonnes li {" << std::endl;
//     pageWeb << "\tdisplay: inline;" << std::endl;
//     pageWeb << "\tfloat: left;" << std::endl;
//     pageWeb << "\twidth: 33%;" << std::endl;
//     pageWeb << "\tmargin: 1px;" << std::endl;
//     pageWeb << "\tbackground-color: #6fadea;" << std::endl;
//     pageWeb << "\ttext-align: center;" << std::endl;
//     pageWeb << "}" << std::endl;
    pageWeb << std::endl;
    pageWeb << "p.bloc {" << std::endl;
    pageWeb << "}" << std::endl;
    pageWeb << "p.texte_documentation {\n    font-family:\"Times New Roman\", Times, serif;\n}\n" << std::endl;    
    pageWeb << std::endl;
    pageWeb << "div.liste {\n    font-family:\"Times New Roman\", Times, serif;\n}\n" << std::endl;
    pageWeb << std::endl;
//     pageWeb << "ul.ligne_tableau_3colonnes {\n    list-style-image: url(Kurodai-icon.gif);\n    list-style-type: disc;\n    padding: 5px 5px;\n    width: 100%;\n   height: 100px;\n}\n" << std::endl;
    pageWeb << std::endl;
    pageWeb << "ul.ligne_tableau_3colonnes li {\n	display: inline;\n	float: left;\n	width: 33%;\n	margin: 1px;\n	text-align: left;\n}\n" << std::endl;
    pageWeb << std::endl;
    pageWeb << "ul.cellule {\n	list-style-type: disc;\n}\n" << std::endl;
    pageWeb << std::endl;
    pageWeb << "ul.cellule li {\n	width: 100%;\n}\n" << std::endl;
    pageWeb << std::endl;
    //pageWeb << "div.code {\n    background-color: #84A8B1;#fde1e7;\n    padding: 0px 85px;\n}\n" << std::endl;
    pageWeb << "div.code {\n	background-color: #84A8B1;#fde1e7;\n	padding: 0px 85px;\n	width: 400%;\n}\n" << std::endl;
    pageWeb << std::endl;
    pageWeb << "div.verbatim {\n	background-color: #c7dad7;\n	padding: 0px 85px;\n	width: 83%;\n}\n" << std::endl;
    pageWeb << std::endl;
    pageWeb << "td.pair {\n    background-color: #BDD2C9;#eaeacf;#9ACBD4;\n}\n" << std::endl;
    pageWeb << std::endl;
    pageWeb << "a:link {\n    text-decoration: none;\n}\n" << std::endl;
    pageWeb << "a:visited {\n    text-decoration: none;\n}\n" << std::endl;
    pageWeb << std::endl;
    pageWeb << "a:hover {\n    text-decoration: underline;\n}\n" << std::endl;
//     pageWeb << "ul.parameter_template li {" << std::endl;
//     pageWeb << "\tdisplay: inline;" << std::endl;
//     pageWeb << "}"  << std::endl;
//     pageWeb << std::endl;

    pageWeb << std::endl;
    pageWeb << std::endl;
    pageWeb << std::endl;
    pageWeb << std::endl;
    pageWeb << std::endl;
    pageWeb << std::endl;
    pageWeb << std::endl;
    pageWeb << std::endl;
    pageWeb << std::endl;
    pageWeb << std::endl;
    pageWeb << std::endl;
    pageWeb << std::endl;
}

const char Documentation::text_search_php[] = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n<html >\n<head>\n <META HTTP-EQUIV=\"CONTENT-TYPE\" CONTENT=\"text/html; charset=utf-8\">\n<link href=\"classic.css\" rel=\"stylesheet\" type=\"text/css\" />\n<title> Documentation (Officielle) de la librairie LMT++ </title>\n</head>\n<body text=\"#000000\" bgcolor=\"#fcfcdf\" link=\"#0000cc\" vlink=\"#551a8b\" >\n<ul class=\"navigation\" >\n<li> <a href=\"index.html\" >Accueil</a></li>\n<li> <a href=\"list_touslesmot-cles.html\" >Tous les MOT-CL&Eacute;S</a> </li>\n<li> <a href=\"list_touslesexemples.html\" >Tous les exemples</a> </li>\n<li> <a href=\"list_touslestutoriels.html\" >Tous les tutoriels</a> </li>\n<li> <a href=\"list_lesclassesprincipales.html\" >Les classes principales</a> </li>\n<li> <a href=\"list_lesfonctionsprincipales.html\" >Les fonctions principales</a> </li>\n</ul>\n\n<?php\n\nfunction search_results()\n{\n  return \"Search Results\";\n}\n\nfunction matches_text($num)\n{\n  if ($num==0)\n  {\n    return \"Sorry, no documents matching your query.\";\n  }\n  else if ($num==1)\n  {\n    return \"Found <b>1</b> document matching your query.\";\n  }\n  else // $num>1\n  {\n    return \"Found <b>$num</b> documents matching your query. Showing best matches first.\";\n  }\n}\n\nfunction report_matches()\n{\n  return \"Matches: \";\n}\nfunction end_form($value)\n{\n  echo \"            <td><input type=\\\"text\\\" name=\\\"query\\\" value=\\\"$value\\\" size=\\\"20\\\" accesskey=\\\"s\\\"/></td>\\n          </tr>\\n        </table>\\n      </form>\\n    </li>\\n  </ul>\\n</div>\\n\";\n}\n\nfunction readInt($file)\n{\n  $b1 = ord(fgetc($file)); $b2 = ord(fgetc($file));\n  $b3 = ord(fgetc($file)); $b4 = ord(fgetc($file));\n  return ($b1<<24)|($b2<<16)|($b3<<8)|$b4;\n}\n\nfunction readString($file)\n{\n  $result=\"\";\n  while (ord($c=fgetc($file))) $result.=$c;\n  return $result;\n}\n\nfunction readHeader($file)\n{\n  $header =fgetc($file); $header.=fgetc($file);\n  $header.=fgetc($file); $header.=fgetc($file);\n  return $header;\n}\n\nfunction computeIndex($word)\n{\n  // Fast string hashing\n  //$lword = strtolower($word);\n  //$l = strlen($lword);\n  //for ($i=0;$i<$l;$i++)\n  //{\n  //  $c = ord($lword{$i});\n  //  $v = (($v & 0xfc00) ^ ($v << 6) ^ $c) & 0xffff;\n  //}\n  //return $v;\n\n  // Simple hashing that allows for substring search\n  if (strlen($word)<2) return -1;\n  // high char of the index\n  $hi = ord($word{0});\n  if ($hi==0) return -1;\n  // low char of the index\n  $lo = ord($word{1});\n  if ($lo==0) return -1;\n  // return index\n  return $hi*256+$lo;\n}\n\nfunction search($file,$word,&$statsList)\n{\n  $index = computeIndex($word);\n  if ($index!=-1) // found a valid index\n  {\n    fseek($file,$index*4+4); // 4 bytes per entry, skip header\n    $index = readInt($file);\n    if ($index) // found words matching the hash key\n    {\n      $start=sizeof($statsList);\n      $count=$start;\n      fseek($file,$index);\n      $w = readString($file);\n      while ($w)\n      {\n        $statIdx = readInt($file);\n        if ($word==substr($w,0,strlen($word)))\n        { // found word that matches (as substring)\n          $statsList[$count++]=array(\n              \"word\"=>$word,\n              \"match\"=>$w,\n              \"index\"=>$statIdx,\n              \"full\"=>strlen($w)==strlen($word),\n              \"docs\"=>array()\n              );\n        }\n        $w = readString($file);\n      }\n      $totalHi=0;\n      $totalFreqHi=0;\n      $totalFreqLo=0;\n      for ($count=$start;$count<sizeof($statsList);$count++)\n      {\n        $statInfo = &$statsList[$count];\n        $multiplier = 1;\n        // whole word matches have a double weight\n        if ($statInfo[\"full\"]) $multiplier=2;\n        fseek($file,$statInfo[\"index\"]); \n        $numDocs = readInt($file);\n        $docInfo = array();\n        // read docs info + occurrence frequency of the word\n        for ($i=0;$i<$numDocs;$i++)\n        {\n          $idx=readInt($file); \n          $freq=readInt($file); \n          $docInfo[$i]=array(\"idx\"  => $idx,\n                             \"freq\" => $freq>>1,\n                             \"rank\" => 0.0,\n                             \"hi\"   => $freq&1\n                            );\n          if ($freq&1) // word occurs in high priority doc\n          {\n            $totalHi++;\n            $totalFreqHi+=$freq*$multiplier;\n          }\n          else // word occurs in low priority doc\n          {\n            $totalFreqLo+=$freq*$multiplier;\n          }\n        }\n        // read name and url info for the doc\n        for ($i=0;$i<$numDocs;$i++)\n        {\n          fseek($file,$docInfo[$i][\"idx\"]);\n          $docInfo[$i][\"name\"]=readString($file);\n          $docInfo[$i][\"url\"]=readString($file);\n        }\n        $statInfo[\"docs\"]=$docInfo;\n      }\n      $totalFreq=($totalHi+1)*$totalFreqLo + $totalFreqHi;\n      for ($count=$start;$count<sizeof($statsList);$count++)\n      {\n        $statInfo = &$statsList[$count];\n        $multiplier = 1;\n        // whole word matches have a double weight\n        if ($statInfo[\"full\"]) $multiplier=2;\n        for ($i=0;$i<sizeof($statInfo[\"docs\"]);$i++)\n        {\n          $docInfo = &$statInfo[\"docs\"];\n          // compute frequency rank of the word in each doc\n          $freq=$docInfo[$i][\"freq\"];\n          if ($docInfo[$i][\"hi\"])\n          {\n            $statInfo[\"docs\"][$i][\"rank\"]=\n              (float)($freq*$multiplier+$totalFreqLo)/$totalFreq;\n          }\n          else\n          {\n            $statInfo[\"docs\"][$i][\"rank\"]=\n              (float)($freq*$multiplier)/$totalFreq;\n          }\n        }\n      }\n    }\n  }\n  return $statsList;\n}\n\nfunction combine_results($results,&$docs)\n{\n  foreach ($results as $wordInfo)\n  {\n    $docsList = &$wordInfo[\"docs\"];\n    foreach ($docsList as $di)\n    {\n      $key=$di[\"url\"];\n      $rank=$di[\"rank\"];\n      if (in_array($key, array_keys($docs)))\n      {\n        $docs[$key][\"rank\"]+=$rank;\n      }\n      else\n      {\n        $docs[$key] = array(\"url\"=>$key,\n            \"name\"=>$di[\"name\"],\n            \"rank\"=>$rank\n            );\n      }\n      $docs[$key][\"words\"][] = array(\n               \"word\"=>$wordInfo[\"word\"],\n               \"match\"=>$wordInfo[\"match\"],\n               \"freq\"=>$di[\"freq\"]\n               );\n    }\n  }\n  return $docs;\n}\n\nfunction filter_results($docs,&$requiredWords,&$forbiddenWords)\n{\n  $filteredDocs=array();\n  while (list ($key, $val) = each ($docs)) \n  {\n    $words = &$docs[$key][\"words\"];\n    $copy=1; // copy entry by default\n    if (sizeof($requiredWords)>0)\n    {\n      foreach ($requiredWords as $reqWord)\n      {\n        $found=0;\n        foreach ($words as $wordInfo)\n        { \n          $found = $wordInfo[\"word\"]==$reqWord;\n          if ($found) break;\n        }\n        if (!$found) \n        {\n          $copy=0; // document contains none of the required words\n          break;\n        }\n      }\n    }\n    if (sizeof($forbiddenWords)>0)\n    {\n      foreach ($words as $wordInfo)\n      {\n        if (in_array($wordInfo[\"word\"],$forbiddenWords))\n        {\n          $copy=0; // document contains a forbidden word\n          break;\n        }\n      }\n    }\n    if ($copy) $filteredDocs[$key]=$docs[$key];\n  }\n  return $filteredDocs;\n}\n\nfunction compare_rank($a,$b)\n{\n  if ($a[\"rank\"] == $b[\"rank\"]) \n  {\n    return 0;\n  }\n  return ($a[\"rank\"]>$b[\"rank\"]) ? -1 : 1; \n}\n\nfunction sort_results($docs,&$sorted)\n{\n  $sorted = $docs;\n  usort($sorted,\"compare_rank\");\n  return $sorted;\n}\n\nfunction report_results(&$docs)\n{\n  echo \"<table cellspacing=\\\"2\\\">\\n\";\n  echo \"  <tr>\\n\";\n  echo \"    <td colspan=\\\"2\\\"><h2>\".search_results().\"</h2></td>\\n\";\n  echo \"  </tr>\\n\";\n  $numDocs = sizeof($docs);\n  if ($numDocs==0)\n  {\n    echo \"  <tr>\\n\";\n    echo \"    <td colspan=\\\"2\\\">\".matches_text(0).\"</td>\\n\";\n    echo \"  </tr>\\n\";\n  }\n  else\n  {\n    echo \"  <tr>\\n\";\n    echo \"    <td colspan=\\\"2\\\">\".matches_text($numDocs);\n    echo \"\\n\";\n    echo \"    </td>\\n\";\n    echo \"  </tr>\\n\";\n    $num=1;\n    foreach ($docs as $doc)\n    {\n      echo \"  <tr>\\n\";\n      echo \"    <td align=\\\"right\\\">$num.</td>\";\n      echo     \"<td><a class=\\\"el\\\" href=\\\"\".$doc[\"url\"].\"\\\">\".$doc[\"name\"].\"</a></td>\\n\";\n      echo \"  <tr>\\n\";\n      echo \"    <td></td><td class=\\\"tiny\\\">\".report_matches().\" \";\n      foreach ($doc[\"words\"] as $wordInfo)\n      {\n        $word = $wordInfo[\"word\"];\n        $matchRight = substr($wordInfo[\"match\"],strlen($word));\n        echo \"<b>$word</b>$matchRight(\".$wordInfo[\"freq\"].\") \";\n      }\n      echo \"    </td>\\n\";\n      echo \"  </tr>\\n\";\n      $num++;\n    }\n  }\n  echo \"</table>\\n\";\n}\n\nfunction main()\n{\n  if(strcmp('4.1.0', phpversion()) > 0) \n  {\n    die(\"Error: PHP version 4.1.0 or above required!\");\n  }\n  if (!($file=fopen(\"search.idx\",\"rb\"))) \n  {\n    die(\"Error: Search index file could NOT be opened!\");\n  }\n  if (readHeader($file)!=\"DOXS\")\n  {\n    die(\"Error: Header of index file is invalid!\");\n  }\n  $query=\"\";\n  if (array_key_exists(\"query\", $_GET))\n  {\n    $query=$_GET[\"query\"];\n  }\n  end_form(ereg_replace(\"[^[:alnum:]:\\\\.\\\\t ]\", \" \", $query ));\n  echo \"&nbsp;\\n<div class=\\\"searchresults\\\">\\n\";\n  $results = array();\n  $requiredWords = array();\n  $forbiddenWords = array();\n  $foundWords = array();\n  $word=strtok($query,\" \");\n  while ($word) // for each word in the search query\n  {\n    if (($word{0}=='+')) { $word=substr($word,1); $requiredWords[]=$word; }\n    if (($word{0}=='-')) { $word=substr($word,1); $forbiddenWords[]=$word; }\n    if (!in_array($word,$foundWords))\n    {\n      $foundWords[]=$word;\n      search($file,strtolower($word),$results);\n    }\n    $word=strtok(\" \");\n  }\n  $docs = array();\n  combine_results($results,$docs);\n  // filter out documents with forbidden word or that do not contain\n  // required words\n  $filteredDocs = filter_results($docs,$requiredWords,$forbiddenWords);\n  // sort the results based on rank\n  $sorted = array();\n  sort_results($filteredDocs,$sorted);\n  // report results to the user\n  report_results($sorted);\n  echo \"</div>\\n\";\n  fclose($file);\n}\n\nmain();\n\n\n?>\n<hr />\n<br>\n<br>\n<br>\n<br>\n<br>\n<br>\n<br>\n</body>\n</html>\n";


//const char Documentation::text_search_php[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<!DOCTYPE html\n   PUBLIC \"-//W3C//DTDXHTML 1.0 Strict//EN\" \"DTD/xhtml1-strict.dtd\">\n<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"fr\" lang=\"fr\">\n<head>\n <META HTTP-EQUIV=\"CONTENT-TYPE\" CONTENT=\"text/html; charset=utf-8\">\n<link href=\"classic.css\" rel=\"stylesheet\" type=\"text/css\" />\n<title> Documentation (Officielle) de la librairie LMT++ </title>\n</head>\n<body text=\"#000000\" bgcolor=\"#fcfcdf\" link=\"#0000cc\" vlink=\"#551a8b\" >\n<ul class=\"navigation\" >\n<li> <a href=\"index.html\" >Accueil</a></li>\n<li> <a href=\"list_touslesmot-cles.html\" >Tous les MOT-CL&Eacute;S</a> </li>\n<li> <a href=\"list_touslesexemples.html\" >Tous les exemples</a> </li>\n<li> <a href=\"list_touslestutoriels.html\" >Tous les tutoriels</a> </li>\n<li> <a href=\"list_lesclassesprincipales.html\" >Les classes principales</a> </li>\n<li> <a href=\"list_lesfonctionsprincipales.html\" >Les fonctions principales</a> </li>\n</ul>\n\n<?php\n\nfunction search_results()\n{\n  return \"Search Results\";\n}\n\nfunction matches_text($num)\n{\n  if ($num==0)\n  {\n    return \"Sorry, no documents matching your query.\";\n  }\n  else if ($num==1)\n  {\n    return \"Found <b>1</b> document matching your query.\";\n  }\n  else // $num>1\n  {\n    return \"Found <b>$num</b> documents matching your query. Showing best matches first.\";\n  }\n}\n\nfunction report_matches()\n{\n  return \"Matches: \";\n}\nfunction end_form($value)\n{\n  echo \"            <td><input type=\"text\" name=\"query\" value=\"$value\" size=\"20\" accesskey=\"s\"/></td>\n          </tr>\n        </table>\n      </form>\n    </li>\n  </ul>\n</div>\n\";\n}\n\nfunction readInt($file)\n{\n  $b1 = ord(fgetc($file)); $b2 = ord(fgetc($file));\n  $b3 = ord(fgetc($file)); $b4 = ord(fgetc($file));\n  return ($b1<<24)|($b2<<16)|($b3<<8)|$b4;\n}\n\nfunction readString($file)\n{\n  $result=\"\";\n  while (ord($c=fgetc($file))) $result.=$c;\n  return $result;\n}\n\nfunction readHeader($file)\n{\n  $header =fgetc($file); $header.=fgetc($file);\n  $header.=fgetc($file); $header.=fgetc($file);\n  return $header;\n}\n\nfunction computeIndex($word)\n{\n  // Fast string hashing\n  //$lword = strtolower($word);\n  //$l = strlen($lword);\n  //for ($i=0;$i<$l;$i++)\n  //{\n  //  $c = ord($lword{$i});\n  //  $v = (($v & 0xfc00) ^ ($v << 6) ^ $c) & 0xffff;\n  //}\n  //return $v;\n\n  // Simple hashing that allows for substring search\n  if (strlen($word)<2) return -1;\n  // high char of the index\n  $hi = ord($word{0});\n  if ($hi==0) return -1;\n  // low char of the index\n  $lo = ord($word{1});\n  if ($lo==0) return -1;\n  // return index\n  return $hi*256+$lo;\n}\n\nfunction search($file,$word,&$statsList)\n{\n  $index = computeIndex($word);\n  if ($index!=-1) // found a valid index\n  {\n    fseek($file,$index*4+4); // 4 bytes per entry, skip header\n    $index = readInt($file);\n    if ($index) // found words matching the hash key\n    {\n      $start=sizeof($statsList);\n      $count=$start;\n      fseek($file,$index);\n      $w = readString($file);\n      while ($w)\n      {\n        $statIdx = readInt($file);\n        if ($word==substr($w,0,strlen($word)))\n        { // found word that matches (as substring)\n          $statsList[$count++]=array(\n              \"word\"=>$word,\n              \"match\"=>$w,\n              \"index\"=>$statIdx,\n              \"full\"=>strlen($w)==strlen($word),\n              \"docs\"=>array()\n              );\n        }\n        $w = readString($file);\n      }\n      $totalHi=0;\n      $totalFreqHi=0;\n      $totalFreqLo=0;\n      for ($count=$start;$count<sizeof($statsList);$count++)\n      {\n        $statInfo = &$statsList[$count];\n        $multiplier = 1;\n        // whole word matches have a double weight\n        if ($statInfo[\"full\"]) $multiplier=2;\n        fseek($file,$statInfo[\"index\"]); \n        $numDocs = readInt($file);\n        $docInfo = array();\n        // read docs info + occurrence frequency of the word\n        for ($i=0;$i<$numDocs;$i++)\n        {\n          $idx=readInt($file); \n          $freq=readInt($file); \n          $docInfo[$i]=array(\"idx\"  => $idx,\n                             \"freq\" => $freq>>1,\n                             \"rank\" => 0.0,\n                             \"hi\"   => $freq&1\n                            );\n          if ($freq&1) // word occurs in high priority doc\n          {\n            $totalHi++;\n            $totalFreqHi+=$freq*$multiplier;\n          }\n          else // word occurs in low priority doc\n          {\n            $totalFreqLo+=$freq*$multiplier;\n          }\n        }\n        // read name and url info for the doc\n        for ($i=0;$i<$numDocs;$i++)\n        {\n          fseek($file,$docInfo[$i][\"idx\"]);\n          $docInfo[$i][\"name\"]=readString($file);\n          $docInfo[$i][\"url\"]=readString($file);\n        }\n        $statInfo[\"docs\"]=$docInfo;\n      }\n      $totalFreq=($totalHi+1)*$totalFreqLo + $totalFreqHi;\n      for ($count=$start;$count<sizeof($statsList);$count++)\n      {\n        $statInfo = &$statsList[$count];\n        $multiplier = 1;\n        // whole word matches have a double weight\n        if ($statInfo[\"full\"]) $multiplier=2;\n        for ($i=0;$i<sizeof($statInfo[\"docs\"]);$i++)\n        {\n          $docInfo = &$statInfo[\"docs\"];\n          // compute frequency rank of the word in each doc\n          $freq=$docInfo[$i][\"freq\"];\n          if ($docInfo[$i][\"hi\"])\n          {\n            $statInfo[\"docs\"][$i][\"rank\"]=\n              (float)($freq*$multiplier+$totalFreqLo)/$totalFreq;\n          }\n          else\n          {\n            $statInfo[\"docs\"][$i][\"rank\"]=\n              (float)($freq*$multiplier)/$totalFreq;\n          }\n        }\n      }\n    }\n  }\n  return $statsList;\n}\n\nfunction combine_results($results,&$docs)\n{\n  foreach ($results as $wordInfo)\n  {\n    $docsList = &$wordInfo[\"docs\"];\n    foreach ($docsList as $di)\n    {\n      $key=$di[\"url\"];\n      $rank=$di[\"rank\"];\n      if (in_array($key, array_keys($docs)))\n      {\n        $docs[$key][\"rank\"]+=$rank;\n      }\n      else\n      {\n        $docs[$key] = array(\"url\"=>$key,\n            \"name\"=>$di[\"name\"],\n            \"rank\"=>$rank\n            );\n      }\n      $docs[$key][\"words\"][] = array(\n               \"word\"=>$wordInfo[\"word\"],\n               \"match\"=>$wordInfo[\"match\"],\n               \"freq\"=>$di[\"freq\"]\n               );\n    }\n  }\n  return $docs;\n}\n\nfunction filter_results($docs,&$requiredWords,&$forbiddenWords)\n{\n  $filteredDocs=array();\n  while (list ($key, $val) = each ($docs)) \n  {\n    $words = &$docs[$key][\"words\"];\n    $copy=1; // copy entry by default\n    if (sizeof($requiredWords)>0)\n    {\n      foreach ($requiredWords as $reqWord)\n      {\n        $found=0;\n        foreach ($words as $wordInfo)\n        { \n          $found = $wordInfo[\"word\"]==$reqWord;\n          if ($found) break;\n        }\n        if (!$found) \n        {\n          $copy=0; // document contains none of the required words\n          break;\n        }\n      }\n    }\n    if (sizeof($forbiddenWords)>0)\n    {\n      foreach ($words as $wordInfo)\n      {\n        if (in_array($wordInfo[\"word\"],$forbiddenWords))\n        {\n          $copy=0; // document contains a forbidden word\n          break;\n        }\n      }\n    }\n    if ($copy) $filteredDocs[$key]=$docs[$key];\n  }\n  return $filteredDocs;\n}\n\nfunction compare_rank($a,$b)\n{\n  if ($a[\"rank\"] == $b[\"rank\"]) \n  {\n    return 0;\n  }\n  return ($a[\"rank\"]>$b[\"rank\"]) ? -1 : 1; \n}\n\nfunction sort_results($docs,&$sorted)\n{\n  $sorted = $docs;\n  usort($sorted,\"compare_rank\");\n  return $sorted;\n}\n\nfunction report_results(&$docs)\n{\n  echo \"<table cellspacing=\"2\">\n\";\n  echo \"  <tr>\n\";\n  echo \"    <td colspan=\"2\"><h2>\".search_results().\"</h2></td>\n\";\n  echo \"  </tr>\n\";\n  $numDocs = sizeof($docs);\n  if ($numDocs==0)\n  {\n    echo \"  <tr>\n\";\n    echo \"    <td colspan=\"2\">\".matches_text(0).\"</td>\n\";\n    echo \"  </tr>\n\";\n  }\n  else\n  {\n    echo \"  <tr>\n\";\n    echo \"    <td colspan=\"2\">\".matches_text($numDocs);\n    echo \"\n\";\n    echo \"    </td>\n\";\n    echo \"  </tr>\n\";\n    $num=1;\n    foreach ($docs as $doc)\n    {\n      echo \"  <tr>\n\";\n      echo \"    <td align=\"right\">$num.</td>\";\n      echo     \"<td><a class=\"el\" href=\"\".$doc[\"url\"].\"\">\".$doc[\"name\"].\"</a></td>\n\";\n      echo \"  <tr>\n\";\n      echo \"    <td></td><td class=\"tiny\">\".report_matches().\" \";\n      foreach ($doc[\"words\"] as $wordInfo)\n      {\n        $word = $wordInfo[\"word\"];\n        $matchRight = substr($wordInfo[\"match\"],strlen($word));\n        echo \"<b>$word</b>$matchRight(\".$wordInfo[\"freq\"].\") \";\n      }\n      echo \"    </td>\n\";\n      echo \"  </tr>\n\";\n      $num++;\n    }\n  }\n  echo \"</table>\n\";\n}\n\nfunction main()\n{\n  if(strcmp('4.1.0', phpversion()) > 0) \n  {\n    die(\"Error: PHP version 4.1.0 or above required!\");\n  }\n  if (!($file=fopen(\"search.idx\",\"rb\"))) \n  {\n    die(\"Error: Search index file could NOT be opened!\");\n  }\n  if (readHeader($file)!=\"DOXS\")\n  {\n    die(\"Error: Header of index file is invalid!\");\n  }\n  $query=\"\";\n  if (array_key_exists(\"query\", $_GET))\n  {\n    $query=$_GET[\"query\"];\n  }\n  end_form(ereg_replace(\"[^[:alnum:]:\\\\.\\\\t ]\", \" \", $query ));\n  echo \"&nbsp;\n<div class=\"searchresults\">\n\";\n  $results = array();\n  $requiredWords = array();\n  $forbiddenWords = array();\n  $foundWords = array();\n  $word=strtok($query,\" \");\n  while ($word) // for each word in the search query\n  {\n    if (($word{0}=='+')) { $word=substr($word,1); $requiredWords[]=$word; }\n    if (($word{0}=='-')) { $word=substr($word,1); $forbiddenWords[]=$word; }\n    if (!in_array($word,$foundWords))\n    {\n      $foundWords[]=$word;\n      search($file,strtolower($word),$results);\n    }\n    $word=strtok(\" \");\n  }\n  $docs = array();\n  combine_results($results,$docs);\n  // filter out documents with forbidden word or that do not contain\n  // required words\n  $filteredDocs = filter_results($docs,$requiredWords,$forbiddenWords);\n  // sort the results based on rank\n  $sorted = array();\n  sort_results($filteredDocs,$sorted);\n  // report results to the user\n  report_results($sorted);\n  echo \"</div>\n\";\n  fclose($file);\n}\n\nmain();\n\n\n?>\n<hr />\n<br>\n<br>\n<br>\n<br>\n<br>\n<br>\n<br>\n</body>\n</html>\n";

















