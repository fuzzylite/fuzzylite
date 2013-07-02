/* 
 * File:   WeightedSum.h
 * Author: jcrada
 *
 * Created on 8 May 2013, 4:15 PM
 */

#ifndef FL_WEIGHTEDSUM_H
#define	FL_WEIGHTEDSUM_H


#include "fl/defuzzifier/Defuzzifier.h"

namespace fl {

    class FL_EXPORT WeightedSum : public Defuzzifier {
    public:

        WeightedSum(int divisions = FL_DIVISIONS);
        virtual ~WeightedSum();

        virtual std::string className() const ;
        virtual scalar defuzzify(const Term* term,
                scalar minimum, scalar maximum) const;

    };
}

#endif	/* FL_WEIGHTEDSUM_H */

