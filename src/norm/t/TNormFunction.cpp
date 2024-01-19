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

#include "fuzzylite/norm/t/TNormFunction.h"

namespace fuzzylite {

    TNormFunction::TNormFunction(const std::string& formula) : TNorm() {
        _function.variables["a"] = fl::nan;
        _function.variables["b"] = fl::nan;
        if (not formula.empty())
            _function.load(formula);
    }

    std::string TNormFunction::className() const {
        return "TNormFunction";
    }

    scalar TNormFunction::compute(scalar a, scalar b) const {
        _function.variables["a"] = a;
        _function.variables["b"] = b;
        return _function.evaluate();
    }

    Function& TNormFunction::function() {
        return this->_function;
    }

    void TNormFunction::setFormula(const std::string& formula) {
        this->_function.load(formula);
    }

    std::string TNormFunction::getFormula() const {
        return _function.getFormula();
    }

    TNormFunction* TNormFunction::clone() const {
        return new TNormFunction(*this);
    }

    TNorm* TNormFunction::constructor() {
        return new TNormFunction;
    }

}
