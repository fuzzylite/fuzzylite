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

#ifndef FL_TNORMFUNCTION_H
#define FL_TNORMFUNCTION_H

#include "fl/norm/TNorm.h"

#include "fl/term/Function.h"

namespace fl {

    /**
     The TNormFunction class is a customizable TNorm via Function, which
     computes any function based on the @f$a@f$ and @f$b@f$ values.
     This TNorm is not registered with the TNormFactory.

     @author Juan Rada-Vilela, Ph.D.
     @see Function
     @see TNorm
     @see Norm
     @see TNormFactory
     @since 6.0
     */

    class FL_API TNormFunction FL_IFINAL : public TNorm {
    private:
        Function _function;
    public:
        explicit TNormFunction(const std::string& formula = "");

        /**
          Returns the reference to the Function
          @return the reference to the Function
         */
        Function& function();

        /**
          Loads the function with the given formula
          @param formula is a valid formula in infix notation
         */
        void setFormula(const std::string& formula);
        /**
          Returns the formula loaded into the function
          @return the formula loaded into the function
         */
        std::string getFormula() const;

        std::string className() const FL_IOVERRIDE;

        Complexity complexity() const FL_IOVERRIDE;
        /**
          Computes the S-Norm utilizing the given function via
          SNormFunction::setFormula(), which automatically assigns the values
          of @f$a@f$ and @f$b@f$.

          @param a is a membership function value
          @param b is a membership function value
          @return the evaluation of the function
         */
        scalar compute(scalar a, scalar b) const FL_IOVERRIDE;
        TNormFunction* clone() const FL_IOVERRIDE;

        static TNorm* constructor();
    };
}
#endif /* FL_TNORMFUNCTION_H */

