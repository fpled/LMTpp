#include<iostream>
#include<cstdlib>
#include<fstream>
#include<vector>

using namespace std;

#include "commentitemkeyword.h"



void CommentItemKeyword::addParameter( string& s )
{
 parametres += s;
}

void CommentItemKeyword::display( std::ostream &os )
{
 os << " name of CommentItemKeyword =" << name.name << endl;
 os << " parametres = ]" << parametres << "[" << endl;
}
