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
 * MamdaniRule.cpp
 *
 *  Created on: 3/12/2012
 *      Author: jcrada
 */

#include "fl/rule/mamdani/MamdaniRule.h"

#include "fl/rule/mamdani/MamdaniAntecedent.h"
#include "fl/rule/mamdani/MamdaniConsequent.h"

#include "fl/operator/Operator.h"

#include "fl/Exception.h"

#include <vector>

namespace fl {

    MamdaniRule::MamdaniRule()
    : Rule(), _antecedent(NULL), _consequent(NULL) { }

    MamdaniRule::~MamdaniRule() {
        if (_consequent) delete _consequent;
        if (_antecedent) delete _antecedent;
    }

    void MamdaniRule::setAntecedent(MamdaniAntecedent* antecedent) {
        this->_antecedent = antecedent;
    }

    MamdaniAntecedent* MamdaniRule::getAntecedent() const {
        return this->_antecedent;
    }

    void MamdaniRule::setConsequent(MamdaniConsequent* consequent) {
        this->_consequent = consequent;
    }

    MamdaniConsequent* MamdaniRule::getConsequent() const {
        return this->_consequent;
    }

    MamdaniRule* MamdaniRule::parse(const std::string& rule, const Engine* engine) {
        MamdaniRule* result = new MamdaniRule();
        result->setUnparsedRule(rule);
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
                        else FL_LOG("ignored token <" << token << "> in rule: " << rule);
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
                        ex << "[syntax error] unexpected token <" << token << "> after the end of rule";
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

            result->_antecedent = new MamdaniAntecedent;
            result->_antecedent->load(ossAntecedent.str(), engine);

            result->_consequent = new MamdaniConsequent;
            result->_consequent->load(ossConsequent.str(), engine);
        } catch (fl::Exception& ex) {
            delete result;
            throw ex;
        }
        return result;
    }

}
