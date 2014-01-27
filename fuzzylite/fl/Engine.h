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
 * Engine.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_ENGINE_H
#define FL_ENGINE_H

#include "fl/fuzzylite.h"
#include "fl/defuzzifier/IntegralDefuzzifier.h"

#include <vector>
#include <string>

/**
 * @class fl::Engine
 * @brief A fuzzy engine.
 * @file Engine.cpp
 * 
 */

namespace fl {

    class InputVariable;
    class OutputVariable;
    class RuleBlock;
    class Hedge;
    class TNorm;
    class SNorm;
    class Defuzzifier;

    class FL_EXPORT Engine {
    protected:
        /** The name of the engine used for information only*/
        std::string _name;
        /** The input variables registered in the engine*/
        std::vector<InputVariable*> _inputVariables;
        /** The output variables registered in the engine*/
        std::vector<OutputVariable*> _outputVariables;
        /** The rule blocks registered in the engine*/
        std::vector<RuleBlock*> _ruleblocks;
        /** The hedges registered in the engine to be used by fuzzy rules. @see fl::HedgeFactory*/
        std::vector<Hedge*> _hedges;

    public:
        Engine(const std::string& name = "");
        virtual ~Engine();

        virtual void configure(const std::string& conjunctionT = "Minimum",
                const std::string& disjunctionS = "Maximum",
                const std::string& activationT = "Minimum",
                const std::string& accumulationS = "Maximum",
                const std::string& defuzzifier = "Centroid",
                int resolution = IntegralDefuzzifier::defaultResolution());

        virtual bool isReady(std::string* status = NULL) const;

        virtual void process();

        virtual void restart();

        virtual void setName(const std::string& name);
        virtual std::string getName() const;

        virtual void setInputValue(const std::string& name, scalar value);
        virtual scalar getOutputValue(const std::string& name);

        virtual std::string toString() const;
        /**
         * Operations for iterable datatype _inputVariables
         */
        virtual void addInputVariable(InputVariable* inputVariable);
        virtual void insertInputVariable(InputVariable* inputVariable, int index);
        virtual InputVariable* getInputVariable(int index) const;
        virtual InputVariable* getInputVariable(const std::string& name) const;
        virtual InputVariable* removeInputVariable(int index);
        virtual InputVariable* removeInputVariable(const std::string& name);
        virtual bool hasInputVariable(const std::string& name) const;
        virtual int numberOfInputVariables() const;
        virtual const std::vector<InputVariable*>& inputVariables() const;

        /**
         * Operations for iterable datatype _outputVariables
         */
        virtual void addOutputVariable(OutputVariable* outputVariable);
        virtual void insertOutputVariable(OutputVariable* outputVariable, int index);
        virtual OutputVariable* getOutputVariable(int index) const;
        virtual OutputVariable* getOutputVariable(const std::string& name) const;
        virtual bool hasOutputVariable(const std::string& name) const;
        virtual OutputVariable* removeOutputVariable(int index);
        virtual OutputVariable* removeOutputVariable(const std::string& name);
        virtual int numberOfOutputVariables() const;
        virtual const std::vector<OutputVariable*>& outputVariables() const;

        /**
         * Operations for iterable datatype _ruleblocks
         */
        virtual void addRuleBlock(RuleBlock* ruleblock);
        virtual void insertRuleBlock(RuleBlock* ruleblock, int index);
        virtual RuleBlock* getRuleBlock(int index) const;
        virtual RuleBlock* getRuleBlock(const std::string& name) const;
        virtual bool hasRuleBlock(const std::string& name) const;
        virtual RuleBlock* removeRuleBlock(int index);
        virtual RuleBlock* removeRuleBlock(const std::string& name);
        virtual int numberOfRuleBlocks() const;
        virtual const std::vector<RuleBlock*>& ruleBlocks() const;

        /**
         * Operations for std::vector _hedges
         */
        virtual void addHedge(Hedge* hedge);
        virtual void insertHedge(Hedge* hedge, int index);
        virtual Hedge* getHedge(int index) const;
        virtual Hedge* getHedge(const std::string& name) const;
        virtual bool hasHedge(const std::string& name) const;
        virtual Hedge* removeHedge(int index);
        virtual Hedge* removeHedge(const std::string& name);
        virtual int numberOfHedges() const;
        virtual const std::vector<Hedge*>& hedges() const;

    };

}
#endif /* FL_ENGINE_H */
