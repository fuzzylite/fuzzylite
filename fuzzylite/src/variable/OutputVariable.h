/*
 * OutputVariable.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_OUTPUTVARIABLE_H_
#define FL_OUTPUTVARIABLE_H_

#include "Variable.h"

namespace fl {
    class Accumulated;
    class Defuzzifier;

    class OutputVariable: public Variable {
    protected:
        Defuzzifier* _defuzzifier;
        Accumulated* _output;
        scalar _defaultValue;
        scalar _unchangedValue;

    public:
        OutputVariable(const std::string& name);
        virtual ~OutputVariable();

        virtual void configure(Configuration* config);

        virtual void setDefaultValue(scalar defaultValue);
        virtual scalar getDefaultValue() const;

        virtual void setUnchangedValue(scalar unchangedValue);
        virtual scalar getUnchangedValue() const;

        virtual void setDefuzzifier(Defuzzifier* defuzzifier);
        virtual Defuzzifier* getDefuzzifier() const;

        virtual Accumulated* output() const;

        virtual scalar defuzzify();

        virtual std::string toString() const;

    };

} /* namespace fl */
#endif /* FL_OUTPUTVARIABLE_H_ */
