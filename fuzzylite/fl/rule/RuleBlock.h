/*
 * RuleBlock.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_RULEBLOCK_H
#define FL_RULEBLOCK_H

#include "fl/config.h"

#include <vector>
#include <string>

namespace fl {

    class Rule;
    class TNorm;
    class SNorm;

    flclass RuleBlock {
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
