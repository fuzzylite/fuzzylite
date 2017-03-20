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

#ifndef FL_LINEAR_H
#define FL_LINEAR_H

#include "fl/term/Term.h"

namespace fl {
    class Engine;

    /**
      The Linear class is a linear polynomial Term expressed as @f$f(x)=
      \mathbf{c}\mathbf{v}+k = \sum_i c_iv_i + k@f$, where variable @f$x@f$ is
      not utilized, @f$\mathbf{v}@f$ is a vector of values from the input
      variables, @f$\mathbf{c}@f$ is a vector of coefficients, and @f$k@f$ is a
      constant. Hereinafter, the vector @f$\mathbf{c}^\star=\{c_1, \ldots, c_i,
      \ldots, c_n, k\}@f$ refers to a vector containing the coefficients of
      @f$\mathbf{c}@f$ and the constant @f$k@f$.

      @author Juan Rada-Vilela, Ph.D.
      @see Term
      @see Variable
      @since 4.0
     */
    class FL_API Linear : public Term {
    private:
        /**Contains the coefficients @f$c_i@f$ and the constant @f$k@f$*/
        std::vector<scalar> _coefficients;
        const Engine* _engine;
    public:
        explicit Linear(const std::string& name = "",
                const std::vector<scalar>& coefficients = std::vector<scalar>(),
                const Engine* engine = fl::null);
        virtual ~Linear() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(Linear)

        virtual std::string className() const FL_IOVERRIDE;

        /**
          Returns the vector @f$\mathbf{c}^\star@f$
          @return `"c1 ... ci ... cn k"`
         */
        virtual std::string parameters() const FL_IOVERRIDE;
        /**
          Configures the term with the values of @f$\mathbf{c}^\star@f$
          @param parameters as `"c1 ... ci ... cn k"`
         */
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        virtual Complexity complexity() const FL_IOVERRIDE;

        /**
          Computes the linear function @f$f(x)=\sum_i c_iv_i +k@f$,
          where @f$v_i@f$ is the value of the input variable @f$i@f$ registered
          in the Linear::getEngine()
          @param x is not utilized
          @return @f$\sum_i c_ix_i +k@f$
         */
        virtual scalar membership(scalar x) const FL_IOVERRIDE;

        /**
          Sets the vector @f$\mathbf{c}^\star@f$ and the Engine from which
          vector @f$\mathbf{v}@f$ will be retrieved when necessary
          @param coefficients is the vector @f$\mathbf{c}^\star@f$
          @param engine is the engine from which @f$\mathbf{v}@f$ will be
          retrieved when necessary
         */
        virtual void set(const std::vector<scalar>& coefficients, const Engine* engine);

        /**
          Sets the vector @f$\mathbf{c}^\star@f$ of the linear function
          @param coefficients is the vector @f$\mathbf{c}^\star@f$
         */
        virtual void setCoefficients(const std::vector<scalar>& coefficients);
        /**
          Gets the immutable vector @f$\mathbf{c}^\star@f$
          @return the immutable vector @f$\mathbf{c}^\star@f$
         */
        virtual const std::vector<scalar>& coefficients() const;
        /**
          Gets the mutable vector @f$\mathbf{c}^\star@f$
          @return the mutable vector @f$\mathbf{c}^\star@f$
         */
        virtual std::vector<scalar>& coefficients();

        /**
          Sets the engine from which the vector @f$\mathbf{v}@f$ will be
          obtained upon computing the Linear::membership()
          @param engine is the engine from which the vector @f$\mathbf{v}@f$
          will be obtained
         */
        virtual void setEngine(const Engine* engine);
        /**
          Gets the engine from which the vector @f$\mathbf{v}@f$ will be
          obtained upon computing the Linear::membership()
          @return the engine from which the vector @f$\mathbf{v}@f$ will be
          obtained
         */
        virtual const Engine* getEngine() const;

        virtual Linear* clone() const FL_IOVERRIDE;

        virtual void updateReference(const Engine* engine) FL_IOVERRIDE;

        static Term* constructor();

        /**
          Creates a Linear term from a variadic set of coefficients.
          Beware: this method is unsafe and must be used with care by
          ensuring:

          - the data type of each variadic arguments is the same, e.g.,
            @f$(1.0, 2.0, 3.0)@f$ are all fl::scalar%s. You *need* to avoid the
            case of @f$(1.0, 2, 3.0)@f$, where the second term will be
            considered as an `int` (different from the others) and cause memory
            issues due to the difference in size between an `int` and
            `fl::scalar`; and

          - the number of variadic arguments is exactly the same as the number
            of input variables in the engine plus one in order to match the
            size of the vector @f$\mathbf{c}^\star@f$

          @param name is the name of the term
          @param engine is the engine from which the vector @f$\mathbf{v}@f$
          will be obtained
          @param firstCoefficient is the coefficient for the first input
          variable, namely @f$c_1@f$
          @param ... is a variadic number of coefficients whose type need to be
          the same as the first coefficient
          @return a new Linear term with the given parameters
         */
        template <typename T>
        static Linear* create(const std::string& name, const Engine* engine,
                T firstCoefficient, ...);
    };
}

/**
  Template implementation
 */

#include "fl/Engine.h"

namespace fl {

    template <typename T>
    inline Linear* Linear::create(const std::string& name,
            const Engine* engine, T firstCoefficient, ...) {
        if (not engine) throw Exception("[linear error] cannot create term <" + name + "> "
                "without a reference to the engine", FL_AT);
        std::vector<scalar> coefficients;
        coefficients.push_back((scalar) firstCoefficient);

        va_list args;
        va_start(args, firstCoefficient);
        for (std::size_t i = 0; i < engine->inputVariables().size(); ++i) {
            coefficients.push_back((scalar) va_arg(args, T));
        }
        va_end(args);

        return new Linear(name, coefficients, engine);
    }
}
#endif  /* FL_LINEAR_H */

