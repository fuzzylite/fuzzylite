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

        virtual FllImporter* clone() const;


    };
}

#endif	/* FL_FLLIMPORTER_H */

