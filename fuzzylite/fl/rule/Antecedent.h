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

#ifndef FL_ANTECEDENT_H
#define FL_ANTECEDENT_H

#include "fl/fuzzylite.h"

#include "fl/Complexity.h"

#include <string>

namespace fl {
    class Engine;
    class Rule;
    class TNorm;
    class SNorm;
    class Expression;

    /**
      The Antecedent class is an expression tree that represents and evaluates
      the antecedent of a Rule. The structure of a rule is: `if (antecedent)
      then (consequent)`. The structure of the antecedent of a rule is:

     `if variable is [hedge]* term [(and|or) variable is [hedge]* term]*`

      where `*`-marked elements may appear zero or more times, elements in
      brackets are optional, and elements in parentheses are compulsory.

      @author Juan Rada-Vilela, Ph.D.
      @see Consequent
      @see Rule
      @since 4.0
     */
    class FL_API Antecedent {
    private:
        std::string _text;
        FL_unique_ptr<Expression> _expression;

    public:
        Antecedent();
        virtual ~Antecedent();

        /**
          Sets the antecedent in text
          @param text is the antecedent in text
         */
        virtual void setText(const std::string& text);
        /**
          Gets the antecedent in text
          @return the antecedent in text
         */
        virtual std::string getText() const;

        /**
          Gets the expression tree of the antecedent
          @return the expression tree of the antecedent
         */
        virtual Expression* getExpression() const;

        /**
          Sets the expression tree of the antecedent
          @param expression is the expression tree of the antecedent
         */
        virtual void setExpression(Expression* expression);

        /**
          Indicates whether the antecedent is loaded
          @return whether the antecedent is loaded
         */
        virtual bool isLoaded() const;

        /**
          Unloads the antecedent
         */
        virtual void unload();

        /**
          Loads the antecedent with the text obtained from
          Antecedent::getText() and uses the engine to identify and retrieve
          references to the input variables and output variables as required

          @param engine is the engine from which the rules are part of
         */
        virtual void load(const Engine* engine);
        /**
          Loads the antecedent with the given text and uses the engine to
          identify and retrieve references to the input variables and output
          variables as required

          @param antecedent is the antecedent of the rule in text
          @param engine is the engine from which the rules are part of
         */
        virtual void load(const std::string& antecedent, const Engine* engine);

        /**
          Computes the estimated complexity of calculating the activation degree
          @return the estimated complexity of calculating the activation degree
         */
        virtual Complexity complexity(const TNorm* conjunction, const SNorm* disjunction) const;
        /**
          Computes the estimated complexity of recursively calculating the
          activation degree from the given node
          @return the estimated complexity of recursively calculating the
          activation degree from the given node
         */
        virtual Complexity complexity(const TNorm* conjunction, const SNorm* disjunction,
                const Expression* node) const;


        /**
          Computes the activation degree of the antecedent on the expression
          tree from the given node

          @param conjunction is the conjunction operator from the RuleBlock
          @param disjunction is the disjunction operator from the RuleBlock
          @param node is a node in the expression tree of the antecedent
          @return the activation degree of the antecedent
         */
        virtual scalar activationDegree(const TNorm* conjunction, const SNorm* disjunction,
                const Expression* node) const;

        /**
          Computes the activation degree of the antecedent on the expression
          tree from the root node

          @param conjunction is the conjunction operator from the RuleBlock
          @param disjunction is the disjunction operator from the RuleBlock
          @return the activation degree of the antecedent on the expression tree
         */
        virtual scalar activationDegree(const TNorm* conjunction, const SNorm* disjunction) const;

        /**
          Returns a string representation of the expression tree in infix
          notation

          @return a string representation of the expression tree in infix
          notation
         */
        virtual std::string toString() const;

        /**
          Returns a string represention of the given expression tree utilizing
          prefix notation
          @param node is a node in the expression tree of the antecedent
          @return a string represention of the given expression tree utilizing
          prefix notation
         */
        virtual std::string toPrefix(const Expression* node = fl::null) const;
        /**
          Returns a string represention of the given expression tree utilizing
          infix notation
          @param node is a node in the expression tree of the antecedent
          @return a string represention of the given expression tree utilizing
          infix notation
         */
        virtual std::string toInfix(const Expression* node = fl::null) const;
        /**
          Returns a string represention of the given expression tree utilizing
          postfix notation
          @param node is a node in the expression tree of the antecedent
          @return a string represention of the given expression tree utilizing
          postfix notation
         */
        virtual std::string toPostfix(const Expression* node = fl::null) const;


    private:
        FL_DISABLE_COPY(Antecedent)
    };
}
#endif /* FL_ANTECEDENT_H */
