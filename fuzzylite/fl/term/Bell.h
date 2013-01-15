/*
 * Bell.h
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#ifndef FL_BELL_H
#define FL_BELL_H

#include "fl/term/Term.h"

namespace fl {

    class Bell : public Term {
    protected:
        scalar _center;
        scalar _width;
        scalar _slope;
    public:
        Bell(const std::string& name = "",
                scalar center = std::numeric_limits<scalar>::quiet_NaN(),
                scalar width = std::numeric_limits<scalar>::quiet_NaN(),
                scalar slope = std::numeric_limits<scalar>::quiet_NaN());
        ~Bell();

        std::string className() const;
        Bell* copy() const;

        scalar membership(scalar x) const;

        std::string toString() const;

        void setCenter(scalar center);
        scalar getCenter() const;

        void setWidth(scalar width);
        scalar getWidth() const;

        void setSlope(scalar slope);
        scalar getSlope() const;

    };

}
#endif /* FL_BELL_H */
