/*
 * Configuration.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_CONFIGURATION_H_
#define FL_CONFIGURATION_H_

#include "fl/engine/Operator.h"

#include "fl/defuzzifier/CenterOfGravity.h"

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
                Defuzzifier* defuzzifier = new CenterOfGravity);
        virtual ~Configuration();

        virtual void setTnorm(Operator* tnorm);
        virtual Operator* getTnorm() const;

        virtual void setSnorm(Operator* snorm);
        virtual Operator* getSnorm() const;

        virtual void setActivation(Operator* activation);
        virtual Operator* getActivation() const;

        virtual void setAccumulation(Operator* accumulation);
        virtual Operator* getAccumulation() const;

        virtual void setDefuzzifier(Defuzzifier* defuzzifier);
        virtual Defuzzifier* getDefuzzifier() const;

        virtual std::string toString() const;
    };

} /* namespace fl */
#endif /* FL_CONFIGURATION_H_ */
