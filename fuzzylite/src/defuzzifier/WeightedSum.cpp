/*
 Copyright © 2010-2016 by FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#include "fl/defuzzifier/WeightedSum.h"

#include "fl/term/Aggregated.h"
#include "fl/term/Activated.h"
#include "fl/norm/SNorm.h"
#include "fl/norm/TNorm.h"

#include <map>

namespace fl {

    WeightedSum::WeightedSum(Type type) : WeightedDefuzzifier(type) {
    }

    WeightedSum::WeightedSum(const std::string& type) : WeightedDefuzzifier(type) {

    }

    WeightedSum::~WeightedSum() {
    }

    std::string WeightedSum::className() const {
        return "WeightedSum";
    }

    scalar WeightedSum::defuzzify(const Term* term,
            scalar minimum, scalar maximum) const {
        const Aggregated* fuzzyOutput = dynamic_cast<const Aggregated*> (term);
        if (not fuzzyOutput) {
            std::ostringstream ss;
            ss << "[defuzzification error]"
                    << "expected an Aggregated term instead of"
                    << "<" << (term ? term->toString() : "null") << ">";
            throw Exception(ss.str(), FL_AT);
        }

        minimum = fuzzyOutput->getMinimum();
        maximum = fuzzyOutput->getMaximum();

        scalar sum = 0.0;
        Type type = getType();
        for (std::size_t i = 0; i < fuzzyOutput->numberOfTerms(); ++i) {
            const Activated& activated = fuzzyOutput->getTerm(i);
            scalar w = activated.getDegree();

            if (type == Automatic) type = inferType(activated.getTerm());

            scalar z = (type == TakagiSugeno)
                    //? activated.getTerm()->membership(fl::nan) Would ensure no Tsukamoto applies, but Inverse Tsukamoto with Functions would not work.
                    ? activated.getTerm()->membership(w) //Provides Takagi-Sugeno and Inverse Tsukamoto of Functions
                    : tsukamoto(activated.getTerm(), w, minimum, maximum);

            sum += w * z;
        }

        return sum;
    }

    WeightedSum* WeightedSum::clone() const {
        return new WeightedSum(*this);
    }

    Defuzzifier* WeightedSum::constructor() {
        return new WeightedSum;
    }

}
