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

#ifndef FL_RULEBLOCK_H
#define FL_RULEBLOCK_H

#include "fl/fuzzylite.h"

#include <string>
#include <vector>


namespace fl {

    class Engine;
    class Rule;
    class TNorm;
    class SNorm;
    class Activation;

    class FL_API RuleBlock {
    private:
        void copyFrom(const RuleBlock& source);
    protected:
        std::string _name;
        std::vector<Rule*> _rules;
        FL_unique_ptr<TNorm> _conjunction;
        FL_unique_ptr<SNorm> _disjunction;
        FL_unique_ptr<TNorm> _implication;
        FL_unique_ptr<Activation> _activation;
        bool _enabled;

    public:
        explicit RuleBlock(const std::string& name = "");
        RuleBlock(const RuleBlock& other);
        RuleBlock& operator=(const RuleBlock& other);
        virtual ~RuleBlock();
        FL_DEFAULT_MOVE(RuleBlock)

        virtual void activate();

        virtual void setName(std::string name);
        virtual std::string getName() const;

        virtual void setConjunction(TNorm* conjunction);
        virtual TNorm* getConjunction() const;

        virtual void setDisjunction(SNorm* disjunction);
        virtual SNorm* getDisjunction() const;

        virtual void setImplication(TNorm* implication);
        virtual TNorm* getImplication() const;

        virtual void setActivation(Activation* activation);
        virtual Activation* getActivation() const;

        virtual void setEnabled(bool enabled);
        virtual bool isEnabled() const;

        virtual void unloadRules() const;
        virtual void loadRules(const Engine* engine);
        virtual void reloadRules(const Engine* engine);

        virtual std::string toString() const;

        /**
         * Operations for iterable datatype _rules
         */
        virtual void addRule(Rule* rule);
        virtual void insertRule(Rule* rule, std::size_t index);
        virtual Rule* getRule(std::size_t index) const;
        virtual Rule* removeRule(std::size_t index);
        virtual std::size_t numberOfRules() const;
        virtual void setRules(const std::vector<Rule*>& rules);
        virtual const std::vector<Rule*>& rules() const;
        virtual std::vector<Rule*>& rules();

    };

}
#endif /* RULEBLOCK_H */
