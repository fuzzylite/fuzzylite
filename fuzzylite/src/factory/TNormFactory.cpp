/*   Copyright 2013 Juan Rada-Vilela

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

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
