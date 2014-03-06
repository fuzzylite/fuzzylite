// #BEGIN_LICENSE
// fuzzylite: a fuzzy logic control library in C++
// Copyright (C) 2014  Juan Rada-Vilela
// 
// This file is part of fuzzylite.
//
// fuzzylite is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// fuzzylite is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with fuzzylite.  If not, see <http://www.gnu.org/licenses/>.
// #END_LICENSE

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
    : Term(name), _left(left), _rising(rising), _falling(falling), _right(right) {
    }

    SigmoidProduct::~SigmoidProduct() {
    }

    std::string SigmoidProduct::className() const {
        return "SigmoidProduct";
    }

    scalar SigmoidProduct::membership(scalar x) const {
        scalar a = 1.0 / (1 + std::exp(-_rising * (x - _left)));
        scalar b = 1.0 / (1 + std::exp(-_falling * (x - _right)));
        return a * b;
    }

    std::string SigmoidProduct::parameters() const {
        return Op::join(4, " ", _left, _rising, _falling, _right);
    }

    void SigmoidProduct::configure(const std::string& parameters) {
        if (parameters.empty()) return;
        std::vector<std::string> values = Op::split(parameters, " ");
        std::size_t required = 4;
        if (values.size() < required) {
            std::ostringstream ex;
            ex << "[configuration error] term <" << className() << ">"
                    << " requires <" << required << "> parameters";
            throw fl::Exception(ex.str(), FL_AT);
        }
        setLeft(Op::toScalar(values.at(0)));
        setRising(Op::toScalar(values.at(1)));
        setFalling(Op::toScalar(values.at(2)));
        setRight(Op::toScalar(values.at(3)));
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

    SigmoidProduct* SigmoidProduct::copy() const {
        return new SigmoidProduct(*this);
    }

    Term* SigmoidProduct::constructor() {
        return new SigmoidProduct;
    }

}
