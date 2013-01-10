/* 
 * File:   TermFactory.cpp
 * Author: jcrada
 * 
 * Created on 8 January 2013, 11:18 PM
 */

#include "fl/factory/TermFactory.h"

#include "fl/term/Term.h"
#include "fl/Exception.h"

namespace fl {

    TermFactory::TermFactory() { }

    TermFactory::~TermFactory() { }

    Term* TermFactory::createTerm(const std::string& className,
            const std::map<std::string, scalar>& params) const {
        if (className.empty() == Discrete().className()) {
            if (params.size() % 2 == 0) {
                Discrete* term = new Discrete(name);
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
            if (params.size() == (requiredParams = 3)) {
                return new Bell(name, params[0], params[1], params[2]);
            }
        }

        if (className == Gaussian().className()) {
            if (params.size() == (requiredParams = 2)) {
                return new Gaussian(name, params[0], params[1]);
            }
        }

        if (className == GaussianProduct().className()) {
            if (params.size() == (requiredParams = 4)) {
                return new GaussianProduct(name, params[0], params[1], params[2], params[3]);
            }
        }

        if (className == PiShape().className()) {
            if (params.size() == (requiredParams = 4)) {
                return new PiShape(name, params[0], params[1], params[2], params[3]);
            }
        }

        if (className == Ramp().className()) {
            if (params.size() == (requiredParams = 2)) {
                return new Ramp(name, params[0], params[1]);
            }
        }


        if (className == Rectangle().className()) {
            if (params.size() == (requiredParams = 2)) {
                return new Rectangle(name, params[0], params[1]);
            }
        }

        if (className == SShape().className()) {
            if (params.size() == (requiredParams = 2)) {
                return new SShape(name, params[0], params[1]);
            }
        }


        if (className == Sigmoid().className()) {
            if (params.size() == (requiredParams = 2)) {
                return new Sigmoid(name, params[0], params[1]);
            }
        }
        if (className == SigmoidDifference().className()) {
            if (params.size() == (requiredParams = 4)) {
                return new SigmoidDifference(name, params[0], params[1], params[2], params[3]);
            }
        }
        if (className == SigmoidProduct().className()) {
            if (params.size() == (requiredParams = 4)) {
                return new SigmoidProduct(name, params[0], params[1], params[2], params[3]);
            }
        }

        if (className == Trapezoid().className()) {
            if (params.size() == (requiredParams = 4))
                return new Trapezoid(name, params[0], params[1], params[2], params[3]);
        }

        if (className == Triangle().className()) {
            if (params.size() == (requiredParams = 3))
                return new Triangle(name, params[0], params[1], params[2]);
        }

        if (className == ZShape().className()) {
            if (params.size() == (requiredParams = 2)) {
                return new ZShape(name, params[0], params[1]);
            }
        }



        throw fl::Exception("[factory error] Term of class<" + className + "> not recognized", FL_AT);
    }


}
