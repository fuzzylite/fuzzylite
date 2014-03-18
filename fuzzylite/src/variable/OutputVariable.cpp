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
    _fuzzyOutput(new Accumulated(name, minimum, maximum)), _defuzzifier(NULL),
    _outputValue(fl::nan), _lastValidOutputValue(fl::nan), _defaultValue(fl::nan),
    _lockOutputRange(false), _lockValidOutput(false) {
    }

    OutputVariable::~OutputVariable() {
        delete _fuzzyOutput;
        if (_defuzzifier) delete _defuzzifier;
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
        this->_defuzzifier = defuzzifier;
    }

    Defuzzifier* OutputVariable::getDefuzzifier() const {
        return this->_defuzzifier;
    }

    void OutputVariable::setOutputValue(scalar outputValue) {
        this->_outputValue = outputValue;
    }

    scalar OutputVariable::getOutputValue() const {
        return this->_outputValue;
    }

    void OutputVariable::setLastValidOutputValue(scalar defuzzifiedValue) {
        this->_lastValidOutputValue = defuzzifiedValue;
    }

    scalar OutputVariable::getLastValidOutputValue() const {
        return this->_lastValidOutputValue;
    }

    void OutputVariable::setDefaultValue(scalar defaultValue) {
        this->_defaultValue = defaultValue;
    }

    scalar OutputVariable::getDefaultValue() const {
        return this->_defaultValue;
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

    void OutputVariable::defuzzify() {
        scalar result = fl::nan;
        bool isValid = this->_enabled and not this->_fuzzyOutput->isEmpty();
        if (isValid) {
            result = this->_defuzzifier->defuzzify(this->_fuzzyOutput, _minimum, _maximum);
        } else {
            //if a previous defuzzification was successfully performed and
            //and the output is supposed to not change when the output is empty
            if (_lockValidOutput and not Op::isNaN(_lastValidOutputValue)) {
                result = _lastValidOutputValue;
            } else {
                result = _defaultValue;
            }
        }

        if (_lockOutputRange) {
            if (Op::isLt(result, _minimum)) result = _minimum;
            if (Op::isGt(result, _maximum)) result = _maximum;
        }

        if (isValid) _lastValidOutputValue = result;

        _outputValue = result;
    }

    std::string OutputVariable::fuzzyOutputValue() const {
        std::ostringstream ss;
        if (not fuzzyOutput()->getAccumulation()) {
            for (std::size_t i = 0; i < _terms.size(); ++i) {
                ss << fl::Op::str(fl::nan) << "/" << _terms.at(i)->getName();
                if (i + 1 < _terms.size()) ss << " + ";
            }
            return ss.str();
        }

        for (std::size_t i = 0; i < _terms.size(); ++i) {
            scalar degree = 0.0;
            for (std::size_t j = 0; j < fuzzyOutput()->terms().size(); ++j) {
                const Activated* activated = dynamic_cast<const Activated*> (fuzzyOutput()->getTerm(j));
                if (activated and activated->getTerm() == _terms.at(i)) {
                    degree = fuzzyOutput()->getAccumulation()->compute(
                            degree, activated->getDegree());
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

    std::string OutputVariable::toString() const {
        return FllExporter("", "; ").toString(this);
    }

    void OutputVariable::clear() {
        Variable::clear();
        fuzzyOutput()->clear();
        if (fuzzyOutput()->getAccumulation()){
            delete fuzzyOutput()->getAccumulation();
            fuzzyOutput()->setAccumulation(NULL);
        }
        setDefaultValue(fl::nan);
        if (_defuzzifier){
            delete _defuzzifier;
            setDefuzzifier(NULL);
        }
        setLastValidOutputValue(fl::nan);
        setLockOutputRange(false);
        setLockValidOutput(false);
        setOutputValue(fl::nan);
    }


}
