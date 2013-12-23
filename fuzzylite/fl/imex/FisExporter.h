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
 * File:   FisExporter.h
 * Author: jcrada
 *
 * Created on 22 December 2012, 5:52 PM
 */

#ifndef FL_FISEXPORTER_H
#define	FL_FISEXPORTER_H

#include "fl/imex/Exporter.h"

#include <vector>

namespace fl {
    class TNorm;
    class SNorm;
    class Defuzzifier;
    class Term;
    class Rule;
    class Proposition;
    class Variable;

    class FL_EXPORT FisExporter : public Exporter {
    protected:
        
        std::string translate(const std::vector<Proposition*>& propositions,
            const std::vector<Variable*> variables) const;
        
    public:
        FisExporter();
        virtual ~FisExporter();

        virtual std::string name() const;
        virtual std::string toString(const Engine* engine) const;
        
        std::string toString(const TNorm* tnorm) const;
        std::string toString(const SNorm* snorm) const;
        std::string toString(const Defuzzifier* defuzzifier) const;
        std::string toString(const Term* term) const;
        
        std::string exportSystem(const Engine* engine) const;
        std::string exportInputs(const Engine* engine) const;
        std::string exportOutputs(const Engine* engine) const;
        std::string exportRules(const Engine* engine) const;
        std::string exportRule(const Rule* rule, const Engine* engine) const;
    };

}

#endif	/* FL_FISEXPORTER_H */

