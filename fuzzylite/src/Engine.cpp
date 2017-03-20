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

#include "fl/Engine.h"

#include "fl/activation/General.h"
#include "fl/defuzzifier/WeightedAverage.h"
#include "fl/defuzzifier/WeightedSum.h"
#include "fl/factory/DefuzzifierFactory.h"
#include "fl/factory/FactoryManager.h"
#include "fl/imex/FllExporter.h"
#include "fl/norm/t/AlgebraicProduct.h"
#include "fl/rule/Consequent.h"
#include "fl/rule/Expression.h"
#include "fl/rule/Rule.h"
#include "fl/rule/RuleBlock.h"
#include "fl/term/Aggregated.h"
#include "fl/term/Constant.h"
#include "fl/term/Linear.h"
#include "fl/term/Ramp.h"
#include "fl/term/Sigmoid.h"
#include "fl/term/SShape.h"
#include "fl/term/ZShape.h"
#include "fl/variable/InputVariable.h"
#include "fl/variable/OutputVariable.h"

namespace fl {

    Engine::Engine(const std::string& name) : _name(name) { }

    Engine::Engine(const Engine& other) : _name(""), _description("") {
        copyFrom(other);
    }

    Engine& Engine::operator=(const Engine& other) {
        if (this != &other) {
            for (std::size_t i = 0; i < _ruleBlocks.size(); ++i)
                delete _ruleBlocks.at(i);
            _ruleBlocks.clear();
            for (std::size_t i = 0; i < _outputVariables.size(); ++i)
                delete _outputVariables.at(i);
            _outputVariables.clear();
            for (std::size_t i = 0; i < _inputVariables.size(); ++i)
                delete _inputVariables.at(i);
            _inputVariables.clear();

            copyFrom(other);
        }
        return *this;
    }

    void Engine::copyFrom(const Engine& other) {
        _name = other._name;
        _description = other._description;
        for (std::size_t i = 0; i < other._inputVariables.size(); ++i)
            _inputVariables.push_back(new InputVariable(*other._inputVariables.at(i)));
        for (std::size_t i = 0; i < other._outputVariables.size(); ++i)
            _outputVariables.push_back(new OutputVariable(*other._outputVariables.at(i)));

        updateReferences();

        for (std::size_t i = 0; i < other._ruleBlocks.size(); ++i) {
            RuleBlock* ruleBlock = new RuleBlock(*other._ruleBlocks.at(i));
            try {
                ruleBlock->loadRules(this);
            } catch (...) {
                //ignore
            }
            _ruleBlocks.push_back(ruleBlock);
        }
    }

    void Engine::updateReferences() const {
        std::vector<Variable*> myVariables = variables();
        for (std::size_t i = 0; i < myVariables.size(); ++i) {
            Variable* variable = myVariables.at(i);
            for (std::size_t t = 0; t < variable->numberOfTerms(); ++t) {
                variable->getTerm(t)->updateReference(this);
            }
        }
    }

    Engine::~Engine() {
        for (std::size_t i = 0; i < _ruleBlocks.size(); ++i)
            delete _ruleBlocks.at(i);
        for (std::size_t i = 0; i < _outputVariables.size(); ++i)
            delete _outputVariables.at(i);
        for (std::size_t i = 0; i < _inputVariables.size(); ++i)
            delete _inputVariables.at(i);
    }

    void Engine::configure(const std::string& conjunction, const std::string& disjunction,
            const std::string& implication, const std::string& aggregation,
            const std::string& defuzzifier, const std::string& activation) {
        TNormFactory* tnormFactory = FactoryManager::instance()->tnorm();
        SNormFactory* snormFactory = FactoryManager::instance()->snorm();
        DefuzzifierFactory* defuzzFactory = FactoryManager::instance()->defuzzifier();
        ActivationFactory* activationFactory = FactoryManager::instance()->activation();

        TNorm* conjunctionObject = tnormFactory->constructObject(conjunction);
        SNorm* disjunctionObject = snormFactory->constructObject(disjunction);
        TNorm* implicationObject = tnormFactory->constructObject(implication);
        SNorm* aggregationObject = snormFactory->constructObject(aggregation);
        Defuzzifier* defuzzifierObject = defuzzFactory->constructObject(defuzzifier);
        Activation* activationObject = activationFactory->constructObject(activation);

        configure(conjunctionObject, disjunctionObject,
                implicationObject, aggregationObject, defuzzifierObject,
                activationObject);
    }

