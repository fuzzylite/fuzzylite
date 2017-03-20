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

#ifndef FL_THRESHOLD_H
#define FL_THRESHOLD_H

#include "fl/fuzzylite.h"

#include "fl/activation/Activation.h"

#include <vector>

namespace fl {

    /**
      The Threshold class is a RuleBlock Activation method that activates the
      rules whose activation degrees satisfy the equation given by the
      comparison operator and the threshold, and deactivates the rules which do
      not satisfy the equation.

      @author Juan Rada-Vilela, Ph.D.
      @see Rule
      @see RuleBlock
      @see ActivationFactory
      @since 6.0
     */

    class FL_API Threshold : public Activation {
    public:

        /**
          Comparison is an enumerator that provides six comparison operators
          between the activation degree @f$a@f$ and the threshold @f$\theta@f$.
         */
        enum Comparison {
            /**@f$a < \theta@f$*/
            LessThan,
            /**@f$a \leq \theta@f$*/
            LessThanOrEqualTo,
            /**@f$a = \theta@f$*/
            EqualTo,
            /**@f$a \neq \theta@f$*/
            NotEqualTo,
            /**@f$a \geq \theta@f$*/
            GreaterThanOrEqualTo,
            /**@f$a > \theta@f$*/
            GreaterThan
        };
    private:
        Comparison _comparison;
        scalar _value;
    public:
        explicit Threshold(Comparison comparison = GreaterThanOrEqualTo, scalar threshold = 0.0);
        explicit Threshold(const std::string& comparison, scalar threshold);
        virtual ~Threshold();
        FL_DEFAULT_COPY_AND_MOVE(Threshold)

        virtual std::string className() const FL_IOVERRIDE;

        /**
          Returns the comparison operator followed by the threshold.
          @return comparison operator and threshold
         */
        virtual std::string parameters() const FL_IOVERRIDE;

        /**
          Configures the activation method with the comparison operator and the
          threshold.
          @param parameters is the comparison operator and threshold
         */
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        /**
          Sets the comparison operator for the activation method
          @param comparison is the operator for the activation method
         */
        virtual void setComparison(Comparison comparison);

        /**
          Gets the comparison operator for the activation method
          @return comparison operator for the activation method
         */
        virtual Comparison getComparison() const;

        /**
          Returns the comparison operator of the activation method
          @return the comparison operator in (`==`, `!=`, `<`, `>`, `<=`, `>=`)
         */
        virtual std::string comparisonOperator() const;

        /**
          Returns the given comparison operator of the activation method
          @param comparison is a valid enum value
          @return the comparison operator for the given enum value
          @throws fl::Exception if the given comparison operator is not valid
         */
        virtual std::string comparisonOperator(Comparison comparison) const;

        /**
          Returns the list of available comparison operators of the activation
          method
          @return (`==`, `!=`, `<`, `>`, `<=`, `>=`)
         */
        virtual std::vector<std::string> availableComparisonOperators() const;


        /**
          Parses the comparison operator, or throws an
          exception if the parameter does not correspond to a valid operator
          @param comparisonOperator is an operator in (`==`, `!=`, `<`, `>`,
          `<=`, `>=`)
         */
        virtual Comparison parseComparison(const std::string& comparisonOperator) const;

        /**
          Sets the threshold value of the activation method
          @param value is the threshold value for activation degrees
         */
        virtual void setValue(scalar value);

        /**
          Gets the threshold value of the activation method
          @return the threshold value of the activation method
         */
        virtual scalar getValue() const;

        /**
          Sets the comparison operator and the threshold for the activation
          method
          @param comparison is the comparison enumerator
          @param value is the threshold of the activation method
         */
        virtual void setThreshold(Comparison comparison, scalar value);

        /**
          Sets the comparison operator and the threshold for the activation method
          @param comparison is a valid comparison operator
          @param value is the threshold for activation degrees
          @throws fl::Exception if the comparison operator is not valid
         */
        virtual void setThreshold(const std::string& comparison, scalar value);

        /**
          Returns whether the activation method will activate a rule with
          the given activation degree
          @param activationDegree an activation degree
          @return whether the comparison equation is satisfied with the
          activation degree and the threshold
         */
        virtual bool activatesWith(scalar activationDegree) const;


        virtual Complexity complexity(const RuleBlock* ruleBlock) const FL_IOVERRIDE;

        /**
          Activates the rules whose activation degrees satisfy the comparison
          equation with the given threshold, and deactivate the rules which do
          not.
          @param ruleBlock is the rule block to activate
         */
        virtual void activate(RuleBlock* ruleBlock) FL_IOVERRIDE;

        virtual Threshold* clone() const FL_IOVERRIDE;

        static Activation* constructor();
    };

}

#endif /* FL_THRESHOLD_H */
