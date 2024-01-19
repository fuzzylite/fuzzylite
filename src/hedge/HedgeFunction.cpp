/*
fuzzylite (R), a fuzzy logic control library in C++.

Copyright (C) 2010-2024 FuzzyLite Limited. All rights reserved.
Author: Juan Rada-Vilela, PhD <jcrada@fuzzylite.com>.

This file is part of fuzzylite.

fuzzylite is free software: you can redistribute it and/or modify it under
the terms of the FuzzyLite License included with the software.

You should have received a copy of the FuzzyLite License along with
fuzzylite. If not, see <https://github.com/fuzzylite/fuzzylite/>.

fuzzylite is a registered trademark of FuzzyLite Limited.
*/

#include "fuzzylite/hedge/HedgeFunction.h"

namespace fuzzylite {

    HedgeFunction::HedgeFunction(const std::string& formula) : Hedge() {
        _function.variables["x"] = fl::nan;
        if (not formula.empty())
            _function.load(formula);
    }

    std::string HedgeFunction::name() const {
        return "HedgeFunction";
    }

    scalar HedgeFunction::hedge(scalar x) const {
        _function.variables["x"] = x;
        return _function.membership(x);
    }

    Function& HedgeFunction::function() {
        return this->_function;
    }

    void HedgeFunction::setFormula(const std::string& formula) {
        _function.load(formula);
    }

    std::string HedgeFunction::getFormula() const {
        return _function.getFormula();
    }

    HedgeFunction* HedgeFunction::clone() const {
        return new HedgeFunction(*this);
    }

    Hedge* HedgeFunction::constructor() {
        return new HedgeFunction;
    }

}
