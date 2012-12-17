/*
 * OutputVariable.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_OUTPUTVARIABLE_H_
#define FL_OUTPUTVARIABLE_H_

#include "fl/variable/Variable.h"

#include <limits>

namespace fl {
    class Accumulated;
    class Defuzzifier;

    class OutputVariable: public Variable {
    protected:
        Accumulated* _output;
        Defuzzifier* _defuzzifier;
        scalar _defaultValue;
        scalar _defuzzifiedValue;
        bool _lockDefuzzifiedValue;

        scalar _minimumOutputRange;
        scalar _maximumOutputRange;

    public:
        OutputVariable(const std::string& name = "",
                scalar defaultValue = std::numeric_limits<scalar>::quiet_NaN(),
                bool lockDefuzzifiedValue = true);
        virtual ~OutputVariable();

        virtual void configure(Configuration* config);

        virtual Accumulated* output() const;

        virtual void setDefuzzifier(Defuzzifier* defuzzifier);
        virtual Defuzzifier* getDefuzzifier() const;

        virtual void setDefaultValue(scalar defaultValue);
        virtual scalar getDefaultValue() const;

        virtual void setDefuzzifiedValue(scalar defuzzifiedValue);
        virtual scalar getDefuzzifiedValue() const;

        virtual void setLockDefuzzifiedValue(bool lock);
        virtual bool lockDefuzzifiedValue() const;

        virtual void setMininumOutputRange(scalar minimum);
        virtual scalar getMinimumOutputRange() const;

        virtual void setMaximumOutputRange(scalar maximum);
        virtual scalar getMaximumOutputRange() const;

        /*if the defuzzified value is locked, executing this method
         stores the defuzzified value to be returned in case the next
         defuzzification no rules applied for this variable.
         The parameter overrideLock, when true, prevents the variable to lock the
         defuzzified value. Thus, multiple calls of defuzzify can be performed
         and yet the last locked value will not be updated.
         By default, overrideLock is false, and hence every call to defuzzify will
         store the defuzzification value if lockDefuzzifiedValue is true.
         In brief, the combinations are:
         a) lockDefuzzifiedValue = false && overrideLock = false, defuzzify does not lock the value.
         b) lockDefuzzifiedValue = false && overrideLock = true, same as before since the value is not to be locked.
         c) lockDefuzzifiedValue = true && overrideLock = false, the defuzzified value is locked
         d) lockDefuzzifiedValue = true && overrideLock = true, the defuzzified value is not locked
         */
        virtual scalar defuzzify(bool overrideLock = false);

    };

} /* namespace fl */
#endif /* FL_OUTPUTVARIABLE_H_ */
