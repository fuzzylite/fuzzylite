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
 * File:   FisImporter.h
 * Author: jcrada
 *
 * Created on 22 December 2012, 5:53 PM
 */

#ifndef FL_FISIMPORTER_H
#define	FL_FISIMPORTER_H

#include "fl/imex/Importer.h"

#include <vector>
#include <utility>

namespace fl {
    class Norm;
    class TNorm;
    class SNorm;
    class Term;
    class Defuzzifier;
    class Variable;

    class FL_EXPORT FisImporter : public Importer {
    protected:

        virtual void importSystem(const std::string& section, Engine* engine,
                std::string& andMethod, std::string& orMethod,
                std::string& impMethod, std::string& aggMethod,
                std::string& defuzzMethod) const;
        virtual void importInput(const std::string& section, Engine* engine) const;
        virtual void importOutput(const std::string& section, Engine* engine) const;
        virtual void importRules(const std::string& section, Engine* engine) const;
        virtual std::string translateProposition(scalar code, Variable* variable) const;

        virtual std::string tnorm(const std::string& tnorm) const;
        virtual std::string snorm(const std::string& tnorm) const;
        virtual std::string defuzzifier(const std::string& tnorm) const;
        
        
        virtual Term* extractTerm(const std::string& line) const;
        virtual Term* prepareTerm(Term* term, const Engine* engine) const ;
        virtual Term* createInstance(const std::string& termClass, const std::string& name,
                const std::vector<std::string>& params) const;
        virtual std::pair<scalar,scalar> extractRange(const std::string& range) const;

    public:
        FisImporter();
        virtual ~FisImporter();

        virtual std::string name() const;

        virtual Engine* fromString(const std::string& fcl) const;
    };

}
#endif	/* FL_FISIMPORTER_H */

