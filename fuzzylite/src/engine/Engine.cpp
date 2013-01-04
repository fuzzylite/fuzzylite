/*
 * Engine.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "fl/engine/Engine.h"

#include "fl/engine/Configuration.h"

#include "fl/variable/InputVariable.h"
#include "fl/variable/OutputVariable.h"
#include "fl/rule/RuleBlock.h"
#include "fl/hedge/Hedge.h"

#include "fl/term/Accumulated.h"

namespace fl {

    Engine::Engine(const std::string& name)
    : _name(name), _configuration(NULL) { }

    Engine::~Engine() {
        //if (_configuration) delete _configuration;
        //Left to user for deletion/.
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

    TNorm* Engine::createTnorm(const std::string& classname) const {
        if (classname == Minimum().className()) return new Minimum;
        if (classname == AlgebraicProduct().className()) return new AlgebraicProduct;
        if (classname == BoundedDifference().className()) return new BoundedDifference;
        if (classname == DrasticProduct().className()) return new DrasticProduct;
        if (classname == EinsteinProduct().className()) return new EinsteinProduct;
        if (classname == HamacherProduct().className()) return new HamacherProduct;
        throw fl::Exception("[syntax error] T-Norm of class <" + classname "> not recognized");
    }

    SNorm* Engine::createSnorm(const std::string& classname) const {
        if (classname == Maximum().className()) return new Maximum;
        if (classname == AlgebraicSum().className()) return new AlgebraicSum;
        if (classname == BoundedSum().className()) return new BoundedSum;
        if (classname == DrasticSum().className()) return new DrasticSum;
        if (classname == EinsteinSum().className()) return new EinsteinSum;
        if (classname == HamacherSum().className()) return new HamacherSum;
        throw fl::Exception("[syntax error] S-Norm of class <" + classname "> not recognized");
    }

    Defuzzifier* Engine::createDefuzzifier(const std::string& classname) const {
        if (classname == CenterOfGravity().className()) return new CenterOfGravity;
        if (classname == SmallestOfMaximum().className()) return new SmallestOfMaximum;
        if (classname == LargestOfMaximum().className()) return new LargestOfMaximum;
        if (classname == MeanOfMaximum().className()) return new MeanOfMaximum;
        throw fl::Exception("[syntax error] Defuzzifier of class <" + classname "> not recognized");
    }

    void Engine::configure(const std::string& tnorm, const std::string& snorm,
            const std::string& activationTnorm, const std::string& accumulationSnorm,
            const std::string& defuzzifier, int divisions) {
        for (std::size_t i = 0; i < _ruleblocks.size(); ++i) {
            _ruleblocks[i]->setTnorm(createTnorm(tnorm));
            _ruleblocks[i]->setSnorm(createSnorm(snorm));
            _ruleblocks[i]->setActivation(createTnorm(activationTnorm));
        }
        for (std::size_t i = 0; i < _outputVariables.size(); ++i) {
            _outputVariables[i]->setDefuzzifier(createDefuzzifier(defuzzifier));
            _outputVariables[i]->getDefuzzifier()->setDivisions(divisions);
            _outputVariables[i]->output()->setAccumulation(
                    createSnorm(accumulationSnorm));
        }
    }

    void Engine::configure(Configuration* config) {
        this->_configuration = config;

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

    Configuration* Engine::getConfiguration() const {
        return this->_configuration;
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
            FL_DBG(_inputVariables[i]->getName() << ".input = " << input);
            FL_DBG(_inputVariables[i]->getName() << ".fuzzyfiedInput = " << _inputVariables[i]->fuzzify(input));
        }
        FL_END_DEBUG_BLOCK


        for (std::size_t i = 0; i < _ruleblocks.size(); ++i) {
            _ruleblocks[i]->fireRules();
        }


        FL_BEGIN_DEBUG_BLOCK;
        FL_DBG("===============");
        FL_DBG("CURRENT OUTPUTS:");
        for (std::size_t i = 0; i < _outputVariables.size(); ++i) {
            FL_DBG(_outputVariables[i]->getName() << ".defaultValue= "
                    << _outputVariables[i]->getDefaultValue());
            FL_DBG(_outputVariables[i]->getName() << ".lockDefuzzifiedValue= "
                    << _outputVariables[i]->lockDefuzzifiedValue()
                    << " (no locking ever performed during this debugging block, i.e., defuzzify(true)");
            scalar output = _outputVariables[i]->defuzzifyIgnoreLock(); // override to not change the system
            FL_DBG(_outputVariables[i]->getName() << ".defuzzifiedOutput = " << output);
            FL_DBG(_outputVariables[i]->getName() << ".fuzzifiedOutput = " <<
                    _outputVariables[i]->fuzzify(output));
            FL_DBG(_outputVariables[i]->output()->toString());
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
    void Engine::addRuleBlock(RuleBlock* ruleblock) {
        this->_ruleblocks.push_back(ruleblock);
    }

    void Engine::insertRuleBlock(RuleBlock* ruleblock, int index) {
        this->_ruleblocks.insert(this->_ruleblocks.begin() + index, ruleblock);
    }

    RuleBlock* Engine::getRuleBlock(int index) const {
        return this->_ruleblocks[index];
    }

    RuleBlock* Engine::getRuleBlock(const std::string& name) const {
        for (std::size_t i = 0; i < _ruleblocks.size(); ++i) {
            if (_ruleblocks[i]->getName() == name)
                return _ruleblocks[i];
        }
        return NULL;
    }

    bool Engine::hasRuleBlock(const std::string& name) const {
        return getRuleBlock(name) != NULL;
    }

    RuleBlock* Engine::removeRuleBlock(int index) {
        RuleBlock* result = this->_ruleblocks[index];
        this->_ruleblocks.erase(this->_ruleblocks.begin() + index);
        return result;
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

    Hedge* Engine::getHedge(int index) const {
        return this->_hedges[index];
    }

    Hedge* Engine::getHedge(const std::string& name) const {
        for (std::size_t i = 0; i < this->_hedges.size(); ++i) {
            if (name == this->_hedges[i]->name()) {
                return this->_hedges[i];
            }
        }
        return NULL;
    }

    Hedge* Engine::removeHedge(int index) {
        Hedge* result = this->_hedges[index];
        this->_hedges.erase(this->_hedges.begin() + index);
        return result;
    }

    int Engine::numberOfHedges() const {
        return this->_hedges.size();
    }

    const std::vector<Hedge*>& Engine::hedges() const {
        return this->_hedges;
    }


}
