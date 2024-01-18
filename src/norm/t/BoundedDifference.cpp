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

#include "fl/norm/t/BoundedDifference.h"

#include "fl/Operation.h"

namespace fl {

    std::string BoundedDifference::className() const {
        return "BoundedDifference";
    }

    Complexity BoundedDifference::complexity() const {
        return Complexity().arithmetic(2).function(1);
    }

    scalar BoundedDifference::compute(scalar a, scalar b) const {
        return Op::max(scalar(0.0), a + b - scalar(1.0));
    }

    BoundedDifference* BoundedDifference::clone() const {
        return new BoundedDifference(*this);
    }

    TNorm* BoundedDifference::constructor() {
        return new BoundedDifference;
    }

}
