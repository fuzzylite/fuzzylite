/*
 * Term.h
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#ifndef FL_TERM_H_
#define FL_TERM_H_

#include "scalar.h"

#include <string>
#include <limits>

namespace fl {

    class Term {
    protected:
        std::string _name;
        scalar _minimum;
        scalar _maximum;

    public:
        Term(const std::string& name,
                scalar minimum = -std::numeric_limits<scalar>::infinity(),
                scalar maximum = std::numeric_limits<scalar>::infinity())
                : _name(name), _minimum(minimum), _maximum(maximum) {
        }

        virtual ~Term() {
        }

        virtual scalar membership(scalar x) = 0;

        virtual std::string toString() const = 0;

    };

} /* namespace fl */
#endif /* FL_TERM_H_ */
