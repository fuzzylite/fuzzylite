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

#ifndef FL_THRESHOLD_H
#define	FL_THRESHOLD_H

#include "fl/fuzzylite.h"

#include "fl/activation/Activation.h"

namespace fl {

    class FL_API Threshold : public Activation {
    public:

        enum Comparison {
            EqualTo, NotEqualTo,
            LessThan, LessThanOrEqualTo,
            GreaterThan, GreaterThanOrEqualTo
        };
    private:
        Comparison _comparison;
        scalar _threshold;
    public:
        Threshold(Comparison comparison = GreaterThan, scalar threshold = 0.0);
        virtual ~Threshold();
        FL_DEFAULT_COPY_AND_MOVE(Threshold)

        virtual std::string className() const FL_IOVERRIDE;

        virtual std::string parameters() const FL_IOVERRIDE;
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        virtual void setComparison(Comparison comparison);
        virtual Comparison getComparison() const;

        virtual std::string getComparisonOperator() const;
        virtual Comparison parseComparisonOperator(const std::string& comparisonOperator) const;

        virtual void setThreshold(scalar threshold);
        virtual scalar getThreshold() const;

        virtual bool activates(scalar activationDegree) const;
        virtual void activate(RuleBlock* ruleBlock) const FL_IOVERRIDE;

        virtual Threshold* clone() const FL_IOVERRIDE;

        static Activation* constructor();
    };

}

#endif	/* FL_THRESHOLD_H */

