/* 
 * File:   AlgebraicSum.h
 * Author: jcrada
 *
 * Created on 30 November 2013, 1:07 AM
 */

#ifndef FL_ALGEBRAICSUM_H
#define	FL_ALGEBRAICSUM_H

#include "fl/norm/SNorm.h"


namespace fl {
    
    class FL_EXPORT AlgebraicSum : public SNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
        
        static SNorm* create();
    };


}

#endif	/* FL_ALGEBRAICSUM_H */

