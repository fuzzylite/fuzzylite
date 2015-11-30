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

    class FL_API Lowest : public Activation {
    private:
        int _activatedRules;
    public:
        explicit Lowest(int numberOfRules = 1);
        virtual ~Lowest();
        FL_DEFAULT_COPY_AND_MOVE(Lowest)

        virtual std::string className() const FL_IOVERRIDE;

        virtual std::string parameters() const FL_IOVERRIDE;
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        virtual void setActivatedRules(int activatedRules);
        virtual int getActivatedRules() const;

        virtual void activate(RuleBlock* ruleBlock) const FL_IOVERRIDE;

        virtual Lowest* clone() const FL_IOVERRIDE;

        static Activation* constructor();
    };
}

#endif	/* FL_LOWEST_H */

