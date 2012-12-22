/* 
 * File:   PiShape.h
 * Author: jcrada
 *
 * Created on 22 December 2012, 5:15 PM
 */

#ifndef FL_PISHAPE_H
#define	FL_PISHAPE_H
#include "fl/term/Term.h"

namespace fl {

    class PiShape : public Term {
    protected:
        scalar _a;
        scalar _b;
        scalar _c;
        scalar _d;

    public:
        PiShape(const std::string& name = "",
                scalar _a = std::numeric_limits<scalar>::quiet_NaN(),
                scalar _b = std::numeric_limits<scalar>::quiet_NaN(),
                scalar _c = std::numeric_limits<scalar>::quiet_NaN(),
                scalar _d = std::numeric_limits<scalar>::quiet_NaN());


        std::string className() const;
        PiShape* copy() const;

        scalar membership(scalar x) const;
        std::string toString() const;


         void setA(scalar a);
         scalar getA() const;

         void setB(scalar b);
         scalar getB() const;

         void setC(scalar c);
         scalar getC() const;

         void setD(scalar d);
         scalar getD() const;


    };
}

#endif	/* FL_PISHAPE_H */

