/*
 * Rule.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "fl/rule/Rule.h"

#include "fl/rule/Antecedent.h"
#include "fl/rule/Consequent.h"

#include <sstream>

namespace fl {

    std::string Rule::FL_IF = "if";
    std::string Rule::FL_IS = "is";
    std::string Rule::FL_THEN = "then";
    std::string Rule::FL_AND = "and";
    std::string Rule::FL_OR = "or";
    std::string Rule::FL_WITH = "with";

    Rule::Rule()
    : _antecedent(NULL), _consequent(NULL) { }

    Rule::~Rule() {
        delete _consequent;
        delete _antecedent;
    }

    void Rule::setAntecedent(Antecedent* antecedent) {
        this->_antecedent = antecedent;
    }

    Antecedent* Rule::getAntecedent() const {
        return this->_antecedent;
    }

    void Rule::setConsequent(Consequent* consequent) {
        this->_consequent = consequent;
    }

    Consequent* Rule::getConsequent() const {
        return this->_consequent;
    }

    scalar Rule::firingStrength(const TNorm* tnorm,
            const SNorm* snorm) const {
        return this->_antecedent->firingStrength(tnorm, snorm);
    }

    void Rule::fire(scalar strength, const TNorm* activation) const {
        return this->_consequent->fire(strength, activation);
    }

    std::string Rule::toString() const {
        std::stringstream ss;
        ss << FL_IF << " " << _antecedent->toString() << " "
                << FL_THEN << " " << _consequent->toString();
        return ss.str();

    }

}

