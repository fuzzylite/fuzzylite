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

#ifndef FL_SSHAPE_H
#define FL_SSHAPE_H

#include "fl/term/Term.h"

namespace fl {

    /**
      The SShape class is an edge Term that represents the S-shaped membership
      function.

      @image html sShape.svg

      @author Juan Rada-Vilela, Ph.D.
      @see Term
      @see Variable
      @since 4.0
     */
    class FL_API SShape : public Term {
    private:
        scalar _start, _end;

    public:
        explicit SShape(const std::string& name = "",
                scalar start = fl::nan,
                scalar end = fl::nan,
                scalar height = 1.0);
        virtual ~SShape() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(SShape)

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
          0h & \mbox{if $x \leq s$} \cr
          h(2 \left((x - s) / (e-s)\right)^2) & \mbox{if $x \leq 0.5(s+e)$}\cr
          h(1 - 2\left((x - e) / (e-s)\right)^2) & \mbox{if $x < e$}\cr
          1h & \mbox{otherwise}
          \end{cases}@f$

          where @f$h@f$ is the height of the Term,
                @f$s@f$ is the start of the SShape,
                @f$e@f$ is the end of the SShape.
         */
        virtual scalar membership(scalar x) const FL_IOVERRIDE;

        virtual scalar tsukamoto(scalar activationDegree,
                scalar minimum, scalar maximum) const FL_IOVERRIDE;

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

        virtual SShape* clone() const FL_IOVERRIDE;

        static Term* constructor();
    };
}
#endif  /* FL_SSHAPE_H */

