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

#ifndef FL_FISEXPORTER_H
#define FL_FISEXPORTER_H

#include "fl/imex/Exporter.h"

#include <vector>

namespace fl {
    class Norm;
    class TNorm;
    class SNorm;
    class Defuzzifier;
    class Term;
    class Rule;
    class Proposition;
    class Variable;

    /**
      The FisExporter class is an Exporter that translates an Engine and its
      components into the Fuzzy Inference System format for Matlab or Octave.

      @author Juan Rada-Vilela, Ph.D.
      @see FisImporter
      @see Exporter
      @since 4.0
     */
    class FL_API FisExporter : public Exporter {
    protected:

        virtual std::string translate(const std::vector<Proposition*>& propositions,
                const std::vector<Variable*> variables) const;

    public:
        FisExporter();
        virtual ~FisExporter() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(FisExporter)

        virtual std::string name() const FL_IOVERRIDE;
        virtual std::string toString(const Engine* engine) const FL_IOVERRIDE;

        /**
         Returns a string representation of the TNorm in the Fuzzy Inference System format
         @param tnorm is the TNorm
         @return a string representation of the TNorm in the Fuzzy Inference System format
         */
        virtual std::string toString(const TNorm* tnorm) const;

        /**
         Returns a string representation of the SNorm in the Fuzzy Inference System format
         @param snorm is the SNorm
         @return a string representation of the SNorm in the Fuzzy Inference System format
         */
        virtual std::string toString(const SNorm* snorm) const;

        /**
         Returns a string representation of the Defuzzifier in the Fuzzy Inference System format
         @param defuzzifier is the defuzzifier
         @return a string representation of the Defuzzifier in the Fuzzy Inference System format
         */
        virtual std::string toString(const Defuzzifier* defuzzifier) const;
        /**
         Returns a string representation of the Term in the Fuzzy Inference System format
         @param term is the term
         @return a string representation of the term in the Fuzzy Inference System format
         */
        virtual std::string toString(const Term* term) const;

        /**
         Returns a string representation of the `[System]` configuration
         @param engine is the engine
         @return a string representation of the `[System]` configuration
         */
        virtual std::string exportSystem(const Engine* engine) const;
        /**
          Returns a string representation of the `[Input]` configuration
          @param engine is the engine
          @return a string representation of the `[Input]` configuration
         */
        virtual std::string exportInputs(const Engine* engine) const;
        /**
          Returns a string representation of the `[Output]` configuration
          @param engine is the engine
          @return a string representation of the `[Output]` configuration
         */
        virtual std::string exportOutputs(const Engine* engine) const;
        /**
          Returns a string representation of the `[Rules]` configuration
          @param engine is the engine
          @return a string representation of the `[Rules]` configuration
         */
        virtual std::string exportRules(const Engine* engine) const;
        /**
          Returns a string representation for the Rule in the Fuzzy Inference System format
          @param rule is the rule
          @param engine is the engine in which the rule is registered
          @return a string representation for the rule in the Fuzzy Inference System format
         */
        virtual std::string exportRule(const Rule* rule, const Engine* engine) const;

        virtual FisExporter* clone() const FL_IOVERRIDE;
    };
}

#endif  /* FL_FISEXPORTER_H */

