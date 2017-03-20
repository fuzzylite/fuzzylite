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

#ifndef FL_DISCRETE_H
#define FL_DISCRETE_H

#include "fl/term/Term.h"

#include "fl/defuzzifier/IntegralDefuzzifier.h"

#include <vector>
#include <utility>

namespace fl {

    /**
      The Discrete class is a basic Term that represents a discrete membership
      function. The pairs of values in any Discrete term **must** be sorted
      ascendently because the membership function is computed using binary search
      to find the lower and upper bounds of @f$x@f$.

      @image html discrete.svg

      @author Juan Rada-Vilela, Ph.D.
      @see Term
      @see Variable
      @since 4.0
     */
    class FL_API Discrete : public Term {
    public:
        typedef std::pair<scalar, scalar> Pair;
    private:
        std::vector<Pair> _xy;
    public:
        explicit Discrete(const std::string& name = "",
                const std::vector<Pair>& xy = std::vector<Pair>(),
                scalar height = 1.0);
        virtual ~Discrete() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(Discrete)

        virtual std::string className() const FL_IOVERRIDE;
        /**
          Returns the parameters of the term as `x1 y1 xn yn [height]`
          @return `x1 y1 xn yn [height]`
         */
        virtual std::string parameters() const FL_IOVERRIDE;
        /**
          Configures the term with the parameters given as `x1 y1 xn yn [height]`
          @param parameters as `x1 y1 xn yn [height]`
         */
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        /**
         Ascendantly sorts the given pairs of values by the @f$x@f$-value,
         as it is required by the Discrete term.
         @param pairs is a vector of pairs of values in the form @f$(x,y)@f$
         */
        static void sort(std::vector<Pair>& pairs);

        /**
         Ascendantly sorts the pairs of values in this Discrete term by the
         @f$x@f$-coordinate
         */
        virtual void sort();

        virtual Complexity complexity() const FL_IOVERRIDE;
        /**
          Computes the membership function evaluated at @f$x@f$ by using binary
          search to find the lower and upper bounds of @f$x@f$ and then linearly
          interpolating the membership function between the bounds.
          @param x
          @return @f$ \dfrac{h (y_{\max} - y_{\min})}{(x_{\max}- x_{\min})}  (x - x_{\min}) + y_{\min}@f$

          where @f$h@f$ is the height of the Term,
                @f$x_{\min}@f$ and @f$x_{\max}@f$is are the lower and upper limits
                     of @f$x@f$ in `xy` (respectively),
                @f$y_{\min}@f$ and @f$y_{\max}@f$is are the membership functions
                     of @f$\mu(x_{\min})@f$ and @f$\mu(x_{\max})@f$ (respectively)
         */
        virtual scalar membership(scalar x) const FL_IOVERRIDE;

        /**
          Sets the vector of pairs defining the discrete membership function
          @param pairs is the vector of pairs defining the discrete membership function
         */
        virtual void setXY(const std::vector<Pair>& pairs);
        /**
          Gets an immutable vector of pairs defining the discrete membership function
          @return an immutable vector of pairs defining the discrete membership function
         */
        virtual const std::vector<Pair>& xy() const;
        /**
          Gets a mutable vector of pairs defining the discrete membership function
          @return a mutable vector of pairs defining the discrete membership function
         */
        virtual std::vector<Pair>& xy();
        /**
          Gets the immutable pair @f$(x_i,y_i)@f$ at index @f$i@f$
          @param index is the index @f$i@f$
          @return the immutable pair @f$(x_i,y_i)@f$
         */
        virtual const Pair& xy(std::size_t index) const;
        /**
          Gets the mutable pair @f$(x_i,y_i)@f$ at index @f$i@f$
          @param index is the index @f$i@f$
          @return a mutable pair @f$(x_i,y_i)@f$
         */
        virtual Pair& xy(std::size_t index);

        /**
          Creates, fills and returns a vector containing the @f$x@f$ values
          @return a vector containing the @f$x@f$ values
         */
        virtual std::vector<scalar> x() const;

        /**
          Gets the @f$x@f$ value at the given index
          @return the @f$x@f$ value at the given index
         */
        virtual scalar x(std::size_t index) const;

        /**
          Gets the reference to the @f$x@f$ value at the given index
          @return the reference to the @f$x@f$ value at the given index
         */
        virtual scalar& x(std::size_t index);

        /**
          Creates, fills and returns a vector containing the @f$y@f$ values
          @return a vector containing the @f$y@f$ values
         */
        virtual std::vector<scalar> y() const;

        /**
          Gets the @f$y@f$ value at the given index
          @param index is the index
          @return the @f$y@f$ value at the given index
         */
        virtual scalar y(std::size_t index) const;

