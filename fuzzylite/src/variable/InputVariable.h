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
		InputVariable(const std::string& name);
		virtual ~InputVariable();

		virtual void setInput(scalar input);
		virtual scalar getInput() const;

};

} /* namespace fl */
#endif /* FL_INPUTVARIABLE_H_ */
