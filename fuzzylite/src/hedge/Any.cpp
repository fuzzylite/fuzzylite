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

#include "fl/hedge/Any.h"

namespace fl {

    Any::Any() {
    }

    Any::~Any() {
    }

    std::string Any::name() const {
        return "any";
    }

    scalar Any::hedge(scalar x) const {
        (void) x;
        return 1.0;
    }

    Any* Any::clone() const {
        return new Any(*this);
    }

    Hedge* Any::constructor() {
        return new Any;
    }

}
