/* 
 * File:   None.h
 * Author: jcrada
 *
 * Created on 12 February 2014, 9:15 PM
 */

#ifndef FL_NONE_H
#define	FL_NONE_H

#include "fl/defuzzifier/Defuzzifier.h"
#include "fl/norm/SNorm.h"
#include "fl/norm/TNorm.h"

namespace fl {

    class FL_EXPORT None : public Defuzzifier, public SNorm, public TNorm {
    public:
        None();
        virtual ~None();

        //Defuzzifier and Norm
        virtual std::string className() const;
        virtual bool isNone() const;

        //Defuzzifier
        virtual scalar defuzzify(const Term* term,
                scalar minimum, scalar maximum) const;

        //Norms
        virtual scalar compute(scalar a, scalar b) const;

        static Defuzzifier* defuzzifierConstructor();
        static SNorm* snormConstructor();
        static TNorm* tnormConstructor();
    };
}

#endif	/* FL_NONE_H */

