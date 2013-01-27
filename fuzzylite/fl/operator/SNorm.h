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

    class FL_EXPORT SNorm : public Operator {
        //Dummy class FL_EXPORT
    };

    class FL_EXPORT Maximum : public SNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
    };

    class FL_EXPORT AlgebraicSum : public SNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
    };

    class FL_EXPORT BoundedSum : public SNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
    };

    class FL_EXPORT NormalizedSum : public SNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
    };

    class FL_EXPORT DrasticSum : public SNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
    };

    class FL_EXPORT EinsteinSum : public SNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
    };

    class FL_EXPORT HamacherSum : public SNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
    };

}
#endif	/* FL_SNORM_H */

