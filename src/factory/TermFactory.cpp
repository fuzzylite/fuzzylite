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

    TermFactory::TermFactory() : ConstructionFactory<Term*>("Term") {
        registerConstructor("", fl::null);
        registerConstructor(Bell().className(), &(Bell::constructor));
        registerConstructor(Binary().className(), &(Binary::constructor));
        registerConstructor(Concave().className(), &(Concave::constructor));
        registerConstructor(Constant().className(), &(Constant::constructor));
        registerConstructor(Cosine().className(), &(Cosine::constructor));
        registerConstructor(Discrete().className(), &(Discrete::constructor));
        registerConstructor(Function().className(), &(Function::constructor));
        registerConstructor(Gaussian().className(), &(Gaussian::constructor));
        registerConstructor(GaussianProduct().className(), &(GaussianProduct::constructor));
        registerConstructor(Linear().className(), &(Linear::constructor));
        registerConstructor(PiShape().className(), &(PiShape::constructor));
        registerConstructor(Ramp().className(), &(Ramp::constructor));
        registerConstructor(Rectangle().className(), &(Rectangle::constructor));
        registerConstructor(SShape().className(), &(SShape::constructor));
        registerConstructor(Sigmoid().className(), &(Sigmoid::constructor));
        registerConstructor(SigmoidDifference().className(), &(SigmoidDifference::constructor));
        registerConstructor(SigmoidProduct().className(), &(SigmoidProduct::constructor));
        registerConstructor(Spike().className(), &(Spike::constructor));
        registerConstructor(Trapezoid().className(), &(Trapezoid::constructor));
        registerConstructor(Triangle().className(), &(Triangle::constructor));
        registerConstructor(ZShape().className(), &(ZShape::constructor));
    }

    TermFactory::~TermFactory() { }

}
