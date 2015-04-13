/*
 Author: Juan Rada-Vilela, Ph.D.
 Copyright © 2010-2015 FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#include "fl/variable/OutputVariable.h"

#include "fl/defuzzifier/Defuzzifier.h"
#include "fl/imex/FllExporter.h"
#include "fl/norm/SNorm.h"
#include "fl/term/Accumulated.h"
#include "fl/term/Activated.h"

namespace fl {

    OutputVariable::OutputVariable(const std::string& name,
            scalar minimum, scalar maximum)
    : Variable(name, minimum, maximum),
    _fuzzyOutput(new Accumulated(name, minimum, maximum)),
    _previousValue(fl::nan), _defaultValue(fl::nan),
    _lockValueInRange(false), _lockPreviousValue(false) {
    }

    OutputVariable::OutputVariable(const OutputVariable& other) : Variable(other) {
        copyFrom(other);
    }

    OutputVariable& OutputVariable::operator=(const OutputVariable& other) {
        if (this != &other) {
            _fuzzyOutput.reset(fl::null);
            _defuzzifier.reset(fl::null);

            Variable::operator=(other);
            copyFrom(other);
        }
        return *this;
    }

    OutputVariable::~OutputVariable() {
    }

    void OutputVariable::copyFrom(const OutputVariable& other) {
        _fuzzyOutput.reset(other._fuzzyOutput->clone());
        if (other._defuzzifier.get()) _defuzzifier.reset(other._defuzzifier->clone());
        _previousValue = other._previousValue;
        _defaultValue = other._defaultValue;
        _lockValueInRange = other._lockValueInRange;
        _lockPreviousValue = other._lockPreviousValue;
    }

    void OutputVariable::setName(const std::string& name) {
        Variable::setName(name);
        this->_fuzzyOutput->setName(name);
    }

    Accumulated* OutputVariable::fuzzyOutput() const {
        return this->_fuzzyOutput.get();
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

    void OutputVariable::setPreviousValue(scalar previousOutputValue) {
        this->_previousValue = previousOutputValue;
    }

    scalar OutputVariable::getPreviousValue() const {
        return this->_previousValue;
    }

    void OutputVariable::setDefaultValue(scalar defaultValue) {
        this->_defaultValue = defaultValue;
    }

    scalar OutputVariable::getDefaultValue() const {
        return this->_defaultValue;
    }

    void OutputVariable::setLockValueInRange(bool lockValueInRange) {
        this->_lockValueInRange = lockValueInRange;
    }

    bool OutputVariable::isLockValueInRange() const {
        return this->_lockValueInRange;
    }

    void OutputVariable::setLockPreviousValue(bool lockPreviousValue) {
        this->_lockPreviousValue = lockPreviousValue;
    }

    bool OutputVariable::isLockPreviousValue() const {
        return this->_lockPreviousValue;
    }

    void OutputVariable::defuzzify() {
        if (fl::Op::isFinite(this->_value)) {
            this->_previousValue = this->_value;
        }

        scalar result = fl::nan;
        bool isValid = this->_enabled and not this->_fuzzyOutput->isEmpty();
        if (isValid) {
            if (not _defuzzifier.get()) {
                throw fl::Exception("[defuzzifier error] "
                        "defuzzifier needed to defuzzify output variable <" + _name + ">", FL_AT);
            }
            result = this->_defuzzifier->defuzzify(this->_fuzzyOutput.get(), _minimum, _maximum);
        } else {
            //if a previous defuzzification was successfully performed and
            //and the output value is supposed not to change when the output is empty
            if (_lockPreviousValue and not Op::isNaN(_previousValue)) {
                result = _previousValue;
            } else {
                result = _defaultValue;
            }
        }

        if (_lockValueInRange) {
            result = fl::Op::bound(result, _minimum, _maximum);
        }

        this->_value = result;
    }

    std::string OutputVariable::fuzzyOutputValue() const {
        std::ostringstream ss;
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            scalar degree = _fuzzyOutput->activationDegree(_terms.at(i));
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
        _fuzzyOutput->clear();
        _value = fl::nan;
        _previousValue = fl::nan;
    }

    std::string OutputVariable::toString() const {
        return FllExporter().toString(this);
    }

}
