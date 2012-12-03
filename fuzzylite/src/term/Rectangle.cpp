/*
 * Rectangle.cpp
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#include "Rectangle.h"

#include "../engine/Operator.h"

#include <sstream>

namespace fl {

    Rectangle::Rectangle(const std::string& name, scalar minimum,
            scalar maximum)
            : Term(name), _minimum(minimum), _maximum(maximum) {
    }

    Rectangle::~Rectangle() {
    }

    scalar Rectangle::membership(scalar x) const{
        if (Op::IsLt(x, _minimum) || Op::IsGt(x, _maximum))
            return 0.0;
        return 1.0;
    }

    std::string Rectangle::toString() const {
        std::stringstream ss;
        ss << "Rectangle (" << _minimum << ", " << _maximum << ")";
        return ss.str();
    }

    void Rectangle::setMinimum(scalar minimum) {
        this->_minimum = minimum;
    }
    scalar Rectangle::minimum() const {
        return this->_minimum;
    }
    void Rectangle::setMaximum(scalar maximum) {
        this->_maximum = maximum;
    }
    scalar Rectangle::maximum() const {
        return this->_maximum;
    }

} /* namespace fl */
