/*   Copyright 2013 Juan Rada-Vilela

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include "fl/defuzzifier/WeightedAverage.h"

#include "fl/term/Accumulated.h"
#include "fl/term/Thresholded.h"

#include "fl/defuzzifier/Tsukamoto.h"
#include "fl/norm/Norm.h"
#include "fl/norm/TNorm.h"

namespace fl {

    WeightedAverage::WeightedAverage() : Defuzzifier() {
    }

    WeightedAverage::~WeightedAverage() {
    }

    std::string WeightedAverage::className() const {
        return "WeightedAverage";
    }

    scalar WeightedAverage::defuzzify(const Term* term,
            scalar minimum, scalar maximum) const {
        (void) minimum;
        (void) maximum;
        const Accumulated* takagiSugeno = dynamic_cast<const Accumulated*> (term);
        if (not takagiSugeno) {
            std::ostringstream ss;
            ss << "[defuzzification error]"
                    << "expected an Accumulated term instead of"
                    << "<" << term->toString() << ">";
            throw fl::Exception(ss.str(), FL_AT);
        }

        scalar sum = 0.0;
        scalar weights = 0.0;
        FL_DBG("Defuzzifying " << takagiSugeno->numberOfTerms() << " output terms");
        for (int i = 0; i < takagiSugeno->numberOfTerms(); ++i) {
            const Thresholded* thresholded = dynamic_cast<const Thresholded*> (takagiSugeno->getTerm(i));
            if (not thresholded) {
                std::ostringstream ss;
                ss << "[defuzzification error]"
                        << "expected a Thresholded term instead of"
                        << "<" << takagiSugeno->getTerm(i)->toString() << ">";
                throw fl::Exception(ss.str(), FL_AT);
            }

            scalar w = thresholded->getThreshold();
            scalar z = Tsukamoto::tsukamoto(thresholded,
                    takagiSugeno->getMinimum(), takagiSugeno->getMaximum());
            //Traditionally, activation is the AlgebraicProduct
            sum += thresholded->getActivation()->compute(w, z);
            weights += w;
        }

        return sum / weights;
    }

    Defuzzifier* WeightedAverage::constructor() {
        return new WeightedAverage;
    }

}