/*   Copyright 2013 Juan Rada-Vilela

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

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
