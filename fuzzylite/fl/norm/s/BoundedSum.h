/* 
 * File:   BoundedSum.h
 * Author: jcrada
 *
 * Created on 30 November 2013, 1:09 AM
 */

#ifndef FL_BOUNDEDSUM_H
#define	FL_BOUNDEDSUM_H

#include "fl/norm/SNorm.h"

namespace fl {

    class FL_EXPORT BoundedSum : public SNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
        
        static SNorm* create();
    };
}

#endif	/* FL_BOUNDEDSUM_H */

