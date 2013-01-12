/* 
 * File:   SNormFactory.cpp
 * Author: jcrada
 * 
 * Created on 8 January 2013, 11:20 PM
 */

#include "fl/factory/SNormFactory.h"
#include "fl/operator/SNorm.h"

namespace fl {

    SNormFactory::SNormFactory() { }

    SNormFactory::~SNormFactory() { }

    SNorm* SNormFactory::create(const std::string& className) const {
        if (className == Maximum().className()) return new Maximum;
        if (className == AlgebraicSum().className()) return new AlgebraicSum;
        if (className == BoundedSum().className()) return new BoundedSum;
        if (className == NormalizedSum().className()) return new NormalizedSum;
        if (className == DrasticSum().className()) return new DrasticSum;
        if (className == EinsteinSum().className()) return new EinsteinSum;
        if (className == HamacherSum().className()) return new HamacherSum;
        throw fl::Exception("[factory error] S-Norm of class <" + className + "> not recognized", FL_AT);
    }

    std::vector<std::string> SNormFactory::available() const {
        std::vector<std::string> result;
        result.push_back(Maximum().className());
        result.push_back(AlgebraicSum().className());
        result.push_back(BoundedSum().className());
        result.push_back(NormalizedSum().className());
        result.push_back(DrasticSum().className());
        result.push_back(EinsteinSum().className());
        result.push_back(HamacherSum().className());
        return result;
    }
}
