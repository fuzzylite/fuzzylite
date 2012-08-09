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

#include <fuzzylite/FuzzyLite.h>

namespace fl_gui {

	class MainWindowController: public QObject {
	Q_OBJECT
	protected slots:
		virtual void loadExample1();
		virtual void loadExample2();
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

