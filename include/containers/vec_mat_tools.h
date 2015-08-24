#ifndef vec_mat_tools_h
#define vec_mat_tools_h

#include "../containers/mat.h"

///                                                  ///////////////////
/// ------------------------------------------------ /// conventions ///
///                                                  ///////////////////

/// --------------------------------------------- /// nb_dim, nb_vec ///

constexpr unsigned int get_nb_dim(const unsigned int &nb_vec) { return std::ceil(double(nb_vec)/2);}
constexpr unsigned int get_nb_vec(const unsigned int &nb_dim) { return nb_dim*(nb_dim+1)/2;}

/// ---------------------------------------------------- /// indices ///

template<unsigned int nb_dim>
inline unsigned int get_IJ(const unsigned int &i, const unsigned int &j)
{
    if (nb_dim == 2)
    {
             if ((i == 0) and (j == 0)) return 0;
        else if ((i == 1) and (j == 1)) return 1;
        else if ((i == 0) and (j == 1)) return 2;
        else if ((i == 1) and (j == 0)) return 2;
        else assert(0);
    }
    else if (nb_dim == 3)
    {
             if ((i == 0) and (j == 0)) return 0;
        else if ((i == 1) and (j == 1)) return 1;
        else if ((i == 2) and (j == 2)) return 2;
        else if ((i == 0) and (j == 1)) return 3;
        else if ((i == 1) and (j == 0)) return 3;
        else if ((i == 0) and (j == 2)) return 4;
        else if ((i == 2) and (j == 0)) return 4;
        else if ((i == 1) and (j == 2)) return 5;
        else if ((i == 2) and (j == 1)) return 5;
        else assert(0);
    }
    else assert(0);
}

template<unsigned int nb_dim>
inline unsigned int get_i(const unsigned int &IJ)
{
    if (nb_dim == 2)
    {
             if (IJ == 0) return 0;
        else if (IJ == 1) return 1;
        else if (IJ == 2) return 0;
        else assert(0);
    }
    if (nb_dim == 3)
    {
             if (IJ == 0) return 0;
        else if (IJ == 1) return 1;
        else if (IJ == 2) return 2;
        else if (IJ == 3) return 0;
        else if (IJ == 4) return 0;
        else if (IJ == 5) return 1;
        else assert(0);
    }
    else assert(0);
}

template<unsigned int nb_dim>
inline unsigned int get_j(const unsigned int &IJ)
{
    if (nb_dim == 2)
    {
             if (IJ == 0) return 0;
        else if (IJ == 1) return 1;
        else if (IJ == 2) return 1;
        else assert(0);
    }
    if (nb_dim == 3)
    {
             if (IJ == 0) return 0;
        else if (IJ == 1) return 1;
        else if (IJ == 2) return 2;
        else if (IJ == 3) return 1;
        else if (IJ == 4) return 2;
        else if (IJ == 5) return 2;
        else assert(0);
    }
    else assert(0);
}

///      ///////////////////////////////////////////////////////////////
/// ---- /// for vectors representing second order symmetric tensors ///
///      ///////////////////////////////////////////////////////////////

/// ------------------------------------------- /// cleaning vectors ///

template<class TV>
void rem_sqr_vec_col(TV &V)
{
    static const unsigned int nb_vec = TV::static_size;
    static const unsigned int nb_dim = get_nb_dim(nb_vec);

    for (unsigned int IJ=nb_dim; IJ<nb_vec; ++IJ) V[IJ] /= sqrt(2.);
}

template<class TV>
void add_sqr_vec_col(TV &V)
{
    static const unsigned int nb_vec = TV::static_size;
    static const unsigned int nb_dim = get_nb_dim(nb_vec);
    
//     PRINT(nb_vec);
//     PRINT(nb_dim);

    for (unsigned int IJ=nb_dim; IJ<nb_vec; ++IJ) V[IJ] *= sqrt(2.);
}

/// ------------------- /// conversions between vectors and matrices ///

