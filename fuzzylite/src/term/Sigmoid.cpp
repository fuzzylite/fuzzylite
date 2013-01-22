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

    Sigmoid::Sigmoid(const std::string& name, scalar inflection, scalar slope)
    : Term(name), _inflection(inflection), _slope(slope){ }

    Sigmoid::~Sigmoid() { }

    std::string Sigmoid::className() const {
        return "Sigmoid";
    }

    Sigmoid* Sigmoid::copy() const {
        return new Sigmoid(*this);
    }

    scalar Sigmoid::membership(scalar x) const {
        if (fl::Op::isNan(x)) return std::numeric_limits<scalar>::quiet_NaN();
        return 1.0 / (1.0 + std::exp(-_slope * (x - _inflection)));
    }

    std::string Sigmoid::toString() const {
        std::ostringstream ss;
        ss << std::setprecision(FL_DECIMALS) << std::fixed;
        ss << className() << " (" << _inflection << ", " << _slope << ")";

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

} 
