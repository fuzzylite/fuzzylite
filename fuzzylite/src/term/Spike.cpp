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

#include "fl/term/Spike.h"

namespace fl {

    Spike::Spike(const std::string& name, scalar center, scalar width, scalar height)
    : Term(name, height), _center(center), _width(width) {
    }

    Spike::~Spike() {

    }

    std::string Spike::className() const {
        return "Spike";
    }

    scalar Spike::membership(scalar x) const {
        if (fl::Op::isNaN(x)) return fl::nan;
        return _height * std::exp(-std::fabs(10.0 / _width * (x - _center)));
    }

    std::string Spike::parameters() const {
        return Op::join(2, " ", _center, _width) +
                (not Op::isEq(_height, 1.0) ? " " + Op::str(_height) : "");
    }

    void Spike::configure(const std::string& parameters) {
        if (parameters.empty()) return;
        std::vector<std::string> values = Op::split(parameters, " ");
        std::size_t required = 2;
        if (values.size() < required) {
            std::ostringstream ex;
            ex << "[configuration error] term <" << className() << ">"
                    << " requires <" << required << "> parameters";
            throw fl::Exception(ex.str(), FL_AT);
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
