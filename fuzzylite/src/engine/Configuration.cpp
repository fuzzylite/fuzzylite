/*
 * Configuration.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "fl/engine/Configuration.h"

#include "fl/defuzzifier/Defuzzifier.h"

#include <sstream>

namespace fl {

    Configuration::Configuration(Operator* tnorm, Operator* snorm,
            Operator* activation, Operator* accumulation, Defuzzifier* defuzzifier)
            : _tnorm(tnorm), _snorm(snorm), _activation(activation), _accumulation(accumulation),
                    _defuzzifier(defuzzifier) {
    }

    Configuration::~Configuration() {
        delete _defuzzifier;
        delete _accumulation;
        delete _activation;
        delete _snorm;
        delete _tnorm;
    }

    void Configuration::setTnorm(Operator* tnorm) {
        this->_tnorm = tnorm;
    }
    Operator* Configuration::getTnorm() const {
        return this->_tnorm;
    }
    void Configuration::setSnorm(Operator* snorm) {
        this->_snorm = snorm;
    }
    Operator* Configuration::getSnorm() const {
        return this->_snorm;
    }
    void Configuration::setActivation(Operator* activation) {
        this->_activation = activation;
    }
    Operator* Configuration::getActivation() const {
        return this->_activation;
    }
    void Configuration::setAccumulation(Operator* accumulation) {
        this->_accumulation = accumulation;
    }
    Operator* Configuration::getAccumulation() const {
        return this->_accumulation;
    }
    void Configuration::setDefuzzifier(Defuzzifier* defuzzifier) {
        this->_defuzzifier = defuzzifier;
    }
    Defuzzifier* Configuration::getDefuzzifier() const {
        return this->_defuzzifier;
    }

    std::string Configuration::toString() const {
        std::ostringstream ss;
        ss
        << "tnorm='" << _tnorm->name() << "' "
                << "snorm='" << _snorm->name() << "' "
                << "activation='" << _activation->name() << "' "
                << "accumulation='" << _accumulation->name() << "' "
                << "defuzzifier='" << _defuzzifier->name() << "' "
                ;
        return ss.str();
    }

} /* namespace fl */
