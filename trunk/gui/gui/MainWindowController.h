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
 * File:   MainWindowController.h
 * Author: jcrada
 *
 * Created on November 16, 2009, 3:34 PM
 */

#ifndef _MAINWINDOWCONTROLLER_H
#define	_MAINWINDOWCONTROLLER_H

#include <QtCore/QObject>

#include "gui/GuiMainWindow.h"
#include "gui/InputController.h"
#include "gui/OutputController.h"
#include "gui/RuleController.h"
#include "gui/TestController.h"

#include <fuzzylite/FuzzyLite.h>

namespace fl_gui {

    class MainWindowController : public QObject {
        Q_OBJECT
    protected slots:
        virtual void resetModel();
        virtual void loadExample1();
        virtual void loadExample2();
        virtual void loadExample3();

        virtual void showSetupWindow();
        virtual void showTestWindow();
    private:
        fl::FuzzyEngine* _model;
        GuiMainWindow* _view;

        InputController* _input_controller;
        OutputController* _output_controller;
        RuleController* _rule_controller;

        TestController* _test_controller;

    protected:
        virtual void connect();
        virtual void disconnect();
        

    public:
        MainWindowController(fl::FuzzyEngine& model, GuiMainWindow& view);
        virtual ~MainWindowController();

        virtual fl::FuzzyEngine& model() const;
        virtual GuiMainWindow& view() const;

        virtual InputController& inputController() const;
        virtual OutputController& outputController() const;
        virtual RuleController& ruleController() const;
        virtual TestController& testController() const;

        virtual void refreshModel();
        

    };
}

#endif	/* _MAINWINDOWCONTROLLER_H */

