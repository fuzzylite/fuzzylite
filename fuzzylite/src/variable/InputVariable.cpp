/*
 * InputVariable.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "InputVariable.h"

#include <sstream>

namespace fl {

    InputVariable::InputVariable(const std::string& name)
            : Variable(name) {
    }

    InputVariable::~InputVariable() {
    }

    void InputVariable::setInput(scalar input) {
        this->_input = input;
    }

    scalar InputVariable::getInput() const {
        return this->_input;
    }


} /* namespace fl */
