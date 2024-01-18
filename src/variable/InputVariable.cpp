/*
 fuzzylite (R), a fuzzy logic control library in C++.
 Copyright (C) 2010-2017 FuzzyLite Limited. All rights reserved.
 Author: Juan Rada-Vilela, Ph.D. <jcrada@fuzzylite.com>

 This file is part of fuzzylite.

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite is a registered trademark of FuzzyLite Limited.
 */

#include "fl/variable/InputVariable.h"

#include "fl/imex/FllExporter.h"

namespace fl {

    InputVariable::InputVariable(const std::string& name, scalar minimum, scalar maximum)
    : Variable(name, minimum, maximum) { }

    InputVariable::~InputVariable() { }

    std::string InputVariable::fuzzyInputValue() const {
        return fuzzify(getValue());
    }

    Variable::Type InputVariable::type() const {
        return Variable::Input;
    }

    std::string InputVariable::toString() const {
        return FllExporter().toString(this);
    }

    InputVariable* InputVariable::clone() const {
        return new InputVariable(*this);
    }

}
