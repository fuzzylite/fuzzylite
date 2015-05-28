/*
 Author: Juan Rada-Vilela, Ph.D.
 Copyright © 2010-2015 FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#ifndef FL_FISIMPORTER_H
#define FL_FISIMPORTER_H

#include "fl/imex/Importer.h"

#include <utility>
#include <vector>


namespace fl {
    class Norm;
    class TNorm;
    class SNorm;
    class Term;
    class Defuzzifier;
    class Variable;

    class FL_API FisImporter : public Importer {
    public:
        FisImporter();
        virtual ~FisImporter() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(FisImporter)

        virtual std::string name() const FL_IOVERRIDE;

        virtual Engine* fromString(const std::string& fcl) const FL_IOVERRIDE;

        virtual FisImporter* clone() const FL_IOVERRIDE;

    protected:
        virtual void importSystem(const std::string& section, Engine* engine,
                std::string& andMethod, std::string& orMethod,
                std::string& impMethod, std::string& aggMethod,
                std::string& defuzzMethod) const;
        virtual void importInput(const std::string& section, Engine* engine) const;
        virtual void importOutput(const std::string& section, Engine* engine) const;
        virtual void importRules(const std::string& section, Engine* engine) const;
        virtual std::string translateProposition(scalar code, Variable* variable) const;

        virtual std::string translateTNorm(const std::string& tnorm) const;
        virtual std::string translateSNorm(const std::string& tnorm) const;
        virtual std::string translateDefuzzifier(const std::string& defuzzifier) const;

        virtual Term* parseTerm(const std::string& line, const Engine* engine) const;
        virtual Term* createInstance(const std::string& termClass, const std::string& name,
                const std::vector<std::string>& params, const Engine* engine) const;

        virtual std::pair<scalar, scalar> parseRange(const std::string& range) const;

    };

}
#endif  /* FL_FISIMPORTER_H */

