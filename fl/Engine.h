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

#ifndef FL_ENGINE_H
#define FL_ENGINE_H

#include "fl/fuzzylite.h"

#include "fl/Complexity.h"

#include <string>
#include <vector>

namespace fl {

    class InputVariable;
    class OutputVariable;
    class Variable;
    class RuleBlock;
    class Hedge;
    class TNorm;
    class SNorm;
    class Defuzzifier;
    class Activation;

    /**
      The Engine class is the core class of the library as it groups the
      necessary components of a fuzzy logic controller.

      @author Juan Rada-Vilela, Ph.D.
      @see InputVariable
      @see OutputVariable
      @see RuleBlock
      @since 4.0
     */
    class FL_API Engine {
    private:
        std::string _name;
        std::string _description;
        std::vector<InputVariable*> _inputVariables;
        std::vector<OutputVariable*> _outputVariables;
        std::vector<RuleBlock*> _ruleBlocks;

        void copyFrom(const Engine& source);

    protected:
        void updateReferences() const;

    public:
        explicit Engine(const std::string& name = "");
        Engine(const Engine& other);
        Engine& operator=(const Engine& other);
        virtual ~Engine();
        FL_DEFAULT_MOVE(Engine)

        /**
          Configures the engine with the given operators
          @param conjunction is a TNorm registered in the TNormFactory
          @param disjunction is an SNorm registered in the SNormFactory
          @param implication is an TNorm registered in the TNormFactory
          @param aggregation is an SNorm registered in the SNormFactory
          @param defuzzifier is a defuzzifier registered in the DefuzzifierFactory
          @param activation is an activation method registered in the ActivationFactory
         */
        virtual void configure(const std::string& conjunction,
                const std::string& disjunction,
                const std::string& implication,
                const std::string& aggregation,
                const std::string& defuzzifier,
                const std::string& activation);

        /**
          Configures the engine with clones of the given object operators, taking
          ownership of the objects.

          @param conjunction is the operator to process the propositions joined
          by `and` in the antecedent of the rules
          @param disjunction is the operator to process the propositions
          joined by `or` in the antecedent of the rules
          @param implication is the operator to modify the consequents of the
          rules based on the activation degree of the antecedents of the rules
          @param aggregation is the operator to aggregate the resulting
          implications of the rules
          @param defuzzifier is the operator to transform the aggregated
          implications into a single scalar value
          @param activation is the activation method to activate and fire the
          rule blocks
         */
        virtual void configure(TNorm* conjunction, SNorm* disjunction,
                TNorm* implication, SNorm* aggregation,
                Defuzzifier* defuzzifier, Activation* activation);

        /**
          Indicates whether the engine has been configured correctly and is
          ready for operation. In more advanced engines, the result of this
          method should be taken as a suggestion and not as a prerequisite to
          operate the engine.

          @param status (if not null) contains the configuration errors of the engine
          @return whether the engine is ready to operate
         */
        virtual bool isReady(std::string* status = fl::null) const;

        /**
         Computes the estimated complexity of operation of the engine
         @return the estimated complexity of operation of the engine
         */
        virtual Complexity complexity() const;
        /**
          Processes the engine in its current state as follows: (a) Clears the
          aggregated fuzzy output variables, (b) Activates the rule blocks, and
          (c) Defuzzifies the output variables
          @see Aggregated::clear()
          @see RuleBlock::activate()
          @see OutputVariable::defuzzify()
         */
        virtual void process();

        /**
          Restarts the engine by setting the values of the input variables to
          fl::nan and clearing the output variables
          @see Variable::setValue()
          @see OutputVariable::clear()
         */
        virtual void restart();

        /**
          Sets the name of the engine
          @param name is the name of the engine
         */
        virtual void setName(const std::string& name);
        /**
          Gets the name of the engine
          @return the name of the engine
         */
        virtual std::string getName() const;

        /**
          Sets the description of the engine
          @param description is the description of the engine
         */
        virtual void setDescription(const std::string& description);
        /**
          Gets the description of the engine
          @return the description of the engine
         */
        virtual std::string getDescription() const;

        /**
          Sets the value of the given input variable.
          The cost of this method is O(n), where n is the number of
          input variables in the engine. For performance, please get the
          variables by index.
          @param name is the name of the input variable
          @param value is the value for the input variable
         */
        virtual void setInputValue(const std::string& name, scalar value);
        /**
          Gets the value of the given output variable.
          The cost of this method is O(n), where n is the number of
          output variables in the engine. For performance, please get the
          variables by index.
          @param name is the name of the output variable
          @return the value of the given output variable
         */
        virtual scalar getOutputValue(const std::string& name);

        /**
        Returns a string representation of the engine in the FuzzyLite
        Language
        @return a string representation of the engine in the FuzzyLite
        Language
         */
        virtual std::string toString() const;

