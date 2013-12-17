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
 * InputVariable.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_INPUTVARIABLE_H
#define FL_INPUTVARIABLE_H

#include "fl/variable/Variable.h"

namespace fl {

    class FL_EXPORT InputVariable : public Variable {
    protected:
        scalar _inputValue;
    public:
        InputVariable(const std::string& name = "",
                scalar minimum = -fl::inf,
                scalar maximum = fl::inf);
        virtual ~InputVariable();

        virtual void setInputValue(scalar inputValue);
        virtual scalar getInputValue() const;

        virtual std::string toString() const;
    };

}
#endif /* FL_INPUTVARIABLE_H */
