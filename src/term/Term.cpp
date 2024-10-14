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

#include "fuzzylite/term/Term.h"

#include "fuzzylite/imex/FllExporter.h"
#include "fuzzylite/term/Function.h"
#include "fuzzylite/term/Linear.h"

namespace fuzzylite {

    Term::Term(const std::string& name, scalar height) : _name(name), _height(height) {}

    Term::~Term() {}

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
        // do nothing
    }

    scalar Term::tsukamoto(scalar y) const {
        FL_IUNUSED(y);
        throw fl::Exception("[tsukamoto error] the term does not support Tsukamoto: " + toString());
    }

    bool Term::isMonotonic() const {
        return false;
    }

}
