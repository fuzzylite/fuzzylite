/*   Copyright 2013 Juan Rada-Vilela

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

/* 
 * File:   SigmoidDifference.cpp
 * Author: jcrada
 * 
 * Created on 30 December 2012, 5:58 PM
 */

#include "fl/term/SigmoidDifference.h"

#include <cmath>
#include <sstream>
namespace fl {

    SigmoidDifference::SigmoidDifference(const std::string& name,
            scalar left, scalar rising,
            scalar falling, scalar right)
    : Term(name), _left(left), _rising(rising), _falling(falling), _right(right) { }

    SigmoidDifference::~SigmoidDifference() { }

    std::string SigmoidDifference::className() const {
        return "SigmoidDifference";
    }

    SigmoidDifference* SigmoidDifference::copy() const {
        return new SigmoidDifference(*this);
    }

    scalar SigmoidDifference::membership(scalar x) const {
        if (fl::Op::isNan(x)) return fl::nan;

        scalar a = 1.0 / (1 + std::exp(-_rising * (x - _left)));
        scalar b = 1.0 / (1 + std::exp(-_falling * (x - _right)));
        return std::abs(a - b);
    }

    std::string SigmoidDifference::toString() const {
        std::ostringstream ss;
        ss << className() << " (" 
                << fl::Op::str(_left) << ", "
                << fl::Op::str(_rising) << ", "
                << fl::Op::str(_falling) << ", " 
                << fl::Op::str(_right) << ")";
        return ss.str();

    }

    void SigmoidDifference::setLeft(scalar leftInflection) {
        this->_left = leftInflection;
    }

    scalar SigmoidDifference::getLeft() const {
        return this->_left;
    }

    void SigmoidDifference::setRising(scalar risingSlope) {
        this->_rising = risingSlope;
    }

    scalar SigmoidDifference::getRising() const {
        return this->_rising;
    }

    void SigmoidDifference::setFalling(scalar fallingSlope) {
        this->_falling = fallingSlope;
    }

    scalar SigmoidDifference::getFalling() const {
        return this->_falling;
    }

    void SigmoidDifference::setRight(scalar rightInflection) {
        this->_right = rightInflection;
    }

    scalar SigmoidDifference::getRight() const {
        return this->_right;
    }





}
