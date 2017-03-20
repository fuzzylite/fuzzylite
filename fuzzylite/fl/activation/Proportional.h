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

#ifndef FL_PROPORTIONAL_H
#define FL_PROPORTIONAL_H

#include "fl/activation/Activation.h"

namespace fl {

    /**
      The Proportional class is a RuleBlock Activation method that activates
      the rules utilizing activation degrees proportional to the activation
      degrees of the other rules, thus the sum of the activation degrees is
      equal to one.

      @author Juan Rada-Vilela, Ph.D.
      @see Rule
      @see RuleBlock
      @see ActivationFactory
      @since 6.0
     */
    class FL_API Proportional : public Activation {
    public:
        Proportional();
        virtual ~Proportional();
        FL_DEFAULT_COPY_AND_MOVE(Proportional)

        virtual std::string className() const FL_IOVERRIDE;

        /**
          No parameters are required to configure the activation method
          @return an empty string
         */
        virtual std::string parameters() const FL_IOVERRIDE;

        /**
          No parameters are required to configure the activation method
          @param parameters is an empty string
         */
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        virtual Complexity complexity(const RuleBlock* ruleBlock) const FL_IOVERRIDE;

        /**
          Activates the rules utilizing activation degrees proportional to
          the activation degrees of the other rules in the rule block.
          @param ruleBlock is the rule block to activate.
         */
        virtual void activate(RuleBlock* ruleBlock) FL_IOVERRIDE;

        virtual Proportional* clone() const FL_IOVERRIDE;

        static Activation* constructor();
    };
}

#endif /* FL_PROPORTIONAL_H */
