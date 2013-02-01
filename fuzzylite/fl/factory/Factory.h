/* 
 * File:   Factory.h
 * Author: jcrada
 *
 * Created on 8 January 2013, 11:10 PM
 */

#ifndef FL_FACTORY_H
#define	FL_FACTORY_H

#include "fl/fuzzylite.h"

namespace fl {
    class TNormFactory;
    class SNormFactory;
    class DefuzzifierFactory;
    class TermFactory;

    class FL_EXPORT Factory {
    protected:
        static Factory* _instance;

        TNormFactory* _tnorm;
        SNormFactory* _snorm;
        DefuzzifierFactory* _defuzzifier;
        TermFactory* _term;

        Factory();
        virtual ~Factory();

    public:
        static Factory* instance();

        virtual void setTnorm(TNormFactory* tnorm);
        virtual TNormFactory* tnorm() const;

        virtual void setSnorm(SNormFactory* snorm);
        virtual SNormFactory* snorm() const;

        virtual void setDefuzzifier(DefuzzifierFactory* defuzzifier);
        virtual DefuzzifierFactory* defuzzifier() const;

        virtual void setTerm(TermFactory* term);
        virtual TermFactory* term() const;




    };
}
#endif	/* FL_FACTORY_H */

