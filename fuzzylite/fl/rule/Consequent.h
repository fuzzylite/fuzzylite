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

#ifndef FL_CONSEQUENT_H
#define FL_CONSEQUENT_H

#include "fl/fuzzylite.h"

#include "fl/Complexity.h"

#include <string>
#include <vector>

namespace fl {
    class Engine;
    class Rule;
    class Proposition;
    class TNorm;

    /**
      The Consequent class is a proposition set that represents and evaluates
      the consequent of a Rule.. The structure of a rule is: `if (antecedent)
      then (consequent)`. The structure of the consequent of a rule is:

      `then variable is [hedge]* term [and variable is [hedge]* term]* [with
      w]?`

      where `*`-marked elements may appear zero or more times, elements in
      brackets are optional, elements in parentheses are compulsory, and
      `?`-marked elements may appear once or not at all.

      @author Juan Rada-Vilela, Ph.D.
      @see Antecedent
      @see Rule
      @since 4.0
     */

    class FL_API Consequent {
    private:
        std::string _text;
        std::vector<Proposition*> _conclusions;

    public:
        Consequent();
        virtual ~Consequent();

        /**
          Sets the text of the consequent
          @param text is the text of the consequent
         */
        virtual void setText(const std::string& text);
        /**
          Gets the text of the consequent
          @return the text of the consequent
         */
        virtual std::string getText() const;

        /**
          Computes the estimated complexity of modifying the consequents
          @return the estimated complexity of modifying the consequents
         */
        virtual Complexity complexity(const TNorm* implication) const;
        /**
          Returns an immutable vector of the propositions that represent the 
          Consequent of a Rule
          @return an immutable vector of the set of propositions that represent
          the Consequent of a Rule
         */
        virtual const std::vector<Proposition*>& conclusions() const;

        /**
          Returns the vector of propositions that represent the Consequent of a 
          Rule
          @return the vector of propositions that represent the Consequent of a 
          Rule
         */
        virtual std::vector<Proposition*>& conclusions();

        /**
          Indicates whether the consequent is loaded
          @return whether the consequent is loaded
         */
        virtual bool isLoaded();
        /**
          Unloads the consequent
         */
        virtual void unload();
        /**
          Loads the consequent with text given from Consequent::getText() and
          uses the engine to identify and retrieve references to the input
          variables and output variables as required
          @param engine is the engine from which the rules are part of
         */
        virtual void load(const Engine* engine);
        /**
          Loads the consequent with the given text and uses the engine to
          identify and retrieve references to the input variables and output
          variables as required
          @param consequent is the consequent of the rule in text
          @param engine is the engine from which the rules are part of
         */
        virtual void load(const std::string& consequent, const Engine* engine);

        /**
          Modifies the proposition set according to the activation degree
          (computed in the Antecedent of the Rule) and the implication operator
          (given in the RuleBlock)
          @param activationDegree is the activation degree computed in the
          Antecedent of the Rule
          @param implication is the implication operator configured in the
          RuleBlock
         */
        virtual void modify(scalar activationDegree, const TNorm* implication);

        /**
          Returns a string representation of the Consequent
          @return a string representation of the Consequent
         */
        virtual std::string toString() const;

    private:
        FL_DISABLE_COPY(Consequent)
    };
}
#endif /* FL_CONSEQUENT_H */
