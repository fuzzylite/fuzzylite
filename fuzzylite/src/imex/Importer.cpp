/*
 Copyright (C) 2010-2016 by FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite(R).

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite(R) is a registered trademark of FuzzyLite Limited.
 */

#include "fl/imex/Importer.h"
#include "fl/Exception.h"

#include <fstream>
#include <ostream>

namespace fl {

    Importer::Importer() {
    }

    Importer::~Importer() {

    }

    Engine* Importer::fromFile(const std::string& path) const {
        std::ifstream reader(path.c_str());
        if (not reader.is_open()) {
            throw Exception("[file error] file <" + path + "> could not be opened", FL_AT);
        }
        std::ostringstream textEngine;
        std::string line;
        while (std::getline(reader, line)) {
            textEngine << line << std::endl;
        }
        reader.close();
        return fromString(textEngine.str());
    }

}
