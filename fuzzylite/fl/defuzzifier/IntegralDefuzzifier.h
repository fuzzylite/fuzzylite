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

#ifndef FL_INTEGRALDEFUZZIFIER_H
#define FL_INTEGRALDEFUZZIFIER_H

#include "fl/defuzzifier/Defuzzifier.h"

namespace fl {
    //TODO: check  http://en.wikipedia.org/wiki/Adaptive_quadrature

    class FL_API IntegralDefuzzifier : public Defuzzifier {
    protected:
        static int _defaultResolution;

        int _resolution;
    public:

        static void setDefaultResolution(int defaultResolution);
        static int defaultResolution();

        explicit IntegralDefuzzifier(int resolution = defaultResolution());
        virtual ~IntegralDefuzzifier() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(IntegralDefuzzifier)

        virtual void setResolution(int resolution);
        virtual int getResolution() const;
    };
}

#endif  /* INTEGRALDEFUZZIFIER_H */

