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

    InputVariable* InputVariable::clone() const {
        return new InputVariable(*this);
    }

}
