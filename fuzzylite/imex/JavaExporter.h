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

#ifndef FL_JAVAEXPORTER_H
#define FL_JAVAEXPORTER_H

#include "fl/imex/Exporter.h"

namespace fl {

    class Engine;
    class InputVariable;
    class OutputVariable;
    class RuleBlock;
    class Term;
    class Defuzzifier;
    class Norm;
    class SNorm;
    class TNorm;
    class Activation;

    /**
      The JavaExporter class is an Exporter that translates an Engine and its
      components to the `Java` programming language using the `jfuzzylite`
      library.

      @author Juan Rada-Vilela, Ph.D.
      @see CppExporter
      @see Exporter
      @since 4.0
     */
    class FL_API JavaExporter : public Exporter {
    private:
        bool _usingVariableNames;
    public:
        explicit JavaExporter(bool usingVariableNames = true);
        virtual ~JavaExporter() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(JavaExporter)

        virtual std::string name() const FL_IOVERRIDE;

        /**
         Sets whether variables are exported using their names
         (e.g., `power.setValue(Double.NaN)`) instead of numbered identifiers
         (e.g., `inputVariable1.setValue(Double.NaN)`)
         @param usingVariableNames indicates whether variables are exported using
         their names
         */
        virtual void setUsingVariableNames(bool usingVariableNames);

        /**
         Gets whether variables are exported using their names
         (e.g., `power.setValue(Double.NaN)`) instead of numbered identifiers
         (e.g., `inputVariable1.setValue(Double.NaN)`)
         @return whether variables are exported using their names
         */
        virtual bool isUsingVariableNames() const;

        virtual std::string toString(const Engine* engine) const FL_IOVERRIDE;
        /**
          Returns a string representation of the InputVariable in the Java
          programming language
          @param inputVariable is the input variable
          @param engine is the engine in which the input variable is registered
          @return a string representation of the input variable in the Java
          programming language
         */
        virtual std::string toString(const InputVariable* inputVariable, const Engine* engine) const;
        /**
          Returns a string representation of the OutputVariable in the Java
          programming language
          @param outputVariable is the output variable
          @param engine is the engine in which the output variable is registered
          @return a string representation of the output variable in the Java
          programming language
         */
        virtual std::string toString(const OutputVariable* outputVariable, const Engine* engine) const;
        /**
          Returns a string representation of the RuleBlock in the Java
          programming language
          @param ruleBlock is the rule block
          @param engine is the engine in which the rule block is registered
          @return a string representation of the rule block in the Java
          programming language
         */
        virtual std::string toString(const RuleBlock* ruleBlock, const Engine* engine) const;

        /**
          Returns a string representation of the Term in the Java programming
          language
          @param term is the term
          @return a string representation of the term in the Java programming
          language
         */
        virtual std::string toString(const Term* term) const;

        /**
          Returns a string representation of the Activation method in the Java
          programming language
          @param activation is the activation method
          @return a string representation of the activation method in the Java
          programming language
         */
        virtual std::string toString(const Activation* activation) const;

        /**
          Returns a string representation of the Defuzzifier in the Java
          programming language
          @param defuzzifier is the defuzzifier
          @return a string representation of the defuzzifier in the Java
          programming language
         */
        virtual std::string toString(const Defuzzifier* defuzzifier) const;

        /**
          Returns a string representation of the Norm in the Java programming
          language
          @param norm is the norm
          @return a string representation of the norm in the Java programming
          language
         */
        virtual std::string toString(const Norm* norm) const;

        /**
          Returns a string representation of the scalar value in the Java
          programming language
          @param value is the scalar value
          @return a string representation of the scalar value in the Java
          programming language
         */
        virtual std::string toString(scalar value) const;

        virtual JavaExporter* clone() const FL_IOVERRIDE;

    };
}

#endif  /* FL_JAVAEXPORTER_H */

