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

#include "fl/term/SShape.h"

namespace fl {

    SShape::SShape(const std::string& name, scalar start, scalar end, scalar height)
    : Term(name, height), _start(start), _end(end) { }

    SShape::~SShape() { }

    std::string SShape::className() const {
        return "SShape";
    }

    Complexity SShape::complexity() const {
        return Complexity().comparison(1 + 3).arithmetic(1 + 3 + 4).function(1);
    }

    scalar SShape::membership(scalar x) const {
        if (Op::isNaN(x)) return fl::nan;

        if (Op::isLE(x, _start))
            return Term::_height * 0.0;

        if (Op::isLE(x, 0.5 * (_start + _end)))
            return Term::_height * (2.0 * std::pow((x - _start) / (_end - _start), 2));

        if (Op::isLt(x, _end))
            return Term::_height * (1.0 - 2.0 * std::pow((x - _end) / (_end - _start), 2));

        return Term::_height * 1.0;
    }

    scalar SShape::tsukamoto(scalar activationDegree, scalar minimum, scalar maximum) const {
        FL_IUNUSED(minimum);
        FL_IUNUSED(maximum);

        scalar w = activationDegree;
        scalar z = fl::nan;

        scalar difference = _end - _start;
        scalar a = _start + std::sqrt(0.5 * w * difference * difference);
        scalar b = _end + std::sqrt(-0.5 * (w - 1.0) * difference * difference);
        if (std::abs(w - membership(a)) < std::abs(w - membership(b))) {
            z = a;
        } else {
            z = b;
        }
        return z;
    }

    bool SShape::isMonotonic() const {
        return true;
    }

    std::string SShape::parameters() const {
        return Op::join(2, " ", _start, _end) +
                (not Op::isEq(getHeight(), 1.0) ? " " + Op::str(getHeight()) : "");
    }

    void SShape::configure(const std::string& parameters) {
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
