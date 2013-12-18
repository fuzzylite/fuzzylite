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
 * File:   Ramp.cpp
 * Author: jcrada
 * 
 * Created on 22 December 2012, 4:27 PM
 */

#include "fl/term/Ramp.h"

namespace fl {

    Ramp::Ramp(const std::string& name, scalar start, scalar end)
    : Term(name), _start(start), _end(end) {
    }

    Ramp::~Ramp() {
    }

    std::string Ramp::className() const {
        return "Ramp";
    }

    std::string Ramp::parameters() const {
        return Op::join(2, " ", _start, _end);
    }

    void Ramp::configure(const std::string& parameters) {
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

    scalar Ramp::membership(scalar x) const {
        if (fl::Op::isNan(x)) return fl::nan;

        if (Op::isEq(_start, _end)) return 0.0;

        if (Op::isLt(_start, _end)) {
            if (Op::isLE(x, _start)) return 0.0;
            if (Op::isGE(x, _end)) return 1.0;
            return (x - _start) / (_end - _start);
        } else {
            if (Op::isGE(x, _start)) return 0.0;
            if (Op::isLE(x, _end)) return 1.0;
            return (_start - x) / (_start - _end);
        }
    }

    void Ramp::setStart(scalar start) {
        this->_start = start;
    }

    scalar Ramp::getStart() const {
        return this->_start;
    }

    void Ramp::setEnd(scalar end) {
        this->_end = end;
    }

    scalar Ramp::getEnd() const {
        return this->_end;
    }

    Ramp* Ramp::copy() const {
        return new Ramp(*this);
    }

    Term* Ramp::constructor() {
        return new Ramp;
    }

}