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
 * RuleBlock.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_RULEBLOCK_H
#define FL_RULEBLOCK_H

#include "fl/fuzzylite.h"

#include <memory>
#include <string>
#include <vector>


namespace fl {

    class Engine;
    class Rule;
    class TNorm;
    class SNorm;

    class FL_EXPORT RuleBlock {
    private:
        void copyFrom(const RuleBlock& source);
    protected:
        std::vector<Rule*> _rules;
        std::string _name;
        std::auto_ptr<TNorm> _conjunction;
        std::auto_ptr<SNorm> _disjunction;
        std::auto_ptr<TNorm> _activation;
        bool _enabled;

    public:
        RuleBlock(const std::string& name = "");
        RuleBlock(const RuleBlock& source);
        RuleBlock& operator=(const RuleBlock& rhs);
        virtual ~RuleBlock();

        virtual void activate();

        virtual void setName(std::string name);
        virtual std::string getName() const;

        virtual void setConjunction(TNorm* conjunction);
        virtual TNorm* getConjunction() const;

        virtual void setDisjunction(SNorm* disjunction);
        virtual SNorm* getDisjunction() const;

        virtual void setActivation(TNorm* activation);
        virtual TNorm* getActivation() const;

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
        virtual void insertRule(Rule* rule, int index);
        virtual Rule* getRule(int index) const;
        virtual Rule* removeRule(int index);
        virtual int numberOfRules() const;
        virtual void setRules(const std::vector<Rule*>& rules);
        virtual const std::vector<Rule*>& rules() const;
        virtual std::vector<Rule*>& rules();

    };

}
#endif /* RULEBLOCK_H */
