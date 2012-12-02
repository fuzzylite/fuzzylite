/*
 * Thresholded.h
 *
 *  Created on: 30/11/2012
 *      Author: jcrada
 */

#ifndef FL_THRESHOLDED_H_
#define FL_THRESHOLDED_H_

#include "Term.h"

namespace fl {
class Operator;

class Thresholded: public Term {
	protected:
		const Term* _term;
		scalar _threshold;
		const Operator* _activationOperator;
		public:
		Thresholded(const std::string& name, const Term* term = NULL, scalar threshold = 1.0,
				const Operator* activationOperator = NULL);
		~Thresholded();

		void setTerm(const Term* term);
		const Term* getTerm() const;

		void setThreshold(scalar threshold);
		scalar getThreshold() const;

		void setActivationOperator(const Operator* activationOperator);
		const Operator* getActivationOperator() const;

		scalar minimum() const;
		scalar maximum() const;

		std::string toString() const;

};

} /* namespace fl */
#endif /* FL_THRESHOLDED_H_ */
