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
 * File:   FllImporter.h
 * Author: jcrada
 *
 * Created on 18 December 2013, 8:53 AM
 */

#ifndef FL_FLLIMPORTER_H
#define	FL_FLLIMPORTER_H

#include "fl/fuzzylite.h"
#include "fl/imex/Importer.h"

#include <utility>

namespace fl {
    class TNorm;
    class SNorm;
    class Term;
    class Defuzzifier;

    class FL_EXPORT FllImporter : public Importer {
    protected:
        std::string _separator;
    public:
        FllImporter(const std::string& separator = "\n");
        virtual ~FllImporter();

        virtual void setSeparator(const std::string& separator);
        virtual std::string getSeparator() const;

        virtual std::string name() const;
        virtual Engine* fromString(const std::string& fll) const;

    protected:
        virtual void process(const std::string& tag, const std::string& block, Engine* engine) const;
        virtual void processInputVariable(const std::string& block, Engine* engine) const;
        virtual void processOutputVariable(const std::string& block, Engine* engine) const;
        virtual void processRuleBlock(const std::string& block, Engine* engine) const;

        virtual TNorm* parseTNorm(const std::string& name) const;
        virtual SNorm* parseSNorm(const std::string& name) const;

        virtual Term* parseTerm(const std::string& text, Engine* engine) const;

        virtual Defuzzifier* parseDefuzzifier(const std::string& line) const;
        virtual std::pair<scalar, scalar> parseRange(const std::string& line) const;
        virtual bool parseBoolean(const std::string& boolean) const;

        virtual std::pair<std::string, std::string> parseKeyValue(const std::string& text,
                char separator = ':') const;
        virtual std::string clean(const std::string& line) const;

    };
}

#endif	/* FL_FLLIMPORTER_H */

