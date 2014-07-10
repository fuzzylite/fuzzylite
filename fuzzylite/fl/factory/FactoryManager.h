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
 * File:   Factory.h
 * Author: jcrada
 *
 * Created on 8 January 2013, 11:10 PM
 */

#ifndef FL_FACTORYMANAGER_H
#define	FL_FACTORYMANAGER_H

#include "fl/fuzzylite.h"
#include <memory>

namespace fl {
    class TNormFactory;
    class SNormFactory;
    class DefuzzifierFactory;
    class TermFactory;
    class HedgeFactory;
    class FunctionFactory;

    class FL_EXPORT FactoryManager {
    protected:
        static FactoryManager _instance;

        std::auto_ptr<TNormFactory> _tnorm;
        std::auto_ptr<SNormFactory> _snorm;
        std::auto_ptr<DefuzzifierFactory> _defuzzifier;
        std::auto_ptr<TermFactory> _term;
        std::auto_ptr<HedgeFactory> _hedge;
        std::auto_ptr<FunctionFactory> _function;

        FactoryManager();
        FactoryManager(TNormFactory* tnorm, SNormFactory* snorm,
                DefuzzifierFactory* defuzzifier, TermFactory* term,
                HedgeFactory* hedge, FunctionFactory* function);
        FactoryManager(const FactoryManager& source);
        FactoryManager& operator=(const FactoryManager& rhs);
        virtual ~FactoryManager();

    public:
        static FactoryManager* instance();

        virtual void setTnorm(TNormFactory* tnorm);
        virtual TNormFactory* tnorm() const;

        virtual void setSnorm(SNormFactory* snorm);
        virtual SNormFactory* snorm() const;

        virtual void setDefuzzifier(DefuzzifierFactory* defuzzifier);
        virtual DefuzzifierFactory* defuzzifier() const;

        virtual void setTerm(TermFactory* term);
        virtual TermFactory* term() const;

        virtual void setHedge(HedgeFactory* hedge);
        virtual HedgeFactory* hedge() const;

        virtual void setFunction(FunctionFactory* function);
        virtual FunctionFactory* function() const;
    };
}
#endif	/* FL_FACTORYMANAGER_H */

