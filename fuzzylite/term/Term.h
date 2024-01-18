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

#ifndef FL_TERM_H
#define FL_TERM_H

#include "fl/fuzzylite.h"

#include "fl/Operation.h"
#include "fl/Complexity.h"

#include <cmath>
#include <string>
#include <vector>

namespace fl {
    class Engine;

    /**
      The Term class is the abstract class for linguistic terms. The linguistic
      terms in this library can be divided in four groups as: `basic`,
      `extended`, `edge`, and `function`. The `basic` terms are Triangle,
      Trapezoid, Rectangle, and Discrete. The `extended` terms are Bell,
      Binary, Cosine, Gaussian, GaussianProduct, PiShape, SigmoidDifference,
      SigmoidProduct, and Spike. The `edge` terms are Concave, Ramp, Sigmoid,
      SShape, and ZShape. The `function` terms are Constant, Linear, and
      Function.

      In the figure below, the `basic` terms are represented in the first
      column, and the `extended` terms in the second and third columns. The
      `edge` terms are represented in the fifth and sixth rows, and the
      `function` terms in the last row.

      @image html terms.svg

      @author Juan Rada-Vilela, Ph.D.
      @see Variable
      @see InputVariable
      @see OutputVariable
      @since 4.0
     */
    class FL_API Term {
    private:
        std::string _name;
    protected:
        scalar _height;
    public:

        explicit Term(const std::string& name = "", scalar height = 1.0);
        virtual ~Term();
        FL_DEFAULT_COPY_AND_MOVE(Term)

        /**
          Sets the name of the term
          @param name is the name of term
         */
        virtual void setName(const std::string& name);
        /**
          Gets the name of the term
          @return the name of the term
         */
        virtual std::string getName() const;

        /**
          Sets the height of the term
          @param height is the height of the term
         */
        virtual void setHeight(scalar height);
        /**
          Gets the height of the term
          @return the height of the term
         */
        virtual scalar getHeight() const;

        /**
          Returns the representation of the term in the FuzzyLite Language
          @return the representation of the term in FuzzyLite Language
          @see FllExporter
         */
        virtual std::string toString() const;

        /**
          Returns the name of the class of the term
          @return the name of the class of the term
         */
        virtual std::string className() const = 0;
        /**
          Returns the parameters to configure the term. The parameters are
          separated by spaces. If there is one additional parameter, the
          parameter will be considered as the height of the term; otherwise,
          the height will be set to @f$1.0@f$
          @return the parameters to configure the term (@see Term::configure())
         */
        virtual std::string parameters() const = 0;
        /**
          Configures the term with the given parameters. The parameters are
          separated by spaces. If there is one additional parameter, the
          parameter will be considered as the height of the term; otherwise,
          the height will be set to @f$1.0@f$
          @param parameters is the parameters to configure the term
         */
        virtual void configure(const std::string& parameters) = 0;

        /**
         Computes the estimated complexity of evaluating the membership function
         @return the estimated complexity of evaluating the membership function
         */
        virtual Complexity complexity() const = 0;

        /**
          Computes the membership function value at @f$x@f$
          @param x
          @return the membership function value @f$\mu(x)@f$
         */
        virtual scalar membership(scalar x) const = 0;

        /**
          Creates a clone of the term
          @return a clone of the term
         */
        virtual Term* clone() const = 0;

        /**
          Updates the references (if any) to point to the current engine (useful
          when cloning engines or creating terms within Importer objects
          @param engine is the engine to which this term belongs to
         */
        virtual void updateReference(const Engine* engine);

        /**
          For monotonic terms, computes the tsukamoto value of the term for the
          given activation degree @f$\alpha@f$, that is,
          @f$ g_j(\alpha) = \{ z \in\mathbb{R} : \mu_j(z) = \alpha \} $@f. If
          the term is not monotonic (or does not override this method) the
          method computes the membership function @f$\mu(\alpha)@f$.
          @param activationDegree is the activationDegree
          @param minimum is the minimum value of the range of the term
          @param maximum is the maximum value of the range of the term
          @return the tsukamoto value of the term for the given activation degree
                  if the term is monotonic (or overrides this method), or
                  the membership function for the activation degree otherwise.
         */
        virtual scalar tsukamoto(scalar activationDegree, scalar minimum, scalar maximum) const;

        /**
          Indicates whether the term is monotonic.
          @return whether the term is monotonic.
         */
        virtual bool isMonotonic() const;
    };
}
#endif /* FL_TERM_H */
