/* 
 * File:   SigmoidProduct.cpp
 * Author: jcrada
 * 
 * Created on 30 December 2012, 5:41 PM
 */

#include "fl/term/SigmoidProduct.h"

#include <cmath>
#include <sstream>
namespace fl {

    SigmoidProduct::SigmoidProduct(const std::string& name,
            scalar leftInflection, scalar risingSlope,
            scalar rightInflection, scalar fallingSlope)
    : Term(name), _leftInflection(leftInflection), _risingSlope(risingSlope),
    _rightInflection(rightInflection), _fallingSlope(fallingSlope) { }

    std::string SigmoidProduct::className() const {
        return "SigmoidProduct";
    }

    SigmoidProduct* SigmoidProduct::copy() const {
        return new SigmoidProduct(*this);
    }

    scalar SigmoidProduct::membership(scalar x) const {
        scalar a = 1.0 / (1 + std::exp(-_risingSlope * (x - _leftInflection)));
        scalar b = 1.0 / (1 + std::exp(-_fallingSlope * (x - _rightInflection)));
        return a * b;
    }

    std::string SigmoidProduct::toString() const {
        std::ostringstream ss;
        ss << std::setprecision(FL_DECIMALS) << std::fixed;
        ss << "SigmoidProduct (" << _leftInflection << ", " << _risingSlope << ", "
                << _rightInflection << ", " << _fallingSlope << ")";
        return ss.str();
    }

    void SigmoidProduct::setRisingSlope(scalar risingSlope) {
        this->_risingSlope = risingSlope;
    }

    scalar SigmoidProduct::getRisingSlope() const {
        return this->_risingSlope;
    }

    void SigmoidProduct::setLeftInflection(scalar leftInflection) {
        this->_leftInflection = leftInflection;
    }

    scalar SigmoidProduct::getLeftInflection() const {
        return this->_leftInflection;
    }

    void SigmoidProduct::setFallingSlope(scalar fallingSlope) {
        this->_fallingSlope = fallingSlope;
    }

    scalar SigmoidProduct::getFallingSlope() const {
        return this->_fallingSlope;
    }

    void SigmoidProduct::setRightInflection(scalar rightInflection) {
        this->_rightInflection = rightInflection;
    }

    scalar SigmoidProduct::getRightInflection() const {
        return this->_rightInflection;
    }

}
