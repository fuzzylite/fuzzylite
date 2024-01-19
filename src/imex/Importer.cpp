/*
fuzzylite (R), a fuzzy logic control library in C++.

Copyright (C) 2010-2024 FuzzyLite Limited. All rights reserved.
Author: Juan Rada-Vilela, PhD <jcrada@fuzzylite.com>.

This file is part of fuzzylite.

fuzzylite is free software: you can redistribute it and/or modify it under
the terms of the FuzzyLite License included with the software.

You should have received a copy of the FuzzyLite License along with
fuzzylite. If not, see <https://github.com/fuzzylite/fuzzylite/>.

fuzzylite is a registered trademark of FuzzyLite Limited.
*/

#include "fuzzylite/imex/Importer.h"

#include <fstream>

#include "fuzzylite/Exception.h"

namespace fuzzylite {

    Importer::Importer() {}

    Importer::~Importer() {}

    Engine* Importer::fromFile(const std::string& path) const {
        std::ifstream reader(path.c_str());
        if (not reader.is_open())
            throw Exception("[file error] file <" + path + "> could not be opened", FL_AT);
        std::ostringstream textEngine;
        std::string line;
        while (std::getline(reader, line))
            textEngine << line << std::endl;
        reader.close();
        return fromString(textEngine.str());
    }

}
