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

#include "fl/norm/s/BoundedSum.h"

#include "fl/Operation.h"

namespace fl {

    std::string BoundedSum::className() const {
        return "BoundedSum";
    }

    scalar BoundedSum::compute(scalar a, scalar b) const {
        return Op::min(scalar(1.0), a + b);
    }

    BoundedSum* BoundedSum::clone() const {
        return new BoundedSum(*this);
    }

    SNorm* BoundedSum::constructor() {
        return new BoundedSum;
    }

}
