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

#ifndef FL_FLLEXPORTER_H
#define FL_FLLEXPORTER_H

#include "fl/imex/Exporter.h"

#include <vector>

namespace fl {
    class Variable;
    class InputVariable;
    class OutputVariable;
    class RuleBlock;
    class Rule;
    class Norm;
    class Activation;
    class Defuzzifier;
    class Term;

    /**
      The FllExporter class is an Exporter that translates an Engine and its
      components to the FuzzyLite Language (FLL), see
      [http://www.fuzzylite.com/fll-fld](http://www.fuzzylite.com/fll-fld) for
      more information.

      @author Juan Rada-Vilela, Ph.D.
      @see FllImporter
      @see Exporter
      @since 4.0
     */
    class FL_API FllExporter : public Exporter {
    private:
        std::string _indent;
        std::string _separator;
    public:
        explicit FllExporter(const std::string& indent = "  ", const std::string& separator = "\n");
        virtual ~FllExporter() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(FllExporter)

        virtual std::string name() const FL_IOVERRIDE;

        /**
          Sets the indent string of the FuzzyLite Language
          @param indent is the indent string of the FuzzyLite Language
         */
        virtual void setIndent(const std::string& indent);
        /**
          Gets the indent string of the FuzzyLite Language
          @return the indent string of the FuzzyLite Language
         */
        virtual std::string getIndent() const;

        /**
          Gets the separator of the FuzzyLite Language
          @param separator of the FuzzyLite Language
         */
        virtual void setSeparator(const std::string& separator);
        /**
          Gets the separator of the FuzzyLite Language
          @return the separator of the FuzzyLite Language
         */
        virtual std::string getSeparator() const;

        virtual std::string toString(const Engine* engine) const FL_IOVERRIDE;

        /**
          Returns a string representation of the vector of variables in the FuzzyLite Language
          @param variables is the vector of variables
          @return a string representation of the vector of variables in the FuzzyLite Language
         */
        virtual std::string toString(const std::vector<Variable*>& variables) const;
        /**
          Returns a string representation of the vector of input variables in the FuzzyLite Language
          @param inputVariables is the vector of input variables
          @return a string representation of the vector of input variables in the FuzzyLite Language
         */
        virtual std::string toString(const std::vector<InputVariable*>& inputVariables) const;
        /**
          Returns a string representation of the vector of output variables in the FuzzyLite Language
          @param outputVariables is a vector of output variables
          @return a string representation of the vector of output variables in the FuzzyLite Language
         */
        virtual std::string toString(const std::vector<OutputVariable*>& outputVariables) const;
        /**
          Returns a string representation of the vector of rule blocks in the FuzzyLite Language
          @param ruleBlocks is the vector of rule blocks
          @return a string representation of the vector of rule blocks in the FuzzyLite Language
         */
        virtual std::string toString(const std::vector<RuleBlock*>& ruleBlocks) const;

        /**
          Returns a string representation of the Variable in the FuzzyLite Language
          @param variable is the variable
          @return a string representation of the variable in the FuzzyLite Language
         */
        virtual std::string toString(const Variable* variable) const;
        /**
          Returns a string representation of the InputVariable in the FuzzyLite Language
          @param inputVariable is the input variable to export
          @return a string representation of the input variable in the FuzzyLite Language
         */
        virtual std::string toString(const InputVariable* inputVariable) const;
        /**
          Returns a string representation of the OutputVariable in the FuzzyLite Language
          @param outputVariable is the output variable
          @return a string representation of the output variable in the FuzzyLite Language
         */
        virtual std::string toString(const OutputVariable* outputVariable) const;

        /**
          Returns a string representation of the RuleBlock in the FuzzyLite Language
          @param ruleBlock is the rule block
          @return a string representation of the rule block in the FuzzyLite Language
         */
        virtual std::string toString(const RuleBlock* ruleBlock) const;
        /**
          Returns a string representation of the Rule in the FuzzyLite Language
          @param rule is the rule
          @return a string representation of the rule in the FuzzyLite Language
         */
        virtual std::string toString(const Rule* rule) const;

        /**
          Returns a string representation of the Norm in the FuzzyLite Language
          @param norm is the norm
          @return a string representation of the norm in the FuzzyLite Language
         */
        virtual std::string toString(const Norm* norm) const;
        /**
          Returns a string representation of the Activation method in the FuzzyLite Language
          @param activation is the activation method
          @return a string representation of the activation method in the FuzzyLite Language
         */
        virtual std::string toString(const Activation* activation) const;
        /**
          Returns a string representation of the Defuzzifier in the FuzzyLite Language
          @param defuzzifier is the defuzzifier
          @return a string representation of the defuzzifier in the FuzzyLite Language
         */
        virtual std::string toString(const Defuzzifier* defuzzifier) const;
        /**
          Returns a string representation of the Term in the FuzzyLite Language
          @param term is the term
          @return a string representation of the term in the FuzzyLite Language
         */
        virtual std::string toString(const Term* term) const;

        virtual FllExporter* clone() const FL_IOVERRIDE;
    };
}

#endif  /* FL_FLLEXPORTER_H */

