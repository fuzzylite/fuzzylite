// #BEGIN_LICENSE
// fuzzylite: a fuzzy logic control library in C++
// Copyright (C) 2014  Juan Rada-Vilela
// 
// This file is part of fuzzylite.
//
// fuzzylite is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// fuzzylite is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with fuzzylite.  If not, see <http://www.gnu.org/licenses/>.
// #END_LICENSE

/* 
 * File:   Factory.cpp
 * Author: jcrada
 * 
 * Created on 8 January 2013, 11:10 PM
 */

#include "fl/factory/FactoryManager.h"

#include "fl/factory/DefuzzifierFactory.h"
#include "fl/factory/FunctionFactory.h"
#include "fl/factory/HedgeFactory.h"
#include "fl/factory/SNormFactory.h"
#include "fl/factory/TermFactory.h"
#include "fl/factory/TNormFactory.h"

namespace fl {

    FactoryManager FactoryManager::_instance;

    FactoryManager* FactoryManager::instance() {
        return &_instance;
    }

    FactoryManager::FactoryManager() :
    _tnorm(new TNormFactory), _snorm(new SNormFactory), _defuzzifier(new DefuzzifierFactory),
    _term(new TermFactory), _hedge(new HedgeFactory), _function(new FunctionFactory) {
    }

    FactoryManager::FactoryManager(TNormFactory* tnorm, SNormFactory* snorm,
            DefuzzifierFactory* defuzzifier, TermFactory* term,
            HedgeFactory* hedge, FunctionFactory* function) :
    _tnorm(tnorm), _snorm(snorm), _defuzzifier(defuzzifier), _term(term), _hedge(hedge),
    _function(function) {
    }

    FactoryManager::FactoryManager(const FactoryManager& source)
    : _tnorm(NULL), _snorm(NULL), _defuzzifier(NULL), _term(NULL), _hedge(NULL), _function(NULL) {
        if (source._tnorm) this->_tnorm = new TNormFactory(*source._tnorm);
        if (source._snorm) this->_snorm = new SNormFactory(*source._snorm);
        if (source._defuzzifier) this->_defuzzifier = new DefuzzifierFactory(*source._defuzzifier);
        if (source._term) this->_term = new TermFactory(*source._term);
        if (source._hedge) this->_hedge = new HedgeFactory(*source._hedge);
        if (source._function) this->_function = new FunctionFactory(*source._function);
    }

    FactoryManager& FactoryManager::operator =(const FactoryManager& rhs) {
        if (this == &rhs) return *this;

        if (this->_tnorm) delete this->_tnorm;
        if (this->_snorm) delete this->_snorm;
        if (this->_defuzzifier) delete this->_defuzzifier;
        if (this->_term) delete this->_term;
        if (this->_hedge) delete this->_hedge;
        if (this->_function) delete this->_function;

        this->_tnorm = NULL;
        this->_snorm = NULL;
        this->_defuzzifier = NULL;
        this->_term = NULL;
        this->_hedge = NULL;
        this->_function = NULL;

        if (rhs._tnorm) this->_tnorm = new TNormFactory(*rhs._tnorm);
        if (rhs._snorm) this->_snorm = new SNormFactory(*rhs._snorm);
        if (rhs._defuzzifier) this->_defuzzifier = new DefuzzifierFactory(*rhs._defuzzifier);
        if (rhs._term) this->_term = new TermFactory(*rhs._term);
        if (rhs._hedge) this->_hedge = new HedgeFactory(*rhs._hedge);
        if (rhs._function) this->_function = new FunctionFactory(*rhs._function);

        return *this;
    }

    FactoryManager::~FactoryManager() {
        if (_function) delete _function;
        if (_hedge) delete _hedge;
        if (_term) delete _term;
        if (_defuzzifier) delete _defuzzifier;
        if (_snorm) delete _snorm;
        if (_tnorm) delete _tnorm;
    }

    void FactoryManager::setTnorm(TNormFactory* tnorm) {
        this->_tnorm = tnorm;
    }

    TNormFactory* FactoryManager::tnorm() const {
        return this->_tnorm;
    }

    void FactoryManager::setSnorm(SNormFactory* snorm) {
        this->_snorm = snorm;
    }

    SNormFactory* FactoryManager::snorm() const {
        return this->_snorm;
    }

    void FactoryManager::setDefuzzifier(DefuzzifierFactory* defuzzifier) {
        this->_defuzzifier = defuzzifier;
    }

    DefuzzifierFactory* FactoryManager::defuzzifier() const {
        return this->_defuzzifier;
    }

    void FactoryManager::setTerm(TermFactory* term) {
        this->_term = term;
    }

    TermFactory* FactoryManager::term() const {
        return this->_term;
    }

    void FactoryManager::setHedge(HedgeFactory* hedge) {
        this->_hedge = hedge;
    }

    HedgeFactory* FactoryManager::hedge() const {
        return this->_hedge;
    }

    void FactoryManager::setFunction(FunctionFactory* function) {
        this->_function = function;
    }

    FunctionFactory* FactoryManager::function() const {
        return this->_function;
    }

}
