/*
 Copyright (C) 2010-2016 by FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite(R).

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite(R) is a registered trademark of FuzzyLite Limited.
 */

#include "fl/norm/s/HamacherSum.h"

namespace fl {

    std::string HamacherSum::className() const {
        return "HamacherSum";
    }

    Complexity HamacherSum::complexity() const {
        return Complexity().arithmetic(7);
    }

    scalar HamacherSum::compute(scalar a, scalar b) const {
        return (a + b - 2.0 * a * b) / (1.0 - a * b);
    }

    HamacherSum* HamacherSum::clone() const {
        return new HamacherSum(*this);
    }

    SNorm* HamacherSum::constructor() {
        return new HamacherSum;
    }

}
