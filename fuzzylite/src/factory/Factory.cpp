/* 
 * File:   Factory.cpp
 * Author: jcrada
 * 
 * Created on 8 January 2013, 11:10 PM
 */

#include "fl/factory/Factory.h"
#include "fl/factory/DefuzzifierFactory.h"
#include "fl/factory/SNormFactory.h"
#include "fl/factory/TNormFactory.h"
#include "fl/factory/TermFactory.h"

namespace fl {

    Factory* Factory::_instance = NULL;

    Factory* Factory::instance() {
        if (not _instance) {
            _instance = new Factory;
            _instance->setTnorm(new TNormFactory);
            _instance->setSnorm(new SNormFactory);
            _instance->setDefuzzifier(new DefuzzifierFactory);
            _instance->setTerm(new TermFactory);
        }
        return _instance;
    }

    Factory::Factory() :
    _tnorm(NULL), _snorm(NULL), _defuzzifier(NULL), _term(NULL) { }

    Factory::~Factory() {
        if (_term) delete _term;
        if (_defuzzifier) delete _defuzzifier;
        if (_snorm) delete _snorm;
        if (_tnorm) delete _tnorm;
    }

    void Factory::setTnorm(TNormFactory* tnorm) {
        if (this->_tnorm) delete this->_tnorm;
        this->_tnorm = tnorm;
    }

    TNormFactory* Factory::getTnorm() const {
        return this->_tnorm;
    }

    void Factory::setSnorm(SNormFactory* snorm) {
        if (this->_snorm) delete this->_snorm;
        this->_snorm = snorm;
    }

    SNormFactory* Factory::getSnorm() const {
        return this->_snorm;
    }

    void Factory::setDefuzzifier(DefuzzifierFactory* defuzzifier) {
        if (this->_defuzzifier) delete this->_defuzzifier;
        this->_defuzzifier = defuzzifier;
    }

    DefuzzifierFactory* Factory::getDefuzzifier() const {
        return this->_defuzzifier;
    }

    void Factory::setTerm(TermFactory* term) {
        if (this->_term) delete this->_term;
        this->_term = term;
    }

    TermFactory* Factory::getTerm() const {
        return this->_term;
    }

}
