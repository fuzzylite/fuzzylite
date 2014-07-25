/*
 Author: Juan Rada-Vilela, Ph.D.
 Copyright (C) 2010-2014 FuzzyLite Limited
 All rights reserved

 This file is part of fuzzylite.

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the GNU Lesser General Public License as published by the Free
 Software Foundation, either version 3 of the License, or (at your option)
 any later version.

 fuzzylite is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with fuzzylite.  If not, see <http://www.gnu.org/licenses/>.
 
 fuzzylite (R) is a registered trademark of FuzzyLite Limited.
 
 */

#ifndef FL_INPUTVARIABLE_H
#define FL_INPUTVARIABLE_H

#include "fl/variable/Variable.h"

namespace fl {

    class FL_EXPORT InputVariable : public Variable {
    protected:
        scalar _inputValue;
    public:
        InputVariable(const std::string& name = "",
                scalar minimum = -fl::inf,
                scalar maximum = fl::inf);
        virtual ~InputVariable() FL_OVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(InputVariable)

        virtual void setInputValue(scalar inputValue);
        virtual scalar getInputValue() const;

        virtual std::string fuzzyInputValue() const;

        virtual std::string toString() const FL_OVERRIDE;

    };

}
#endif /* FL_INPUTVARIABLE_H */
