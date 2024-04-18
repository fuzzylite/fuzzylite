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

#include "fuzzylite/defuzzifier/IntegralDefuzzifier.h"

namespace fuzzylite {

    int IntegralDefuzzifier::_defaultResolution = 1000;

    void IntegralDefuzzifier::setDefaultResolution(int defaultResolution) {
        _defaultResolution = defaultResolution;
    }

    int IntegralDefuzzifier::defaultResolution() {
        return _defaultResolution;
    }

    IntegralDefuzzifier::IntegralDefuzzifier(int resolution) : Defuzzifier(), _resolution(resolution) {}

    IntegralDefuzzifier::~IntegralDefuzzifier() {}

    void IntegralDefuzzifier::setResolution(int resolution) {
        this->_resolution = resolution;
    }

    int IntegralDefuzzifier::getResolution() const {
        return this->_resolution;
    }

}
