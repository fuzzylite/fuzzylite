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

#include "fl/defuzzifier/IntegralDefuzzifier.h"

namespace fl {

    int IntegralDefuzzifier::_defaultResolution = 100;

    void IntegralDefuzzifier::setDefaultResolution(int defaultResolution) {
        _defaultResolution = defaultResolution;
    }

    int IntegralDefuzzifier::defaultResolution() {
        return _defaultResolution;
    }

    IntegralDefuzzifier::IntegralDefuzzifier(int resolution)
    : Defuzzifier(), _resolution(resolution) { }

    IntegralDefuzzifier::~IntegralDefuzzifier() { }

    void IntegralDefuzzifier::setResolution(int resolution) {
        this->_resolution = resolution;
    }

    int IntegralDefuzzifier::getResolution() const {
        return this->_resolution;
    }

}
