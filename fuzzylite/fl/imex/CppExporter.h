// #BEGIN_LICENSE
// fuzzylite: a fuzzy logic control library in C++
// Copyright (C) 2014  Juan Rada-Vilela
// 
// This file is part of fuzzylite.
//
// fuzzylite is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// fuzzylite is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with fuzzylite.  If not, see <http://www.gnu.org/licenses/>.
// #END_LICENSE

/* 
 * File:   CppExporter.h
 * Author: jcrada
 *
 * Created on 1 January 2013, 2:41 PM
 */

#ifndef FL_CPPEXPORTER_H
#define	FL_CPPEXPORTER_H

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

    class FL_EXPORT CppExporter : public Exporter {
    protected:
        bool _prefixNamespace;
        virtual std::string fl(const std::string& clazz) const;
    public:
        CppExporter(bool prefixNamespace = false);
        virtual ~CppExporter();
        virtual std::string name() const;
        virtual std::string toString(const Engine* engine) const;

        virtual std::string toString(const InputVariable* inputVariable, const Engine* engine) const;
        virtual std::string toString(const OutputVariable* outputVariable, const Engine* engine) const;
        virtual std::string toString(const RuleBlock* ruleBlock, const Engine* engine) const;
        virtual std::string toString(scalar value) const;

        virtual std::string toString(const Hedge* hedge) const;
        virtual std::string toString(const Term* term) const;
        virtual std::string toString(const Norm* op) const;
        virtual std::string toString(const Defuzzifier* defuzzifier) const;

        virtual CppExporter* clone() const;

    };
}
#endif	/* FL_CPPEXPORTER_H */

