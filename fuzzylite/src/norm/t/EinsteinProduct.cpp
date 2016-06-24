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

#include "fl/norm/t/EinsteinProduct.h"

namespace fl {

    std::string EinsteinProduct::className() const {
        return "EinsteinProduct";
    }

    scalar EinsteinProduct::compute(scalar a, scalar b) const {
        return (a * b) / (2.0 - (a + b - a * b));
    }

    EinsteinProduct* EinsteinProduct::clone() const {
        return new EinsteinProduct(*this);
    }

    TNorm* EinsteinProduct::constructor() {
        return new EinsteinProduct;
    }

}
