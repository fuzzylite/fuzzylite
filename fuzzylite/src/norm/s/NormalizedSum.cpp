/*
 Author: Juan Rada-Vilela, Ph.D.
 Copyright © 2010-2015 FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#include "fl/norm/s/NormalizedSum.h"

namespace fl {

    std::string NormalizedSum::className() const {
        return "NormalizedSum";
    }

    scalar NormalizedSum::compute(scalar a, scalar b) const {
        return a + b / Op::max(scalar(1.0), Op::max(a, b));
    }

    NormalizedSum* NormalizedSum::clone() const {
        return new NormalizedSum(*this);
    }

    SNorm* NormalizedSum::constructor() {
        return new NormalizedSum;
    }


}
