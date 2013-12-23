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

    Rectangle::Rectangle(const std::string& name, scalar start,
            scalar end)
    : Term(name), _start(start), _end(end) {
    }

    Rectangle::~Rectangle() {
    }

    std::string Rectangle::className() const {
        return "Rectangle";
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

    scalar Rectangle::membership(scalar x) const {
        if (fl::Op::isNan(x)) return fl::nan;
        if (fl::Op::isLt(x, _start) or fl::Op::isGt(x, _end))
            return 0.0;
        return 1.0;
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
