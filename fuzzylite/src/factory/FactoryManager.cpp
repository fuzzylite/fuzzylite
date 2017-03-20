/*
 fuzzylite (R), a fuzzy logic control library in C++.
 Copyright (C) 2010-2017 FuzzyLite Limited. All rights reserved.
 Author: Juan Rada-Vilela, Ph.D. <jcrada@fuzzylite.com>

 This file is part of fuzzylite.

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite is a registered trademark of FuzzyLite Limited.
 */

#include "fl/factory/FactoryManager.h"

namespace fl {

    FactoryManager* FactoryManager::instance() {
        static FL_ITHREAD_LOCAL FactoryManager _instance;
        return &_instance;
    }

    FactoryManager::FactoryManager() :
    _tnorm(new TNormFactory), _snorm(new SNormFactory), _activation(new ActivationFactory),
    _defuzzifier(new DefuzzifierFactory), _term(new TermFactory),
    _hedge(new HedgeFactory), _function(new FunctionFactory) { }

    FactoryManager::FactoryManager(TNormFactory* tnorm, SNormFactory* snorm,
            ActivationFactory* activation, DefuzzifierFactory* defuzzifier,
            TermFactory* term, HedgeFactory* hedge, FunctionFactory* function) :
    _tnorm(tnorm), _snorm(snorm), _activation(activation),
    _defuzzifier(defuzzifier), _term(term), _hedge(hedge), _function(function) { }

    FactoryManager::FactoryManager(const FactoryManager& other)
    : _tnorm(fl::null), _snorm(fl::null), _activation(fl::null),
    _defuzzifier(fl::null), _term(fl::null), _hedge(fl::null),
    _function(fl::null) {
        if (other._tnorm.get()) this->_tnorm.reset(new TNormFactory(*other._tnorm.get()));
        if (other._snorm.get()) this->_snorm.reset(new SNormFactory(*other._snorm.get()));
        if (other._activation.get()) this->_activation.reset(new ActivationFactory(*other._activation.get()));
        if (other._defuzzifier.get()) this->_defuzzifier.reset(new DefuzzifierFactory(*other._defuzzifier.get()));
        if (other._term.get()) this->_term.reset(new TermFactory(*other._term.get()));
        if (other._hedge.get()) this->_hedge.reset(new HedgeFactory(*other._hedge.get()));
        if (other._function.get()) this->_function.reset(new FunctionFactory(*other._function.get()));
    }

    FactoryManager& FactoryManager::operator=(const FactoryManager& other) {
        if (this != &other) {
            _tnorm.reset(fl::null);
            _snorm.reset(fl::null);
            _activation.reset(fl::null);
            _defuzzifier.reset(fl::null);
            _term.reset(fl::null);
            _hedge.reset(fl::null);
            _function.reset(fl::null);

            if (other._tnorm.get()) this->_tnorm.reset(new TNormFactory(*other._tnorm.get()));
            if (other._snorm.get()) this->_snorm.reset(new SNormFactory(*other._snorm.get()));
            if (other._activation.get()) this->_activation.reset(new ActivationFactory(*other._activation.get()));
            if (other._defuzzifier.get()) this->_defuzzifier.reset(new DefuzzifierFactory(*other._defuzzifier.get()));
            if (other._term.get()) this->_term.reset(new TermFactory(*other._term.get()));
            if (other._hedge.get()) this->_hedge.reset(new HedgeFactory(*other._hedge.get()));
            if (other._function.get()) this->_function.reset(new FunctionFactory(*other._function.get()));
        }
        return *this;
    }

    FactoryManager::~FactoryManager() { }

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

}
