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

#ifndef FL_ZSHAPE_H
#define FL_ZSHAPE_H

#include "fl/term/Term.h"

namespace fl {

    class FL_API ZShape : public Term {
    private:
        scalar _start, _end;

    public:
        explicit ZShape(const std::string& name = "",
                scalar _start = fl::nan,
                scalar _end = fl::nan,
                scalar _height = 1.0);
        virtual ~ZShape() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(ZShape)

        virtual std::string className() const FL_IOVERRIDE;
        virtual std::string parameters() const FL_IOVERRIDE;
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        virtual scalar membership(scalar x) const FL_IOVERRIDE;

        virtual void setStart(scalar start);
        virtual scalar getStart() const;

        virtual void setEnd(scalar end);
        virtual scalar getEnd() const;

        virtual ZShape* clone() const FL_IOVERRIDE;

        static Term* constructor();

    };
}
#endif  /* ZSHAPE_H */

