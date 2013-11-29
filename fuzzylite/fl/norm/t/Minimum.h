/* 
 * File:   Minimum.h
 * Author: jcrada
 *
 * Created on 30 November 2013, 1:46 AM
 */

#ifndef FL_MINIMUM_H
#define	FL_MINIMUM_H

#include "fl/norm/TNorm.h"

namespace fl {

    class FL_EXPORT Minimum : public TNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
        static TNorm* create();
    };

}

#endif	/* FL_MINIMUM_H */

