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

#ifndef FL_HIGHEST_H
#define	FL_HIGHEST_H

#include "fl/fuzzylite.h"

#include "fl/activation/Activation.h"

namespace fl {

    /**
     * This class activates the $n$ loaded rules with the highest activation 
     * degrees in any given {@link RuleBlock}.  If the number of rules to
     * activate is negative, then this class activates the $n$ loaded rules with
     * the lowest activation degrees, which is equivalent to the {@link Lowest}
     * activation.
     * 
     * @author Juan Rada-Vilela
     * @see Lowest
     * @see Rule
     * @see RuleBlock
     * @see ActivationFactory
     * @since 6.0
     */
    class FL_API Highest : public Activation {
    protected:
        //The number of rules to activate. 
        int _activatedRules;
    public:
        Highest(int numberOfRules = 1);
        virtual ~Highest();
        FL_DEFAULT_COPY_AND_MOVE(Highest)

        virtual std::string className() const FL_IOVERRIDE;

        virtual std::string parameters() const FL_IOVERRIDE;
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        virtual void setActivatedRules(int activatedRules);
        virtual int getActivatedRules() const;

        /**
         * Activates the loaded rules with the highest activation 
         * degrees in any given {@link ruleBlock}.
         * 
         * If the number of rules to activate is negative, then this class 
         * activates the $n$ loaded rules with the lowest activation degrees, 
         * which is equivalent to the {@link Lowest} activation.
         * 
         * @param ruleBlock is the rule block to activate.
         */
        virtual void activate(RuleBlock* ruleBlock) const FL_IOVERRIDE;

        virtual Highest* clone() const FL_IOVERRIDE;

        static Activation* constructor();
    };
}

#endif	/* FL_HIGHEST_H */

