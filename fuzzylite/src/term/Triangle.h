/*
 * Triangle.h
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "../Term.h"

namespace fl {

    class Triangle: public Term {
    protected:
        scalar _b;

    public:
        Triangle(const std::string& name,
                scalar a = -std::numeric_limits<scalar>::infinity(),
                scalar b = std::numeric_limits<scalar>::quiet_NaN(),
                scalar c = std::numeric_limits<scalar>::infinity());
        ~Triangle();

        scalar membership(scalar x);

        void setA(scalar a);
        scalar getA() const;

        void setB(scalar b);
        scalar getB() const;

        void setC(scalar c);
        scalar getC() const;

        std::string toString() const;

    };

} /* namespace fl */
#endif /* TRIANGLE_H_ */
