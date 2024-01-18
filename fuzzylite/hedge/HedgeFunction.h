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

#ifndef FL_HEDGEFUNCTION_H
#define FL_HEDGEFUNCTION_H

#include "fl/hedge/Hedge.h"

#include "fl/term/Function.h"

namespace fl {

    /**
     The HedgeFunction class is a customizable Hedge via Function, which
     computes any function based on the @f$x@f$ value. This hedge is not
     registered with the HedgeFactory due to issues configuring the formula
     within. To register the hedge, a static method with the
     constructor needs to be manually created and registered. Please, check the
     file `test/hedge/HedgeFunction.cpp` for further details.

     @author Juan Rada-Vilela, Ph.D.
     @see Function
     @see Hedge
     @see HedgeFactory
     @since 6.0
     */

    class FL_API HedgeFunction FL_IFINAL : public Hedge {
    private:
        Function _function;
    public:
        explicit HedgeFunction(const std::string& formula = "");

        std::string name() const FL_IOVERRIDE;

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

        Complexity complexity() const FL_IOVERRIDE;

        /**
          Computes the hedge for the membership function value @f$x@f$ utilizing
          the given function via HedgeFunction::setFormula()
          @param x is a membership function value
          @return the evaluation of the function
         */
        scalar hedge(scalar x) const FL_IOVERRIDE;
        HedgeFunction* clone() const FL_IOVERRIDE;

        static Hedge* constructor();
    };
}

#endif /* FL_HEDGEFUNCTION_H */

