/*
 * Sigmoid.cpp
 *
 *  Created on: 30/11/2012
 *      Author: jcrada
 */

#include "Sigmoid.h"

#include <cmath>
#include <sstream>

namespace fl {

    Sigmoid::Sigmoid(const std::string& name, scalar a, scalar c,
            scalar minimum, scalar maximum)
            : Term(name), _a(a), _c(c), _minimum(minimum), _maximum(maximum) {
    }

    Sigmoid::~Sigmoid() {
    }

    scalar Sigmoid::membership(scalar x) const {
        return 1.0 / (1.0 + std::exp(-_a * (x - _c)));
    }

    std::string Sigmoid::toString() const {
        std::stringstream ss;
        ss << "Sigmoid (" << _minimum << ", " << _maximum << ", "
                << _a << ", " << _c << ")";
        return ss.str();
    }

    void Sigmoid::setA(scalar a) {
        this->_a = a;
    }
    scalar Sigmoid::getA() const {
        return this->_a;
    }
    void Sigmoid::setC(scalar c) {
        this->_c = c;
    }
    scalar Sigmoid::getC() const {
        return this->_c;
    }

    void Sigmoid::setMinimum(scalar minimum) {
        this->_minimum = minimum;
    }
    scalar Sigmoid::minimum() const {
        return this->_minimum;
    }
    void Sigmoid::setMaximum(scalar maximum) {
        this->_maximum = maximum;
    }
    scalar Sigmoid::maximum() const {
        return this->_maximum;
    }

} /* namespace fl */