template<class TV>
LMT::Mat<typename LMT::TypeReduction<LMT::Multiplies, TV>::T, LMT::Sym<get_nb_dim(TV::static_size)> > vec_col_to_mat_sym(const TV &V) // pour le type on n'a pas de typename TV::T car les vecteurs peuvent être hétérogènes
{
    static const unsigned int nb_vec = TV::static_size;
    static const unsigned int nb_dim = get_nb_dim(nb_vec);

//     PRINT(V);
//     PRINT(nb_vec);
//     PRINT(nb_dim);

    typedef typename LMT::TypeReduction<LMT::Multiplies, TV>::T T;
    LMT::Mat<T, LMT::Sym<nb_dim> > M;
//     PRINT(M);

    for (unsigned int IJ=0; IJ<nb_dim; ++IJ)
    {
//         PRINT(IJ);
//         PRINT(get_i<nb_dim>(IJ));
//         PRINT(get_j<nb_dim>(IJ));
        M(get_i<nb_dim>(IJ), get_j<nb_dim>(IJ)) = V[IJ];
    }
//     PRINT(M);

    for (unsigned int IJ=nb_dim; IJ<nb_vec; ++IJ)
    {
//         PRINT(IJ);
//         PRINT(get_i<nb_dim>(IJ));
//         PRINT(get_j<nb_dim>(IJ));
        M(get_i<nb_dim>(IJ), get_j<nb_dim>(IJ)) = V[IJ]/sqrt(2.);
    }
//     PRINT(M);

    return M;
}

template<class TM>
LMT::Vec<typename TM::T, get_nb_vec(TM::static_rows)> mat_sym_to_vec_col(const TM &M)
{
    static const unsigned int nb_dim = TM::static_rows;
    static const unsigned int nb_vec = get_nb_vec(nb_dim);

    typedef typename TM::T T;
    LMT::Vec<T, nb_vec> V;
    for (unsigned int IJ=0;      IJ<nb_dim; ++IJ) V[IJ] = M(get_i<nb_dim>(IJ), get_j<nb_dim>(IJ));
    for (unsigned int IJ=nb_dim; IJ<nb_vec; ++IJ) V[IJ] = M(get_i<nb_dim>(IJ), get_j<nb_dim>(IJ)) * sqrt(2.);
    return V;
}

/// ------------------------------------------------------ /// unity ///

template<unsigned int nb_dim>
inline LMT::Mat<double, LMT::Diag<nb_dim> > I2_mat()
{
    LMT::Mat<double, LMT::Diag<nb_dim> > res;
    res.set(1.);
    return res;
}

template<unsigned int nb_dim>
inline LMT::Vec<double, get_nb_vec(nb_dim)> I2_vec()
{
    return mat_sym_to_vec_col(I2_mat<nb_dim>());
}

/// ------------------------------------------------------ /// trace ///

template<class TV>
typename LMT::TypeReduction<LMT::Multiplies, TV>::T trace_vec_col(const TV &V) // pour le type on n'a pas de typename TV::T car les vecteurs peuvent être hétérogènes
{
    static const unsigned int nb_vec = TV::static_size;
    static const unsigned int nb_dim = get_nb_dim(nb_vec);

    typedef typename LMT::TypeReduction<LMT::Multiplies, TV>::T T;
    T trace = 0.;
    for (unsigned int i=0; i<nb_dim; ++i) trace += V[i];
    return trace;
}

/// ------------------------------------------- /// produit scalaire ///

template<class TV>
typename LMT::TypeReduction<LMT::Multiplies, TV>::T dot_vec_col(const TV &V1, const TV &V2)
{
    return LMT::dot(V1, V2);
}

template<class TV>
typename LMT::TypeReduction<LMT::Multiplies, TV>::T self_dot_vec_col(const TV &V)
{
    return LMT::dot(V, V);
}

/// ---------------------------------------------------- /// produit ///

// le produit de deux matrices symétriques n'est symétrique que si les deux matrices commutent
// ici on ne le définit que pour une matrice avec elle-même, auquel cas le produit est forcément symétrique

template<class TV>
TV self_prod_vec_col(const TV &V)
{
    return mat_sym_to_vec_col(vec_col_to_mat_sym(V) * vec_col_to_mat_sym(V));
}

template<class TV>
TV double_prod_vec_col(const TV &V1, const TV &V2, const TV &V3)
{
    return mat_sym_to_vec_col(vec_col_to_mat_sym(V1) * vec_col_to_mat_sym(V2) * vec_col_to_mat_sym(V3));
}

/// -------------------------------------------------- /// puissance ///

template<class TM>
TM pow_mat_sym(const TM &M, const unsigned &p)
{
    TM res;
    res.set(0.);
    res.diag() = 1.;
    for (unsigned int k=0; k<p; ++k) res *= M;
    return res;
}

