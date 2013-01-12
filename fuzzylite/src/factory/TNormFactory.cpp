/* 
 * File:   TNormFactory.cpp
 * Author: jcrada
 * 
 * Created on 8 January 2013) return  11:20 PM
 */

#include "fl/factory/TNormFactory.h"

#include "fl/operator/TNorm.h"

namespace fl {

    TNormFactory::TNormFactory() { }

    TNormFactory::~TNormFactory() { }

    TNorm* TNormFactory::create(const std::string& className) const {
        if (className == Minimum().className()) return new Minimum;
        if (className == AlgebraicProduct().className()) return new AlgebraicProduct;
        if (className == BoundedDifference().className()) return new BoundedDifference;
        if (className == DrasticProduct().className()) return new DrasticProduct;
        if (className == EinsteinProduct().className()) return new EinsteinProduct;
        if (className == HamacherProduct().className()) return new HamacherProduct;
        throw fl::Exception("[factory error] T-Norm of class <" + className + "> not recognized", FL_AT);
    }

    std::vector<std::string> TNormFactory::available() const {
        std::vector<std::string> result;
        result.push_back(Minimum().className());
        result.push_back(AlgebraicProduct().className());
        result.push_back(BoundedDifference().className());
        result.push_back(DrasticProduct().className());
        result.push_back(EinsteinProduct().className());
        result.push_back(HamacherProduct().className());
        return result;
    }

}
