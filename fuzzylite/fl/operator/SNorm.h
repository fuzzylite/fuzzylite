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

    class SNorm : public Operator {
        //Dummy class
    };

    class Maximum : public SNorm {
    public:
        std::string name() const;
        scalar compute(scalar a, scalar b) const;
    };

    class AlgebraicSum : public SNorm {
    public:
        std::string name() const;
        scalar compute(scalar a, scalar b) const;
    };

    class BoundedSum : public SNorm {
    public:
        std::string name() const;
        scalar compute(scalar a, scalar b) const;
    };

    class NormalizedSum : public SNorm {
    public:
        std::string name() const;
        scalar compute(scalar a, scalar b) const;
    };

    class DrasticSum : public SNorm {
    public:
        std::string name() const;
        scalar compute(scalar a, scalar b) const;
    };

    class EinsteinSum : public SNorm {
    public:
        std::string name() const;
        scalar compute(scalar a, scalar b) const;
    };

    class HamacherSum : public SNorm {
    public:
        std::string name() const;
        scalar compute(scalar a, scalar b) const;
    };

}
#endif	/* FL_SNORM_H */

