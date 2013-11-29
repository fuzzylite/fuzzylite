/* 
 * File:   EinsteinProduct.h
 * Author: jcrada
 *
 * Created on 30 November 2013, 1:48 AM
 */

#ifndef FL_EINSTEINPRODUCT_H
#define	FL_EINSTEINPRODUCT_H

#include "fl/norm/TNorm.h"

namespace fl {

    class FL_EXPORT EinsteinProduct : public TNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
        static TNorm* create();
    };


}

#endif	/* FL_EINSTEINPRODUCT_H */

