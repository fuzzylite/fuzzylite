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
 * Trapezoid.cpp
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#include "fl/term/Trapezoid.h"

#include <sstream>

namespace fl {

    Trapezoid::Trapezoid(const std::string& name, scalar a, scalar b, scalar c,
            scalar d)
    : Term(name), _a(a), _b(b), _c(c), _d(d) { }

    Trapezoid::~Trapezoid() { }

    std::string Trapezoid::className() const {
        return "Trapezoid";
    }

    Trapezoid* Trapezoid::copy() const {
        return new Trapezoid(*this);
    }

    scalar Trapezoid::membership(scalar x) const {
        if (fl::Op::isNan(x)) return fl::nan;

        scalar minimum = _a;
        scalar maximum = _d;
        if (Op::isLt(x, minimum) or Op::isGt(x, maximum))
            return 0.0;
        else if (Op::isLE(x, _b))
            return Op::min(1.0, (x - minimum) / (_b - minimum));
        else if (Op::isLE(x, _c))
            return 1.0;
        else if (Op::isLE(x, maximum))
            return (maximum - x) / (maximum - _c);
        return 0.0;
    }

    std::string Trapezoid::toString() const {
        std::ostringstream ss;
        ss << className() << " ("
                << fl::Op::str(_a) << ", "
                << fl::Op::str(_b) << ", "
                << fl::Op::str(_c) << ", "
                << fl::Op::str(_d) << ")";
        return ss.str();
    }

    void Trapezoid::setA(scalar a) {
        this->_a = a;
    }

    scalar Trapezoid::getA() const {
        return this->_a;
    }

    void Trapezoid::setB(scalar b) {
        this->_b = b;
    }

    scalar Trapezoid::getB() const {
        return this->_b;
    }

    void Trapezoid::setC(scalar c) {
        this->_c = c;
    }

    scalar Trapezoid::getC() const {
        return this->_c;
    }

    void Trapezoid::setD(scalar d) {
        this->_d = d;
    }

    scalar Trapezoid::getD() const {
        return this->_d;
    }

}
