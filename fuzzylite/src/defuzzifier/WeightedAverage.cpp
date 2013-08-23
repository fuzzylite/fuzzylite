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

namespace fl {

    WeightedAverage::WeightedAverage(int divisions) : Defuzzifier(divisions) { }

    WeightedAverage::~WeightedAverage() { }

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

            /**z is tsukamoto, and when it is takagi, it will not matter as it will be a function
            previously, takagi-sugeno was:
            sum += thresholded->getThreshold() * thresholded->getTerm()->membership(0);
            so replacing 0 with threshold w will give the same for takagi-sugeno, 
            plus provide tsukamoto**/
            
            scalar z = thresholded->getTerm()->membership(thresholded->getThreshold());
            sum += thresholded->getThreshold() * z;
            weights += thresholded->getThreshold();
            FL_DBG("sum=" << sum << "\tweights=" << weights);
        }

        return sum / weights;
    }

}