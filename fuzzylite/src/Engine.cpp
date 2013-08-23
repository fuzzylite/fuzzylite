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
 * Engine.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "fl/Engine.h"

#include "fl/variable/InputVariable.h"
#include "fl/variable/OutputVariable.h"
#include "fl/rule/RuleBlock.h"
#include "fl/hedge/Hedge.h"

#include "fl/term/Accumulated.h"

#include "fl/factory/Factory.h"
#include "fl/factory/DefuzzifierFactory.h"
#include "fl/factory/SNormFactory.h"
#include "fl/factory/TNormFactory.h"

#include "fl/imex/CppExporter.h"
#include "fl/imex/FclExporter.h"
#include "fl/imex/FisExporter.h"

#include "fl/defuzzifier/WeightedAverage.h"
#include "fl/defuzzifier/WeightedSum.h"

#include "fl/term/Constant.h"
#include "fl/term/Linear.h"
#include "fl/term/Function.h"


namespace fl {

    Engine::Engine(const std::string& name) : _name(name) { }

    Engine::~Engine() {
        for (int i = numberOfRuleBlocks() - 1; i >= 0; --i) {
            delete removeRuleBlock(i);
        }

        for (int i = numberOfHedges() - 1; i >= 0; --i) {
            delete removeHedge(i);
        }

        for (int i = numberOfOutputVariables() - 1; i >= 0; --i) {
            delete removeOutputVariable(i);
        }

        for (int i = numberOfInputVariables() - 1; i >= 0; --i) {
            delete removeInputVariable(i);
        }
    }

    void Engine::configure(const std::string& tnorm, const std::string& snorm,
            const std::string& activationTnorm, const std::string& accumulationSnorm,
            const std::string& defuzzifier, int divisions) {
        TNormFactory* tnormFactory = Factory::instance()->tnorm();
        SNormFactory* snormFactory = Factory::instance()->snorm();
        DefuzzifierFactory* defuzzFactory = Factory::instance()->defuzzifier();
        for (std::size_t i = 0; i < _ruleblocks.size(); ++i) {
            _ruleblocks.at(i)->setTnorm(tnormFactory->create(tnorm));
            _ruleblocks.at(i)->setSnorm(snormFactory->create(snorm));
            _ruleblocks.at(i)->setActivation(tnormFactory->create(activationTnorm));
        }

        for (std::size_t i = 0; i < _outputVariables.size(); ++i) {
            _outputVariables.at(i)->setDefuzzifier(defuzzFactory->create(defuzzifier));
            if (_outputVariables.at(i)->getDefuzzifier()) {
                _outputVariables.at(i)->getDefuzzifier()->setDivisions(divisions);
            }
            _outputVariables.at(i)->output()->setAccumulation(
                    snormFactory->create(accumulationSnorm));
        }
    }

