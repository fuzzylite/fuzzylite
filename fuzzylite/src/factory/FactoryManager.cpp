/*
 Author: Juan Rada-Vilela, Ph.D.
 Copyright (C) 2010-2014 FuzzyLite Limited
 All rights reserved

 This file is part of fuzzylite.

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the GNU Lesser General Public License as published by the Free
 Software Foundation, either version 3 of the License, or (at your option)
 any later version.

 fuzzylite is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with fuzzylite.  If not, see <http://www.gnu.org/licenses/>.
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
        this->_tnorm.reset(new TNormFactory(*source._tnorm.get()));
        this->_snorm.reset(new SNormFactory(*source._snorm.get()));
        this->_defuzzifier.reset(new DefuzzifierFactory(*source._defuzzifier.get()));
        this->_term.reset(new TermFactory(*source._term.get()));
        this->_hedge.reset(new HedgeFactory(*source._hedge.get()));
        this->_function.reset(new FunctionFactory(*source._function.get()));
    }

    FactoryManager& FactoryManager::operator =(const FactoryManager& rhs) {
        if (this == &rhs) return *this;

        this->_tnorm.reset(new TNormFactory(*rhs._tnorm.get()));
        this->_snorm.reset(new SNormFactory(*rhs._snorm.get()));
        this->_defuzzifier.reset(new DefuzzifierFactory(*rhs._defuzzifier.get()));
        this->_term.reset(new TermFactory(*rhs._term.get()));
        this->_hedge.reset(new HedgeFactory(*rhs._hedge.get()));
        this->_function.reset(new FunctionFactory(*rhs._function.get()));

        return *this;
    }

    FactoryManager::~FactoryManager() {
    }

    void FactoryManager::setTnorm(TNormFactory* tnorm) {
        this->_tnorm.reset(tnorm);
    }

    TNormFactory* FactoryManager::tnorm() const {
        return this->_tnorm.get();
    }

    void FactoryManager::setSnorm(SNormFactory* snorm) {
        this->_snorm.reset(snorm);
    }

    SNormFactory* FactoryManager::snorm() const {
        return this->_snorm.get();
    }

    void FactoryManager::setDefuzzifier(DefuzzifierFactory* defuzzifier) {
        this->_defuzzifier.reset(defuzzifier);
    }

    DefuzzifierFactory* FactoryManager::defuzzifier() const {
        return this->_defuzzifier.get();
    }

    void FactoryManager::setTerm(TermFactory* term) {
        this->_term.reset(term);
    }

    TermFactory* FactoryManager::term() const {
        return this->_term.get();
    }

    void FactoryManager::setHedge(HedgeFactory* hedge) {
        this->_hedge.reset( hedge);
    }

    HedgeFactory* FactoryManager::hedge() const {
        return this->_hedge.get();
    }

    void FactoryManager::setFunction(FunctionFactory* function) {
        this->_function.reset(function);
    }

    FunctionFactory* FactoryManager::function() const {
        return this->_function.get();
    }

}
