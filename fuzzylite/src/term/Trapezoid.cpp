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
        if (fl::Op::isNaN(x)) return fl::nan;

        if (Op::isLt(x, _a) or Op::isGt(x, _d))
            return 0.0;

        if (Op::isLt(x, _b))
            return Op::min(scalar(1.0), (x - _a) / (_b - _a));

        if (Op::isLE(x, _c))
            return 1.0;

        if (Op::isLt(x, _d))
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

    Trapezoid* Trapezoid::clone() const {
        return new Trapezoid(*this);
    }

    Term* Trapezoid::constructor() {
        return new Trapezoid;
    }


}
