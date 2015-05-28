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
#ifndef FL_LOWEST_H
#define	FL_LOWEST_H

#include "fl/fuzzylite.h"

#include "fl/activation/Activation.h"

namespace fl {

    /**
     * This class activates the $n$ loaded rules with the lowest activation 
     * degrees in any given {@link RuleBlock}.  If the number of rules to
     * activate is negative, then this class activates the $n$ loaded rules with
     * the highest activation degrees, which is equivalent to the {@link Highest}
     * activation.
     * 
     * @author Juan Rada-Vilela
     * @see Highest
     * @see Rule
     * @see RuleBlock
     * @see ActivationFactory
     * @since 6.0
     */
    class FL_API Lowest : public Activation {
    protected:
        //The number of rules to activate. 
        int _activatedRules;
    public:
        Lowest(int numberOfRules = 1);
        virtual ~Lowest();
        FL_DEFAULT_COPY_AND_MOVE(Lowest)

        virtual std::string className() const FL_IOVERRIDE;

        virtual std::string parameters() const FL_IOVERRIDE;
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        virtual void setActivatedRules(int activatedRules);
        virtual int getActivatedRules() const;

        /**
         * Activates the loaded rules with the lowest activation 
         * degrees in any given {@link ruleBlock}.
         * 
         * If the number of rules to activate is negative, then this class 
         * activates the $n$ loaded rules with the highest activation degrees, 
         * which is equivalent to the {@link Highest} activation.
         * 
         * @param ruleBlock is the rule block to activate.
         */
        virtual void activate(RuleBlock* ruleBlock) const FL_IOVERRIDE;

        virtual Lowest* clone() const FL_IOVERRIDE;

        static Activation* constructor();
    };
}

#endif	/* FL_LOWEST_H */

