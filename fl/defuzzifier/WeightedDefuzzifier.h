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

#ifndef FL_WEIGHTEDDEFUZZIFIER_H
#define FL_WEIGHTEDDEFUZZIFIER_H

#include "fl/defuzzifier/Defuzzifier.h"

namespace fl {
    class Activated;

    /**
      The WeightedDefuzzifier class is the base class for defuzzifiers which
      compute a weighted function on the fuzzy set without requiring to
      integrate over the fuzzy set.

      @author Juan Rada-Vilela, Ph.D.
      @since 5.0
     */
    class FL_API WeightedDefuzzifier : public Defuzzifier {
    public:

        /**The Type enum indicates the type of the WeightedDefuzzifier based
        the terms included in the fuzzy set.*/
        enum Type {
            /**Automatic: Automatically inferred from the terms */
            Automatic,
            /**TakagiSugeno: Manually set to TakagiSugeno (or Inverse Tsukamoto)*/
            TakagiSugeno,
            /**Tsukamoto: Manually set to Tsukamoto*/
            Tsukamoto
        };
        /**
          Returns a string representation of the given type
          @param type is the type of a defuzzifier
          @return a string representation of the given type
         */
        static std::string typeName(Type type);
    private:
        Type _type;

    public:
        explicit WeightedDefuzzifier(Type type = Automatic);
        explicit WeightedDefuzzifier(const std::string& type);
        virtual ~WeightedDefuzzifier() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(WeightedDefuzzifier)

        /**
          Sets the type of the weighted defuzzifier
          @param type is the type of the weighted defuzzifier
         */
        void setType(Type type);
        /**
          Gets the type of the weighted defuzzifier
          @return the type of the weighted defuzzifier
         */
        Type getType() const;
        /**
          Returns a string representation of the type of the defuzzifier
          @return a string representation of the type of the defuzzifier
         */
        virtual std::string getTypeName() const;
        /**
          Infers the type of the defuzzifier based on the given term. If the
          given term is Constant, Linear or Function, then the type is
          TakagiSugeno; otherwise, the type is Tsukamoto

          @param term is the given term
          @return the inferred type of the defuzzifier based on the given term
         */
        virtual Type inferType(const Term* term) const;

    };
}

#endif  /* FL_WEIGHTEDDEFUZZIFIER_H */

