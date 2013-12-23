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
 * OutputVariable.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_OUTPUTVARIABLE_H
#define FL_OUTPUTVARIABLE_H

#include "fl/variable/Variable.h"

#include <limits>

namespace fl {
    class Accumulated;
    class Defuzzifier;

    class FL_EXPORT OutputVariable : public Variable {
    protected:
        Accumulated* _fuzzyOutput;
        Defuzzifier* _defuzzifier;
        scalar _defaultValue;
        scalar _lastValidOutput;
        bool _lockOutputRange;
        bool _lockValidOutput;

    public:
        OutputVariable(const std::string& name = "",
                scalar minimum = -fl::inf, scalar maximum = fl::inf);
        virtual ~OutputVariable();

        virtual Accumulated* fuzzyOutput() const;

        virtual void setMinimum(scalar minimum);
        virtual void setMaximum(scalar maximum);

        virtual void setDefuzzifier(Defuzzifier* defuzzifier);
        virtual Defuzzifier* getDefuzzifier() const;

        virtual void setDefaultValue(scalar defaultValue);
        virtual scalar getDefaultValue() const;

        virtual void setLastValidOutput(scalar defuzzifiedValue);
        virtual scalar getLastValidOutput() const;

        virtual void setLockOutputRange(bool lockOutputRange);
        virtual bool isLockingOutputRange() const;

        virtual void setLockValidOutput(bool lockValidOutput);
        virtual bool isLockingValidOutput() const;

        virtual scalar defuzzify();
        virtual scalar defuzzifyNoLocks() const;

        virtual std::string toString() const;

    };

}
#endif /* FL_OUTPUTVARIABLE_H */
