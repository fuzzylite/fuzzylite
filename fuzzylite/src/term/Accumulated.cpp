/*
 * Accumulated.cpp
 *
 *  Created on: 30/11/2012
 *      Author: jcrada
 */

#include "fl/term/Accumulated.h"

#include "fl/engine/Operator.h"

#include <sstream>

namespace fl {

    Accumulated::Accumulated(const std::string& name,
            const Operator* accumulation)
            : Term(name), _accumulation(accumulation) {
    }

    Accumulated::~Accumulated() {
    }

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
        ss << "Accumulated (";
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            ss << _terms[i]->toString();
            if (i < _terms.size() - 1)
                ss << ", ";
        }
        ss << ") accumulated using " << _accumulation->name();
        return ss.str();
    }

    scalar Accumulated::minimum() const {
        return this->_minimum;
    }

    scalar Accumulated::maximum() const {
        return this->_maximum;
    }

    void Accumulated::setAccumulation(const Operator* accumulation) {
        this->_accumulation = accumulation;
    }

    const Operator* Accumulated::getAccumulation() const {
        return this->_accumulation;
    }

    /**
     * Operations for std::vector _terms
     */
    void Accumulated::addTerm(const Term* term) {
        if (Op::IsInf(_minimum) or term->minimum() < _minimum) {
            _minimum = term->minimum();
        }
        if (Op::IsInf(_maximum) or term->maximum() > _maximum) {
            _maximum = term->maximum();
        }
        _terms.push_back(term);
    }

    const Term* Accumulated::removeTerm(int index) {
        const Term* result = this->_terms[index];
        this->_terms.erase(this->_terms.begin() + index);

        _minimum = std::numeric_limits<scalar>::infinity();
        _maximum = -std::numeric_limits<scalar>::infinity();
        for (std::size_t i = 0; i < this->_terms.size(); ++i) {
            _minimum = fl::Op::Min(_minimum, _terms[i]->minimum());
            _maximum = fl::Op::Max(_maximum, _terms[i]->maximum());
        }
        return result;
    }

    void Accumulated::clear() {
        _minimum = -std::numeric_limits<scalar>::infinity();
        _maximum = std::numeric_limits<scalar>::infinity();
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

} /* namespace fl */
