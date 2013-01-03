/*
 * Defuzzifier.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_DEFUZZIFIER_H_
#define FL_DEFUZZIFIER_H_

#include "fl/scalar.h"
#include "fl/definitions.h"

#include <string>

namespace fl {
    class Term;

    class Defuzzifier {
    protected:
        int _divisions;

    public:
        Defuzzifier(int divisions = FL_DEFAULT_DIVISIONS);
        virtual ~Defuzzifier();

        virtual std::string className() const = 0;
        virtual scalar defuzzify(const Term* term, scalar minimum, scalar maximum) const = 0;

        virtual void setDivisions(int divisions);
        virtual int getDivisions() const;



    };

} 
#endif /* FL_DEFUZZIFIER_H_ */
