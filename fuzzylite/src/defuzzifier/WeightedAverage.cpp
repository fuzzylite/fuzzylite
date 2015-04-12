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

#include "fl/defuzzifier/WeightedAverage.h"

#include "fl/term/Accumulated.h"
#include "fl/term/Activated.h"
#include "fl/norm/Norm.h"
#include "fl/norm/SNorm.h"
#include "fl/norm/TNorm.h"

#include <map>

namespace fl {

    WeightedAverage::WeightedAverage(Type type) : WeightedDefuzzifier(type) {
    }

    WeightedAverage::WeightedAverage(const std::string& type) : WeightedDefuzzifier(type) {
    }

    WeightedAverage::~WeightedAverage() {
    }

    std::string WeightedAverage::className() const {
        return "WeightedAverage";
    }

    scalar WeightedAverage::defuzzify(const Term* term,
            scalar minimum, scalar maximum) const {
        const Accumulated* fuzzyOutput = dynamic_cast<const Accumulated*> (term);
        if (not fuzzyOutput) {
            std::ostringstream ss;
            ss << "[defuzzification error]"
                    << "expected an Accumulated term instead of"
                    << "<" << term->toString() << ">";
            throw fl::Exception(ss.str(), FL_AT);
        }

        minimum = fuzzyOutput->getMinimum();
        maximum = fuzzyOutput->getMaximum();

        scalar sum = 0.0;
        scalar weights = 0.0;

        if (not fuzzyOutput->getAccumulation()) {
            Type type = _type;
            for (int i = 0; i < fuzzyOutput->numberOfTerms(); ++i) {
                Activated* activated = fuzzyOutput->getTerm(i);
                scalar w = activated->getDegree();

                if (type == Automatic) type = inferType(activated->getTerm());

                scalar z = (type == TakagiSugeno)
                        //? activated.getTerm()->membership(fl::nan) Would ensure no Tsukamoto applies, but Inverse Tsukamoto with Functions would not work.
                        ? activated->getTerm()->membership(w) //Provides Takagi-Sugeno and Inverse Tsukamoto of Functions
                        : tsukamoto(activated->getTerm(), w, minimum, maximum);

                sum += w * z;
                weights += w;
            }
        } else {
            typedef std::map<const Term*, std::vector<Activated*> > TermGroup;
            TermGroup groups;
            for (int i = 0; i < fuzzyOutput->numberOfTerms(); ++i) {
                Activated* value = fuzzyOutput->getTerm(i);
                const Term* key = value->getTerm();
                groups[key].push_back(value);
            }
            TermGroup::const_iterator it = groups.begin();
            Type type = _type;
            while (it != groups.end()) {
                const Term* activatedTerm = it->first;
                scalar accumulatedDegree = 0.0;
                for (std::size_t i = 0; i < it->second.size(); ++i)
                    accumulatedDegree = fuzzyOutput->getAccumulation()->compute(
                        accumulatedDegree, it->second.at(i)->getDegree());

                if (type == Automatic) type = inferType(activatedTerm);

                scalar z = (type == TakagiSugeno)
                        //? activated.getTerm()->membership(fl::nan) Would ensure no Tsukamoto applies, but Inverse Tsukamoto with Functions would not work.
                        ? activatedTerm->membership(accumulatedDegree) //Provides Takagi-Sugeno and Inverse Tsukamoto of Functions
                        : tsukamoto(activatedTerm, accumulatedDegree, minimum, maximum);

                sum += accumulatedDegree * z;
                weights += accumulatedDegree;

                ++it;
            }
        }
        return sum / weights;
    }

    WeightedAverage* WeightedAverage::clone() const {
        return new WeightedAverage(*this);
    }

    Defuzzifier* WeightedAverage::constructor() {
        return new WeightedAverage;
    }
}
