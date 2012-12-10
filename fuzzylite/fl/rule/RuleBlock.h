/*
 * RuleBlock.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_RULEBLOCK_H_
#define FL_RULEBLOCK_H_

#include <vector>
#include <string>

namespace fl {

    class Rule;
    class Operator;
    class Configuration;

    class RuleBlock {
    protected:
        std::vector<Rule*> _rules;
        std::string _name;
        const Operator* _tnorm;
        const Operator* _snorm;
        const Operator* _activation;
        public:
        RuleBlock(const std::string& name = "");
        virtual ~RuleBlock();

        virtual void configure(Configuration* config);

        virtual void fireRules();

        virtual void setName(std::string name);
        virtual std::string getName() const;

        virtual void setTnorm(const Operator* tnorm);
        virtual const Operator* getTnorm() const;

        virtual void setSnorm(const Operator* snorm);
        virtual const Operator* getSnorm() const;

        virtual void setActivation(const Operator* activation);
        virtual const Operator* getActivation() const;

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

} /* namespace fl */
#endif /* RULEBLOCK_H_ */
