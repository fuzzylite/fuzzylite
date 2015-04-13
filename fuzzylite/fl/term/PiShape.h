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

#ifndef FL_PISHAPE_H
#define FL_PISHAPE_H

#include "fl/term/Term.h"

namespace fl {

    class FL_API PiShape : public Term {
    protected:
        scalar _bottomLeft;
        scalar _topLeft;
        scalar _topRight;
        scalar _bottomRight;

    public:
        explicit PiShape(const std::string& name = "",
                scalar bottomLeft = fl::nan,
                scalar topLeft = fl::nan,
                scalar topRight = fl::nan,
                scalar bottomRight = fl::nan,
                scalar height = 1.0);
        virtual ~PiShape() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(PiShape)

        virtual std::string className() const FL_IOVERRIDE;
        virtual std::string parameters() const FL_IOVERRIDE;
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        virtual scalar membership(scalar x) const FL_IOVERRIDE;

        virtual void setBottomLeft(scalar a);
        virtual scalar getBottomLeft() const;

        virtual void setTopLeft(scalar b);
        virtual scalar getTopLeft() const;

        virtual void setTopRight(scalar d);
        virtual scalar getTopRight() const;

        virtual void setBottomRight(scalar c);
        virtual scalar getBottomRight() const;

        virtual PiShape* clone() const FL_IOVERRIDE;

        static Term* constructor();
    };
}

#endif  /* FL_PISHAPE_H */

