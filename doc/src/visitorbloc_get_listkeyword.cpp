#include <iostream>
#include <string>
#include <vector>
#include<fstream>
#include<iomanip>
#include<cstdlib>

#include<cmath>
#include"time.h"

using namespace std;

#include "op.h"
#include "parameter.h"
#include "classe.h"
#include "classemetil.h"
#include "struct.h"
#include "function.h"
#include "functionmetil.h"
#include "bloc.h"
#include "visitorbloc.h"
#include "visitorbloc_get_listkeyword.h"

#include "util2html.h"
#include "token.h"
#include "templateparameter.h"
#include "visitorcommentitem_getkeyword_of_extuge.h"

void VisitorBloc_getListKeyword::exec( Classe* c ) {

    int i,n,j,m;

    /// on veut récupérer tous les keyword de tous les commentaires associés à cette classe.
    VisitorCommentItem_getKeyword_of_ExTuGe visi;
    n = c->listTag.size();
    for(i=0;i<n;i++) {
        m = c->listTag[i]->items.size();
        for(j=0;j<m;j++) {
            c->listTag[i]->items[j]->execute_function( &visi );
        }
    }
    if (visi.isExtuge==0)
        listkeyword_of_class_struct = visi.listNameKeyword;
}

void VisitorBloc_getListKeyword::exec( ClasseMetil* c ) {

    int i,n,j,m;

    /// on veut récupérer tous les keyword de tous les commentaires associés à cette classe.
    VisitorCommentItem_getKeyword_of_ExTuGe visi;
    n = c->listTag.size();
    for(i=0;i<n;i++) {
        m = c->listTag[i]->items.size();
        for(j=0;j<m;j++) {
            c->listTag[i]->items[j]->execute_function( &visi );
        }
    }
    if (visi.isExtuge==0)
        listkeyword_of_class_struct = visi.listNameKeyword;
}

void VisitorBloc_getListKeyword::exec( Struct* s ) { 

    int i,n,j,m;

    // on veut récupérer tous les keyword de tous les commentaires associés à cette classe.
    VisitorCommentItem_getKeyword_of_ExTuGe visi;
    n = s->listTag.size();
    for(i=0;i<n;i++) {
        m = s->listTag[i]->items.size();
        for(j=0;j<m;j++) {
            s->listTag[i]->items[j]->execute_function( &visi );
            //s->listTag[i]->items[j]->display( cerr );
        }
    }
    if (visi.isExtuge==0)
        listkeyword_of_class_struct = visi.listNameKeyword;
}

void VisitorBloc_getListKeyword::exec( Function* f ) {

    int i,n,j,m;

    // on veut récupérer tous les keyword de tous les commentaires associés à cette classe.
    VisitorCommentItem_getKeyword_of_ExTuGe visi;
    n = f->listTag.size();
    for(i=0;i<n;i++) {
        m = f->listTag[i]->items.size();
        for(j=0;j<m;j++) {
            f->listTag[i]->items[j]->execute_function( &visi );
        }
    }
    if (visi.isExtuge==0)
        listkeyword_of_function = visi.listNameKeyword;
}

void VisitorBloc_getListKeyword::exec( FunctionMetil* f ) {

    int i,n,j,m;

    // on veut récupérer tous les keyword de tous les commentaires associés à cette classe.
    VisitorCommentItem_getKeyword_of_ExTuGe visi;
    n = f->listTag.size();
    for(i=0;i<n;i++) {
        m = f->listTag[i]->items.size();
        for(j=0;j<m;j++) {
            f->listTag[i]->items[j]->execute_function( &visi );
        }
    }
    if (visi.isExtuge==0)
        listkeyword_of_function = visi.listNameKeyword;
}









