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

#ifndef FL_NORM_H
#define FL_NORM_H

#include "fl/fuzzylite.h"

#include <string>

namespace fl {

    class FL_API Norm {
    public:

        Norm() {
        }

        virtual ~Norm() {
        }

        FL_DEFAULT_COPY_AND_MOVE(Norm)

        virtual std::string className() const = 0;
        virtual scalar compute(scalar a, scalar b) const = 0;

        virtual Norm* clone() const = 0;

    };
}
#endif  /* FL_NORM_H */

