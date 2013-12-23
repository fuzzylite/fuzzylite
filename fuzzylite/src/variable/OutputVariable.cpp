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
#include "fl/imex/FllExporter.h"

#include <sstream>

namespace fl {

    OutputVariable::OutputVariable(const std::string& name,
            scalar minimum, scalar maximum)
    : Variable(name, minimum, maximum),
    _fuzzyOutput(new Accumulated("fuzzyOutput", minimum, maximum)),
    _defuzzifier(NULL), _defaultValue(fl::nan),
    _lastValidOutput(fl::nan),
    _lockOutputRange(false),
    _lockValidOutput(false) {
    }

    OutputVariable::~OutputVariable() {
        delete _fuzzyOutput;
    }

    Accumulated* OutputVariable::fuzzyOutput() const {
        return this->_fuzzyOutput;
    }

    void OutputVariable::setMinimum(scalar minimum) {
        Variable::setMinimum(minimum);
        this->_fuzzyOutput->setMinimum(minimum);
    }

    void OutputVariable::setMaximum(scalar maximum) {
        Variable::setMaximum(maximum);
        this->_fuzzyOutput->setMaximum(maximum);
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

    void OutputVariable::setLastValidOutput(scalar lastValidOutput) {
        this->_lastValidOutput = lastValidOutput;
    }

    scalar OutputVariable::getLastValidOutput() const {
        return this->_lastValidOutput;
    }

    void OutputVariable::setLockOutputRange(bool lock) {
        this->_lockOutputRange = lock;
    }

    bool OutputVariable::isLockingOutputRange() const {
        return this->_lockOutputRange;
    }

    void OutputVariable::setLockValidOutput(bool lock) {
        this->_lockValidOutput = lock;
    }

    bool OutputVariable::isLockingValidOutput() const {
        return this->_lockValidOutput;
    }

    scalar OutputVariable::defuzzify() {
        scalar result = fl::nan;
        bool isValid = this->_enabled and not this->_fuzzyOutput->isEmpty();
        if (isValid) {
            result = this->_defuzzifier->defuzzify(this->_fuzzyOutput, _minimum, _maximum);
        } else {
            //if a previous defuzzification was successfully performed and
            //and the output is supposed to not change when the output is empty
            if (_lockValidOutput and not Op::isNan(_lastValidOutput)) {
                result = _lastValidOutput;
            } else {
                result = _defaultValue;
            }
        }

        if (_lockOutputRange) {
            if (Op::isLt(result, _minimum)) result = _minimum;
            if (Op::isGt(result, _maximum)) result = _maximum;
        }

        if (_lockValidOutput and isValid) _lastValidOutput = result;
        return result;
    }

    scalar OutputVariable::defuzzifyNoLocks() const {
        scalar result = fl::nan;
        bool isValid = this->_enabled and not this->_fuzzyOutput->isEmpty();
        if (isValid) {
            result = this->_defuzzifier->defuzzify(_fuzzyOutput, _minimum, _maximum);
        } else {
            result = _defaultValue;
        }
        return result;
    }

    std::string OutputVariable::toString() const {
        return FllExporter("", "; ").toString(this);
    }

}
