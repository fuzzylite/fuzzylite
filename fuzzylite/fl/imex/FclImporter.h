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
 * FclImporter.h
 *
 *  Created on: 8/12/2012
 *      Author: jcrada
 */

#ifndef FL_FCLIMPORTER_H
#define FL_FCLIMPORTER_H

#include "fl/imex/Importer.h"



#include <string>
#include <vector>
#include <utility>

namespace fl {
    class Norm;
    class TNorm;
    class SNorm;
    class Term;
    class Defuzzifier;

    class FL_EXPORT FclImporter : public Importer {
    protected:
        virtual void processBlock(const std::string& tag, const std::string& block, Engine* engine) const;
        virtual void processVar(const std::string& var, const std::string& block, Engine* engine)const;
        virtual void processFuzzify(const std::string& block, Engine* engine)const;
        virtual void processDefuzzify(const std::string& block, Engine* engine)const;
        virtual void processRuleBlock(const std::string& block, Engine* engine)const;

        virtual TNorm* extractTNorm(const std::string& line) const;
        virtual SNorm* extractSNorm(const std::string& line) const;

        virtual Term* extractTerm(const std::string& line) const;
        virtual Term* prepareTerm(Term* term, const Engine* engine) const;

        virtual Defuzzifier* extractDefuzzifier(const std::string& line) const;
        virtual std::pair<scalar, bool> extractDefaultValue(const std::string& line) const;
        virtual std::pair<scalar, scalar> extractRange(const std::string& line) const;
        virtual std::pair<bool, bool> extractLocksOutputRange(const std::string& line) const;
        virtual bool extractEnabled(const std::string& line) const;

    public:
        FclImporter();
        virtual ~FclImporter();

        virtual std::string name() const;

        virtual Engine* fromString(const std::string& fcl) const;
        
        virtual FclImporter* clone() const;

    };

}
#endif /* FL_FCLIMPORTER_H */
