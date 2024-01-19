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

#include "fuzzylite/imex/Exporter.h"

#include <fstream>

#include "fuzzylite/Exception.h"

namespace fuzzylite {

    Exporter::Exporter() {}

    Exporter::~Exporter() {}

    void Exporter::toFile(const std::string& path, const Engine* engine) const {
        std::ofstream writer(path.c_str());
        if (not writer.is_open())
            throw Exception("[file error] file <" + path + "> could not be created", FL_AT);
        writer << toString(engine) << std::endl;
        writer.close();
    }

}
