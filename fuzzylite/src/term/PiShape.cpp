/* 
 * File:   PiShape.cpp
 * Author: jcrada
 * 
 * Created on 22 December 2012, 5:15 PM
 */

#include "fl/term/PiShape.h"
#include "fl/operator/Operator.h"

namespace fl {

    PiShape::PiShape(const std::string& name, scalar a, scalar b, scalar c, scalar d)
    : Term(name), _a(a), _b(b), _c(c), _d(d) {
    }

    std::string PiShape::className() const {
        return "PiShape";
    }

    PiShape* PiShape::copy() const {
        return new PiShape(*this);
    }

    scalar PiShape::membership(scalar x) const {
        //from Octave smf.m
        scalar a_b_ave = (_a + _b) / 2.0;
        scalar b_minus_a = _b - _a;
        scalar c_d_ave = (_c + _d) / 2.0;
        scalar d_minus_c = _d - _c;

        if (Op::isLE(x, _a)) return 0.0;

        else if (Op::isLE(x, a_b_ave))
            return 2.0 * std::pow((x - _a) / b_minus_a, 2);

        else if (Op::isLt(x, _b))
            return 1.0 - 2.0 * std::pow((x - _b) / b_minus_a, 2);

        else if (Op::isLE(x, _c))
            return 1;

        else if (Op::isLE(x, c_d_ave))
            return 1 - 2 * std::pow((x - _c) / d_minus_c, 2);

        else if (Op::isLt(x, _d))
            return 2 * std::pow((x - _d) / d_minus_c, 2);

        return 0.0;

    }

    std::string PiShape::toString() const {
        std::ostringstream ss;
        ss << std::setprecision(FL_DECIMALS) << std::fixed;
        ss << className() << " (" << _a << ", " << _b << ", " << _c << ", " << _d << ")";
        return ss.str();
    }

    void PiShape::setA(scalar a) {
        this->_a = a;
    }

    scalar PiShape::getA() const {
        return this->_a;
    }

    void PiShape::setB(scalar b) {
        this->_b = b;
    }

    scalar PiShape::getB() const {
        return this->_b;
    }

    void PiShape::setC(scalar c) {
        this->_c = c;
    }

    scalar PiShape::getC() const {
        return this->_c;
    }

    void PiShape::setD(scalar d) {
        this->_d = d;
    }

    scalar PiShape::getD() const {
        return this->_d;
    }
}