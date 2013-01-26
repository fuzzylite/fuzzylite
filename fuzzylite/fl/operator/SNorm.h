/* 
 * File:   SNorm.h
 * Author: jcrada
 *
 * Created on 21 December 2012, 9:34 AM
 */

#ifndef FL_SNORM_H
#define	FL_SNORM_H

#include "fl/operator/Operator.h"

namespace fl {

    flclass SNorm : public Operator {
        //Dummy flclass
    };

    flclass Maximum : public SNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
    };

    flclass AlgebraicSum : public SNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
    };

    flclass BoundedSum : public SNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
    };

    flclass NormalizedSum : public SNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
    };

    flclass DrasticSum : public SNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
    };

    flclass EinsteinSum : public SNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
    };

    flclass HamacherSum : public SNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
    };

}
#endif	/* FL_SNORM_H */

