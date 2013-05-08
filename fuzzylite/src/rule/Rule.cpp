/*   Copyright 2013 Juan Rada-Vilela

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

/*
 * Rule.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "fl/rule/Rule.h"

#include "fl/rule/Antecedent.h"
#include "fl/rule/Consequent.h"

#include "fl/norm/Norm.h"
#include <sstream>

namespace fl {
    std::string Rule::FL_IF = "if";
    std::string Rule::FL_IS = "is";
    std::string Rule::FL_ASSIGN = "=";
    std::string Rule::FL_THEN = "then";
    std::string Rule::FL_AND = "and";
    std::string Rule::FL_OR = "or";
    std::string Rule::FL_WITH = "with";

    Rule::Rule() : _weight(1.0) { }

    Rule::~Rule() { }

    void Rule::setWeight(scalar weight) {
        this->_weight = weight;
    }

    scalar Rule::getWeight() const {
        return this->_weight;
    }

    scalar Rule::firingStrength(const TNorm* tnorm,
            const SNorm* snorm) const {
        return getAntecedent()->firingStrength(tnorm, snorm) * _weight;
    }

    void Rule::fire(scalar strength, const TNorm* activation) const {
        return getConsequent()->fire(strength, activation);
    }

    void Rule::setUnparsedRule(const std::string& unparsedRule) {
        this->_unparsedRule = unparsedRule;
    }

    std::string Rule::getUnparsedRule() const {
        return this->_unparsedRule;
    }

    std::string Rule::toString() const {
        std::stringstream ss;
        ss << FL_IF << " " << getAntecedent()->toString() << " "
                << FL_THEN << " " << getConsequent()->toString();
        if (not fl::Op::isEq(_weight, 1.0)) {
            ss << " " << FL_WITH << " " << _weight;
        }
        return ss.str();
    }

    std::string Rule::ifKeyword() {
        return fl::Rule::FL_IF;
    }

    std::string Rule::isKeyword() {
        return fl::Rule::FL_IS;
    }
    
    std::string Rule::assignKeyword() {
        return fl::Rule::FL_ASSIGN;
    }

    std::string Rule::thenKeyword() {
        return fl::Rule::FL_THEN;
    }

    std::string Rule::andKeyword() {
        return fl::Rule::FL_AND;
    }

    std::string Rule::orKeyword() {
        return fl::Rule::FL_OR;
    }

    std::string Rule::withKeyword() {
        return fl::Rule::FL_WITH;
    }







}

