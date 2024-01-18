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

#ifndef FL_SNORMFUNCTION_H
#define FL_SNORMFUNCTION_H

#include "fl/norm/SNorm.h"

#include "fl/term/Function.h"

namespace fl {

    /**
     The SNormFunction class is a customizable SNorm via Function, which
     computes any function based on the @f$a@f$ and @f$b@f$ values.
     This SNorm is not registered with the SNormFactory.

     @author Juan Rada-Vilela, Ph.D.
     @see Function
     @see SNorm
     @see Norm
     @see SNormFactory
     @since 6.0
     */

    class FL_API SNormFunction FL_IFINAL : public SNorm {
    private:
        Function _function;
    public:
        explicit SNormFunction(const std::string& formula = "");

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
        SNormFunction* clone() const FL_IOVERRIDE;

        static SNorm* constructor();
    };
}
#endif /* FL_SNORMFUNCTION_H */

