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

#include "fl/norm/s/HamacherSum.h"

#include "fl/Operation.h"

namespace fl {

    std::string HamacherSum::className() const {
        return "HamacherSum";
    }

    Complexity HamacherSum::complexity() const {
        return Complexity().arithmetic(7);
    }

    scalar HamacherSum::compute(scalar a, scalar b) const {
        if (Op::isEq(a * b, 1.0)) return 1.0;
        return (a + b - 2.0 * a * b) / (1.0 - a * b);
    }

    HamacherSum* HamacherSum::clone() const {
        return new HamacherSum(*this);
    }

    SNorm* HamacherSum::constructor() {
        return new HamacherSum;
    }

}
