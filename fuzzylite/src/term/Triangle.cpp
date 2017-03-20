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

#include "fl/term/Triangle.h"

namespace fl {

    Triangle::Triangle(const std::string& name, scalar vertexA, scalar vertexB, scalar vertexC, scalar height)
    : Term(name, height), _vertexA(vertexA), _vertexB(vertexB), _vertexC(vertexC) {
        if (Op::isNaN(vertexC)) {
            this->_vertexC = _vertexB;
            this->_vertexB = 0.5 * (_vertexA + _vertexB);
        }
    }

    Triangle::~Triangle() { }

    std::string Triangle::className() const {
        return "Triangle";
    }

    Complexity Triangle::complexity() const {
        return Complexity().comparison(1 + 5).arithmetic(4);
    }

    scalar Triangle::membership(scalar x) const {
        if (Op::isNaN(x)) return fl::nan;

        if (Op::isLt(x, _vertexA) or Op::isGt(x, _vertexC))
            return Term::_height * 0.0;

        if (Op::isEq(x, _vertexB))
            return Term::_height * 1.0;

        if (Op::isLt(x, _vertexB)) {
            if (_vertexA == -fl::inf)
                return Term::_height * 1.0;
            return Term::_height * (x - _vertexA) / (_vertexB - _vertexA);
        }
        if (_vertexC == fl::inf)
            return Term::_height * 1.0;
        return Term::_height * (_vertexC - x) / (_vertexC - _vertexB);
    }

    std::string Triangle::parameters() const {
        return Op::join(3, " ", _vertexA, _vertexB, _vertexC) +
                (not Op::isEq(getHeight(), 1.0) ? " " + Op::str(getHeight()) : "");
    }

    void Triangle::configure(const std::string& parameters) {
        if (parameters.empty()) return;
        std::vector<std::string> values = Op::split(parameters, " ");
        std::size_t required = 3;
        if (values.size() < required) {
            std::ostringstream ex;
            ex << "[configuration error] term <" << className() << ">"
                    << " requires <" << required << "> parameters";
            throw Exception(ex.str(), FL_AT);
        }
        setVertexA(Op::toScalar(values.at(0)));
        setVertexB(Op::toScalar(values.at(1)));
        setVertexC(Op::toScalar(values.at(2)));
        if (values.size() > required)
            setHeight(Op::toScalar(values.at(required)));
    }

    void Triangle::setVertexA(scalar a) {
        this->_vertexA = a;
    }

    scalar Triangle::getVertexA() const {
        return this->_vertexA;
    }

    void Triangle::setVertexB(scalar b) {
        this->_vertexB = b;
    }

    scalar Triangle::getVertexB() const {
        return this->_vertexB;
    }

    void Triangle::setVertexC(scalar c) {
        this->_vertexC = c;
    }

    scalar Triangle::getVertexC() const {
        return this->_vertexC;
    }

    Triangle* Triangle::clone() const {
        return new Triangle(*this);
    }

    Term* Triangle::constructor() {
        return new Triangle;
    }


}
