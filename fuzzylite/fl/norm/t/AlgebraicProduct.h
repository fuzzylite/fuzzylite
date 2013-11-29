/* 
 * File:   AlgebraicProduct.h
 * Author: jcrada
 *
 * Created on 30 November 2013, 1:47 AM
 */

#ifndef FL_ALGEBRAICPRODUCT_H
#define	FL_ALGEBRAICPRODUCT_H

#include "fl/norm/TNorm.h"

namespace fl {

    class FL_EXPORT AlgebraicProduct : public TNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
        static TNorm* create();
    };

    
}

#endif	/* FL_ALGEBRAICPRODUCT_H */

