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

#include "fl/term/Activated.h"

#include "fl/imex/FllExporter.h"

namespace fl {

    Activated::Activated(const Term* term, scalar degree, const TNorm* implication)
    : Term(""), _term(term), _degree(degree), _implication(implication) {
        if (term) setName(term->getName());
    }

    Activated::~Activated() { }

    std::string Activated::className() const {
        return "Activated";
    }

    Complexity Activated::complexity() const {
        Complexity result;
        result.comparison(3);
        if (_implication) {
            result += _implication->complexity();
        }
        if (_term) {
            result += _term->complexity();
        }
        return result;
    }

    scalar Activated::membership(scalar x) const {
        if (Op::isNaN(x)) return fl::nan;
        if (not _term)
            throw Exception("[activation error] no term available to activate", FL_AT);
        if (not _implication)
            throw Exception("[implication error] implication operator needed "
                "to activate " + getTerm()->toString(), FL_AT);
        return _implication->compute(_term->membership(x), _degree);
    }

    std::string Activated::parameters() const {
        FllExporter exporter;
        std::ostringstream ss;
        ss << Op::str(getDegree()) << " " << exporter.toString(getImplication()) << " "
                << exporter.toString(getTerm());
        return ss.str();
    }

    void Activated::configure(const std::string& parameters) {
        FL_IUNUSED(parameters);
    }

    std::string Activated::toString() const {
        FllExporter exporter;
        std::ostringstream ss;
        if (getImplication()) {
            ss << exporter.toString(getImplication()) << "("
                    << Op::str(getDegree()) << ","
                    << getTerm()->getName() << ")";
        } else {
            ss << "(" << Op::str(getDegree()) << "*" //"\u2297: (*)"
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
        this->_degree = degree;
    }

    scalar Activated::getDegree() const {
        return this->_degree;
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

}
