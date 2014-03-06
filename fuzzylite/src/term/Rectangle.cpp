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
 * Rectangle.cpp
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#include "fl/term/Rectangle.h"

#include <sstream>

namespace fl {

    Rectangle::Rectangle(const std::string& name, scalar start,
            scalar end)
    : Term(name), _start(start), _end(end) {
    }

    Rectangle::~Rectangle() {
    }

    std::string Rectangle::className() const {
        return "Rectangle";
    }

    scalar Rectangle::membership(scalar x) const {
        if (fl::Op::isNaN(x)) return fl::nan;
        if (fl::Op::isLt(x, _start) or fl::Op::isGt(x, _end))
            return 0.0;
        return 1.0;
    }

    std::string Rectangle::parameters() const {
        return Op::join(2, " ", _start, _end);
    }

    void Rectangle::configure(const std::string& parameters) {
        if (parameters.empty()) return;
        std::vector<std::string> values = Op::split(parameters, " ");
        std::size_t required = 2;
        if (values.size() < required) {
            std::ostringstream ex;
            ex << "[configuration error] term <" << className() << ">"
                    << " requires <" << required << "> parameters";
            throw fl::Exception(ex.str(), FL_AT);
        }
        setStart(Op::toScalar(values.at(0)));
        setEnd(Op::toScalar(values.at(1)));
    }

    void Rectangle::setStart(scalar minimum) {
        this->_start = minimum;
    }

    scalar Rectangle::getStart() const {
        return this->_start;
    }

    void Rectangle::setEnd(scalar maximum) {
        this->_end = maximum;
    }

    scalar Rectangle::getEnd() const {
        return this->_end;
    }

    Rectangle* Rectangle::copy() const {
        return new Rectangle(*this);
    }

    Term* Rectangle::constructor() {
        return new Rectangle;
    }

}
