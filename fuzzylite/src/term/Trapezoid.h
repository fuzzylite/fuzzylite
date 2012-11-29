/*
 * Trapezoid.h
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#ifndef TRAPEZOID_H_
#define TRAPEZOID_H_

#include "../Term.h"

namespace fl {

    class Trapezoid : public Term{
    protected:
        scalar _b, _c;
    public:
        Trapezoid(const std::string& name,
                scalar a = -std::numeric_limits<scalar>::infinity(),
                scalar b = std::numeric_limits<scalar>::quiet_NaN(),
                scalar c = std::numeric_limits<scalar>::quiet_NaN(),
                scalar d = std::numeric_limits<scalar>::infinity());
        ~Trapezoid();

        scalar membership(scalar x);

        void setA(scalar a);
        scalar getA() const;

        void setB(scalar b);
        scalar getB() const;

        void setC(scalar c);
        scalar getC() const;

        void setD(scalar d);
        scalar getD() const;

        std::string toString() const;
    };

} /* namespace fl */
#endif /* TRAPEZOID_H_ */
