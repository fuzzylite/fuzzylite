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

#include "fl/term/Activated.h"

#include "fl/imex/FllExporter.h"
#include "fl/norm/TNorm.h"

namespace fl {

    Activated::Activated(const Term* term, scalar degree, const TNorm* implication)
    : Term(""), _term(term), _degree(degree), _implication(implication) {
        if (term) this->_name = term->getName();
    }

    Activated::~Activated() {
    }

    std::string Activated::className() const {
        return "Activated";
    }

    scalar Activated::membership(scalar x) const {
        if (fl::Op::isNaN(x)) return fl::nan;
        if (not _implication) throw fl::Exception("[implication error] "
                "implication operator needed to activate " + _term->toString(), FL_AT);
        return _implication->compute(this->_term->membership(x), _degree);
    }

    std::string Activated::parameters() const {
        FllExporter exporter;
        std::ostringstream ss;
        ss << Op::str(_degree) << " " << exporter.toString(_implication) << " "
                << exporter.toString(_term);
        return ss.str();
    }

    void Activated::configure(const std::string& parameters) {
        (void) parameters;
    }

    std::string Activated::toString() const {
        FllExporter exporter;
        std::ostringstream ss;
        ss << exporter.toString(_implication) << "("
                << Op::str(_degree) << ","
                << _term->getName() << ")";
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
