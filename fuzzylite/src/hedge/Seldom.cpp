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

#include "fl/hedge/Seldom.h"

#include "fl/Operation.h"

namespace fl {

    std::string Seldom::name() const {
        return "seldom";
    }

    Complexity Seldom::complexity() const {
        return Complexity().comparison(1).function(1).arithmetic(3);
    }

    scalar Seldom::hedge(scalar x) const {
        return Op::isLE(x, 0.5)
                ? std::sqrt(0.5 * x)
                : (1.0 - std::sqrt(0.5 * (1.0 - x)));
    }

    Seldom* Seldom::clone() const {
        return new Seldom(*this);
    }

    Hedge* Seldom::constructor() {
        return new Seldom;
    }

}
