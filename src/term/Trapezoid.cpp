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

#include "fl/term/Trapezoid.h"

namespace fl {

    Trapezoid::Trapezoid(const std::string& name,
            scalar vertexA, scalar vertexB, scalar vertexC, scalar vertexD, scalar height)
    : Term(name, height), _vertexA(vertexA), _vertexB(vertexB), _vertexC(vertexC), _vertexD(vertexD) {
        if (Op::isNaN(vertexC) and Op::isNaN(vertexD)) {
            this->_vertexD = _vertexB;
            scalar range = _vertexD - _vertexA;
            this->_vertexB = _vertexA + range * 1.0 / 5.0;
            this->_vertexC = _vertexA + range * 4.0 / 5.0;
        }
    }

    Trapezoid::~Trapezoid() { }

    std::string Trapezoid::className() const {
        return "Trapezoid";
    }

    Complexity Trapezoid::complexity() const {
        return Complexity().comparison(1 + 6).arithmetic(1 + 3).function(1);
    }

    scalar Trapezoid::membership(scalar x) const {
        if (Op::isNaN(x)) return fl::nan;

        if (Op::isLt(x, _vertexA) or Op::isGt(x, _vertexD))
            return Term::_height * 0.0;

        if (Op::isLt(x, _vertexB)) {
            if (_vertexA == -fl::inf) return Term::_height * 1.0;
            return Term::_height * Op::min(scalar(1.0), (x - _vertexA) / (_vertexB - _vertexA));
        }
        if (Op::isLE(x, _vertexC))
            return Term::_height * 1.0;

        if (Op::isLt(x, _vertexD)) {
            if (_vertexD == fl::inf) return Term::_height * 1.0;
            return Term::_height * (_vertexD - x) / (_vertexD - _vertexC);
        }

        if (_vertexD == fl::inf) return Term::_height * 1.0;
        return Term::_height * 0.0;
    }

    std::string Trapezoid::parameters() const {
        return Op::join(4, " ", _vertexA, _vertexB, _vertexC, _vertexD)+
                (not Op::isEq(getHeight(), 1.0) ? " " + Op::str(getHeight()) : "");
    }

    void Trapezoid::configure(const std::string& parameters) {
        if (parameters.empty()) return;
        std::vector<std::string> values = Op::split(parameters, " ");
        std::size_t required = 4;
        if (values.size() < required) {
            std::ostringstream ex;
            ex << "[configuration error] term <" << className() << ">"
                    << " requires <" << required << "> parameters";
            throw Exception(ex.str(), FL_AT);
        }
        setVertexA(Op::toScalar(values.at(0)));
        setVertexB(Op::toScalar(values.at(1)));
        setVertexC(Op::toScalar(values.at(2)));
        setVertexD(Op::toScalar(values.at(3)));
        if (values.size() > required)
            setHeight(Op::toScalar(values.at(required)));
    }

    void Trapezoid::setVertexA(scalar a) {
        this->_vertexA = a;
    }

    scalar Trapezoid::getVertexA() const {
        return this->_vertexA;
    }

    void Trapezoid::setVertexB(scalar b) {
        this->_vertexB = b;
    }

    scalar Trapezoid::getVertexB() const {
        return this->_vertexB;
    }

    void Trapezoid::setVertexC(scalar c) {
        this->_vertexC = c;
    }

    scalar Trapezoid::getVertexC() const {
        return this->_vertexC;
    }

    void Trapezoid::setVertexD(scalar d) {
        this->_vertexD = d;
    }

    scalar Trapezoid::getVertexD() const {
        return this->_vertexD;
    }

    Trapezoid* Trapezoid::clone() const {
        return new Trapezoid(*this);
    }

    Term* Trapezoid::constructor() {
        return new Trapezoid;
    }


}
