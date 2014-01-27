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
 * Accumulated.cpp
 *
 *  Created on: 30/11/2012
 *      Author: jcrada
 */

#include "fl/term/Accumulated.h"

#include "fl/norm/SNorm.h"

#include "fl/imex/FllExporter.h"

#include <sstream>

namespace fl {

    Accumulated::Accumulated(const std::string& name, scalar minimum, scalar maximum,
            const SNorm* accumulation)
    : Term(name), _minimum(minimum), _maximum(maximum), _accumulation(accumulation) {
    }

    Accumulated::~Accumulated() {
    }

    std::string Accumulated::className() const {
        return "Accumulated";
    }

    std::string Accumulated::parameters() const {
        FllExporter exporter;
        std::ostringstream ss;
        ss << Op::str(_minimum) << " " << Op::str(_maximum) << " ";
        ss << exporter.toString(_accumulation);
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            ss << " " << exporter.toString(_terms.at(i));
        }
        return ss.str();
    }

    void Accumulated::configure(const std::string& parameters) {
        (void) parameters;
    }

    Accumulated* Accumulated::copy() const {
        return new Accumulated(*this);
    }

    scalar Accumulated::membership(scalar x) const {
        if (fl::Op::isNan(x)) return fl::nan;
        scalar mu = 0.0;
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            mu = _accumulation->compute(mu, _terms.at(i)->membership(x));
        }
        return mu;
    }

    void Accumulated::setMinimum(scalar minimum) {
        this->_minimum = minimum;
    }

    scalar Accumulated::getMinimum() const {
        return this->_minimum;
    }

    void Accumulated::setMaximum(scalar maximum) {
        this->_maximum = maximum;
    }

    scalar Accumulated::getMaximum() const {
        return this->_maximum;
    }

    void Accumulated::setAccumulation(const SNorm* accumulation) {
        if (this->_accumulation) delete this->_accumulation;
        this->_accumulation = accumulation;
    }

    const SNorm* Accumulated::getAccumulation() const {
        return this->_accumulation;
    }

    /**
     * Operations for std::vector _terms
     */
    void Accumulated::addTerm(const Term* term) {
        this->_terms.push_back(term);
    }

    const Term* Accumulated::removeTerm(int index) {
        const Term* result = this->_terms.at(index);
        this->_terms.erase(this->_terms.begin() + index);
        return result;
    }

    void Accumulated::clear() {
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            delete _terms.at(i);
        }
        _terms.clear();
    }

    const Term* Accumulated::getTerm(int index) const {
        return this->_terms.at(index);
    }

    const std::vector<const Term*>& Accumulated::terms() const {
        return this->_terms;
    }

    int Accumulated::numberOfTerms() const {
        return _terms.size();
    }

    bool Accumulated::isEmpty() const {
        return _terms.size() == 0;
    }

}
