/* 
 * File:   Maximum.h
 * Author: jcrada
 *
 * Created on 30 November 2013, 1:06 AM
 */

#ifndef FL_MAXIMUM_H
#define	FL_MAXIMUM_H

#include "fl/norm/SNorm.h"

namespace fl {
    
    class FL_EXPORT Maximum : public SNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
        
        static SNorm* create();
    };

    
}

#endif	/* FL_MAXIMUM_H */

