/* 
 * File:   ZShape.h
 * Author: jcrada
 *
 * Created on 22 December 2012, 5:07 PM
 */

#ifndef FL_ZSHAPE_H
#define	FL_ZSHAPE_H

#include "fl/term/Term.h"

namespace fl {

    class ZShape : public Term {
    protected:
        scalar _start, _end;

    public:
        ZShape(const std::string& name = "",
                scalar _start = -std::numeric_limits<scalar>::infinity(),
                scalar _end = std::numeric_limits<scalar>::infinity());


        std::string className() const;
        ZShape* copy() const;

        scalar membership(scalar x) const;
        std::string toString() const;

        void setStart(scalar start);
        scalar getStart() const;

        void setEnd(scalar end);
        scalar getEnd() const;
    };
}
#endif	/* ZSHAPE_H */

