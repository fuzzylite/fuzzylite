/* 
 * File:   MeanOfMaximum.h
 * Author: jcrada
 *
 * Created on 8 May 2013, 6:38 PM
 */

#ifndef FL_MEANOFMAXIMUM_H
#define	FL_MEANOFMAXIMUM_H

#include "fl/defuzzifier/Defuzzifier.h"

namespace fl {

    class FL_EXPORT MeanOfMaximum : public Defuzzifier {
    public:
        MeanOfMaximum(int divisions = FL_DIVISIONS);
        virtual ~MeanOfMaximum();

        virtual std::string className() const;
        virtual scalar defuzzify(const Term* term, scalar minimum, scalar maximum) const;

    };
}

#endif	/* FL_MEANOFMAXIMUM_H */