    void Engine::configure(TNorm* conjunction, SNorm* disjunction,
            TNorm* implication, SNorm* aggregation, Defuzzifier* defuzzifier,
            Activation* activation) {
        for (std::size_t i = 0; i < numberOfRuleBlocks(); ++i) {
            RuleBlock* ruleBlock = ruleBlocks().at(i);
            ruleBlock->setConjunction(conjunction ? conjunction->clone() : fl::null);
            ruleBlock->setDisjunction(disjunction ? disjunction->clone() : fl::null);
            ruleBlock->setImplication(implication ? implication->clone() : fl::null);
            ruleBlock->setActivation(activation ? activation->clone() : new General);
        }

        for (std::size_t i = 0; i < numberOfOutputVariables(); ++i) {
            OutputVariable* outputVariable = getOutputVariable(i);
            outputVariable->setDefuzzifier(defuzzifier ? defuzzifier->clone() : fl::null);
            outputVariable->setAggregation(aggregation ? aggregation->clone() : fl::null);
        }
        if (defuzzifier) delete defuzzifier;
        if (aggregation) delete aggregation;
        if (implication) delete implication;
        if (disjunction) delete disjunction;
        if (conjunction) delete conjunction;
        if (activation) delete activation;
    }

    bool Engine::isReady(std::string* status) const {
        std::ostringstream ss;
        if (inputVariables().empty()) {
            ss << "- Engine <" << getName() << "> has no input variables\n";
        }
        for (std::size_t i = 0; i < inputVariables().size(); ++i) {
            InputVariable* inputVariable = inputVariables().at(i);
            if (not inputVariable) {
                ss << "- Engine <" << getName() << "> has a fl::null input variable at index <" << i << ">\n";
            }
            /*else if (inputVariable->terms().empty()) {
            ignore because sometimes inputs can be empty: takagi-sugeno/matlab/slcpp1.fis
                            ss << "- Input variable <" << _inputVariables.at(i)->getName() << ">"
                                    << " has no terms\n";
            }*/
        }

        if (outputVariables().empty()) {
            ss << "- Engine <" << _name << "> has no output variables\n";
        }
        for (std::size_t i = 0; i < outputVariables().size(); ++i) {
            OutputVariable* outputVariable = outputVariables().at(i);
            if (not outputVariable) {
                ss << "- Engine <" << getName() << "> has a fl::null output variable at index <" << i << ">\n";
            } else {
                if (outputVariable->terms().empty()) {
                    ss << "- Output variable <" << outputVariable->getName() << ">"
                            << " has no terms\n";
                }
                Defuzzifier* defuzzifier = outputVariable->getDefuzzifier();
                if (not defuzzifier) {
                    ss << "- Output variable <" << outputVariable->getName() << ">"
                            << " has no defuzzifier\n";
                }
                SNorm* aggregation = outputVariable->fuzzyOutput()->getAggregation();
                if (not aggregation and dynamic_cast<IntegralDefuzzifier*> (defuzzifier)) {
                    ss << "- Output variable <" << outputVariable->getName() << ">"
                            << " has no aggregation operator\n";
                }
            }
        }

        if (ruleBlocks().empty()) {
            ss << "- Engine <" << getName() << "> has no rule blocks\n";
        }
        for (std::size_t i = 0; i < ruleBlocks().size(); ++i) {
            RuleBlock* ruleblock = ruleBlocks().at(i);
            if (not ruleblock) {
                ss << "- Engine <" << getName() << "> has a fl::null rule block at index <" << i << ">\n";
            } else {
                if (ruleblock->rules().empty()) {
                    ss << "- Rule block " << (i + 1) << " <" << ruleblock->getName() << "> has no rules\n";
                }
                int requiresConjunction = 0;
                int requiresDisjunction = 0;
                int requiresImplication = 0;
                for (std::size_t r = 0; r < ruleblock->numberOfRules(); ++r) {
                    Rule* rule = ruleblock->getRule(r);
                    if (not rule) {
                        ss << "- Rule block " << (i + 1) << " <" << ruleblock->getName()
                                << "> has a fl::null rule at index <" << r << ">\n";
                    } else {
                        std::size_t thenIndex = rule->getText().find(" " + Rule::thenKeyword() + " ");
                        std::size_t andIndex = rule->getText().find(" " + Rule::andKeyword() + " ");
                        std::size_t orIndex = rule->getText().find(" " + Rule::orKeyword() + " ");
                        if (andIndex != std::string::npos and andIndex < thenIndex) {
                            ++requiresConjunction;
                        }
                        if (orIndex != std::string::npos and orIndex < thenIndex) {
                            ++requiresDisjunction;
                        }
                        if (rule->isLoaded()) {
                            Consequent* consequent = rule->getConsequent();
                            for (std::size_t c = 0; c < consequent->conclusions().size(); ++c) {
                                Proposition* proposition = consequent->conclusions().at(c);
                                const OutputVariable* outputVariable =
                                        dynamic_cast<const OutputVariable*> (proposition->variable);
                                if (outputVariable and dynamic_cast<IntegralDefuzzifier*> (outputVariable->getDefuzzifier())) {
                                    ++requiresImplication;
                                    break;
                                }
                            }
                        }
                    }
                }
                const TNorm* conjunction = ruleblock->getConjunction();
                if (requiresConjunction > 0 and not conjunction) {
                    ss << "- Rule block " << (i + 1) << " <" << ruleblock->getName() << "> has no conjunction operator\n";
                    ss << "- Rule block " << (i + 1) << " <" << ruleblock->getName() << "> has "
                            << requiresConjunction << " rules that require conjunction operator\n";
                }
                const SNorm* disjunction = ruleblock->getDisjunction();
                if (requiresDisjunction > 0 and not disjunction) {
                    ss << "- Rule block " << (i + 1) << " <" << ruleblock->getName() << "> has no disjunction operator\n";
                    ss << "- Rule block " << (i + 1) << " <" << ruleblock->getName() << "> has "
                            << requiresDisjunction << " rules that require disjunction operator\n";
                }
                const TNorm* implication = ruleblock->getImplication();
                if (requiresImplication > 0 and not implication) {
                    ss << "- Rule block " << (i + 1) << " <" << ruleblock->getName() << "> has no implication operator\n";
                    ss << "- Rule block " << (i + 1) << " <" << ruleblock->getName() << "> has "
                            << requiresImplication << " rules that require implication operator\n";
                }
            }
        }
        if (status) *status = ss.str();
        return ss.str().empty();
    }

