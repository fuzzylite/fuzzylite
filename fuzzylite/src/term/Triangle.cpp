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
 * Triangle.cpp
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#include "fl/term/Triangle.h"

#include <sstream>

namespace fl {

    Triangle::Triangle(const std::string& name, scalar a, scalar b, scalar c)
    : Term(name), _a(a), _b(b), _c(c) {
    }

    Triangle::~Triangle() {
    }

    std::string Triangle::className() const {
        return "Triangle";
    }

    Triangle* Triangle::copy() const {
        return new Triangle(*this);
    }

    scalar Triangle::membership(scalar x) const {
        if (fl::Op::isNan(x)) return std::numeric_limits<scalar>::quiet_NaN();

        scalar minimum = _a;
        scalar maximum = _c;
        if (Op::isLt(x, minimum) or Op::isGt(x, maximum))
            return 0.0;
        else if (Op::isEq(x, _b))
            return 1.0;
        else if (Op::isLt(x, _b))
            return (x - minimum) / (_b - minimum);
        else
            return (maximum - x) / (maximum - _b);
    }

    std::string Triangle::toString() const {
        std::ostringstream ss;
        ss << std::setprecision(FL_DECIMALS) << std::fixed;
        ss << className() << " (" << _a << ", " << _b << ", " << _c << ")";
        return ss.str();
    }

    void Triangle::setA(scalar a) {
        this->_a = a;
    }

    scalar Triangle::getA() const {
        return this->_a;
    }

    void Triangle::setB(scalar b) {
        this->_b = b;
    }

    scalar Triangle::getB() const {
        return this->_b;
    }

    void Triangle::setC(scalar c) {
        this->_c = c;
    }

    scalar Triangle::getC() const {
        return this->_c;
    }

}
