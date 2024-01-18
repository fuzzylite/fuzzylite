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

#ifndef FL_TRIANGLE_H
#define FL_TRIANGLE_H

#include "fl/term/Term.h"

namespace fl {

    /**
      The Triangle class is a basic Term that represents the triangular
      membership function.

      @image html triangle.svg

      @author Juan Rada-Vilela, Ph.D.
      @see Term
      @see Variable
      @since 4.0
     */
    class FL_API Triangle : public Term {
    private:
        scalar _vertexA;
        scalar _vertexB;
        scalar _vertexC;
    public:
        explicit Triangle(const std::string& name = "",
                scalar vertexA = fl::nan,
                scalar vertexB = fl::nan,
                scalar vertexC = fl::nan,
                scalar height = 1.0);
        virtual ~Triangle() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(Triangle)

        virtual std::string className() const FL_IOVERRIDE;
        /**
          Returns the parameters of the term
          @return `"vertexA vertexB vertexC [height]"`
         */
        virtual std::string parameters() const FL_IOVERRIDE;
        /**
          Configures the term with the parameters
          @param parameters as `"vertexA vertexB vertexC [height]"`
         */
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        virtual Complexity complexity() const FL_IOVERRIDE;
        /**
          Computes the membership function evaluated at @f$x@f$
          @param x
          @return @f$\begin{cases}
          0h & \mbox{if $x \not\in [a,c]$}\cr
          1h & \mbox{if $x = b$}\cr
          h (x - a) / (b - a) & \mbox{if $x < b$} \cr
          h (c - x) / (c - b) & \mbox{otherwise}
          \end{cases}@f$

          where @f$h@f$ is the height of the Term,
                @f$a@f$ is the first vertex of the Triangle,
                @f$b@f$ is the second vertex of the Triangle,
                @f$c@f$ is the third vertex of the Triangle
         */
        virtual scalar membership(scalar x) const FL_IOVERRIDE;

        /**
          Sets the first vertex of the triangle
          @param a is the first vertex of the triangle
         */
        virtual void setVertexA(scalar a);
        /**
          Gets the first vertex of the triangle
          @return the first vertex of the triangle
         */
        virtual scalar getVertexA() const;

        /**
          Sets the second vertex of the triangle
          @param b is the second vertex of the triangle
         */
        virtual void setVertexB(scalar b);
        /**
          Gets the second vertex of the triangle
          @return the second vertex of the triangle
         */
        virtual scalar getVertexB() const;

        /**
          Sets the third vertex of the triangle
          @param c is the third vertex of the triangle
         */
        virtual void setVertexC(scalar c);
        /**
          Gets the third vertex of the triangle
          @return the third vertex of the triangle
         */
        virtual scalar getVertexC() const;

        virtual Triangle* clone() const FL_IOVERRIDE;

        static Term* constructor();

    };
}
#endif /* FL_TRIANGLE_H */
