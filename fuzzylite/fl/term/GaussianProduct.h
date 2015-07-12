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

#ifndef FL_GAUSSIANPRODUCT_H
#define FL_GAUSSIANPRODUCT_H

#include "fl/term/Term.h"

namespace fl {

    class FL_API GaussianProduct : public Term {
    private:
        scalar _meanA;
        scalar _standardDeviationA;
        scalar _meanB;
        scalar _standardDeviationB;

    public:
        explicit GaussianProduct(const std::string& name = "",
                scalar meanA = fl::nan,
                scalar standardDeviationA = fl::nan,
                scalar meanB = fl::nan,
                scalar standardDeviationB = fl::nan,
                scalar height = 1.0);
        virtual ~GaussianProduct() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(GaussianProduct)

        virtual std::string className() const FL_IOVERRIDE;
        virtual std::string parameters() const FL_IOVERRIDE;
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        virtual scalar membership(scalar x) const FL_IOVERRIDE;

        virtual void setMeanA(scalar meanA);
        virtual scalar getMeanA() const;

        virtual void setStandardDeviationA(scalar sigmaA);
        virtual scalar getStandardDeviationA() const;

        virtual void setMeanB(scalar meanB);
        virtual scalar getMeanB() const;

        virtual void setStandardDeviationB(scalar sigmaB);
        virtual scalar getStandardDeviationB() const;

        virtual GaussianProduct* clone() const FL_IOVERRIDE;

        static Term* constructor();
    };
}
#endif  /* FL_GAUSSIANPRODUCT_H */

