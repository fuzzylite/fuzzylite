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
 * File:   JavaExporter.h
 * Author: jcrada
 *
 * Created on 28 November 2013, 10:20 PM
 */

#ifndef FL_JAVAEXPORTER_H
#define	FL_JAVAEXPORTER_H

#include "fl/imex/Exporter.h"

namespace fl {

    class Engine;
    class InputVariable;
    class OutputVariable;
    class RuleBlock;
    class Term;
    class Defuzzifier;
    class Norm;

    class FL_EXPORT JavaExporter : public Exporter {
    public:
        JavaExporter();
        virtual ~JavaExporter();

        virtual std::string name() const;

        virtual std::string toString(const Engine* engine) const;
        virtual std::string toString(const InputVariable* inputVariable, const Engine* engine) const;
        virtual std::string toString(const OutputVariable* outputVariable, const Engine* engine) const;
        virtual std::string toString(const RuleBlock* ruleBlock, const Engine* engine) const;
        virtual std::string toString(const Term* term) const;
        virtual std::string toString(const Defuzzifier* defuzzifier) const;
        virtual std::string toString(const Norm* norm) const;
        virtual std::string toString(scalar value) const;

    };

}

#endif	/* FL_JAVAEXPORTER_H */

