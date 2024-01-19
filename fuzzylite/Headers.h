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

#ifndef FL_HEADERS_H
#define FL_HEADERS_H

/**
    The Headers.h file contains the headers of all the classes in the
    `fuzzylite` library, thereby encouraging the use of the directive `#include
    "fl/Headers.h"` in projects using the library.
 */

#include "fuzzylite/Benchmark.h"
#include "fuzzylite/Console.h"
#include "fuzzylite/Engine.h"
#include "fuzzylite/Exception.h"
#include "fuzzylite/Operation.h"
#include "fuzzylite/activation/Activation.h"
#include "fuzzylite/activation/First.h"
#include "fuzzylite/activation/General.h"
#include "fuzzylite/activation/Highest.h"
#include "fuzzylite/activation/Last.h"
#include "fuzzylite/activation/Lowest.h"
#include "fuzzylite/activation/Proportional.h"
#include "fuzzylite/activation/Threshold.h"
#include "fuzzylite/defuzzifier/Bisector.h"
#include "fuzzylite/defuzzifier/Centroid.h"
#include "fuzzylite/defuzzifier/Defuzzifier.h"
#include "fuzzylite/defuzzifier/IntegralDefuzzifier.h"
#include "fuzzylite/defuzzifier/LargestOfMaximum.h"
#include "fuzzylite/defuzzifier/MeanOfMaximum.h"
#include "fuzzylite/defuzzifier/SmallestOfMaximum.h"
#include "fuzzylite/defuzzifier/WeightedAverage.h"
#include "fuzzylite/defuzzifier/WeightedDefuzzifier.h"
#include "fuzzylite/defuzzifier/WeightedSum.h"
#include "fuzzylite/factory/ActivationFactory.h"
#include "fuzzylite/factory/CloningFactory.h"
#include "fuzzylite/factory/ConstructionFactory.h"
#include "fuzzylite/factory/DefuzzifierFactory.h"
#include "fuzzylite/factory/FactoryManager.h"
#include "fuzzylite/factory/FunctionFactory.h"
#include "fuzzylite/factory/HedgeFactory.h"
#include "fuzzylite/factory/SNormFactory.h"
#include "fuzzylite/factory/TNormFactory.h"
#include "fuzzylite/factory/TermFactory.h"
#include "fuzzylite/fuzzylite.h"
#include "fuzzylite/hedge/Any.h"
#include "fuzzylite/hedge/Extremely.h"
#include "fuzzylite/hedge/Hedge.h"
#include "fuzzylite/hedge/HedgeFunction.h"
#include "fuzzylite/hedge/Not.h"
#include "fuzzylite/hedge/Seldom.h"
#include "fuzzylite/hedge/Somewhat.h"
#include "fuzzylite/hedge/Very.h"
#include "fuzzylite/imex/CppExporter.h"
#include "fuzzylite/imex/FclExporter.h"
#include "fuzzylite/imex/FclImporter.h"
#include "fuzzylite/imex/FisExporter.h"
#include "fuzzylite/imex/FisImporter.h"
#include "fuzzylite/imex/FldExporter.h"
#include "fuzzylite/imex/FllExporter.h"
#include "fuzzylite/imex/FllImporter.h"
#include "fuzzylite/imex/JavaExporter.h"
#include "fuzzylite/imex/RScriptExporter.h"
#include "fuzzylite/norm/Norm.h"
#include "fuzzylite/norm/SNorm.h"
#include "fuzzylite/norm/TNorm.h"
#include "fuzzylite/norm/s/AlgebraicSum.h"
#include "fuzzylite/norm/s/BoundedSum.h"
#include "fuzzylite/norm/s/DrasticSum.h"
#include "fuzzylite/norm/s/EinsteinSum.h"
#include "fuzzylite/norm/s/HamacherSum.h"
#include "fuzzylite/norm/s/Maximum.h"
#include "fuzzylite/norm/s/NilpotentMaximum.h"
#include "fuzzylite/norm/s/NormalizedSum.h"
#include "fuzzylite/norm/s/SNormFunction.h"
#include "fuzzylite/norm/s/UnboundedSum.h"
#include "fuzzylite/norm/t/AlgebraicProduct.h"
#include "fuzzylite/norm/t/BoundedDifference.h"
#include "fuzzylite/norm/t/DrasticProduct.h"
#include "fuzzylite/norm/t/EinsteinProduct.h"
#include "fuzzylite/norm/t/HamacherProduct.h"
#include "fuzzylite/norm/t/Minimum.h"
#include "fuzzylite/norm/t/NilpotentMinimum.h"
#include "fuzzylite/norm/t/TNormFunction.h"
#include "fuzzylite/rule/Antecedent.h"
#include "fuzzylite/rule/Consequent.h"
#include "fuzzylite/rule/Expression.h"
#include "fuzzylite/rule/Rule.h"
#include "fuzzylite/rule/RuleBlock.h"
#include "fuzzylite/term/Activated.h"
#include "fuzzylite/term/Aggregated.h"
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
#include "fuzzylite/term/Term.h"
#include "fuzzylite/term/Trapezoid.h"
#include "fuzzylite/term/Triangle.h"
#include "fuzzylite/term/ZShape.h"
#include "fuzzylite/variable/InputVariable.h"
#include "fuzzylite/variable/OutputVariable.h"
#include "fuzzylite/variable/Variable.h"

#endif /* FL_HEADERS_H */
