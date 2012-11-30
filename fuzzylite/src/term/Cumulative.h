/*
 * Cumulative.h
 *
 *  Created on: 30/11/2012
 *      Author: jcrada
 */

#ifndef FL_CUMULATIVE_H_
#define FL_CUMULATIVE_H_

#include "../Term.h"

#include <vector>

namespace fl {

    class Operator;
    class Cumulative: public Term {
    protected:
        std::vector<const Term*> _terms;
        const Operator* _accumulationOperator;
        scalar _minimum, _maximum;
        public:
        Cumulative(const std::string& name,
                const Operator* accumulationOperator = NULL);
        ~Cumulative();

        scalar membership(scalar x) const;
        std::string toString() const;

        void append(const Term* term);
        int size() const;
        void clear();

        scalar minimum() const;
        scalar maximum() const;

        void setAccumulationOperator(const Operator* accumulationOperator);
        const Operator* getAccumulationOperator() const;

    };

} /* namespace fl */
#endif /* FL_CUMULATIVE_H_ */
