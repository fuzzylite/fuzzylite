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

#ifndef FL_GAUSSIAN_H
#define FL_GAUSSIAN_H

#include "fl/term/Term.h"

namespace fl {

    class FL_API Gaussian : public Term {
    private:
        scalar _mean;
        scalar _standardDeviation;

    public:
        explicit Gaussian(const std::string& name = "",
                scalar mean = fl::nan,
                scalar standardDeviation = fl::nan,
                scalar height = 1.0);
        virtual ~Gaussian() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(Gaussian)

        virtual std::string className() const FL_IOVERRIDE;
        virtual std::string parameters() const FL_IOVERRIDE;
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        virtual scalar membership(scalar x) const FL_IOVERRIDE;

        virtual void setMean(scalar c);
        virtual scalar getMean() const;

        virtual void setStandardDeviation(scalar sigma);
        virtual scalar getStandardDeviation() const;

        virtual Gaussian* clone() const FL_IOVERRIDE;

        static Term* constructor();
    };

}
#endif /* FL_GAUSSIAN_H */
