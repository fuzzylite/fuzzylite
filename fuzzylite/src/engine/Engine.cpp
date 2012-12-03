/*
 * Engine.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "Engine.h"

#include "../variable/Variable.h"
#include "../rule/RuleBlock.h"
#include "../hedge/Hedge.h"
#include "Configuration.h"

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
    void Engine::setName(const std::string& name) {
        this->_name = name;
    }
    std::string Engine::getName() const {
        return this->_name;
    }
    /**
     * Operations for iterable datatype _inputVariables
     */
    void Engine::addInputVariable(Variable* inputVariable) {
        this->_inputVariables.push_back(inputVariable);
    }
    void Engine::insertInputVariable(Variable* inputVariable, int index) {
        this->_inputVariables.insert(this->_inputVariables.begin() + index,
                inputVariable);
    }
    Variable* Engine::getInputVariable(int index) const {
        return this->_inputVariables[index];
    }
    Variable* Engine::removeInputVariable(int index) {
        Variable* result = this->_inputVariables[index];
        this->_inputVariables.erase(this->_inputVariables.begin() + index);
        return result;
    }
    int Engine::numberOfInputVariables() const {
        return this->_inputVariables.size();
    }
    const std::vector<Variable*>& Engine::inputVariables() const {
        return this->_inputVariables;
    }

    /**
     * Operations for iterable datatype _outputVariables
     */
    void Engine::addOutputVariable(Variable* outputVariable) {
        this->_outputVariables.push_back(outputVariable);
    }
    void Engine::insertOutputVariable(Variable* outputVariable, int index) {
        this->_outputVariables.insert(this->_outputVariables.begin() + index,
                outputVariable);
    }
    Variable* Engine::getOutputVariable(int index) const {
        return this->_outputVariables[index];
    }
    Variable* Engine::removeOutputVariable(int index) {
        Variable* result = this->_outputVariables[index];
        this->_outputVariables.erase(this->_outputVariables.begin() + index);
        return result;
    }
    int Engine::numberOfOutputVariables() const {
        return this->_outputVariables.size();
    }
    const std::vector<Variable*>& Engine::outputVariables() const {
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

    void Engine::addHedge(Hedge* hedge){
        this->_hedges[hedge->name()] = hedge;
    }

    Hedge* Engine::removeHedge(const std::string& name){
        std::map<std::string, Hedge*>::iterator it = this->_hedges.find(name);
        if (it == this->_hedges.end()) return NULL;
        Hedge* result = it->second;
        this->_hedges.erase(it);
        return result;
    }

    Hedge* Engine::getHedge(const std::string& name) const{
        std::map<std::string, Hedge*>::const_iterator it = this->_hedges.find(name);
        if (it == this->_hedges.end()) return NULL;
        return it->second;
    }

    const std::map<std::string, Hedge*>& Engine::hedges() const{
        return this->_hedges;
    }

} /* namespace fl */
