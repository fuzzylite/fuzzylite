/*
fuzzylite (R), a fuzzy logic control library in C++.

Copyright (C) 2010-2024 FuzzyLite Limited. All rights reserved.
Author: Juan Rada-Vilela, PhD <jcrada@fuzzylite.com>.

This file is part of fuzzylite.

fuzzylite is free software: you can redistribute it and/or modify it under
the terms of the FuzzyLite License included with the software.

You should have received a copy of the FuzzyLite License along with
fuzzylite. If not, see <https://github.com/fuzzylite/fuzzylite/>.

fuzzylite is a registered trademark of FuzzyLite Limited.
*/

#include "fuzzylite/factory/FactoryManager.h"

namespace fuzzylite {

    FactoryManager* FactoryManager::instance() {
        // TODO: enable thread_local, compile library with -ftls-model=initial-exec?
        static FL_ITHREAD_LOCAL FactoryManager _instance;
        return &_instance;
    }

    FactoryManager::FactoryManager() :
        _tnorm(new TNormFactory),
        _snorm(new SNormFactory),
        _activation(new ActivationFactory),
        _defuzzifier(new DefuzzifierFactory),
        _term(new TermFactory),
        _hedge(new HedgeFactory),
        _function(new FunctionFactory) {}

    FactoryManager::FactoryManager(
        TNormFactory* tnorm,
        SNormFactory* snorm,
        ActivationFactory* activation,
        DefuzzifierFactory* defuzzifier,
        TermFactory* term,
        HedgeFactory* hedge,
        FunctionFactory* function
    ) :
        _tnorm(tnorm),
        _snorm(snorm),
        _activation(activation),
        _defuzzifier(defuzzifier),
        _term(term),
        _hedge(hedge),
        _function(function) {}

    FactoryManager::FactoryManager(const FactoryManager& other) {
        copyFrom(other);
    }

    FactoryManager& FactoryManager::operator=(const FactoryManager& other) {
        copyFrom(other);
        return *this;
    }

    FactoryManager::~FactoryManager() {}

    void FactoryManager::copyFrom(const FactoryManager& other) {
        if (this != &other) {
            _tnorm.reset(other.tnorm() ? other.tnorm()->clone() : fl::null);
            _snorm.reset(other.snorm() ? other.snorm()->clone() : fl::null);
            _activation.reset(other.activation() ? other.activation()->clone() : fl::null);
            _defuzzifier.reset(other.defuzzifier() ? other.defuzzifier()->clone() : fl::null);
            _term.reset(other.term() ? other.term()->clone() : fl::null);
            _hedge.reset(other.hedge() ? other.hedge()->clone() : fl::null);
            _function.reset(other.function() ? other.function()->clone() : fl::null);
        }
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

    void FactoryManager::setActivation(ActivationFactory* activation) {
        this->_activation.reset(activation);
    }

    ActivationFactory* FactoryManager::activation() const {
        return this->_activation.get();
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
        this->_hedge.reset(hedge);
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

    FactoryManager* FactoryManager::clone() const {
        return new FactoryManager(*this);
    }

}
