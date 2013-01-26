/* 
 * File:   TNorm.h
 * Author: jcrada
 *
 * Created on 21 December 2012, 9:33 AM
 */

#ifndef FL_TNORM_H
#define	FL_TNORM_H

#include "fl/operator/Operator.h"

namespace fl {

    flclass TNorm : public Operator {
        //dummy class
    };

    flclass Minimum : public TNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
    };

    class AlgebraicProduct : public TNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
    };

    flclass BoundedDifference : public TNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
    };

    flclass DrasticProduct : public TNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
    };

    flclass EinsteinProduct : public TNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
    };

    flclass HamacherProduct : public TNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
    };

}
#endif	/* TNORM_H */

