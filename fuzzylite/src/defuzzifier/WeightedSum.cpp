
#include "fl/defuzzifier/WeightedSum.h"


namespace fl {

    WeightedSum::WeightedSum() { }

    WeightedSum::~WeightedSum() { }

    std::string WeightedSum::className() const {
        return "WeightedSum";
    }

    scalar WeightedSum::defuzzify(const Term* term,
            scalar minimum, scalar maximum) const {
        (void) minimum;
        (void) maximum;
        const Accumulated* takagiSugeno = dynamic_cast<Accumulated*> (term);
        if (not takagiSugeno) {
            std::ostringstream ss;
            ss << "[defuzzification error]"
                    << "expected an Accumulated term instead of"
                    << "<" << term->toString() << ">";
            throw fl::Exception(ss.str(), FL_AT);
        }

        scalar sum = 0.0;
        for (int i = 0; i < takagiSugeno->numberOfTerms(); ++i) {
            Weighted* weightedTerm = dynamic_cast<Weighted*> (takagiSugeno->getTerm(i));
            if (not weightedTerm) {
                std::ostringstream ss;
                ss << "[defuzzification error]"
                        << "expected a Weighted term instead of"
                        << "<" << takagiSugeno->getTerm(i)->toString() << ">";
                throw fl::Exception(ss.str(), FL_AT);
            }

            sum += weightedTerm->getWeight() * weightedTerm->getTerm()->membership();
        }

        return sum;
    }

}