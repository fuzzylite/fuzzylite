/*
fuzzylite (R), a fuzzy logic control library in C++.

Copyright (C) 2010-2024 FuzzyLite Limited. All rights reserved.
Author: Juan Rada-Vilela, PhD <jcrada@fuzzylite.com>.

This file is part of fuzzylite.

fuzzylite is free software: you can redistribute it and/or modify it under
the terms of the FuzzyLite License included with the software.

You should have received a copy of the FuzzyLite License along with
fuzzylite. If not, see <https://github.com/fuzzylite/fuzzylite/>.

fuzzylite is a registered trademark of FuzzyLite Limited.
*/

#include "fuzzylite/term/Activated.h"

#include "fuzzylite/imex/FllExporter.h"

namespace fuzzylite {

    Activated::Activated(const Term* term, scalar degree, const TNorm* implication) :
        Term("", degree),
        _term(term),
        _implication(implication) {}

    Activated::~Activated() {}

    std::string Activated::className() const {
        return "Activated";
    }

    std::string Activated::getName() const {
        return _term ? _term->getName() : "";
    }

    bool Activated::isMonotonic() const {
        return _term ? _term->isMonotonic() : false;
    }

    scalar Activated::membership(scalar x) const {
        if (Op::isNaN(x))
            return fl::nan;
        if (not _term)
            throw Exception("[activation error] no term available to activate", FL_AT);
        if (not _implication)
            throw Exception(
                "[implication error] implication operator needed "
                "to activate "
                    + getTerm()->toString(),
                FL_AT
            );
        return _implication->compute(_term->membership(x), _height);
    }

    std::string Activated::parameters() const {
        FllExporter exporter;
        std::ostringstream ss;
        ss << Op::str(getDegree()) << " " << exporter.toString(getImplication()) << " " << exporter.toString(getTerm());
        return ss.str();
    }

    void Activated::configure(const std::string& parameters) {
        FL_IUNUSED(parameters);
    }

    std::string Activated::toString() const {
        FllExporter exporter;
        std::ostringstream ss;
        if (getImplication()) {
            ss << exporter.toString(getImplication()) << "(" << Op::str(getDegree()) << "," << getTerm()->getName()
               << ")";
        } else {
            ss << "(" << Op::str(getDegree()) << "*"  //"\u2297: (*)"
               << getTerm()->getName() << ")";
        }
        return ss.str();
    }

    void Activated::setTerm(const Term* term) {
        this->_term = term;
    }

    const Term* Activated::getTerm() const {
        return this->_term;
    }

    void Activated::setDegree(scalar degree) {
        setHeight(degree);
    }

    scalar Activated::getDegree() const {
        return getHeight();
    }

    void Activated::setImplication(const TNorm* implication) {
        this->_implication = implication;
    }

    const TNorm* Activated::getImplication() const {
        return this->_implication;
    }

    Activated* Activated::clone() const {
        return new Activated(*this);
    }

    std::string Activated::fuzzyValue() const {
        std::stringstream ss;
        const scalar y = getDegree();
        ss << (y >= 0 or Op::isNaN(y) ? '+' : '-') << Op::str(std::abs(y)) << '/' << getTerm()->getName();
        return ss.str();
    }

}
