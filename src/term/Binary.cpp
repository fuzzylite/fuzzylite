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

#include "fuzzylite/term/Binary.h"

namespace fuzzylite {

    Binary::Binary(const std::string& name, scalar start, scalar direction, scalar height) :
        Term(name, height),
        _start(start),
        _direction(direction) {}

    Binary::~Binary() {}

    std::string Binary::className() const {
        return "Binary";
    }

    scalar Binary::membership(scalar x) const {
        if (Op::isNaN(x))
            return fl::nan;
        if (_direction > _start and Op::isGE(x, _start))
            return Term::_height * 1.0;
        if (_direction < _start and Op::isLE(x, _start))
            return Term::_height * 1.0;
        return Term::_height * 0.0;
    }

    std::string Binary::parameters() const {
        return Op::join(2, " ", getStart(), getDirection())
               + (not Op::isEq(getHeight(), 1.0) ? " " + Op::str(getHeight()) : "");
    }

    void Binary::configure(const std::string& parameters) {
        if (parameters.empty())
            return;
        std::vector<std::string> values = Op::split(parameters, " ");
        std::size_t required = 2;
        if (values.size() < required) {
            std::ostringstream ex;
            ex << "[configuration error] term <" << className() << ">" << " requires <" << required << "> parameters";
            throw Exception(ex.str(), FL_AT);
        }
        setStart(Op::toScalar(values.at(0)));
        setDirection(Op::toScalar(values.at(1)));
        if (values.size() > required)
            setHeight(Op::toScalar(values.at(required)));
    }

    void Binary::setStart(scalar minimum) {
        this->_start = minimum;
    }

    scalar Binary::getStart() const {
        return this->_start;
    }

    void Binary::setDirection(scalar direction) {
        this->_direction = direction;
    }

    scalar Binary::getDirection() const {
        return this->_direction;
    }

    Binary::Direction Binary::direction() const {
        if (this->_direction > _start)
            return Positive;
        if (this->_direction < _start)
            return Negative;
        return Undefined;
    }

    Binary* Binary::clone() const {
        return new Binary(*this);
    }

    Term* Binary::constructor() {
        return new Binary;
    }

}
