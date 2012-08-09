/* 
 * File:   main.cpp
 * Author: jcrada
 *
 * Created on November 14, 2009, 12:32 AM
 */

#include <QtGui/QApplication>

#include "InputController.h"
#include "MainWindowController.h"
#include "GuiGrapher.h"
#include <fuzzylite/FuzzyLite.h>

using namespace fl_gui;
using namespace fl;

void ApplicationCatch(int signal) {
    FL_LOG("Application Catch: " << signal);
}

int main2(int argc, char** argv) {
    int result;
    QApplication app(argc, argv);
    //    void (*handler)(int);
    //    handler = signal(SIGTERM,ApplicationCatch);
    try {
        FuzzyEngine model(FuzzyOperator::DefaultFuzzyOperator());
        GuiMainWindow view;
        view.setup();
        MainWindowController c(model, view);
        view.show();
        result = app.exec();
    } catch (std::exception& e) {
        FL_LOG(e.what());
        return EXIT_FAILURE;
    }
    FL_LOG("RESULT: " << result);
    return result;
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    FuzzyEngine engine(FuzzyOperator::DefaultFuzzyOperator());

    GuiMainWindow view;
    view.setup();
    MainWindowController c(engine, view);
    view.show();
    return app.exec();
}

int mainAggregate(int argc, char** argv) {
    QApplication app(argc, argv);
//    DiscreteTerm a("a");
//    a.addCoord(0, 0);
//    a.addCoord(0.3, 0.8);
//    a.addCoord(0.6, 0.8);
//    a.addCoord(1.0, 0);
//
//    DiscreteTerm b("b");
//    b.addCoord(0.4, 0);
//    b.addCoord(0.7, 0.4);
//    b.addCoord(0.9, 0.4);
//    b.addCoord(1.0, 0);
//
//
////    flScalar ix,iy;
////    FL_LOG("(0.6,0.8),(1.0,0), (0.7,0.4),(0.9,0.4)")
////    FL_LOG("INTER " << DiscreteTerm::Intersects(0.6,0.8,1.0,0, 0.7,0.4,0.9,0.4,ix,iy));
////    FL_LOG("x=" << ix << " y=" << iy);
////    return 0;
//
//    OutputLVar x("");
//    x.addTerm(a);
//    x.addTerm(b);
//    x.aggregateToOutput(a,FuzzyOperator::DefaultFuzzyOperator());
//    x.aggregateToOutput(b,FuzzyOperator::DefaultFuzzyOperator());
////    return 0;
//    FL_LOG("ooooooooooo");

//    GuiGrapher g(x);
//    g.setup();
//    g.show();
    //    a.aggregate(b);
    return app.exec();

}
