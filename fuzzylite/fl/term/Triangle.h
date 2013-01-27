/*
 * Triangle.h
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#ifndef FL_TRIANGLE_H
#define FL_TRIANGLE_H

#include "fl/term/Term.h"

namespace fl {

    class FL_EXPORT Triangle : public Term {
    protected:
        scalar _a;
        scalar _b;
        scalar _c;
    public:
        Triangle(const std::string& name = "",
                scalar a = -std::numeric_limits<scalar>::infinity(),
                scalar b = std::numeric_limits<scalar>::quiet_NaN(),
                scalar c = std::numeric_limits<scalar>::infinity());
        ~Triangle();

        std::string className() const;
        Triangle* copy() const;

        scalar membership(scalar x) const;

        std::string toString() const;

        void setA(scalar a);
        scalar getA() const;

        void setB(scalar b);
        scalar getB() const;

        void setC(scalar c);
        scalar getC() const;

    };

} 
#endif /* FL_TRIANGLE_H */
