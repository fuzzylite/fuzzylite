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

#ifndef FL_FLLIMPORTER_H
#define FL_FLLIMPORTER_H

#include "fl/imex/Importer.h"

#include <utility>

namespace fl {
    class TNorm;
    class SNorm;
    class Activation;
    class Term;
    class Defuzzifier;

    /**
      The FllImporter class is an Importer that configures an Engine and its
      components utilizing the FuzzyLite Language (FLL), see
      [http://www.fuzzylite.com/fll-fld](http://www.fuzzylite.com/fll-fld) for
      more information.

      @author Juan Rada-Vilela, Ph.D.
      @see FllExporter
      @see Importer
      @since 4.0
      @todo parse methods returning respective instances from blocks of text
     */
    class FL_API FllImporter : public Importer {
    private:
        std::string _separator;
    public:
        explicit FllImporter(const std::string& separator = "\n");
        virtual ~FllImporter() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(FllImporter)

        /**
          Sets the separator of the language (default separator is a new line '\n')
          @param separator is the separator of the language
         */
        virtual void setSeparator(const std::string& separator);
        /**
          Gets the separator of the language (default separator is a new line '\n')
          @return the separator of the language
         */
        virtual std::string getSeparator() const;

        virtual std::string name() const FL_IOVERRIDE;
        virtual Engine* fromString(const std::string& fll) const FL_IOVERRIDE;

        virtual FllImporter* clone() const FL_IOVERRIDE;

    protected:

        virtual void process(const std::string& tag, const std::string& block, Engine* engine) const;
        virtual void processInputVariable(const std::string& block, Engine* engine) const;
        virtual void processOutputVariable(const std::string& block, Engine* engine) const;
        virtual void processRuleBlock(const std::string& block, Engine* engine) const;

        virtual TNorm* parseTNorm(const std::string& name) const;
        virtual SNorm* parseSNorm(const std::string& name) const;
        virtual Activation* parseActivation(const std::string& name) const;

        virtual Term* parseTerm(const std::string& text, Engine* engine) const;

        virtual Defuzzifier* parseDefuzzifier(const std::string& line) const;
        virtual std::pair<scalar, scalar> parseRange(const std::string& line) const;
        virtual bool parseBoolean(const std::string& boolean) const;

        virtual std::pair<std::string, std::string> parseKeyValue(const std::string& text,
                char separator = ':') const;

    };
}

#endif  /* FL_FLLIMPORTER_H */

