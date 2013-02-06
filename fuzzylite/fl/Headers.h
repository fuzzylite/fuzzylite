/*   Copyright 2013 Juan Rada-Vilela

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

/*
 * Headers.h
 *
 *  Created on: 30/11/2012
 *      Author: jcrada
 */

#ifndef FL_HEADERS_H
#define FL_HEADERS_H

#include "fl/fuzzylite.h"

#include "fl/Engine.h"
#include "fl/Exception.h"

#include "fl/defuzzifier/Centroid.h"
#include "fl/defuzzifier/Defuzzifier.h"
#include "fl/defuzzifier/MaximumDefuzzifier.h"

#include "fl/factory/Factory.h"
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

#include "fl/hedge/Any.h"
#include "fl/hedge/Extremely.h"
#include "fl/hedge/Hedge.h"
#include "fl/hedge/Not.h"
#include "fl/hedge/Seldom.h"
#include "fl/hedge/Somewhat.h"
#include "fl/hedge/Very.h"

#include "fl/operator/Operation.h"
#include "fl/operator/Operator.h"
#include "fl/operator/SNorm.h"
#include "fl/operator/TNorm.h"

#include "fl/rule/Antecedent.h"
#include "fl/rule/Consequent.h"
#include "fl/rule/Infix.h"
#include "fl/rule/Rule.h"
#include "fl/rule/RuleBlock.h"

#include "fl/rule/mamdani/MamdaniAntecedent.h"
#include "fl/rule/mamdani/MamdaniConsequent.h"
#include "fl/rule/mamdani/MamdaniExpression.h"
#include "fl/rule/mamdani/MamdaniRule.h"

#include "fl/term/Accumulated.h"
#include "fl/term/Bell.h"
#include "fl/term/Discrete.h"
#include "fl/term/Gaussian.h"
#include "fl/term/GaussianProduct.h"
#include "fl/term/PiShape.h"
#include "fl/term/Ramp.h"
#include "fl/term/Rectangle.h"
#include "fl/term/SShape.h"
#include "fl/term/Sigmoid.h"
#include "fl/term/SigmoidDifference.h"
#include "fl/term/SigmoidProduct.h"
#include "fl/term/Term.h"
#include "fl/term/Thresholded.h"
#include "fl/term/Trapezoid.h"
#include "fl/term/Triangle.h"
#include "fl/term/ZShape.h"

#include "fl/variable/InputVariable.h"
#include "fl/variable/OutputVariable.h"
#include "fl/variable/Variable.h"


#endif /* FL_HEADERS_H */
