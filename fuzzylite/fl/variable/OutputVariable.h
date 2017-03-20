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

#ifndef FL_OUTPUTVARIABLE_H
#define FL_OUTPUTVARIABLE_H

#include "fl/variable/Variable.h"

#include "fl/term/Activated.h"
#include "fl/term/Aggregated.h"

#include "fl/defuzzifier/Defuzzifier.h"

namespace fl {

    /**
      The OutputVariable class is a Variable that represents an output of the
      fuzzy logic controller. During the activation of a RuleBlock, the
      Activated terms of each Rule will be Aggregated in the
      OutputVariable::fuzzyOutput(), which represents a fuzzy set hereinafter
      referred to as @f$\tilde{y}@f$. The defuzzification of @f$\tilde{y}@f$
      translates the fuzzy output value @f$\tilde{y}@f$ into a crisp output
      value @f$y@f$, which can be retrieved using Variable::getValue(). The
      value of the OutputVariable is computed and automatically stored when
      calling OutputVariable::defuzzify(), but the value depends on the
      following properties (expressed in the FuzzyLite Language):

        - Property `default: scalar` overrides the output value @f$y@f$ with
          the given fl::scalar whenever the defuzzification process results in
          a non-finite value (i.e., fl::nan and fl::inf). For example,
          considering `default: 0.0`, if RuleBlock::activate() does not
          activate any rules whose Consequent contribute to the OutputVariable,
          then the fuzzy output value is empty, the Defuzzifier does not
          operate, and hence @f$y=0.0@f$. By default, `default: NaN`. Relevant
          methods are OutputVariable::getDefaultValue() and
          OutputVariable::setDefaultValue().

        - Property `lock-previous: boolean`, if enabled, overrides the output
          value @f$y^t@f$ at time @f$t@f$ with the previously defuzzified valid
          output value @f$y^{t-1}@f$ if defuzzification process results in a
          non-finite value (i.e., fl::nan and fl::inf). When enabled, the
          property takes precedence over `default` if @f$y^{t-1}@f$ is a finite
          value. By default, `lock-previous: false`, @f$y^{t-1}=\mbox{NaN}@f$
          for @f$t=0@f$, and @f$y^{t-1}=\mbox{NaN}@f$ when
          OutputVariable::clear(). Relevant methods are
          OutputVariable::lockPreviousValue(),
          OutputVariable::isLockPreviousValue,
          OutputVariable::getPreviousValue(), and
          OutputVariable::setPreviousValue().

        - Property `lock-range: boolean` overrides the output value @f$y@f$ to
          enforce it lies within the range of the variable determined by
          Variable::getMinimum() and Variable::getMaximum(). When enabled, this
          property takes precedence over `lock-previous` and `default`. For
          example, considering `range: -1.0 1.0` and `lock-range: true`,
          @f$y=-1.0@f$ if the result from the Defuzzifier is smaller than
          `-1.0`, and @f$y=1.0@f$ if the result from the Defuzzifier is greater
          than `1.0`. The property `lock-range` was introduced in version 5.0
          to substitute the property `lock-valid` in version 4.0. By default,
          `lock-range: false`. Relevant methods are
          Variable::lockValueInRange(), Variable::isLockValueInRange(),
          Variable::getMinimum(), and Variable::getMaximum()


      @author Juan Rada-Vilela, Ph.D.
      @see Variable
      @see InputVariable
      @see RuleBlock::activate()
      @see Term
      @since 4.0
     */
    class FL_API OutputVariable : public Variable {
    private:
        FL_unique_ptr<Aggregated> _fuzzyOutput;
        FL_unique_ptr<Defuzzifier> _defuzzifier;
        scalar _previousValue;
        scalar _defaultValue;
        bool _lockPreviousValue;

        void copyFrom(const OutputVariable& other);

