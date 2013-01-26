/*
 * Rectangle.h
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#ifndef FL_RECTANGLE_H
#define FL_RECTANGLE_H

#include "fl/term/Term.h"

namespace fl {

    flclass Rectangle : public Term {
    protected:
        scalar _minimum, _maximum;

    public:
        Rectangle(const std::string& name = "",
                scalar minimum = -std::numeric_limits<scalar>::infinity(),
                scalar maximum = std::numeric_limits<scalar>::infinity());
        ~Rectangle();

        std::string className() const;
        Rectangle* copy() const;

        scalar membership(scalar x) const;
        std::string toString() const;

        virtual void setMinimum(scalar minimum);
        virtual scalar getMinimum() const;

        virtual void setMaximum(scalar maximum);
        virtual scalar getMaximum() const;

    };

} 
#endif /* FL_RECTANGLE_H */
