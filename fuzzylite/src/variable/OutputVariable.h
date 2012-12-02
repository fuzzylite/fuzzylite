/*
 * OutputVariable.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef OUTPUTVARIABLE_H_
#define OUTPUTVARIABLE_H_

#include "Variable.h"

namespace fl {
class Cumulative;
class Defuzzifier;

class OutputVariable: public Variable {
	protected:
		scalar _defaultValue;
		Defuzzifier* _defuzzifier;
		Cumulative* _output;

	public:
		OutputVariable(const std::string& name);
		~OutputVariable();

		void setDefaultValue(scalar defaultValue);
		scalar getDefaultValue() const;

		void setDefuzzifier(Defuzzifier* defuzzifier);
		Defuzzifier* getDefuzzifier() const;

		Cumulative* output() const;

		scalar defuzzify() const;

		std::string toString() const;

};

} /* namespace fl */
#endif /* OUTPUTVARIABLE_H_ */
