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

    class FL_API CppExporter : public Exporter {
    private:
        bool _prefixNamespace;
        virtual std::string fl(const std::string& clazz) const;
    public:
        explicit CppExporter(bool prefixNamespace = false);
        virtual ~CppExporter() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(CppExporter)

        virtual std::string name() const FL_IOVERRIDE;
        virtual std::string toString(const Engine* engine) const FL_IOVERRIDE;

        virtual void setPrefixNamespace(bool prefixNamespace);
        virtual bool isPrefixNamespace() const;

        virtual std::string toString(const InputVariable* inputVariable, const Engine* engine) const;
        virtual std::string toString(const OutputVariable* outputVariable, const Engine* engine) const;
        virtual std::string toString(const RuleBlock* ruleBlock, const Engine* engine) const;
        virtual std::string toString(scalar value) const;

        virtual std::string toString(const Hedge* hedge) const;
        virtual std::string toString(const Term* term) const;
        virtual std::string toString(const Norm* op) const;
        virtual std::string toString(const Defuzzifier* defuzzifier) const;

        virtual CppExporter* clone() const FL_IOVERRIDE;

    };
}
#endif  /* FL_CPPEXPORTER_H */

