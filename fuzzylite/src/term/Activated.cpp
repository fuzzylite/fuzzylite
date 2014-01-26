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
 * Activated.cpp
 *
 *  Created on: 27/01/2014
 *      Author: jcrada
 */

#include "fl/term/Activated.h"

#include "fl/norm/TNorm.h"

#include "fl/imex/FllExporter.h"

#include <sstream>

namespace fl {

    Activated::Activated(const Term* term, scalar degree, const TNorm* activation)
    : Term(""), _term(term), _degree(degree), _activation(activation) {
        if (term) this->_name = term->getName();
    }

    Activated::~Activated() {
    }

    std::string Activated::className() const {
        return "Activated";
    }

    std::string Activated::parameters() const {
        FllExporter exporter;
        std::ostringstream ss;
        ss << Op::str(_degree) << " " << exporter.toString(_activation) << " "
                << exporter.toString(_term);
        return ss.str();
    }

    void Activated::configure(const std::string& parameters) {
        (void) parameters;
    }

    scalar Activated::membership(scalar x) const {
        if (fl::Op::isNaN(x)) return fl::nan;
        return _activation->compute(this->_term->membership(x), _degree);
    }

    std::string Activated::toString() const {
        FllExporter exporter;
        std::ostringstream ss;
        ss << exporter.toString(_activation) << "("
                << Op::str(_degree) << ","
                << _term->getName() << ")";
        return ss.str();
    }

    void Activated::setTerm(const Term* term) {
        this->_term = term;
    }

    const Term* Activated::getTerm() const {
        return this->_term;
    }

    void Activated::setDegree(scalar degree) {
        this->_degree = degree;
    }

    scalar Activated::getDegree() const {
        return this->_degree;
    }

    void Activated::setActivation(const TNorm* activation) {
        this->_activation = activation;
    }

    const TNorm* Activated::getActivation() const {
        return this->_activation;
    }

    Activated* Activated::copy() const {
        return new Activated(*this);
    }

}
