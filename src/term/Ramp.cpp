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

#include "fl/term/Ramp.h"

namespace fl {

    Ramp::Ramp(const std::string& name, scalar start, scalar end, scalar height)
    : Term(name, height), _start(start), _end(end) { }

    Ramp::~Ramp() { }

    std::string Ramp::className() const {
        return "Ramp";
    }

    Complexity Ramp::complexity() const {
        return Complexity().comparison(1 + 4).arithmetic(1 + 3);
    }

    scalar Ramp::membership(scalar x) const {
        if (Op::isNaN(x)) return fl::nan;

        if (Op::isEq(_start, _end))
            return Term::_height * 0.0;

        if (Op::isLt(_start, _end)) {
            if (Op::isLE(x, _start))
                return Term::_height * 0.0;
            if (Op::isGE(x, _end))
                return Term::_height * 1.0;
            return Term::_height * (x - _start) / (_end - _start);
        } else {
            if (Op::isGE(x, _start))
                return Term::_height * 0.0;
            if (Op::isLE(x, _end))
                return Term::_height * 1.0;
            return Term::_height * (_start - x) / (_start - _end);
        }
    }

    scalar Ramp::tsukamoto(scalar activationDegree, scalar minimum, scalar maximum) const {
        FL_IUNUSED(minimum);
        FL_IUNUSED(maximum);
        return Op::scale(activationDegree, 0, 1, _start, _end);
    }

    bool Ramp::isMonotonic() const {
        return true;
    }

    std::string Ramp::parameters() const {
        return Op::join(2, " ", _start, _end) +
                (not Op::isEq(getHeight(), 1.0) ? " " + Op::str(getHeight()) : "");
    }

    void Ramp::configure(const std::string& parameters) {
        if (parameters.empty()) return;
        std::vector<std::string> values = Op::split(parameters, " ");
        std::size_t required = 2;
        if (values.size() < required) {
            std::ostringstream ex;
            ex << "[configuration error] term <" << className() << ">"
                    << " requires <" << required << "> parameters";
            throw Exception(ex.str(), FL_AT);
        }
        setStart(Op::toScalar(values.at(0)));
        setEnd(Op::toScalar(values.at(1)));
        if (values.size() > required)
            setHeight(Op::toScalar(values.at(required)));
    }

    void Ramp::setStart(scalar start) {
        this->_start = start;
    }

    scalar Ramp::getStart() const {
        return this->_start;
    }

    void Ramp::setEnd(scalar end) {
        this->_end = end;
    }

    scalar Ramp::getEnd() const {
        return this->_end;
    }

    Ramp::Direction Ramp::direction() const {
        scalar range = this->_end - this->_start;
        if (not Op::isFinite(range) or Op::isEq(range, 0.0)) return Zero;

        if (Op::isGt(range, 0.0)) return Positive;

        return Negative;
    }

    Ramp* Ramp::clone() const {
        return new Ramp(*this);
    }

    Term* Ramp::constructor() {
        return new Ramp;
    }

}
