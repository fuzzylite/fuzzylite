/*
 * Cumulative.cpp
 *
 *  Created on: 30/11/2012
 *      Author: jcrada
 */

#include "Cumulative.h"

#include "../engine/Operator.h"

#include <sstream>

namespace fl {

    Cumulative::Cumulative(const std::string& name,
            const Operator* accumulation)
            : Term(name), _accumulation(accumulation) {
    }

    Cumulative::~Cumulative() {
    }

    scalar Cumulative::membership(scalar x) const {
        scalar mu = 0.0;
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            mu = _accumulation->compute(mu, _terms[i]->membership(x));
        }
        return mu;
    }

    std::string Cumulative::toString() const {
        std::stringstream ss;
        ss << "Cumulative (" << _accumulation->name() << ", ";
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            ss << _terms[i]->toString();
            if (i < _terms.size() - 1)
                ss << ", ";
        }
        ss << ")";
        return ss.str();
    }

    void Cumulative::append(const Term* term) {
        if (Op::IsInf(_minimum)  or  term->minimum() < _minimum) {
            _minimum = term->minimum();
        }
        if (Op::IsInf(_maximum)  or  term->maximum() > _maximum) {
            _maximum = term->maximum();
        }
        _terms.push_back(term);
    }

    int Cumulative::size() const {
        return _terms.size();
    }

    void Cumulative::clear() {
        _minimum = -std::numeric_limits<scalar>::infinity();
        _maximum = std::numeric_limits<scalar>::infinity();
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            delete _terms[i];
        }
        _terms.clear();
    }

    scalar Cumulative::minimum() const {
        return this->_minimum;
    }

    scalar Cumulative::maximum() const {
        return this->_maximum;
    }

    void Cumulative::setAccumulation(const Operator* accumulation) {
        this->_accumulation = accumulation;
    }

    const Operator* Cumulative::getAccumulation() const {
        return this->_accumulation;
    }

} /* namespace fl */
