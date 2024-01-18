/*
 fuzzylite (R), a fuzzy logic control library in C++.
 Copyright (C) 2010-2017 FuzzyLite Limited. All rights reserved.
 Author: Juan Rada-Vilela, Ph.D. <jcrada@fuzzylite.com>

 This file is part of fuzzylite.

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite is a registered trademark of FuzzyLite Limited.
 */

#ifndef FL_RECTANGLE_H
#define FL_RECTANGLE_H

#include "fl/term/Term.h"

namespace fl {

    /**
      The Rectangle class is a basic Term that represents the rectangle
      membership function.

      @image html rectangle.svg

      @author Juan Rada-Vilela, Ph.D.
      @see Term
      @see Variable
      @since 4.0
     */
    class FL_API Rectangle : public Term {
    private:
        scalar _start, _end;

    public:
        explicit Rectangle(const std::string& name = "",
                scalar start = fl::nan,
                scalar end = fl::nan,
                scalar height = 1.0);
        virtual ~Rectangle() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(Rectangle)

        virtual std::string className() const FL_IOVERRIDE;
        /**
          Returns the parameters of the term
          @return `"start end [height]"`
         */
        virtual std::string parameters() const FL_IOVERRIDE;
        /**
          Configures the term with the parameters
          @param parameters as `"start end [height]"`
         */
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        virtual Complexity complexity() const FL_IOVERRIDE;

        /**
          Computes the membership function evaluated at @f$x@f$
          @param x
          @return @f$\begin{cases}
          1h & \mbox{if $x \in [s, e]$} \cr
          0h & \mbox{otherwise}
          \end{cases}@f$

          where @f$h@f$ is the height of the Term,
                @f$s@f$ is the start of the Rectangle,
                @f$e@f$ is the end of the Rectangle.
         */
        virtual scalar membership(scalar x) const FL_IOVERRIDE;

        /**
          Sets the start of the rectangle
          @param start is the start of the rectangle
         */
        virtual void setStart(scalar start);
        /**
          Gets the start of the rectangle
          @return the start of the rectangle
         */
        virtual scalar getStart() const;

        /**
          Sets the end of the rectangle
          @param end is the end of the rectangle
         */
        virtual void setEnd(scalar end);
        /**
          Gets the end of the rectangle
          @return the end of the rectangle
         */
        virtual scalar getEnd() const;

        virtual Rectangle* clone() const FL_IOVERRIDE;

        static Term* constructor();
    };
}
#endif /* FL_RECTANGLE_H */
