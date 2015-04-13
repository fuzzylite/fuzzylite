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

#ifndef FL_HAMACHERSUM_H
#define FL_HAMACHERSUM_H

#include "fl/norm/SNorm.h"

namespace fl {

    class FL_API HamacherSum : public SNorm {
    public:
        std::string className() const FL_IOVERRIDE;
        scalar compute(scalar a, scalar b) const FL_IOVERRIDE;
        HamacherSum* clone() const FL_IOVERRIDE;

        static SNorm* constructor();
    };

}

#endif  /* FL_HAMACHERSUM_H */

