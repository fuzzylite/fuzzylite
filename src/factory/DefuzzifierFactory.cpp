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

#include "fuzzylite/factory/DefuzzifierFactory.h"

#include "fuzzylite/defuzzifier/Bisector.h"
#include "fuzzylite/defuzzifier/Centroid.h"
#include "fuzzylite/defuzzifier/SmallestOfMaximum.h"
#include "fuzzylite/defuzzifier/LargestOfMaximum.h"
#include "fuzzylite/defuzzifier/MeanOfMaximum.h"
#include "fuzzylite/defuzzifier/WeightedAverage.h"
#include "fuzzylite/defuzzifier/WeightedAverageCustom.h"
#include "fuzzylite/defuzzifier/WeightedSum.h"
#include "fuzzylite/defuzzifier/WeightedSumCustom.h"

namespace fl {

    DefuzzifierFactory::DefuzzifierFactory() : ConstructionFactory<Defuzzifier*>("Defuzzifier") {
        registerConstructor("", fl::null);
        registerConstructor(Bisector().className(), &(Bisector::constructor));
        registerConstructor(Centroid().className(), &(Centroid::constructor));
        registerConstructor(LargestOfMaximum().className(), &(LargestOfMaximum::constructor));
        registerConstructor(MeanOfMaximum().className(), &(MeanOfMaximum::constructor));
        registerConstructor(SmallestOfMaximum().className(), &(SmallestOfMaximum::constructor));
        registerConstructor(WeightedAverage().className(), &(WeightedAverage::constructor));
//        registerConstructor(WeightedAverageCustom().className(), &(WeightedAverageCustom::constructor));
        registerConstructor(WeightedSum().className(), &(WeightedSum::constructor));
//        registerConstructor(WeightedSumCustom().className(), &(WeightedSumCustom::constructor));
    }

    DefuzzifierFactory::~DefuzzifierFactory() { }

    Defuzzifier* DefuzzifierFactory::constructDefuzzifier(const std::string& key,
            int resolution, WeightedDefuzzifier::Type type) const {
        Defuzzifier* result = constructObject(key);
        if (IntegralDefuzzifier * integralDefuzzifier = dynamic_cast<IntegralDefuzzifier*> (result)) {
            integralDefuzzifier->setResolution(resolution);
        } else if (WeightedDefuzzifier * weightedDefuzzifier = dynamic_cast<WeightedDefuzzifier*> (result)) {
            weightedDefuzzifier->setType(type);
        }
        return result;
    }

    Defuzzifier* DefuzzifierFactory::constructDefuzzifier(const std::string& key, int resolution) const {
        Defuzzifier* result = constructObject(key);
        if (IntegralDefuzzifier * integralDefuzzifier = dynamic_cast<IntegralDefuzzifier*> (result)) {
            integralDefuzzifier->setResolution(resolution);
        }
        return result;
    }

    Defuzzifier* DefuzzifierFactory::constructDefuzzifier(const std::string& key, WeightedDefuzzifier::Type type) {
        Defuzzifier* result = constructObject(key);
        if (WeightedDefuzzifier * weightedDefuzzifier = dynamic_cast<WeightedDefuzzifier*> (result)) {
            weightedDefuzzifier->setType(type);
        }
        return result;
    }

}
