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
 * RuleBlock.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_RULEBLOCK_H
#define FL_RULEBLOCK_H

#include "fl/fuzzylite.h"

#include <vector>
#include <string>

namespace fl {

    class Rule;
    class TNorm;
    class SNorm;

    class FL_EXPORT RuleBlock {
    protected:
        std::vector<Rule*> _rules;
        std::string _name;
        const TNorm* _conjunction;
        const SNorm* _disjunction;
        const TNorm* _activation;
        bool _enabled;
    public:
        RuleBlock(const std::string& name = "");
        virtual ~RuleBlock();

        virtual void activate();

        virtual void setName(std::string name);
        virtual std::string getName() const;

        virtual void setConjunction(const TNorm* conjunction);
        virtual const TNorm* getConjunction() const;

        virtual void setDisjunction(const SNorm* disjunction);
        virtual const SNorm* getDisjunction() const;

        virtual void setActivation(const TNorm* activation);
        virtual const TNorm* getActivation() const;

        virtual void setEnabled(bool enabled);
        virtual bool isEnabled() const;

        virtual std::string toString() const;

        /**
         * Operations for iterable datatype _rules
         */
        virtual void addRule(Rule* rule);
        virtual void insertRule(Rule* rule, int index);
        virtual Rule* getRule(int index) const;
        virtual Rule* removeRule(int index);
        virtual int numberOfRules() const;
        virtual const std::vector<Rule*>& rules() const;



    };

}
#endif /* RULEBLOCK_H */
