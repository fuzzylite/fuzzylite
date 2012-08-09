/* 
 * File:   MainWindowController.h
 * Author: jcrada
 *
 * Created on November 16, 2009, 3:34 PM
 */

#ifndef _MAINWINDOWCONTROLLER_H
#define	_MAINWINDOWCONTROLLER_H

#include <QtCore/QObject>

#include "GuiMainWindow.h"
#include "InputController.h"
#include "OutputController.h"
#include "RuleController.h"
#include "TestController.h"

using namespace fuzzy_lite;
namespace fuzzy_lite_gui {

    class MainWindowController : public QObject {
        Q_OBJECT
    private:
        FuzzyEngine* _model;
        GuiMainWindow* _view;

        InputController* _input_controller;
        OutputController* _output_controller;
        RuleController* _rule_controller;

        TestController* _test_controller;
        
    protected:
        virtual void connect();
        virtual void disconnect();
    public:
        MainWindowController(FuzzyEngine& model, GuiMainWindow& view);
        virtual ~MainWindowController();
    };
}

#endif	/* _MAINWINDOWCONTROLLER_H */

