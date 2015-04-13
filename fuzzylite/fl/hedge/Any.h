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

#ifndef FL_ANY_H
#define FL_ANY_H

#include "fl/hedge/Hedge.h"

namespace fl {
    //Only this hedge has virtual methods due to its special case use.

    class FL_API Any : public Hedge {
    public:
        Any();
        virtual ~Any() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(Any)

        virtual std::string name() const FL_IOVERRIDE;
        virtual scalar hedge(scalar x) const FL_IOVERRIDE;
        virtual Any* clone() const FL_IOVERRIDE;

        static Hedge* constructor();
    };

}
#endif /* FL_ANY_H */
