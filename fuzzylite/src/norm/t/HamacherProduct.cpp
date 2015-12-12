/*
 Copyright © 2010-2015 by FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#include "fl/norm/t/HamacherProduct.h"


namespace fl {

    std::string HamacherProduct::className() const {
        return "HamacherProduct";
    }

    scalar HamacherProduct::compute(scalar a, scalar b) const {
        return (a * b) / (a + b - a * b);
    }

    HamacherProduct* HamacherProduct::clone() const {
        return new HamacherProduct(*this);
    }

    TNorm* HamacherProduct::constructor() {
        return new HamacherProduct;
    }

}
