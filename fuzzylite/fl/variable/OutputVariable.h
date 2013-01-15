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

    class OutputVariable : public Variable {
    protected:
        Accumulated* _output;
        Defuzzifier* _defuzzifier;
        scalar _defaultValue;
        scalar _defuzzifiedValue;
        bool _lockDefuzzifiedValue;

    public:
        OutputVariable(const std::string& name = "",
                scalar minimum = -std::numeric_limits<scalar>::infinity(),
                scalar maximum = std::numeric_limits<scalar>::infinity());
        virtual ~OutputVariable();

        virtual Accumulated* output() const;
        
        virtual void setMinimum(scalar minimum);
        virtual void setMaximum(scalar maximum);

        virtual void setDefuzzifier(Defuzzifier* defuzzifier);
        virtual Defuzzifier* getDefuzzifier() const;

        virtual void setDefaultValue(scalar defaultValue);
        virtual scalar getDefaultValue() const;

        virtual void setDefuzzifiedValue(scalar defuzzifiedValue);
        virtual scalar getDefuzzifiedValue() const;

        virtual void setLockDefuzzifiedValue(bool lock);
        virtual bool lockDefuzzifiedValue() const;

        virtual scalar defuzzify();
        virtual scalar defuzzifyIgnoreLock() const;

    };

}
#endif /* FL_OUTPUTVARIABLE_H */
