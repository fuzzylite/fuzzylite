/*
 * main.cpp
 *
 *  Created on: 10/12/2012
 *      Author: jcrada
 */

#include <QtGui/QApplication>
#include <fl/Headers.h>

#include "fl/qt/Canvas.h"
#include "fl/qt/Configuration.h"
#include "fl/qt/Window.h"
#include "fl/qt/Term.h"

int main(int argc, char* argv[]){

    QApplication fuzzylite(argc,argv);
//    fl::qt::Canvas::main();
//    fl::qt::Configuration::main();
    fl::qt::Window::main();
//    fl::qt::Term::main();

    return fuzzylite.exec();

}

