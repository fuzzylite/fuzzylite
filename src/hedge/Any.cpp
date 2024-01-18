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

#include "fl/hedge/Any.h"

namespace fl {

    Any::Any() { }

    Any::~Any() { }

    std::string Any::name() const {
        return "any";
    }

    Complexity Any::complexity() const {
        return Complexity();
    }

    scalar Any::hedge(scalar x) const {
        FL_IUNUSED(x);
        return 1.0;
    }

    Any* Any::clone() const {
        return new Any(*this);
    }

    Hedge* Any::constructor() {
        return new Any;
    }

}

