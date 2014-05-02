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
 * Rule.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_RULE_H
#define FL_RULE_H

#include "fl/fuzzylite.h"

#include <string>
#include <vector>

namespace fl {
    class Engine;
    class Antecedent;
    class Consequent;
    class Hedge;
    class TNorm;
    class SNorm;

    class FL_EXPORT Rule {
    protected:
        std::string _text;
        scalar _weight;
        Antecedent* _antecedent;
        Consequent* _consequent;
        std::vector<Hedge*> _hedges;

    public:
        Rule(const std::string& text = "", scalar weight = 1.0);
        Rule(const Rule& source);
        Rule& operator=(const Rule& rhs);
        virtual ~Rule();

        virtual void setText(const std::string& text);
        virtual std::string getText() const;

        virtual void setWeight(scalar weight);
        virtual scalar getWeight() const;

        virtual void setAntecedent(Antecedent* antecedent);
        virtual Antecedent* getAntecedent() const;

        virtual void setConsequent(Consequent* consequent);
        virtual Consequent* getConsequent() const;

        virtual void addHedge(Hedge* hedge);
        virtual void insertHedge(Hedge* hedge, int index);
        virtual Hedge* getHedge(int index) const;
        virtual Hedge* getHedge(const std::string& name) const;
        virtual bool hasHedge(const std::string& name) const;
        virtual Hedge* removeHedge(int index);
        virtual Hedge* removeHedge(const std::string& name);
        virtual int numberOfHedges() const;
        virtual const std::vector<Hedge*>& constHedges() const;
        virtual void setHedges(const std::vector<Hedge*>& hedges);
        virtual std::vector<Hedge*>& hedges();


        virtual scalar activationDegree(const TNorm* conjunction, const SNorm* disjunction) const;
        virtual void activate(scalar degree, const TNorm* activation) const;

        virtual std::string toString() const;

        virtual bool isLoaded() const;
        virtual void unload();
        virtual void load(const Engine* engine);
        virtual void load(const std::string& rule, const Engine* engine);

        static Rule* parse(const std::string& rule, const Engine* engine);

        static std::string ifKeyword() {
            return "if";
        }

        static std::string isKeyword() {
            return "is";
        }

        static std::string thenKeyword() {
            return "then";
        }

        static std::string andKeyword() {
            return "and";
        }

        static std::string orKeyword() {
            return "or";
        }

        static std::string withKeyword() {
            return "with";
        }

    };

}


#endif /* FL_RULE_H */
