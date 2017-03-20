/*
 fuzzylite (R), a fuzzy logic control library in C++.
 Copyright (C) 2010-2017 FuzzyLite Limited. All rights reserved.
 Author: Juan Rada-Vilela, Ph.D. <jcrada@fuzzylite.com>

 This file is part of fuzzylite.

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite is a registered trademark of FuzzyLite Limited.
 */

#include "fl/rule/Consequent.h"

#include "fl/Engine.h"
#include "fl/factory/HedgeFactory.h"
#include "fl/factory/FactoryManager.h"
#include "fl/hedge/Any.h"
#include "fl/rule/Expression.h"
#include "fl/rule/Rule.h"
#include "fl/term/Aggregated.h"
#include "fl/variable/OutputVariable.h"

namespace fl {

    Consequent::Consequent() { }

    Consequent::~Consequent() {
        for (std::size_t i = 0; i < _conclusions.size(); ++i) {
            delete _conclusions.at(i);
        }
        _conclusions.clear();
    }

    std::string Consequent::getText() const {
        return this->_text;
    }

    void Consequent::setText(const std::string& text) {
        this->_text = text;
    }

    const std::vector<Proposition*>& Consequent::conclusions() const {
        return this->_conclusions;
    }

    std::vector<Proposition*>& Consequent::conclusions() {
        return this->_conclusions;
    }

    Complexity Consequent::complexity(const TNorm* implication) const {
        Complexity result;
        result.comparison(1);

        for (std::size_t i = 0; i < _conclusions.size(); ++i) {
            Proposition* proposition = _conclusions.at(i);
            result.comparison(2);
            for (std::size_t h = 0; h < proposition->hedges.size(); ++h) {
                result += proposition->hedges.at(h)->complexity();
            }
            result += static_cast<OutputVariable*> (proposition->variable)
                    ->complexity(Activated(proposition->term, fl::nan, implication));
        }
        return result;
    }

    void Consequent::modify(scalar activationDegree, const TNorm* implication) {
        if (not isLoaded()) {
            throw Exception("[consequent error] consequent <" + getText() + "> is not loaded", FL_AT);
        }
        for (std::size_t i = 0; i < _conclusions.size(); ++i) {
            Proposition* proposition = _conclusions.at(i);
            if (proposition->variable->isEnabled()) {
                if (not proposition->hedges.empty()) {
                    for (std::vector<Hedge*>::const_reverse_iterator rit = proposition->hedges.rbegin();
                            rit != proposition->hedges.rend(); ++rit) {
                        activationDegree = (*rit)->hedge(activationDegree);
                    }
                }

                static_cast<OutputVariable*> (proposition->variable)->fuzzyOutput()
                        ->addTerm(proposition->term, activationDegree, implication);
            }
        }
    }

    bool Consequent::isLoaded() {
        return not _conclusions.empty();
    }

    void Consequent::unload() {
        for (std::size_t i = 0; i < _conclusions.size(); ++i) {
            delete _conclusions.at(i);
        }
        _conclusions.clear();
    }

    void Consequent::load(const Engine* engine) {
        load(getText(), engine);
    }

    void Consequent::load(const std::string& consequent, const Engine* engine) {
        unload();
        setText(consequent);

        if (Op::trim(consequent).empty()) {
            throw Exception("[syntax error] consequent is empty", FL_AT);
        }

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
            S_AND = 16, S_WITH = 32
        };
        int state = S_VARIABLE;

        Proposition* proposition = fl::null;

        std::stringstream tokenizer(consequent);
        std::string token;
        try {
            while (tokenizer >> token) {
                if (state bitand S_VARIABLE) {
                    if (engine->hasOutputVariable(token)) {
                        proposition = new Proposition;
                        proposition->variable = engine->getOutputVariable(token);
                        conclusions().push_back(proposition);
                        state = S_IS;
                        continue;
                    }
                }

                if (state bitand S_IS) {
                    if (token == Rule::isKeyword()) {
                        state = S_HEDGE bitor S_TERM;
                        continue;
                    }
                }

                if (state bitand S_HEDGE) {
                    HedgeFactory* factory = FactoryManager::instance()->hedge();
                    if (factory->hasConstructor(token)) {
                        Hedge* hedge = factory->constructObject(token);
                        proposition->hedges.push_back(hedge);
                        state = S_HEDGE bitor S_TERM;
                        continue;
                    }
                }

                if (state bitand S_TERM) {
                    if (proposition->variable->hasTerm(token)) {
                        proposition->term = proposition->variable->getTerm(token);
                        state = S_AND bitor S_WITH;
                        continue;
                    }
                }

                if (state bitand S_AND) {
                    if (token == Rule::andKeyword()) {
                        state = S_VARIABLE;
                        continue;
                    }
                }

                //if reached this point, there was an error:
                if (state bitand S_VARIABLE) {
                    std::ostringstream ex;
                    ex << "[syntax error] consequent expected output variable, but found <" << token << ">";
                    throw Exception(ex.str(), FL_AT);
                }
                if (state bitand S_IS) {
                    std::ostringstream ex;
                    ex << "[syntax error] consequent expected keyword <" << Rule::isKeyword() << ">, "
                            "but found <" << token << ">";
                    throw Exception(ex.str(), FL_AT);
                }

                if ((state bitand S_HEDGE) or (state bitand S_TERM)) {
                    std::ostringstream ex;
                    ex << "[syntax error] consequent expected hedge or term, but found <" << token << ">";
                    throw Exception(ex.str(), FL_AT);
                }

                if ((state bitand S_AND) or (state bitand S_WITH)) {
                    std::ostringstream ex;
                    ex << "[syntax error] consequent expected operator <" << Rule::andKeyword() << "> "
                            << "or keyword <" << Rule::withKeyword() << ">, "
                            << "but found <" << token << ">";
                    throw Exception(ex.str(), FL_AT);
                }

                std::ostringstream ex;
                ex << "[syntax error] unexpected token <" << token << "> in consequent";
                throw Exception(ex.str(), FL_AT);
            }

            if (not ((state bitand S_AND) or (state bitand S_WITH))) { //only acceptable final state
                if (state bitand S_VARIABLE) {
                    std::ostringstream ex;
                    ex << "[syntax error] consequent expected output variable after <" << token << ">";
                    throw Exception(ex.str(), FL_AT);
                }
                if (state bitand S_IS) {
                    std::ostringstream ex;
                    ex << "[syntax error] consequent expected keyword <" << Rule::isKeyword() << "> "
                            "after <" << token << ">";
                    throw Exception(ex.str(), FL_AT);
                }
                if ((state bitand S_HEDGE) or (state bitand S_TERM)) {
                    std::ostringstream ex;
                    ex << "[syntax error] consequent expected hedge or term after <" << token << ">";
                    throw Exception(ex.str(), FL_AT);
                }
            }
        } catch (...) {
            unload();
            throw;
        }
    }

    std::string Consequent::toString() const {
        std::stringstream ss;
        for (std::size_t i = 0; i < conclusions().size(); ++i) {
            ss << conclusions().at(i)->toString();
            if (i + 1 < conclusions().size())
                ss << " " << Rule::andKeyword() << " ";
        }
        return ss.str();
    }

}