    bool Engine::isReady(std::string* status) const {
        std::ostringstream ss;
        if (_inputVariables.size() == 0) {
            ss << "- Engine has no input variables\n";
        }
        for (std::size_t i = 0; i < _inputVariables.size(); ++i) {
            InputVariable* inputVariable = _inputVariables.at(i);
            if (not inputVariable) {
                ss << "- Engine has a NULL input variable at index <" << i << ">\n";
            } else if (inputVariable->isEmpty()) {
                //ignore because sometimes inputs can be empty: takagi-sugeno/matlab/slcpp1.fis
                //                ss << "- Input variable <" << _inputVariables.at(i)->getName() << ">"
                //                        << " has no terms\n";
            }
        }

        if (_outputVariables.size() == 0) {
            ss << "- Engine has no output variables\n";
        }
        for (std::size_t i = 0; i < _outputVariables.size(); ++i) {
            OutputVariable* outputVariable = _outputVariables.at(i);
            if (not outputVariable) {
                ss << "- Engine has a NULL output variable at index <" << i << ">\n";
            } else {
                if (outputVariable->isEmpty()) {
                    ss << "- Output variable <" << outputVariable->getName() << ">"
                            << " has no terms\n";
                }
                Defuzzifier* defuzzifier = outputVariable->getDefuzzifier();
                if (not defuzzifier) {
                    ss << "- Output variable <" << outputVariable->getName() << ">"
                            << " has no defuzzifier\n";
                } else if (not (defuzzifier->className() == WeightedAverage().className()
                        or defuzzifier->className() == WeightedSum().className())) {
                    
                    if (not outputVariable->output()->getAccumulation()) {
                        ss << "- Output variable <" << outputVariable->getName() << ">"
                                << " has no accumulation S-Norm\n";
                    }
                } 
            }
        }

        if (_ruleblocks.size() == 0) {
            ss << "- Engine has no rule blocks\n";
        }
        for (std::size_t i = 0; i < _ruleblocks.size(); ++i) {
            RuleBlock* ruleblock = _ruleblocks.at(i);
            if (not ruleblock) {
                ss << "- Engine has a NULL rule block at index <" << i << ">\n";
            } else {
                if (ruleblock->isEmpty()) {
                    ss << "- Rule block <" << ruleblock->getName() << "> has no rules\n";
                }
                if (not ruleblock->getTnorm()) {
                    ss << "- Rule block <" << ruleblock->getName() << "> has no T-Norm\n";
                }
                if (not ruleblock->getSnorm()) {
                    ss << "- Rule block <" << ruleblock->getName() << "> has no S-Norm\n";
                }
                if (not ruleblock->getActivation()) {
                    ss << "- Rule block <" << ruleblock->getName() << "> has no activation T-Norm\n";
                }
            }
        }
        if (status) *status = ss.str();
        return ss.str().empty();
    }

    void Engine::process() {
        for (std::size_t i = 0; i < _outputVariables.size(); ++i) {
            _outputVariables.at(i)->output()->clear();
        }

        FL_BEGIN_DEBUG_BLOCK;
        FL_DBG("===============");
        FL_DBG("CURRENT INPUTS:");
        for (std::size_t i = 0; i < _inputVariables.size(); ++i) {
            scalar input = _inputVariables.at(i)->getInput();
            FL_DBG(_inputVariables.at(i)->getName() << ".input = " << input);
            FL_DBG(_inputVariables.at(i)->getName() << ".fuzzyfiedInput = " << _inputVariables.at(i)->fuzzify(input));
        }
        FL_END_DEBUG_BLOCK


        for (std::size_t i = 0; i < _ruleblocks.size(); ++i) {
            _ruleblocks.at(i)->fireRules();
        }


        FL_BEGIN_DEBUG_BLOCK;
        FL_DBG("===============");
        FL_DBG("CURRENT OUTPUTS:");
        for (std::size_t i = 0; i < _outputVariables.size(); ++i) {
            FL_DBG(_outputVariables.at(i)->getName() << ".defaultValue= "
                    << _outputVariables.at(i)->getDefaultValue());

            FL_DBG(_outputVariables.at(i)->getName() << ".lockingRange= "
                    << _outputVariables.at(i)->isLockingOutputRange());

            FL_DBG(_outputVariables.at(i)->getName() << ".lockingValid= "
                    << _outputVariables.at(i)->isLockingValidOutput());

            //no locking is ever performed during this debugging block;
            scalar output = _outputVariables.at(i)->defuzzifyNoLocks();
            FL_DBG(_outputVariables.at(i)->getName() << ".defuzzifiedOutput = " << output);
            FL_DBG(_outputVariables.at(i)->getName() << ".fuzzifiedOutput = " <<
                    _outputVariables.at(i)->fuzzify(output));
            FL_DBG(_outputVariables.at(i)->output()->toString());
        }
        FL_DBG("==============");
        FL_END_DEBUG_BLOCK
    }

    void Engine::setName(const std::string& name) {
        this->_name = name;
    }

    std::string Engine::getName() const {
        return this->_name;
    }

