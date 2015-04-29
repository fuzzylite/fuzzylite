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

    class FL_API JavaExporter : public Exporter {
    public:
        JavaExporter();
        virtual ~JavaExporter() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(JavaExporter)

        virtual std::string name() const FL_IOVERRIDE;

        virtual std::string toString(const Engine* engine) const FL_IOVERRIDE;
        virtual std::string toString(const InputVariable* inputVariable, const Engine* engine) const;
        virtual std::string toString(const OutputVariable* outputVariable, const Engine* engine) const;
        virtual std::string toString(const RuleBlock* ruleBlock, const Engine* engine) const;
        virtual std::string toString(const Term* term) const;
        virtual std::string toString(const Defuzzifier* defuzzifier) const;
        virtual std::string toString(const Norm* norm) const;
        
        virtual std::string toString(scalar value) const;

        virtual JavaExporter* clone() const FL_IOVERRIDE;

    };

}

#endif  /* FL_JAVAEXPORTER_H */

