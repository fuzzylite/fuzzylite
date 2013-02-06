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
 * OutputVariable.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "fl/variable/OutputVariable.h"

#include "fl/term/Accumulated.h"
#include "fl/defuzzifier/Defuzzifier.h"

#include <sstream>

namespace fl {

    OutputVariable::OutputVariable(const std::string& name,
            scalar minimum, scalar maximum)
    : Variable(name, minimum, maximum),
    _output(new Accumulated("output", minimum, maximum)),
    _defuzzifier(NULL), _defaultValue(std::numeric_limits<scalar>::quiet_NaN()),
    _defuzzifiedValue(std::numeric_limits<scalar>::quiet_NaN()),
    _lockDefuzzifiedValue(true) {
    }

    OutputVariable::~OutputVariable() {
        delete _output;
    }

    Accumulated* OutputVariable::output() const {
        return this->_output;
    }

    void OutputVariable::setMinimum(scalar minimum) {
        Variable::setMinimum(minimum);
        this->_output->setMinimum(minimum);
    }

    void OutputVariable::setMaximum(scalar maximum) {
        Variable::setMaximum(maximum);
        this->_output->setMaximum(maximum);
    }

    void OutputVariable::setDefuzzifier(Defuzzifier* defuzzifier) {
        if (this->_defuzzifier) delete this->_defuzzifier;
        this->_defuzzifier = defuzzifier;
    }

    Defuzzifier* OutputVariable::getDefuzzifier() const {
        return this->_defuzzifier;
    }

    void OutputVariable::setDefaultValue(scalar defaultValue) {
        this->_defaultValue = defaultValue;
    }

    scalar OutputVariable::getDefaultValue() const {
        return this->_defaultValue;
    }

    void OutputVariable::setDefuzzifiedValue(scalar defuzzifiedValue) {
        this->_defuzzifiedValue = defuzzifiedValue;
    }

    scalar OutputVariable::getDefuzzifiedValue() const {
        return this->_defuzzifiedValue;
    }

    void OutputVariable::setLockDefuzzifiedValue(bool lock) {
        this->_lockDefuzzifiedValue = lock;
    }

    bool OutputVariable::lockDefuzzifiedValue() const {
        return this->_lockDefuzzifiedValue;
    }

    scalar OutputVariable::defuzzify() {
       scalar result = defuzzifyIgnoreLock();
       if (_lockDefuzzifiedValue) _defuzzifiedValue = result;
       return result;
    }

    scalar OutputVariable::defuzzifyIgnoreLock() const {
         if (this->_output->isEmpty()) {
            //if a previous defuzzification was successfully performed and
            //and the output is supposed to not change when the output is empty
            if (_lockDefuzzifiedValue and not Op::isNan(_defuzzifiedValue))
                return _defuzzifiedValue;
            return _defaultValue;
        }
        scalar result = this->_defuzzifier->defuzzify(this->_output, _minimum, _maximum);

        if (Op::isLt(result, _minimum)) result = _minimum;
        if (Op::isGt(result, _maximum)) result = _maximum;

        return result;
    }

}
