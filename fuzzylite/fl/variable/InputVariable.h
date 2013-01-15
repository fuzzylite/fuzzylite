/*
 * InputVariable.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_INPUTVARIABLE_H
#define FL_INPUTVARIABLE_H

#include "fl/variable/Variable.h"

namespace fl {

    class InputVariable : public Variable {
    protected:
        scalar _input;
    public:
        InputVariable(const std::string& name = "", 
                scalar minimum = -std::numeric_limits<scalar>::infinity(),
                scalar maximum = std::numeric_limits<scalar>::infinity());
        virtual ~InputVariable();

        virtual void setInput(scalar input);
        virtual scalar getInput() const;

    };

} 
#endif /* FL_INPUTVARIABLE_H */
