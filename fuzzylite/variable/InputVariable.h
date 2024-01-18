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

#ifndef FL_INPUTVARIABLE_H
#define FL_INPUTVARIABLE_H

#include "fl/variable/Variable.h"

namespace fl {

    /**
      The InputVariable class is a Variable that represents an input of the
      fuzzy logic controller.

      @author Juan Rada-Vilela, Ph.D.
      @see Variable
      @see OutputVariable
      @see Term
      @since 4.0
     */
    class FL_API InputVariable : public Variable {
    public:
        explicit InputVariable(const std::string& name = "",
                scalar minimum = -fl::inf,
                scalar maximum = fl::inf);
        virtual ~InputVariable() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(InputVariable)

        /**
          Evaluates the membership function of the current input value @f$x@f$
          for each term @f$i@f$, resulting in a fuzzy input value in the form
          @f$\tilde{x}=\sum_i{\mu_i(x)/i}@f$. This is equivalent to a call to
          Variable::fuzzify() passing @f$x@f$ as input value

          @return the fuzzy input value expressed as @f$\sum_i{\mu_i(x)/i}@f$
         */
        virtual std::string fuzzyInputValue() const;

        virtual Variable::Type type() const FL_IOVERRIDE;

        virtual std::string toString() const FL_IOVERRIDE;

        virtual InputVariable* clone() const FL_IOVERRIDE;

    };
}
#endif /* FL_INPUTVARIABLE_H */
