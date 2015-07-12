/*
 Author: Juan Rada-Vilela, Ph.D.
 Copyright © 2010-2015 FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#ifndef FL_SIGMOIDPRODUCT_H
#define FL_SIGMOIDPRODUCT_H

#include "fl/term/Term.h"

namespace fl {

    class FL_API SigmoidProduct : public Term {
    private:
        scalar _left;
        scalar _rising;
        scalar _falling;
        scalar _right;

    public:
        explicit SigmoidProduct(const std::string& name = "",
                scalar left = fl::nan,
                scalar rising = fl::nan,
                scalar falling = fl::nan,
                scalar right = fl::nan,
                scalar height = 1.0);
        virtual ~SigmoidProduct() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(SigmoidProduct)

        virtual std::string className() const FL_IOVERRIDE;
        virtual std::string parameters() const FL_IOVERRIDE;
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        virtual scalar membership(scalar x) const FL_IOVERRIDE;

        virtual void setLeft(scalar leftInflection);
        virtual scalar getLeft() const;

        virtual void setRising(scalar risingSlope);
        virtual scalar getRising() const;

        virtual void setFalling(scalar fallingSlope);
        virtual scalar getFalling() const;

        virtual void setRight(scalar rightInflection);
        virtual scalar getRight() const;

        virtual SigmoidProduct* clone() const FL_IOVERRIDE;

        static Term* constructor();
    };
}
#endif  /* FL_SIGMOIDPRODUCT_H */

