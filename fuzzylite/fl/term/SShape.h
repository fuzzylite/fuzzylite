/* 
 * File:   SShape.h
 * Author: jcrada
 *
 * Created on 22 December 2012, 4:43 PM
 */

#ifndef FL_SSHAPE_H
#define	FL_SSHAPE_H

#include "fl/term/Term.h"

namespace fl {

    class SShape  : public Term{
    protected:
        scalar _start, _end;

    public:
        SShape(const std::string& name = "",
                scalar _start = -std::numeric_limits<scalar>::infinity(),
                scalar _end = std::numeric_limits<scalar>::infinity());


        std::string className() const;
        SShape* copy() const;

        scalar membership(scalar x) const;
        std::string toString() const;

        void setStart(scalar start);
        scalar getStart() const;

        void setEnd(scalar end);
        scalar getEnd() const;
    };
}

#endif	/* FL_SSHAPE_H */

