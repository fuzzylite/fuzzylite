/*
 * Rectangle.h
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#ifndef FL_RECTANGLE_H_
#define FL_RECTANGLE_H_

#include "fl/term/Term.h"

namespace fl {

    class Rectangle : public Term {
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
        virtual scalar minimum() const;

        virtual void setMaximum(scalar maximum);
        virtual scalar maximum() const;

    };

} /* namespace fl */
#endif /* FL_RECTANGLE_H_ */
