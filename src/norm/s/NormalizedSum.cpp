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

#include "fl/norm/s/NormalizedSum.h"

#include "fl/Operation.h"

namespace fl {

    std::string NormalizedSum::className() const {
        return "NormalizedSum";
    }

    Complexity NormalizedSum::complexity() const {
        return Complexity().arithmetic(3).function(1);
    }

    scalar NormalizedSum::compute(scalar a, scalar b) const {
        return (a + b) / Op::max(scalar(1.0), a + b);
    }

    NormalizedSum* NormalizedSum::clone() const {
        return new NormalizedSum(*this);
    }

    SNorm* NormalizedSum::constructor() {
        return new NormalizedSum;
    }

}
