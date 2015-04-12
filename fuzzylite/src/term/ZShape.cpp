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

#include "fl/term/ZShape.h"

namespace fl {

    ZShape::ZShape(const std::string& name, scalar start, scalar end, scalar height)
    : Term(name, height), _start(start), _end(end) {
    }

    ZShape::~ZShape() {
    }

    std::string ZShape::className() const {
        return "ZShape";
    }

    scalar ZShape::membership(scalar x) const {
        if (fl::Op::isNaN(x)) return fl::nan;
        //from Octave zmf.m
        scalar average = (_start + _end) / 2;
        scalar difference = _end - _start;

        if (Op::isLE(x, _start)) return _height * 1.0;

        if (Op::isLE(x, average))
            return _height * (1.0 - 2.0 * std::pow((x - _start) / difference, 2));

        if (Op::isLt(x, _end))
            return _height * (2.0 * std::pow((x - _end) / difference, 2));

        return _height * 0.0;
    }

    std::string ZShape::parameters() const {
        return Op::join(2, " ", _start, _end) +
                (not Op::isEq(_height, 1.0) ? " " + Op::str(_height) : "");
    }

    void ZShape::configure(const std::string& parameters) {
        if (parameters.empty()) return;
        std::vector<std::string> values = Op::split(parameters, " ");
        std::size_t required = 2;
        if (values.size() < required) {
            std::ostringstream ex;
            ex << "[configuration error] term <" << className() << ">"
                    << " requires <" << required << "> parameters";
            throw fl::Exception(ex.str(), FL_AT);
        }
        setStart(Op::toScalar(values.at(0)));
        setEnd(Op::toScalar(values.at(1)));
        if (values.size() > required)
            setHeight(Op::toScalar(values.at(required)));
    }

    void ZShape::setStart(scalar start) {
        this->_start = start;
    }

    scalar ZShape::getStart() const {
        return this->_start;
    }

    void ZShape::setEnd(scalar end) {
        this->_end = end;
    }

    scalar ZShape::getEnd() const {
        return this->_end;
    }

    ZShape* ZShape::clone() const {
        return new ZShape(*this);
    }

    Term* ZShape::constructor() {
        return new ZShape;
    }

}
