/*
 * Defuzzifier.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_DEFUZZIFIER_H
#define FL_DEFUZZIFIER_H

#include "fl/definitions.h"

#include "fl/scalar.h"

#include <string>

namespace fl {
    class Term;

    class FL_EXPORT Defuzzifier {
    protected:
        int _divisions;

    public:
        Defuzzifier(int divisions = FL_DIVISIONS);
        virtual ~Defuzzifier();

        virtual std::string className() const = 0;
        virtual scalar defuzzify(const Term* term, scalar minimum, scalar maximum) const = 0;

        virtual void setDivisions(int divisions);
        virtual int getDivisions() const;



    };

} 
#endif /* FL_DEFUZZIFIER_H */
