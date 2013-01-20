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
            scalar left, scalar rising,
            scalar right, scalar falling)
    : Term(name), _left(left), _rising(rising), _right(right), _falling(falling) { }

    std::string SigmoidProduct::className() const {
        return "SigmoidProduct";
    }

    SigmoidProduct* SigmoidProduct::copy() const {
        return new SigmoidProduct(*this);
    }

    scalar SigmoidProduct::membership(scalar x) const {
        scalar a = 1.0 / (1 + std::exp(-_rising * (x - _left)));
        scalar b = 1.0 / (1 + std::exp(-_falling * (x - _right)));
        return a * b;
    }

    std::string SigmoidProduct::toString() const {
        std::ostringstream ss;
        ss << std::setprecision(FL_DECIMALS) << std::fixed;
        ss << "SigmoidProduct (" << _left << ", " << _rising << ", "
                << _right << ", " << _falling << ")";
        return ss.str();
    }

    void SigmoidProduct::setRising(scalar risingSlope) {
        this->_rising = risingSlope;
    }

    scalar SigmoidProduct::getRising() const {
        return this->_rising;
    }

    void SigmoidProduct::setLeft(scalar leftInflection) {
        this->_left = leftInflection;
    }

    scalar SigmoidProduct::getLeft() const {
        return this->_left;
    }

    void SigmoidProduct::setFalling(scalar fallingSlope) {
        this->_falling = fallingSlope;
    }

    scalar SigmoidProduct::getFalling() const {
        return this->_falling;
    }

    void SigmoidProduct::setRight(scalar rightInflection) {
        this->_right = rightInflection;
    }

    scalar SigmoidProduct::getRight() const {
        return this->_right;
    }

}
