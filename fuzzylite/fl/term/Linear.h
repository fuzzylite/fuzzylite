/* 
 * File:   Linear.h
 * Author: jcrada
 *
 * Created on 8 May 2013, 3:42 PM
 */

#ifndef FL_LINEAR_H
#define	FL_LINEAR_H

#include "fl/term/Term.h"
#include "fl/variable/InputVariable.h"

#include <map>
#include <utility>

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
        virtual bool hasCoefficient(const std::string& input) const;

        virtual void clearCoefficients();

        virtual scalar membership(scalar x) const;

        virtual std::string className() const;
        virtual std::string toString() const;

        virtual Linear* copy() const;

    };

}

#endif	/* FL_LINEAR_H */

