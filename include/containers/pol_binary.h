namespace LMT {

template <int nd, int ne, int nx, class T1, class T2>
Pol<MIN(nd,ne),nx,typename TypePromote<Plus,T1,T2>::T> operator+( const Pol<nd,nx,T1> &p, const Pol<ne,nx,T2> &q ) {
    PolBinOp<Plus,nd,ne,nx> op;
    return Pol<MIN(nd,ne),nx,typename TypePromote<Plus,T1,T2>::T>( op(p.coefficients(),q.coefficients()) );
}

template <int nd, int nx, class T1, class T2>
Pol<nd,nx,typename TypePromote<Plus,T1,typename IsScalar<T2>::T>::T> operator+( const Pol<nd,nx,T1> &p, const T2 &q ) {
    PolBinOp<Plus,nd,nd,nx> op;
    return Pol<nd,nx,typename TypePromote<Plus,T1,typename IsScalar<T2>::T>::T>(op(p.coefficients(),q));
}

template <int nd, int nx, class T1, class T2>
Pol<nd,nx,typename TypePromote<Plus,typename IsScalar<T1>::T,T2>::T> operator+( const T1 &p, const Pol<nd,nx,T2> &q ) {
    PolBinOp<Plus,nd,nd,nx> op;
    return Pol<nd,nx,typename TypePromote<Plus,T1,typename IsScalar<T2>::T>::T>(op(p,q.coefficients()));
}


template <int nd, int ne, int nx, class T1, class T2>
Pol<MIN(nd,ne),nx,typename TypePromote<Minus,T1,T2>::T> operator-( const Pol<nd,nx,T1> &p, const Pol<ne,nx,T2> &q ) {
    PolBinOp<Minus,nd,ne,nx> op;
    return Pol<MIN(nd,ne),nx,typename TypePromote<Minus,T1,T2>::T>( op(p.coefficients(),q.coefficients()) );
}

template <int nd, int nx, class T1, class T2>
Pol<nd,nx,typename TypePromote<Minus,T1,typename IsScalar<T2>::T>::T> operator-( const Pol<nd,nx,T1> &p, const T2 &q ) {
    PolBinOp<Minus,nd,nd,nx> op;
    return Pol<nd,nx,typename TypePromote<Minus,T1,typename IsScalar<T2>::T>::T>(op(p.coefficients(),q));
}

template <int nd, int nx, class T1, class T2>
Pol<nd,nx,typename TypePromote<Minus,typename IsScalar<T1>::T,T2>::T> operator-( const T1 &p, const Pol<nd,nx,T2> &q ) {
    PolBinOp<Minus,nd,nd,nx> op;
    return Pol<nd,nx,typename TypePromote<Minus,T1,typename IsScalar<T2>::T>::T>(op(p,q.coefficients()));
}


template <int nd, int ne, int nx, class T1, class T2>
Pol<MIN(nd,ne),nx,typename TypePromote<Multiplies,T1,T2>::T> operator*( const Pol<nd,nx,T1> &p, const Pol<ne,nx,T2> &q ) {
    PolBinOp<Multiplies,nd,ne,nx> op;
    return Pol<MIN(nd,ne),nx,typename TypePromote<Multiplies,T1,T2>::T>( op(p.coefficients(),q.coefficients()) );
}

template <int nd, int nx, class T1, class T2>
Pol<nd,nx,typename TypePromote<Multiplies,T1,typename IsScalar<T2>::T>::T> operator*( const Pol<nd,nx,T1> &p, const T2 &q ) {
    PolBinOp<Multiplies,nd,nd,nx> op;
    return Pol<nd,nx,typename TypePromote<Multiplies,T1,typename IsScalar<T2>::T>::T>(op(p.coefficients(),q));
}

template <int nd, int nx, class T1, class T2>
Pol<nd,nx,typename TypePromote<Multiplies,typename IsScalar<T1>::T,T2>::T> operator*( const T1 &p, const Pol<nd,nx,T2> &q ) {
    PolBinOp<Multiplies,nd,nd,nx> op;
    return Pol<nd,nx,typename TypePromote<Multiplies,T1,typename IsScalar<T2>::T>::T>(op(p,q.coefficients()));
}


template <int nd, int ne, int nx, class T1, class T2>
Pol<MIN(nd,ne),nx,typename TypePromote<Divides,T1,T2>::T> operator/( const Pol<nd,nx,T1> &p, const Pol<ne,nx,T2> &q ) {
    PolBinOp<Divides,nd,ne,nx> op;
    return Pol<MIN(nd,ne),nx,typename TypePromote<Divides,T1,T2>::T>( op(p.coefficients(),q.coefficients()) );
}

template <int nd, int nx, class T1, class T2>
Pol<nd,nx,typename TypePromote<Divides,T1,typename IsScalar<T2>::T>::T> operator/( const Pol<nd,nx,T1> &p, const T2 &q ) {
    PolBinOp<Divides,nd,nd,nx> op;
    return Pol<nd,nx,typename TypePromote<Divides,T1,typename IsScalar<T2>::T>::T>(op(p.coefficients(),q));
}

template <int nd, int nx, class T1, class T2>
Pol<nd,nx,typename TypePromote<Divides,typename IsScalar<T1>::T,T2>::T> operator/( const T1 &p, const Pol<nd,nx,T2> &q ) {
    PolBinOp<Divides,nd,nd,nx> op;
    return Pol<nd,nx,typename TypePromote<Divides,T1,typename IsScalar<T2>::T>::T>(op(p,q.coefficients()));
}


template <int nd, int ne, int nx, class T1, class T2>
Pol<MIN(nd,ne),nx,typename TypePromote<Pow,T1,T2>::T> pow( const Pol<nd,nx,T1> &p, const Pol<ne,nx,T2> &q ) {
    PolBinOp<Pow,nd,ne,nx> op;
    return Pol<MIN(nd,ne),nx,typename TypePromote<Pow,T1,T2>::T>( op(p.coefficients(),q.coefficients()) );
}

template <int nd, int nx, class T1, class T2>
Pol<nd,nx,typename TypePromote<Pow,T1,typename IsScalar<T2>::T>::T> pow( const Pol<nd,nx,T1> &p, const T2 &q ) {
    PolBinOp<Pow,nd,nd,nx> op;
    return Pol<nd,nx,typename TypePromote<Pow,T1,typename IsScalar<T2>::T>::T>(op(p.coefficients(),q));
}

template <int nd, int nx, class T1, class T2>
Pol<nd,nx,typename TypePromote<Pow,typename IsScalar<T1>::T,T2>::T> pow( const T1 &p, const Pol<nd,nx,T2> &q ) {
    PolBinOp<Pow,nd,nd,nx> op;
    return Pol<nd,nx,typename TypePromote<Pow,T1,typename IsScalar<T2>::T>::T>(op(p,q.coefficients()));
}


template <int nd, int ne, int nx, class T1, class T2>
bool operator<( const Pol<nd,nx,T1> &p, const Pol<ne,nx,T2> &q ) {
    return p.coefficients()[0]<q.coefficients()[0];
}

template <int nd, int nx, class T1, class T2>
bool operator<( const Pol<nd,nx,T1> &p, const T2 &q ) {
    return p.coefficients()[0]<q;
}

template <int nd, int nx, class T1, class T2>
bool operator<( const T1 &p, const Pol<nd,nx,T2> &q ) {
    return p<q.coefficients()[0];
}


template <int nd, int ne, int nx, class T1, class T2>
bool operator>( const Pol<nd,nx,T1> &p, const Pol<ne,nx,T2> &q ) {
    return p.coefficients()[0]>q.coefficients()[0];
}

template <int nd, int nx, class T1, class T2>
bool operator>( const Pol<nd,nx,T1> &p, const T2 &q ) {
    return p.coefficients()[0]>q;
}

template <int nd, int nx, class T1, class T2>
bool operator>( const T1 &p, const Pol<nd,nx,T2> &q ) {
    return p>q.coefficients()[0];
}


template <int nd, int ne, int nx, class T1, class T2>
bool operator<=( const Pol<nd,nx,T1> &p, const Pol<ne,nx,T2> &q ) {
    return p.coefficients()[0]<=q.coefficients()[0];
}

template <int nd, int nx, class T1, class T2>
bool operator<=( const Pol<nd,nx,T1> &p, const T2 &q ) {
    return p.coefficients()[0]<=q;
}

template <int nd, int nx, class T1, class T2>
bool operator<=( const T1 &p, const Pol<nd,nx,T2> &q ) {
    return p<=q.coefficients()[0];
}


template <int nd, int ne, int nx, class T1, class T2>
bool operator>=( const Pol<nd,nx,T1> &p, const Pol<ne,nx,T2> &q ) {
    return p.coefficients()[0]>=q.coefficients()[0];
}

template <int nd, int nx, class T1, class T2>
bool operator>=( const Pol<nd,nx,T1> &p, const T2 &q ) {
    return p.coefficients()[0]>=q;
}

template <int nd, int nx, class T1, class T2>
bool operator>=( const T1 &p, const Pol<nd,nx,T2> &q ) {
    return p>=q.coefficients()[0];
}


template <int nd, int nx, class T>
Pol<nd,nx,T> pow(const Pol<nd,nx,T> &p, const int q) {
    if (q>0) {
        PolBinOp<Multiplies,nd,nd,nx> opmultiplies;
        Vec<T,DimPol<nd,nx>::valeur> res = p.coefficients();
        for (int i=1;i<q;i++)
            res=opmultiplies(res,p.coefficients());
        return Pol<nd,nx,T>(res);
    }
    else if (q<0) {
        PolBinOp<Divides,nd,nd,nx> opdivides;
        Vec<T,DimPol<nd,nx>::valeur> one(T(0));
        one[0] = T(1);
        return opdivides(one,pow(p,-q).coefficients());
    }    
    else
        return (Pol<nd,nx,T>(T(1)));
}

}

