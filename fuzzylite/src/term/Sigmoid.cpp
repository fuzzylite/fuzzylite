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

#include "fl/term/Sigmoid.h"

namespace fl {

    Sigmoid::Sigmoid(const std::string& name, scalar inflection, scalar slope, scalar height)
    : Term(name, height), _inflection(inflection), _slope(slope) {
    }

    Sigmoid::~Sigmoid() {
    }

    std::string Sigmoid::className() const {
        return "Sigmoid";
    }

    scalar Sigmoid::membership(scalar x) const {
        if (fl::Op::isNaN(x)) return fl::nan;
        return _height * 1.0 / (1.0 + std::exp(-_slope * (x - _inflection)));
    }

    std::string Sigmoid::parameters() const {
        return Op::join(2, " ", _inflection, _slope) +
                (not Op::isEq(_height, 1.0) ? " " + Op::str(_height) : "");
    }

    void Sigmoid::configure(const std::string& parameters) {
        if (parameters.empty()) return;
        std::vector<std::string> values = Op::split(parameters, " ");
        std::size_t required = 2;
        if (values.size() < required) {
            std::ostringstream ex;
            ex << "[configuration error] term <" << className() << ">"
                    << " requires <" << required << "> parameters";
            throw fl::Exception(ex.str(), FL_AT);
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
        if (not fl::Op::isFinite(_slope) or fl::Op::isEq(_slope, 0.0)) return ZERO;

        if (fl::Op::isGt(_slope, 0.0)) return POSITIVE;

        return NEGATIVE;
    }

    Sigmoid* Sigmoid::clone() const {
        return new Sigmoid(*this);
    }

    Term* Sigmoid::constructor() {
        return new Sigmoid;
    }

}
