/* 
 * File:   InputLVar.h
 * Author: jcrada
 *
 * Created on November 1, 2009, 2:25 PM
 */

#ifndef _INPUTLVAR_H
#define	_INPUTLVAR_H

#include "LinguisticVariable.h"

namespace fuzzy_lite {

    class InputLVar : public LinguisticVariable {
    private:
        flScalar _input;
    public:
        InputLVar();
        InputLVar(const std::string& name);
        virtual ~InputLVar();

        virtual void setInput(flScalar input);
        virtual flScalar getInput() const;

    };
}

#endif	/* _INPUTLVAR_H */

