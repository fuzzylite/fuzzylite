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

#ifndef FL_WEIGHTEDAVERAGE_H
#define FL_WEIGHTEDAVERAGE_H

#include "fl/defuzzifier/WeightedDefuzzifier.h"

namespace fl {
    class Activated;

    class FL_API WeightedAverage : public WeightedDefuzzifier {
    public:
        explicit WeightedAverage(Type type = Automatic);
        explicit WeightedAverage(const std::string& type);
        virtual ~WeightedAverage() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(WeightedAverage)

        virtual std::string className() const FL_IOVERRIDE;
        virtual scalar defuzzify(const Term* term,
                scalar minimum, scalar maximum) const FL_IOVERRIDE;
        virtual WeightedAverage* clone() const FL_IOVERRIDE;

        static Defuzzifier* constructor();
    };
}

#endif  /* FL_WEIGHTEDAVERAGE_H */

