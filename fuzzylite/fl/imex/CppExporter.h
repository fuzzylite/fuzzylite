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

#ifndef FL_CPPEXPORTER_H
#define FL_CPPEXPORTER_H

#include "fl/imex/Exporter.h"

namespace fl {
    class Engine;
    class InputVariable;
    class OutputVariable;
    class Term;
    class RuleBlock;
    class Norm;
    class Defuzzifier;
    class Hedge;
    class Activation;

    /**
      The CppExporter class is an Exporter that translates an Engine and its
      components to the `C++` programming language using the `fuzzylite`
      library.

      @author Juan Rada-Vilela, Ph.D.
      @see JavaExporter
      @see Exporter
      @since 4.0
     */
    class FL_API CppExporter : public Exporter {
    private:
        bool _usingNamespace;
        bool _usingVariableNames;
    public:
        explicit CppExporter(bool usingNamespace = false, bool usingVariableNames = true);
        virtual ~CppExporter() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(CppExporter)

        virtual std::string name() const FL_IOVERRIDE;
        virtual std::string toString(const Engine* engine) const FL_IOVERRIDE;

        /**
          Sets whether the fl namespace of the library is prepended to types
          (e.g., fl::Engine)

          @param usingNamespace whether the fl namespace of the library is
          prepended to types (e.g., fl::Engine)
         */
        virtual void setUsingNamespace(bool usingNamespace);
        /**
          Gets whether the fl namespace of the library is prepended to types
          (e.g., fl::Engine)
          @return whether the fl namespace of the library is prepended to types
         */
        virtual bool isUsingNamespace() const;

        /**
          Returns the given text prepended with the `fl` namespace if
          CppExporter::isUsingNamespace is `true`, or the text otherwise

          @param clazz is the text to be prepended the `fl::`.
          @return the given text prepended with the `fl` namespace if
          CppExporter::isUsingNamespace is `true`, or the text otherwise
         */
        virtual std::string fl(const std::string& clazz) const;
        /**
         Sets whether variables are exported using their names
         (e.g., `power->setValue(fl::nan)`) instead of numbered identifiers
         (e.g., `inputVariable1->setValue(fl::nan)`)
         @param usingVariableNames indicates whether variables are exported using
         their names
         */
        virtual void setUsingVariableNames(bool usingVariableNames);
        /**
         Gets whether variables are exported using their names
         (e.g., `power->setValue(fl::nan)`) instead of numbered identifiers
         (e.g., `inputVariable1->setValue(fl::nan)`)
         @return whether variables are exported using their names
         */
        virtual bool isUsingVariableNames() const;

        /**
          Returns a string representation of InputVariable in the `C++` programming language
          @param inputVariable is the input variable
          @param engine is the engine in which the input variable is registered
          @return a string representation of the input variable in the `C++` programming language
         */
        virtual std::string toString(const InputVariable* inputVariable, const Engine* engine) const;
        /**
          Returns a string representation of the OutputVariable in the `C++` programming language
          @param outputVariable is the output variable
          @param engine is the engine in which the output variable is registered
          @return a string representation of the output variable in the `C++` programming language
         */
        virtual std::string toString(const OutputVariable* outputVariable, const Engine* engine) const;
        /**
          Returns a string representation of the RuleBlock in the `C++` programming language
          @param ruleBlock is the rule block
          @param engine is the engine in which the rule block is registered
          @return a string representation of the rule block in the `C++` programming language
         */
        virtual std::string toString(const RuleBlock* ruleBlock, const Engine* engine) const;

        /**
          Returns a string representation of the Activation method in the `C++` programming language
          @param activation is the activation method
          @return a string representation of the activation method in the `C++` programming language
         */
        virtual std::string toString(const Activation* activation) const;

        /**
          Returns a string representation of the scalar value in the `C++` programming language
          @param value is the scalar value
          @return a string representation of the scalar value in the `C++` programming language
         */
        virtual std::string toString(scalar value) const;

        /**
          Returns a string representation of the Hedge in the `C++` programming language
          @param hedge is the hedge
          @return a string representation of the hedge in the `C++` programming language
         */
        virtual std::string toString(const Hedge* hedge) const;
        /**
          Returns a string representation of the Term in the `C++` programming language
          @param term is the term
          @return a string representation of the term in the `C++` programming language
         */
        virtual std::string toString(const Term* term) const;
        /**
          Returns a string representation of the Norm in the `C++` programming language
          @param norm is the norm
          @return a string representation of the norm in the `C++` programming language
         */
        virtual std::string toString(const Norm* norm) const;
        /**
          Returns a string representation of the Defuzzifier in the `C++` programming language
          @param defuzzifier is the defuzzifier
          @return a string representation of the defuzzifier in the `C++` programming language
         */
        virtual std::string toString(const Defuzzifier* defuzzifier) const;

        virtual CppExporter* clone() const FL_IOVERRIDE;

    };
}
#endif  /* FL_CPPEXPORTER_H */

