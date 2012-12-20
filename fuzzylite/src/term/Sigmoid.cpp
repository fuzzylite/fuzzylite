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

    Sigmoid::Sigmoid(const std::string& name, scalar inflection, scalar slope,
            scalar minimum, scalar maximum)
    : Term(name), _inflection(inflection), _slope(slope), _minimum(minimum), _maximum(maximum) { }

    Sigmoid::~Sigmoid() { }

    std::string Sigmoid::className() const {
        return "Sigmoid";
    }

    Sigmoid* Sigmoid::copy() const {
        return new Sigmoid(*this);
    }

    scalar Sigmoid::membership(scalar x) const {
        return 1.0 / (1.0 + std::exp(-_slope * (x - _inflection)));
    }

    std::string Sigmoid::toString() const {
        std::stringstream ss;
        ss << "Sigmoid (" << _inflection << ", " << _slope << ", "
                << _minimum << ", " << _maximum << ")";

        return ss.str();
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
