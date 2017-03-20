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

#include "fl/norm/s/DrasticSum.h"

#include "fl/Operation.h"

namespace fl {

    std::string DrasticSum::className() const {
        return "DrasticSum";
    }

    Complexity DrasticSum::complexity() const {
        return Complexity().comparison(1).function(2);
    }

    scalar DrasticSum::compute(scalar a, scalar b) const {
        if (Op::isEq(Op::min(a, b), 0.0)) {
            return Op::max(a, b);
        }
        return 1.0;
    }

    DrasticSum* DrasticSum::clone() const {
        return new DrasticSum(*this);
    }

    SNorm* DrasticSum::constructor() {
        return new DrasticSum;
    }

}
