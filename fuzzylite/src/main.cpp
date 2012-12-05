/*
 * main.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "Headers.h"



using namespace fl;

int main(int argc, char** argv){
    FL_LOG("Hello, FuzzyLite!");
    scalar someScalar = 0;
    int scalarSize = sizeof(someScalar);

    if (scalarSize == 4){
        FL_LOG("scalar is defined as a FLOAT with " << scalarSize << " bytes");
    }else if (scalarSize == 8){
        FL_LOG("scalar is defined as a DOUBLE with " << scalarSize << " bytes");
    }else{
        FL_LOG("scalar is UNKNOWN represented with " << scalarSize << " bytes");
    }

    FL_LOG("tolerance to floating-point value is " << FL_EPSILON);

    InfixToPostfix ip;
    InfixToPostfix::GenericFunction* f = ip.getGenericFunction("acos");
    FL_LOG("acos(0.4) = " << f->oneArgFunction(0.4));
    std::map<std::string, InfixToPostfix::GenericFunction*> gf =  ip.genericFunctions();
    std::map<std::string, InfixToPostfix::GenericFunction*>::const_iterator it = gf.begin();
    for (; it != gf.end(); ++it){
        FL_LOG(it->first << "{" << it->second->toString() << "}");
    }

    FL_LOG("Bye, FuzzyLite!");

}