    void Engine::restart() {
        for (std::size_t i = 0; i < inputVariables().size(); ++i) {
            inputVariables().at(i)->setValue(fl::nan);
        }
        for (std::size_t i = 0; i < outputVariables().size(); ++i) {
            outputVariables().at(i)->clear();
        }
    }

    Complexity Engine::complexity() const {
        Complexity result;
        for (std::size_t i = 0; i < _ruleBlocks.size(); ++i) {
            const RuleBlock* ruleBlock = _ruleBlocks.at(i);
            if (ruleBlock->isEnabled()) {
                result += ruleBlock->complexity();
            }
        }
        return result;
    }

    void Engine::process() {
        for (std::size_t i = 0; i < _outputVariables.size(); ++i) {
            _outputVariables.at(i)->fuzzyOutput()->clear();
        }

        FL_DEBUG_BEGIN;
        FL_DBG("===============");
        FL_DBG("CURRENT INPUTS:");
        for (std::size_t i = 0; i < _inputVariables.size(); ++i) {
            InputVariable* inputVariable = _inputVariables.at(i);
            scalar inputValue = inputVariable->getValue();
            if (inputVariable->isEnabled()) {
                FL_DBG(inputVariable->getName() << ".input = " << Op::str(inputValue));
                FL_DBG(inputVariable->getName() << ".fuzzy = " << inputVariable->fuzzify(inputValue));
            } else {
                FL_DBG(inputVariable->getName() << ".enabled = false");
            }
        }
        FL_DEBUG_END;


        for (std::size_t i = 0; i < _ruleBlocks.size(); ++i) {
            RuleBlock* ruleBlock = _ruleBlocks.at(i);
            if (ruleBlock->isEnabled()) {
                FL_DBG("===============");
                FL_DBG("RULE BLOCK: " << ruleBlock->getName());
                ruleBlock->activate();
            }
        }

        for (std::size_t i = 0; i < _outputVariables.size(); ++i) {
            _outputVariables.at(i)->defuzzify();
        }

        FL_DEBUG_BEGIN;
        FL_DBG("===============");
        FL_DBG("CURRENT OUTPUTS:");
        for (std::size_t i = 0; i < _outputVariables.size(); ++i) {
            OutputVariable* outputVariable = _outputVariables.at(i);
            if (outputVariable->isEnabled()) {
                FL_DBG(outputVariable->getName() << ".default = "
                        << outputVariable->getDefaultValue());

                FL_DBG(outputVariable->getName() << ".lockValueInRange = "
                        << outputVariable->isLockValueInRange());

                FL_DBG(outputVariable->getName() << ".lockPreviousValue= "
                        << outputVariable->isLockPreviousValue());

                scalar output = outputVariable->getValue();
                FL_DBG(outputVariable->getName() << ".output = " << output);
                FL_DBG(outputVariable->getName() << ".fuzzy = " <<
                        outputVariable->fuzzify(output));
                FL_DBG(outputVariable->fuzzyOutput()->toString());
            } else {
                FL_DBG(outputVariable->getName() << ".enabled = false");
            }
        }
        FL_DBG("==============");
        FL_DEBUG_END;
    }

