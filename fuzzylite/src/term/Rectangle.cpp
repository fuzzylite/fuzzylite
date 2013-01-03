/*
 * Rectangle.cpp
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#include "fl/term/Rectangle.h"

#include "fl/operator/Operator.h"

#include <sstream>

namespace fl {

    Rectangle::Rectangle(const std::string& name, scalar minimum,
            scalar maximum)
    : Term(name), _minimum(minimum), _maximum(maximum) { }

    Rectangle::~Rectangle() { }

    std::string Rectangle::className() const {
        return "Rectangle";
    }

    Rectangle* Rectangle::copy() const {
        return new Rectangle(*this);
    }

    scalar Rectangle::membership(scalar x) const {
        if (Op::isLt(x, _minimum) or Op::isGt(x, _maximum))
            return 0.0;
        return 1.0;
    }

    std::string Rectangle::toString() const {
        std::stringstream ss;
        ss << className() << " (" << _minimum << ", " << _maximum << ")";
        return ss.str();
    }

    void Rectangle::setMinimum(scalar minimum) {
        this->_minimum = minimum;
    }

    scalar Rectangle::getMinimum() const {
        return this->_minimum;
    }

    void Rectangle::setMaximum(scalar maximum) {
        this->_maximum = maximum;
    }

    scalar Rectangle::getMaximum() const {
        return this->_maximum;
    }

} 
