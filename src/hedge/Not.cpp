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

#include "fuzzylite/hedge/Not.h"

namespace fuzzylite {

    std::string Not::name() const {
        return "not";
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
