/*
 * Triangle.cpp
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#include "Triangle.h"

#include <sstream>

namespace fl {

    Triangle::Triangle(const std::string& name, scalar a, scalar b, scalar c)
            : Term(name, a, c), _b(b) {
    }

    Triangle::~Triangle() {
    }

    scalar Triangle::membership(scalar x) {
        if (x <= _minimum || x >= _maximum)
            return 0.0;
        else if (x == _b)
            return 1.0;
        else if (x < _b)
            return (x - _minimum) / (_b - _minimum);
        else
            return (_maximum - x) / (_maximum - _b);
    }

    void Triangle::setA(scalar a) {
        this->_minimum = a;
    }
    scalar Triangle::getA() const {
        return this->_minimum;
    }
    void Triangle::setB(scalar b) {
        this->_b = b;
    }
    scalar Triangle::getB() const {
        return this->_b;
    }
    void Triangle::setC(scalar c) {
        this->_maximum = c;
    }

    scalar Triangle::getC() const {
        return this->_maximum;
    }

    std::string Triangle::toString() const {
        std::stringstream ss;
        ss << "Triangle (" << _minimum << ", " << _b << ", " << _maximum << ")";
        return ss.str();
    }

} /* namespace fl */
