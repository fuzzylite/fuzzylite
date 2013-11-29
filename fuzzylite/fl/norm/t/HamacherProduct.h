/* 
 * File:   HamacherProduct.h
 * Author: jcrada
 *
 * Created on 30 November 2013, 1:48 AM
 */

#ifndef FL_HAMACHERPRODUCT_H
#define	FL_HAMACHERPRODUCT_H

#include "fl/norm/TNorm.h"

namespace fl {

    class FL_EXPORT HamacherProduct : public TNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
        static TNorm* create();
    };

}

#endif	/* FL_HAMACHERPRODUCT_H */

