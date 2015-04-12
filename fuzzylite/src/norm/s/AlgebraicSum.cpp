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

#include "fl/norm/s/AlgebraicSum.h"

namespace fl {

    std::string AlgebraicSum::className() const {
        return "AlgebraicSum";
    }

    scalar AlgebraicSum::compute(scalar a, scalar b) const {
        return a + b - (a * b);
    }

    AlgebraicSum* AlgebraicSum::clone() const {
        return new AlgebraicSum(*this);
    }

    SNorm* AlgebraicSum::constructor() {
        return new AlgebraicSum;
    }

}
