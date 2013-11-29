/* 
 * File:   HamacherSum.h
 * Author: jcrada
 *
 * Created on 30 November 2013, 1:11 AM
 */

#ifndef FL_HAMACHERSUM_H
#define	FL_HAMACHERSUM_H

#include "fl/norm/SNorm.h"

namespace fl {

    class FL_EXPORT HamacherSum : public SNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
        
        static SNorm* create();
    };

}

#endif	/* FL_HAMACHERSUM_H */

