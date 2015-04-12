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

#include "fl/hedge/Seldom.h"

#include "fl/Operation.h"

namespace fl {

    std::string Seldom::name() const {
        return "seldom";
    }

    scalar Seldom::hedge(scalar x) const {
        return Op::isLE(x, 0.5)
                ? std::sqrt(x / 2.0)
                : 1.0 - std::sqrt((1.0 - x) / 2.0);
    }

    Seldom* Seldom::clone() const {
        return new Seldom(*this);
    }

    Hedge* Seldom::constructor() {
        return new Seldom;
    }


}
