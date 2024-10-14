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

#include "fuzzylite/term/SShape.h"

namespace fuzzylite {

    SShape::SShape(const std::string& name, scalar start, scalar end, scalar height) :
        Term(name, height),
        _start(start),
        _end(end) {}

    SShape::~SShape() {}

    std::string SShape::className() const {
        return "SShape";
    }

    scalar SShape::membership(scalar x) const {
        if (Op::isNaN(x))
            return fl::nan;

        if (Op::isLE(x, _start))
            return Term::_height * 0.0;

        if (Op::isLE(x, 0.5 * (_start + _end)))
            return Term::_height * (2.0 * std::pow((x - _start) / (_end - _start), 2));

        if (Op::isLt(x, _end))
            return Term::_height * (1.0 - 2.0 * std::pow((x - _end) / (_end - _start), 2));

        return Term::_height * 1.0;
    }

    scalar SShape::tsukamoto(scalar y) const {
        const scalar h = getHeight();
        const scalar s = getStart();
        const scalar e = getEnd();
        scalar x;
        if (y <= h / 2)
            x = s + (e - s) * std::sqrt(y / (2 * h));
        else
            x = e - (e - s) * std::sqrt((h - y) / (2 * h));
        return x;
    }

    bool SShape::isMonotonic() const {
        return true;
    }

    std::string SShape::parameters() const {
        return Op::join(2, " ", getStart(), getEnd())
               + (not Op::isEq(getHeight(), 1.0) ? " " + Op::str(getHeight()) : "");
    }

    void SShape::configure(const std::string& parameters) {
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
        setStart(Op::toScalar(values.at(0)));
        setEnd(Op::toScalar(values.at(1)));
        if (values.size() > required)
            setHeight(Op::toScalar(values.at(required)));
    }

    void SShape::setStart(scalar start) {
        this->_start = start;
    }

    scalar SShape::getStart() const {
        return this->_start;
    }

    void SShape::setEnd(scalar end) {
        this->_end = end;
    }

    scalar SShape::getEnd() const {
        return this->_end;
    }

    SShape* SShape::clone() const {
        return new SShape(*this);
    }

    Term* SShape::constructor() {
        return new SShape;
    }

}
