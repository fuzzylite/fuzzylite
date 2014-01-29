/* 
 * File:   NilpotentMaximum.h
 * Author: jcrada
 *
 * Created on 29 January 2014, 1:49 PM
 */

#ifndef FL_NILPOTENTMAXIMUM_H
#define	FL_NILPOTENTMAXIMUM_H

#include "fl/norm/SNorm.h"

namespace fl {

    class FL_EXPORT NilpotentMaximum : public SNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;

        static SNorm* constructor();
    };
}

#endif	/* FL_NILPOTENTMAXIMUM_H */

