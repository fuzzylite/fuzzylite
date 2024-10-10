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

#include "fuzzylite/factory/TermFactory.h"

#include "fuzzylite/term/Bell.h"
#include "fuzzylite/term/Binary.h"
#include "fuzzylite/term/Concave.h"
#include "fuzzylite/term/Constant.h"
#include "fuzzylite/term/Cosine.h"
#include "fuzzylite/term/Discrete.h"
#include "fuzzylite/term/Function.h"
#include "fuzzylite/term/Gaussian.h"
#include "fuzzylite/term/GaussianProduct.h"
#include "fuzzylite/term/Linear.h"
#include "fuzzylite/term/PiShape.h"
#include "fuzzylite/term/Ramp.h"
#include "fuzzylite/term/Rectangle.h"
#include "fuzzylite/term/SShape.h"
#include "fuzzylite/term/Sigmoid.h"
#include "fuzzylite/term/SigmoidDifference.h"
#include "fuzzylite/term/SigmoidProduct.h"
#include "fuzzylite/term/Spike.h"
#include "fuzzylite/term/Trapezoid.h"
#include "fuzzylite/term/Triangle.h"
#include "fuzzylite/term/ZShape.h"

namespace fuzzylite {

    TermFactory::TermFactory(const std::string& name) : ConstructionFactory(name) {
        ConstructionFactory::registerConstructor("", fl::null);
        ConstructionFactory::registerConstructor(Bell().className(), &(Bell::constructor));
        ConstructionFactory::registerConstructor(Binary().className(), &(Binary::constructor));
        ConstructionFactory::registerConstructor(Concave().className(), &(Concave::constructor));
        ConstructionFactory::registerConstructor(Constant().className(), &(Constant::constructor));
        ConstructionFactory::registerConstructor(Cosine().className(), &(Cosine::constructor));
        ConstructionFactory::registerConstructor(Discrete().className(), &(Discrete::constructor));
        ConstructionFactory::registerConstructor(Function().className(), &(Function::constructor));
        ConstructionFactory::registerConstructor(Gaussian().className(), &(Gaussian::constructor));
        ConstructionFactory::registerConstructor(GaussianProduct().className(), &(GaussianProduct::constructor));
        ConstructionFactory::registerConstructor(Linear().className(), &(Linear::constructor));
        ConstructionFactory::registerConstructor(PiShape().className(), &(PiShape::constructor));
        ConstructionFactory::registerConstructor(Ramp().className(), &(Ramp::constructor));
        ConstructionFactory::registerConstructor(Rectangle().className(), &(Rectangle::constructor));
        ConstructionFactory::registerConstructor(SShape().className(), &(SShape::constructor));
        ConstructionFactory::registerConstructor(Sigmoid().className(), &(Sigmoid::constructor));
        ConstructionFactory::registerConstructor(SigmoidDifference().className(), &(SigmoidDifference::constructor));
        ConstructionFactory::registerConstructor(SigmoidProduct().className(), &(SigmoidProduct::constructor));
        ConstructionFactory::registerConstructor(Spike().className(), &(Spike::constructor));
        ConstructionFactory::registerConstructor(Trapezoid().className(), &(Trapezoid::constructor));
        ConstructionFactory::registerConstructor(Triangle().className(), &(Triangle::constructor));
        ConstructionFactory::registerConstructor(ZShape().className(), &(ZShape::constructor));
    }

    TermFactory::~TermFactory() {}

    TermFactory* TermFactory::clone() const {
        return new TermFactory(*this);
    }

}
