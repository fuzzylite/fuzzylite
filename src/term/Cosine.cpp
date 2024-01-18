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

#include "fl/term/Cosine.h"

namespace fl {

    Cosine::Cosine(const std::string& name, scalar center, scalar width, scalar height)
    : Term(name, height), _center(center), _width(width) { }

    Cosine::~Cosine() { }

    std::string Cosine::className() const {
        return "Cosine";
    }

    std::string Cosine::parameters() const {
        return Op::join(2, " ", _center, _width) +
                (not Op::isEq(getHeight(), 1.0) ? " " + Op::str(getHeight()) : "");
    }

    void Cosine::configure(const std::string& parameters) {
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

    Complexity Cosine::complexity() const {
        return Complexity().comparison(3).arithmetic(4 + 1 + 7).function(2);
    }

    scalar Cosine::membership(scalar x) const {
        if (Op::isNaN(x)) return fl::nan;
        if (Op::isLt(x, _center - 0.5 * _width)
                or Op::isGt(x, _center + 0.5 * _width))
            return Term::_height * 0.0;
        const scalar pi = 4.0 * std::atan(1.0);
        return Term::_height * (0.5 * (1.0 + std::cos(2.0 / _width * pi * (x - _center))));
    }

    void Cosine::setCenter(scalar center) {
        this->_center = center;
    }

    scalar Cosine::getCenter() const {
        return this->_center;
    }

    void Cosine::setWidth(scalar width) {
        this->_width = width;
    }

    scalar Cosine::getWidth() const {
        return this->_width;
    }

    Cosine* Cosine::clone() const {
        return new Cosine(*this);
    }

    Term* Cosine::constructor() {
        return new Cosine;
    }

}
