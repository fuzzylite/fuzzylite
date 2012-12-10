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
class Operator;

class Rule {

	protected:
		Antecedent* _antecedent;
		Consequent* _consequent;

	public:
		Rule();
		virtual ~Rule();

		virtual void setAntecedent(Antecedent* antecedent);
		virtual Antecedent* getAntecedent() const;

		virtual void setConsequent(Consequent* consequent);
		virtual Consequent* getConsequent() const;

		virtual scalar firingStrength(const Operator* tnorm, const Operator* snorm) const;
		virtual void fire(scalar strength, const Operator* activation) const;

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
