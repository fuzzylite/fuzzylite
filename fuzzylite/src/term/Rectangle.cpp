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

#include "fl/term/Rectangle.h"

namespace fl {

    Rectangle::Rectangle(const std::string& name, scalar start, scalar end, scalar height)
    : Term(name, height), _start(start), _end(end) { }

    Rectangle::~Rectangle() { }

    std::string Rectangle::className() const {
        return "Rectangle";
    }

    Complexity Rectangle::complexity() const {
        return Complexity().comparison(1 + 2).arithmetic(1);
    }

    scalar Rectangle::membership(scalar x) const {
        if (Op::isNaN(x)) return fl::nan;
        if (Op::isGE(x, _start) and Op::isLE(x, _end))
            return Term::_height * 1.0;
        return Term::_height * 0.0;
    }

    std::string Rectangle::parameters() const {
        return Op::join(2, " ", _start, _end) +
                (not Op::isEq(getHeight(), 1.0) ? " " + Op::str(getHeight()) : "");
    }

    void Rectangle::configure(const std::string& parameters) {
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

    void Rectangle::setStart(scalar minimum) {
        this->_start = minimum;
    }

    scalar Rectangle::getStart() const {
        return this->_start;
    }

    void Rectangle::setEnd(scalar maximum) {
        this->_end = maximum;
    }

    scalar Rectangle::getEnd() const {
        return this->_end;
    }

    Rectangle* Rectangle::clone() const {
        return new Rectangle(*this);
    }

    Term* Rectangle::constructor() {
        return new Rectangle;
    }


}
