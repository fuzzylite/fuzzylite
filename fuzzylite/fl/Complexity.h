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

#ifndef FL_COMPLEXITY_H
#define FL_COMPLEXITY_H

#include "fl/fuzzylite.h"

#include <vector>

namespace fl {
    class Engine;
    class InputVariable;
    class OutputVariable;
    class Variable;
    class RuleBlock;
    class Rule;

    /**
      The Complexity class is used throughout the library to estimate the
      computational cost of the different components of the library

      @author Juan Rada-Vilela, Ph.D.
      @see Engine
      @see Variable
      @see OutputVariable
      @see RuleBlock
      @see Activation
      @see Rule
      @see Antecedent
      @see Consequent
      @see Hedge
      @see Norm
      @since 6.0
     */

    class FL_API Complexity {
    private:
        scalar _comparison;
        scalar _arithmetic;
        scalar _function;

    public:
        explicit Complexity(scalar all = 0.0);
        explicit Complexity(scalar comparison, scalar arithmetic, scalar function);
        virtual ~Complexity();
        FL_DEFAULT_COPY_AND_MOVE(Complexity)

        Complexity& operator+=(const Complexity& other);
        Complexity& operator-=(const Complexity& other);
        Complexity& operator*=(const Complexity& other);
        Complexity& operator/=(const Complexity& other);

        Complexity operator+(const Complexity& rhs) const;
        Complexity operator-(const Complexity& rhs) const;
        Complexity operator*(const Complexity& rhs) const;
        Complexity operator/(const Complexity& rhs) const;

        bool operator==(const Complexity& rhs) const;
        bool operator!=(const Complexity& rhs) const;
        bool operator<(const Complexity& rhs) const;
        bool operator<=(const Complexity& rhs) const;
        bool operator>(const Complexity& rhs) const;
        bool operator>=(const Complexity& rhs) const;

        /**
         Increases the comparison measure by the given amount
         @param comparison is the amount to increase the comparison measure by
         @return the reference to the Complexity object with the updated comparison
         measure
         */
        virtual Complexity& comparison(scalar comparison);
        virtual void setComparison(scalar comparison);
        virtual scalar getComparison() const;

        /**
         Increases the arithmetic measure by the given amount
         @param arithmetic is the amount to increase the comparison measure by
         @return the reference to the Complexity object with the updated arithmetic
         measure
         */
        virtual Complexity& arithmetic(scalar arithmetic);
        virtual void setArithmetic(scalar arithmetic);
        virtual scalar getArithmetic() const;

        /**
         Increases the function measure by the given amount
         @param function is the amount to increase the function measure by
         @return the reference to the Complexity object with the updated function
         measure
         */
        virtual Complexity& function(scalar function);
        virtual void setFunction(scalar function);
        virtual scalar getFunction() const;

        /**
         Returns a vector containing the measures of complexity
         @return a vector containing the measures of complexity
         */
        typedef std::pair<std::string, scalar> Measure;
        virtual std::vector<Measure> measures() const;

        /**
         Increases the complexity by the given parameter
         @param x is the addend
         @return the reference to the updated complexity
         */
        virtual Complexity& plus(const Complexity& x);
        /**
         Reduces the complexity by the given parameter
         @param x is the subtrahend
         @return the reference to the updated complexity object
         */
        virtual Complexity& minus(const Complexity& x);
        /**
         Multiplies the complexity by the given parameter
         @param x is the multiplicand
         @return the reference to the updated complexity object
         */
        virtual Complexity& multiply(const Complexity& x);
        /**
         Divides the complexity by the given parameter
         @param x is the divisor
         @return the reference to the updated complexity object
         */
        virtual Complexity& divide(const Complexity& x);

        /**
         Increases each measure by the given parameter
         @param x is the addend
         @return the reference to the updated complexity
         */
        virtual Complexity& plus(scalar x);
        /**
         Reduces each measure by the given parameter
         @param x is the subtrahend
         @return the reference to the updated complexity
         */
        virtual Complexity& minus(scalar x);
        /**
         Multiplies each measure by the given parameter
         @param x is the multiplicand
         @return the reference to the updated complexity
         */
        virtual Complexity& multiply(scalar x);
        /**
         Divides each measure by the given parameter
         @param x is the divisor
         @return the reference to the updated complexity
         */
        virtual Complexity& divide(scalar x);

