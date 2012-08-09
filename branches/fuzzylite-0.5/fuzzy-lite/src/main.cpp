/* 
 * File:   main.cpp
 * Author: jcrada
 *
 * Created on October 28, 2009, 2:29 PM
 */


#include <vector>
#include "DiscreteTerm.h"
#include "defs.h"
#include "MamdaniRule.h"
#include "FuzzyException.h"
#include "test.h"
/*
 * 
 */

using namespace fuzzy_lite;

int main(int argc, char** argv) {
	FL_LOG("Running");
    //    MamdaniRule::main(argc,argv);
//        DiscreteTerm::main(argc,argv);
//    FuzzyEngine::main(argc, argv);
    //    FuzzyException::main(argc, argv);
    Test::main(argc,argv);
    return (EXIT_SUCCESS);
}

