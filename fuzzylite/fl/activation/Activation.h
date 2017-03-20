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

#ifndef FL_ACTIVATION_H
#define FL_ACTIVATION_H

#include "fl/fuzzylite.h"

#include "fl/Complexity.h"

namespace fl {
    class RuleBlock;

    /**
      The Activation class is the abstract class for RuleBlock activation
      methods. An activation method implements the criteria to activate the
      rules within a given rule block. An activation method needs to process
      every rule and determine whether the rule is to be activated or
      deactivated. The activation methods were first introduced in version 6.0,
      but in earlier versions the term `activation` referred to the TNorm that
      modulated the consequent of a rule, which is now referred to as the
      `implication` operator.

      @author Juan Rada-Vilela, Ph.D.
      @see Rule
      @see RuleBlock
      @see ActivationFactory
      @since 6.0
     */

    class FL_API Activation {
    public:

        Activation() { }

        virtual ~Activation() { }

        FL_DEFAULT_COPY_AND_MOVE(Activation)

        /**
          Returns the name of the activation method, which is also utilized to
          register the activation method in the ActivationFactory.
          @return the name of the activation method
          @see ActivationFactory
         */
        virtual std::string className() const = 0;

        /**
          Returns the parameters of the activation method, which can be used to
          configure other instances of the activation method.
          @return the parameters of the activation method
         */
        virtual std::string parameters() const = 0;

        /**
          Configures the activation method with the given parameters.
          @param parameters contains a list of space-separated parameter values
         */
        virtual void configure(const std::string& parameters) = 0;

        /**
          Computes the estimated complexity of activating the given rule block
          @return the estimated complexity of activating the given rule block
         */
        virtual Complexity complexity(const RuleBlock* ruleBlock) const = 0;

        /**
          Activates the rule block
          @param ruleBlock is the rule block to activate
         */
        virtual void activate(RuleBlock* ruleBlock) = 0;

        /**
          Clones the activation method.
          @return a clone of the activation method
         */
        virtual Activation* clone() const = 0;
    };

}

#endif /* FL_ACTIVATION_H */
