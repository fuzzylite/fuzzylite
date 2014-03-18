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
 * File:   Copy.cpp
 * Author: jcrada
 *
 * Created on 18 March 2014, 2:43 PM
 */

#include "fl/Copy.h"

#include "fl/Engine.h"
#include "fl/rule/RuleBlock.h"
#include "fl/variable/InputVariable.h"
#include "fl/variable/OutputVariable.h"
#include "fl/variable/Variable.h"


namespace fl {

    Copy::Copy() {

    }

    Copy::~Copy() {

    }

    Engine* Copy::engine(const Engine* source, Engine* target, Type type) {
        if (type == SHALLOW) return shallowEngine(source, target);
        if (type == DEEP) return deepEngine(source, target);
        throw fl::Exception("[copy error] unexpected type of copy <" << type << ">", FL_AT);
    }

    Variable* Copy::variable(const Variable* source, Variable* target, Type type) {
        if (type == SHALLOW) return shallowVariable();
        if (type == DEEP) return deepVariable();
        throw fl::Exception("[copy error] unexpected type of copy <" << type << ">", FL_AT);
    }

    InputVariable* Copy::inputVariable(const InputVariable* source, InputVariable* target, Type type) {
        if (type == SHALLOW) return shallowInputVariable();
        if (type == DEEP) return deepInputVariable();
        throw fl::Exception("[copy error] unexpected type of copy <" << type << ">", FL_AT);
    }

    OutputVariable* Copy::outputVariable(const OutputVariable* source, OutputVariable* target, Type type) {
        if (type == SHALLOW) return shallowVariable();
        if (type == DEEP) return deepVariable();
        throw fl::Exception("[copy error] unexpected type of copy <" << type << ">", FL_AT);
    }

    RuleBlock* Copy::ruleBlock(const RuleBlock* source, RuleBlock* target, Type type) {
        if (type == SHALLOW) return shallowRuleBlock();
        if (type == DEEP) return deepRuleBlock();
        throw fl::Exception("[copy error] unexpected type of copy <" << type << ">", FL_AT);
    }

    Engine* Copy::shallowEngine(const Engine* source, Engine* target) {
        target->setName(source->getName());
        target->setInputVariables(source->inputVariables());
        target->setOutputVariables(source->outputVariables());
        target->setRuleBlocks(source->ruleBlocks());
        return target;
    }

    Variable* Copy::shallowVariable(const Variable* source, Variable* target) {
        target->setEnabled(source->isEnabled());
        target->setMaximum(source->getMaximum());
        target->setMinimum(source->getMinimum());
        target->setName(source->getName());
        for (std::size_t i = 0; i < source->terms().size(); ++i) {
            target->addTerm(source->terms().at(i));
        }
        return target;
    }

    InputVariable* Copy::shallowInputVariable(const InputVariable* source, InputVariable* target) {
        Copy::variable(source, target, SHALLOW);
        target->setInputValue(source->getInputValue());
        return target;
    }

    OutputVariable* Copy::shallowOutputVariable(const OutputVariable* source, OutputVariable* target) {
        Copy::variable(source, target, SHALLOW);
        target->setDefaultValue(source->getDefaultValue());
        target->setDefuzzifier(source->getDefuzzifier());
        target->setLastValidOutputValue(source->getLastValidOutputValue());
        target->setLockOutputRange(source->isLockingOutputRange());
        target->setLockValidOutput(source->isLockingValidOutput());
        target->setOutputValue(source->getOutputValue());
        target->fuzzyOutput()->setAccumulation(source->fuzzyOutput()->getAccumulation());
        return target;
    }

    RuleBlock* Copy::shallowRuleBlock(const RuleBlock* source, RuleBlock* target) {
        target->setActivation(source->getActivation());
        target->setConjunction(source->getConjunction());
        target->setDisjunction(source->getDisjunction());
        target->setEnabled(source->isEnabled());
        target->setName(source->getName());
        target->setRules(source->rules());
        return target;
    }

    Engine* Copy::deepEngine(const Engine* source, Engine* target) {
        target->setName(source->getName());
        for (std::size_t i = 0; i < source->inputVariables().size(); ++i) {
            const InputVariable* inputVariable = source->inputVariables().at(i);
            InputVariable* copy = new InputVariable;
            target->addInputVariable(deepInputVariable(inputVariable, copy));
        }

        for (std::size_t i = 0; i < source->outputVariables().size(); ++i) {
            const OutputVariable* outputVariable = source->outputVariables().at(i);
            OutputVariable* copy = new OutputVariable;
            target->addOutputVariable(deepOutputVariable(outputVariable, copy));
        }

        for (std::size_t i = 0; i < source->ruleBlocks().size(); ++i) {
            const RuleBlock* ruleBlock = source->ruleBlocks().at(i);
            RuleBlock* copy = new RuleBlock;
            target->addRuleBlock(deepRuleBlock(ruleBlock, copy));
            try {
                copy->loadRules(engine);
            } catch (std::exception& ex) {
                (void) ex; //ignore exceptions
            }
        }
        return target;
    }

    Variable* Copy::deepVariable(const Variable* source, Variable* target) {
        target->setEnabled(source->isEnabled());
        target->setMaximum(source->getMaximum());
        target->setMinimum(source->getMinimum());
        target->setName(source->getName());
        for (std::size_t i = 0; i < source->terms().size(); ++i) {
            target->addTerm(source->terms().at(i)->copy());
        }
        return target;
    }

    InputVariable* Copy::deepInputVariable(const InputVariable* source, InputVariable* target) {
        deepVariable(source, target);
        return target;
    }

    OutputVariable* Copy::deepOutputVariable(const OutputVariable* source, OutputVariable* target) {
        deepVariable(source, target);
        FactoryManager* factory = fl::FactoryManager::instance();
        if (source->getDefuzzifier()) {
            Defuzzifier* defuzzifier = factory->defuzzifier()->createInstance(
                    source->getDefuzzifier()->className());
            target->setDefuzzifier(defuzzifier);
            IntegralDefuzzifier* integralDefuzzifier =
                    dynamic_cast<IntegralDefuzzifier*> (defuzzifier);
            if (integralDefuzzifier) integralDefuzzifier->setResolution(
                    dynamic_cast<IntegralDefuzzifier*> (source->getDefuzzifier())->getResolution());
            
        }
        if (source->fuzzyOutput()->getAccumulation()){
            SNorm* accumulation = factory->snorm()->createInstance(
                    source->fuzzyOutput()->getAccumulation()->className());
            target->fuzzyOutput()->setAccumulation(accumulation);
        }
        return target;
    }

    RuleBlock* Copy::deepRuleBlock(const RuleBlock* source, RuleBlock* target) {
        target->setEnabled(source->isEnabled());
        target->setName(source->getName());

        fl::FactoryManager* factory = fl::FactoryManager::instance();
        if (source->getActivation()) {
            target->setActivation(factory->tnorm()->createInstance(
                    source->getActivation()->className()));
        }
        if (source->getConjunction()) {
            target->setConjunction(factory->tnorm()->createInstance(
                    source->getConjunction()->className()));
        }
        if (source->getDisjunction()) {
            target->setDisjunction(factory->snorm()->createInstance(
                    source->getDisjunction()->className()));
        }

        for (std::size_t i = 0; i < source->rules().size(); ++i) {
            const Rule* rule = source->rules().at(i);
            Rule* copy = new Rule;
            copy->setText(rule->getText());
            copy->setWeight(rule->getWeight());
            target->addRule(copy);
        }
        return target;
    }


}