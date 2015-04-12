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

#include "fl/term/Constant.h"

namespace fl {

    Constant::Constant(const std::string& name, scalar value)
    : Term(name), _value(value) {
    }

    Constant::~Constant() {
    }

    std::string Constant::className() const {
        return "Constant";
    }

    scalar Constant::membership(scalar x) const {
        (void) x;
        return this->_value;
    }

    std::string Constant::parameters() const {
        return Op::str(_value);
    }

    void Constant::configure(const std::string& parameters) {
        if (parameters.empty()) return;
        setValue(Op::toScalar(parameters));
    }

    void Constant::setValue(scalar value) {
        this->_value = value;
    }

    scalar Constant::getValue() const {
        return this->_value;
    }

    Constant* Constant::clone() const {
        return new Constant(*this);
    }

    Term* Constant::constructor() {
        return new Constant;
    }

}
