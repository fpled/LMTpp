#include <iostream>
#include <string>
#include <vector>


using namespace std ;
//#include "token.h"
//#include "op.h"
//#include "names.h"
//#include "target.h"
//#include "listtargetcommentitem.h"


/*inline*/  /* std::ostream &operator<<( std::ostream &os, ListTargetCommentItem& t ) { 

    int i,n,m,j ;

    n = t.target_items.size() ;
    for(i=0;i<n;i++) {
        //os << "++++++++++++++++++++++++++++++++++++++" << endl ;
        os << "+++++++++ TARGET +++++++++++++++++++++" << endl ;
        os << *(t.target_items[i]->pt) << std::endl ;
        os << "+++++++++ listCommentItem ++++++++++++" << endl ;
        m = t.target_items[i]->listCommentItem.size() ;
        for(j=0;j<m;j++)
            os << t.target_items[i]->listCommentItem[j] << endl ;
    }
    return os ;
} ;
*/
