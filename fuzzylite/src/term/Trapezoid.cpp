/*
 * Trapezoid.cpp
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#include "Trapezoid.h"

#include <sstream>

namespace fl {

    Trapezoid::Trapezoid(const std::string& name, scalar a, scalar b, scalar c,
            scalar d)
            : Term(name, a, d), _b(b), _c(c) {
    }

    Trapezoid::~Trapezoid() {
    }

    scalar Trapezoid::membership(scalar x) {
        if (x <= _minimum || x >= _maximum)
            return 0.0;
        else if (x <= _b)
            return (x - _minimum) / (_b - _minimum);
        else if (x <= _c)
            return 1.0;
        else if (x <= _maximum)
            return (_maximum - x) / (_maximum - _c);
        else
            return 0.0;
    }

    std::string Trapezoid::toString() const {
            std::stringstream ss;
            ss << "Trapezoid (" << _minimum << ", " << _b << ", " << _c << ", "
                    << _maximum << ")";
            return ss.str();
    }

    void Trapezoid::setA(scalar a) {
        this->_minimum = a;
    }
    scalar Trapezoid::getA() const {
        return this->_minimum;
    }
    void Trapezoid::setB(scalar b) {
        this->_b = b;
    }
    scalar Trapezoid::getB() const {
        return this->_b;
    }
    void Trapezoid::setC(scalar c) {
        this->_c = c;
    }
    scalar Trapezoid::getC() const {
        return this->_c;
    }
    void Trapezoid::setD(scalar d) {
        this->_maximum = d;
    }
    scalar Trapezoid::getD() const {
        return this->_maximum;
    }


} /* namespace fl */
