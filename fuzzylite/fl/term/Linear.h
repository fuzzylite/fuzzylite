/* 
 * File:   Linear.h
 * Author: jcrada
 *
 * Created on 8 May 2013, 3:42 PM
 */

#ifndef FL_LINEAR_H
#define	FL_LINEAR_H

#include "fl/term/Term.h"

#include <map>

namespace fl {

    class FL_EXPORT Linear : public Term {
    protected:
        std::map<std::string, std::pair<scalar, const InputVariable*> > _coefficients;

    public:
        Linear(const std::string& name = "");
        virtual ~Linear();

        virtual void setCoefficient(scalar coefficient, const InputVariable* input);
        virtual scalar getCoefficient(const std::string& input) const;
        virtual scalar removeCoefficient(const std::string& input);
        virtual bool hasCoefficient() const;
        
        virtual void clearCoefficients();

        virtual scalar membership(scalar x) const = 0;

        virtual std::string className() const = 0;
        virtual std::string toString() const = 0;

        virtual Linear* copy() const = 0;

    };

}

#endif	/* FL_LINEAR_H */

