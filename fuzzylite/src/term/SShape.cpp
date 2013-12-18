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
 * File:   SShape.cpp
 * Author: jcrada
 * 
 * Created on 22 December 2012, 4:43 PM
 */

#include "fl/term/SShape.h"

namespace fl {

    SShape::SShape(const std::string& name, scalar start, scalar end)
    : Term(name), _start(start), _end(end) {
    }

    SShape::~SShape() {
    }

    std::string SShape::className() const {
        return "SShape";
    }

    std::string SShape::parameters() const {
        return Op::join(2, " ", _start, _end);
    }

    void SShape::configure(const std::string& parameters) {
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

    scalar SShape::membership(scalar x) const {
        if (fl::Op::isNan(x)) return fl::nan;
        //from Octave smf.m
        scalar average = (_start + _end) / 2;
        scalar difference = _end - _start;

        if (Op::isLE(x, _start)) return 0.0;

        else if (Op::isLE(x, average))
            return 2 * std::pow((x - _start) / difference, 2);

        else if (Op::isLt(x, _end))
            return 1.0 - 2.0 * std::pow((x - _end) / difference, 2);

        return 1.0;
    }

    void SShape::setStart(scalar start) {
        this->_start = start;
    }

    scalar SShape::getStart() const {
        return this->_start;
    }

    void SShape::setEnd(scalar end) {
        this->_end = end;
    }

    scalar SShape::getEnd() const {
        return this->_end;
    }

    SShape* SShape::copy() const {
        return new SShape(*this);
    }

    Term* SShape::constructor() {
        return new SShape;
    }



}