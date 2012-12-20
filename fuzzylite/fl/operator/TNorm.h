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

    class TNorm : public Operator {
        //dummy class
    };

    class Minimum : public TNorm {
    public:
        std::string name() const;
        scalar compute(scalar a, scalar b) const;
    };

    class Product : public TNorm {
    public:
        std::string name() const;
        scalar compute(scalar a, scalar b) const;
    };

    class BoundedDifference : public TNorm {
    public:
        std::string name() const;
        scalar compute(scalar a, scalar b) const;
    };

    class DrasticProduct : public TNorm {
    public:
        std::string name() const;
        scalar compute(scalar a, scalar b) const;
    };

    class EinsteinProduct : public TNorm {
    public:
        std::string name() const;
        scalar compute(scalar a, scalar b) const;
    };

    class HamacherProduct : public TNorm {
    public:
        std::string name() const;
        scalar compute(scalar a, scalar b) const;
    };

}
#endif	/* TNORM_H */

