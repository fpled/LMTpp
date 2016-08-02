/*
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License version 2 as published by the Free Software Foundation.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESST FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public License
along with this library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.
*/

#ifndef METIL_TENSOR_ORDER_H
#define METIL_TENSOR_ORDER_H

#include "../containers/vec.h"
#include "../containers/mat.h"
#include "../containers/Tens3.h"
#include "../containers/Tens4.h"

/**
  @brief To get the static dimensionnality of objects

  Meaning of \c res
   - 0 -> scalar
   - 1 -> vector
   - 2 -> matrix
   - ...
   - -1 -> dynamic tensor order (not fixed during the compilation)
*/
template<class T> struct TensorOrder { static const int res = 0; };

template<class T,int s,class IVO>
struct TensorOrder<LMT::Vec<T,s,IVO> > {
    static const int res = 1;
};

template<class T,class STR,class STO,class IO>
struct TensorOrder<LMT::Mat<T,STR,STO,IO> > {
    static const int res = 2;
};

template<class T>
struct TensorOrder<LMT::Tens3<T> > {
    static const int res = 3;
};

template<class T>
struct TensorOrder<LMT::Tens4<T> > {
    static const int res = 4;
};

#endif // METIL_TENSOR_ORDER_H