    void Engine::setName(const std::string& name) {
        this->_name = name;
    }

    std::string Engine::getName() const {
        return this->_name;
    }

    void Engine::setDescription(const std::string& description) {
        this->_description = description;
    }

    std::string Engine::getDescription() const {
        return this->_description;
    }

    std::string Engine::toString() const {
        return FllExporter().toString(this);
    }

    Engine::Type Engine::type(std::string* name, std::string* reason) const {
        if (outputVariables().empty()) {
            if (name) *name = "Unknown";
            if (reason) *reason = "- Engine has no output variables";
            return Engine::Unknown;
        }

        //Mamdani
        bool mamdani = true;
        for (std::size_t i = 0; mamdani and i < outputVariables().size(); ++i) {
            OutputVariable* outputVariable = outputVariables().at(i);
            //Defuzzifier must be integral
            mamdani = mamdani and dynamic_cast<IntegralDefuzzifier*> (outputVariable->getDefuzzifier());
        }
        //Larsen
        bool larsen = mamdani and not ruleBlocks().empty();
        //Larsen is Mamdani with AlgebraicProduct as Implication
        if (mamdani) {
            for (std::size_t i = 0; larsen and i < ruleBlocks().size(); ++i) {
                RuleBlock* ruleBlock = ruleBlocks().at(i);
                larsen = larsen and dynamic_cast<const AlgebraicProduct*> (ruleBlock->getImplication());
            }
        }
        if (larsen) {
            if (name) *name = "Larsen";
            if (reason) *reason = "- Output variables have integral defuzzifiers\n"
                    "- Implication in rule blocks is the algebraic product T-Norm";
            return Engine::Larsen;
        }
        if (mamdani) {
            if (name) *name = "Mamdani";
            if (reason) *reason = "-Output variables have integral defuzzifiers";
            return Engine::Mamdani;
        }
        //Else, keep checking

        //TakagiSugeno
        bool takagiSugeno = true;
        for (std::size_t i = 0; takagiSugeno and i < outputVariables().size(); ++i) {
            OutputVariable* outputVariable = outputVariables().at(i);
            //Defuzzifier is Weighted
            WeightedDefuzzifier* weightedDefuzzifier =
                    dynamic_cast<WeightedDefuzzifier*> (outputVariable->getDefuzzifier());

            takagiSugeno = takagiSugeno and weightedDefuzzifier and
                    (weightedDefuzzifier->getType() == WeightedDefuzzifier::Automatic or
                    weightedDefuzzifier->getType() == WeightedDefuzzifier::TakagiSugeno);

            if (takagiSugeno) {
                //Takagi-Sugeno has only Constant, Linear or Function terms
                for (std::size_t t = 0; takagiSugeno and t < outputVariable->numberOfTerms(); ++t) {
                    Term* term = outputVariable->getTerm(t);
                    takagiSugeno = takagiSugeno and
                            weightedDefuzzifier->inferType(term) == WeightedDefuzzifier::TakagiSugeno;
                }
            }
        }
        if (takagiSugeno) {
            if (name) *name = "Takagi-Sugeno";
            if (reason) *reason = "- Output variables have weighted defuzzifiers\n"
                    "- Output variables have constant, linear or function terms";
            return Engine::TakagiSugeno;
        }

        //Tsukamoto
        bool tsukamoto = true;
        for (std::size_t i = 0; tsukamoto and i < outputVariables().size(); ++i) {
            OutputVariable* outputVariable = outputVariables().at(i);
            //Defuzzifier is Weighted
            WeightedDefuzzifier* weightedDefuzzifier =
                    dynamic_cast<WeightedDefuzzifier*> (outputVariable->getDefuzzifier());

            tsukamoto = tsukamoto and weightedDefuzzifier and
                    (weightedDefuzzifier->getType() == WeightedDefuzzifier::Automatic or
                    weightedDefuzzifier->getType() == WeightedDefuzzifier::Tsukamoto);
            if (tsukamoto) {
                //Tsukamoto has only monotonic terms: Concave, Ramp, Sigmoid, SShape, or ZShape
                for (std::size_t t = 0; tsukamoto and t < outputVariable->numberOfTerms(); ++t) {
                    Term* term = outputVariable->getTerm(t);
                    tsukamoto = tsukamoto and term->isMonotonic();
                }
            }
        }
        if (tsukamoto) {
            if (name) *name = "Tsukamoto";
            if (reason) *reason = "- Output variables have weighted defuzzifiers\n"
                    "- Output variables only have monotonic terms";
            return Engine::Tsukamoto;
        }

        //Inverse Tsukamoto
        bool inverseTsukamoto = true;
        for (std::size_t i = 0; inverseTsukamoto and i < outputVariables().size(); ++i) {
            OutputVariable* outputVariable = outputVariables().at(i);
            //Defuzzifier cannot be integral
            WeightedDefuzzifier* weightedDefuzzifier =
                    dynamic_cast<WeightedDefuzzifier*> (outputVariable->getDefuzzifier());
            inverseTsukamoto = inverseTsukamoto and weightedDefuzzifier;
        }
        if (inverseTsukamoto) {
            if (name) *name = "Inverse Tsukamoto";
            if (reason) *reason = "- Output variables have weighted defuzzifiers\n"
                    "- Output variables do not only have constant, linear or function terms\n"
                    "- Output variables do not only have monotonic terms";
            return Engine::InverseTsukamoto;
        }

        bool hybrid = true;
        for (std::size_t i = 0; i < outputVariables().size(); ++i) {
            OutputVariable* outputVariable = outputVariables().at(i);
            //Output variables have non-fl::null defuzzifiers
            hybrid = hybrid and outputVariable->getDefuzzifier();
        }
        if (hybrid) {
            if (name) *name = "Hybrid";
            if (reason) *reason = "- Output variables have different types of defuzzifiers";
            return Engine::Hybrid;
        }

        if (name) *name = "Unknown";
        if (reason) *reason = "- One or more output variables do not have a defuzzifier";
        return Engine::Unknown;
    }

