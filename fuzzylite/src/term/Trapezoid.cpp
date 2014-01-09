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
    : Term(name), _a(a), _b(b), _c(c), _d(d) {
    }

    Trapezoid::~Trapezoid() {
    }

    std::string Trapezoid::className() const {
        return "Trapezoid";
    }

    std::string Trapezoid::parameters() const {
        return Op::join(4, " ", _a, _b, _c, _d);
    }

    void Trapezoid::configure(const std::string& parameters) {
        if (parameters.empty()) return;
        std::vector<std::string> values = Op::split(parameters, " ");
        std::size_t required = 4;
        if (values.size() < required) {
            std::ostringstream ex;
            ex << "[configuration error] term <" << className() << ">"
                    << " requires <" << required << "> parameters";
            throw fl::Exception(ex.str(), FL_AT);
        }
        setA(Op::toScalar(values.at(0)));
        setB(Op::toScalar(values.at(1)));
        setC(Op::toScalar(values.at(2)));
        setD(Op::toScalar(values.at(3)));
    }

    scalar Trapezoid::membership(scalar x) const {
        if (fl::Op::isNan(x)) return fl::nan;

        if (Op::isLE(x, _a) or Op::isGE(x, _d))
            return 0.0;
        else if (Op::isLt(x, _b))
            return Op::min(scalar(1.0), (x - _a) / (_b - _a));
        else if (Op::isLE(x, _c))
            return 1.0;
        else if (Op::isLt(x, _d))
            return (_d - x) / (_d - _c);
        return 0.0;
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

    Trapezoid* Trapezoid::copy() const {
        return new Trapezoid(*this);
    }

    Term* Trapezoid::constructor() {
        return new Trapezoid;
    }

}
