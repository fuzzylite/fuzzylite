/*
 Copyright © 2010-2016 by FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#include "fl/norm/s/UnboundedSum.h"

#include "fl/Operation.h"

namespace fl {

    std::string UnboundedSum::className() const {
        return "UnboundedSum";
    }

    scalar UnboundedSum::compute(scalar a, scalar b) const {
        return a + b;
    }

    UnboundedSum* UnboundedSum::clone() const {
        return new UnboundedSum(*this);
    }

    SNorm* UnboundedSum::constructor() {
        return new UnboundedSum;
    }

}
