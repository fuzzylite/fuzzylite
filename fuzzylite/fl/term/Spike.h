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

#ifndef FL_SPIKE_H
#define FL_SPIKE_H

#include "fl/term/Term.h"

namespace fl {

    /**
      The Spike class is an extended Term that represents the spike membership
      function.

      @image html spike.svg

      @author Juan Rada-Vilela, Ph.D.
      @see Term
      @see Variable
      @since 5.0
     */
    class FL_API Spike : public Term {
    private:
        scalar _center, _width;
    public:
        explicit Spike(const std::string& name = "",
                scalar center = fl::nan,
                scalar width = fl::nan,
                scalar height = 1.0);
        virtual ~Spike() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(Spike)

        virtual std::string className() const FL_IOVERRIDE;
        /**
          Returns the parameters of the term
          @return `"center width [height]"`
         */
        virtual std::string parameters() const FL_IOVERRIDE;
        /**
          Configures the term with the parameters
          @param parameters as `"center width [height]"`
         */
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        virtual Complexity complexity() const FL_IOVERRIDE;
        /**
          Computes the membership function evaluated at @f$x@f$
          @param x
          @return @f$h \times \exp(-|10 / w (x - c)|)@f$

          where @f$h@f$ is the height of the Term,
                @f$w@f$ is the width of the Spike,
                @f$c@f$ is the center of the Spike
         */
        virtual scalar membership(scalar x) const FL_IOVERRIDE;

        /**
          Sets the center of the spike
          @param center is the center of the spike
         */
        virtual void setCenter(scalar center);
        /**
          Gets the center of the spike
          @return the center of the spike
         */
        virtual scalar getCenter() const;

        /**
          Sets the width of the spike
          @param width is the width of the spike
         */
        virtual void setWidth(scalar width);
        /**
          Gets the width of the spike
          @return the width of the spike
         */
        virtual scalar getWidth() const;

        virtual Spike* clone() const FL_IOVERRIDE;

        static Term* constructor();
    };
}
#endif  /* FL_SPIKE_H */

