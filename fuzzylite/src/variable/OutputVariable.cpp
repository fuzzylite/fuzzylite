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

    OutputVariable::OutputVariable(const std::string& name)
            : Variable(name), _defuzzifier(NULL), _output(new Accumulated("output")),
              _defaultValue(std::numeric_limits<scalar>::quiet_NaN()),
              _unchangedValue(std::numeric_limits<scalar>::quiet_NaN()) {
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

    void OutputVariable::setUnchangedValue(scalar previousValue) {
        this->_unchangedValue = previousValue;
    }

    scalar OutputVariable::getUnchangedValue() const {
        return this->_unchangedValue;
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
            //nan in defaultValue is assumed as NoChange (NC)
            if (Op::IsNan(this->_defaultValue)) {
                return this->_unchangedValue;
            }
            return this->_defaultValue;
        }
        this->_unchangedValue = this->_defuzzifier->defuzzify(this->_output);
        return this->_unchangedValue;
    }

    std::string OutputVariable::toString() const {
        std::stringstream ss;
        ss << "OutputVariable(" << _name << ")";
        return ss.str();
    }

} /* namespace fl */
