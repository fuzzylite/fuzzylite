/*
 Copyright © 2010-2016 by FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#include "fl/term/PiShape.h"

namespace fl {

    PiShape::PiShape(const std::string& name, scalar bottomLeft, scalar topLeft,
            scalar topRight, scalar bottomRight, scalar height)
    : Term(name, height), _bottomLeft(bottomLeft), _topLeft(topLeft),
    _topRight(topRight), _bottomRight(bottomRight) {
    }

    PiShape::~PiShape() {
    }

    std::string PiShape::className() const {
        return "PiShape";
    }

    scalar PiShape::membership(scalar x) const {
        if (Op::isNaN(x)) return fl::nan;
        //from Octave smf.m
        scalar a_b_ave = 0.5 * (_bottomLeft + _topLeft);
        scalar b_minus_a = _topLeft - _bottomLeft;
        scalar c_d_ave = 0.5 * (_topRight + _bottomRight);
        scalar d_minus_c = _bottomRight - _topRight;

        if (Op::isLE(x, _bottomLeft))
            return Term::_height * 0.0;

        if (Op::isLE(x, a_b_ave))
            return Term::_height * (2.0 * std::pow((x - _bottomLeft) / b_minus_a, 2));

        if (Op::isLt(x, _topLeft))
            return Term::_height * (1.0 - 2.0 * std::pow((x - _topLeft) / b_minus_a, 2));

        if (Op::isLE(x, _topRight))
            return Term::_height * 1.0;

        if (Op::isLE(x, c_d_ave))
            return Term::_height * (1.0 - 2.0 * std::pow((x - _topRight) / d_minus_c, 2));

        if (Op::isLt(x, _bottomRight))
            return Term::_height * (2.0 * std::pow((x - _bottomRight) / d_minus_c, 2));

        return Term::_height * 0.0;
    }

    std::string PiShape::parameters() const {
        return Op::join(4, " ", _bottomLeft, _topLeft, _topRight, _bottomRight) +
                (not Op::isEq(getHeight(), 1.0) ? " " + Op::str(getHeight()) : "");
    }

    void PiShape::configure(const std::string& parameters) {
        if (parameters.empty()) return;
        std::vector<std::string> values = Op::split(parameters, " ");
        std::size_t required = 4;
        if (values.size() < required) {
            std::ostringstream ex;
            ex << "[configuration error] term <" << className() << ">"
                    << " requires <" << required << "> parameters";
            throw Exception(ex.str(), FL_AT);
        }
        setBottomLeft(Op::toScalar(values.at(0)));
        setTopLeft(Op::toScalar(values.at(1)));
        setTopRight(Op::toScalar(values.at(2)));
        setBottomRight(Op::toScalar(values.at(3)));
        if (values.size() > required)
            setHeight(Op::toScalar(values.at(required)));
    }

    void PiShape::setBottomLeft(scalar a) {
        this->_bottomLeft = a;
    }

    scalar PiShape::getBottomLeft() const {
        return this->_bottomLeft;
    }

    void PiShape::setTopLeft(scalar b) {
        this->_topLeft = b;
    }

    scalar PiShape::getTopLeft() const {
        return this->_topLeft;
    }

    void PiShape::setTopRight(scalar d) {
        this->_topRight = d;
    }

    scalar PiShape::getTopRight() const {
        return this->_topRight;
    }

    void PiShape::setBottomRight(scalar c) {
        this->_bottomRight = c;
    }

    scalar PiShape::getBottomRight() const {
        return this->_bottomRight;
    }

    PiShape* PiShape::clone() const {
        return new PiShape(*this);
    }

    Term* PiShape::constructor() {
        return new PiShape;
    }

}
