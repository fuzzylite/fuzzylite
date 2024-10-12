/*
fuzzylite (R), a fuzzy logic control library in C++.

Copyright (C) 2010-2024 FuzzyLite Limited. All rights reserved.
Author: Juan Rada-Vilela, PhD <jcrada@fuzzylite.com>.

This file is part of fuzzylite.

fuzzylite is free software: you can redistribute it and/or modify it under
the terms of the FuzzyLite License included with the software.

You should have received a copy of the FuzzyLite License along with
fuzzylite. If not, see <https://github.com/fuzzylite/fuzzylite/>.

fuzzylite is a registered trademark of FuzzyLite Limited.
*/

#include "fuzzylite/term/SigmoidProduct.h"

namespace fuzzylite {

    SigmoidProduct::SigmoidProduct(
        const std::string& name, scalar left, scalar rising, scalar falling, scalar right, scalar height
    ) :
        Term(name, height),
        _left(left),
        _rising(rising),
        _falling(falling),
        _right(right) {}

    SigmoidProduct::~SigmoidProduct() {}

    std::string SigmoidProduct::className() const {
        return "SigmoidProduct";
    }

    scalar SigmoidProduct::membership(scalar x) const {
        if (Op::isNaN(x))
            return fl::nan;
        const scalar a = 1.0 + std::exp(-_rising * (x - _left));
        const scalar b = 1.0 + std::exp(-_falling * (x - _right));
        return Term::_height * 1.0 / (a * b);
    }

    std::string SigmoidProduct::parameters() const {
        return Op::join(4, " ", getLeft(), getRising(), getFalling(), getRight())
               + (not Op::isEq(getHeight(), 1.0) ? " " + Op::str(getHeight()) : "");
    }

    void SigmoidProduct::configure(const std::string& parameters) {
        if (parameters.empty())
            return;
        std::vector<std::string> values = Op::split(parameters, " ");
        std::size_t required = 4;
        if (values.size() < required) {
            std::ostringstream ex;
            ex << "[configuration error] term <" << className() << ">" << " requires <" << required << "> parameters";
            throw Exception(ex.str(), FL_AT);
        }
        setLeft(Op::toScalar(values.at(0)));
        setRising(Op::toScalar(values.at(1)));
        setFalling(Op::toScalar(values.at(2)));
        setRight(Op::toScalar(values.at(3)));
        if (values.size() > required)
            setHeight(Op::toScalar(values.at(required)));
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

    SigmoidProduct* SigmoidProduct::clone() const {
        return new SigmoidProduct(*this);
    }

    Term* SigmoidProduct::constructor() {
        return new SigmoidProduct;
    }

}