template<class TV>
TV pow_vec_col(const TV &V, const unsigned &p)
{
    return mat_sym_to_vec_col(pow_mat_sym(vec_col_to_mat_sym(V), p));
}

/// ------------------------------------------ /// produit tensoriel ///

template<class TV>
LMT::Mat<typename LMT::TypeReduction<LMT::Multiplies, TV>::T, LMT::Gen<TV::static_size> > tens_prod_vec_col(const TV &V1, const TV &V2) // pour le type on n'a pas de typename TV::T car les vecteurs peuvent être hétérogènes
{
    static const unsigned int nb_vec = TV::static_size;

    typedef typename LMT::TypeReduction<LMT::Multiplies, TV>::T T;
    LMT::Mat<T, LMT::Gen<nb_vec> > M;
    for (unsigned int IJ=0; IJ<nb_vec; ++IJ)
    for (unsigned int KL=0; KL<nb_vec; ++KL)
    {
        M(IJ,KL) = V1[IJ] * V2[KL];
    }
    return M;
}

template<class TV>
LMT::Mat<typename LMT::TypeReduction<LMT::Multiplies, TV>::T, LMT::Sym<TV::static_size> > self_tens_prod_vec_col(const TV &V) // pour le type on n'a pas de typename TV::T car les vecteurs peuvent être hétérogènes
{
    static const unsigned int nb_vec = TV::static_size;

    typedef typename LMT::TypeReduction<LMT::Multiplies, TV>::T T;
    LMT::Mat<T, LMT::Sym<nb_vec> > M;
    for (unsigned int IJ=0; IJ<nb_vec; ++IJ)
    for (unsigned int KL=IJ; KL<nb_vec; ++KL)
    {
        M(IJ,KL) = V[IJ] * V[KL];
    }
    return M;
}

/// -------------------------------- /// produit tensoriel symétrisé ///

template<class TM>
void add_sqr_mat_col_gen(TM &M)
{
    static const unsigned int nb_vec = TM::static_rows;
    static const unsigned int nb_dim = get_nb_dim(nb_vec);

    for (unsigned int IJ=0;      IJ<nb_vec; ++IJ)
    for (unsigned int KL=nb_dim; KL<nb_vec; ++KL)
    {
        M(IJ,KL) *= sqrt(2.);
        M(KL,IJ) *= sqrt(2.);
    }
}

template<class TM>
void add_sqr_mat_col_sym(TM &M)
{
    static const unsigned int nb_vec = TM::static_rows;
    static const unsigned int nb_dim = get_nb_dim(nb_vec);

    for (unsigned int IJ=0;      IJ<nb_vec; ++IJ)
    for (unsigned int KL=nb_dim; KL<nb_vec; ++KL)
    {
        if (KL >= IJ) M(IJ,KL) *= sqrt(2.);
        if (IJ >= KL) M(KL,IJ) *= sqrt(2.);
    }
}

template<class TV>
LMT::Mat<typename LMT::TypeReduction<LMT::Multiplies, TV>::T, LMT::Gen<TV::static_size> > sym_tens_prod_vec_col(const TV &V1, const TV &V2)
{
    static const unsigned int nb_vec = TV::static_size;
    static const unsigned int nb_dim = get_nb_dim(nb_vec);

    typedef typename LMT::TypeReduction<LMT::Multiplies, TV>::T T;
    LMT::Mat<T, LMT::Gen<nb_vec> > M;

    LMT::Mat<T, LMT::Sym<nb_dim> > MV1 = vec_col_to_mat_sym(V1); // because of the micmac with indices, one has to go back to the matrices
    LMT::Mat<T, LMT::Sym<nb_dim> > MV2 = vec_col_to_mat_sym(V2);

    for (unsigned int IJ=0; IJ<nb_vec; ++IJ)
    for (unsigned int KL=0; KL<nb_vec; ++KL)
    {
        M(IJ,KL) = (MV1(get_i<nb_dim>(IJ), get_i<nb_dim>(KL)) * MV2(get_j<nb_dim>(KL), get_j<nb_dim>(IJ)) +
                    MV1(get_i<nb_dim>(IJ), get_j<nb_dim>(KL)) * MV2(get_i<nb_dim>(KL), get_j<nb_dim>(IJ)))/2;
    }
    add_sqr_mat_col_gen(M); // and put the sqrt() afterwards
    return M;
}

