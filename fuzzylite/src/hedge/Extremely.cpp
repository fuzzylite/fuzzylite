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

#include "fl/hedge/Extremely.h"

#include "fl/Operation.h"

namespace fl {

    std::string Extremely::name() const {
        return "extremely";
    }

    scalar Extremely::hedge(scalar x) const {
        return Op::isLE(x, 0.5)
                ? 2.0 * x * x
                : 1.0 - 2.0 * (1.0 - x) * (1.0 - x);
    }

    Extremely* Extremely::clone() const {
        return new Extremely(*this);
    }

    Hedge* Extremely::constructor() {
        return new Extremely;
    }


}
