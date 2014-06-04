// #BEGIN_LICENSE
// fuzzylite: a fuzzy logic control library in C++
// Copyright (C) 2014  Juan Rada-Vilela
// 
// This file is part of fuzzylite.
//
// fuzzylite is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// fuzzylite is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with fuzzylite.  If not, see <http://www.gnu.org/licenses/>.
// #END_LICENSE

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
#include "fl/term/Activated.h"
#include "fl/norm/SNorm.h"

#include <sstream>

namespace fl {

    OutputVariable::OutputVariable(const std::string& name,
            scalar minimum, scalar maximum)
    : Variable(name, minimum, maximum),
    _fuzzyOutput(new Accumulated(name, minimum, maximum)), _outputValue(fl::nan),
    _previousOutputValue(fl::nan), _defaultValue(fl::nan),
    _lockOutputValueInRange(false), _lockPreviousOutputValue(false) {
    }

    OutputVariable::OutputVariable(const OutputVariable& copy) : Variable(copy) {
        copyFrom(copy);
    }

    OutputVariable& OutputVariable::operator =(const OutputVariable& rhs) {
        if (this == &rhs) return *this;
        delete _fuzzyOutput;
        _defuzzifier.reset(NULL);

        Variable::operator =(rhs);
        copyFrom(rhs);
        return *this;
    }

    OutputVariable::~OutputVariable() {
        delete _fuzzyOutput;
    }

    void OutputVariable::copyFrom(const OutputVariable& rhs) {
        _fuzzyOutput = rhs._fuzzyOutput->clone();
        if (rhs._defuzzifier.get()) _defuzzifier.reset(rhs._defuzzifier->clone());
        _outputValue = rhs._outputValue;
        _previousOutputValue = rhs._previousOutputValue;
        _defaultValue = rhs._defaultValue;
        _lockOutputValueInRange = rhs._lockOutputValueInRange;
        _lockPreviousOutputValue = rhs._lockPreviousOutputValue;
    }

    void OutputVariable::setName(const std::string& name) {
        Variable::setName(name);
        this->_fuzzyOutput->setName(name);
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
        this->_defuzzifier.reset(defuzzifier);
    }

    Defuzzifier* OutputVariable::getDefuzzifier() const {
        return this->_defuzzifier.get();
    }

    void OutputVariable::setOutputValue(scalar outputValue) {
        this->_outputValue = outputValue;
    }

    scalar OutputVariable::getOutputValue() const {
        return this->_outputValue;
    }

    void OutputVariable::setPreviousOutputValue(scalar previousOutputValue) {
        this->_previousOutputValue = previousOutputValue;
    }

    scalar OutputVariable::getPreviousOutputValue() const {
        return this->_previousOutputValue;
    }

    void OutputVariable::setDefaultValue(scalar defaultValue) {
        this->_defaultValue = defaultValue;
    }

    scalar OutputVariable::getDefaultValue() const {
        return this->_defaultValue;
    }

    void OutputVariable::setLockOutputValueInRange(bool lockOutputValueInRange) {
        this->_lockOutputValueInRange = lockOutputValueInRange;
    }

    bool OutputVariable::isLockedOutputValueInRange() const {
        return this->_lockOutputValueInRange;
    }

    void OutputVariable::setLockPreviousOutputValue(bool lockPreviousOutputValue) {
        this->_lockPreviousOutputValue = lockPreviousOutputValue;
    }

    bool OutputVariable::isLockedPreviousOutputValue() const {
        return this->_lockPreviousOutputValue;
    }

    void OutputVariable::defuzzify() {
        if (fl::Op::isFinite(this->_outputValue)) {
            this->_previousOutputValue = this->_outputValue;
        }

        scalar result = fl::nan;
        bool isValid = this->_enabled and not this->_fuzzyOutput->isEmpty();
        if (isValid) {
            result = this->_defuzzifier->defuzzify(this->_fuzzyOutput, _minimum, _maximum);
        } else {
            //if a previous defuzzification was successfully performed and
            //and the output value is supposed not to change when the output is empty
            if (_lockPreviousOutputValue and not Op::isNaN(_previousOutputValue)) {
                result = _previousOutputValue;
            } else {
                result = _defaultValue;
            }
        }

        if (_lockOutputValueInRange) {
            result = fl::Op::bound(result, _minimum, _maximum);
        }

        this->_outputValue = result;
    }

    std::string OutputVariable::fuzzyOutputValue() const {
        std::ostringstream ss;
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            scalar degree = 0.0;
            for (std::size_t j = 0; j < fuzzyOutput()->terms().size(); ++j) {
                const Activated* activated = dynamic_cast<const Activated*> (fuzzyOutput()->getTerm(j));
                if (activated and activated->getTerm() == _terms.at(i)) {
                    degree += activated->getDegree();
                }
            }

            if (i == 0) {
                ss << fl::Op::str(degree);
            } else {
                if (fl::Op::isNaN(degree) or fl::Op::isGE(degree, 0.0))
                    ss << " + " << fl::Op::str(degree);
                else
                    ss << " - " << fl::Op::str(std::fabs(degree));
            }
            ss << "/" << _terms.at(i)->getName();
        }
        return ss.str();
    }

    void OutputVariable::clear() {
        fuzzyOutput()->clear();
        setPreviousOutputValue(fl::nan);
        setOutputValue(fl::nan);
    }

    std::string OutputVariable::toString() const {
        return FllExporter().toString(this);
    }

}
