#include "../containers/mat.h"
#include <fstream>
using namespace std;
using namespace LMT;


int main() {
    Mat<double,Sym<>,SparseLine<> > M( 5, 5 );
    std::ofstream f( "toto.txt" );
    f << double( M( 1, 2 ) ) << std::endl;
}


