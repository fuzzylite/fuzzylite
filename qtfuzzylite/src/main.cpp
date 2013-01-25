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

#include <signal.h>

class QtFuzzyLite : public QApplication {
public:

    QtFuzzyLite(int& argc, char ** argv) : QApplication(argc, argv) { }

    bool notify(QObject * receiver, QEvent * event) {
        try {
            return QApplication::notify(receiver, event);
        } catch (std::exception& ex) {
            this->catchException(ex);
        }
        return false;
    }

    void catchException(const std::exception& ex) {
        QString error = QString::fromUtf8(ex.what());
        QMessageBox::critical(NULL, "Internal Error",
                "<qt><b>qtfuzzylite</b> has experienced an internal error and will exit.<br><br>"
                "Please report this error to &nbsp; <a href='mailto:errors@fuzzylite.com'>"
                "errors@fuzzylite.com</a><br><br>"

                "Your report will help to make <b>fuzzylite</b> and <b>qtfuzzylite</b> a better "
                "free open source fuzzy logic library!<br><br>"
                "Many thanks in advance for your help!<br><br>"
                "Error message:"
                "<br><br> <font size='2'> <tt>" +
                Qt::escape(error) +
                "</tt></font></qt>",
                QMessageBox::Ok);
        QApplication::quit();
    }

};

int main(int argc, char* argv[]) {
    QtFuzzyLite qtfuzzylite(argc, argv);
    signal(SIGSEGV, fl::Exception::signalHandler);
    try {
        fl::qt::Window::main();
        //        int *x = (int*) - 1; // make a bad pointer
        //        FL_LOG(*x);
        return qtfuzzylite.exec();
    } catch (std::exception& ex) {
        qtfuzzylite.catchException(ex);
        return EXIT_FAILURE;
    }
}

