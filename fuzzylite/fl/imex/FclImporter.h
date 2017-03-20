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

#ifndef FL_FCLIMPORTER_H
#define FL_FCLIMPORTER_H

#include "fl/imex/Importer.h"

#include <string>
#include <utility>
#include <vector>


namespace fl {
    class Norm;
    class TNorm;
    class SNorm;
    class Term;
    class Defuzzifier;

    /**
      The FclImporter class is an Importer that configures an Engine and its
      components utilizing the Fuzzy Control Language specification.

      @author Juan Rada-Vilela, Ph.D.
      @see FclExporter
      @see Importer
      @since 4.0
     */
    class FL_API FclImporter : public Importer {
    public:
        FclImporter();
        virtual ~FclImporter() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(FclImporter)

        virtual std::string name() const FL_IOVERRIDE;

        virtual Engine* fromString(const std::string& fcl) const FL_IOVERRIDE;

        virtual FclImporter* clone() const FL_IOVERRIDE;

    protected:
        virtual void processBlock(const std::string& tag, const std::string& block, Engine* engine) const;
        virtual void processVar(const std::string& var, const std::string& block, Engine* engine)const;
        virtual void processFuzzify(const std::string& block, Engine* engine)const;
        virtual void processDefuzzify(const std::string& block, Engine* engine)const;
        virtual void processRuleBlock(const std::string& block, Engine* engine)const;

        virtual TNorm* parseTNorm(const std::string& line) const;
        virtual SNorm* parseSNorm(const std::string& line) const;
        virtual Term* parseTerm(const std::string& line, const Engine* engine) const;

        virtual Defuzzifier* parseDefuzzifier(const std::string& line) const;
        virtual std::pair<scalar, bool> parseDefaultValue(const std::string& line) const;
        virtual std::pair<scalar, scalar> parseRange(const std::string& line) const;
        virtual std::pair<bool, bool> parseLocks(const std::string& line) const;
        virtual bool parseEnabled(const std::string& line) const;

    };
}
#endif /* FL_FCLIMPORTER_H */
