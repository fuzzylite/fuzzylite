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
#include <FuzzyLite.h>

using namespace fuzzy_lite_gui;
using namespace fuzzy_lite;

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

    InputLVar energy("Energy");
    TriangularDTerm low_energy("LOW", 0, 50);
    //    low_energy.setFromInfinity(true);
    TriangularDTerm med_energy("MEDIUM", 25, 75);
    TriangularDTerm hi_energy("HIGH", 50, 100);
    //    hi_energy.setToInfinity(true);
    energy.addTerm(low_energy);
    energy.addTerm(med_energy);
    energy.addTerm(hi_energy);
    engine.addInputLVar(energy);

    OutputLVar health("Health");
    TriangularDTerm bad_health("BAD", 0, 50);
    TriangularDTerm reg_health("REGULAR", 25, 75);
    TriangularDTerm good_health("GOOD", 50, 100);
    health.addTerm(bad_health);
    health.addTerm(reg_health);
    health.addTerm(good_health);
    engine.addOutputLVar(health);

    MamdaniRule rule1;
    MamdaniRule rule2;
    MamdaniRule rule3;

    rule1.parse("if Energy is LOW then Health is BAD", engine);
    rule2.parse("if Energy is MEDIUM then Health is REGULAR", engine);
    rule3.parse("if Energy is HIGH then Health is GOOD", engine);

    engine.addRule(rule1);
    engine.addRule(rule2);
    engine.addRule(rule3);



    HedgeNot h_not;
    HedgeSomewhat h_somewhat;
    HedgeVery h_very;
    engine.addHedge(h_not);
    engine.addHedge(h_somewhat);
    engine.addHedge(h_very);

    GuiMainWindow view;
    view.setup();
    MainWindowController c(engine, view);
    view.show();
    return app.exec();
}

int mainAggregate(int argc, char** argv) {
    QApplication app(argc, argv);
    DiscreteTerm a("a");
    a.addCoord(0, 0);
    a.addCoord(0.3, 0.8);
    a.addCoord(0.6, 0.8);
    a.addCoord(1.0, 0);

    DiscreteTerm b("b");
    b.addCoord(0.4, 0);
    b.addCoord(0.7, 0.4);
    b.addCoord(0.9, 0.4);
    b.addCoord(1.0, 0);


    flScalar ix,iy;
//    FL_LOG("(0.6,0.8),(1.0,0), (0.7,0.4),(0.9,0.4)")
//    FL_LOG("INTER " << DiscreteTerm::Intersects(0.6,0.8,1.0,0, 0.7,0.4,0.9,0.4,ix,iy));
//    FL_LOG("x=" << ix << " y=" << iy);
//    return 0;

    OutputLVar x("");
    x.addTerm(a);
    x.addTerm(b);
    x.aggregateToOutput(a);
    x.aggregateToOutput(b);
//    return 0;
    FL_LOG("ooooooooooo");

    GuiGrapher g(x);
    g.setup();
    g.show();
    //    a.aggregate(b);
    return app.exec();

}
