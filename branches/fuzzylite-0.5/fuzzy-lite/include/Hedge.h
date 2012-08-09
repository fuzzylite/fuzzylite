/* 
 * File:   Hedge.h
 * Author: jcrada
 *
 * Created on November 1, 2009, 2:55 PM
 */

#ifndef _HEDGE_H
#define	_HEDGE_H

#include <string>
#include <math.h>

#include "flScalar.h"
#include "defs.h"

namespace fuzzy_lite {

    //TODO: Ask if a hedge should affect the output. It does on the inputs, but outputs?
    //if x is low then y is very high. does very affect the defuzzification? If so, isn't it y^2 WAY too much? 
    //if it doesn't affect the defuzzification, how does 'very' affect a term?
    class Hedge {
    private:
        std::string _name;
    public:

        Hedge(const std::string& name) : _name(name) {
        }

        virtual ~Hedge() {
        }

        virtual std::string getName() const {
            return _name;
        }

        virtual flScalar hedge(flScalar mu) = 0;
    };

    class HedgeNot : public Hedge {
    public:

        HedgeNot() : Hedge("not") {
        }

        FL_INLINE virtual flScalar hedge(flScalar mu) {
            return 1 - mu;
        }
    };

    class HedgeSomewhat : public Hedge {
    public:

        HedgeSomewhat() : Hedge("somewhat") {
        };

        FL_INLINE virtual flScalar hedge(flScalar mu) {
            return sqrt(mu);
        }
    };

    class HedgeVery : public Hedge {
    public:

        HedgeVery() : Hedge("very") {
        }

        FL_INLINE virtual flScalar hedge(flScalar mu) {
            return mu * mu;
        }
    };
}

#endif	/* _HEDGE_H */

