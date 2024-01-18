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

#ifndef FL_RAMP_H
#define FL_RAMP_H

#include "fl/term/Term.h"

namespace fl {

    /**
      The Ramp class is an edge Term that represents the ramp membership
      function.

      @image html ramp.svg

      @author Juan Rada-Vilela, Ph.D.
      @see Term
      @see Variable
      @since 4.0
     */
    class FL_API Ramp : public Term {
    private:
        scalar _start, _end;

    public:

        /**
          Direction is an enumerator that indicates the direction of the ramp.
         */
        enum Direction {
            /** `(_/)` increases to the right */ Positive,
            /** `(--)` slope is zero */ Zero,
            /** `(\\_)` increases to the left */ Negative
        };
        explicit Ramp(const std::string& name = "",
                scalar start = fl::nan,
                scalar end = fl::nan,
                scalar height = 1.0);
        virtual ~Ramp() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(Ramp)

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
          @return
          @f$\begin{cases}

          0h & \mbox{if $x = e$}\cr

          \begin{cases}
          0h & \mbox{if $x \leq s$}\cr
          1h & \mbox{if $x \geq e$}\cr
          h (x - s) / (e - s) & \mbox{otherwise}\cr
          \end{cases} & \mbox{if $s < e$}\cr

          \begin{cases}
          0h & \mbox{if $x \geq s$}\cr
          1h & \mbox{if $x \leq e$}\cr
          h (s - x) / (s - e) & \mbox{otherwise}
          \end{cases} & \mbox{if $s > e$}\cr
          \end{cases}@f$

          where @f$h@f$ is the height of the Term,
                @f$s@f$ is the start of the Ramp,
                @f$e@f$ is the end of the Ramp
         */
        virtual scalar membership(scalar x) const FL_IOVERRIDE;

        virtual scalar tsukamoto(scalar activationDegree,
                scalar minimum, scalar maximum) const FL_IOVERRIDE;

        virtual bool isMonotonic() const FL_IOVERRIDE;

        /**
          Sets the start of the ramp
          @param start is the start of the ramp
         */
        virtual void setStart(scalar start);
        /**
          Gets the start of the ramp
          @return the start of the ramp
         */
        virtual scalar getStart() const;

        /**
          Sets the end of the ramp
          @param end is the end of the ramp
         */
        virtual void setEnd(scalar end);
        /**
          Gets the end of the ramp
          @return the end of the ramp
         */
        virtual scalar getEnd() const;

        /**
          Returns the direction of the ramp
          @return the direction of the ramp
         */
        virtual Direction direction() const;

        virtual Ramp* clone() const FL_IOVERRIDE;

        static Term* constructor();
    };
}
#endif  /* FL_RAMP_H */

