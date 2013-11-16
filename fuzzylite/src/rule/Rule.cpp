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
 *  Created on: 3/12/2012
 *      Author: jcrada
 */

#include "fl/rule/Rule.h"

#include "fl/rule/Antecedent.h"
#include "fl/rule/Consequent.h"

#include "fl/norm/Norm.h"

#include "fl/Exception.h"

#include <sstream>

#include <vector>

namespace fl {

    std::string Rule::FL_IF = "if";
    std::string Rule::FL_IS = "is";
    std::string Rule::FL_EQUALS = "=";
    std::string Rule::FL_THEN = "then";
    std::string Rule::FL_AND = "and";
    std::string Rule::FL_OR = "or";
    std::string Rule::FL_WITH = "with";

    Rule::Rule()
    : _weight(1.0), _antecedent(NULL), _consequent(NULL) {
    }

    Rule::~Rule() {
        if (_consequent) delete _consequent;
        if (_antecedent) delete _antecedent;
    }

    void Rule::setWeight(scalar weight) {
        this->_weight = weight;
    }

    scalar Rule::getWeight() const {
        return this->_weight;
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

    scalar Rule::activationDegree(const TNorm* tnorm,
            const SNorm* snorm) const {
        return getAntecedent()->activationDegree(tnorm, snorm) * _weight;
    }

    void Rule::activate(scalar strength, const TNorm* activation) const {
        return getConsequent()->modify(strength, activation);
    }

    void Rule::setText(const std::string& text) {
        this->_text = text;
    }

    std::string Rule::getText() const {
        return this->_text;
    }

    std::string Rule::toString() const {
        std::stringstream ss;
        ss << FL_IF << " " << getAntecedent()->toString() << " "
                << FL_THEN << " " << getConsequent()->toString();
        if (not fl::Op::isEq(_weight, 1.0)) {
            ss << " " << FL_WITH << " " << fl::Op::str(_weight);
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
        return fl::Rule::FL_EQUALS;
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

    Rule* Rule::parse(const std::string& rule, const Engine* engine) {
        Rule* result = new Rule();
        result->setText(rule);
        std::istringstream tokenizer(rule);
        std::string token;
        std::ostringstream ossAntecedent, ossConsequent;

        enum FSM {
            S_NONE, S_IF, S_THEN, S_WITH, S_END
        };
        FSM state = S_NONE;
        try {
            while (tokenizer >> token) {

                switch (state) {
                    case S_NONE:
                        if (token == Rule::FL_IF) state = S_IF;
                        else {
                            std::ostringstream ex;
                            ex << "[syntax error] expected keyword <" << Rule::FL_IF <<
                                    ">, but found <" << token << "> in rule: " << rule;
                            throw fl::Exception(ex.str(), FL_AT);
                        }
                        break;
                    case S_IF:
                        if (token == Rule::FL_THEN) state = S_THEN;
                        else ossAntecedent << token << " ";
                        break;
                    case S_THEN:
                        if (token == Rule::FL_WITH) state = S_WITH;
                        else ossConsequent << token << " ";
                        break;
                    case S_WITH:
                        try {
                            result->setWeight(fl::Op::toScalar(token));
                            state = S_END;
                        } catch (fl::Exception& e) {
                            std::ostringstream ex;
                            ex << "[syntax error] expected a numeric value as the weight of the rule: "
                                    << rule;
                            e.append(ex.str(), FL_AT);
                            throw e;
                        }
                        break;
                    case S_END:
                        std::ostringstream ex;
                        ex << "[syntax error] unexpected token <" << token << "> at the end of rule";
                        throw fl::Exception(ex.str(), FL_AT);
                }
            }
            if (state == S_NONE) {
                std::ostringstream ex;
                ex << "[syntax error] keyword <" << Rule::FL_IF << "> not found in rule: " << rule;
                throw fl::Exception(ex.str(), FL_AT);
            } else if (state == S_IF) {
                std::ostringstream ex;
                ex << "[syntax error] keyword <" << Rule::FL_THEN << "> not found in rule: " << rule;
                throw fl::Exception(ex.str(), FL_AT);
            } else if (state == S_WITH) {
                std::ostringstream ex;
                ex << "[syntax error] expected a numeric value as the weight of the rule: " << rule;
                throw fl::Exception(ex.str(), FL_AT);
            }

            result->_antecedent = new Antecedent;
            result->_antecedent->load(ossAntecedent.str(), engine);

            result->_consequent = new Consequent;
            result->_consequent->load(ossConsequent.str(), engine);
        } catch (fl::Exception& ex) {
            delete result;
            throw ex;
        }
        return result;
    }

}
