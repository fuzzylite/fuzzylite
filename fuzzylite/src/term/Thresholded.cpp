/*
 * Thresholded.cpp
 *
 *  Created on: 30/11/2012
 *      Author: jcrada
 */

#include "Thresholded.h"

#include "../engine/Operator.h"

#include <sstream>

namespace fl {

    Thresholded::Thresholded(const Term* term, scalar threshold, const Operator* activation)
            : Term(""), _term(term), _threshold(threshold), _activation(activation) {
        if (term) this->_name = term->getName();
    }

    Thresholded::~Thresholded() {
    }

    scalar Thresholded::membership(scalar x) const {
        return _activation->compute(x, _threshold);
    }

    scalar Thresholded::minimum() const {
        return this->_term->minimum();
    }

    scalar Thresholded::maximum() const {
        return this->_term->maximum();
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

    void Thresholded::setActivation(const Operator* activation) {
        this->_activation = activation;
    }

    const Operator* Thresholded::getActivation() const {
        return this->_activation;
    }

    std::string Thresholded::toString() const {
        std::stringstream ss;
        ss << "Thresholded (" << _term->toString() << ") to " << _threshold
                << " activated using " << _activation->name();
        return ss.str();
    }

} /* namespace fl */
