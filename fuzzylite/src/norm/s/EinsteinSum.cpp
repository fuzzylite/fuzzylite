/* 
 * File:   EinsteinSum.cpp
 * Author: jcrada
 *
 * Created on 30 November 2013, 1:17 AM
 */

#include "fl/norm/s/EinsteinSum.h"

namespace fl {

    std::string EinsteinSum::className() const {
        return "EinsteinSum";
    }

    scalar EinsteinSum::compute(scalar a, scalar b) const {
        return (a + b) / (1.0 + a * b);
    }

    SNorm* EinsteinSum::create() {
        return new EinsteinSum;
    }
}