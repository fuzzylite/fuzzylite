/*
 * main.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "Headers.h"

#include <typeinfo>

using namespace fl;

int main(int argc, char** argv) {
    FL_LOG("Hello, FuzzyLite!");
    scalar someScalar = 0;
    int scalarSize = sizeof(someScalar);

    FL_LOG("scalar is defined as type <" << (typeid(scalar).name()) << "> with " << scalarSize << " bytes");
    FL_LOG("tolerance to floating-point value is " << FL_EPSILON);

    SimpleMamdani m;
    m.create();
    m.engine();
    FL_LOG(FclExporter().toFcl(m.engine()));
    FL_LOG("Bye, FuzzyLite!");

}

