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

#include "fl/defuzzifier/WeightedSumCustom.h"

#include "fl/term/Aggregated.h"
#include "fl/term/Activated.h"
#include "fl/norm/SNorm.h"
#include "fl/norm/TNorm.h"

#include <map>
namespace fl {

    WeightedSumCustom::WeightedSumCustom(Type type) : WeightedDefuzzifier(type) {
    }

    WeightedSumCustom::WeightedSumCustom(const std::string& type) : WeightedDefuzzifier(type) {

    }

    WeightedSumCustom::~WeightedSumCustom() {
    }

    std::string WeightedSumCustom::className() const {
        return "WeightedSumCustom";
    }

    scalar WeightedSumCustom::defuzzify(const Term* term,
            scalar minimum, scalar maximum) const {
          const Aggregated* fuzzyOutput = dynamic_cast<const Aggregated*> (term);
        if (not fuzzyOutput) {
            std::ostringstream ss;
            ss << "[defuzzification error]"
                    << "expected an Aggregated term instead of"
                    << "<" << term->toString() << ">";
            throw fl::Exception(ss.str(), FL_AT);
        }

        minimum = fuzzyOutput->getMinimum();
        maximum = fuzzyOutput->getMaximum();

        scalar sum = 0.0;

        SNorm* aggregation = fuzzyOutput->getAggregation();
        Type type = getType();
        for (std::size_t i = 0; i < fuzzyOutput->terms().size(); ++i) {
            const Activated& activated = fuzzyOutput->getTerm(i);
            scalar w = activated.getDegree();
            const TNorm* implication = activated.getImplication();

            if (type == Automatic) type = inferType(activated.getTerm());

            scalar z = (type == TakagiSugeno)
                    //? activated.getTerm()->membership(fl::nan) Would ensure no Tsukamoto applies, but Inverse Tsukamoto with Functions would not work.
                    ? activated.getTerm()->membership(w) //Provides Takagi-Sugeno and Inverse Tsukamoto of Functions
                    : tsukamoto(activated.getTerm(), w, minimum, maximum);

            scalar wz = implication ? implication->compute(w, z) : w * z;
            sum = aggregation ? aggregation->compute(sum, wz) : sum + wz;
        }
        return sum;
    }

    WeightedSumCustom* WeightedSumCustom::clone() const {
        return new WeightedSumCustom(*this);
    }

    Defuzzifier* WeightedSumCustom::constructor() {
        return new WeightedSumCustom;
    }

}
