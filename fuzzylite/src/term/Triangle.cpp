/*
 * Triangle.cpp
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#include "fl/term/Triangle.h"

#include "fl/engine/Operator.h"

#include <sstream>

namespace fl {

    Triangle::Triangle(const std::string& name, scalar a, scalar b, scalar c)
    : Term(name), _a(a), _b(b), _c(c) { }

    Triangle::~Triangle() { }

    std::string Triangle::className() const {
        return "Triangle";
    }

    Triangle* Triangle::copy() const {
        return new Triangle(*this);
    }

    scalar Triangle::membership(scalar x) const {
        scalar minimum = _a;
        scalar maximum = _c;
        if (Op::IsLE(x, minimum) or Op::IsGE(x, maximum))
            return 0.0;
        else if (Op::IsEq(x, _b))
            return 1.0;
        else if (Op::IsLt(x, _b))
            return (x - minimum) / (_b - minimum);
        else
            return (maximum - x) / (maximum - _b);
    }

    std::string Triangle::toString() const {
        std::stringstream ss;
        ss << "Triangle (" << _a << ", " << _b << ", " << _c << ")";
        return ss.str();
    }

    scalar Triangle::minimum() const {
        return this->_a;
    }

    scalar Triangle::maximum() const {
        return this->_c;
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
