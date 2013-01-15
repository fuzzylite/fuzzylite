/*
 * Accumulated.h
 *
 *  Created on: 30/11/2012
 *      Author: jcrada
 */

#ifndef FL_ACCUMULATED_H
#define FL_ACCUMULATED_H

#include "fl/term/Term.h"

#include <vector>

namespace fl {

    class SNorm;

    class Accumulated : public Term {
    protected:
        std::vector<const Term*> _terms;
        scalar _minimum, _maximum;
        const SNorm* _accumulation;
    public:
        Accumulated(const std::string& name = "",
                scalar minimum = -std::numeric_limits<scalar>::infinity(),
                scalar maximum = std::numeric_limits<scalar>::infinity(),
                const SNorm* accumulation = NULL);
        ~Accumulated();

        std::string className() const;
        Accumulated* copy() const;

        scalar membership(scalar x) const;
        std::string toString() const;

        void setMinimum(scalar minimum);
        scalar getMinimum() const;

        void setMaximum(scalar maximum);
        scalar getMaximum() const;

        void setAccumulation(const SNorm* accumulation);
        const SNorm* getAccumulation() const;

        /**
         * Operations for std::vector _terms
         */
        void addTerm(const Term* term);
        const Term* getTerm(int index) const;
        const Term* removeTerm(int index);
        int numberOfTerms() const;
        const std::vector<const Term*>& terms() const;
        bool isEmpty() const;
        void clear();



    };

}
#endif /* FL_ACCUMULATED_H */
