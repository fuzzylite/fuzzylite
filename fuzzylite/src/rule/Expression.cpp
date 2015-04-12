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

#include "fl/rule/Expression.h"

#include "fl/hedge/Hedge.h"
#include "fl/term/Term.h"
#include "fl/rule/Rule.h"
#include "fl/variable/Variable.h"

namespace fl {

    Expression::Expression() {
    }

    Expression::~Expression() {
    }

    Proposition::Proposition()
    : Expression(), variable(fl::null), term(fl::null) {
    }

    Proposition::~Proposition() {

    }

    std::string Proposition::toString() const {
        std::ostringstream ss;
        if (variable) {
            ss << variable->getName();
        } else {
            ss << "?";
        }
        if (not hedges.empty()) {
            ss << " " << Rule::isKeyword() << " ";
            for (std::size_t i = 0; i < hedges.size(); ++i) {
                ss << hedges.at(i)->name() << " ";
            }
        }

        if (term) { //term is fl::null if hedge is any
            if (hedges.empty()) {
                ss << " " << Rule::isKeyword() << " ";
            }
            ss << term->getName();
        }
        return ss.str();
    }

    Operator::Operator() : Expression(), name(""), left(fl::null), right(fl::null) {
    }

    Operator::~Operator() {
        if (left) delete left;
        if (right) delete right;
    }

    std::string Operator::toString() const {
        return name;
    }

}
