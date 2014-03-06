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

