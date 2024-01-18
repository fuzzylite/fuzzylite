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

#include "fl/term/Spike.h"

namespace fl {

    Spike::Spike(const std::string& name, scalar center, scalar width, scalar height)
    : Term(name, height), _center(center), _width(width) { }

    Spike::~Spike() { }

    std::string Spike::className() const {
        return "Spike";
    }

    Complexity Spike::complexity() const {
        return Complexity().comparison(1).arithmetic(1 + 3).function(2);
    }

    scalar Spike::membership(scalar x) const {
        if (Op::isNaN(x)) return fl::nan;
        return Term::_height * std::exp(-std::abs(10.0 / _width * (x - _center)));
    }

    std::string Spike::parameters() const {
        return Op::join(2, " ", _center, _width) +
                (not Op::isEq(getHeight(), 1.0) ? " " + Op::str(getHeight()) : "");
    }

    void Spike::configure(const std::string& parameters) {
        if (parameters.empty()) return;
        std::vector<std::string> values = Op::split(parameters, " ");
        std::size_t required = 2;
        if (values.size() < required) {
            std::ostringstream ex;
            ex << "[configuration error] term <" << className() << ">"
                    << " requires <" << required << "> parameters";
            throw Exception(ex.str(), FL_AT);
        }
        setCenter(Op::toScalar(values.at(0)));
        setWidth(Op::toScalar(values.at(1)));
        if (values.size() > required)
            setHeight(Op::toScalar(values.at(required)));
    }

    void Spike::setCenter(scalar center) {
        this->_center = center;
    }

    scalar Spike::getCenter() const {
        return this->_center;
    }

    void Spike::setWidth(scalar width) {
        this->_width = width;
    }

    scalar Spike::getWidth() const {
        return this->_width;
    }

    Spike* Spike::clone() const {
        return new Spike(*this);
    }

    Term* Spike::constructor() {
        return new Spike;
    }

}
