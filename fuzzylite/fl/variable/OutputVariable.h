/*
 Author: Juan Rada-Vilela, Ph.D.
 Copyright © 2010-2015 FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#ifndef FL_OUTPUTVARIABLE_H
#define FL_OUTPUTVARIABLE_H

#include "fl/variable/Variable.h"

namespace fl {
    class Accumulated;
    class Defuzzifier;

    class FL_API OutputVariable : public Variable {
    private:
        void copyFrom(const OutputVariable& other);
    protected:
        FL_unique_ptr<Accumulated> _fuzzyOutput;
        FL_unique_ptr<Defuzzifier> _defuzzifier;
        scalar _previousValue;
        scalar _defaultValue;
        bool _lockValueInRange;
        bool _lockPreviousValue;

    public:
        explicit OutputVariable(const std::string& name = "",
                scalar minimum = -fl::inf, scalar maximum = fl::inf);
        OutputVariable(const OutputVariable& other);
        OutputVariable& operator=(const OutputVariable& other);
        virtual ~OutputVariable() FL_IOVERRIDE;
        FL_DEFAULT_MOVE(OutputVariable)

        virtual Accumulated* fuzzyOutput() const;

        virtual void setName(const std::string& name) FL_IOVERRIDE;

        virtual void setMinimum(scalar minimum) FL_IOVERRIDE;
        virtual void setMaximum(scalar maximum) FL_IOVERRIDE;

        virtual void setDefuzzifier(Defuzzifier* defuzzifier);
        virtual Defuzzifier* getDefuzzifier() const;

        virtual void setPreviousValue(scalar defuzzifiedValue);
        virtual scalar getPreviousValue() const;

        virtual void setDefaultValue(scalar defaultValue);
        virtual scalar getDefaultValue() const;

        virtual void setLockValueInRange(bool lockValueInRange);
        virtual bool isLockValueInRange() const;

        virtual void setLockPreviousValue(bool lockPreviousOutputValue);
        virtual bool isLockPreviousValue() const;

        virtual void defuzzify();

        virtual std::string fuzzyOutputValue() const;

        virtual void clear();

        virtual std::string toString() const FL_IOVERRIDE;

    };

}
#endif /* FL_OUTPUTVARIABLE_H */
