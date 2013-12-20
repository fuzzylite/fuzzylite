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
 * Thresholded.cpp
 *
 *  Created on: 30/11/2012
 *      Author: jcrada
 */

#include "fl/term/Thresholded.h"

#include "fl/norm/TNorm.h"

#include "fl/imex/FllExporter.h"

#include <sstream>

namespace fl {

    Thresholded::Thresholded(const Term* term, scalar threshold, const TNorm* activation)
    : Term(""), _term(term), _threshold(threshold), _activation(activation) {
        if (term) this->_name = term->getName();
    }

    Thresholded::~Thresholded() {
    }

    std::string Thresholded::className() const {
        return "Thresholded";
    }

    std::string Thresholded::parameters() const {
        FllExporter exporter;
        std::ostringstream ss;
        ss << Op::str(_threshold) << " " << exporter.toString(_activation) << " "
                << exporter.toString(_term);
        return ss.str();
    }

    void Thresholded::configure(const std::string& parameters) {
        (void) parameters;
    }

    scalar Thresholded::membership(scalar x) const {
        if (fl::Op::isNan(x)) return fl::nan;
        return _activation->compute(this->_term->membership(x), _threshold);
    }

    void Thresholded::setTerm(const Term* term) {
        this->_term = term;
    }

    const Term* Thresholded::getTerm() const {
        return this->_term;
    }

    void Thresholded::setThreshold(scalar threshold) {
        this->_threshold = threshold;
    }

    scalar Thresholded::getThreshold() const {
        return this->_threshold;
    }

    void Thresholded::setActivation(const TNorm* activation) {
        this->_activation = activation;
    }

    const TNorm* Thresholded::getActivation() const {
        return this->_activation;
    }

    Thresholded* Thresholded::copy() const {
        return new Thresholded(*this);
    }

}
