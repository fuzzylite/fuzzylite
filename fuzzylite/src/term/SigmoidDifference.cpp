/* 
 * File:   SigmoidDifference.cpp
 * Author: jcrada
 * 
 * Created on 30 December 2012, 5:58 PM
 */

#include "fl/term/SigmoidDifference.h"

#include "fl/operator/Operator.h"

#include <cmath>
#include <sstream>
namespace fl {

    SigmoidDifference::SigmoidDifference(const std::string& name,
            scalar leftInflection, scalar risingSlope,
            scalar rightInflection, scalar fallingSlope)
    : _leftInflection(leftInflection), _risingSlope(risingSlope),
    _rightInflection(rightInflection), _fallingSlope(fallingSlope) { }

    std::string SigmoidDifference::className() const {
        return "SigmoidDifference";
    }

    SigmoidDifference* SigmoidDifference::copy() const {
        return new SigmoidDifference(*this);
    }

    scalar SigmoidDifference::membership(scalar x) const {
        scalar a = 1.0 / (1 + std::exp(-_risingSlope * (x - _leftInflection)));
        scalar b = 1.0 / (1 + std::exp(-_fallingSlope * (x - _rightInflection)));
        return std::abs(a - b);
    }

    std::string SigmoidDifference::toString() const {
        std::ostringstream ss;
        ss << "SigmoidDifference (" << _leftInflection << ", " << _risingSlope << ", "
                << _rightInflection << ", " << _fallingSlope << ")";
        return ss.str();

    }

    void SigmoidDifference::setLeftInflection(scalar leftInflection) {
        this->_leftInflection = leftInflection;
    }

    scalar SigmoidDifference::getLeftInflection() const {
        return this->_leftInflection;
    }

    void SigmoidDifference::setRisingSlope(scalar risingSlope) {
        this->_risingSlope = risingSlope;
    }

    scalar SigmoidDifference::getRisingSlope() const {
        return this->_risingSlope;
    }

    void SigmoidDifference::setRightInflection(scalar rightInflection) {
        this->_rightInflection = rightInflection;
    }

    scalar SigmoidDifference::getRightInflection() const {
        return this->_rightInflection;
    }

    void SigmoidDifference::setFallingSlope(scalar fallingSlope) {
        this->_fallingSlope = fallingSlope;
    }

    scalar SigmoidDifference::getFallingSlope() const {
        return this->_fallingSlope;
    }



}
