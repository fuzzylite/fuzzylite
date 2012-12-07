/*
 * OutputVariable.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_OUTPUTVARIABLE_H_
#define FL_OUTPUTVARIABLE_H_

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
		virtual ~OutputVariable();

		virtual void configure(Configuration* config);

		virtual void setDefaultValue(scalar defaultValue);
		virtual scalar getDefaultValue() const;

		virtual void setDefuzzifier(Defuzzifier* defuzzifier);
		virtual Defuzzifier* getDefuzzifier() const;

		virtual Cumulative* output() const;

		virtual scalar defuzzify() const;

		virtual std::string toString() const;

};

} /* namespace fl */
#endif /* FL_OUTPUTVARIABLE_H_ */
