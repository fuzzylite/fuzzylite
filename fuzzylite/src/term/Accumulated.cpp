/*
 * Accumulated.cpp
 *
 *  Created on: 30/11/2012
 *      Author: jcrada
 */

#include "fl/term/Accumulated.h"

#include "fl/operator/SNorm.h"

#include <sstream>

namespace fl {

    Accumulated::Accumulated(const std::string& name, scalar minimum, scalar maximum,
            const SNorm* accumulation)
    : Term(name), _minimum(minimum), _maximum(maximum), _accumulation(accumulation) { }

    Accumulated::~Accumulated() { }

    std::string Accumulated::className() const {
        return "Accumulated";
    }

    Accumulated* Accumulated::copy() const {
        return new Accumulated(*this);
    }

    scalar Accumulated::membership(scalar x) const {
        scalar mu = 0.0;
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            mu = _accumulation->compute(mu, _terms[i]->membership(x));
        }
        return mu;
    }

    std::string Accumulated::toString() const {
        std::stringstream ss;
        ss << className() << " (";
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            ss << _terms[i]->toString();
            if (i < _terms.size() - 1)
                ss << ", ";
        }
        ss << ") using " << _accumulation->className();
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
        const Term* result = this->_terms[index];
        this->_terms.erase(this->_terms.begin() + index);
        return result;
    }

    void Accumulated::clear() {
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            delete _terms[i];
        }
        _terms.clear();
    }

    const Term* Accumulated::getTerm(int index) const {
        return this->_terms[index];
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
