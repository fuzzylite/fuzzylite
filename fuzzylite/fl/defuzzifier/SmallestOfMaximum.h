/* 
 * File:   SmallestOfMaximum.h
 * Author: jcrada
 *
 * Created on 8 May 2013, 6:38 PM
 */

#ifndef FL_SMALLESTOFMAXIMUM_H
#define	FL_SMALLESTOFMAXIMUM_H

#include "fl/defuzzifier/Defuzzifier.h"

namespace fl {

    class FL_EXPORT SmallestOfMaximum : public Defuzzifier {
    public:
        SmallestOfMaximum(int divisions = FL_DIVISIONS);
        virtual ~SmallestOfMaximum();

        virtual std::string className() const;
        virtual scalar defuzzify(const Term* term, scalar minimum, scalar maximum) const;

    };
}

#endif	/* FL_SMALLESTOFMAXIMUM_H */

