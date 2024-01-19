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

#include "fuzzylite/norm/s/SNormFunction.h"

namespace fuzzylite {

    SNormFunction::SNormFunction(const std::string& formula) : SNorm() {
        _function.variables["a"] = fl::nan;
        _function.variables["b"] = fl::nan;
        if (not formula.empty())
            _function.load(formula);
    }

    std::string SNormFunction::className() const {
        return "SNormFunction";
    }

    scalar SNormFunction::compute(scalar a, scalar b) const {
        _function.variables["a"] = a;
        _function.variables["b"] = b;
        return _function.evaluate();
    }

    Function& SNormFunction::function() {
        return this->_function;
    }

    void SNormFunction::setFormula(const std::string& formula) {
        this->_function.load(formula);
    }

    std::string SNormFunction::getFormula() const {
        return _function.getFormula();
    }

    SNormFunction* SNormFunction::clone() const {
        return new SNormFunction(*this);
    }

    SNorm* SNormFunction::constructor() {
        return new SNormFunction;
    }

}
