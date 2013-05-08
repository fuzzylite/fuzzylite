/* 
 * File:   LargestOfMaximum.h
 * Author: jcrada
 *
 * Created on 8 May 2013, 6:38 PM
 */

#ifndef FL_LARGESTOFMAXIMUM_H
#define	FL_LARGESTOFMAXIMUM_H

#include "fl/defuzzifier/Defuzzifier.h"

namespace fl {

    class FL_EXPORT LargestOfMaximum : public Defuzzifier {
    public:
        LargestOfMaximum(int divisions = FL_DIVISIONS);
        virtual ~LargestOfMaximum();

        virtual std::string className() const;
        virtual scalar defuzzify(const Term* term, scalar minimum, scalar maximum) const;

    };
}
#endif	/* FL_LARGESTOFMAXIMUM_H */

