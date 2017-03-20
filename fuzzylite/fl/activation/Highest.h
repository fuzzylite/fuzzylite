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

#ifndef FL_HIGHEST_H
#define FL_HIGHEST_H

#include "fl/fuzzylite.h"

#include "fl/activation/Activation.h"

namespace fl {

    /**
      The Highest class is a RuleBlock Activation method that activates a given
      number of rules with highest activation degrees in descending order.

      @author Juan Rada-Vilela, Ph.D.
      @see Lowest
      @see Rule
      @see RuleBlock
      @see ActivationFactory
      @since 6.0
     */
    class FL_API Highest : public Activation {
    private:
        int _numberOfRules;
    public:
        explicit Highest(int numberOfRules = 1);
        virtual ~Highest();
        FL_DEFAULT_COPY_AND_MOVE(Highest)

        virtual std::string className() const FL_IOVERRIDE;

        /**
          Returns the number of rules to activate.
          @return number of rules to activate
         */
        virtual std::string parameters() const FL_IOVERRIDE;

        /**
          Configures the activation method with the number of rules to activate.
          @param parameters contains the number of rules to activate
         */
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        /**
          Sets the number of rules to activate
          @param numberOfRules is the number of rules to activate
         */
        virtual void setNumberOfRules(int numberOfRules);

        /**
          Returns the number of rules to activate
          @return the number of rules to activate
         */
        virtual int getNumberOfRules() const;

        virtual Complexity complexity(const RuleBlock* ruleBlock) const FL_IOVERRIDE;

        /**
          Activates the given number of rules with the highest activation
          degrees
          @param ruleBlock is the rule block to activate.
         */
        virtual void activate(RuleBlock* ruleBlock) FL_IOVERRIDE;

        virtual Highest* clone() const FL_IOVERRIDE;

        static Activation* constructor();
    };
}

#endif /* FL_HIGHEST_H */