    Engine* Engine::clone() const {
        return new Engine(*this);
    }

    std::vector<Variable*> Engine::variables() const {
        std::vector<Variable*> result;
        result.reserve(inputVariables().size() + outputVariables().size());
        result.insert(result.end(), inputVariables().begin(), inputVariables().end());
        result.insert(result.end(), outputVariables().begin(), outputVariables().end());
        return result;
    }

    /**
     * Operations for InputVariables
     */
    void Engine::setInputValue(const std::string& name, scalar value) {
        InputVariable* inputVariable = getInputVariable(name);
        inputVariable->setValue(value);
    }

    void Engine::addInputVariable(InputVariable* inputVariable) {
        inputVariables().push_back(inputVariable);
    }

    InputVariable* Engine::setInputVariable(InputVariable* inputVariable, std::size_t index) {
        InputVariable* result = inputVariables().at(index);
        inputVariables().at(index) = inputVariable;
        return result;
    }

    void Engine::insertInputVariable(InputVariable* inputVariable, std::size_t index) {
        inputVariables().insert(inputVariables().begin() + index, inputVariable);
    }

    InputVariable* Engine::getInputVariable(std::size_t index) const {
        return inputVariables().at(index);
    }

    InputVariable* Engine::getInputVariable(const std::string& name) const {
        for (std::size_t i = 0; i < inputVariables().size(); ++i) {
            if (inputVariables().at(i)->getName() == name)
                return inputVariables().at(i);
        }
        throw Exception("[engine error] input variable <" + name + "> not found", FL_AT);
    }

