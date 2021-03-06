#include<iostream>
#include<cstdlib>
#include<fstream>
#include<vector>

using namespace std ;

#include "commentitemtutorial.h"


CommentItemTutorial :: CommentItemTutorial( string& ref  ) {
   name.name = ref ;
   name.principalName = ref ;
}


void CommentItemTutorial :: display( std::ostream &os ) {

    int i,n ;

    n = items.size() ;
    os << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl ;
    os << "~~~~~~~~~~ CommentItemTutorial : reference =" << name.name << " ~~~~~~~~~~" << endl ;
    os << " nombre d'items = " << n << endl ;
    for(i=0;i<n;i++) {
        os  << *items[i] << endl ;
 }
 os << "fin CommentItemTutorial" << endl ;
}

CommentItemTutorial :: ~CommentItemTutorial() {

    int i,n ;

    n = items.size() ;
    for(i=0;i<n;i++)
        delete items[i] ;

}
