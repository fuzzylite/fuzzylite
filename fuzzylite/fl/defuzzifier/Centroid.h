/*
 * Centroid.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_CENTROID_H
#define FL_CENTROID_H

#include "fl/defuzzifier/Defuzzifier.h"

namespace fl {

    flclass Centroid : public Defuzzifier {
    public:
        Centroid(int divisions = FL_DIVISIONS);

        std::string className() const;
        scalar defuzzify(const Term* term, scalar minimum, scalar maximum) const;

    };

} 
#endif /* FL_CENTROID_H */
