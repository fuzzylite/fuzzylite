/*
 Copyright © 2010-2015 by FuzzyLite Limited.
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
#include "fl/term/Aggregated.h"
#include "fl/term/Activated.h"

namespace fl {

    OutputVariable::OutputVariable(const std::string& name,
            scalar minimum, scalar maximum)
    : Variable(name, minimum, maximum),
    _fuzzyOutput(new Aggregated(name, minimum, maximum)),
    _previousValue(fl::nan), _defaultValue(fl::nan),
    _lockPreviousValue(false) {
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
        _lockPreviousValue = other._lockPreviousValue;
    }

    void OutputVariable::setName(const std::string& name) {
        Variable::setName(name);
        fuzzyOutput()->setName(name);
    }

    Aggregated* OutputVariable::fuzzyOutput() const {
        return this->_fuzzyOutput.get();
    }

    void OutputVariable::setMinimum(scalar minimum) {
        Variable::setMinimum(minimum);
        fuzzyOutput()->setMinimum(minimum);
    }

    void OutputVariable::setMaximum(scalar maximum) {
        Variable::setMaximum(maximum);
        fuzzyOutput()->setMaximum(maximum);
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

    void OutputVariable::setLockPreviousValue(bool lockPreviousValue) {
        this->_lockPreviousValue = lockPreviousValue;
    }

    bool OutputVariable::isLockPreviousValue() const {
        return this->_lockPreviousValue;
    }

    void OutputVariable::defuzzify() {
        if (not isEnabled()) return;

        if (fl::Op::isFinite(getValue())) {
            setPreviousValue(getValue());
        }

        std::string exception;
        scalar result = fl::nan;
        bool isValid = isEnabled() and not fuzzyOutput()->isEmpty();
        if (isValid) {
            /* Checks whether the variable can be defuzzified without exceptions.
             * If it cannot be defuzzified, be that due to a missing defuzzifier  
             * or aggregation operator, the expected behaviour is to leave the 
             * variable in a state that reflects an invalid defuzzification, 
             * that is, apply logic of default values and previous values.*/
            if (getDefuzzifier()) {
                try{
                    result = getDefuzzifier()->defuzzify(fuzzyOutput(), getMinimum(), getMaximum());
                }catch(std::exception& ex){
                    exception = ex.what();
                    isValid = false;
                }
            }else{
                exception = "[defuzzifier error] "
                        "defuzzifier needed to defuzzify output variable <" + getName() + ">";
                isValid = false;
            }
        }
        
        if (not isValid){
            //if a previous defuzzification was successfully performed and
            //and the output value is supposed not to change when the output is empty
            if (isLockPreviousValue() and not Op::isNaN(getPreviousValue())) {
                result = getPreviousValue();
            } else {
                result = getDefaultValue();
            }
        }

        if (isLockValueInRange()) {
            result = fl::Op::bound(result, getMinimum(), getMaximum());
        }

        setValue(result);
        
        if (not exception.empty()){
            throw fl::Exception(exception, FL_AT);
        }
    }

    std::string OutputVariable::fuzzyOutputValue() const {
        std::ostringstream ss;
        if (not terms().empty()) {
            Term* first = terms().front();
            ss << fl::Op::str(fuzzyOutput()->activationDegree(first))
                    << "/" << first->getName();
        }
        for (std::size_t i = 1; i < terms().size(); ++i) {
            scalar degree = fuzzyOutput()->activationDegree(terms().at(i));
            if (fl::Op::isNaN(degree) or fl::Op::isGE(degree, 0.0))
                ss << " + " << fl::Op::str(degree);
            else
                ss << " - " << fl::Op::str(std::fabs(degree));
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
