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

#ifndef FL_RULEBLOCK_H
#define FL_RULEBLOCK_H

#include "fl/fuzzylite.h"

#include "fl/activation/Activation.h"
#include "fl/Complexity.h"

#include <string>
#include <vector>


namespace fl {

    class Engine;
    class Rule;
    class TNorm;
    class SNorm;

    /**
      The RuleBlock class contains a set of Rule%s and fuzzy logic
      operators required to control an Engine.

      @author Juan Rada-Vilela, Ph.D.
      @see Engine
      @see Rule
      @see Antecedent
      @see Consequent
      @since 4.0
     */
    class FL_API RuleBlock {
    private:
        bool _enabled;
        std::string _name;
        std::string _description;
        std::vector<Rule*> _rules;
        FL_unique_ptr<TNorm> _conjunction;
        FL_unique_ptr<SNorm> _disjunction;
        FL_unique_ptr<TNorm> _implication;
        FL_unique_ptr<Activation> _activation;

        void copyFrom(const RuleBlock& source);

    public:
        explicit RuleBlock(const std::string& name = "");
        RuleBlock(const RuleBlock& other);
        RuleBlock& operator=(const RuleBlock& other);
        virtual ~RuleBlock();
        FL_DEFAULT_MOVE(RuleBlock)

        /**
        Enables the rule block
        @param enabled whether the rule block is enabled
         */
        virtual void setEnabled(bool enabled);
        /**
          Indicates whether the rule block is enabled
          @return whether the rule block is enabled
         */
        virtual bool isEnabled() const;
        
        /**
          Activates the rule block
         */
        virtual void activate();

        /**
          Sets the name of the rule block
          @param name is the name of the rule block
         */
        virtual void setName(std::string name);
        /**
          Gets the name of the rule block
          @return the name of the rule block
         */
        virtual std::string getName() const;

        /**
          Gets the description of the rule block
          @return the description of the rule block
         */
        virtual std::string getDescription() const;

        /**
          Sets the description of the rule block
          @param description is the description of the rule block
         */
        virtual void setDescription(const std::string& description);
        /**
          Sets the conjunction operator
          @param conjunction is the conjunction operator
         */
        virtual void setConjunction(TNorm* conjunction);
        /**
          Gets the conjunction operator
          @return the conjunction operator
         */
        virtual TNorm* getConjunction() const;

        /**
          Sets the disjunction operator
          @param disjunction is the disjunction operator
         */
        virtual void setDisjunction(SNorm* disjunction);
        /**
          Gets the disjunction operator
          @return the disjunction operator
         */
        virtual SNorm* getDisjunction() const;

        /**
          Sets the implication operator
          @param implication is the implication operator
         */
        virtual void setImplication(TNorm* implication);
        /**
          Gets the implication operator
          @return the implication operator
         */
        virtual TNorm* getImplication() const;

        /**
          Sets the activation method
          @param activation is the activation method
         */
        virtual void setActivation(Activation* activation);
        /**
          Gets the activation method
          @return the activation method
         */
        virtual Activation* getActivation() const;

        /**
          Unloads all the rules in the rule block
         */
        virtual void unloadRules() const;
        /**
          Loads all the rules into the rule block
          @param engine is the engine where this rule block is registered
         */
        virtual void loadRules(const Engine* engine);

        /**
          Unloads all the rules in the rule block and then loads each rule again
          @param engine is the engine where this rule block is registered
         */
        virtual void reloadRules(const Engine* engine);

        /**
          Returns a string representation of the rule block in the FuzzyLite
          Language
          @return a string representation of the rule block in the  FuzzyLite
          Language
         */
        virtual std::string toString() const;

        /**
          Returns the estimated complexity of activating the rule block
          @return the estimated complexity of activating the rule block
         */
        virtual Complexity complexity() const;

        /**
          Adds the given rule to the rule block
          @param rule is the rule to add
         */
        virtual void addRule(Rule* rule);
        /**
          Inserts the rule at the specified index, shifting other rules to
          the right
          @param rule is the rule to insert
          @param index is the index at which to insert the rule
         */
        virtual void insertRule(Rule* rule, std::size_t index);
        /**
          Gets the rule at the specified index
          @param index is the index at which the rule is retrieved
          @return the rule at the specified index
         */
        virtual Rule* getRule(std::size_t index) const;
        /**
          Removes the rule at the specified index
          @param index is the index at which the rule will be removed,
          shifting other rules to the left
          @return the rule at the specified index
         */
        virtual Rule* removeRule(std::size_t index);
        /**
          Returns the number of rules added to the rule block
          @return the number of rules added to the rule block
         */
        virtual std::size_t numberOfRules() const;
        /**
          Sets the rules of the rule block
          @param rules is a vector of rules
         */
        virtual void setRules(const std::vector<Rule*>& rules);
        /**
          Returns an immutable vector of the rules added to the rule block
          @return an immutable vector of the rules added to the rule block
         */
        virtual const std::vector<Rule*>& rules() const;
        /**
          Returns a mutable vector of the rules added to the rule block
          @return a mutable vector of the rules added to the rule block
         */
        virtual std::vector<Rule*>& rules();

        /**
          Creates a clone of the rule block without the rules being loaded
          @return a clone of the rule block without the rules being loaded
         */
        virtual RuleBlock* clone() const;

    };
}
#endif /* RULEBLOCK_H */