    std::string Engine::toStringCpp() const {
        return CppExporter().toString(this);
    }

    std::string Engine::toStringFcl() const {
        return FclExporter().toString(this);
    }

    std::string Engine::toStringFis() const {
        return FisExporter().toString(this);
    }

    /**
     * Operations for iterable datatype _inputVariables
     */
    void Engine::addInputVariable(InputVariable* inputVariable) {
        this->_inputVariables.push_back(inputVariable);
    }

    void Engine::insertInputVariable(InputVariable* inputVariable, int index) {
        this->_inputVariables.insert(this->_inputVariables.begin() + index,
                inputVariable);
    }

    InputVariable* Engine::getInputVariable(int index) const {
        return this->_inputVariables.at(index);
    }

    InputVariable* Engine::getInputVariable(const std::string& name) const {
        for (std::size_t i = 0; i < _inputVariables.size(); ++i) {
            if (_inputVariables.at(i)->getName() == name)
                return _inputVariables.at(i);
        }
        throw fl::Exception("[engine error] input variable <" + name + "> not found", FL_AT);
    }

    bool Engine::hasInputVariable(const std::string& name) const {
        for (std::size_t i = 0; i < _inputVariables.size(); ++i) {
            if (_inputVariables.at(i)->getName() == name)
                return true;
        }
        return false;
    }

    InputVariable* Engine::removeInputVariable(int index) {
        InputVariable* result = this->_inputVariables.at(index);
        this->_inputVariables.erase(this->_inputVariables.begin() + index);
        return result;
    }

    InputVariable* Engine::removeInputVariable(const std::string& name) {
        for (std::size_t i = 0; i < _inputVariables.size(); ++i) {
            if (_inputVariables.at(i)->getName() == name) {
                InputVariable* result = this->_inputVariables.at(i);
                this->_inputVariables.erase(this->_inputVariables.begin() + i);
                return result;
            }
        }
        throw fl::Exception("[engine error] input variable <" + name + "> not found", FL_AT);
    }

    int Engine::numberOfInputVariables() const {
        return this->_inputVariables.size();
    }

    const std::vector<InputVariable*>& Engine::inputVariables() const {
        return this->_inputVariables;
    }

    /**
     * Operations for iterable datatype _outputVariables
     */
    void Engine::addOutputVariable(OutputVariable* outputVariable) {
        this->_outputVariables.push_back(outputVariable);
    }

    void Engine::insertOutputVariable(OutputVariable* outputVariable, int index) {
        this->_outputVariables.insert(this->_outputVariables.begin() + index,
                outputVariable);
    }

    OutputVariable* Engine::getOutputVariable(int index) const {
        return this->_outputVariables.at(index);
    }

    OutputVariable* Engine::getOutputVariable(const std::string& name) const {
        for (std::size_t i = 0; i < _outputVariables.size(); ++i) {
            if (_outputVariables.at(i)->getName() == name)
                return _outputVariables.at(i);
        }
        throw fl::Exception("[engine error] output variable <" + name + "> not found", FL_AT);
    }

    bool Engine::hasOutputVariable(const std::string& name) const {
        for (std::size_t i = 0; i < _outputVariables.size(); ++i) {
            if (_outputVariables.at(i)->getName() == name)
                return true;
        }
        return false;
    }

    OutputVariable* Engine::removeOutputVariable(int index) {
        OutputVariable* result = this->_outputVariables.at(index);
        this->_outputVariables.erase(this->_outputVariables.begin() + index);
        return result;
    }

    OutputVariable* Engine::removeOutputVariable(const std::string& name) {
        for (std::size_t i = 0; i < _outputVariables.size(); ++i) {
            if (_outputVariables.at(i)->getName() == name) {
                OutputVariable* result = this->_outputVariables.at(i);
                this->_outputVariables.erase(this->_outputVariables.begin() + i);
                return result;
            }
        }
        throw fl::Exception("[engine error] output variable <" + name + "> not found", FL_AT);
    }

