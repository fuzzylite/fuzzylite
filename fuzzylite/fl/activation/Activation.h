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

#ifndef FL_ACTIVATION_H
#define	FL_ACTIVATION_H

#include "fl/fuzzylite.h"

namespace fl {
    class RuleBlock;

    class FL_API Activation {
    public:

        Activation() {
        }

        virtual ~Activation() {
        }

        FL_DEFAULT_COPY_AND_MOVE(Activation)

        virtual std::string className() const = 0;

        virtual std::string parameters() const = 0;
        virtual void configure(const std::string& parameters) = 0;

        virtual void activate(RuleBlock* ruleBlock) const = 0;

        virtual Activation* clone() const = 0;
    };

}

#endif	/* FL_ACTIVATION_H */