        enum Type {
            /**Mamdani: When the output variables have IntegralDefuzzifier%s*/
            Mamdani,
            /**Larsen: When Mamdani and AlgebraicProduct is the implication operator of
            the rule blocks */
            Larsen,
            /**TakagiSugeno: When output variables have WeightedDefuzzifier%s of type
            TakagiSugeno and the output variables have Constant, Linear, or
            Function terms*/
            TakagiSugeno,
            /**Tsukamoto: When output variables have WeightedDefuzzifier%s of type
            Tsukamoto and the output variables only have monotonic terms
            (Concave, Ramp, Sigmoid, SShape, and ZShape)*/
            Tsukamoto,
            /**InverseTsukamoto: When output variables have WeightedDefuzzifier%s of type
            TakagiSugeno and the output variables do not only have Constant,
            Linear or Function terms*/
            InverseTsukamoto,
            /**Hybrid: When output variables have different defuzzifiers*/
            Hybrid,
            /**Unknown: When output variables have no defuzzifiers*/
            Unknown
        };
        /**
          Infers the type of the engine based on its current configuration

          @param name stores a string representation of the engine type (if the
          pointer passed is not `fl::null`)
          @param reason stores a string representation explaining the reasons
          for the inferred type (if the pointer passed is not `fl::null`)
          @return the inferred type of the engine based on its current
          configuration
         */
        virtual Type type(std::string* name = fl::null, std::string* reason = fl::null) const;

        /**
          Creates a clone of the engine
          @return a clone of the engine
         */
        virtual Engine* clone() const;

        /**
          Returns a vector that contains the input variables followed by the
          output variables in the order of insertion

          @return a vector that contains the input variables followed by the
          output variables in the order of insertion
         */
        virtual std::vector<Variable*> variables() const;

        /**
          Adds the input variable
          @param inputVariable is the input variable
         */
        virtual void addInputVariable(InputVariable* inputVariable);
        /**
          Sets the input variable at the given index
          @param inputVariable is the input variable to set
          @param index is the index at which the input variable is to be stored
          @return the input variable previously stored at the given index
         */
        virtual InputVariable* setInputVariable(InputVariable* inputVariable, std::size_t index);
        /**
          Inserts the input variable at the given index, shifting other
          variables one position to the right
          @param inputVariable is the input variable to insert
          @param index is the index at which the input variable is to be
          inserted
         */
        virtual void insertInputVariable(InputVariable* inputVariable, std::size_t index);
        /**
          Gets the input variable at the given index
          @param index is the given index
          @return the input variable at the given index
         */
        virtual InputVariable* getInputVariable(std::size_t index) const;
        /**
          Gets the input variable of the given name after iterating the input
          variables. The cost of this method is O(n), where n is the number of
          input variables in the engine. For performance, please get the
          variables by index.
          @param name is the name of the input variable
          @return input variable of the given name
          @throws fl::Exception if there is no variable with the given name
         */
        virtual InputVariable* getInputVariable(const std::string& name) const;
        /**
          Removes the input variable at the given index (without deleting it)
          and shifts the remaining input variables one position to the left
          @param index is the given index
          @return the input variable at the given index
         */
        virtual InputVariable* removeInputVariable(std::size_t index);
        /**
          Removes the input variable of the given name (without deleting it) and
          shifts the remaining input variables one position to the left
          @param name is the name of the input variable
          @return the input variable of the given name
          @throws fl::Exception if there is no variable with the given name
         */
        virtual InputVariable* removeInputVariable(const std::string& name);
        /**
          Indicates whether an input variable of the given name is in the input
          variables
          @param name is the name of the input variable
          @return whether an input variable is registered with the given name
         */
        virtual bool hasInputVariable(const std::string& name) const;
        /**
          Returns the number of input variables added to the engine
          @return the number of input variables added to the engine
         */
        virtual std::size_t numberOfInputVariables() const;
        /**
          Returns an immutable vector of input variables
          @return an immutable vector of input variables
         */
        virtual const std::vector<InputVariable*>& inputVariables() const;
        /**
          Sets the vector of input variables
          @param inputVariables is the vector of input variables
         */
        virtual void setInputVariables(const std::vector<InputVariable*>& inputVariables);
        /**
          Returns a mutable vector of input variables
          @return a mutable vector of input variables
         */
        virtual std::vector<InputVariable*>& inputVariables();

