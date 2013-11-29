/* 
 * File:   DrasticSum.h
 * Author: jcrada
 *
 * Created on 30 November 2013, 1:10 AM
 */

#ifndef FL_DRASTICSUM_H
#define	FL_DRASTICSUM_H

#include "fl/norm/SNorm.h"
namespace fl {

    class FL_EXPORT DrasticSum : public SNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
        
        static SNorm* create();
    };

}

#endif	/* FL_DRASTICSUM_H */

