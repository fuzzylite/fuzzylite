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
 * File:   Factory.h
 * Author: jcrada
 *
 * Created on 8 January 2013, 11:10 PM
 */

#ifndef FL_FACTORYMANAGER_H
#define	FL_FACTORYMANAGER_H

#include "fl/fuzzylite.h"

namespace fl {
    class TNormFactory;
    class SNormFactory;
    class DefuzzifierFactory;
    class TermFactory;
    class HedgeFactory;

    class FL_EXPORT FactoryManager {
    protected:
        static FactoryManager* _instance;

        TNormFactory* _tnorm;
        SNormFactory* _snorm;
        DefuzzifierFactory* _defuzzifier;
        TermFactory* _term;
        HedgeFactory* _hedge;

        FactoryManager();
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

    };
}
#endif	/* FL_FACTORYMANAGER_H */

