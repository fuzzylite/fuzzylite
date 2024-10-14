/*
fuzzylite (R), a fuzzy logic control library in C++.

Copyright (C) 2010-2024 FuzzyLite Limited. All rights reserved.
Author: Juan Rada-Vilela, PhD <jcrada@fuzzylite.com>.

This file is part of fuzzylite.

fuzzylite is free software: you can redistribute it and/or modify it under
the terms of the FuzzyLite License included with the software.

You should have received a copy of the FuzzyLite License along with
fuzzylite. If not, see <https://github.com/fuzzylite/fuzzylite/>.

fuzzylite is a registered trademark of FuzzyLite Limited.
*/

#ifndef FL_ZSHAPE_H
#define FL_ZSHAPE_H

#include "fuzzylite/term/Term.h"

namespace fuzzylite {

    /**
      The ZShape class is an edge Term that represents the Z-shaped membership
      function.

      @image html zShape.svg

      @author Juan Rada-Vilela, Ph.D.
      @see Term
      @see Variable
      @since 4.0
     */

    class FL_API ZShape : public Term {
      private:
        scalar _start, _end;

      public:
        explicit ZShape(
            const std::string& name = "", scalar _start = fl::nan, scalar _end = fl::nan, scalar _height = 1.0
        );
        virtual ~ZShape() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(ZShape)

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

        /**
          Computes the membership function evaluated at @f$x@f$
          @param x
          @return @f$  \begin{cases}
          1h & \mbox{if $x \leq s$} \cr
          h(1 - 2\left((x - s) / (e-s)\right)^2) & \mbox{if $x \leq 0.5(s+e)$}\cr
          h(2 \left((x - e) / (e-s)\right)^2) & \mbox{if $x < e$}\cr
          0h & \mbox{otherwise}
          \end{cases}@f$

          where @f$h@f$ is the height of the Term,
                @f$s@f$ is the start of the ZShape,
                @f$e@f$ is the end of the ZShape.
         */
        virtual scalar membership(scalar x) const FL_IOVERRIDE;

        /**
        Compute the tsukamoto value of the monotonic term for activation degree @f$y$@f.

        Equation:

            @f$y = \begin{cases} 1 & \mbox{if } x \leq s \cr h - 2h\left(\dfrac{x - s}{e-s}\right)^2 & \mbox{if }  s < x
        < \dfrac{s+e}{2} \cr 2h \left(\dfrac{x - e}{e-s}\right)^2 & \mbox{if } \dfrac{s+e}{2} \le  x < e\cr 0 &
        \mbox{otherwise} \end{cases}$@f

            @f$x = \begin{cases}
                e + (e-s) \sqrt{\dfrac{y}{2h}} & \mbox{if } y \le \dfrac{h}{2} \cr
                s + (e-s) \sqrt{\dfrac{h-y}{2h}} & \mbox{otherwise}
            \end{cases}$@f

        @param y is the activation degree

        @return @f$x = \begin{cases} e + (e-s) \sqrt{\dfrac{y}{2h}} & \mbox{if } y \le \dfrac{h}{2} \cr s + (e-s)
        \sqrt{\dfrac{h-y}{2h}} & \mbox{otherwise} \end{cases}$@f
         */
        virtual scalar tsukamoto(scalar y) const FL_IOVERRIDE;

        virtual bool isMonotonic() const FL_IOVERRIDE;

        /**
          Sets the start of the edge
          @param start is the start of the edge
         */
        virtual void setStart(scalar start);
        /**
          Gets the start of the edge
          @return the start of the edge
         */
        virtual scalar getStart() const;

        /**
          Sets the end of the edge
          @param end is the end of the edge
         */
        virtual void setEnd(scalar end);
        /**
          Gets the end of the edge
          @return the end of the edge
         */
        virtual scalar getEnd() const;

        virtual ZShape* clone() const FL_IOVERRIDE;

        static Term* constructor();
    };
}
#endif /* ZSHAPE_H */
