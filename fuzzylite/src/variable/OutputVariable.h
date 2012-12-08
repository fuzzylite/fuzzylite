/*
 * OutputVariable.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_OUTPUTVARIABLE_H_
#define FL_OUTPUTVARIABLE_H_

#include "Variable.h"

#include <limits>

namespace fl {
    class Accumulated;
    class Defuzzifier;

    class OutputVariable: public Variable {
    protected:
        Defuzzifier* _defuzzifier;
        Accumulated* _output;
        scalar _defaultValue;
        scalar _defuzzifiedValue;
        bool _lockDefuzzifiedValue;

    public:
        OutputVariable(const std::string& name,
                scalar defaultValue = std::numeric_limits<scalar>::quiet_NaN(),
                bool lockDefuzzifiedValue = true);
        virtual ~OutputVariable();

        virtual void configure(Configuration* config);

        virtual void setDefaultValue(scalar defaultValue);
        virtual scalar getDefaultValue() const;

        virtual void setDefuzzifiedValue(scalar defuzzifiedValue);
        virtual scalar getDefuzzifiedValue() const;

        virtual void setLockDefuzzifiedValue(bool lock);
        virtual bool lockDefuzzifiedValue() const;

        virtual void setDefuzzifier(Defuzzifier* defuzzifier);
        virtual Defuzzifier* getDefuzzifier() const;

        virtual Accumulated* output() const;

        virtual scalar defuzzify();

    };

} /* namespace fl */
#endif /* FL_OUTPUTVARIABLE_H_ */
