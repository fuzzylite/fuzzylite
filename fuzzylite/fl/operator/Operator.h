/* 
 * File:   Operator.h
 * Author: jcrada
 *
 * Created on 21 December 2012, 9:31 AM
 */

#ifndef FL_OPERATOR_H
#define	FL_OPERATOR_H

#include "fl/fuzzylite.h"

#include "fl/operator/Operation.h"

#include <string>

namespace fl {

    class FL_EXPORT Operator {
    public:

        Operator() { }

        virtual ~Operator() { }

        virtual std::string className() const = 0;
        virtual scalar compute(scalar a, scalar b) const = 0;

    };
}
#endif	/* FL_OPERATOR_H */

