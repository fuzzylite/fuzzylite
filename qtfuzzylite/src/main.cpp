/*
 * main.cpp
 *
 *  Created on: 10/12/2012
 *      Author: jcrada 
 */

#include <QtGui/QApplication>
#include <fl/Headers.h>

#include "fl/qt/Configuration.h" 
#include "fl/qt/Window.h"
#include "fl/qt/Term.h"
#include "fl/qt/Viewer.h"  
 
int main(int argc, char* argv[]){ 
  
    QApplication fuzzylite(argc,argv);
//    fl::qt::Canvas::main(); 
//    fl::qt::Configuration::main();
//    fl::qt::Term::main(); 
    fl::qt::Window::main(); 
//    fl::qt::Viewer::main(); 
      
//http://octave.sourceforge.net/fuzzy-logic-toolkit/overview.html
    return fuzzylite.exec();

}

