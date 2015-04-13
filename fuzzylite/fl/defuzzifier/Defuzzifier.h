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

//TODO Check http://en.wikipedia.org/wiki/Defuzzification for other defuzzifiers.

#ifndef FL_DEFUZZIFIER_H
#define FL_DEFUZZIFIER_H

#include "fl/fuzzylite.h"
#include <string>

namespace fl {
    class Term;

    class FL_API Defuzzifier {
    public:

        Defuzzifier() {
        }

        virtual ~Defuzzifier() {
        }
        FL_DEFAULT_COPY_AND_MOVE(Defuzzifier)

        virtual std::string className() const = 0;
        virtual Defuzzifier* clone() const = 0;
        virtual scalar defuzzify(const Term* term, scalar minimum, scalar maximum) const = 0;

    };

}
#endif /* FL_DEFUZZIFIER_H */
