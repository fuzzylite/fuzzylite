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
 * MaximumDefuzzifier.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "fl/defuzzifier/MaximumDefuzzifier.h"

#include "fl/term/Term.h"

#include "fl/Exception.h"




namespace fl {

    MaximumDefuzzifier::MaximumDefuzzifier(Type type, int divisions)
    : Defuzzifier(divisions), _type(type) { }

    MaximumDefuzzifier::~MaximumDefuzzifier() { }

    std::string MaximumDefuzzifier::className() const {
        switch (_type) {
            case SMALLEST:
                return "SmallestOfMaximum";
            case LARGEST:
                return "LargestOfMaximum";
            case MEAN:
                return "MeanOfMaximum";
            default:
                std::ostringstream ex;
                ex << "[defuzzifier error] maximum defuzzifier of type <" <<  _type << ">"
                        " not recognized";
                throw fl::Exception(ex.str(), FL_AT);
        }
    }

    scalar MaximumDefuzzifier::defuzzify(const Term* term, scalar minimum, scalar maximum) const {
        if (maximum - minimum > _divisions) {
            FL_LOG("[accuracy warning] the number of divisions ( " << _divisions << ") "
                    "is less than the range (" << minimum << ", " << maximum << "). In order to "
                    "improve the accuracy, the number of divisions should be greater than the range.");
        }
        scalar dx = (maximum - minimum) / _divisions;
        scalar x, y;
        scalar ymax = -1.0, xsmallest, xlargest;
        bool samePlateau = false;
        for (int i = 0; i < _divisions; ++i) {
            x = minimum + (i + 0.5) * dx;
            y = term->membership(x);

            if (Op::isGt(y, ymax)) {
                xsmallest = x;
                ymax = y;
                samePlateau = true;
            } else if (Op::isEq(y, ymax) and samePlateau) {
                xlargest = x;
            } else if (Op::isLt(y, ymax)) {
                samePlateau = false;
            }
        }

        switch (_type) {
            case SMALLEST:
                return xsmallest;
            case LARGEST:
                return xlargest;
            case MEAN:
                return (xlargest + xsmallest) / 2.0;
            default:
                return fl::nan;
        }
    }
    
    void MaximumDefuzzifier::setType(Type type){
        this->_type = type;
    }
    
    MaximumDefuzzifier::Type MaximumDefuzzifier::getType() const{
        return this->_type;
    }
}
