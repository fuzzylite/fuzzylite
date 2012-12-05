/*
 * Discrete.h
 *
 *  Created on: 5/12/2012
 *      Author: jcrada
 */

#ifndef FL_DISCRETE_H_
#define FL_DISCRETE_H_

#include "Term.h"

#include <vector>

namespace fl {

    class Discrete : public Term{
    public:
        std::vector<scalar> x;
        std::vector<scalar> y;
        Discrete(const std::string& name,
                const std::vector<scalar>& x = std::vector<scalar>(),
                const std::vector<scalar>& y = std::vector<scalar>());
        ~Discrete();

        scalar minimum() const;
        scalar maximum() const;

        scalar membership(scalar x) const;

        std::string toString() const;

    };

} /* namespace fl */
#endif /* FL_DISCRETE_H_ */
