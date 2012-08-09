/*   Copyright 2010 Juan Rada-Vilela

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */
/* 
 * File:   main.cpp
 * Author: jcrada
 *
 * Created on November 14, 2009, 12:32 AM
 */

#include <QtGui/QApplication>

#include "gui/InputController.h"
#include "gui/MainWindowController.h"
#include "gui/GuiGrapher.h"
#include <fuzzylite/FuzzyLite.h>
#include <QtCore/qdir.h>

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
        FuzzyEngine model("",FuzzyOperator::DefaultFuzzyOperator());
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
//    FL_LOG("Test");
    QApplication app(argc, argv);

    FuzzyEngine engine;
    engine.addRuleBlock(new RuleBlock("Rules"));
    engine.hedgeSet().add(new HedgeAny);
    engine.hedgeSet().add(new HedgeNot);
    engine.hedgeSet().add(new HedgeSomewhat);
    engine.hedgeSet().add(new HedgeVery);
    GuiMainWindow view;
    view.setup();
    MainWindowController c(engine, view);
    view.show();
    return app.exec();
}

