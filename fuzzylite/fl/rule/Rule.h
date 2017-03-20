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

#ifndef FL_RULE_H
#define FL_RULE_H

#include "fl/fuzzylite.h"

#include "fl/rule/Consequent.h"
#include "fl/rule/Antecedent.h"

#include <map>
#include <string>


namespace fl {
    class Engine;
    class Hedge;
    class TNorm;
    class SNorm;

    /**
      The Rule class is a conditional statement that contributes to the control
      of an Engine. Each rule consists of an Antecedent and a Consequent, each
      of which comprises propositions in the form `variable is term`. The
      propositions in the Antecedent can be connected by the conjunctive `and`
      or the disjunctive `or`, both of which are fuzzy logic operators (TNorm
      and SNorm, respectively). Differently, the propositions in the Consequent
      are independent from each other and are separated with a symbolic `and`.
      The Term in any proposition can be preceded by a Hedge that modifies its
      membership function to model cases such as Very, Somewhat, Seldom and
      Not. Additionally, the contribution of a rule to the control of the
      engine can be determined by its weight @f$w \in [0.0, 1.0]@f$, which is
      equal to 1.0 if omitted. The structure of a rule is the following: `if
      (antecedent) then (consequent) [with weight]`. The structures of
      the antecedent and the consequent are:

     `if variable is [hedge]* term [(and|or) variable is [hedge]* term]*`

     `then variable is [hedge]* term [and variable is [hedge]* term]* [with w]?`

      where elements in brackets are optional, elements in parentheses are
      compulsory, `*`-marked elements may appear zero or more times, and
     `?`-marked elements may appear once or not at all.

      @author Juan Rada-Vilela, Ph.D.
      @see Antecedent
      @see Consequent
      @see Hedge
      @see RuleBlock
      @since 4.0
     */
    class FL_API Rule {
    private:
        bool _enabled;
        std::string _text;
        scalar _weight;
        scalar _activationDegree;
        bool _triggered;
        FL_unique_ptr<Antecedent> _antecedent;
        FL_unique_ptr<Consequent> _consequent;

    public:
        explicit Rule(const std::string& text = "", scalar weight = 1.0);
        Rule(const Rule& other);
        Rule& operator=(const Rule& other);
        virtual ~Rule();
        FL_DEFAULT_MOVE(Rule)

        /**
         Sets whether the rule is enabled. An enabled rule will be fired, whereas
         a disabled rule will not.
         @param enabled determines whether the rule is enabled
         */
        virtual void setEnabled(bool enabled);

        /**
          Gets whether the rule is enabled.  An enabled rule will be fired, whereas
          a disabled rule will not.
          @return whether the rule is enabled
         */
        virtual bool isEnabled() const;

        /**
          Sets the text of the rule
          @param text is the text of the rule
         */
        virtual void setText(const std::string& text);
        /**
          Gets the text of the rule
          @return the text of the rule
         */
        virtual std::string getText() const;

        /**
          Sets the weight of the rule
          @param weight is the weight of the rule
         */
        virtual void setWeight(scalar weight);
        /**
          Gets the weight of the rule
          @return the weight of the rule
         */
        virtual scalar getWeight() const;

        /**
          Sets the antecedent of the rule
          @param antecedent is the antecedent of the rule
         */
        virtual void setAntecedent(Antecedent* antecedent);
        /**
          Gets the antecedent of the rule
          @return the antecedent of the rule
         */
        virtual Antecedent* getAntecedent() const;

        /**
          Sets the consequent of the rule
          @param consequent the consequent of the rule
         */
        virtual void setConsequent(Consequent* consequent);
        /**
          Gets the consequent of the rule
          @return the consequent of the rule
         */
        virtual Consequent* getConsequent() const;

        /**
          Sets the activation degree of the rule
          @param activationDegree is the activation degree of the rule
         */
        virtual void setActivationDegree(scalar activationDegree);

        /**
          Gets the activation degree of the rule
          @return the activation degree of the rule
         */
        virtual scalar getActivationDegree() const;

