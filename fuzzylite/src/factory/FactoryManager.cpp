/*   Copyright 2013 Juan Rada-Vilela

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

/* 
 * File:   Factory.cpp
 * Author: jcrada
 * 
 * Created on 8 January 2013, 11:10 PM
 */

#include "fl/factory/FactoryManager.h"
#include "fl/factory/DefuzzifierFactory.h"
#include "fl/factory/SNormFactory.h"
#include "fl/factory/TNormFactory.h"
#include "fl/factory/TermFactory.h"
#include "fl/factory/HedgeFactory.h"

namespace fl {

    FactoryManager* FactoryManager::_instance = NULL;

    FactoryManager* FactoryManager::instance() {
        if (not _instance) {
            _instance = new FactoryManager;
            _instance->setTnorm(new TNormFactory);
            _instance->setSnorm(new SNormFactory);
            _instance->setDefuzzifier(new DefuzzifierFactory);
            _instance->setTerm(new TermFactory);
            _instance->setHedge(new HedgeFactory);
        }
        return _instance;
    }

    FactoryManager::FactoryManager() :
    _tnorm(NULL), _snorm(NULL), _defuzzifier(NULL), _term(NULL), _hedge(NULL) { }

    FactoryManager::~FactoryManager() {
        if (_hedge) delete _hedge;
        if (_term) delete _term;
        if (_defuzzifier) delete _defuzzifier;
        if (_snorm) delete _snorm;
        if (_tnorm) delete _tnorm;
    }

    void FactoryManager::setTnorm(TNormFactory* tnorm) {
        if (this->_tnorm) delete this->_tnorm;
        this->_tnorm = tnorm;
    }

    TNormFactory* FactoryManager::tnorm() const {
        return this->_tnorm;
    }

    void FactoryManager::setSnorm(SNormFactory* snorm) {
        if (this->_snorm) delete this->_snorm;
        this->_snorm = snorm;
    }

    SNormFactory* FactoryManager::snorm() const {
        return this->_snorm;
    }

    void FactoryManager::setDefuzzifier(DefuzzifierFactory* defuzzifier) {
        if (this->_defuzzifier) delete this->_defuzzifier;
        this->_defuzzifier = defuzzifier;
    }

    DefuzzifierFactory* FactoryManager::defuzzifier() const {
        return this->_defuzzifier;
    }

    void FactoryManager::setTerm(TermFactory* term) {
        if (this->_term) delete this->_term;
        this->_term = term;
    }

    TermFactory* FactoryManager::term() const {
        return this->_term;
    }

    void FactoryManager::setHedge(HedgeFactory* hedge) {
        if (this->_hedge) delete this->_hedge;
        this->_hedge = hedge;
    }

    HedgeFactory* FactoryManager::hedge() const {
        return this->_hedge;
    }

}
