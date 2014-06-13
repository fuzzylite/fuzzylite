// #BEGIN_LICENSE
// fuzzylite: a fuzzy logic control library in C++
// Copyright (C) 2014  Juan Rada-Vilela
// 
// This file is part of fuzzylite.
//
// fuzzylite is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// fuzzylite is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with fuzzylite.  If not, see <http://www.gnu.org/licenses/>.
// #END_LICENSE

#include "fl/defuzzifier/WeightedAverage.h"

#include "fl/term/Accumulated.h"
#include "fl/term/Activated.h"
#include "fl/norm/Norm.h"
#include "fl/norm/SNorm.h"
#include "fl/norm/TNorm.h"

namespace fl {

    WeightedAverage::WeightedAverage(Type type) : WeightedDefuzzifier(type) {
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
        for (int i = 0; i < fuzzyOutput->numberOfTerms(); ++i) {
            Activated* activated = fuzzyOutput->getTerm(i);
            const Term* term = activated->getTerm();
            scalar w = activated->getDegree();

            Type type = _type;
            if (type == Automatic) type = inferType(term);

            scalar z = (type == TakagiSugeno)
                    //? activated.getTerm()->membership(fl::nan) Would ensure no Tsukamoto applies, but Inverse Tsukamoto with Functions would not work.
                    ? term->membership(w) //Provides Takagi-Sugeno and Inverse Tsukamoto of Functions
                    : tsukamoto(activated, minimum, maximum);

            //Traditionally, activation is the AlgebraicProduct
            sum += activated->getActivation()->compute(w, z);
            weights += w;
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