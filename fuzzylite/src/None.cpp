/* 
 * File:   None.cpp
 * Author: jcrada
 *
 * Created on 12 February 2014, 9:23 PM
 */

#include "fl/None.h"

namespace fl {

    None::None() {
    }

    None::~None() {
    }

    //Defuzzifier and Norms:

    std::string None::className() const {
        return "None";
    }

    bool None::isNone() const {
        return true;
    }

    //Defuzzifier:

    scalar None::defuzzify(const Term* term, scalar minimum, scalar maximum) const {
        (void) term;
        (void) minimum;
        (void) maximum;
        throw fl::Exception("[defuzzifier error] cannot use defuzzifier <" + className() + "> for regular operation", FL_AT);
    }

    //Norms:

    scalar None::compute(scalar a, scalar b) const {
        (void) a;
        (void) b;
        throw fl::Exception("[norm error] cannot use (S|T)-Norm <" + className() + "> for regular operation", FL_AT);
    }

    Defuzzifier* None::defuzzifierConstructor() {
        return new None;
    }

    TNorm* None::tnormConstructor() {
        return new None;
    }

    SNorm* None::snormConstructor() {
        return new None;
    }


}
