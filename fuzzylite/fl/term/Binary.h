/*
 Copyright © 2010-2016 by FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.
 */

#ifndef FL_BINARY_H
#define FL_BINARY_H

#include "fl/term/Term.h"

namespace fl {

    /**

      The Binary class is an edge Term that represents the binary membership
      function.

      @image html binary.svg

      @author Juan Rada-Vilela, Ph.D.
      @see Term
      @see Variable
      @since 6.0

     */
    class FL_API Binary : public Term {
    private:
        scalar _start;
        scalar _direction;
    public:

        /**
         Direction is an enumerator that determines the direction of the
         edge.
         */
        enum Direction {
            /** `(_|)` increases to the right */
            Positive,
            /** `(--)` direction is NaN */
            Undefined,
            /** `(|_)` increases to the left */
            Negative
        };

        explicit Binary(const std::string& name = "", scalar start = fl::nan,
                scalar direction = fl::nan, scalar height = 1.0);
        virtual ~Binary() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(Binary)

        virtual std::string className() const FL_IOVERRIDE;
        /**
          Returns the parameters of the term
          @return `"start direction [height]"`
         */
        virtual std::string parameters() const FL_IOVERRIDE;
        /**
          Configures the term with the parameters
          @param parameters as `"start direction [height]"`
         */
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        /**
          Computes the membership function evaluated at @f$x@f$
          @param x
          @return @f$\begin{cases}
          1h & \mbox{if $ \left(s < d \vedge x \in [s, d)\right) \wedge
          \left( s > d \vedge x \in (d, s] \right) $} \cr
          0h & \mbox{otherwise}
          \end{cases}@f$

          where @f$h@f$ is the height of the Term,
                @f$s@f$ is the start of the Binary edge,
                @f$d@f$ is the direction of the Binary edge.
         */
        virtual scalar membership(scalar x) const FL_IOVERRIDE;

        /**
         Sets the start of the binary edge
         @param start is the start of the binary edge
         */
        virtual void setStart(scalar start);
        /**
          Gets the start of the binary edge
          @return the start of the binary edge
         */
        virtual scalar getStart() const;

        /**
          Sets the direction of the binary edge. The direction is automatically
          converted to:

          @f$\begin{cases}
          \infty & \mbox{if $ d > s $}\cr
          -\infty & \mbox{if $ d < s $}\cr
          \mbox{\tt NaN} & \mbox{otherwise}
          \end{cases}
          @f$

          where @f$d@f$ is the given direction, and
                @f$s@f$ is the start of the Binary edge

          @param direction is the direction of the binary edge
         */
        virtual void setDirection(scalar direction);
        /**
          Gets the direction of the binary edge
          @return the direction of the binary edge
         */
        virtual scalar getDirection() const;

        /**
          Gets the Direction of the binary edge as an enum
          @return the Direction of the binary edge as an enum
         */
        virtual Direction direction() const;

        virtual Binary* clone() const FL_IOVERRIDE;

        static Term* constructor();
    };
}
#endif /* FL_BINARY_H */
