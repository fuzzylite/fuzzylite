/*
fuzzylite (R), a fuzzy logic control library in C++.

Copyright (C) 2010-2024 FuzzyLite Limited. All rights reserved.
Author: Juan Rada-Vilela, PhD <jcrada@fuzzylite.com>.

This file is part of fuzzylite.

fuzzylite is free software: you can redistribute it and/or modify it under
the terms of the FuzzyLite License included with the software.

You should have received a copy of the FuzzyLite License along with
fuzzylite. If not, see <https://github.com/fuzzylite/fuzzylite/>.

fuzzylite is a registered trademark of FuzzyLite Limited.
*/

#include "fuzzylite/variable/OutputVariable.h"

#include "fuzzylite/imex/FllExporter.h"

namespace fuzzylite {

    OutputVariable::OutputVariable(const std::string& name, scalar minimum, scalar maximum) :
        Variable(name, minimum, maximum),
        _fuzzyOutput(new Aggregated(name, minimum, maximum)),
        _previousValue(fl::nan),
        _defaultValue(fl::nan),
        _lockPreviousValue(false) {}

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

    OutputVariable::~OutputVariable() {}

    void OutputVariable::copyFrom(const OutputVariable& other) {
        _fuzzyOutput.reset(other._fuzzyOutput->clone());
        if (other._defuzzifier.get())
            _defuzzifier.reset(other._defuzzifier->clone());
        _previousValue = other._previousValue;
        _defaultValue = other._defaultValue;
        _lockPreviousValue = other._lockPreviousValue;
    }

    void OutputVariable::setName(const std::string& name) {
        Variable::setName(name);
        _fuzzyOutput->setName(name);
    }

    Aggregated* OutputVariable::fuzzyOutput() const {
        return this->_fuzzyOutput.get();
    }

    void OutputVariable::setMinimum(scalar minimum) {
        Variable::setMinimum(minimum);
        _fuzzyOutput->setMinimum(minimum);
    }

    void OutputVariable::setMaximum(scalar maximum) {
        Variable::setMaximum(maximum);
        _fuzzyOutput->setMaximum(maximum);
    }

    void OutputVariable::setDefuzzifier(Defuzzifier* defuzzifier) {
        this->_defuzzifier.reset(defuzzifier);
    }

    Defuzzifier* OutputVariable::getDefuzzifier() const {
        return this->_defuzzifier.get();
    }

    void OutputVariable::setAggregation(SNorm* aggregation) {
        this->_fuzzyOutput->setAggregation(aggregation);
    }

    SNorm* OutputVariable::getAggregation() const {
        return this->_fuzzyOutput->getAggregation();
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

    void OutputVariable::setLockPreviousValue(bool lockPreviousValue) {
        this->_lockPreviousValue = lockPreviousValue;
    }

    bool OutputVariable::isLockPreviousValue() const {
        return this->_lockPreviousValue;
    }

    Variable::Type OutputVariable::type() const {
        return Variable::Output;
    }

    void OutputVariable::defuzzify() {
        if (not _enabled)
            return;

        if (Op::isFinite(_value))
            _previousValue = _value;

        std::string exception;
        scalar result = fl::nan;
        bool isValid = not _fuzzyOutput->isEmpty();
        if (isValid) {
            /* Checks whether the variable can be defuzzified without exceptions.
             * If it cannot be defuzzified, be that due to a missing defuzzifier
             * or aggregation operator, the expected behaviour is to leave the
             * variable in a state that reflects an invalid defuzzification,
             * that is, apply logic of default values and previous values.*/
            isValid = false;
            if (_defuzzifier.get()) {
                try {
                    result = _defuzzifier->defuzzify(_fuzzyOutput.get(), _minimum, _maximum);
                    isValid = true;
                } catch (std::exception& ex) { exception = ex.what(); }
            } else {
                exception = "[defuzzifier error] "
                            "defuzzifier needed to defuzzify output variable <"
                            + getName() + ">";
            }
        }

        if (not isValid) {
            // if a previous defuzzification was successfully performed and
            // and the output value is supposed not to change when the output is empty
            if (_lockPreviousValue and not Op::isNaN(_previousValue))
                result = _previousValue;
            else
                result = _defaultValue;
        }

        setValue(result);

        if (not exception.empty())
            throw Exception(exception, FL_AT);
    }

    std::string OutputVariable::fuzzyOutputValue() const {
        std::ostringstream ss;
        if (not _terms.empty()) {
            Term* first = _terms.front();
            ss << Op::str(fuzzyOutput()->activationDegree(first)) << "/" << first->getName();
        }
        for (std::size_t i = 1; i < _terms.size(); ++i) {
            scalar degree = fuzzyOutput()->activationDegree(_terms.at(i));
            if (Op::isNaN(degree) or Op::isGE(degree, 0.0))
                ss << " + " << Op::str(degree);
            else
                ss << " - " << Op::str(std::abs(degree));
            ss << "/" << terms().at(i)->getName();
        }
        return ss.str();
    }

    void OutputVariable::clear() {
        fuzzyOutput()->clear();
        setValue(fl::nan);
        setPreviousValue(fl::nan);
    }

    std::string OutputVariable::toString() const {
        return FllExporter().toString(this);
    }

    OutputVariable* OutputVariable::clone() const {
        return new OutputVariable(*this);
    }

}
