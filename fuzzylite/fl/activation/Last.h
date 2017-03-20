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

#ifndef FL_LAST_H
#define FL_LAST_H


#include "fl/activation/Activation.h"

namespace fl {

    /**
     The Last class is a RuleBlock Activation method that activates the last
     @f$n@f$ rules whose activation degrees are greater than or equal to the given
     threshold. The rules are iterated in the reverse order in which they were
     added to the rule block.

     @author Juan Rada-Vilela, Ph.D.
     @see First
     @see Rule
     @see RuleBlock
     @see ActivationFactory
     @since 6.0
     */

    class FL_API Last : public Activation {
    private:
        int _numberOfRules;
        scalar _threshold;
    public:
        explicit Last(int numberOfRules = 1, scalar threshold = 0.0);
        virtual ~Last();
        FL_DEFAULT_COPY_AND_MOVE(Last)

        virtual std::string className() const FL_IOVERRIDE;

        /**
         Returns the number of rules and the threshold of the activation method
         @return "numberOfRules threshold"
         */
        virtual std::string parameters() const FL_IOVERRIDE;

        /**
          Configures the activation method with the given number of rules and
          threshold
          @param parameters as "numberOfRules threshold"
         */
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        /**
        Sets the number of rules for the activation degree
        @param numberOfRules is the number of rules for the activation degree
         */
        virtual void setNumberOfRules(int numberOfRules);

        /**
          Gets the number of rules for the activation degree
          @return the number of rules for the activation degree
         */
        virtual int getNumberOfRules() const;
        /**
          Sets the threshold for the activation degree
          @param threshold is the threshold for the activation degree
         */
        virtual void setThreshold(scalar threshold);

        /**
          Gets the threshold for the activation degree
          @return the threshold for the activation degree
         */
        virtual scalar getThreshold() const;


        virtual Complexity complexity(const RuleBlock* ruleBlock) const FL_IOVERRIDE;

        /**
          Activates the last @f$n@f$ rules whose activation degrees are greater
          than the given threshold. The rules are iterated in the reverse order
          that the rules were added to the rule block.
          @param ruleBlock is the rule block to activate
         */
        virtual void activate(RuleBlock* ruleBlock) FL_IOVERRIDE;

        virtual Last* clone() const FL_IOVERRIDE;

        static Activation* constructor();
    };
}

#endif /* FL_LAST_H */
