/*
 * Variable.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_VARIABLE_H_
#define FL_VARIABLE_H_

#include "fl/scalar.h"

#include <string>
#include <vector>
#include <limits>
namespace fl {

    class Term;
    class Configuration;

    class Variable {
    protected:
        std::string _name;
        std::vector<Term*> _terms;
        scalar _minimum, _maximum;

    public:
        Variable(const std::string& name = "",
                scalar minimum = -std::numeric_limits<scalar>::infinity(),
                scalar maximum = std::numeric_limits<scalar>::infinity());
        Variable(const Variable& copy);

        virtual ~Variable();

        virtual void configure(Configuration* config);

        virtual void setName(const std::string& name);
        virtual std::string getName() const;

        virtual void setMinimum(scalar minimum);
        virtual scalar getMinimum() const;

        virtual void setMaximum(scalar maximum);
        virtual scalar getMaximum() const;

        virtual std::string fuzzify(scalar x) const;
        virtual Term* highestMembership(scalar x, scalar* yhighest = NULL) const;

        virtual std::string toString() const;

        /**
         * Operations for iterable datatype _terms
         */
        virtual void addTerm(Term* term);
        virtual void insertTerm(Term* term, int index);
        virtual Term* getTerm(int index) const;
        virtual Term* getTerm(const std::string& name) const;
        virtual bool hasTerm(const std::string& name) const;
        virtual Term* removeTerm(int index);
        virtual int numberOfTerms() const;
        virtual const std::vector<Term*>& terms() const;

    };

}

#endif /* FL_VARIABLE_H_ */
