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

#include "fuzzylite/term/Sigmoid.h"

namespace fuzzylite {

    Sigmoid::Sigmoid(const std::string& name, scalar inflection, scalar slope, scalar height) :
        Term(name, height),
        _inflection(inflection),
        _slope(slope) {}

    Sigmoid::~Sigmoid() {}

    std::string Sigmoid::className() const {
        return "Sigmoid";
    }

    scalar Sigmoid::membership(scalar x) const {
        if (Op::isNaN(x))
            return fl::nan;
        return Term::_height * 1.0 / (1.0 + std::exp(-_slope * (x - _inflection)));
    }

    scalar Sigmoid::tsukamoto(scalar y) const {
        const scalar h = getHeight();
        const scalar i = getInflection();
        const scalar s = getSlope();
        const scalar x = i + std::log(h / y - 1.0) / -s;
        return x;
    }

    bool Sigmoid::isMonotonic() const {
        return true;
    }

    std::string Sigmoid::parameters() const {
        return Op::join(2, " ", getInflection(), getSlope())
               + (not Op::isEq(getHeight(), 1.0) ? " " + Op::str(getHeight()) : "");
    }

    void Sigmoid::configure(const std::string& parameters) {
        if (parameters.empty())
            return;
        std::vector<std::string> values = Op::split(parameters, " ");
        std::size_t required = 2;
        if (values.size() < required) {
            std::ostringstream ex;
            ex << "[configuration error] term <" << className() << ">"
               << " requires <" << required << "> parameters";
            throw Exception(ex.str(), FL_AT);
        }
        setInflection(Op::toScalar(values.at(0)));
        setSlope(Op::toScalar(values.at(1)));
        if (values.size() > required)
            setHeight(Op::toScalar(values.at(required)));
    }

    void Sigmoid::setSlope(scalar a) {
        this->_slope = a;
    }

    scalar Sigmoid::getSlope() const {
        return this->_slope;
    }

    void Sigmoid::setInflection(scalar c) {
        this->_inflection = c;
    }

    scalar Sigmoid::getInflection() const {
        return this->_inflection;
    }

    Sigmoid::Direction Sigmoid::direction() const {
        if (not Op::isFinite(_slope) or Op::isEq(_slope, 0.0))
            return Zero;

        if (Op::isGt(_slope, 0.0))
            return Positive;

        return Negative;
    }

    Sigmoid* Sigmoid::clone() const {
        return new Sigmoid(*this);
    }

    Term* Sigmoid::constructor() {
        return new Sigmoid;
    }

}
