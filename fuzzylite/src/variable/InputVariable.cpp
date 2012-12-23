/*
 * InputVariable.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "fl/variable/InputVariable.h"

#include <sstream>

namespace fl {

    InputVariable::InputVariable(const std::string& name, scalar minimum, scalar maximum)
    : Variable(name, minimum, maximum), _input(std::numeric_limits<scalar>::quiet_NaN()) {
    }

    InputVariable::~InputVariable() {
    }

    void InputVariable::setInput(scalar input) {
        this->_input = input;
    }

    scalar InputVariable::getInput() const {
        return this->_input;
    }


}
