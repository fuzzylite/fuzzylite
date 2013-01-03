/*
 * Rule.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "fl/rule/Rule.h"

#include "fl/rule/Antecedent.h"
#include "fl/rule/Consequent.h"

#include "fl/operator/Operator.h"
#include <sstream>

namespace fl {

    std::string Rule::FL_IF = "if";
    std::string Rule::FL_IS = "is";
    std::string Rule::FL_THEN = "then";
    std::string Rule::FL_AND = "and";
    std::string Rule::FL_OR = "or";
    std::string Rule::FL_WITH = "with";

    Rule::Rule() : _weight(1.0) { }

    Rule::~Rule() {
    }

    void Rule::setWeight(scalar weight) {
        this->_weight = weight;
    }

    scalar Rule::getWeight() const {
        return this->_weight;
    }

    scalar Rule::firingStrength(const TNorm* tnorm,
            const SNorm* snorm) const {
        return getAntecedent()->firingStrength(tnorm, snorm);
    }

    void Rule::fire(scalar strength, const TNorm* activation) const {
        return getConsequent()->fire(strength * _weight, activation);
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

}

