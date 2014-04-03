/* 
 * File:   FunctionFactory.h
 * Author: jcrada
 *
 * Created on 26 March 2014, 10:08 PM
 */

#ifndef FL_FUNCTIONFACTORY_H
#define	FL_FUNCTIONFACTORY_H

#include "fl/factory/CloningFactory.h"
#include "fl/term/Function.h"

namespace fl {

    class FunctionFactory : public CloningFactory<Function::Element*> {
    public:
        FunctionFactory();
        virtual ~FunctionFactory();

        virtual std::vector<std::string> availableOperators() const;
        virtual std::vector<std::string> availableFunctions() const;

    };

}

#endif	/* FL_FUNCTIONFACTORY_H */

