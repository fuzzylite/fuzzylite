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

#include "fl/imex/Exporter.h"
#include "fl/Exception.h"

#include <fstream>

namespace fl {

    Exporter::Exporter() {

    }

    Exporter::~Exporter() {

    }

    void Exporter::toFile(const std::string& path, const Engine* engine) const {
        std::ofstream writer(path.c_str());
        if (not writer.is_open()) {
            throw fl::Exception("[file error] file <" + path + "> could not be created", FL_AT);
        }
        writer << toString(engine) << std::endl;
        writer.close();
    }

}