        /**
         Compares the complexity for equality to another with the given tolerance
         @param x is the complexity to compare against
         @param macheps is the tolerance to compare floating-point values
         @return `true` if every measure in this satisfies Op::isEq(this, x, macheps),
         and `false` otherwise
         */
        virtual bool equals(const Complexity& x, scalar macheps = fuzzylite::macheps()) const;
        /**
         Compares the complexity for strict inequality (less than) to another
         with the given tolerance
         @param x is the complexity to compare against
         @param macheps is the tolerance to compare floating-point values
         @return `true` if every measure in this satisfies Op::isLt(this, x, macheps),
         and `false` otherwise
         */
        virtual bool lessThan(const Complexity& x, scalar macheps = fuzzylite::macheps()) const;
        /**
         Compares the complexity for inequality (less than or equal to) to another
         with the given tolerance
         @param x is the complexity to compare against
         @param macheps is the tolerance to compare floating-point values
         @return `true` if every measure in this satisfies Op::isLE(this, x, macheps),
         and `false` otherwise
         */
        virtual bool lessThanOrEqualsTo(const Complexity& x, scalar macheps = fuzzylite::macheps()) const;
        /**
         Compares the complexity for strict inequality (greater than) to another
         with the given tolerance
         @param x is the complexity to compare against
         @param macheps is the tolerance to compare floating-point values
         @return `true` if every measure in this satisfies Op::isGt(this, x, macheps),
         and `false` otherwise
         */
        virtual bool greaterThan(const Complexity& x, scalar macheps = fuzzylite::macheps()) const;
        /**
         Compares the complexity for inequality (greater than or equal to) to
         another with the given tolerance
         @param x is the complexity to compare against
         @param macheps is the tolerance to compare floating-point values
         @return `true` if every measure in this satisfies Op::isGE(this, x, macheps),
         and `false` otherwise
         */
        virtual bool greaterThanOrEqualsTo(const Complexity& x, scalar macheps = fuzzylite::macheps()) const;

        /**
         Computes the sum of the measures
         @return the sum of the measures
         */
        virtual scalar sum() const;

        /**
         Computes the norm of the complexity
         @return the norm of the complexity
         */
        virtual scalar norm() const;

        /**
         Returns the measures of the complexity
         @return the measures of the complexity
         */
        virtual std::string toString() const;

        /**
         Computes the complexity of the given engine as the sum of complexities
         of the rule blocks
         @param engine is the engine for which to compute the complexity
         @return the complexity of the given engine as the sum of complexities
         of the rule blocks
         */
        virtual Complexity compute(const Engine* engine) const;

        /**
         Computes the complexity of the given input variable
         @param inputVariable is the input variable for which to compute the complexity
         @return the complexity of the given input variable
         */
        virtual Complexity compute(const InputVariable* inputVariable) const;
        /**
         Computes the complexity of the given output variable
         @param outputVariable is the output variable for which to compute the complexity
         @return the complexity of the given output variable
         */
        virtual Complexity compute(const OutputVariable* outputVariable) const;

        /**
         Computes the complexity of the given input variables
         @param inputVariables is the vector of input variables for which to
         compute the complexity
         @return the complexity of the given input variables
         */
        virtual Complexity compute(const std::vector<InputVariable*>& inputVariables) const;
        /**
         Computes the complexity of the given output variables
         @param outputVariables is the vector of output variables for which to
         compute the complexity
         @param complexityOfDefuzzification indicates whether to compute the
         complexity of the variable including the defuzzification process
         @return the complexity of the given output variables
         */
        virtual Complexity compute(const std::vector<OutputVariable*>& outputVariables,
                bool complexityOfDefuzzification = false) const;
        /**
         Computes the complexity of the given variables
         @param variables is the vector of variables for which to compute the
         complexity
         @return the complexity of the given variables
         */
        virtual Complexity compute(const std::vector<Variable*>& variables) const;

        /**
         Computes the complexity of the given rule block
         @param ruleBlock is the rule block for which to compute the complexity
         @return the complexity of the given rule block
         */
        virtual Complexity compute(const RuleBlock* ruleBlock) const;

        /**
         Computes the complexity of the given rule blocks
         @param ruleBlocks is the vector of rule blocks for which to compute the
         complexity
         @return Computes the complexity of the given rule blocks
         */
        virtual Complexity compute(const std::vector<RuleBlock*>& ruleBlocks) const;

    };


}

#endif /* COMPLEXITY_H */

