/*
 * OutputVariable.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "fl/variable/OutputVariable.h"

#include "fl/term/Accumulated.h"
#include "fl/defuzzifier/Defuzzifier.h"

#include "fl/engine/Configuration.h"

#include <sstream>

namespace fl {

    OutputVariable::OutputVariable(const std::string& name, scalar defaultValue,
            bool lockDefuzzifiedValue)
            : Variable(name), _output(new Accumulated("output")), _defuzzifier(NULL),
                    _defaultValue(defaultValue),
                    _defuzzifiedValue(std::numeric_limits<scalar>::quiet_NaN()),
                    _lockDefuzzifiedValue(lockDefuzzifiedValue),
                    _minimumOutputRange(-std::numeric_limits<scalar>::infinity()),
                    _maximumOutputRange(std::numeric_limits<scalar>::infinity()) {
    }

    OutputVariable::~OutputVariable() {
        delete _output;
    }

    void OutputVariable::configure(Configuration* config) {
        this->_defuzzifier = config->getDefuzzifier();
        this->_output->setAccumulation(config->getAccumulation());
    }

    Accumulated* OutputVariable::output() const {
        return this->_output;
    }

    void OutputVariable::setDefuzzifier(Defuzzifier* defuzzifier) {
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

    void OutputVariable::setMininumOutputRange(scalar minimum) {
        this->_minimumOutputRange = minimum;
    }

    scalar OutputVariable::getMinimumOutputRange() const {
        return this->_minimumOutputRange;
    }

    void OutputVariable::setMaximumOutputRange(scalar maximum) {
        this->_maximumOutputRange = maximum;
    }

    scalar OutputVariable::getMaximumOutputRange() const {
        return this->_maximumOutputRange;
    }

    scalar OutputVariable::defuzzify() {
        if (this->_output->isEmpty()) {
            //if a previous defuzzification was successfully performed and
            //and the output is supposed to not change when the output is empty
            if (_lockDefuzzifiedValue and not Op::IsNan(_defuzzifiedValue))
                return _defuzzifiedValue;
            return _defaultValue;
        }
        scalar result = this->_defuzzifier->defuzzify(this->_output);

        if (Op::IsLt(result, _minimumOutputRange)) result = _minimumOutputRange;
        if (Op::IsGt(result, _maximumOutputRange)) result = _maximumOutputRange;

        if (_lockDefuzzifiedValue) _defuzzifiedValue = result;

        return result;
    }

    scalar OutputVariable::defuzzifyIgnoreLock() const {
        //Same as defuzzify, only the defuzzified value is not stored.
        //if _lockDefuzzifiedValue is false, this method is the same as defuzzify.
        if (this->_output->isEmpty()) {
            //if a previous defuzzification was successfully performed and
            //and the output is supposed to not change when the output is empty
            if (_lockDefuzzifiedValue and not Op::IsNan(_defuzzifiedValue))
                return _defuzzifiedValue;
            return _defaultValue;
        }
        scalar result = this->_defuzzifier->defuzzify(this->_output);

        if (Op::IsLt(result, _minimumOutputRange)) result = _minimumOutputRange;
        if (Op::IsGt(result, _maximumOutputRange)) result = _maximumOutputRange;

//        if (_lockDefuzzifiedValue) _defuzzifiedValue = result;

        return result;
    }

} /* namespace fl */
