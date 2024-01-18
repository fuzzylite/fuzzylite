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

#ifndef FL_EXPRESSION_H
#define FL_EXPRESSION_H

#include "fl/fuzzylite.h"

#include <string>
#include <vector>



namespace fl {
    class Variable;
    class Hedge;
    class Term;

    /**
      The Expression class is the base class to build an expression tree.

      @author Juan Rada-Vilela, Ph.D.
      @see Antecedent
      @see Consequent
      @see Rule
      @since 4.0
     */
    class FL_API Expression {
    public:

        enum Type {
            Proposition, Operator
        };
        Expression();
        virtual ~Expression();

        /**
          Returns the type of the expression
          @return the type of the expression
         */
        virtual Type type() const = 0;
        virtual std::string toString() const = 0;

    private:
        FL_DISABLE_COPY(Expression)
    };

    /**
      The Proposition class is an Expression that represents a terminal node in
      the expression tree as `variable is [hedge]* term`.

      @author Juan Rada-Vilela, Ph.D.
      @see Antecedent
      @see Consequent
      @see Rule
      @since 4.0
     */
    class FL_API Proposition FL_IFINAL : public Expression {
    public:
        /**Variable in `variable is [hedge]* term`*/
        Variable* variable;
        /**Hedge%s in `variable is [hedge]* term`, owned by the object,
         destroyed on destructor*/
        std::vector<Hedge*> hedges;
        /**Term in `variable is [hedge]* term`*/
        Term* term;

        Proposition();
        ~Proposition() FL_IOVERRIDE;

        Expression::Type type() const FL_IOVERRIDE;

        /**
          Returns a string representation of the proposition
          @return a string representation of the proposition
         */
        std::string toString() const FL_IOVERRIDE;


    private:
        FL_DISABLE_COPY(Proposition)
    };

    /**
      The Operator class is an Expression that represents a non-terminal node
      in the expression tree as a binary operator (i.e., `and` or `or`) on two
      Expression nodes.

      @author Juan Rada-Vilela, Ph.D.
      @see Antecedent
      @see Consequent
      @see Rule
      @since 4.0
     */
    class FL_API Operator FL_IFINAL : public Expression {
    public:
        /**Name of the operator*/
        std::string name;
        /**Left expression in the binary tree*/
        Expression* left;
        /**Right expression in the binary tree*/
        Expression* right;

        Operator();
        ~Operator() FL_IOVERRIDE;

        Expression::Type type() const FL_IOVERRIDE;

        /**
          Returns the name of the operator
          @return the name of the operator
         */
        std::string toString() const FL_IOVERRIDE;

    private:
        FL_DISABLE_COPY(Operator)

    };
}
#endif /* FL_FUZZYEXPRESSION_H */
