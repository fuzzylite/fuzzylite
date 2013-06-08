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

    TermFactory::TermFactory() { }

    TermFactory::~TermFactory() { }

    Term* TermFactory::create(const std::string& className,
            const std::vector<scalar>& params) const {
        int requiredParams = -1;
        if (className == Discrete().className()) {
            if ((int)params.size() % 2 == 0) {
                Discrete* term = new Discrete();
                for (int i = 0; i < (int)params.size() - 1; i += 2) {
                    term->x.push_back(params.at(i));
                    term->y.push_back(params.at(i+1));
                }
                return term;
            } else {
                std::ostringstream ex;
                ex << "[syntax error] a discrete term requires an even list of values, "
                        "but found <" << params.size() << "> values";
                throw fl::Exception(ex.str(), FL_AT);
            }
        }

        if (className == Bell().className()) {
            if ((int)params.size() >= (requiredParams = 3)) {
                return new Bell("", params.at(0), params.at(1), params.at(2));
            }
        }
        
        if (className == Constant().className()){
            if ((int)params.size() == (requiredParams = 1)){
                return new Constant("", params.at(0));
            }
        }
        
        if (className == Function().className()){
            if ((int)params.size() == (requiredParams = 0)){
                return new Function;
            }
        }

        if (className == Gaussian().className()) {
            if ((int)params.size() >= (requiredParams = 2)) {
                return new Gaussian("", params.at(0), params.at(1));
            }
        }

        if (className == GaussianProduct().className()) {
            if ((int)params.size() >= (requiredParams = 4)) {
                return new GaussianProduct("", params.at(0), params.at(1), params.at(2), params.at(3));
            }
        }
        
        if (className == Linear().className()){
            if ((int)params.size() >= (requiredParams = 1)){
                return new Linear("", params);
            }
        }

        if (className == PiShape().className()) {
            if ((int)params.size() >= (requiredParams = 4)) {
                return new PiShape("", params.at(0), params.at(1), params.at(2), params.at(3));
            }
        }

        if (className == Ramp().className()) {
            if ((int)params.size() >= (requiredParams = 2)) {
                return new Ramp("", params.at(0), params.at(1));
            }
        }


        if (className == Rectangle().className()) {
            if ((int)params.size() >= (requiredParams = 2)) {
                return new Rectangle("", params.at(0), params.at(1));
            }
        }

        if (className == SShape().className()) {
            if ((int)params.size() >= (requiredParams = 2)) {
                return new SShape("", params.at(0), params.at(1));
            }
        }

        if (className == Sigmoid().className()) {
            if ((int)params.size() >= (requiredParams = 2)) {
                return new Sigmoid("", params.at(0), params.at(1));
            }
        }

        if (className == SigmoidDifference().className()) {
            if ((int)params.size() >= (requiredParams = 4)) {
                return new SigmoidDifference("", params.at(0), params.at(1), params.at(2), params.at(3));
            }
        }

        if (className == SigmoidProduct().className()) {
            if ((int)params.size() >= (requiredParams = 4)) {
                return new SigmoidProduct("", params.at(0), params.at(1), params.at(2), params.at(3));
            }
        }

        if (className == Trapezoid().className()) {
            if ((int)params.size() >= (requiredParams = 4))
                return new Trapezoid("", params.at(0), params.at(1), params.at(2), params.at(3));
        }

        if (className == Triangle().className()) {
            if ((int)params.size() >= (requiredParams = 3))
                return new Triangle("", params.at(0), params.at(1), params.at(2));
        }

        if (className == ZShape().className()) {
            if ((int)params.size() >= (requiredParams = 2)) {
                return new ZShape("", params.at(0), params.at(1));
            }
        }

        if (requiredParams >= 0) {
            std::ostringstream ex;
            ex << "[factory error] Term of class<" + className + "> "
                    "requires " << requiredParams << " parameters";
            throw fl::Exception(ex.str(), FL_AT);
        }
        throw fl::Exception("[factory error] Term of class <" + className + "> not recognized", FL_AT);
    }

    std::vector<std::string> TermFactory::available() const {
        std::vector<std::string> result;
        result.push_back(Discrete().className());
        result.push_back(Bell().className());
        result.push_back(Constant().className());
        result.push_back(Function().className());
        result.push_back(Gaussian().className());
        result.push_back(GaussianProduct().className());
        result.push_back(Linear().className());

        result.push_back(PiShape().className());
        result.push_back(Ramp().className());
        result.push_back(Rectangle().className());
        result.push_back(SShape().className());
        result.push_back(Sigmoid().className());

        result.push_back(SigmoidDifference().className());
        result.push_back(SigmoidProduct().className());
        result.push_back(Trapezoid().className());
        result.push_back(Triangle().className());
        result.push_back(ZShape().className());
        return result;
    }

}
