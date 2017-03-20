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

#ifndef FL_SIGMOIDDIFFERENCE_H
#define FL_SIGMOIDDIFFERENCE_H

#include "fl/term/Term.h"

namespace fl {

    /**
      The SigmoidDifference class is an extended Term that represents the
      difference between two sigmoidal membership functions.

      @image html sigmoidDifference.svg

      @author Juan Rada-Vilela, Ph.D.
      @see Term
      @see Variable
      @since 4.0
     */
    class FL_API SigmoidDifference : public Term {
    private:
        scalar _left;
        scalar _rising;
        scalar _falling;
        scalar _right;

    public:
        explicit SigmoidDifference(const std::string& name = "",
                scalar left = fl::nan,
                scalar rising = fl::nan,
                scalar falling = fl::nan,
                scalar right = fl::nan,
                scalar height = 1.0);
        virtual ~SigmoidDifference() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(SigmoidDifference)

        virtual std::string className() const FL_IOVERRIDE;
        /**
          Returns the parameters of the term
          @return `"left rising falling right [height]"`
         */
        virtual std::string parameters() const FL_IOVERRIDE;
        /**
          Configures the term with the parameters
          @param parameters as `"left rising falling right [height]"`
         */
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        virtual Complexity complexity() const FL_IOVERRIDE;

        /**
          Computes the membership function evaluated at @f$x@f$
          @param x
          @return @f$ h (a-b)@f$

          where @f$h@f$ is the height of the Term,
                @f$a= 1 / (1 + \exp(-s_l \times (x - i_l))) @f$,
                @f$b = 1 / (1 + \exp(-s_r \times (x - i_r)))@f$,
                @f$i_l@f$ is the left inflection of the SigmoidDifference,
                @f$s_l@f$ is the left slope of the SigmoidDifference,
                @f$i_r@f$ is the right inflection of the SigmoidDifference,
                @f$s_r@f$ is the right slope of the SigmoidDifference
         */
        virtual scalar membership(scalar x) const FL_IOVERRIDE;

        /**
          Sets the inflection of the left sigmoidal curve
          @param leftInflection is the inflection of the left sigmoidal curve
         */
        virtual void setLeft(scalar leftInflection);
        /**
          Gets the inflection of the left sigmoidal curve
          @return the inflection of the left sigmoidal curve
         */
        virtual scalar getLeft() const;

        /**
          Sets the slope of the left sigmoidal curve
          @param risingSlope is the slope of the left sigmoidal curve
         */
        virtual void setRising(scalar risingSlope);
        /**
          Gets the slope of the left sigmoidal curve
          @return the slope of the left sigmoidal curve
         */
        virtual scalar getRising() const;

        /**
          Sets the slope of the right sigmoidal curve
          @param fallingSlope is the slope of the right sigmoidal curve
         */
        virtual void setFalling(scalar fallingSlope);
        /**
          Gets the slope of the right sigmoidal curve
          @return the slope of the right sigmoidal curve
         */
        virtual scalar getFalling() const;

        /**
          Sets the inflection of the right sigmoidal curve
          @param rightInflection is the inflection of the right sigmoidal curve
         */
        virtual void setRight(scalar rightInflection);
        /**
          Gets the inflection of the right sigmoidal curve
          @return the inflection of the right sigmoidal curve
         */
        virtual scalar getRight() const;

        virtual SigmoidDifference* clone() const FL_IOVERRIDE;

        static Term* constructor();
    };
}
#endif  /* FL_SIGMOIDDIFFERENCE_H */

