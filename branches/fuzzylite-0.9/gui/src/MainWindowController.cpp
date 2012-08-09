#include "MainWindowController.h"

namespace fl_gui {

	MainWindowController::MainWindowController(fl::FuzzyEngine& model,
		GuiMainWindow& view) :
		_model(&model), _view(&view),
			_input_controller(new InputController(model, view)), _output_controller(
				new OutputController(model, view)), _rule_controller(new RuleController(
				model, view)), _test_controller(new TestController(model, view)) {
		connect();
	}

	MainWindowController::~MainWindowController() {
		delete _test_controller;
		delete _rule_controller;
		delete _output_controller;
		delete _input_controller;
		disconnect();
	}

	fl::FuzzyEngine& MainWindowController::model() const {
		return *this->_model;
	}

	GuiMainWindow& MainWindowController::view() const {
		return *this->_view;
	}

	InputController& MainWindowController::inputController() const {
		return *this->_input_controller;
	}

	OutputController& MainWindowController::outputController() const {
		return *this->_output_controller;
	}

	RuleController& MainWindowController::ruleController() const {
		return *this->_rule_controller;
	}

	TestController& MainWindowController::testController() const {
		return *this->_test_controller;
	}

	void MainWindowController::connect() {
		QObject::connect(view().getUi().actionSimple_1x1, SIGNAL(triggered()), this,
			SLOT(loadExample1()));
		QObject::connect(view().getUi().actionPower_3x1, SIGNAL(triggered()), this,
			SLOT(loadExample2()));
	}

	void MainWindowController::disconnect() {
		QObject::disconnect(view().getUi().actionSimple_1x1, SIGNAL(triggered()), this,
			SLOT(loadExample1()));
		QObject::disconnect(view().getUi().actionPower_3x1, SIGNAL(triggered()), this,
			SLOT(loadExample2()));
	}

	void MainWindowController::refreshModel() {
		inputController().refreshModel();
		outputController().refreshModel();
		ruleController().refreshModel();
		view().getUi().tlx_main->setCurrentIndex(0);
	}

	void MainWindowController::loadExample1() {
		model().hardReset();

		model().addHedge(*new fl::HedgeNot);
		model().addHedge(*new fl::HedgeSomewhat);
		model().addHedge(*new fl::HedgeVery);

		fl::InputLVar* energy = new fl::InputLVar("Energy");
		energy->addTerm(*new fl::ShoulderTerm("LOW", 0.25, 0.5, true));
		energy->addTerm(*new fl::TriangularTerm("MEDIUM", 0.25, 0.75));
		energy->addTerm(*new fl::ShoulderTerm("HIGH", 0.50, 0.75, false));
		model().addInputLVar(*energy);

		fl::OutputLVar* health = new fl::OutputLVar("Health");
		health->addTerm(*new fl::TriangularTerm("BAD", 0.0, 0.50));
		health->addTerm(*new fl::TriangularTerm("REGULAR", 0.25, 0.75));
		health->addTerm(*new fl::TriangularTerm("GOOD", 0.50, 1.00));
		model().addOutputLVar(*health);

		fl::MamdaniRule* rule1 = new fl::MamdaniRule();
		fl::MamdaniRule* rule2 = new fl::MamdaniRule();
		fl::MamdaniRule* rule3 = new fl::MamdaniRule();
		rule1->parse("if Energy is LOW then Health is BAD", model());
		rule2->parse("if Energy is MEDIUM then Health is REGULAR", model());
		rule3->parse("if Energy is HIGH then Health is GOOD", model());

		model().addRule(*rule1);
		model().addRule(*rule2);
		model().addRule(*rule3);

		refreshModel();
	}

	void MainWindowController::loadExample2() {
		model().hardReset();

		model().addHedge(*new fl::HedgeNot);
		model().addHedge(*new fl::HedgeSomewhat);
		model().addHedge(*new fl::HedgeVery);

		fl::InputLVar* energy = new fl::InputLVar("Energy");
		energy->addTerm(*new fl::TriangularTerm("LOW", 0, 50));
		energy->addTerm(*new fl::TriangularTerm("MEDIUM", 25, 75));
		energy->addTerm(*new fl::TriangularTerm("HIGH", 50, 100));
		model().addInputLVar(*energy);

		fl::InputLVar* distance = new fl::InputLVar("Distance");
		distance->addTerm(*new fl::TriangularTerm("NEAR", 0, 500));
		distance->addTerm(*new fl::TriangularTerm("FAR", 250, 750));
		distance->addTerm(*new fl::TriangularTerm("FAR_AWAY", 500, 1000));
		model().addInputLVar(*distance);

		fl::OutputLVar* power = new fl::OutputLVar("Power");
		power->addTerm(*new fl::TriangularTerm("LOW", 0, 100));
		power->addTerm(*new fl::TriangularTerm("MEDIUM", 50, 150));
		power->addTerm(*new fl::TriangularTerm("HIGH", 100, 200));
		model().addOutputLVar(*power);

		fl::MamdaniRule* rule1 = new fl::MamdaniRule;
		fl::MamdaniRule* rule2 = new fl::MamdaniRule;
		fl::MamdaniRule* rule3 = new fl::MamdaniRule;
		fl::MamdaniRule* rule4 = new fl::MamdaniRule;
		fl::MamdaniRule* rule5 = new fl::MamdaniRule;
		fl::MamdaniRule* rule6 = new fl::MamdaniRule;
		fl::MamdaniRule* rule7 = new fl::MamdaniRule;
		fl::MamdaniRule* rule8 = new fl::MamdaniRule;
		fl::MamdaniRule* rule9 = new fl::MamdaniRule;

		rule1->parse("if Energy is LOW and Distance is FAR_AWAY then Power is LOW",
			model());
		rule2->parse("if Energy is LOW and Distance is FAR then Power is MEDIUM", model());
		rule3->parse("if Energy is LOW and Distance is NEAR then Power is HIGH", model());
		rule4->parse("if Energy is MEDIUM and Distance is FAR_AWAY then Power is LOW",
			model());
		rule5->parse("if Energy is MEDIUM and Distance is FAR then Power is MEDIUM",
			model());
		rule6->parse("if Energy is MEDIUM and Distance is NEAR then Power is HIGH",
			model());
		rule7->parse("if Energy is HIGH and Distance is FAR_AWAY then Power is LOW",
			model());
		rule8->parse("if Energy is HIGH and Distance is FAR then Power is MEDIUM",
			model());
		rule9->parse("if Energy is HIGH and Distance is NEAR then Power is HIGH", model());

		model().addRule(*rule1);
		model().addRule(*rule2);
		model().addRule(*rule3);
		model().addRule(*rule4);
		model().addRule(*rule5);
		model().addRule(*rule6);
		model().addRule(*rule7);
		model().addRule(*rule8);
		model().addRule(*rule9);

		refreshModel();
	}

}
