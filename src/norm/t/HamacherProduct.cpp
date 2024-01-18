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

#include "fl/norm/t/HamacherProduct.h"

#include "fl/Operation.h"

namespace fl {

    std::string HamacherProduct::className() const {
        return "HamacherProduct";
    }

    Complexity HamacherProduct::complexity() const {
        return Complexity().arithmetic(5);
    }

    scalar HamacherProduct::compute(scalar a, scalar b) const {
        if (Op::isEq(a + b, 0.0)) return 0.0;
        return (a * b) / (a + b - a * b);
    }

    HamacherProduct* HamacherProduct::clone() const {
        return new HamacherProduct(*this);
    }

    TNorm* HamacherProduct::constructor() {
        return new HamacherProduct;
    }

}
