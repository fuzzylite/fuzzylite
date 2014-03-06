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
 * Sigmoid.cpp
 *
 *  Created on: 30/11/2012
 *      Author: jcrada
 */

#include "fl/term/Sigmoid.h"

#include <cmath>
#include <sstream>

namespace fl {

    Sigmoid::Sigmoid(const std::string& name, scalar inflection, scalar slope)
    : Term(name), _inflection(inflection), _slope(slope) {
    }

    Sigmoid::~Sigmoid() {
    }

    std::string Sigmoid::className() const {
        return "Sigmoid";
    }

    scalar Sigmoid::membership(scalar x) const {
        if (fl::Op::isNaN(x)) return fl::nan;
        return 1.0 / (1.0 + std::exp(-_slope * (x - _inflection)));
    }

    std::string Sigmoid::parameters() const {
        return Op::join(2, " ", _inflection, _slope);
    }

    void Sigmoid::configure(const std::string& parameters) {
        if (parameters.empty()) return;
        std::vector<std::string> values = Op::split(parameters, " ");
        std::size_t required = 2;
        if (values.size() < required) {
            std::ostringstream ex;
            ex << "[configuration error] term <" << className() << ">"
                    << " requires <" << required << "> parameters";
            throw fl::Exception(ex.str(), FL_AT);
        }
        setInflection(Op::toScalar(values.at(0)));
        setSlope(Op::toScalar(values.at(1)));
    }

    void Sigmoid::setSlope(scalar a) {
        this->_slope = a;
    }

    scalar Sigmoid::getSlope() const {
        return this->_slope;
    }

    void Sigmoid::setInflection(scalar c) {
        this->_inflection = c;
    }

    scalar Sigmoid::getInflection() const {
        return this->_inflection;
    }

    Sigmoid* Sigmoid::copy() const {
        return new Sigmoid(*this);
    }

    Term* Sigmoid::constructor() {
        return new Sigmoid;
    }
}
