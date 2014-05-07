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
 * Accumulated.cpp
 *
 *  Created on: 30/11/2012
 *      Author: jcrada
 */

#include "fl/term/Accumulated.h"

#include <sstream>

#include "fl/imex/FllExporter.h"
#include "fl/norm/SNorm.h"

namespace fl {

    Accumulated::Accumulated(const std::string& name, scalar minimum, scalar maximum,
            SNorm* accumulation)
    : Term(name), _minimum(minimum), _maximum(maximum) {
        _accumulation.reset(accumulation);
    }

    Accumulated::Accumulated(const Accumulated& source) : Term(source) {
        copyFrom(source);
    }

    Accumulated& Accumulated::operator =(const Accumulated& rhs) {
        if (this == &rhs) return *this;
        clear();
        _accumulation.reset(NULL);

        Term::operator =(rhs);
        copyFrom(rhs);
        return *this;
    }

    Accumulated::~Accumulated() {
        this->clear();
    }

    void Accumulated::copyFrom(const Accumulated& source) {
        _minimum = source._minimum;
        _maximum = source._maximum;

        if (source._accumulation.get()) _accumulation.reset(source._accumulation->clone());

        for (std::size_t i = 0; i < source._terms.size(); ++i) {
            _terms.push_back(source._terms.at(i)->clone());
        }
    }

    std::string Accumulated::className() const {
        return "Accumulated";
    }

    scalar Accumulated::membership(scalar x) const {
        if (fl::Op::isNaN(x)) return fl::nan;
        scalar mu = 0.0;
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            mu = _accumulation->compute(mu, _terms.at(i)->membership(x));
        }
        return mu;
    }

    std::string Accumulated::parameters() const {
        FllExporter exporter;
        std::ostringstream ss;
        ss << exporter.toString(_accumulation.get());
        ss << " " << Op::str(_minimum) << " " << Op::str(_maximum) << " ";
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            ss << " " << exporter.toString(_terms.at(i));
        }
        return ss.str();
    }

    void Accumulated::configure(const std::string& parameters) {
        (void) parameters;
    }

    Accumulated* Accumulated::clone() const {
        return new Accumulated(*this);
    }

    std::string Accumulated::toString() const {
        std::vector<std::string> accumulate;
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            accumulate.push_back(_terms.at(i)->toString());
        }
        FllExporter exporter;
        std::ostringstream ss;
        ss << _name << ": " << className() << " "
                << exporter.toString(_accumulation.get()) << "["
                << fl::Op::join(accumulate, ",") << "]";
        return ss.str();
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

    void Accumulated::setAccumulation(SNorm* accumulation) {
        this->_accumulation.reset(accumulation);
    }

    SNorm* Accumulated::getAccumulation() const {
        return this->_accumulation.get();
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

    const std::vector<const Term*>& Accumulated::constTerms() const {
        return this->_terms;
    }

    std::vector<const Term*>& Accumulated::terms() {
        return this->_terms;
    }

    int Accumulated::numberOfTerms() const {
        return _terms.size();
    }

    bool Accumulated::isEmpty() const {
        return _terms.empty();
    }

}
