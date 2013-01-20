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
                for (std::size_t i = 0; i < params.size() - 1; i += 2) {
                    term->x.push_back(params[i]);
                    term->y.push_back(params[i + 1]);
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
                return new Bell("", params[0], params[1], params[2]);
            }
        }

        if (className == Gaussian().className()) {
            if ((int)params.size() >= (requiredParams = 2)) {
                return new Gaussian("", params[0], params[1]);
            }
        }

        if (className == GaussianProduct().className()) {
            if ((int)params.size() >= (requiredParams = 4)) {
                return new GaussianProduct("", params[0], params[1], params[2], params[3]);
            }
        }

        if (className == PiShape().className()) {
            if ((int)params.size() >= (requiredParams = 4)) {
                return new PiShape("", params[0], params[1], params[2], params[3]);
            }
        }

        if (className == Ramp().className()) {
            if ((int)params.size() >= (requiredParams = 2)) {
                return new Ramp("", params[0], params[1]);
            }
        }


        if (className == Rectangle().className()) {
            if ((int)params.size() >= (requiredParams = 2)) {
                return new Rectangle("", params[0], params[1]);
            }
        }

        if (className == SShape().className()) {
            if ((int)params.size() >= (requiredParams = 2)) {
                return new SShape("", params[0], params[1]);
            }
        }

        if (className == Sigmoid().className()) {
            if ((int)params.size() >= (requiredParams = 2)) {
                return new Sigmoid("", params[0], params[1]);
            }
        }

        if (className == SigmoidDifference().className()) {
            if ((int)params.size() >= (requiredParams = 4)) {
                return new SigmoidDifference("", params[0], params[1], params[2], params[3]);
            }
        }

        if (className == SigmoidProduct().className()) {
            if ((int)params.size() >= (requiredParams = 4)) {
                return new SigmoidProduct("", params[0], params[1], params[2], params[3]);
            }
        }

        if (className == Trapezoid().className()) {
            if ((int)params.size() >= (requiredParams = 4))
                return new Trapezoid("", params[0], params[1], params[2], params[3]);
        }

        if (className == Triangle().className()) {
            if ((int)params.size() >= (requiredParams = 3))
                return new Triangle("", params[0], params[1], params[2]);
        }

        if (className == ZShape().className()) {
            if ((int)params.size() >= (requiredParams = 2)) {
                return new ZShape("", params[0], params[1]);
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
        result.push_back(Gaussian().className());
        result.push_back(GaussianProduct().className());

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
