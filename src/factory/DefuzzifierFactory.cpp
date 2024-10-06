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
#include "fuzzylite/defuzzifier/LargestOfMaximum.h"
#include "fuzzylite/defuzzifier/MeanOfMaximum.h"
#include "fuzzylite/defuzzifier/SmallestOfMaximum.h"
#include "fuzzylite/defuzzifier/WeightedAverage.h"
#include "fuzzylite/defuzzifier/WeightedSum.h"

namespace fuzzylite {

    DefuzzifierFactory::DefuzzifierFactory(const std::string& name) : ConstructionFactory(name) {
        ConstructionFactory::registerConstructor("", fl::null);
        ConstructionFactory::registerConstructor(Bisector().className(), &(Bisector::constructor));
        ConstructionFactory::registerConstructor(Centroid().className(), &(Centroid::constructor));
        ConstructionFactory::registerConstructor(LargestOfMaximum().className(), &(LargestOfMaximum::constructor));
        ConstructionFactory::registerConstructor(MeanOfMaximum().className(), &(MeanOfMaximum::constructor));
        ConstructionFactory::registerConstructor(SmallestOfMaximum().className(), &(SmallestOfMaximum::constructor));
        ConstructionFactory::registerConstructor(WeightedAverage().className(), &(WeightedAverage::constructor));
        ConstructionFactory::registerConstructor(WeightedSum().className(), &(WeightedSum::constructor));
    }

    DefuzzifierFactory::~DefuzzifierFactory() {}

    Defuzzifier* DefuzzifierFactory::constructDefuzzifier(
        const std::string& key, int resolution, WeightedDefuzzifier::Type type
    ) const {
        Defuzzifier* result = constructObject(key);
        if (IntegralDefuzzifier* integralDefuzzifier = dynamic_cast<IntegralDefuzzifier*>(result))
            integralDefuzzifier->setResolution(resolution);
        else if (WeightedDefuzzifier* weightedDefuzzifier = dynamic_cast<WeightedDefuzzifier*>(result))
            weightedDefuzzifier->setType(type);
        return result;
    }

    Defuzzifier* DefuzzifierFactory::constructIntegral(const std::string& defuzzifier, int resolution) const {
        return constructDefuzzifier(defuzzifier, resolution, WeightedDefuzzifier::Automatic);
    }

    Defuzzifier*
    DefuzzifierFactory::constructWeighted(const std::string& defuzzifier, WeightedDefuzzifier::Type type) const {
        return constructDefuzzifier(defuzzifier, 0, type);
    }

    DefuzzifierFactory* DefuzzifierFactory::clone() const {
        return new DefuzzifierFactory(*this);
    }

}
