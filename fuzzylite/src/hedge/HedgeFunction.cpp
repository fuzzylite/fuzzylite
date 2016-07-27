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

#include "fl/hedge/HedgeFunction.h"

namespace fl {

    HedgeFunction::HedgeFunction(const std::string& formula) : Hedge() {
        _function.variables["x"] = fl::nan;
        if (not formula.empty()) {
            _function.load(formula);
        }
    }

    std::string HedgeFunction::name() const {
        return "HedgeFunction";
    }

    Complexity HedgeFunction::complexity() const {
        if (_function.root())
            return _function.complexity().function(2 * std::log(scalar(_function.variables.size())));
        return _function.complexity();
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
