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
 * Bell.cpp
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#include "fl/term/Bell.h"

#include <cmath>
#include <sstream>

namespace fl {

    Bell::Bell(const std::string& name, scalar center, scalar width, scalar slope)
    : Term(name), _center(center), _width(width), _slope(slope) {
    }

    Bell::~Bell() {
    }

    std::string Bell::className() const {
        return "Bell";
    }

    std::string Bell::parameters() const {
        return Op::join(3, " ", _center, _width, _slope);
    }

    void Bell::configure(const std::string& parameters) {
        if (parameters.empty()) return;
        std::vector<std::string> values = Op::split(parameters, " ");
        std::size_t required = 3;
        if (values.size() < required) {
            std::ostringstream ex;
            ex << "[configuration error] term <" << className() << ">"
                    << " requires <" << required << "> parameters";
            throw fl::Exception(ex.str(), FL_AT);
        }
        setCenter(Op::toScalar(values.at(0)));
        setWidth(Op::toScalar(values.at(1)));
        setSlope(Op::toScalar(values.at(2)));
    }

    scalar Bell::membership(scalar x) const {
        if (fl::Op::isNan(x)) return fl::nan;
        //from octave: gbellmf.m
        return 1.0 / (1.0 + std::pow(std::abs((x - _center) / _width), 2 * _slope));
    }

    void Bell::setWidth(scalar a) {
        this->_width = a;
    }

    scalar Bell::getWidth() const {
        return this->_width;
    }

    void Bell::setSlope(scalar b) {
        this->_slope = b;
    }

    scalar Bell::getSlope() const {
        return this->_slope;
    }

    void Bell::setCenter(scalar c) {
        this->_center = c;
    }

    scalar Bell::getCenter() const {
        return this->_center;
    }

    Bell* Bell::copy() const {
        return new Bell(*this);
    }

    Term* Bell::constructor() {
        return new Bell;
    }

}
