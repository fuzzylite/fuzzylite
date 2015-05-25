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

#include "fl/variable/InputVariable.h"

#include "fl/imex/FllExporter.h"

namespace fl {

    InputVariable::InputVariable(const std::string& name, scalar minimum, scalar maximum)
    : Variable(name, minimum, maximum) {
    }

    InputVariable::~InputVariable() {
    }

    std::string InputVariable::fuzzyInputValue() const {
        return fuzzify(getValue());
    }

    std::string InputVariable::toString() const {
        return FllExporter().toString(this);
    }

}
