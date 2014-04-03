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
 * FclExporter.h
 *
 *  Created on: 8/12/2012
 *      Author: jcrada
 */

#ifndef FL_FCLEXPORTER_H
#define FL_FCLEXPORTER_H

#include "fl/imex/Exporter.h"

namespace fl {
    class TNorm;
    class SNorm;
    class Defuzzifier;
    class Term;

    class FL_EXPORT FclExporter : public Exporter {
    protected:
        std::string _indent;

    public:
        FclExporter(const std::string& indent = "  ");
        virtual ~FclExporter();

        virtual void setIndent(const std::string& indent);
        virtual std::string getIndent() const;

        virtual std::string name() const;
        virtual std::string toString(const Engine* engine) const;
        
        virtual std::string toString(const TNorm* tnorm) const;
        virtual std::string toString(const SNorm* snorm) const;
        virtual std::string toString(const Defuzzifier* defuzzifier) const;
        virtual std::string toString(const Term* term) const;
        
        virtual FclExporter* clone() const;
    };

}
#endif /* FL_FCLEXPORTER_H */
