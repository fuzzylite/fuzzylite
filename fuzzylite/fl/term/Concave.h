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

#ifndef FL_CONCAVE_H
#define FL_CONCAVE_H

#include "fl/term/Term.h"

namespace fl {

    /**
      The Concave class is an edge Term that represents the concave membership
      function.

      @image html concave.svg

      @author Juan Rada-Vilela, Ph.D.
      @see Term
      @see Variable
      @since 5.0
     */
    class FL_API Concave : public Term {
    private:
        scalar _inflection, _end;
    public:
        explicit Concave(const std::string& name = "",
                scalar inflection = fl::nan,
                scalar end = fl::nan,
                scalar height = 1.0);
        virtual ~Concave() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(Concave)

        virtual std::string className() const FL_IOVERRIDE;
        /**
          Returns the parameters of the term as
          @return `"inflection end [height]"`
         */
        virtual std::string parameters() const FL_IOVERRIDE;
        /**
          Configures the term with the parameters given
          @param parameters as `"inflection end [height]"`
         */
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        virtual Complexity complexity() const FL_IOVERRIDE;

        /**
          Computes the membership function evaluated at @f$x@f$
          @param x
          @return @f$\begin{cases}
          h \times (e - i) / (2e - i - x) & \mbox{if $i \leq e \wedge x < e$
          (increasing concave)} \cr
          h \times (i - e) / (-2e + i + x) & \mbox{if $i > e \wedge x > e$
          (decreasing concave)} \cr
          h & \mbox{otherwise} \cr
          \end{cases}@f$

          where @f$h@f$ is the height of the Term,
                @f$i@f$ is the inflection of the Concave,
                @f$e@f$ is the end of the Concave
         */
        virtual scalar membership(scalar x) const FL_IOVERRIDE;

        virtual scalar tsukamoto(scalar activationDegree,
                scalar minimum, scalar maximum) const FL_IOVERRIDE;

        virtual bool isMonotonic() const FL_IOVERRIDE;

        /**
          Sets the inflection of the curve
          @param inflection is the inflection of the curve
         */
        virtual void setInflection(scalar inflection);
        /**
          Gets the inflection of the curve
          @return the inflection of the curve
         */
        virtual scalar getInflection() const;

        /**
          Sets the end of the curve
          @param end is the end of the curve
         */
        virtual void setEnd(scalar end);
        /**
          Gets the end of the curve
          @return the end of the curve
         */
        virtual scalar getEnd() const;

        virtual Concave* clone() const FL_IOVERRIDE;

        static Term* constructor();
    };
}
#endif  /* FL_CONCAVE_H */

