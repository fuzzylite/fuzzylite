/* 
 * File:   DrasticProduct.h
 * Author: jcrada
 *
 * Created on 30 November 2013, 1:47 AM
 */

#ifndef FL_DRASTICPRODUCT_H
#define	FL_DRASTICPRODUCT_H

#include "fl/norm/TNorm.h"

namespace fl {

    class FL_EXPORT DrasticProduct : public TNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
        static TNorm* create();
    };


}

#endif	/* FL_DRASTICPRODUCT_H */

