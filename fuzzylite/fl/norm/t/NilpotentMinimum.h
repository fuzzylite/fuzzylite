/* 
 * File:   NilpotentMinimum.h
 * Author: jcrada
 *
 * Created on 29 January 2014, 1:42 PM
 */

#ifndef FL_NILPOTENTMINIMUM_H
#define	FL_NILPOTENTMINIMUM_H

#include "fl/norm/TNorm.h"

namespace fl {

    class FL_EXPORT NilpotentMinimum : public TNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
        static TNorm* constructor();
    };
}

#endif	/* FL_NILPOTENTMINIMUM_H */

