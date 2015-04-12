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

#ifndef FL_INPUTVARIABLE_H
#define FL_INPUTVARIABLE_H

#include "fl/variable/Variable.h"

namespace fl {

    class FL_API InputVariable : public Variable {
    public:
        explicit InputVariable(const std::string& name = "",
                scalar minimum = -fl::inf,
                scalar maximum = fl::inf);
        virtual ~InputVariable() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(InputVariable)

        virtual std::string fuzzyInputValue() const;

        virtual std::string toString() const FL_IOVERRIDE;

    };

}
#endif /* FL_INPUTVARIABLE_H */
