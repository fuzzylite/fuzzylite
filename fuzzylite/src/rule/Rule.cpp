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

#include "fl/hedge/Hedge.h"

#include "fl/rule/Antecedent.h"
#include "fl/rule/Consequent.h"

#include "fl/norm/Norm.h"

#include "fl/Exception.h"

#include "fl/imex/FllExporter.h"

#include <sstream>

#include <vector>

namespace fl {

    std::string Rule::FL_IF = "if";
    std::string Rule::FL_IS = "is";
    std::string Rule::FL_THEN = "then";
    std::string Rule::FL_AND = "and";
    std::string Rule::FL_OR = "or";
    std::string Rule::FL_WITH = "with";

    Rule::Rule()
    : _text(""), _weight(1.0), _antecedent(new Antecedent), _consequent(new Consequent) {
    }

    Rule::~Rule() {
        unload();
        delete _antecedent;
        delete _consequent;
    }

    void Rule::setText(const std::string& text) {
        this->_text = text;
    }

    std::string Rule::getText() const {
        return this->_text;
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

    /**
     * Operations for std::vector _hedges
     */
    void Rule::addHedge(Hedge* hedge) {
        this->_hedges.push_back(hedge);
    }

    void Rule::insertHedge(Hedge* hedge, int index) {
        this->_hedges.insert(this->_hedges.begin() + index, hedge);
    }

    Hedge* Rule::getHedge(int index) const {
        return this->_hedges.at(index);
    }

    Hedge* Rule::getHedge(const std::string& name) const {
        for (std::size_t i = 0; i < this->_hedges.size(); ++i) {
            if (name == this->_hedges.at(i)->name())
                return this->_hedges.at(i);
        }
        throw fl::Exception("[engine error] hedge <" + name + "> not found", FL_AT);
    }

    bool Rule::hasHedge(const std::string& name) const {
        for (std::size_t i = 0; i < this->_hedges.size(); ++i) {
            if (name == this->_hedges.at(i)->name())
                return true;
        }
        return false;
    }

    Hedge* Rule::removeHedge(int index) {
        Hedge* result = this->_hedges.at(index);
        this->_hedges.erase(this->_hedges.begin() + index);
        return result;
    }

    Hedge* Rule::removeHedge(const std::string& name) {
        for (std::size_t i = 0; i < this->_hedges.size(); ++i) {
            if (name == this->_hedges.at(i)->name()) {
                Hedge* result = this->_hedges.at(i);
                this->_hedges.erase(this->_hedges.begin() + i);
                return result;
            }
        }
        throw fl::Exception("[engine error] hedge <" + name + "> not found", FL_AT);
    }

    int Rule::numberOfHedges() const {
        return this->_hedges.size();
    }

    const std::vector<Hedge*>& Rule::hedges() const {
        return this->_hedges;
    }

    void Rule::setHedges(const std::vector<Hedge*>& hedges) {
        this->_hedges = hedges;
    }

    scalar Rule::activationDegree(const TNorm* conjunction, const SNorm* disjunction) const {
        if (not isLoaded()) {
            throw fl::Exception("[rule error] the following rule is not loaded: " + _text, FL_AT);
        }
        return getAntecedent()->activationDegree(conjunction, disjunction);
    }

    void Rule::activate(scalar degree, const TNorm* activation) const {
        if (not isLoaded()) {
            throw fl::Exception("[rule error] the following rule is not loaded: " + _text, FL_AT);
        }
        getConsequent()->modify(degree * _weight, activation);
    }

    bool Rule::isLoaded() const {
        return _antecedent->isLoaded() and _consequent->isLoaded();
    }

    void Rule::unload() {
        _antecedent->unload();
        _consequent->unload();
        for (std::size_t i = 0; i < _hedges.size(); ++i) {
            delete _hedges.at(i);
        }
        _hedges.clear();
    }

    void Rule::load(const Engine* engine) {
        load(_text, engine);
    }

    void Rule::load(const std::string& rule, const Engine* engine) {
        this->_text = rule;
        std::istringstream tokenizer(rule);
        std::string token;
        std::ostringstream ossAntecedent, ossConsequent;
        scalar weight = 1.0;

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
                            weight = fl::Op::toScalar(token);
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

            _antecedent->load(ossAntecedent.str(), this, engine);
            _consequent->load(ossConsequent.str(), this, engine);
            _weight = weight;
            
        } catch (fl::Exception& ex) {
            unload();
            throw;
        }
    }

    std::string Rule::toString() const {
        return FllExporter("", "; ").toString(this);
    }

    std::string Rule::ifKeyword() {
        return fl::Rule::FL_IF;
    }

    std::string Rule::isKeyword() {
        return fl::Rule::FL_IS;
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
        Rule* result = new Rule;
        try {
            result->load(rule, engine);
        } catch (std::exception& ex) {
            delete result;
            throw;
        }
        return result;
    }

}