    bool Engine::hasInputVariable(const std::string& name) const {
        for (std::size_t i = 0; i < inputVariables().size(); ++i) {
            if (inputVariables().at(i)->getName() == name)
                return true;
        }
        return false;
    }

    InputVariable* Engine::removeInputVariable(std::size_t index) {
        InputVariable* result = inputVariables().at(index);
        inputVariables().erase(inputVariables().begin() + index);
        return result;
    }

    InputVariable* Engine::removeInputVariable(const std::string& name) {
        for (std::size_t i = 0; i < inputVariables().size(); ++i) {
            if (inputVariables().at(i)->getName() == name) {
                InputVariable* result = inputVariables().at(i);
                inputVariables().erase(inputVariables().begin() + i);
                return result;
            }
        }
        throw Exception("[engine error] input variable <" + name + "> not found", FL_AT);
    }

    std::size_t Engine::numberOfInputVariables() const {
        return inputVariables().size();
    }

    const std::vector<InputVariable*>& Engine::inputVariables() const {
        return this->_inputVariables;
    }

    void Engine::setInputVariables(const std::vector<InputVariable*>& inputVariables) {
        this->_inputVariables = inputVariables;
    }

    std::vector<InputVariable*>& Engine::inputVariables() {
        return this->_inputVariables;
    }

    /**
     * Operations for OutputVariables
     */
    scalar Engine::getOutputValue(const std::string& name) {
        OutputVariable* outputVariable = getOutputVariable(name);
        return outputVariable->getValue();
    }

    void Engine::addOutputVariable(OutputVariable* outputVariable) {
        outputVariables().push_back(outputVariable);
    }

    OutputVariable* Engine::setOutputVariable(OutputVariable* outputVariable, std::size_t index) {
        OutputVariable* result = outputVariables().at(index);
        outputVariables().at(index) = outputVariable;
        return result;
    }

    void Engine::insertOutputVariable(OutputVariable* outputVariable, std::size_t index) {
        outputVariables().insert(outputVariables().begin() + index, outputVariable);
    }

    OutputVariable* Engine::getOutputVariable(std::size_t index) const {
        return outputVariables().at(index);
    }

    OutputVariable* Engine::getOutputVariable(const std::string& name) const {
        for (std::size_t i = 0; i < outputVariables().size(); ++i) {
            if (outputVariables().at(i)->getName() == name)
                return outputVariables().at(i);
        }
        throw Exception("[engine error] output variable <" + name + "> not found", FL_AT);
    }

