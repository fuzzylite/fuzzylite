/*
 * OutputVariable.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "OutputVariable.h"

#include "../term/Accumulated.h"
#include "../defuzzifier/Defuzzifier.h"

#include "../engine/Configuration.h"
#include <sstream>

namespace fl {

    OutputVariable::OutputVariable(const std::string& name, scalar defaultValue,
            bool lockDefuzzifiedValue)
            : Variable(name), _defuzzifier(NULL), _output(new Accumulated("output")),
              _defaultValue(defaultValue), _defuzzifiedValue(std::numeric_limits<scalar>::quiet_NaN()),
              _lockDefuzzifiedValue(lockDefuzzifiedValue) {
    }

    OutputVariable::~OutputVariable() {
        delete _output;
    }

    void OutputVariable::configure(Configuration* config) {
        this->_defuzzifier = config->getDefuzzifier();
        this->_output->setAccumulation(config->getAccumulation());
    }

    void OutputVariable::setDefaultValue(scalar defaultValue) {
        this->_defaultValue = defaultValue;
    }

    scalar OutputVariable::getDefaultValue() const {
        return this->_defaultValue;
    }

    void OutputVariable::setDefuzzifiedValue(scalar defuzzifiedValue){
        this->_defuzzifiedValue = defuzzifiedValue;
    }

    scalar OutputVariable::getDefuzzifiedValue() const{
        return this->_defuzzifiedValue;
    }

    void OutputVariable::setLockDefuzzifiedValue(bool lock) {
        this->_lockDefuzzifiedValue = lock;
    }

    bool OutputVariable::lockDefuzzifiedValue() const {
        return this->_lockDefuzzifiedValue;
    }

    void OutputVariable::setDefuzzifier(Defuzzifier* defuzzifier) {
        this->_defuzzifier = defuzzifier;
    }

    Defuzzifier* OutputVariable::getDefuzzifier() const {
        return this->_defuzzifier;
    }

    Accumulated* OutputVariable::output() const {
        return this->_output;
    }

    scalar OutputVariable::defuzzify() {
        if (this->_output->isEmpty()) {
            //if a previous defuzzification was successfully performed and
            //and the output is supposed to not change when the output is empty
            if (_lockDefuzzifiedValue and not Op::IsNan(_defuzzifiedValue))
                return _defuzzifiedValue;
            return _defaultValue;
        }
        this->_defuzzifiedValue = this->_defuzzifier->defuzzify(this->_output);
        return this->_defuzzifiedValue;
    }

} /* namespace fl */
