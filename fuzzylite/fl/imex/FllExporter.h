/*   Copyright 2013 Juan Rada-Vilela

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

/* 
 * File:   FllExporter.h
 * Author: jcrada
 *
 * Created on 18 December 2013, 8:53 AM
 */

#ifndef FL_FLLEXPORTER_H
#define	FL_FLLEXPORTER_H

#include "fl/fuzzylite.h"
#include "fl/imex/Exporter.h"

namespace fl {
    class Variable;
    class InputVariable;
    class OutputVariable;
    class RuleBlock;
    class Rule;
    class Norm;
    class Defuzzifier;
    class Term;

    class FL_EXPORT FllExporter : public Exporter {
    protected:
        std::string _indent;
        std::string _separator;
    public:
        FllExporter(const std::string& indent = "  ", const std::string& separator = "\n");
        virtual ~FllExporter();

        virtual std::string name() const;

        virtual void setIndent(const std::string& indent);
        virtual std::string getIndent() const;

        virtual void setSeparator(const std::string& separator);
        virtual std::string getSeparator() const;

        virtual std::string toString(const Engine* engine) const;

        virtual std::string toString(const Variable* variable) const;
        virtual std::string toString(const InputVariable* inputVariable) const;
        virtual std::string toString(const OutputVariable* outputVariable) const;
        virtual std::string toString(const RuleBlock* ruleBlock) const;
        virtual std::string toString(const Rule* rule) const;

        virtual std::string toString(const Norm* norm) const;
        virtual std::string toString(const Defuzzifier* defuzzifier) const;
        virtual std::string toString(const Term* term) const;
    };

}

#endif	/* FL_FLLEXPORTER_H */

