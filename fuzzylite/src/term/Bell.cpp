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

    Bell* Bell::copy() const {
        return new Bell(*this);
    }

    scalar Bell::membership(scalar x) const {
        if (fl::Op::isNan(x)) return std::numeric_limits<scalar>::quiet_NaN();
        //from octave: gbellmf.m
        return 1.0 / (1.0 + std::pow(std::abs((x - _center) / _width), 2 * _slope));
    }

    std::string Bell::toString() const {
        std::ostringstream ss;
        ss << std::setprecision(FL_DECIMALS) << std::fixed;
        ss << className() << " (" << _center << ", " << _width << ", " << _slope << ")";
        return ss.str();
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

}
