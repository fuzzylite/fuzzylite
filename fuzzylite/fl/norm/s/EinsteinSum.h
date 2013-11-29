/* 
 * File:   EinsteinSum.h
 * Author: jcrada
 *
 * Created on 30 November 2013, 1:11 AM
 */

#ifndef FL_EINSTEINSUM_H
#define	FL_EINSTEINSUM_H

#include "fl/norm/SNorm.h"

namespace fl {

    class FL_EXPORT EinsteinSum : public SNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
        
        static SNorm* create();
    };

}

#endif	/* FL_EINSTEINSUM_H */

