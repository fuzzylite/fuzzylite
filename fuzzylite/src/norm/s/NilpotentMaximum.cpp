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

#include "fl/norm/s/NilpotentMaximum.h"

#include "fl/Operation.h"

namespace fl {

    std::string NilpotentMaximum::className() const {
        return "NilpotentMaximum";
    }

    scalar NilpotentMaximum::compute(scalar a, scalar b) const {
        if (Op::isLt(a + b, 1.0)) {
            return Op::max(a, b);
        }
        return 1.0;
    }

    NilpotentMaximum* NilpotentMaximum::clone() const {
        return new NilpotentMaximum(*this);
    }

    SNorm* NilpotentMaximum::constructor() {
        return new NilpotentMaximum;
    }

}
