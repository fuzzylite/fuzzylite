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

#include "fl/norm/t/NilpotentMinimum.h"

#include "fl/Operation.h"

namespace fl {

    std::string NilpotentMinimum::className() const {
        return "NilpotentMinimum";
    }

    scalar NilpotentMinimum::compute(scalar a, scalar b) const {
        if (Op::isGt(a + b, 1.0)) {
            return Op::min(a, b);
        }
        return 0.0;
    }

    NilpotentMinimum* NilpotentMinimum::clone() const {
        return new NilpotentMinimum(*this);
    }

    TNorm* NilpotentMinimum::constructor() {
        return new NilpotentMinimum;
    }


}
