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

#ifndef FL_IMPORTER_H
#define FL_IMPORTER_H

#include "fl/fuzzylite.h"

#include <string>

namespace fl {
    class Engine;

    class FL_API Importer {
    public:

        Importer();
        virtual ~Importer();
        FL_DEFAULT_COPY_AND_MOVE(Importer)

        virtual Engine* fromString(const std::string& s) const = 0;
        virtual Engine* fromFile(const std::string& path) const;

        virtual std::string name() const = 0;
        virtual Importer* clone() const = 0;
    };

}

#endif  /* IMPORTER_H */

