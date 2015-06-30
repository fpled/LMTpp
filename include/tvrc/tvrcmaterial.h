//
// C++ Interface: tvrcmaterial
//
// Description: 
//
//
// Author: Hugo LECLERC <leclerc@lmt.ens-cachan.fr>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LMT_tvrcmaterial_HEADER
#define LMT_tvrcmaterial_HEADER

#include "../io/xmlnode.h"

namespace LMT {


class TvrcMaterial {
public:
    TvrcMaterial():has_been_read(false) {}
    bool has_been_read;
};

/**
contain young modulus, volumic_mass, ...

@author Hugo LECLERC
*/
template<class T>
class TvrcMaterialSolidVolume : public TvrcMaterial {
public:
    void read_xml_data(const XmlNode &sn) throw(IoException) {
        sn.get_attribute_with_unit("young","Pa",young,young);
        sn.get_attribute_with_unit("poisson","1",poisson,poisson);
        sn.get_attribute_with_unit("volumic_mass","kg/m^3",volumic_mass,volumic_mass);
        has_been_read = true;
    }
    T young;
    T poisson;
    T volumic_mass;
};

///
template<class T>
class TvrcMaterialSolidSurface : public TvrcMaterialSolidVolume<T> {
public:
    void read_xml_data(const XmlNode &sn) throw(IoException) {
        TvrcMaterialSolidVolume<T>::read_xml_data(sn);
        sn.get_attribute_with_unit("thickness","m",thickness,thickness);
    }
    T thickness;
};

///
template<class T>
class TvrcMaterialSolidCurve : public TvrcMaterialSolidVolume<T> {
public:
    void read_xml_data(const XmlNode &sn) throw(IoException) {
        TvrcMaterialSolidVolume<T>::read_xml_data(sn);
        sn.get_attribute_with_unit("thickness","m",thickness,thickness);
    }
    T thickness;
};

};

#endif // LMT_tvrcmaterial_HEADER
