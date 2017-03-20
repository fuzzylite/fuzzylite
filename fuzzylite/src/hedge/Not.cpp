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

#include "fl/hedge/Not.h"

namespace fl {

    std::string Not::name() const {
        return "not";
    }

    Complexity Not::complexity() const {
        return Complexity().arithmetic(1);
    }

    scalar Not::hedge(scalar x) const {
        return 1.0 - x;
    }

    Not* Not::clone() const {
        return new Not(*this);
    }

    Hedge* Not::constructor() {
        return new Not;
    }

}
