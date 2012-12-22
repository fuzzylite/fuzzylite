/*
 * Configuration.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_CONFIGURATION_H_
#define FL_CONFIGURATION_H_

#include "fl/operator/TNorm.h"
#include "fl/operator/SNorm.h"

#include "fl/defuzzifier/CenterOfGravity.h"

#include <string>

namespace fl {
    class Defuzzifier;

    class Configuration {
    protected:
        TNorm* _tnorm;
        SNorm* _snorm;
        TNorm* _activation;
        SNorm* _accumulation;
        Defuzzifier* _defuzzifier;

    public:
        Configuration(TNorm* tnorm = new Minimum(), SNorm* snorm = new Maximum(),
                TNorm* activation = new Minimum(), SNorm* accumulation = new Maximum(),
                Defuzzifier* defuzzifier = new CenterOfGravity);
        virtual ~Configuration();

        virtual void setTnorm(TNorm* tnorm);
        virtual TNorm* getTnorm() const;

        virtual void setSnorm(SNorm* snorm);
        virtual SNorm* getSnorm() const;

        virtual void setActivation(TNorm* activation);
        virtual TNorm* getActivation() const;

        virtual void setAccumulation(SNorm* accumulation);
        virtual SNorm* getAccumulation() const;

        virtual void setDefuzzifier(Defuzzifier* defuzzifier);
        virtual Defuzzifier* getDefuzzifier() const;

        virtual std::string toString() const;
    };

} 
#endif /* FL_CONFIGURATION_H_ */
