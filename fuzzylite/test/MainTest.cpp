/*
 fuzzylite (R), a fuzzy logic control library in C++.
 Copyright (C) 2010-2017 FuzzyLite Limited. All rights reserved.
 Author: Juan Rada-Vilela, Ph.D. <jcrada@fuzzylite.com>

 This file is part of fuzzylite.

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite is a registered trademark of FuzzyLite Limited.
 */
 
#define CATCH_CONFIG_RUNNER

#include "test/catch.hpp"

#include "fl/Headers.h"

int main(int argc, char** argv) {

    // global setup...
    fl::fuzzylite::setDebugging(false);
    fl::fuzzylite::setLogging(true);

    int result = Catch::Session().run(argc, argv);

    // global clean-up...

    return result;
}
