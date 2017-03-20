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

#ifndef FL_COSINE_H
#define FL_COSINE_H

#include "fl/term/Term.h"

namespace fl {

    /**
      The Cosine class is an extended Term that represents the cosine
      membership function.

      @image html cosine.svg

      @author Juan Rada-Vilela, Ph.D.
      @see Term
      @see Variable
      @since 5.0
     */

    class FL_API Cosine : public Term {
    private:
        scalar _center, _width;
    public:
        explicit Cosine(const std::string& name = "",
                scalar center = fl::nan,
                scalar width = fl::nan,
                scalar height = 1.0);
        virtual ~Cosine() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(Cosine)

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
          @return @f$\begin{cases}
          0h & \mbox{if $x < c - 0.5w \vee x > c + 0.5w$} \cr
          0.5h \times ( 1 + \cos(2.0 / w\pi(x-c))) & \mbox{otherwise}
          \end{cases}@f$

          where @f$h@f$ is the height of the Term,
                @f$c@f$ is the center of the Cosine,
                @f$w@f$ is the width of the Cosine
         */
        virtual scalar membership(scalar x) const FL_IOVERRIDE;
        /**
          Sets the center of the cosine
          @param center is the center of the cosine
         */
        virtual void setCenter(scalar center);
        /**
          Gets the center of the cosine
          @return the center of the cosine
         */
        virtual scalar getCenter() const;

        /**
          Sets the width of the cosine
          @param width is the width of the cosine
         */
        virtual void setWidth(scalar width);
        /**
          Gets the width of the cosine
          @return the width of the cosine
         */
        virtual scalar getWidth() const;

        virtual Cosine* clone() const FL_IOVERRIDE;

        static Term* constructor();
    };
}
#endif  /* FL_COSINE_H */

