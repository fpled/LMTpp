#include<iostream>
#include<cstdlib>
#include<fstream>
#include<vector>

using namespace std ;

#include "commentitemsection.h"


CommentItemSection :: CommentItemSection(string& t, int d) {
    order = d ;
    titre = t ; 
}

void CommentItemSection :: display( std::ostream &os ) {

 os << "CommentItemSection " << endl ;
 os << "titre =" << titre << endl ;
 os << "order =" << order << endl ;
 os << " fin CommentItemSection " << endl ;
}


