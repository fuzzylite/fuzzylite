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

    Configuration::Configuration(TNorm* tnorm, SNorm* snorm,
            TNorm* activation, SNorm* accumulation, Defuzzifier* defuzzifier)
    : _tnorm(tnorm), _snorm(snorm), _activation(activation), _accumulation(accumulation),
    _defuzzifier(defuzzifier) {
    }

    Configuration::~Configuration() {
        //These will be deleted by ruleblocks, and output variables.
        delete _defuzzifier;
        delete _accumulation;
        delete _activation;
        delete _snorm;
        delete _tnorm;
    }

    void Configuration::setTnorm(TNorm* tnorm) {
        this->_tnorm = tnorm;
    }

    TNorm* Configuration::getTnorm() const {
        return this->_tnorm;
    }

    void Configuration::setSnorm(SNorm* snorm) {
        this->_snorm = snorm;
    }

    SNorm* Configuration::getSnorm() const {
        return this->_snorm;
    }

    void Configuration::setActivation(TNorm* activation) {
        this->_activation = activation;
    }

    TNorm* Configuration::getActivation() const {
        return this->_activation;
    }

    void Configuration::setAccumulation(SNorm* accumulation) {
        this->_accumulation = accumulation;
    }

    SNorm* Configuration::getAccumulation() const {
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
                << "tnorm='" << _tnorm->className() << "' "
                << "snorm='" << _snorm->className() << "' "
                << "activation='" << _activation->className() << "' "
                << "accumulation='" << _accumulation->className() << "' "
                << "defuzzifier='" << _defuzzifier->className() << "' "
                ;
        return ss.str();
    }

}
