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
 * Consequent.cpp
 *
 *  Created on: 3/12/2012
 *      Author: jcrada
 */

#include "fl/rule/Consequent.h"

#include "fl/rule/Expression.h"

#include "fl/rule/Rule.h"

#include "fl/Engine.h"
#include "fl/variable/OutputVariable.h"


#include "fl/hedge/Hedge.h"
#include "fl/norm/TNorm.h"

#include "fl/term/Thresholded.h"
#include "fl/term/Accumulated.h"

#include "fl/factory/FactoryManager.h"
#include "fl/factory/HedgeFactory.h"

#include <algorithm>

namespace fl {

    Consequent::Consequent() {
    }

    Consequent::~Consequent() {
        for (std::size_t i = 0; i < _conclusions.size(); ++i) {
            delete _conclusions.at(i);
        }
    }

    std::vector<Proposition*> Consequent::conclusions() const {
        return this->_conclusions;
    }

    void Consequent::modify(scalar strength, const TNorm* activation) {
        for (std::size_t i = 0; i < _conclusions.size(); ++i) {
            Proposition* proposition = _conclusions.at(i);
            if (not proposition->variable->isEnabled()){
                continue;
            }
            scalar threshold = strength;
            for (std::size_t h = 0; h < proposition->hedges.size(); ++h) {
                threshold = proposition->hedges.at(h)->hedge(threshold);
            }
            Thresholded* term = new Thresholded(_conclusions.at(i)->term);
            term->setThreshold(threshold);
            term->setActivation(activation);
            OutputVariable* outputVariable = dynamic_cast<OutputVariable*> (proposition->variable);
            outputVariable->fuzzyOutput()->addTerm(term);
            FL_DBG("Accumulating " << term->toString());
        }
    }

    void Consequent::load(const std::string& consequent, const Engine* engine) {

        /**
         Extracts the list of propositions from the consequent
         The rules are:
         1) After a variable comes 'is' or '=',
         2) After 'is' comes a hedge or a term
         3) After a hedge comes a hedge or a term
         4) After a term comes operators 'and' or 'with'
         5) After operator 'and' comes a variable
         6) After operator 'with' comes a float
         */
        enum FSM {
            S_VARIABLE = 1, S_IS = 2, S_HEDGE = 4, S_TERM = 8,
            S_AND = 16
        };
        int state = S_VARIABLE;

        _conclusions.clear();

        Proposition* proposition = NULL;

        std::stringstream tokenizer(consequent);
        std::string token;
        while (tokenizer >> token) {
            if (state bitand S_VARIABLE) {
                if (engine->hasOutputVariable(token)) {
                    proposition = new Proposition;
                    proposition->variable = engine->getOutputVariable(token);
                    _conclusions.push_back(proposition);

                    state = S_IS;
                    continue;
                }
            }

            if (state bitand S_IS) {
                if (token == Rule::FL_IS or token == Rule::FL_EQUALS) {
                    state = S_HEDGE | S_TERM;
                    continue;
                }
            }

            if (state bitand S_HEDGE) {
                Hedge* hedge = NULL;
                if (engine->hasHedge(token)) {
                    hedge = engine->getHedge(token);
                } else {
                    std::vector<std::string> hedges = FactoryManager::instance()->hedge()->available();
                    if (std::find(hedges.begin(), hedges.end(), token) != hedges.end()) {
                        hedge = FactoryManager::instance()->hedge()->createInstance(token);
                        //TODO: find a better way, eventually.
                        const_cast<Engine*> (engine)->addHedge(hedge);
                    }
                }
                if (hedge) {
                    proposition->hedges.push_back(hedge);
                    state = S_HEDGE | S_TERM;
                    continue;
                }
            }

            if (state bitand S_TERM) {
                if (proposition->variable->hasTerm(token)) {
                    proposition->term = proposition->variable->getTerm(token);
                    state = S_AND;
                    continue;
                }
            }

            if (state bitand S_AND) {
                if (token == Rule::FL_AND) {
                    state = S_VARIABLE;
                    continue;
                }
            }

            //if reached this point, there was an error:
            if (state bitand S_VARIABLE) {
                std::ostringstream ex;
                ex << "[syntax error] expected output variable, but found <" << token << ">";
                throw fl::Exception(ex.str(), FL_AT);
            }
            if (state bitand S_IS) {
                std::ostringstream ex;
                ex << "[syntax error] expected keyword <" << Rule::FL_IS << "> or <"
                        << Rule::FL_EQUALS << ">, but found <" << token << ">";
                throw fl::Exception(ex.str(), FL_AT);
            }

            if ((state bitand S_HEDGE) or (state bitand S_TERM)) {
                std::ostringstream ex;
                ex << "[syntax error] expected hedge or term, but found <" << token << ">";
                throw fl::Exception(ex.str(), FL_AT);
            }

            if (state bitand S_AND) {
                std::ostringstream ex;
                ex << "[syntax error] expected operator <" << Rule::FL_AND << ">, "
                        << "but found <" << token << ">";
                throw fl::Exception(ex.str(), FL_AT);
            }

            std::ostringstream ex;
            ex << "[syntax error] unexpected token <" << token << ">";
            throw fl::Exception(ex.str(), FL_AT);
        }
    }

    std::string Consequent::toString() const {
        std::stringstream ss;
        for (std::size_t i = 0; i < _conclusions.size(); ++i) {
            ss << _conclusions.at(i)->toString();
            if (i + 1 < _conclusions.size())
                ss << " " << Rule::FL_AND << " ";
        }
        return ss.str();
    }

}
