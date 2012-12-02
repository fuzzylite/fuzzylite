/*
 * MaximumDefuzzifier.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef MAXIMUMDEFUZZIFIER_H_
#define MAXIMUMDEFUZZIFIER_H_

#include "Defuzzifier.h"

namespace fl {

class MaximumDefuzzifier : public Defuzzifier{
	public:
		enum Type{
			SMALLEST, LARGEST, MEAN
		};
	protected:
		Type _type;
	public:
		MaximumDefuzzifier(Type type, int divisions);

		std::string name() const;
		scalar defuzzify(const Term* term) const;

		void setType(Type type);
		Type getType() const;
};

} /* namespace fl */
#endif /* MAXIMUMDEFUZZIFIER_H_ */
