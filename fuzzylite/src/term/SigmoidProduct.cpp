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
            scalar falling, scalar right)
    : Term(name), _left(left), _rising(rising), _falling(falling), _right(right) { }

    SigmoidProduct::~SigmoidProduct() { }

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
        ss << "SigmoidProduct (" 
                << fl::Op::str(_left) << ", "
                << fl::Op::str(_rising) << ", "
                << fl::Op::str(_falling) << ", " 
                << fl::Op::str(_right) << ")";
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

    void SigmoidProduct::setRight(scalar rightInflection) {
        this->_right = rightInflection;
    }

    scalar SigmoidProduct::getRight() const {
        return this->_right;
    }

    void SigmoidProduct::setFalling(scalar fallingSlope) {
        this->_falling = fallingSlope;
    }

    scalar SigmoidProduct::getFalling() const {
        return this->_falling;
    }



}
