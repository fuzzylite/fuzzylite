/*
 * InputVariable.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_INPUTVARIABLE_H_
#define FL_INPUTVARIABLE_H_

#include "Variable.h"

namespace fl {

class InputVariable : public Variable{
	protected:
		scalar _input;
	public:
		InputVariable(const std::string& name = "");
		~InputVariable();

		void setInput(scalar input);
		scalar getInput() const;

		std::string toString() const;
};

} /* namespace fl */
#endif /* FL_INPUTVARIABLE_H_ */
