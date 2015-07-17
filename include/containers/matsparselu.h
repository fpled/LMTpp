//
// C++ Interface: matsparselu
//
// Description: 
//
//
// Author: Hugo LECLERC <hugo.leclerc@lmt.ens-cachan.fr>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMT_matsparselu_HEADER
#define LMT_matsparselu_HEADER

namespace LMT {

template<class TT,int sr,int sc>
class Mat<TT,Gen<sr,sc>,SparseLU,void> {
public:
    static const bool fixed_size = ( sr>=0 && sc>=0 );
    static const int static_rows = sr;
    static const int static_cols = sc;
    typedef Vec<Sparse<TT> > TVl;
    typedef Vec<Sparse<TT> > TVu;
    typedef typename TVl::template SubType<0>::T T;
    typedef Vec<TVl,static_rows> TVL;
    typedef Vec<TVu,static_cols> TVU;
    typedef Gen<sr,sc> Structure;
    typedef SparseLU Storage;
    
    typedef typename TVl::RetOp RetOp;
    typedef typename TVl::RetOpConst RetOpConst;

    template<class TV> void allocate(const TV &v) { resize(v.size()); }

    void free() {
        for(unsigned i=0;i<L.size();++i) { L[i].indices.free(); L[i].data.free(); }
        for(unsigned i=0;i<U.size();++i) { U[i].indices.free(); U[i].data.free(); }
    } /// set all values to 0
    
    RetOp operator()(unsigned r,unsigned c) {
        if ( r > c )
            return L[r][c]; 
        return U[c][r];
    }
    RetOpConst operator()(unsigned r,unsigned c) const {  if ( r > c ) return L[r][c]; return U[c][r];  }
    

    unsigned nb_rows() const { return nr.val; }
    unsigned nb_cols() const { return nc.val; }

    Mat() {}
    Mat(unsigned nr) { resize(nr,nr); }
    Mat(unsigned nr,unsigned nc) { resize(nr,nc); }

    template<class T2,class STR2,class STO2,class O2> Mat(const Mat<T2,STR2,STO2,O2> &val) {
        resize( val.nb_rows(), val.nb_cols() );
        for(unsigned i=0;i<nb_rows();++i)
            for(unsigned j=0;j<nb_cols();++j)
                if ( boolean_( val(i,j) ) )
                    operator()(i,j) = val(i,j);
    }
    template<class T2> Mat(const Mat<T2,Sym<>,SparseLine<> > &val) {
        resize( val.nb_rows(), val.nb_cols() );
        for(unsigned i=0;i<nb_rows();++i) {
            L[i].indices = val.data[i].indices;
            L[i].data = val.data[i].data;
            U[i].indices = val.data[i].indices;
            U[i].data = val.data[i].data;
            if ( L[i].indices.size() and L[i].indices.back()==i ) {
                L[i].indices.pop_back();
                L[i].data.pop_back();
            }
        }
    }
    template<class T2> Mat(const Mat<T2,Gen<>,SparseLine<> > &val) {
        resize( val.nb_rows(), val.nb_cols() );
        for(unsigned i=0;i<nb_rows();++i) {
            for(unsigned j=0;j<val.data[i].indices.size();++j)
                operator()( i, val.data[i].indices[j] ) = val.data[i].data[j];
        }
    }
    
    void resize(unsigned nr) { resize( nr, nr ); }
    void resize(unsigned nr,unsigned nc) { this->nr.set(nr); this->nc.set(nc);
        L.resize(nr); for(unsigned i=0;i<nr;++i) L[i].resize(i);
        U.resize(nc); for(unsigned i=0;i<nc;++i) U[i].resize(i+1);
    }
    void reserve(unsigned n) {
        for(unsigned i=0;i<nb_rows();++i) L[i].reserve(n);
        for(unsigned i=0;i<nb_cols();++i) U[i].reserve(n);
    }
    
//     typedef Vec<VecSubMat<Mat,false,ExtractDiag>,MIN(sr,sc)> RetDiag;
//     typedef Vec<VecSubMat<Mat,true ,ExtractDiag>,MIN(sr,sc)> RetDiagConst;
//     typedef Vec<VecSubMat<Mat,true ,ExtractCol>,sr> RetColConst;
//     typedef Vec<VecSubMat<Mat,false,ExtractCol>,sr> RetCol;
//     typedef const Vec<Sparse<TT>,sc> & RetRowConst;
//     typedef Vec<Sparse<TT>,sc> & RetRow;
//     
//     // LINEVEC get_line() { return ; }
//     Vec<VecSubMat<Mat,false,ExtractDiag>,MIN(sr,sc)> diag() { return Vec<VecSubMat<Mat,false,ExtractDiag>,MIN(sr,sc)>(*this); }
//     Vec<VecSubMat<Mat,true ,ExtractDiag>,MIN(sr,sc)> diag() const { return Vec<VecSubMat<Mat,true ,ExtractDiag>,MIN(sr,sc)>(*this); }
//     
//     Vec<Sparse<TT>,sc> & row(unsigned i) { return data[i]; }
//     const Vec<Sparse<TT>,sc> & row(unsigned i) const { return data[i]; }
//     Vec<VecSubMat<Mat,false,ExtractCol>,sr> col(unsigned i) { return Vec<VecSubMat<Mat,false,ExtractCol>,sr>(*this,i); }
//     Vec<VecSubMat<Mat,true ,ExtractCol>,sr> col(unsigned i) const { return Vec<VecSubMat<Mat,true ,ExtractCol>,sr>(*this,i); }
// 
    template<class T2> Mat &operator*=(const T2 &val) {
        for(unsigned r=0;r<L.size();++r)
            for(unsigned i=0;i<L[r].data.size();++i)
                L[r].data[i] *= val; 
        for(unsigned r=0;r<U.size();++r)
            for(unsigned i=0;i<U[r].data.size();++i)
                U[r].data[i] *= val; 
        return *this;
    }
//     template<class T2> Mat &operator/=(const T2 &val) { data /= val; return *this; }
// 
//     template<class T2,class STR2,class STO2> Mat &operator+=(const Mat<T2,STR2,STO2> &val) { *this = *this + val; return *this; }
//     template<class T2,class STR2,class STO2> Mat &operator-=(const Mat<T2,STR2,STO2> &val) { *this = *this - val; return *this; }
//     template<class T2,class STR2,class STO2> Mat &operator*=(const Mat<T2,STR2,STO2> &val) { *this = *this * val; return *this; }
//     template<class T2,class STR2,class STO2> Mat &operator/=(const Mat<T2,STR2,STO2> &val) { *this = *this / val; return *this; }
// 
    typedef Vec<VecSubMat<Mat,false,ExtractDiag>,MIN(sr,sc)> RetDiag;
    typedef Vec<VecSubMat<Mat,true ,ExtractDiag>,MIN(sr,sc)> RetDiagConst;
    typedef Vec<VecSubMat<Mat,true ,ExtractCol>,sr> RetColConst;
    typedef Vec<VecSubMat<Mat,false,ExtractCol>,sr> RetCol;
    typedef const Vec<Sparse<TT>,sc> & RetRowConst;
    typedef Vec<Sparse<TT>,sc> & RetRow;
    
