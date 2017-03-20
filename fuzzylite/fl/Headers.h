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

#ifndef FL_HEADERS_H
#define FL_HEADERS_H

/**
    The Headers.h file contains the headers of all the classes in the
    `fuzzylite` library, thereby encouraging the use of the directive `#include
    "fl/Headers.h"` in projects using the library.
 */


#include "fl/fuzzylite.h"

#include "fl/Benchmark.h"
#include "fl/Complexity.h"
#include "fl/Console.h"
#include "fl/Engine.h"
#include "fl/Exception.h"

#include "fl/activation/Activation.h"
#include "fl/activation/First.h"
#include "fl/activation/General.h"
#include "fl/activation/Highest.h"
#include "fl/activation/Last.h"
#include "fl/activation/Lowest.h"
#include "fl/activation/Proportional.h"
#include "fl/activation/Threshold.h"

#include "fl/defuzzifier/Bisector.h"
#include "fl/defuzzifier/Centroid.h"
#include "fl/defuzzifier/Defuzzifier.h"
#include "fl/defuzzifier/IntegralDefuzzifier.h"
#include "fl/defuzzifier/SmallestOfMaximum.h"
#include "fl/defuzzifier/LargestOfMaximum.h"
#include "fl/defuzzifier/MeanOfMaximum.h"
#include "fl/defuzzifier/WeightedAverage.h"
#include "fl/defuzzifier/WeightedDefuzzifier.h"
#include "fl/defuzzifier/WeightedSum.h"

#include "fl/factory/ActivationFactory.h"
#include "fl/factory/CloningFactory.h"
#include "fl/factory/ConstructionFactory.h"
#include "fl/factory/FactoryManager.h"
#include "fl/factory/FunctionFactory.h"
#include "fl/factory/DefuzzifierFactory.h"
#include "fl/factory/HedgeFactory.h"
#include "fl/factory/SNormFactory.h"
#include "fl/factory/TNormFactory.h"
#include "fl/factory/TermFactory.h"

#include "fl/imex/CppExporter.h"
#include "fl/imex/FclImporter.h"
#include "fl/imex/FclExporter.h"
#include "fl/imex/FisImporter.h"
#include "fl/imex/FisExporter.h"
#include "fl/imex/FldExporter.h"
#include "fl/imex/FllImporter.h"
#include "fl/imex/FllExporter.h"
#include "fl/imex/JavaExporter.h"
#include "fl/imex/RScriptExporter.h"

#include "fl/hedge/Any.h"
#include "fl/hedge/Extremely.h"
#include "fl/hedge/Hedge.h"
#include "fl/hedge/HedgeFunction.h"
#include "fl/hedge/Not.h"
#include "fl/hedge/Seldom.h"
#include "fl/hedge/Somewhat.h"
#include "fl/hedge/Very.h"

#include "fl/Operation.h"

#include "fl/norm/Norm.h"
#include "fl/norm/SNorm.h"
#include "fl/norm/TNorm.h"

#include "fl/norm/s/AlgebraicSum.h"
#include "fl/norm/s/BoundedSum.h"
#include "fl/norm/s/DrasticSum.h"
#include "fl/norm/s/EinsteinSum.h"
#include "fl/norm/s/HamacherSum.h"
#include "fl/norm/s/Maximum.h"
#include "fl/norm/s/NilpotentMaximum.h"
#include "fl/norm/s/NormalizedSum.h"
#include "fl/norm/s/SNormFunction.h"
#include "fl/norm/s/UnboundedSum.h"

#include "fl/norm/t/AlgebraicProduct.h"
#include "fl/norm/t/BoundedDifference.h"
#include "fl/norm/t/DrasticProduct.h"
#include "fl/norm/t/EinsteinProduct.h"
#include "fl/norm/t/HamacherProduct.h"
#include "fl/norm/t/Minimum.h"
#include "fl/norm/t/NilpotentMinimum.h"
#include "fl/norm/t/TNormFunction.h"

#include "fl/rule/Antecedent.h"
#include "fl/rule/Consequent.h"
#include "fl/rule/Rule.h"
#include "fl/rule/RuleBlock.h"
#include "fl/rule/Expression.h"

#include "fl/term/Aggregated.h"
#include "fl/term/Bell.h"
#include "fl/term/Binary.h"
#include "fl/term/Concave.h"
#include "fl/term/Constant.h"
#include "fl/term/Cosine.h"
#include "fl/term/Discrete.h"
#include "fl/term/Function.h"
#include "fl/term/Gaussian.h"
#include "fl/term/GaussianProduct.h"
#include "fl/term/Linear.h"
#include "fl/term/PiShape.h"
#include "fl/term/Ramp.h"
#include "fl/term/Rectangle.h"
#include "fl/term/SShape.h"
#include "fl/term/Sigmoid.h"
#include "fl/term/SigmoidDifference.h"
#include "fl/term/SigmoidProduct.h"
#include "fl/term/Spike.h"
#include "fl/term/Term.h"
#include "fl/term/Activated.h"
#include "fl/term/Trapezoid.h"
#include "fl/term/Triangle.h"
#include "fl/term/ZShape.h"

#include "fl/variable/InputVariable.h"
#include "fl/variable/OutputVariable.h"
#include "fl/variable/Variable.h"


#endif /* FL_HEADERS_H */
