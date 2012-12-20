/*
 * Bell.cpp
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#include "fl/term/Bell.h"
#include "fl/engine/Operator.h"

#include <cmath>
#include <sstream>

namespace fl {

    Bell::Bell(const std::string& name, scalar center, scalar width, scalar slope,
            scalar minimum, scalar maximum)
    : Term(name), _center(center), _width(width), _slope(slope),
    _minimum(minimum), _maximum(maximum) { }

    Bell::~Bell() { }

    std::string Bell::className() const {
        return "Bell";
    }

    Bell* Bell::copy() const {
        return new Bell(*this);
    }

    scalar Bell::membership(scalar x) const {
        // from matlab: gbellmf.m
        scalar tmp = ((x - _center) / _width) * ((x - _center) / _width);
        if (Op::IsEq(tmp, 0.0) and Op::IsEq(_slope, 0.0))
            return 0.5;
        else if (Op::IsEq(tmp, 0.0) and Op::IsLt(_slope, 0.0))
            return 0.0;
        else {
            tmp = std::pow(tmp, _slope);
            return 1.0 / (1.0 + tmp);
        }
    }

    std::string Bell::toString() const {
        std::stringstream ss;
        ss << "Bell (" << _center << ", " << _width << ", " << _slope << ", "
                << _minimum << ", " << _maximum << ") ";
        return ss.str();
    }

    void Bell::setWidth(scalar a) {
        this->_width = a;
    }

    scalar Bell::getWidth() const {
        return this->_width;
    }

    void Bell::setSlope(scalar b) {
        this->_slope = b;
    }

    scalar Bell::getSlope() const {
        return this->_slope;
    }

    void Bell::setCenter(scalar c) {
        this->_center = c;
    }

    scalar Bell::getCenter() const {
        return this->_center;
    }

    void Bell::setMinimum(scalar minimum) {
        this->_minimum = minimum;
    }

    scalar Bell::minimum() const {
        return this->_minimum;
    }

    void Bell::setMaximum(scalar maximum) {
        this->_maximum = maximum;
    }

    scalar Bell::maximum() const {
        return this->_maximum;
    }


} /* namespace fl */
