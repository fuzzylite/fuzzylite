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
 * File:   TermFactory.cpp
 * Author: jcrada
 * 
 * Created on 8 January 2013, 11:18 PM
 */

#include "fl/factory/TermFactory.h"

#include "fl/term/Term.h"
#include "fl/Exception.h"

#include "fl/term/Bell.h"
#include "fl/term/Constant.h"
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
#include "fl/term/Trapezoid.h"
#include "fl/term/Triangle.h"
#include "fl/term/ZShape.h"

namespace fl {

    TermFactory::TermFactory() {
        registerClass(Bell().className(), &(Bell::constructor));
        registerClass(Constant().className(), &(Constant::constructor));
        registerClass(Discrete().className(), &(Discrete::constructor));
        registerClass(Function().className(), &(Function::constructor));
        registerClass(Gaussian().className(), &(Gaussian::constructor));
        registerClass(GaussianProduct().className(), &(GaussianProduct::constructor));
        registerClass(Linear().className(), &(Linear::constructor));
        registerClass(PiShape().className(), &(PiShape::constructor));
        registerClass(Ramp().className(), &(Ramp::constructor));
        registerClass(Rectangle().className(), &(Rectangle::constructor));
        registerClass(SShape().className(), &(SShape::constructor));
        registerClass(Sigmoid().className(), &(Sigmoid::constructor));
        registerClass(SigmoidDifference().className(), &(SigmoidDifference::constructor));
        registerClass(SigmoidProduct().className(), &(SigmoidProduct::constructor));
        registerClass(Trapezoid().className(), &(Trapezoid::constructor));
        registerClass(Triangle().className(), &(Triangle::constructor));
        registerClass(ZShape().className(), &(ZShape::constructor));
    }

    TermFactory::~TermFactory() {
    }

}
