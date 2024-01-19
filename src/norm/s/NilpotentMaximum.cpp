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

#include "fuzzylite/norm/s/NilpotentMaximum.h"

#include "fuzzylite/Operation.h"

namespace fuzzylite {

    std::string NilpotentMaximum::className() const {
        return "NilpotentMaximum";
    }

    scalar NilpotentMaximum::compute(scalar a, scalar b) const {
        if (Op::isLt(a + b, 1.0))
            return Op::max(a, b);
        return 1.0;
    }

    NilpotentMaximum* NilpotentMaximum::clone() const {
        return new NilpotentMaximum(*this);
    }

    SNorm* NilpotentMaximum::constructor() {
        return new NilpotentMaximum;
    }

}
