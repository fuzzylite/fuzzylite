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
 * Rectangle.cpp
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#include "fl/term/Rectangle.h"

#include <sstream>

namespace fl {

    Rectangle::Rectangle(const std::string& name, scalar minimum,
            scalar maximum)
    : Term(name), _minimum(minimum), _maximum(maximum) { }

    Rectangle::~Rectangle() { }

    std::string Rectangle::className() const {
        return "Rectangle";
    }

    Rectangle* Rectangle::copy() const {
        return new Rectangle(*this);
    }

    scalar Rectangle::membership(scalar x) const {
        if (fl::Op::isNan(x)) return fl::nan;
        if (fl::Op::isLt(x, _minimum) or fl::Op::isGt(x, _maximum))
            return 0.0;
        return 1.0;
    }

    std::string Rectangle::toString() const {
        std::ostringstream ss;
        ss << className() << " ("
                << fl::Op::str(_minimum) << ", "
                << fl::Op::str(_maximum) << ")";
        return ss.str();
    }

    void Rectangle::setMinimum(scalar minimum) {
        this->_minimum = minimum;
    }

    scalar Rectangle::getMinimum() const {
        return this->_minimum;
    }

    void Rectangle::setMaximum(scalar maximum) {
        this->_maximum = maximum;
    }

    scalar Rectangle::getMaximum() const {
        return this->_maximum;
    }

}
