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

#include "fl/norm/s/EinsteinSum.h"

namespace fl {

    std::string EinsteinSum::className() const {
        return "EinsteinSum";
    }

    Complexity EinsteinSum::complexity() const {
        return Complexity().arithmetic(4);
    }

    scalar EinsteinSum::compute(scalar a, scalar b) const {
        return (a + b) / (1.0 + a * b);
    }

    EinsteinSum* EinsteinSum::clone() const {
        return new EinsteinSum(*this);
    }

    SNorm* EinsteinSum::constructor() {
        return new EinsteinSum;
    }

}
