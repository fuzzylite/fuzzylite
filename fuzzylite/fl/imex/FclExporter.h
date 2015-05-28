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

    class FL_API FclExporter : public Exporter {
    protected:
        std::string _indent;

    public:
        explicit FclExporter(const std::string& indent = "  ");
        virtual ~FclExporter() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(FclExporter)

        virtual void setIndent(const std::string& indent);
        virtual std::string getIndent() const;

        virtual std::string name() const FL_IOVERRIDE;
        virtual std::string toString(const Engine* engine) const FL_IOVERRIDE;

        virtual std::string toString(const InputVariable* variable) const;
        virtual std::string toString(const OutputVariable* variable) const;
        virtual std::string toString(const RuleBlock* ruleBlock) const;

        virtual std::string toString(const Norm* norm) const;

        virtual std::string toString(const Defuzzifier* defuzzifier) const;
        virtual std::string toString(const Term* term) const;

        virtual FclExporter* clone() const FL_IOVERRIDE;
    };

}
#endif /* FL_FCLEXPORTER_H */
