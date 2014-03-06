// #BEGIN_LICENSE
// fuzzylite: a fuzzy logic control library in C++
// Copyright (C) 2014  Juan Rada-Vilela
// 
// This file is part of fuzzylite.
//
// fuzzylite is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// fuzzylite is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with fuzzylite.  If not, see <http://www.gnu.org/licenses/>.
// #END_LICENSE

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
        if (fl::Op::isNaN(c)) {
            this->_c = b;
            this->_b = (a + b) / 2.0;
        }
    }

    Triangle::~Triangle() {
    }

    std::string Triangle::className() const {
        return "Triangle";
    }

    std::string Triangle::parameters() const {
        return Op::join(3, " ", _a, _b, _c);
    }

    void Triangle::configure(const std::string& parameters) {
        if (parameters.empty()) return;
        std::vector<std::string> values = Op::split(parameters, " ");
        std::size_t required = 3;
        if (values.size() < required) {
            std::ostringstream ex;
            ex << "[configuration error] term <" << className() << ">"
                    << " requires <" << required << "> parameters";
            throw fl::Exception(ex.str(), FL_AT);
        }
        setA(Op::toScalar(values.at(0)));
        setB(Op::toScalar(values.at(1)));
        setC(Op::toScalar(values.at(2)));
    }

    scalar Triangle::membership(scalar x) const {
        if (fl::Op::isNaN(x)) return fl::nan;

        if (Op::isLt(x, _a) or Op::isGt(x, _c))
            return 0.0;

        if (Op::isEq(x, _b))
            return 1.0;

        if (Op::isLt(x, _b))
            return (x - _a) / (_b - _a);

        return (_c - x) / (_c - _b);
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

    Triangle* Triangle::copy() const {
        return new Triangle(*this);
    }

    Term* Triangle::constructor() {
        return new Triangle;
    }

}
