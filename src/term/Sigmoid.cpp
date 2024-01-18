/*
 fuzzylite (R), a fuzzy logic control library in C++.
 Copyright (C) 2010-2017 FuzzyLite Limited. All rights reserved.
 Author: Juan Rada-Vilela, Ph.D. <jcrada@fuzzylite.com>

 This file is part of fuzzylite.

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite is a registered trademark of FuzzyLite Limited.
 */

#include "fl/term/Sigmoid.h"

namespace fl {

    Sigmoid::Sigmoid(const std::string& name, scalar inflection, scalar slope, scalar height)
    : Term(name, height), _inflection(inflection), _slope(slope) { }

    Sigmoid::~Sigmoid() { }

    std::string Sigmoid::className() const {
        return "Sigmoid";
    }

    Complexity Sigmoid::complexity() const {
        return Complexity().comparison(1).arithmetic(1 + 4).function(1);
    }

    scalar Sigmoid::membership(scalar x) const {
        if (Op::isNaN(x)) return fl::nan;
        return Term::_height * 1.0 / (1.0 + std::exp(-_slope * (x - _inflection)));
    }

    scalar Sigmoid::tsukamoto(scalar activationDegree,
            scalar minimum, scalar maximum) const {

        scalar w = activationDegree;
        scalar z = fl::nan;

        if (Op::isEq(w, 1.0)) {
            if (Op::isGE(_slope, 0.0)) {
                z = maximum;
            } else {
                z = minimum;
            }

        } else if (Op::isEq(w, 0.0)) {
            if (Op::isGE(_slope, 0.0)) {
                z = minimum;
            } else {
                z = maximum;
            }
        } else {
            scalar a = _slope;
            scalar b = _inflection;
            z = b + (std::log(1.0 / w - 1.0) / -a);
        }

        return z;
    }

    bool Sigmoid::isMonotonic() const {
        return true;
    }

    std::string Sigmoid::parameters() const {
        return Op::join(2, " ", _inflection, _slope) +
                (not Op::isEq(getHeight(), 1.0) ? " " + Op::str(getHeight()) : "");
    }

    void Sigmoid::configure(const std::string& parameters) {
        if (parameters.empty()) return;
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
        if (not Op::isFinite(_slope) or Op::isEq(_slope, 0.0)) return Zero;

        if (Op::isGt(_slope, 0.0)) return Positive;

        return Negative;
    }

    Sigmoid* Sigmoid::clone() const {
        return new Sigmoid(*this);
    }

    Term* Sigmoid::constructor() {
        return new Sigmoid;
    }

}
