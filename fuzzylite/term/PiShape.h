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

#ifndef FL_PISHAPE_H
#define FL_PISHAPE_H

#include "fl/term/Term.h"

namespace fl {

    /**
      The PiShape class is an extended Term that represents the Pi-shaped curve
      membership function.

      @image html piShape.svg

      @author Juan Rada-Vilela, Ph.D.
      @see Term
      @see Variable
      @since 4.0
     */
    class FL_API PiShape : public Term {
    private:
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
        /**
          Returns the parameters of the term
          @return `"bottomLeft topLeft topRight bottomRight [height]"`
         */
        virtual std::string parameters() const FL_IOVERRIDE;
        /**
          Configures the term with the parameters
          @param parameters as `"bottomLeft topLeft topRight bottomRight
          [height]"`
         */
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        virtual Complexity complexity() const FL_IOVERRIDE;
        /**
          Computes the membership function evaluated at @f$x@f$
          @param x
          @return @f$\begin{cases}
          0h & \mbox{if $x \leq b_l$}\cr
          2h \left((x - b_l) / (t_l-b_l)\right)^2 & \mbox{if $x \leq 0.5(a+b)$}\cr
          h (1 - 2 \left((x - t_l) / (t_l-b_l)\right)^2) & \mbox{if $ x < t_l$}\cr
          h & \mbox{if $x \leq t_r$}\cr
          h (1 - 2\left((x - t_r) / (b_r - t_r)\right)^2) & \mbox{if $x \leq 0.5(t_r + b_r)$}\cr
          2h \left((x - b_r) / (b_r-t_r)\right)^2 & \mbox{if $x < b_r$} \cr
          0h & \mbox{otherwise}
          \end{cases}@f$

          where @f$h@f$ is the height of the Term,
                @f$b_l@f$ is the bottom left of the PiShape,
                @f$t_l@f$ is the top left of the PiShape,
                @f$t_r@f$ is the top right of the PiShape
                @f$b_r@f$ is the bottom right of the PiShape,
         */
        virtual scalar membership(scalar x) const FL_IOVERRIDE;

        /**
          Sets the bottom-left value of the curve
          @param bottomLeft is the bottom-left value of the curve
         */
        virtual void setBottomLeft(scalar bottomLeft);
        /**
          Gets the bottom-left value of the curve
          @return the bottom-left value of the curve
         */
        virtual scalar getBottomLeft() const;

        /**
          Sets the top-left value of the curve
          @param topLeft is the top-left value of the curve
         */
        virtual void setTopLeft(scalar topLeft);
        /**
          Gets the top-left value of the curve
          @return the top-left value of the curve
         */
        virtual scalar getTopLeft() const;

        /**
          Sets the top-right value of the curve
          @param topRight is the top-right value of the curve
         */
        virtual void setTopRight(scalar topRight);
        /**
          Gets the top-right value of the curve
          @return the top-right value of the curve
         */
        virtual scalar getTopRight() const;

        /**
          Sets the bottom-right value of the curve
          @param bottomRight is the bottom-right value of the curve
         */
        virtual void setBottomRight(scalar bottomRight);
        /**
          Gets the bottom-right value of the curve
          @return the bottom-right value of the curve
         */
        virtual scalar getBottomRight() const;

        virtual PiShape* clone() const FL_IOVERRIDE;

        static Term* constructor();
    };
}
#endif  /* FL_PISHAPE_H */