    // LINEVEC get_line() { return ; }
    Vec<VecSubMat<Mat,false,ExtractDiag>,MIN(sr,sc)> diag() { return Vec<VecSubMat<Mat,false,ExtractDiag>,MIN(sr,sc)>(*this); }
    Vec<VecSubMat<Mat,true ,ExtractDiag>,MIN(sr,sc)> diag() const { return Vec<VecSubMat<Mat,true ,ExtractDiag>,MIN(sr,sc)>(*this); }
    
//     Vec<Sparse<TT>,sc> & row(unsigned i) { return Vec<VecSubMat<Mat,false,ExtractRow>,sr>(*this,i); }
//     const Vec<Sparse<TT>,sc> & row(unsigned i) const { return Vec<VecSubMat<Mat,true ,ExtractRow>,sr>(*this,i); }
//     Vec<VecSubMat<Mat,false,ExtractCol>,sr> col(unsigned i) { return Vec<VecSubMat<Mat,false,ExtractCol>,sr>(*this,i); }
//     Vec<VecSubMat<Mat,true ,ExtractCol>,sr> col(unsigned i) const { return Vec<VecSubMat<Mat,true ,ExtractCol>,sr>(*this,i); }



    template<class TVEC> Mat<SubMat<Mat,true ,TVEC>,Gen<sr,sc>,SparseLU,int> operator[](const TVEC &v) const {
        return Mat<SubMat<Mat,true ,TVEC>,Gen<sr,sc>,SparseLU,int>( *this, v );
    }
    template<class TVEC> Mat<SubMat<Mat,false,TVEC>,Gen<sr,sc>,SparseLU,int> operator[](const TVEC &v) {
        return Mat<SubMat<Mat,false,TVEC>,Gen<sr,sc>,SparseLU,int>( *this, v );
    }
// 
//     template<class TVEC1,int s1,class TVEC2,int s2> Mat<SubMat<Mat,true ,Vec<TVEC1,s1>,Vec<TVEC2,s2> > > operator()(const Vec<TVEC1,s1> &v1,const Vec<TVEC2,s2> &v2) const {
//         return Mat<SubMat<Mat,true ,Vec<TVEC1,s1>,Vec<TVEC2,s2> > >( *this, v1, v2 );
//     }
//     template<class TVEC1,int s1,class TVEC2,int s2> Mat<SubMat<Mat,false,Vec<TVEC1,s1>,Vec<TVEC2,s2> > > operator()(const Vec<TVEC1,s1> &v1,const Vec<TVEC2,s2> &v2) {
//         return Mat<SubMat<Mat,false,Vec<TVEC1,s1>,Vec<TVEC2,s2> > >( *this, v1, v2 );
//     }

    void clear() {
        for(unsigned i=0;i<L.size();++i)
            L[i].clear(); 
        for(unsigned i=0;i<U.size();++i)
            U[i].clear();
    }

    TVL L;
    TVU U;
private:
    StaticValIf<unsigned,sr>=0,(unsigned)sr> nr;
    StaticValIf<unsigned,sc>=0,(unsigned)sc> nc;
};

template<class TM,class TV1,class TV2,class Structure,class Storage,class T,int s1,int s2>
void operator+=(Mat<SubMat<TM,false,TV1,TV2>,Structure,Storage> &ms,const Mat<T,Gen<s1,s2>,SparseLU> &m)  {
    for(unsigned line=0;line<m.nb_rows();++line) {
        for(unsigned ind=0;ind<m.L[line].indices.size();++ind)
            ms( line, m.L[line].indices[ind] ) += m.L[line].data[ind];
        for(unsigned ind=0;ind<m.U[line].indices.size();++ind)
            ms( m.U[line].indices[ind], line ) += m.U[line].data[ind];
    }
}

};

#endif // LMT_matsparselu_HEADER
