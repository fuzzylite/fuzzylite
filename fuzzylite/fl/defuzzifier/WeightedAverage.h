/* 
 * File:   WeightedAverage.h
 * Author: jcrada
 *
 * Created on 8 May 2013, 4:15 PM
 */

#ifndef FL_WEIGHTEDAVERAGE_H
#define	FL_WEIGHTEDAVERAGE_H

#include "fl/defuzzifier/Defuzzifier.h"

namespace fl {

    class FL_EXPORT WeightedAverage : public Defuzzifier {
    public:

        WeightedAverage();
        virtual ~WeightedAverage();

        virtual std::string className() const;
        virtual scalar defuzzify(const Term* term,
                scalar minimum, scalar maximum) const;

    };
}

#endif	/* FL_WEIGHTEDAVERAGE_H */

