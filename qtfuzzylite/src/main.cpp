/**
    This file is part of qtfuzzylite.

    qtfuzzylite is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    qtfuzzylite is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with qtfuzzylite.  If not, see <http://www.gnu.org/licenses/>.

    Juan Rada-Vilela, 01 February 2013
    jcrada@fuzzylite.com 
 **/

/*
 * main.cpp
 *
 *  Created on: 10/12/2012
 *      Author: jcrada 
 */

#include <QApplication> 
#include <QCoreApplication>
#include <QMessageBox>
#include <QMenuBar>
#include <QFileInfo>
#include <QSettings>

#include <fl/Headers.h>

#include "fl/qt/Window.h"  
#include "fl/qt/Term.h"
#include "fl/qt/Viewer.h"  
#include "fl/qt/qtfuzzylite.h"

#include <signal.h>

namespace fl {

    class QtFuzzyLite : public QApplication {
    public:

        QtFuzzyLite(int& argc, char ** argv) : QApplication(argc, argv) {
            QCoreApplication::setOrganizationName("fuzzylite");
            QCoreApplication::setOrganizationDomain("fuzzylite.com");
            QCoreApplication::setApplicationName(
                    QString::fromStdString("qtfuzzylite-" +
                    fl::qt::qtfuzzylite::version()));

            QApplication::setWindowIcon(QIcon(":/qtfuzzylite.png"));
        }

        bool notify(QObject * receiver, QEvent * event) {
            try {
                return QApplication::notify(receiver, event);
            } catch (std::exception& ex) {
                this->catchException(ex);
            } catch (...) {
                std::string message = "[unexpected exception] backtrace:\n"
                        + fl::Exception::btCallStack();
                fl::Exception ex(message, FL_AT);
                this->catchException(ex);
            }
            return false;
        }

        void catchException(const std::exception& ex) {
            QString error = QString::fromUtf8(ex.what());
            QString message = "<qt><b>qtfuzzylite</b> has experienced an internal error and will exit.<br><br>"
                    "Please report this error in the appropriate <a href='http://www.fuzzylite.com/forums/'>"
                    "<b>forums</b></a><br><br>"
                    "Your report will help to make <b>fuzzylite</b> a better "
                    "free open source fuzzy logic library!<br><br>"
                    "Many thanks in advance for your help!"
                    "</qt>";
            QMessageBox x(NULL);
            x.setText(message);
            x.setWindowTitle("Internal Error");
            x.setIcon(QMessageBox::Critical);
            QLabel dummy;
            x.layout()->addWidget(&dummy);

            QLabel viewLabel("Error message:");
            QPlainTextEdit viewError;
            viewError.setReadOnly(true);
            viewError.setPlainText(error);
            viewError.setLineWrapMode(QPlainTextEdit::NoWrap);
            QFont tt("?");
            tt.setStyleHint(QFont::TypeWriter);
            tt.setPointSize(tt.pointSize() - 2);
            viewError.setFont(tt);

            QWidget* view = new QWidget;
            view->setLayout(new QVBoxLayout);
            view->layout()->addWidget(&viewLabel);
            view->layout()->addWidget(&viewError);
            x.layout()->addWidget(view);
            x.exec();
            //            QApplication::quit();
        }

    };

    static QtFuzzyLite* qtfuzzylite = NULL;

    void terminate() {
        std::string message = "[unexpected exception] backtrace:\n"
                + fl::Exception::btCallStack(50);
        fl::Exception ex(message, FL_AT);
        qtfuzzylite->catchException(ex);
        exit(EXIT_FAILURE);
    }

    void signalHandler(int signal) {
        std::ostringstream message;
        message << "[caught signal " << signal << "] backtrace:\n"
                << fl::Exception::btCallStack(50);
        fl::Exception ex(message.str(), FL_AT);
        qtfuzzylite->catchException(ex);
        exit(EXIT_FAILURE);
    }
}

//std::string flPlatform() {
//#ifdef Q_OS_X11
//    return "Linux";
//#endif
//
//#ifdef Q_OS_MAC
//    return "Mac";
//#endif
//
//#ifdef Q_OS_QWS
//    //    return "Embedded Linux";
//    return "Linux";
//#endif
//
//#ifdef Q_OS_WIN
//    return "Windows";
//#endif
//}

int main(int argc, char* argv[]) {
    fl::qtfuzzylite = new fl::QtFuzzyLite(argc, argv);
    std::set_terminate(fl::terminate);
    std::set_unexpected(fl::terminate);
    signal(SIGSEGV, fl::signalHandler);
    signal(SIGABRT, fl::signalHandler);
    signal(SIGILL, fl::signalHandler);
    signal(SIGSEGV, fl::signalHandler);
    signal(SIGFPE, fl::signalHandler);
#ifdef FL_UNIX
    signal(SIGBUS, fl::signalHandler);
    signal(SIGPIPE, fl::signalHandler);
#endif
    try {
        QSettings settings;
        int decimals = settings.value("general/decimals", 3).toInt();
        fl::fuzzylite::setDecimals(decimals);
        fl::fuzzylite::setLogging(true);
        fl::qt::Window::main(argc, argv);
        //int *x = (int*) - 1; // make a bad pointer
        //FL_LOG(*x);
        //                throw 0;
        int result = fl::qtfuzzylite->exec();
        delete fl::qtfuzzylite;
        return result;
    } catch (std::exception& ex) {
        fl::qtfuzzylite->catchException(ex);
    } catch (...) {
        fl::Exception ex("[uncaught exception]", FL_AT);
        fl::qtfuzzylite->catchException(ex);
    }
    return EXIT_FAILURE;
}

#ifdef FL_WINDOWS
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow
        ) {
    return main(0, NULL);
}
#endif