        /**
          Adds the output variable
          @param outputVariable is the output variable
         */
        virtual void addOutputVariable(OutputVariable* outputVariable);
        /**
          Sets the output variable at the given index
          @param outputVariable is the output variable to set
          @param index is the index at which the output variable is to be stored
          @return the output variable previously stored at the given index
         */
        virtual OutputVariable* setOutputVariable(OutputVariable* outputVariable, std::size_t index);
        /**
          Inserts the output variable at the given index, shifting other
          variables one position to the right
          @param outputVariable is the output variable to insert
          @param index is the index at which the output variable is to be inserted
         */
        virtual void insertOutputVariable(OutputVariable* outputVariable, std::size_t index);
        /**
          Gets the output variable at the given index
          @param index is the given index
          @return the output variable at the given index
         */
        virtual OutputVariable* getOutputVariable(std::size_t index) const;
        /**
          Gets the output variable of the given name after iterating the output
          variables. The cost of this method is O(n), where n is the number of
          output variables in the engine. For performance, please get the
          variables by index.
          @param name is the name of the output variable
          @return output variable of the given name
          @throws fl::Exception if there is no variable with the given name
         */
        virtual OutputVariable* getOutputVariable(const std::string& name) const;

        /**
          Indicates whether an output variable of the given name is in the
          output variables
          @param name is the name of the output variable
          @return whether an output variable is registered with the given name
         */
        virtual bool hasOutputVariable(const std::string& name) const;
        /**
          Removes the output variable at the given index (without deleting it)
          and shifts the remaining output variables one position to the left
          @param index is the given index
          @return the output variable at the given index
         */
        virtual OutputVariable* removeOutputVariable(std::size_t index);
        /**
          Removes the output variable of the given name (without deleting it)
          and shifts the remaining output variables one position to the left
          @param name is the name of the output variable
          @return the output variable of the given name
          @throws fl::Exception if there is no variable with the given name
         */
        virtual OutputVariable* removeOutputVariable(const std::string& name);
        /**
          Returns the number of output variables added to the engine
          @return the number of output variables added to the engine
         */
        virtual std::size_t numberOfOutputVariables() const;
        /**
          Returns an immutable vector of output variables
          @return an immutable vector of output variables
         */
        virtual const std::vector<OutputVariable*>& outputVariables() const;
        /**
          Sets the vector of output variables
          @param outputVariables is the vector of output variables
         */
        virtual void setOutputVariables(const std::vector<OutputVariable*>& outputVariables);
        /**
          Returns a mutable vector of output variables
          @return a mutable vector of output variables
         */
        virtual std::vector<OutputVariable*>& outputVariables();

        /**
          Adds the rule block
          @param ruleBlock is the rule block
         */
        virtual void addRuleBlock(RuleBlock* ruleBlock);
        /**
          Sets the rule block at the given index
          @param ruleBlock is the rule block to set
          @param index is the index at which the rule block is to be stored
          @return the rule block previously stored at the given index
         */
        virtual RuleBlock* setRuleBlock(RuleBlock* ruleBlock, std::size_t index);
        /**
          Inserts the rule block at the given index, shifting other blocks one
          position to the right
          @param ruleBlock is the rule block to insert
          @param index is the index at which the rule block is to be inserted
         */
        virtual void insertRuleBlock(RuleBlock* ruleBlock, std::size_t index);
        /**
          Gets the rule block at the given index
          @param index is the given index
          @return the rule block at the given index
         */
        virtual RuleBlock* getRuleBlock(std::size_t index) const;
        /**
          Gets the rule block of the given name after iterating the rule blocks.
          The cost of this method is O(n), where n is the number of
          rule blocks in the engine. For performance, please get the rule blocks
          by index.
          @param name is the name of the rule block
          @return rule block of the given name
          @throws fl::Exception if there is no block with the given name
         */
        virtual RuleBlock* getRuleBlock(const std::string& name) const;
        /**
          Indicates whether an rule block of the given name is in the rule
          blocks
          @param name is the name of the rule block
          @return whether an rule block of the given name is in the rule blocks

         */
        virtual bool hasRuleBlock(const std::string& name) const;
        /**
          Removes the rule block at the given index (without deleting it) and
          shifts the remaining rule blocks one position to the left
          @param index is the given index
          @return the rule block at the given index
         */
        virtual RuleBlock* removeRuleBlock(std::size_t index);
        /**
          Removes the rule block of the given name (without deleting it) and
          shifts the remaining rule blocks one position to the left
          @param name is the name of the rule block
          @return the rule block of the given name
          @throws fl::Exception if there is no rule block with the given name
         */
        virtual RuleBlock* removeRuleBlock(const std::string& name);
        /**
          Returns the number of rule blocks added to the engine
          @return the number of rule blocks added to the engine
         */
        virtual std::size_t numberOfRuleBlocks() const;
        /**
          Returns an immutable vector of rule blocks
          @return an immutable vector of rule blocks
         */
        virtual const std::vector<RuleBlock*>& ruleBlocks() const;
        /**
          Sets the vector of rule blocks
          @param ruleBlocks is the vector of rule blocks
         */
        virtual void setRuleBlocks(const std::vector<RuleBlock*>& ruleBlocks);
        /**
          Returns a mutable vector of rule blocks
          @return a mutable vector of rule blocks
         */
        virtual std::vector<RuleBlock*>& ruleBlocks();

    };
}
#endif /* FL_ENGINE_H */
