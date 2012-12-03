/*
 * Trapezoid.cpp
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#include "Trapezoid.h"

#include "../engine/Operator.h"

#include <sstream>

namespace fl {

    Trapezoid::Trapezoid(const std::string& name, scalar a, scalar b, scalar c,
            scalar d)
            : Term(name), _a(a), _b(b), _c(c), _d(d) {
    }

    Trapezoid::~Trapezoid() {
    }

    scalar Trapezoid::minimum() const {
        return this->_a;
    }

    scalar Trapezoid::maximum() const {
        return this->_d;
    }

    scalar Trapezoid::membership(scalar x) const{
        scalar minimum = _a;
        scalar maximum = _d;
        if (Op::IsLE(x, minimum) || Op::IsGE(x, maximum))
            return 0.0;
        else if (Op::IsLE(x, _b))
            return (x - minimum) / (_b - minimum);
        else if (Op::IsLE(x, _c))
            return 1.0;
        else if (Op::IsLE(x, maximum))
            return (maximum - x) / (maximum - _c);
        else
            return 0.0;
    }

    std::string Trapezoid::toString() const {
        std::stringstream ss;
        ss << "Trapezoid (" << _a << ", " << _b << ", " << _c << ", "
                << _d << ")";
        return ss.str();
    }

    void Trapezoid::setA(scalar a) {
        this->_a = a;
    }
    scalar Trapezoid::getA() const {
        return this->_a;
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
        this->_d = d;
    }
    scalar Trapezoid::getD() const {
        return this->_d;
    }

} /* namespace fl */
