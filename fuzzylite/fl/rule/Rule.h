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
        Rule();
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
        virtual const std::vector<Hedge*>& hedges() const;
        virtual void setHedges(const std::vector<Hedge*>& hedges);


        virtual scalar activationDegree(const TNorm* conjunction, const SNorm* disjunction) const;
        virtual void activate(scalar degree, const TNorm* activation) const;

        virtual std::string toString() const;

        virtual bool isLoaded() const;
        virtual void unload();
        virtual void load(const Engine* engine);
        virtual void load(const std::string& rule, const Engine* engine);

        static Rule* parse(const std::string& rule, const Engine* engine);

        static std::string FL_IF;
        static std::string FL_IS;
        static std::string FL_THEN;
        static std::string FL_AND;
        static std::string FL_OR;
        static std::string FL_WITH;

        static std::string ifKeyword();
        static std::string isKeyword();
        static std::string thenKeyword();
        static std::string andKeyword();
        static std::string orKeyword();
        static std::string withKeyword();


    };
}


#endif /* FL_RULE_H */