    bool Engine::hasOutputVariable(const std::string& name) const {
        for (std::size_t i = 0; i < outputVariables().size(); ++i) {
            if (outputVariables().at(i)->getName() == name)
                return true;
        }
        return false;
    }

    OutputVariable* Engine::removeOutputVariable(std::size_t index) {
        OutputVariable* result = outputVariables().at(index);
        outputVariables().erase(outputVariables().begin() + index);
        return result;
    }

    OutputVariable* Engine::removeOutputVariable(const std::string& name) {
        for (std::size_t i = 0; i < outputVariables().size(); ++i) {
            if (outputVariables().at(i)->getName() == name) {
                OutputVariable* result = outputVariables().at(i);
                outputVariables().erase(outputVariables().begin() + i);
                return result;
            }
        }
        throw Exception("[engine error] output variable <" + name + "> not found", FL_AT);
    }

    std::size_t Engine::numberOfOutputVariables() const {
        return outputVariables().size();
    }

    const std::vector<OutputVariable*>& Engine::outputVariables() const {
        return this->_outputVariables;
    }

    void Engine::setOutputVariables(const std::vector<OutputVariable*>& outputVariables) {
        this->_outputVariables = outputVariables;
    }

    std::vector<OutputVariable*>& Engine::outputVariables() {
        return this->_outputVariables;
    }

    /**
     * Operations for iterable datatype _ruleblocks
     */
    void Engine::addRuleBlock(RuleBlock* ruleblock) {
        ruleBlocks().push_back(ruleblock);
    }

    RuleBlock* Engine::setRuleBlock(RuleBlock* ruleBlock, std::size_t index) {
        RuleBlock* result = ruleBlocks().at(index);
        ruleBlocks().at(index) = ruleBlock;
        return result;
    }

    void Engine::insertRuleBlock(RuleBlock* ruleblock, std::size_t index) {
        ruleBlocks().insert(ruleBlocks().begin() + index, ruleblock);
    }

    RuleBlock* Engine::getRuleBlock(std::size_t index) const {
        return ruleBlocks().at(index);
    }

    RuleBlock* Engine::getRuleBlock(const std::string& name) const {
        for (std::size_t i = 0; i < ruleBlocks().size(); ++i) {
            if (ruleBlocks().at(i)->getName() == name)
                return ruleBlocks().at(i);
        }
        throw Exception("[engine error] rule block <" + name + "> not found", FL_AT);
    }

    bool Engine::hasRuleBlock(const std::string& name) const {
        for (std::size_t i = 0; i < ruleBlocks().size(); ++i) {
            if (ruleBlocks().at(i)->getName() == name)
                return true;
        }
        return false;
    }

    RuleBlock* Engine::removeRuleBlock(std::size_t index) {
        RuleBlock* result = ruleBlocks().at(index);
        ruleBlocks().erase(ruleBlocks().begin() + index);
        return result;
    }

    RuleBlock* Engine::removeRuleBlock(const std::string& name) {
        for (std::size_t i = 0; i < ruleBlocks().size(); ++i) {
            if (ruleBlocks().at(i)->getName() == name) {
                RuleBlock* result = ruleBlocks().at(i);
                ruleBlocks().erase(ruleBlocks().begin() + i);
                return result;
            }
        }
        throw Exception("[engine error] rule block <" + name + "> not found", FL_AT);
    }

    std::size_t Engine::numberOfRuleBlocks() const {
        return ruleBlocks().size();
    }

    const std::vector<RuleBlock*>& Engine::ruleBlocks() const {
        return this->_ruleBlocks;
    }

    void Engine::setRuleBlocks(const std::vector<RuleBlock*>& ruleBlocks) {
        this->_ruleBlocks = ruleBlocks;
    }

    std::vector<RuleBlock*>& Engine::ruleBlocks() {
        return this->_ruleBlocks;
    }

}
