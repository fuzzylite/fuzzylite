// #BEGIN_LICENSE
// fuzzylite: a fuzzy logic control library in C++
// Copyright (C) 2014  Juan Rada-Vilela
// 
// This file is part of fuzzylite.
//
// fuzzylite is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// fuzzylite is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with fuzzylite.  If not, see <http://www.gnu.org/licenses/>.
// #END_LICENSE

#include "fl/rule/Expression.h"

#include "fl/variable/Variable.h"
#include "fl/hedge/Hedge.h"
#include "fl/term/Term.h"
#include "fl/rule/Rule.h"

namespace fl {

    Expression::Expression() {
    }

    Expression::~Expression() {
    }

    Proposition::Proposition()
    : Expression(), variable(NULL), term(NULL) {
    }

    Proposition::~Proposition() {

    }

    Proposition* Proposition::clone() const {
        return new Proposition(*this);
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

        if (term) { //term is NULL if hedge is any
            if (hedges.empty()) {
                ss << " " << Rule::isKeyword() << " ";
            }
            ss << term->getName();
        }
        return ss.str();
    }

    Operator::Operator() : Expression(), name(""), left(NULL), right(NULL) {
    }

    Operator::Operator(const Operator& copy) : Expression(copy),
    name(copy.name), left(NULL), right(NULL) {
        if (copy.left) left = copy.left->clone();
        if (copy.right) right = copy.right->clone();
    }

    Operator& Operator::operator =(const Operator& rhs) {
        if (this == &rhs) return *this;
        if (left) delete left;
        if (right) delete right;
        left = NULL;
        right = NULL;
        Expression::operator=(rhs);
        name = rhs.name;
        if (rhs.left) left = rhs.left->clone();
        if (rhs.right) right = rhs.right->clone();
        return *this;
    }

    Operator::~Operator() {
        if (left) delete left;
        if (right) delete right;
    }

    Operator* Operator::clone() const {
        return new Operator(*this);
    }

    std::string Operator::toString() const {
        return name;
    }

}