        /**
          Gets the reference to the @f$y@f$ value at the given index
          @param index is the index
          @return the reference to the @f$y@f$ value at the given index
         */
        virtual scalar& y(std::size_t index);
        /**
          Creates a vector of fl::scalar from a vector of Pair given in the
          form @f$\left(\{x_1,y_1\},...,\{x_n,y_n\}\right)@f$
          @param xy is the vector of Pair
          @return a vector of fl::scalar as @f$(x_1,y_1,...,x_n,y_n)@f$
         */
        static std::vector<scalar> toVector(const std::vector<Pair>& xy);
        /**
          Creates a vector of Pair from a vector of fl::scalar given in the
          form @f$(x_1,y_1,...,x_n,y_n)@f$
          @param xy is a vector of fl::scalar given as
          @f$(x_1,y_1,...,x_n,y_n)@f$
          @return a vector of Pair in the form
          @f$\left(\{x_1,y_1\},...,\{x_n,y_n\}\right)@f$
          @throws fl::Exception if a value is missing, that is, if the length
          of @f$xy@f$ is odd: @f$|xy|\mod 2 = 1@f$
         */
        static std::vector<Pair> toPairs(const std::vector<scalar>& xy);
        /**
          Creates a vector of Pair from a vector of fl::scalar given in the
          form @f$(x_1,y_1,...,x_n,y_n)@f$
          @param xy is a vector of fl::scalar given as
          @f$(x_1,y_1,...,x_n,y_n)@f$ possibly missing a value
          @param missingValue is the replacement in the case a value is missing
          from @f$xy@f$
          @return a vector of Pair in the form
          @f$\left(\{x_1,y_1\},...,\{x_n,y_n\}\right)@f$
         */
        static std::vector<Pair> toPairs(const std::vector<scalar>& xy,
                scalar missingValue) FL_INOEXCEPT;

        /**
          Formats a vector of Pair into a std::string in the form
          @f$(x_1,y_1) ... (x_n,y_n)@f$
          @param xy is the vector of Pair
          @param prefix indicates the prefix of a Pair, e.g., `(` results in
          @f$(x_i@f$
          @param innerSeparator indicates the separator between
          @f$x@f$ and @f$y@f$, e.g., `,` results in @f$x_i,y_i@f$
          @param suffix indicates the postfix of a Pair, e.g., `]` results in
          @f$y_i]@f$
          @param outerSeparator indicates the separator between Pair, e.g.,
          `;` results in @f$(x_i,y_i);(x_j,y_j)@f$
          @return a formatted string containing the pairs of @f$(x,y)@f$ values
         */
        static std::string formatXY(const std::vector<Pair>& xy,
                const std::string& prefix = "(", const std::string& innerSeparator = ",",
                const std::string& suffix = ")", const std::string& outerSeparator = " ");

        /**
          Discretizes the given term
          @param term is the term to discretize
          @param start is the value from which discretization starts
          @param end is the value at which discretization ends
          @param resolution is the number of equally-distributed samples to
          perform between start and end
          @param boundedMembershipFunction indicates whether to ensure that
          @f$\mu(x)\in[0.0,1.0]@f$
          @return a Discrete term that approximates the given term
         */
        static Discrete* discretize(const Term* term, scalar start, scalar end,
                int resolution = IntegralDefuzzifier::defaultResolution(),
                bool boundedMembershipFunction = true);

        virtual Discrete* clone() const FL_IOVERRIDE;

        static Term* constructor();

        /**
          Creates a Discrete term from a variadic set of values.
          Beware: this method is unsafe and must be used with care by
          ensuring:
          - the value `argc` correctly and exactly determines the number of
          values passed,
          - the data type of each variadic arguments is the same, e.g.,
          @f$(1.0, 2.0, 3.0)@f$ are all fl::scalar, whereas in
          @f$(1.0, 2, 3.0)@f$ the second term is an integer, which will cause
          memory access issues due to the difference in size between
          `int` and `fl::scalar`.
          @param name is the name of the resulting term
          @param argc is the number of values passed
          @param x1 is the @f$x@f$ value of the first Pair
          @param y1 is the @f$y@f$ value of the first Pair
          @param ... are the remaining pairs of values @f$x_i@f$ and @f$y_i@f$
          @return a new Discrete term with the given parameters
         */
        template <typename T>
        static Discrete* create(const std::string& name, int argc,
                T x1, T y1, ...);
    };
}

/**
  Template implementation
 */

namespace fl {

    template <typename T>
    inline Discrete* Discrete::create(const std::string& name, int argc,
            T x1, T y1, ...) {
        std::vector<scalar> xy(argc);
        xy.at(0) = x1;
        xy.at(1) = y1;
        va_list args;
        va_start(args, y1);
        for (int i = 2; i < argc; ++i) {
            xy.at(i) = (scalar) va_arg(args, T);
        }
        va_end(args);

        FL_unique_ptr<Discrete> result(new Discrete(name));
        if (xy.size() % 2 != 0) {
            result->setHeight(xy.back());
            xy.pop_back();
        }
        result->setXY(toPairs(xy));
        return result.release();
    }
}
#endif /* FL_DISCRETE_H */
