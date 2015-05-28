/*
 Author: Juan Rada-Vilela, Ph.D.
 Copyright © 2010-2015 FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#ifndef FL_PROPORTIONAL_H
#define	FL_PROPORTIONAL_H

#include "fl/activation/Activation.h"

namespace fl {

    /**
     * This class activates each rule proportional to the activation of the 
     * other rules in any given {@link RuleBlock}. The sum of the 
     * the activation degrees of all rules is equal to one.
     * 
     * @author Juan Rada-Vilela
     * @see Lowest
     * @see Rule
     * @see RuleBlock
     * @see ActivationFactory
     * @since 6.0
     */
    class FL_API Proportional : public Activation {
    public:
        Proportional();
        virtual ~Proportional();
        FL_DEFAULT_COPY_AND_MOVE(Proportional)

        virtual std::string className() const FL_IOVERRIDE;

        virtual std::string parameters() const FL_IOVERRIDE;
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        /**
         * Activates the loaded rules with activation degrees proportional to 
         * the other rules degrees in any given {@link ruleBlock}.
         * 
         * @param ruleBlock is the rule block to activate.
         */
        virtual void activate(RuleBlock* ruleBlock) const FL_IOVERRIDE;

        virtual Proportional* clone() const FL_IOVERRIDE;

        static Activation* constructor();
    };
}

#endif	/* FL_PROPORTIONAL_H */