    public:
        explicit OutputVariable(const std::string& name = "",
                scalar minimum = -fl::inf, scalar maximum = fl::inf);
        explicit OutputVariable(const OutputVariable& other);
        OutputVariable& operator=(const OutputVariable& other);
        virtual ~OutputVariable() FL_IOVERRIDE;
        FL_DEFAULT_MOVE(OutputVariable)

        /**
          Gets the fuzzy output value @f$\tilde{y}@f$
          @return the fuzzy output value @f$\tilde{y}@f$
          @todo rename to fuzzyValue
         */
        virtual Aggregated* fuzzyOutput() const;

        virtual void setName(const std::string& name) FL_IOVERRIDE;

        virtual void setMinimum(scalar minimum) FL_IOVERRIDE;
        virtual void setMaximum(scalar maximum) FL_IOVERRIDE;

        /**
          Sets the defuzzifier of the output variable
          @param defuzzifier is the defuzzifier of the output variable
         */
        virtual void setDefuzzifier(Defuzzifier* defuzzifier);
        /**
          Gets the defuzzifier of the output variable
          @return the defuzzifier of the output variable
         */
        virtual Defuzzifier* getDefuzzifier() const;

        /**
         Sets the aggregation operator
         @param aggregation is the aggregation
         */
        virtual void setAggregation(SNorm* aggregation);

        /**
         Gets the aggregation operator
         @return the aggregation operator
         */
        virtual SNorm* getAggregation() const;

        /**
          Sets the previous value of the output variable
          @param previousValue is the previous value of the output variable
         */
        virtual void setPreviousValue(scalar previousValue);
        /**
          Gets the previous value of the output variable
          @return the previous value of the output variable
         */
        virtual scalar getPreviousValue() const;

        /**
          Sets the default value of the output variable
          @param defaultValue is the default value of the output variable
         */
        virtual void setDefaultValue(scalar defaultValue);
        /**
          Gets the default value of the output variable
          @return the default value of the output variable
         */
        virtual scalar getDefaultValue() const;

        /**
          Sets whether to lock the previous value of the output variable
          @param lockPreviousValue indicates whether to lock the previous value
          of the output variable
         */
        virtual void setLockPreviousValue(bool lockPreviousValue);
        /**
          Gets whether to lock the previous value of the output variable
          @return whether the previous output value of the output variable is
          locked
         */
        virtual bool isLockPreviousValue() const;

        using Variable::complexity;
        /**
          Computes the estimated complexity of defuzzifying the activated term
          with the current configuration of the variable (namely aggregation and
          defuzzifier
          @param term is the activated term
          @return the estimated complexity of defuzzifying the activated term
          with the current configuration of the variable
         */
        virtual Complexity complexity(const Activated& term) const;
        /**
          Computes the estimated complexity of aggregating and defuzzifying all
          the terms in the variable.
          @return the estimated complexity of aggregating and defuzzifying all
          the terms in the variable.
         */
        virtual Complexity complexityOfDefuzzification() const;

        /**
          Computes the estimated complexity of aggregating and defuzzifying the
          terms currently found in the fuzzy output variable.
          @return the estimated complexity of aggregating and defuzzifying the
          terms currently found in the fuzzy output variable
         */
        virtual Complexity currentComplexity() const;

        /**
          Defuzzifies the output variable and stores the output value and the
          previous output value
         */
        virtual void defuzzify();

        /**
          Gets a string representation of the fuzzy output value @f$\tilde{y}@f$
          @return a string representation of the fuzzy output value
          @f$\tilde{y}@f$
         */
        virtual std::string fuzzyOutputValue() const;

        /**
          Clears the output variable by setting @f$\tilde{y}=\{\}@f$,
          @f$y^{t}=\mbox{NaN}@f$, @f$y^{t-1}=\mbox{NaN}@f$
         */
        virtual void clear();

        virtual Variable::Type type() const FL_IOVERRIDE;

        virtual std::string toString() const FL_IOVERRIDE;

        virtual OutputVariable* clone() const FL_IOVERRIDE;

    };
}
#endif /* FL_OUTPUTVARIABLE_H */
