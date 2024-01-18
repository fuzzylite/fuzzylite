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

#include "fl/term/Term.h"

#include "fl/imex/FllExporter.h"
#include "fl/term/Linear.h"
#include "fl/term/Function.h"

namespace fl {

    Term::Term(const std::string& name, scalar height) : _name(name), _height(height) { }

    Term::~Term() { }

    void Term::setName(const std::string& name) {
        this->_name = name;
    }

    std::string Term::getName() const {
        return this->_name;
    }

    void Term::setHeight(scalar height) {
        this->_height = height;
    }

    scalar Term::getHeight() const {
        return this->_height;
    }

    std::string Term::toString() const {
        return FllExporter().toString(this);
    }

    void Term::updateReference(const Engine* engine) {
        FL_IUNUSED(engine);
        //do nothing
    }

    scalar Term::tsukamoto(scalar activationDegree, scalar minimum, scalar maximum) const {
        FL_IUNUSED(minimum);
        FL_IUNUSED(maximum);
        return membership(activationDegree);
    }

    bool Term::isMonotonic() const {
        return false;
    }


}
