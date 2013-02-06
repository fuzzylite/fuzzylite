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
        const TNorm* _tnorm;
        const SNorm* _snorm;
        const TNorm* _activation;
    public:
        RuleBlock(const std::string& name = "");
        virtual ~RuleBlock();

        virtual void fireRules();

        virtual void setName(std::string name);
        virtual std::string getName() const;

        virtual void setTnorm(const TNorm* tnorm);
        virtual const TNorm* getTnorm() const;

        virtual void setSnorm(const SNorm* snorm);
        virtual const SNorm* getSnorm() const;

        virtual void setActivation(const TNorm* activation);
        virtual const TNorm* getActivation() const;

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