template<class TV>
LMT::Mat<typename LMT::TypeReduction<LMT::Multiplies, TV>::T, LMT::Sym<TV::static_size> > self_sym_tens_prod_vec_col(const TV &V)
{
    static const unsigned int nb_vec = TV::static_size;
    static const unsigned int nb_dim = get_nb_dim(nb_vec);

    typedef typename LMT::TypeReduction<LMT::Multiplies, TV>::T T;
    LMT::Mat<T, LMT::Sym<nb_vec> > M;

    LMT::Mat<T, LMT::Sym<nb_dim> > MV = vec_col_to_mat_sym(V); // because of the micmac with indices, one has to go back to the matrices

    for (unsigned int IJ=0;  IJ<nb_vec; ++IJ)
    for (unsigned int KL=IJ; KL<nb_vec; ++KL)
    {
        M(IJ,KL) = (MV(get_i<nb_dim>(IJ), get_i<nb_dim>(KL)) * MV(get_j<nb_dim>(KL), get_j<nb_dim>(IJ)) +
                    MV(get_i<nb_dim>(IJ), get_j<nb_dim>(KL)) * MV(get_i<nb_dim>(KL), get_j<nb_dim>(IJ)))/2;
    }
    add_sqr_mat_col_sym(M); // and put the sqrt() afterwards
    return M;
}

/// ----------------------------------------------------- /// normes ///

template<class TV>
typename LMT::TypeReduction<LMT::Multiplies, TV>::T norm_2_vec_col(const TV &V) // pour le type on n'a pas de typename TV::T car les vecteurs peuvent être hétérogènes
{
    return sqrt(self_dot_vec_col(V));
}

///     ////////////////////////////////////////////////////////////////
/// --- /// for matrices representing fourth order symmetric tensors ///
///     ////////////////////////////////////////////////////////////////

// only small symmetries are required for the representation of fourth order tensors as matrices
// large symmetry implies that the representation matrix is symmetric

/// ------------------------------------------ /// cleaning matrices ///

template<class TM>
void rem_sqr_mat_col_gen(TM &M)
{
    static const unsigned int nb_vec = TM::static_rows;
    static const unsigned int nb_dim = get_nb_dim(nb_vec);

    for (unsigned int IJ=0;      IJ<nb_vec; ++IJ)
    for (unsigned int KL=nb_dim; KL<nb_vec; ++KL)
    {
        M(IJ,KL) /= sqrt(2.);
        M(KL,IJ) /= sqrt(2.);
    }
}

template<class TM>
void rem_sqr_mat_col_sym(TM &M)
{
    static const unsigned int nb_vec = TM::static_rows;
    static const unsigned int nb_dim = get_nb_dim(nb_vec);

    for (unsigned int IJ=0;      IJ<nb_vec; ++IJ)
    for (unsigned int KL=nb_dim; KL<nb_vec; ++KL)
    {
        if (KL >= IJ) M(IJ,KL) /= sqrt(2.);
        if (IJ >= KL) M(KL,IJ) /= sqrt(2.);
    }
}

/// ------------------------------------------------------ /// unity ///

template<unsigned int nb_dim>
inline LMT::Mat<double, LMT::Diag<get_nb_vec(nb_dim)> > I4_mat()
{
    return self_sym_tens_prod_vec_col(I2_vec<nb_dim>());
}
/// ------------------------------------------- /// produit scalaire ///

template<class TM>
typename TM::T dot_mat_col(const TM &M1, const TM &M2)
{
    static const unsigned int nb_vec = TM::static_rows;

    typedef typename TM::T T;
    T res = 0.;
    for (unsigned int IJ=0; IJ<nb_vec; ++IJ)
    for (unsigned int KL=0; KL<nb_vec; ++KL)
    {
        res += M1(IJ,KL) * M2(IJ,KL);
    }
    return res;
}

template<class TM>
typename TM::T self_dot_mat_col(const TM &M)
{
    return dot_mat_col(M, M);
}

/// --------------------------------------------- /// double produit ///

template<class TM, class TV>
typename TM::T double_prod_mat_vec_col(const TV &V1, const TM &M, const TV &V2)
{
    TV tmp = M * V2;
    return dot_vec_col(V1, tmp);
}

/// ----------------------------------------------------- /// normes ///

template<class TM>
typename TM::T norm_2_mat_col(const TM &M)
{
    return sqrt(self_dot_mat_col(M));
}

#endif
