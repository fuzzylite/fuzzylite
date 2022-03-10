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

#include "fuzzylite/norm/s/Maximum.h"

#include "fuzzylite/Operation.h"

namespace fl {

    std::string Maximum::className() const {
        return "Maximum";
    }

    Complexity Maximum::complexity() const {
        return Complexity().function(1);
    }

    scalar Maximum::compute(scalar a, scalar b) const {
        return Op::max(a, b);
    }

    Maximum* Maximum::clone() const {
        return new Maximum(*this);
    }

    SNorm* Maximum::constructor() {
        return new Maximum;
    }

}
