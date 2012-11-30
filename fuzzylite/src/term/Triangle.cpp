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
            : Term(name), _a(a), _b(b), _c(c) {
    }

    Triangle::~Triangle() {
    }

    scalar Triangle::minimum() const {
        return this->_a;
    }

    scalar Triangle::maximum() const {
        return this->_c;
    }

    scalar Triangle::membership(scalar x) {
        scalar minimum = _a;
        scalar maximum = _c;
        if (x <= minimum || x >= maximum)
            return 0.0;
        else if (x == _b)
            return 1.0;
        else if (x < _b)
            return (x - minimum) / (_b - minimum);
        else
            return (maximum - x) / (maximum - _b);
    }

    std::string Triangle::toString() const {
        std::stringstream ss;
        ss << "Triangle (" << _a << ", " << _b << ", " << _c << ")";
        return ss.str();
    }

    void Triangle::setA(scalar a) {
        this->_a = a;
    }
    scalar Triangle::getA() const {
        return this->_a;
    }
    void Triangle::setB(scalar b) {
        this->_b = b;
    }
    scalar Triangle::getB() const {
        return this->_b;
    }
    void Triangle::setC(scalar c) {
        this->_c = c;
    }

    scalar Triangle::getC() const {
        return this->_c;
    }

} /* namespace fl */
