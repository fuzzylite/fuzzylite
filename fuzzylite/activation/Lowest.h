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

#ifndef FL_LOWEST_H
#define FL_LOWEST_H

#include "fl/fuzzylite.h"

#include "fl/activation/Activation.h"

namespace fl {

    /**
      The Lowest class is a RuleBlock Activation method that activates a given
      number of rules with lowest activation degrees in ascending order

      @author Juan Rada-Vilela, Ph.D.
      @see Highest
      @see Rule
      @see RuleBlock
      @see ActivationFactory
      @since 6.0
     */
    class FL_API Lowest : public Activation {
    private:
        int _numberOfRules;
    public:
        explicit Lowest(int numberOfRules = 1);
        virtual ~Lowest();
        FL_DEFAULT_COPY_AND_MOVE(Lowest)

        virtual std::string className() const FL_IOVERRIDE;

        /**
          Returns the number of rules to activate
          @return number of rules to activate
         */
        virtual std::string parameters() const FL_IOVERRIDE;

        /**
          Configures the activation method with the number of rules to activate
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
          Activates the rules with the lowest activation degrees in the given
          rule block
          @param ruleBlock is the rule block to activate
         */
        virtual void activate(RuleBlock* ruleBlock) FL_IOVERRIDE;

        virtual Lowest* clone() const FL_IOVERRIDE;

        static Activation* constructor();
    };
}

#endif /* FL_LOWEST_H */
