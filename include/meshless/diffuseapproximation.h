//
// C++ Interface: diffuseapproximation
//
// Description: 
//
//
// Author: Grover <hugo@grosnordi>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DIFFUSE_APPROXIMATION_H
#define DIFFUSE_APPROXIMATION_H

#include "../containers/vec.h"
#include "../containers/matumfpack.h"

namespace LMT {

/**
    Example :

    DiffuseApproximation da;
    da.add_node( p0, r0 );
    da.add_node( p1, r1 );
    ...
    da.calc_mat();
    da.get_coeffs( Vec<double>(v0,v1,...) );

    cout << da.get_val( pos );

 */
template<class T,unsigned dim>
class DiffuseApproximation {
public:
    typedef Vec<T,dim> V;
    struct Kernel {
        T val(const V &p) const {
            T l = 0.5 - 0.5 * dot( p - pos, p - pos ) / r2;
            return pow( l+abs(l), 2 );
        }
        
        bool contains(const V &p) const {
            return dot( p - pos, p - pos ) <= r2;
        }
        
        Vec<T,dim> pos;
        T r2;
    };
    
    void add_node( const V &pos, T r ) {
        Kernel *k = nodes.new_elem();
        k->pos = pos;
        k->r2 = r * r;
    }
    
    void calc_mat() {
        //
        Vec<Vec<unsigned> > indices; indices.resize( nodes.size() );
        for(unsigned i=0;i<nodes.size();++i)
            for(unsigned j=0;j<nodes.size();++j)
                if ( nodes[i].contains( nodes[j].pos ) )
                    indices[i].push_back(j);
        //
        mat.allocate( indices );
        for(unsigned i=0;i<nodes.size();++i)
            for(unsigned j=0;j<nodes.size();++j)
                if ( nodes[i].contains( nodes[j].pos ) )
                    mat(j,i) += nodes[i].val( nodes[j].pos );
        mat.get_factorization();
        coeffs_for_unity = mat.solve( cst_vec(T(1),nodes.size()) );
    }
    
    void get_coeffs( const Vec<double> &nodal_values ) {
        coeffs = mat.solve( nodal_values );
    }
    
    T get_val( const V &pos ) const {
        T num = 0.0;
        T den = 0.0;
        for(unsigned i=0;i<nodes.size();++i) {
            if ( nodes[i].contains( pos ) ) {
                T c = nodes[i].val( pos );
                num += coeffs[i] * c;
                den += coeffs_for_unity[i] * c;
            }
        }
        return num / den;
    }
    
    
    Vec<Splitted<Kernel,64> > nodes;
    Mat<double,Gen<>,SparseUMFPACK> mat;
    Vec<double> coeffs, coeffs_for_unity;
};

}

#endif // DIFFUSE_APPROXIMATION_H
