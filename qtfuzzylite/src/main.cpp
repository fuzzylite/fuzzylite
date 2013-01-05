/*
 * main.cpp
 *
 *  Created on: 10/12/2012
 *      Author: jcrada 
 */

#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <fl/Headers.h>

#include "fl/qt/Configuration.h"  
#include "fl/qt/Window.h" 
#include "fl/qt/Term.h"
#include "fl/qt/Viewer.h"   
 
class QtFuzzyLite : public QApplication {
public:

    QtFuzzyLite(int& argc, char ** argv) : QApplication(argc, argv) { }

    bool notify(QObject * receiver, QEvent * event) {
        try {
            return QApplication::notify(receiver, event);
        } catch (std::exception& ex) {
            QString error = QString::fromUtf8(ex.what());
            //        error.replace("\n", "<br>"); 

            QMessageBox::critical(NULL, "Internal Error",
                    "<qt><b>qtfuzzylite</b> has experienced an internal error and will exit.<br><br>"
                    "Please report this error at:<br><br><a href='http://code.google.com/p/fuzzylite/issues/'>"
                    "http://code.google.com/p/fuzzylite/issues/</a><br><br>"
 
                    "Your report will help to make fuzzylite and qtfuzzylite a better "
                    "<b>open source and free</b> fuzzy logic library!<br><br>"
                    "Many thanks in advance for your help!<br><br>"
                    "Error message:<br><br> " +
                    Qt::escape(error) + "</qt>",
                    QMessageBox::Ok);
            QApplication::quit();
        } 
        return false;
    }
};

int main(int argc, char* argv[]) {
    QtFuzzyLite qtfuzzylite(argc, argv);   
    try {
        fl::qt::Window::main(); 
        return qtfuzzylite.exec();  
    } catch (std::exception& ex) {
        QString error = QString::fromUtf8(ex.what());
        //        error.replace("\n", "<br>"); 
 
        QMessageBox::critical(NULL, "Internal Error",
                "<qt><b>qtfuzzylite</b> has experienced an internal error and will exit.<br><br>"
                "Please report this error at:<br><br><a href='http://code.google.com/p/fuzzylite/issues/'>"
                "http://code.google.com/p/fuzzylite/issues/</a><br><br>"
   
                "Your report will help to make fuzzylite and qtfuzzylite a better "
                "<b>open source and free</b> fuzzy logic library!<br><br>"
                "Many thanks in advance for your help!<br><br>"
                "Error message:<br><br> " +
                Qt::escape(error) + "</qt>",
                QMessageBox::Ok);
    }
    return 1;
}

