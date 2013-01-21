/* 
 * File:   SigmoidProduct.h
 * Author: jcrada
 *
 * Created on 30 December 2012, 5:41 PM
 */

#ifndef FL_SIGMOIDPRODUCT_H
#define	FL_SIGMOIDPRODUCT_H

#include "fl/term/Term.h"

namespace fl {

    class SigmoidProduct : public Term {
    protected:
        scalar _left;
        scalar _rising;
        scalar _falling;
        scalar _right;

    public:
        SigmoidProduct(const std::string& name = "",
                scalar left = std::numeric_limits<scalar>::quiet_NaN(),
                scalar rising = std::numeric_limits<scalar>::quiet_NaN(),
                scalar falling = std::numeric_limits<scalar>::quiet_NaN(),
                scalar right = std::numeric_limits<scalar>::quiet_NaN());

        std::string className() const;
        SigmoidProduct* copy() const;

        scalar membership(scalar x) const;

        std::string toString() const;


        void setLeft(scalar leftInflection);
        scalar getLeft() const;

        void setRising(scalar risingSlope);
        scalar getRising() const;

        void setFalling(scalar fallingSlope);
        scalar getFalling() const;

        void setRight(scalar rightInflection);
        scalar getRight() const;


    };
}
#endif	/* FL_SIGMOIDPRODUCT_H */

