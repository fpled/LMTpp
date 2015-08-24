//
// C++ Interface: %{MODULE}
//
// Description: 
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DISCRETEVALUES_H
#define DISCRETEVALUES_H

#include <string>
#include <sstream>
#include <vector>
#include "../io/ioexception.h"

namespace LMT {

/**
class to manage a set of discrete values. Usefull mainly because it contains it is a serializable object
 and store things like 0:0.1:10 ...
@author Hugo LECLERC
*/
template<class T>
class DiscreteValues {
public:
    DiscreteValues() {}
    ~DiscreteValues() {}
    
    static DiscreteValues fromString(const std::string s) throw(IoException) {
        DiscreteValues res;
        
        std::string tmp;
        std::istringstream is( s );
        while(true) {
            is >> tmp;
            if ( !is )
                break;
            for(unsigned i=0;i<tmp.size();++i)
                if ( tmp[i]==':' )
                    tmp[i] = ' ';
            std::istringstream is2( tmp );
            std::vector<T> v;
            while( is2.eof()==false ) {
                T t;
                is2 >> t;
                if ( ! is2 )
                    throw IoException( "Bad DiscreteValues data (bad type) : " + s );
                v.push_back(t);
            }
            TV tv;
            if ( v.size()==1 ) {
                tv.begin = v[0];
                tv.step = 1;
                tv.end = v[0];
            }
            else if ( v.size()==3 ) {
                tv.begin = v[0];
                tv.step = v[1];
                tv.end = v[2];
            }
            else
                throw IoException( "Bad DiscreteValues data (b:s:e or b) : " + s );
             res.values.push_back( tv );
        }
        return res;
    }
    std::string toString(XmlNode &sn) const {
        std::ostringstream res;
        for(unsigned i=0;i<values.size();++i) {
            if ( values[i].begin==values[i].end )
                res << values[i].begin << ( i<values.size()-1 ? " " : "" );
            else
                res << values[i].begin << ':' << values[i].step << ':' << values[i].end << ( i<values.size()-1 ? " " : "" );
        }
        return res;
    }
    
    struct TV {
        T begin,end,step;
    };
    class iterator {
    public:
        operator bool() { return (num_in_values<values->size()); }
        void operator++() {
            current_val += (*values)[num_in_values].step;
            if ( current_val > (*values)[num_in_values].end )
                if ( ++num_in_values < values->size() )
                    current_val = (*values)[num_in_values].begin;
        }
        T operator*() const { return current_val; }
    private:
        unsigned num_in_values;
        T current_val;
        std::vector<TV> *values;
        friend class DiscreteValues;
    };
    iterator begin() {
        iterator res;
        res.num_in_values=0;
        res.values = &values;
        if ( values.size() )
            res.current_val = values[0].begin;
        return res;
    }
private:
    std::vector<TV> values;
};

};
#endif