        /**
          Activates the rule by computing its activation degree using the given
          conjunction and disjunction operators
          @param conjunction is the conjunction operator
          @param disjunction is the disjunction operator
          @return the activation degree of the rule
         */
        virtual scalar activateWith(const TNorm* conjunction, const SNorm* disjunction);

        /**
          Deactivates the rule
         */
        virtual void deactivate();

        /**
          Triggers the rule's implication (if the rule is enabled) using the
          given implication operator and the underlying activation degree
          @param implication is the implication operator
         */
        virtual void trigger(const TNorm* implication);

        /**
          Indicates whether the rule's implication was triggered
          @return whether the rule's implication was triggered
         */
        virtual bool isTriggered() const;

        /**
          Returns a string representation of the rule in the FuzzyLite Language
          @return a string representation of the rule in the FuzzyLite Language
         */
        virtual std::string toString() const;

        /**
          Indicates whether the rule is loaded
          @return whether the rule is loaded
         */
        virtual bool isLoaded() const;
        /**
          Unloads the rule
         */
        virtual void unload();
        /**
          Loads the rule with the text from Rule::getText(), and uses the
          engine to identify and retrieve references to the input variables and
          output variables as required
          @param engine is the engine from which the rule is part of
         */
        virtual void load(const Engine* engine);
        /**
          Loads the rule with the given text, and uses the engine to identify
          and retrieve references to the input variables and output variables
          as required

          @param rule is the rule in text
          @param engine is the engine from which the rule is part of
         */
        virtual void load(const std::string& rule, const Engine* engine);

        /**
          Creates a clone of the rule without the rule being loaded
          @return a clone of the rule without the rule being loaded
         */
        virtual Rule* clone() const;

        /**
          Parses and creates a new rule based on the text passed
          @param rule is the rule in text
          @param engine is the engine from which the rule is part of
          @return a new rule parsed from the given text
         */
        static Rule* parse(const std::string& rule, const Engine* engine);

        /**
          Returns a string representation of the `if` keyword in rules
          @return a string representation of the `if` keyword in rules
         */
        inline static std::string ifKeyword() {
            return "if";
        }

        /**
          Returns a string representation of the `is` keyword in rules
          @return a string representation of the `is` keyword in rules
         */
        inline static std::string isKeyword() {
            return "is";
        }

        /**
          Returns a string representation of the `then` keyword in rules
          @return a string representation of the `then` keyword in rules
         */
        inline static std::string thenKeyword() {
            return "then";
        }

        /**
          Returns a string representation of the `and` keyword in rules
          @return a string representation of the `and` keyword in rules
         */
        inline static std::string andKeyword() {
            return "and";
        }

        /**
          Returns a string representation of the `or` keyword in rules
          @return a string representation of the `or` keyword in rules
         */
        inline static std::string orKeyword() {
            return "or";
        }

        /**
          Returns a string representation of the `with` keyword in rules
          @return a string representation of the `with` keyword in rules
         */
        inline static std::string withKeyword() {
            return "with";
        }

        /**
          Computes the estimated complexity of calculating the activation degree
          of the rule
          @param conjunction is the conjunction operator
          @param disjunction is the disjunction operator
          @return the estimated complexity of calculating the activation degree
          of the rule
         */
        virtual Complexity complexityOfActivation(const TNorm* conjunction,
                const SNorm* disjunction) const;

        /**
          Computes the estimated complexity of firing the rule
          @param implication is the implication operator
          @return the estimated complexity of firing the rule
         */
        virtual Complexity complexityOfFiring(const TNorm* implication) const;

        /**
         Returns the estimated complexity of activating and firing the rule
         @param conjunction is the conjunction operator
         @param disjunction is the disjunction operator
         @param implication is the implication operator
         @return the estimated complexity of activating and firing the rule
         */
        virtual Complexity complexity(const TNorm* conjunction,
                const SNorm* disjunction, const TNorm* implication) const;

    };
}

#endif /* FL_RULE_H */
