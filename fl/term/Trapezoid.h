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

#ifndef FL_TRAPEZOID_H
#define FL_TRAPEZOID_H

#include "fl/term/Term.h"

namespace fl {

    /**
      The Trapezoid class is a basic Term that represents the trapezoidal
      membership function.

      @image html trapezoid.svg

      @author Juan Rada-Vilela, Ph.D.
      @see Term
      @see Variable
      @since 4.0
     */
    class FL_API Trapezoid : public Term {
    private:
        scalar _vertexA, _vertexB, _vertexC, _vertexD;
    public:
        explicit Trapezoid(const std::string& name = "",
                scalar vertexA = fl::nan,
                scalar vertexB = fl::nan,
                scalar vertexC = fl::nan,
                scalar vertexD = fl::nan,
                scalar height = 1.0);
        virtual ~Trapezoid() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(Trapezoid)

        virtual std::string className() const FL_IOVERRIDE;
        /**
          Returns the parameters of the term
          @return `"vertexA vertexB vertexC vertexD [height]"`
         */
        virtual std::string parameters() const FL_IOVERRIDE;
        /**
          Configures the term with the parameters
          @param parameters as `"vertexA vertexB vertexC vertexD [height]"`
         */
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        virtual Complexity complexity() const FL_IOVERRIDE;
        /**
          Computes the membership function evaluated at @f$x@f$
          @param x
          @return @f$\begin{cases}
          0h & \mbox{if $x \not\in[a,d]$}\cr
          h \times \min(1, (x - a) / (b - a)) & \mbox{if $x < b$}\cr
          1h & \mbox{if $x \leq c$}\cr
          h (d - x) / (d - c) & \mbox{if $x < d$}\cr
          0h & \mbox{otherwise}
          \end{cases}@f$

          where @f$h@f$ is the height of the Term,
                @f$a@f$ is the first vertex of the Trapezoid,
                @f$b@f$ is the second vertex of the Trapezoid,
                @f$c@f$ is the third vertex of the Trapezoid,
                @f$d@f$ is the fourth vertex of the Trapezoid
         */
        virtual scalar membership(scalar x) const FL_IOVERRIDE;

        /**
          Sets the first vertex of the trapezoid
          @param a is the first vertex of the trapezoid
         */
        virtual void setVertexA(scalar a);
        /**
          Gets the first vertex of the trapezoid
          @return the first vertex of the trapezoid
         */
        virtual scalar getVertexA() const;

        /**
          Sets the second vertex of the trapezoid
          @param b is the second vertex of the trapezoid
         */
        virtual void setVertexB(scalar b);
        /**
          Gets the second vertex of the trapezoid
          @return the second vertex of the trapezoid
         */
        virtual scalar getVertexB() const;

        /**
          Sets the third vertex of the trapezoid
          @param c is the third vertex of the trapezoid
         */
        virtual void setVertexC(scalar c);
        /**
          Gets the third vertex of the trapezoid
          @return the third vertex of the trapezoid
         */
        virtual scalar getVertexC() const;

        /**
          Sets the fourth vertex of the trapezoid
          @param d is the fourth vertex of the trapezoid
         */
        virtual void setVertexD(scalar d);
        /**
          Gets the fourth vertex of the trapezoid
          @return the fourth vertex of the trapezoid
         */
        virtual scalar getVertexD() const;

        virtual Trapezoid* clone() const FL_IOVERRIDE;

        static Term* constructor();
    };
}
#endif /* FL_TRAPEZOID_H */
