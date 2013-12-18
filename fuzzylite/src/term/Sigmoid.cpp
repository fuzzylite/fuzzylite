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

    scalar Sigmoid::membership(scalar x) const {
        if (fl::Op::isNan(x)) return fl::nan;
        return 1.0 / (1.0 + std::exp(-_slope * (x - _inflection)));
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
