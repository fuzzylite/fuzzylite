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

#ifndef FL_CONSTANT_H
#define FL_CONSTANT_H

#include "fl/term/Term.h"

namespace fl {

    class FL_API Constant : public Term {
    private:
        scalar _value;

    public:
        explicit Constant(const std::string& name = "",
                scalar value = fl::nan);
        virtual ~Constant() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(Constant)

        virtual std::string className() const FL_IOVERRIDE;
        virtual std::string parameters() const FL_IOVERRIDE;
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        virtual scalar membership(scalar x) const FL_IOVERRIDE;

        virtual void setValue(scalar value);
        virtual scalar getValue() const;

        virtual Constant* clone() const FL_IOVERRIDE;

        static Term* constructor();
    };
}

#endif  /* FL_CONSTANT_H */

