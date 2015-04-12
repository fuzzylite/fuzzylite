/*
 Author: Juan Rada-Vilela, Ph.D.
 Copyright © 2010-2015 FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#ifndef FL_RULE_H
#define FL_RULE_H

#include "fl/fuzzylite.h"

#include <map>
#include <string>


namespace fl {
    class Engine;
    class Antecedent;
    class Consequent;
    class Hedge;
    class TNorm;
    class SNorm;

    class FL_API Rule {
    protected:
        std::string _text;
        scalar _weight;
        FL_unique_ptr<Antecedent> _antecedent;
        FL_unique_ptr<Consequent> _consequent;
        std::map<std::string, Hedge*> _hedges;

    public:
        explicit Rule(const std::string& text = "", scalar weight = 1.0);
        Rule(const Rule& other);
        Rule& operator=(const Rule& other);
        virtual ~Rule();
        FL_DEFAULT_MOVE(Rule)

        virtual void setText(const std::string& text);
        virtual std::string getText() const;

        virtual void setWeight(scalar weight);
        virtual scalar getWeight() const;

        virtual void setAntecedent(Antecedent* antecedent);
        virtual Antecedent* getAntecedent() const;

        virtual void setConsequent(Consequent* consequent);
        virtual Consequent* getConsequent() const;

        virtual void addHedge(Hedge* hedge);
        virtual Hedge* getHedge(const std::string& name) const;
        virtual Hedge* removeHedge(const std::string& hedge);
        virtual bool hasHedge(const std::string& name) const;
        virtual int numberOfHedges() const;
        virtual void setHedges(const std::map<std::string, Hedge*>& hedges);
        virtual const std::map<std::string, Hedge*>& hedges() const;
        virtual std::map<std::string, Hedge*>& hedges();

        virtual scalar activationDegree(const TNorm* conjunction, const SNorm* disjunction) const;
        virtual void activate(scalar degree, const TNorm* activation) const;

        virtual std::string toString() const;

        virtual bool isLoaded() const;
        virtual void unload();
        virtual void load(const Engine* engine);
        virtual void load(const std::string& rule, const Engine* engine);
        
        virtual Rule* clone() const;

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
