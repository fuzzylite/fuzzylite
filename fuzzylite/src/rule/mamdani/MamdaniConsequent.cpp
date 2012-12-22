/*
 * MamdaniConsequent.cpp
 *
 *  Created on: 3/12/2012
 *      Author: jcrada
 */

#include "fl/rule/mamdani/MamdaniConsequent.h"

#include "fl/operator/TNorm.h"

#include "fl/rule/mamdani/MamdaniExpression.h"

#include "fl/engine/Engine.h"

#include "fl/term/Thresholded.h"
#include "fl/term/Accumulated.h"

#include "fl/definitions.h"

namespace fl {

    MamdaniConsequent::MamdaniConsequent() { }

    MamdaniConsequent::~MamdaniConsequent() {
        for (std::size_t i = 0; i < _conclusions.size(); ++i) {
            delete _conclusions[i];
        }
    }

    void MamdaniConsequent::fire(scalar strength, const TNorm* activation) {
        for (std::size_t i = 0; i < _conclusions.size(); ++i) {
            MamdaniConsequentProposition* proposition = _conclusions[i];
            scalar threshold = strength * proposition->weight;
            for (std::size_t h = 0; h < proposition->hedges.size(); ++h) {
                threshold = proposition->hedges[h]->hedge(threshold);
            }
            Thresholded* term = new Thresholded(_conclusions[i]->term);
            term->setThreshold(threshold);
            term->setActivation(activation);
            proposition->outputVariable->output()->addTerm(term);
            FL_DBG("Accumulating " << term->toString());
        }
    }

    void MamdaniConsequent::load(const std::string& consequent, const Engine* engine) {

        /**
         Extracts the list of propositions from the consequent
         The rules are:
         1) After a variable comes 'is',
         2) After 'is' comes a hedge or a term
         3) After a hedge comes a hedge or a term
         4) After a term comes operators 'and' or 'with'
         5) After operator 'and' comes a variable
         6) After operator 'with' comes a float
         */
        enum FSM {
            S_VARIABLE = 1, S_IS = 2, S_HEDGE = 4, S_TERM = 8,
            S_AND = 16, S_WITH = 32, S_FLOAT = 64
        };
        int state = S_VARIABLE;

        _conclusions.clear();

        MamdaniConsequentProposition* proposition;

        std::stringstream tokenizer(consequent);
        std::string token;
        while (tokenizer >> token) {
            if (state bitand S_VARIABLE) {
                if (engine->hasOutputVariable(token)) {
                    proposition = new MamdaniConsequentProposition;
                    proposition->outputVariable = engine->getOutputVariable(token);
                    _conclusions.push_back(proposition);

                    state = S_IS;
                    continue;
                }
            }

            if (state bitand S_IS) {
                if (token == Rule::FL_IS) {
                    state = S_HEDGE | S_TERM;
                    continue;
                }
            }

            if (state bitand S_HEDGE) {
                if (engine->hasHedge(token)) {
                    proposition->hedges.push_back(engine->getHedge(token));
                    state = S_HEDGE | S_TERM;
                    continue;
                }
            }

            if (state bitand S_TERM) {
                if (proposition->outputVariable->hasTerm(token)) {
                    proposition->term = proposition->outputVariable->getTerm(token);
                    state = S_AND | S_WITH;
                    continue;
                }
            }

            if (state bitand S_AND) {
                if (token == Rule::FL_AND) {
                    state = S_VARIABLE;
                    continue;
                }
            }

            if (state bitand S_WITH) {
                if (token == Rule::FL_WITH) {
                    state = S_FLOAT;
                    continue;
                }
            }

            if (state bitand S_FLOAT) {
                std::stringstream ss(token);
                ss >> proposition->weight;
                if (not ss.fail()) {
                    state = S_AND;
                    continue;
                }
            }

            //if reached this point, there was an error:
            if (state bitand S_VARIABLE) {
                std::ostringstream ex;
                ex << "[syntax error] expected output variable, but found <" << token << ">";
                throw fl::Exception(ex.str());
            }
            if (state bitand S_IS) {
                std::ostringstream ex;
                ex << "[syntax error] expected keyword <" << Rule::FL_IS << ">, but found <"
                        << token << ">";
                throw fl::Exception(ex.str());
            }

            if ((state bitand S_HEDGE) or (state bitand S_TERM)) {
                std::ostringstream ex;
                ex << "[syntax error] expected hedge or term, but found <" << token << ">";
                throw fl::Exception(ex.str());
            }

            if ((state bitand S_AND) or (state bitand S_WITH)) {
                std::ostringstream ex;
                ex << "[syntax error] expected operators <" << Rule::FL_AND << "> or <"
                        << Rule::FL_WITH << ">, "
                        << "but found <" << token << ">";
                throw fl::Exception(ex.str());
            }

            if (state bitand S_FLOAT) {
                std::ostringstream ex;
                ex << "[syntax error] expected numeric value to weight the proposition, "
                        << "but found <" << token << ">";
                throw fl::Exception(ex.str());
            }
            std::ostringstream ex;
            ex << "[syntax error] unexpected token <" << token << ">";
            throw fl::Exception(ex.str());
        }
    }

    std::string MamdaniConsequent::toString() const {
        std::stringstream ss;
        for (std::size_t i = 0; i < _conclusions.size(); ++i) {
            ss << _conclusions[i]->toString();
            if (i < _conclusions.size() - 1)
                ss << " " << Rule::FL_AND << " ";
        }
        return ss.str();
    }

} 
