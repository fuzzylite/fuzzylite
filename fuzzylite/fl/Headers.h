/*
 * Headers.h
 *
 *  Created on: 30/11/2012
 *      Author: jcrada
 */

#ifndef FL_HEADERS_H_
#define FL_HEADERS_H_

#include "fl/definitions.h"
#include "fl/scalar.h"
#include "fl/Exception.h"

#include "fl/defuzzifier/CenterOfGravity.h"
#include "fl/defuzzifier/Defuzzifier.h"
#include "fl/defuzzifier/MaximumDefuzzifier.h"

#include "fl/engine/Configuration.h"
#include "fl/engine/Engine.h"

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

#include "fl/example/Example.h"
#include "fl/example/Example1.h"
#include "fl/example/Example2.h"
#include "example/AllTermsExample.h"

#endif /* FL_HEADERS_H_ */
