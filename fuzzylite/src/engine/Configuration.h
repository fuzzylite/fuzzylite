/*
 * Configuration.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_CONFIGURATION_H_
#define FL_CONFIGURATION_H_
#include "Operator.h"

#include <string>



namespace fl {
class Defuzzifier;

class Configuration {
	protected:
		Operator* _tnorm;
		Operator* _snorm;
		Operator* _activation;
		Operator* _accumulation;
		Defuzzifier* _defuzzifier;
		public:
		Configuration(Operator* tnorm = new Min(), Operator* snorm = new Max(),
				Operator* activation = new Min(), Operator* accumulation = new Max(),
				Defuzzifier* defuzzifier = NULL);
		~Configuration();

		void setTnorm(Operator* tnorm);
		Operator* getTnorm() const;

		void setSnorm(Operator* snorm);
		Operator* getSnorm() const;

		void setActivation(Operator* activation);
		Operator* getActivation() const;

		void setAccumulation(Operator* accumulation);
		Operator* getAccumulation() const;

		void setDefuzzifier(Defuzzifier* defuzzifier);
		Defuzzifier* getDefuzzifier() const;

		std::string toString() const;
};

} /* namespace fl */
#endif /* FL_CONFIGURATION_H_ */
