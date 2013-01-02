/*
 * Rule.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_RULE_H_
#define FL_RULE_H_

#include "fl/scalar.h"

#include <string>

namespace fl {

    class Antecedent;
    class Consequent;
    class TNorm;
    class SNorm;

    class Rule {
    protected:
        scalar _weight;
        std::string _unparsedRule;

        virtual void setUnparsedRule(const std::string& unparsedRule);
    public:
        Rule();
        virtual ~Rule();

        virtual Antecedent* getAntecedent() const = 0;

        virtual Consequent* getConsequent() const = 0;

        virtual scalar firingStrength(const TNorm* tnorm, const SNorm* snorm) const;
        virtual void fire(scalar strength, const TNorm* activation) const;
        
        virtual void setWeight(scalar weight);
        virtual scalar getWeight() const;
        
        virtual std::string getUnparsedRule() const;

        virtual std::string toString() const;

        static std::string FL_IF;
        static std::string FL_IS;
        static std::string FL_THEN;
        static std::string FL_AND;
        static std::string FL_OR;
        static std::string FL_WITH;
    };
}




#endif /* FL_RULE_H_ */
