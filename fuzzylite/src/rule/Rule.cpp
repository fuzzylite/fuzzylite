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

    Rule::Rule(const std::string& text, scalar weight)
    : _text(text), _weight(weight), _antecedent(new Antecedent), _consequent(new Consequent) {
    }

    Rule::Rule(const Rule& source) : _text(source._text), _weight(source._weight),
    _antecedent(new Antecedent), _consequent(new Consequent) {
    }

    Rule& Rule::operator =(const Rule& rhs) {
        if (this == &rhs) return *this;
        unload();
        if (_consequent) delete _consequent;
        if (_antecedent) delete _antecedent;

        _text = rhs._text;
        _weight = rhs._weight;
        _antecedent = new Antecedent;
        _consequent = new Consequent;
        return *this;
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

    const std::vector<Hedge*>& Rule::constHedges() const {
        return this->_hedges;
    }

    void Rule::setHedges(const std::vector<Hedge*>& hedges) {
        this->_hedges = hedges;
    }

    std::vector<Hedge*>& Rule::hedges() {
        return this->_hedges;
    }

    scalar Rule::activationDegree(const TNorm* conjunction, const SNorm* disjunction) const {
        if (not isLoaded()) {
            throw fl::Exception("[rule error] the following rule is not loaded: " + _text, FL_AT);
        }
        return _weight * getAntecedent()->activationDegree(conjunction, disjunction);
    }

    void Rule::activate(scalar degree, const TNorm* activation) const {
        if (not isLoaded()) {
            throw fl::Exception("[rule error] the following rule is not loaded: " + _text, FL_AT);
        }
        getConsequent()->modify(degree, activation);
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
                        if (token == Rule::ifKeyword()) state = S_IF;
                        else {
                            std::ostringstream ex;
                            ex << "[syntax error] expected keyword <" << Rule::ifKeyword() <<
                                    ">, but found <" << token << "> in rule: " << rule;
                            throw fl::Exception(ex.str(), FL_AT);
                        }
                        break;
                    case S_IF:
                        if (token == Rule::thenKeyword()) state = S_THEN;
                        else ossAntecedent << token << " ";
                        break;
                    case S_THEN:
                        if (token == Rule::withKeyword()) state = S_WITH;
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
                ex << "[syntax error] keyword <" << Rule::ifKeyword() << "> not found in rule: " << rule;
                throw fl::Exception(ex.str(), FL_AT);
            } else if (state == S_IF) {
                std::ostringstream ex;
                ex << "[syntax error] keyword <" << Rule::thenKeyword() << "> not found in rule: " << rule;
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
            (void) ex;
            unload();
            throw;
        }
    }

    std::string Rule::toString() const {
        return FllExporter("", "; ").toString(this);
    }

    Rule* Rule::parse(const std::string& rule, const Engine* engine) {
        Rule* result = new Rule;
        try {
            result->load(rule, engine);
        } catch (std::exception& ex) {
            (void) ex;
            delete result;
            throw;
        }
        return result;
    }

}
