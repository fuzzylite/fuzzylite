/*
 * Accumulated.h
 *
 *  Created on: 30/11/2012
 *      Author: jcrada
 */

#ifndef FL_ACCUMULATED_H_
#define FL_ACCUMULATED_H_

#include "fl/term/Term.h"

#include <vector>

namespace fl {

    class Operator;
    class Accumulated: public Term {
    protected:
        std::vector<const Term*> _terms;
        const Operator* _accumulation;
        scalar _minimum, _maximum;
        public:
        Accumulated(const std::string& name = "",
                const Operator* accumulation= NULL);
        ~Accumulated();

        std::string className() const{
            return "Accumulated";
        }

        scalar membership(scalar x) const;
        std::string toString() const;

        void accumulate(const Term* term);
        int size() const;
        bool isEmpty() const;
        void clear();

        scalar minimum() const;
        scalar maximum() const;

        void setAccumulation(const Operator* accumulation);
        const Operator* getAccumulation() const;

    };

} /* namespace fl */
#endif /* FL_ACCUMULATED_H_ */
