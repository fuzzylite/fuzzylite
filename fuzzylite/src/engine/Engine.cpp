/*
 * Engine.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "Engine.h"

#include "../variable/InputVariable.h"
#include "../variable/OutputVariable.h"
#include "../rule/RuleBlock.h"
#include "../hedge/Hedge.h"
#include "Configuration.h"

#include "../term/Accumulated.h"

namespace fl {

    Engine::Engine(const std::string& name)
            : _name(name), _configuration(NULL) {
    }

    Engine::~Engine() {
        if (_configuration)
            delete _configuration;
    }

    void Engine::configure(Configuration* config, bool storeToDelete) {
        if (storeToDelete) {
            this->_configuration = config;
        }
        for (std::size_t i = 0; i < _inputVariables.size(); ++i) {
            _inputVariables[i]->configure(config);
        }
        for (std::size_t i = 0; i < _outputVariables.size(); ++i) {
            _outputVariables[i]->configure(config);
        }
        for (std::size_t i = 0; i < _ruleblocks.size(); ++i) {
            _ruleblocks[i]->configure(config);
        }
    }

    void Engine::process() {
        for (std::size_t i = 0; i < _outputVariables.size(); ++i) {
            _outputVariables[i]->output()->clear();
        }
        FL_BEGIN_DEBUG_BLOCK;
            FL_DBG("===============");
            FL_DBG("CURRENT INPUTS:");
            for (std::size_t i = 0; i < _inputVariables.size(); ++i) {
                scalar input = _inputVariables[i]->getInput();
                FL_DBG( _inputVariables[i]->getName() << ".input = " << input);
                FL_DBG( _inputVariables[i]->getName() << ".fuzzyfiedInput = " << _inputVariables[i]->fuzzify(input));
            }
            FL_END_DEBUG_BLOCK

        for (std::size_t i = 0; i < _ruleblocks.size(); ++i) {
            _ruleblocks[i]->fireRules();
        }
        FL_BEGIN_DEBUG_BLOCK;
            FL_DBG("===============");
            FL_DBG("CURRENT OUTPUTS:");
            for (std::size_t i = 0; i < _inputVariables.size(); ++i) {
                FL_DBG( _outputVariables[i]->getName() << ".defaultValue= "
                        << _outputVariables[i]->getDefaultValue());
                FL_DBG( _outputVariables[i]->getName() << ".lockDefuzzifiedValue= "
                        << _outputVariables[i]->lockDefuzzifiedValue()
                        << " (no locking ever performed during this debugging block, i.e., defuzzify(true)");
                scalar output = _outputVariables[i]->defuzzify(true); // override to not change the system
                FL_DBG( _outputVariables[i]->getName() << ".defuzzifiedOutput = " << output);
                FL_DBG( _outputVariables[i]->getName() << ".fuzzifiedOutput = " <<
                        _outputVariables[i]->fuzzify(output));
                FL_DBG( _outputVariables[i]->output()->toString());
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
        return this->_inputVariables[index];
    }
    InputVariable* Engine::getInputVariable(const std::string& name) const {
        for (std::size_t i = 0; i < _inputVariables.size(); ++i) {
            if (_inputVariables[i]->getName() == name)
                return _inputVariables[i];
        }
        return NULL;
    }
    bool Engine::hasInputVariable(const std::string& name) const {
        return getInputVariable(name) != NULL;
    }
    InputVariable* Engine::removeInputVariable(int index) {
        InputVariable* result = this->_inputVariables[index];
        this->_inputVariables.erase(this->_inputVariables.begin() + index);
        return result;
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
        return this->_outputVariables[index];
    }
    OutputVariable* Engine::getOutputVariable(const std::string& name) const {
        for (std::size_t i = 0; i < _outputVariables.size(); ++i) {
            if (_outputVariables[i]->getName() == name)
                return _outputVariables[i];
        }
        return NULL;
    }
    bool Engine::hasOutputVariable(const std::string& name) const {
        return getOutputVariable(name) != NULL;
    }
    OutputVariable* Engine::removeOutputVariable(int index) {
        OutputVariable* result = this->_outputVariables[index];
        this->_outputVariables.erase(this->_outputVariables.begin() + index);
        return result;
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
    void Engine::addRuleblock(RuleBlock* ruleblock) {
        this->_ruleblocks.push_back(ruleblock);
    }
    void Engine::insertRuleblock(RuleBlock* ruleblock, int index) {
        this->_ruleblocks.insert(this->_ruleblocks.begin() + index, ruleblock);
    }
    RuleBlock* Engine::getRuleblock(int index) const {
        return this->_ruleblocks[index];
    }
    RuleBlock* Engine::getRuleblock(const std::string& name) const {
        for (std::size_t i = 0; i < _ruleblocks.size(); ++i) {
            if (_ruleblocks[i]->getName() == name)
                return _ruleblocks[i];
        }
        return NULL;
    }
    bool Engine::hasRuleblock(const std::string& name) const {
        return getRuleblock(name) != NULL;
    }
    RuleBlock* Engine::removeRuleblock(int index) {
        RuleBlock* result = this->_ruleblocks[index];
        this->_ruleblocks.erase(this->_ruleblocks.begin() + index);
        return result;
    }
    int Engine::numberOfRuleblocks() const {
        return this->_ruleblocks.size();
    }
    const std::vector<RuleBlock*>& Engine::ruleblocks() const {
        return this->_ruleblocks;
    }

    /**
     * Operations for map _hedges
     */

    void Engine::addHedge(Hedge* hedge) {
        this->_hedges[hedge->name()] = hedge;
    }

    Hedge* Engine::removeHedge(const std::string& name) {
        std::map<std::string, Hedge*>::iterator it = this->_hedges.find(name);
        if (it == this->_hedges.end())
            return NULL;
        Hedge* result = it->second;
        this->_hedges.erase(it);
        return result;
    }

    Hedge* Engine::getHedge(const std::string& name) const {
        std::map<std::string, Hedge*>::const_iterator it = this->_hedges.find(name);
        if (it == this->_hedges.end())
            return NULL;
        return it->second;
    }

    bool Engine::hasHedge(const std::string& name) const {
        return getHedge(name) != NULL;
    }

    const std::map<std::string, Hedge*>& Engine::hedges() const {
        return this->_hedges;
    }

} /* namespace fl */
