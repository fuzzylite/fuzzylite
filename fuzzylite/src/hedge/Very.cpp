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

#include "fl/hedge/Very.h"

namespace fl {

    std::string Very::name() const {
        return "very";
    }

    scalar Very::hedge(scalar x) const {
        return x * x;
    }

    Very* Very::clone() const {
        return new Very(*this);
    }

    Hedge* Very::constructor() {
        return new Very;
    }

}
