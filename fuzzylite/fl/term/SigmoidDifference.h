/* 
 * File:   SigmoidDifference.h
 * Author: jcrada
 *
 * Created on 30 December 2012, 5:58 PM
 */

#ifndef FL_SIGMOIDDIFFERENCE_H
#define	FL_SIGMOIDDIFFERENCE_H

#include "fl/term/Term.h"

namespace fl {

    class SigmoidDifference : public Term {
    protected:
        scalar _leftInflection;
        scalar _risingSlope;
        scalar _rightInflection;
        scalar _fallingSlope;
    public:
        SigmoidDifference(const std::string& name = "",
                scalar leftInflection = std::numeric_limits<scalar>::quiet_NaN(),
                scalar risingSlope = std::numeric_limits<scalar>::quiet_NaN(),
                scalar rightInflection = std::numeric_limits<scalar>::quiet_NaN(),
                scalar fallingSlope = std::numeric_limits<scalar>::quiet_NaN());

        std::string className() const;
        SigmoidDifference* copy() const;

        scalar membership(scalar x) const;

        std::string toString() const;


        void setLeftInflection(scalar leftInflection);
        scalar getLeftInflection() const;

        void setRisingSlope(scalar risingSlope);
        scalar getRisingSlope() const;

        void setRightInflection(scalar rightInflection);
        scalar getRightInflection() const;

        void setFallingSlope(scalar fallingSlope);
        scalar getFallingSlope() const;


    };
}
#endif	/* FL_SIGMOIDDIFFERENCE_H */

