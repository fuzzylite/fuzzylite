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

#ifndef FL_SIGMOID_H
#define FL_SIGMOID_H

#include "fl/term/Term.h"

namespace fl {

    /**
      The Sigmoid class is an edge Term that represents the sigmoid membership
      function.

      @image html sigmoid.svg

      @author Juan Rada-Vilela, Ph.D.
      @see Term
      @see Variable
      @since 4.0
     */
    class FL_API Sigmoid : public Term {
    private:
        scalar _inflection;
        scalar _slope;
    public:

        /**
          Direction is an enumerator that indicates the direction of the
          sigmoid.
         */
        enum Direction {
            /** `(_/)` increases to the right */ Positive,
            /** `(--)` slope is zero */ Zero,
            /** `(\\_)` increases to the left */ Negative
        };
        explicit Sigmoid(const std::string& name = "",
                scalar inflection = fl::nan,
                scalar slope = fl::nan,
                scalar height = 1.0);
        virtual ~Sigmoid() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(Sigmoid)

        virtual std::string className() const FL_IOVERRIDE;
        /**
          Returns the parameters of the term
          @return `"inflection slope [height]"`
         */
        virtual std::string parameters() const FL_IOVERRIDE;
        /**
          Configures the term with the parameters
          @param parameters as `"inflection slope [height]"`
         */
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        virtual Complexity complexity() const FL_IOVERRIDE;

        /**
          Computes the membership function evaluated at @f$x@f$
          @param x
          @return @f$ h / (1 + \exp(-s(x-i)))@f$

          where @f$h@f$ is the height of the Term,
                @f$s@f$ is the slope of the Sigmoid,
                @f$i@f$ is the inflection of the Sigmoid
         */
        virtual scalar membership(scalar x) const FL_IOVERRIDE;

        virtual scalar tsukamoto(scalar activationDegree,
                scalar minimum, scalar maximum) const FL_IOVERRIDE;

        virtual bool isMonotonic() const FL_IOVERRIDE;
        
        /**
          Sets the inflection of the sigmoid
          @param inflection is the inflection of the sigmoid
         */
        virtual void setInflection(scalar inflection);
        /**
          Gets the inflection of the sigmoid
          @return the inflection of the sigmoid
         */
        virtual scalar getInflection() const;

        /**
          Sets the slope of the sigmoid
          @param slope is the slope of the sigmoid
         */
        virtual void setSlope(scalar slope);
        /**
          Gets the slope of the sigmoid
          @return the slope of the sigmoid
         */
        virtual scalar getSlope() const;

        /**
          Returns the direction of the sigmoid
          @return the direction of the sigmoid
         */
        virtual Direction direction() const;

        virtual Sigmoid* clone() const FL_IOVERRIDE;

        static Term* constructor();
    };
}
#endif /* FL_SIGMOID_H */
