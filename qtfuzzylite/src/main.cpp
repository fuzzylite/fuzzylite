/*
 * main.cpp
 *
 *  Created on: 10/12/2012
 *      Author: jcrada 
 */

#include <QtGui/QApplication> 
#include <QtGui/QMessageBox>
#include <QtGui/QMenuBar>
#include <fl/Headers.h>

#include "fl/qt/Preferences.h"  
#include "fl/qt/Window.h"  
#include "fl/qt/Term.h"
#include "fl/qt/Viewer.h"   
 
#include <signal.h>

namespace fl {

    class QtFuzzyLite : public QApplication {
    public:

        QtFuzzyLite(int& argc, char ** argv) : QApplication(argc, argv) { }

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
                    "Please report this error to &nbsp; <a href='mailto:errors@fuzzylite.com'>"
                    "errors@fuzzylite.com</a><br><br>"
                    "Your report will help to make <b>fuzzylite</b> and <b>qtfuzzylite</b> a better "
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
//#ifdef Q_WS_X11
//    return "Linux";
//#endif
//
//#ifdef Q_WS_MAC
//    return "Mac";
//#endif
//
//#ifdef Q_WS_QWS
//    //    return "Embedded Linux";
//    return "Linux";
//#endif
//
//#ifdef Q_WS_WIN
//    return "Windows";
//#endif
//}


int main(int argc, char* argv[]) {
    std::set_terminate(fl::terminate);
    std::set_unexpected(fl::terminate);
    fl::qtfuzzylite = new fl::QtFuzzyLite(argc, argv);
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
        fl::qt::Window::main();
        //int *x = (int*) - 1; // make a bad pointer
        //FL_LOG(*x);
        //        throw 0;
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

