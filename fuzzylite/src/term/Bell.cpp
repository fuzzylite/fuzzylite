/*
 Author: Juan Rada-Vilela, Ph.D.
 Copyright © 2010-2015 FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#include "fl/term/Bell.h"

namespace fl {

    Bell::Bell(const std::string& name, scalar center, scalar width, scalar slope, scalar height)
    : Term(name, height), _center(center), _width(width), _slope(slope) {
    }

    Bell::~Bell() {
    }

    std::string Bell::className() const {
        return "Bell";
    }

    scalar Bell::membership(scalar x) const {
        if (fl::Op::isNaN(x)) return fl::nan;
        return _height * (1.0 / (1.0 + std::pow(std::abs((x - _center) / _width), 2 * _slope)));
    }

    std::string Bell::parameters() const {
        return Op::join(3, " ", _center, _width, _slope) +
                (not Op::isEq(_height, 1.0) ? " " + Op::str(_height) : "");
    }

    void Bell::configure(const std::string& parameters) {
        if (parameters.empty()) return;
        std::vector<std::string> values = Op::split(parameters, " ");
        std::size_t required = 3;
        if (values.size() < required) {
            std::ostringstream ex;
            ex << "[configuration error] term <" << className() << ">"
                    << " requires <" << required << "> parameters";
            throw fl::Exception(ex.str(), FL_AT);
        }
        setCenter(Op::toScalar(values.at(0)));
        setWidth(Op::toScalar(values.at(1)));
        setSlope(Op::toScalar(values.at(2)));
        if (values.size() > required)
            setHeight(Op::toScalar(values.at(required)));
    }

    void Bell::setWidth(scalar a) {
        this->_width = a;
    }

    scalar Bell::getWidth() const {
        return this->_width;
    }

    void Bell::setSlope(scalar b) {
        this->_slope = b;
    }

    scalar Bell::getSlope() const {
        return this->_slope;
    }

    void Bell::setCenter(scalar c) {
        this->_center = c;
    }

    scalar Bell::getCenter() const {
        return this->_center;
    }

    Bell* Bell::clone() const {
        return new Bell(*this);
    }

    Term* Bell::constructor() {
        return new Bell;
    }

}
