/*
 * Defuzzifier.cpp
 *
 *  Created on: 11/12/2012
 *      Author: jcrada
 */

#include "fl/defuzzifier/Defuzzifier.h"

namespace fl{

    Defuzzifier::Defuzzifier(int divisions) : _divisions(divisions){
    }

    Defuzzifier::~Defuzzifier(){}

    void Defuzzifier::setDivisions(int divisions) {
        this->_divisions = divisions;
    }

    int Defuzzifier::getDivisions() const{
        return this->_divisions;
    }



}


