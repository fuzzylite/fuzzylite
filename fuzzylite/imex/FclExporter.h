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

#ifndef FL_FCLEXPORTER_H
#define FL_FCLEXPORTER_H

#include "fl/imex/Exporter.h"

namespace fl {
    class InputVariable;
    class OutputVariable;
    class RuleBlock;
    class Norm;
    class TNorm;
    class SNorm;
    class Defuzzifier;
    class Term;

    /**
      The FclExporter class is an Exporter that translates an Engine and its
      components to the Fuzzy Control Language specification.

      @author Juan Rada-Vilela, Ph.D.
      @see FclImporter
      @see Exporter
      @since 4.0
     */
    class FL_API FclExporter : public Exporter {
    private:
        std::string _indent;

    public:
        explicit FclExporter(const std::string& indent = "  ");
        virtual ~FclExporter() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(FclExporter)

        /**
          Sets the indentation string within blocks
          @param indent is the indentation string within blocks
         */
        virtual void setIndent(const std::string& indent);
        /**
          Gets the indentation string within blocks
          @return the indentation string within blocks
         */
        virtual std::string getIndent() const;

        virtual std::string name() const FL_IOVERRIDE;
        virtual std::string toString(const Engine* engine) const FL_IOVERRIDE;

        /**
          Returns a string representation of the InputVariable according to the Fuzzy Control Language specification
          @param variable is the input variable
          @return a string representation of the input variable  according to the Fuzzy Control Language specification
         */
        virtual std::string toString(const InputVariable* variable) const;
        /**
          Returns a string representation of the OutputVariable according to the Fuzzy Control Language specification
          @param variable is the output variable
          @return a string representation of the output variable according to the Fuzzy Control Language specification
         */
        virtual std::string toString(const OutputVariable* variable) const;
        /**
          Returns a string representation of the RuleBlock according to the Fuzzy Control Language specification
          @param ruleBlock is the rule block
          @return a string representation of the rule block according to the Fuzzy Control Language specification
         */
        virtual std::string toString(const RuleBlock* ruleBlock) const;
        /**
          Returns a string representation of the Norm according to the Fuzzy Control Language specification
          @param norm is the norm
          @return a string representation of the norm according to the Fuzzy Control Language specification
         */
        virtual std::string toString(const Norm* norm) const;
        /**
          Returns a string representation of the Defuzzifier according to the Fuzzy Control Language specification
          @param defuzzifier is the defuzzifier
          @return a string representation of the defuzzifier according to the Fuzzy Control Language specification
         */
        virtual std::string toString(const Defuzzifier* defuzzifier) const;
        /**
          Returns a string representation of the Term according to the Fuzzy Control Language specification
          @param term is the term
          @return a string representation of the term according to the Fuzzy Control Language specification
         */
        virtual std::string toString(const Term* term) const;

        virtual FclExporter* clone() const FL_IOVERRIDE;
    };
}

#endif /* FL_FCLEXPORTER_H */