    int Engine::numberOfOutputVariables() const {
        return this->_outputVariables.size();
    }

    const std::vector<OutputVariable*>& Engine::outputVariables() const {
        return this->_outputVariables;
    }

    /**
     * Operations for iterable datatype _ruleblocks
     */
    void Engine::addRuleBlock(RuleBlock* ruleblock) {
        this->_ruleblocks.push_back(ruleblock);
    }

    void Engine::insertRuleBlock(RuleBlock* ruleblock, int index) {
        this->_ruleblocks.insert(this->_ruleblocks.begin() + index, ruleblock);
    }

    RuleBlock* Engine::getRuleBlock(int index) const {
        return this->_ruleblocks.at(index);
    }

    RuleBlock* Engine::getRuleBlock(const std::string& name) const {
        for (std::size_t i = 0; i < _ruleblocks.size(); ++i) {
            if (_ruleblocks.at(i)->getName() == name)
                return _ruleblocks.at(i);
        }
        throw fl::Exception("[engine error] rule block <" + name + "> not found", FL_AT);
    }

    bool Engine::hasRuleBlock(const std::string& name) const {
        for (std::size_t i = 0; i < _ruleblocks.size(); ++i) {
            if (_ruleblocks.at(i)->getName() == name)
                return true;
        }
        return false;
    }

    RuleBlock* Engine::removeRuleBlock(int index) {
        RuleBlock* result = this->_ruleblocks.at(index);
        this->_ruleblocks.erase(this->_ruleblocks.begin() + index);
        return result;
    }

    RuleBlock* Engine::removeRuleBlock(const std::string& name) {
        for (std::size_t i = 0; i < _ruleblocks.size(); ++i) {
            if (_ruleblocks.at(i)->getName() == name) {
                RuleBlock* result = this->_ruleblocks.at(i);
                this->_ruleblocks.erase(this->_ruleblocks.begin() + i);
                return result;
            }
        }
        throw fl::Exception("[engine error] rule block <" + name + "> not found", FL_AT);
    }

    int Engine::numberOfRuleBlocks() const {
        return this->_ruleblocks.size();
    }

    const std::vector<RuleBlock*>& Engine::ruleBlocks() const {
        return this->_ruleblocks;
    }

    /**
     * Operations for std::vector _hedges
     */
    void Engine::addHedge(Hedge* hedge) {
        this->_hedges.push_back(hedge);
    }

    void Engine::insertHedge(Hedge* hedge, int index) {
        this->_hedges.insert(this->_hedges.begin() + index, hedge);
    }

    Hedge* Engine::getHedge(int index) const {
        return this->_hedges.at(index);
    }

    Hedge* Engine::getHedge(const std::string& name) const {
        for (std::size_t i = 0; i < this->_hedges.size(); ++i) {
            if (name == this->_hedges.at(i)->name())
                return this->_hedges.at(i);
        }
        throw fl::Exception("[engine error] hedge <" + name + "> not found", FL_AT);
    }

    bool Engine::hasHedge(const std::string& name) const {
        for (std::size_t i = 0; i < this->_hedges.size(); ++i) {
            if (name == this->_hedges.at(i)->name())
                return true;
        }
        return false;
    }

    Hedge* Engine::removeHedge(int index) {
        Hedge* result = this->_hedges.at(index);
        this->_hedges.erase(this->_hedges.begin() + index);
        return result;
    }

    Hedge* Engine::removeHedge(const std::string& name) {
        for (std::size_t i = 0; i < this->_hedges.size(); ++i) {
            if (name == this->_hedges.at(i)->name()) {
                Hedge* result = this->_hedges.at(i);
                this->_hedges.erase(this->_hedges.begin() + i);
                return result;
            }
        }
        throw fl::Exception("[engine error] hedge <" + name + "> not found", FL_AT);
    }

    int Engine::numberOfHedges() const {
        return this->_hedges.size();
    }

    const std::vector<Hedge*>& Engine::hedges() const {
        return this->_hedges;
    }


}
