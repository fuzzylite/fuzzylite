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
    private:
        void copyFrom(const Engine& source);
    protected:
        /** The name of the engine used for information only*/
        std::string _name;
        /** The input variables registered in the engine*/
        std::vector<InputVariable*> _inputVariables;
        /** The output variables registered in the engine*/
        std::vector<OutputVariable*> _outputVariables;
        /** The rule blocks registered in the engine*/
        std::vector<RuleBlock*> _ruleblocks;

    public:
        Engine(const std::string& name = "");
        Engine(const Engine& source);
        Engine& operator=(const Engine& rhs);
        virtual ~Engine();

        virtual void configure(const std::string& activationT,
                const std::string& accumulationS,
                const std::string& defuzzifier,
                int resolution = IntegralDefuzzifier::defaultResolution());

        virtual void configure(const std::string& conjunctionT,
                const std::string& disjunctionS,
                const std::string& activationT,
                const std::string& accumulationS,
                const std::string& defuzzifier,
                int resolution = IntegralDefuzzifier::defaultResolution());

        virtual void configure(const TNorm* activation, const SNorm* accumulation,
                const Defuzzifier* defuzzifier);

        virtual void configure(const TNorm* conjunction, const SNorm* disjunction,
                const TNorm* activation, const SNorm* accumulation,
                const Defuzzifier* defuzzifier);

        virtual bool isReady(std::string* status = NULL) const;

        virtual void process();

        virtual void restart();

        virtual void setName(const std::string& name);
        virtual std::string getName() const;

        virtual void setInputValue(const std::string& name, scalar value);
        virtual scalar getOutputValue(const std::string& name);

        virtual std::string toString() const;

        enum Type {
            None, Mamdani, Larsen, TakagiSugeno,
            Tsukamoto, InverseTsukamoto, Unknown
        };
        virtual Type type(std::string* name = NULL) const;

        /**
         * Operations for iterable datatype _inputVariables
         */
        virtual void addInputVariable(InputVariable* inputVariable);
        virtual InputVariable* setInputVariable(InputVariable* inputVariable, int index);
        virtual void insertInputVariable(InputVariable* inputVariable, int index);
        virtual InputVariable* getInputVariable(int index) const;
        virtual InputVariable* getInputVariable(const std::string& name) const;
        virtual InputVariable* removeInputVariable(int index);
        virtual InputVariable* removeInputVariable(const std::string& name);
        virtual bool hasInputVariable(const std::string& name) const;
        virtual int numberOfInputVariables() const;
        virtual const std::vector<InputVariable*>& inputVariables() const;
        virtual void setInputVariables(const std::vector<InputVariable*>& inputVariables);
        virtual std::vector<InputVariable*>& inputVariables();

        /**
         * Operations for iterable datatype _outputVariables
         */
        virtual void addOutputVariable(OutputVariable* outputVariable);
        virtual OutputVariable* setOutputVariable(OutputVariable* outputVariable, int index);
        virtual void insertOutputVariable(OutputVariable* outputVariable, int index);
        virtual OutputVariable* getOutputVariable(int index) const;
        virtual OutputVariable* getOutputVariable(const std::string& name) const;
        virtual bool hasOutputVariable(const std::string& name) const;
        virtual OutputVariable* removeOutputVariable(int index);
        virtual OutputVariable* removeOutputVariable(const std::string& name);
        virtual int numberOfOutputVariables() const;
        virtual const std::vector<OutputVariable*>& outputVariables() const;
        virtual void setOutputVariables(const std::vector<OutputVariable*>& outputVariables);
        virtual std::vector<OutputVariable*>& outputVariables();

        /**
         * Operations for iterable datatype _ruleblocks
         */
        virtual void addRuleBlock(RuleBlock* ruleblock);
        virtual RuleBlock* setRuleBlock(RuleBlock* ruleBlock, int index);
        virtual void insertRuleBlock(RuleBlock* ruleblock, int index);
        virtual RuleBlock* getRuleBlock(int index) const;
        virtual RuleBlock* getRuleBlock(const std::string& name) const;
        virtual bool hasRuleBlock(const std::string& name) const;
        virtual RuleBlock* removeRuleBlock(int index);
        virtual RuleBlock* removeRuleBlock(const std::string& name);
        virtual int numberOfRuleBlocks() const;
        virtual const std::vector<RuleBlock*>& ruleBlocks() const;
        virtual void setRuleBlocks(const std::vector<RuleBlock*>& ruleBlocks);
        virtual std::vector<RuleBlock*>& ruleBlocks();

    };

}
#endif /* FL_ENGINE_H */
