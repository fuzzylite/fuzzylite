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

#ifndef FL_CONSTANT_H
#define FL_CONSTANT_H

#include "fl/term/Term.h"

namespace fl {

    /**
      The Constant class is a (zero) polynomial Term that represents a constant
      value @f$ f(x) = k @f$

      @author Juan Rada-Vilela, Ph.D.
      @see Term
      @see Variable
      @since 4.0
     */
    class FL_API Constant : public Term {
    private:
        scalar _value;

    public:
        explicit Constant(const std::string& name = "",
                scalar value = fl::nan);
        virtual ~Constant() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(Constant)

        virtual std::string className() const FL_IOVERRIDE;
        /**
          Returns the parameters of the term
          @return `"value"`
         */
        virtual std::string parameters() const FL_IOVERRIDE;
        /**
          Configures the term with the parameters
          @param parameters as `"value"`
         */
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        virtual Complexity complexity() const FL_IOVERRIDE;

        /**
          Computes the membership function evaluated at @f$x@f$
          @param x is irrelevant
          @return @f$c@f$, where @f$c@f$ is the constant value
         */
        virtual scalar membership(scalar x) const FL_IOVERRIDE;

        /**
          Sets the constant value
          @param value is the constant value
         */
        virtual void setValue(scalar value);
        /**
          Gets the constant value
          @return the constant value
         */
        virtual scalar getValue() const;

        virtual Constant* clone() const FL_IOVERRIDE;

        static Term* constructor();
    };
}
#endif  /* FL_CONSTANT_H */

