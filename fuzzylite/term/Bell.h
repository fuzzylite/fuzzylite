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

#ifndef FL_BELL_H
#define FL_BELL_H

#include "fl/term/Term.h"

namespace fl {

    /**
      The Bell class is an extended Term that represents the generalized bell
      curve membership function.

      @image html bell.svg

      @author Juan Rada-Vilela, Ph.D.
      @see Term
      @see Variable
      @since 4.0
     */
    class FL_API Bell : public Term {
    private:
        scalar _center;
        scalar _width;
        scalar _slope;
    public:
        explicit Bell(const std::string& name = "",
                scalar center = fl::nan,
                scalar width = fl::nan,
                scalar slope = fl::nan,
                scalar height = 1.0);
        virtual ~Bell() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(Bell)

        virtual std::string className() const FL_IOVERRIDE;
        /**
          Returns the parameters of the term
          @return `"center width slope [height]"`
         */
        virtual std::string parameters() const FL_IOVERRIDE;
        /**
          Configures the term with the parameters
          @param parameters as `"center width slope [height]"`
         */
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        virtual Complexity complexity() const FL_IOVERRIDE;

        /**
          Computes the membership function evaluated at @f$x@f$
          @param x
          @return @f$h / (1 + \left(|x-c|/w\right)^{2s}@f$

          where @f$h@f$ is the height of the Term,
                @f$c@f$ is the center of the Bell,
                @f$w@f$ is the width of the Bell,
                @f$s@f$ is the slope of the Bell
         */
        virtual scalar membership(scalar x) const FL_IOVERRIDE;

        /**
          Sets the center of the bell curve
          @param center is the center of the bell curve
         */
        virtual void setCenter(scalar center);
        /**
          Gets the center of the bell curve
          @return the center of the bell curve
         */
        virtual scalar getCenter() const;

        /**
          Sets the width of the bell curve
          @param width is the width of the bell curve
         */
        virtual void setWidth(scalar width);
        /**
          Gets the width of the bell curve
          @return the width of the bell curve
         */
        virtual scalar getWidth() const;

        /**
          Sets the slope of the bell curve
          @param slope is the slope of the bell curve
         */
        virtual void setSlope(scalar slope);
        /**
          Gets the slope of the bell curve
          @return the slope of the bell curve
         */
        virtual scalar getSlope() const;

        virtual Bell* clone() const FL_IOVERRIDE;

        static Term* constructor();

    };
}
#endif /* FL_BELL_H */
