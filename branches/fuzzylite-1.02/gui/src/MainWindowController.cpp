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
#include "gui/MainWindowController.h"

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
        QObject::connect(view().getUi().actionApproximation_1x1, SIGNAL(triggered()), this,
                SLOT(loadExample3()));

        QObject::connect(view().getUi().actionSetup, SIGNAL(triggered()),
                this, SLOT(showSetupWindow()));
        QObject::connect(view().getUi().actionTest, SIGNAL(triggered()),
                this, SLOT(showTestWindow()));
        
        QObject::connect(view().getUi().btn_clear, SIGNAL(clicked()),
                this, SLOT(resetModel()));

    }

    void MainWindowController::disconnect() {
        QObject::disconnect(view().getUi().actionSimple_1x1, SIGNAL(triggered()), this,
                SLOT(loadExample1()));
        QObject::disconnect(view().getUi().actionPower_3x1, SIGNAL(triggered()), this,
                SLOT(loadExample2()));
        QObject::disconnect(view().getUi().actionApproximation_1x1, SIGNAL(triggered()), this,
                SLOT(loadExample3()));

        QObject::disconnect(view().getUi().actionSetup, SIGNAL(triggered()),
                this, SLOT(showSetupWindow()));
        QObject::disconnect(view().getUi().actionTest, SIGNAL(triggered()),
                this, SLOT(showTestWindow()));
        QObject::disconnect(view().getUi().btn_clear, SIGNAL(clicked()),
                this, SLOT(resetModel()));
    }

    void MainWindowController::refreshModel() {
        inputController().refreshModel();
        outputController().refreshModel();
        ruleController().refreshModel();
        testController().refreshModel();
        showSetupWindow();
    }

    void MainWindowController::resetModel() {
        model().reset();

        model().fuzzyOperator().setDefuzzifier(new fl::CoGDefuzzifier);
        model().hedgeSet().add(new fl::HedgeNot);
        model().hedgeSet().add(new fl::HedgeSomewhat);
        model().hedgeSet().add(new fl::HedgeVery);

        model().addRuleBlock(new fl::RuleBlock("Rules"));

        refreshModel();
    }

    void MainWindowController::loadExample1() {
        resetModel();

        fl::InputLVar* energy = new fl::InputLVar("Energy");
        energy->addTerm(new fl::ShoulderTerm("LOW", 0.25, 0.5, true));
        energy->addTerm(new fl::TriangularTerm("MEDIUM", 0.25, 0.75));
        energy->addTerm(new fl::ShoulderTerm("HIGH", 0.50, 0.75, false));
        model().addInputLVar(energy);

        fl::OutputLVar* health = new fl::OutputLVar("Health");
        health->addTerm(new fl::TriangularTerm("BAD", 0.0, 0.50));
        health->addTerm(new fl::TriangularTerm("REGULAR", 0.25, 0.75));
        health->addTerm(new fl::TriangularTerm("GOOD", 0.50, 1.00));
        model().addOutputLVar(health);

        fl::MamdaniRule* rule1 = new fl::MamdaniRule();
        fl::MamdaniRule* rule2 = new fl::MamdaniRule();
        fl::MamdaniRule* rule3 = new fl::MamdaniRule();
        rule1->parse("if Energy is LOW then Health is BAD", model());
        rule2->parse("if Energy is MEDIUM then Health is REGULAR", model());
        rule3->parse("if Energy is HIGH then Health is GOOD", model());
        
        model().ruleBlock(0)->addRule(rule1);
        model().ruleBlock(0)->addRule(rule2);
        model().ruleBlock(0)->addRule(rule3);

        refreshModel();
    }

    void MainWindowController::loadExample2() {
        resetModel();

        fl::InputLVar* energy = new fl::InputLVar("Energy");
        energy->addTerm(new fl::TriangularTerm("LOW", 0, 50));
        energy->addTerm(new fl::TriangularTerm("MEDIUM", 25, 75));
        energy->addTerm(new fl::TriangularTerm("HIGH", 50, 100));
        model().addInputLVar(energy);

        fl::InputLVar* distance = new fl::InputLVar("Distance");
        distance->addTerm(new fl::TriangularTerm("NEAR", 0, 500));
        distance->addTerm(new fl::TriangularTerm("FAR", 250, 750));
        distance->addTerm(new fl::TriangularTerm("FAR_AWAY", 500, 1000));
        model().addInputLVar(distance);

        fl::OutputLVar* power = new fl::OutputLVar("Power");
        power->addTerm(new fl::TriangularTerm("LOW", 0, 100));
        power->addTerm(new fl::TriangularTerm("MEDIUM", 50, 150));
        power->addTerm(new fl::TriangularTerm("HIGH", 100, 200));
        model().addOutputLVar(power);

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

        model().ruleBlock(0)->addRule(rule1);
        model().ruleBlock(0)->addRule(rule2);
        model().ruleBlock(0)->addRule(rule3);
        model().ruleBlock(0)->addRule(rule4);
        model().ruleBlock(0)->addRule(rule5);
        model().ruleBlock(0)->addRule(rule6);
        model().ruleBlock(0)->addRule(rule7);
        model().ruleBlock(0)->addRule(rule8);
        model().ruleBlock(0)->addRule(rule9);

        refreshModel();
    }

    void MainWindowController::loadExample3() {
        resetModel();
        model().fuzzyOperator().setDefuzzifier(new fl::TakagiSugenoDefuzzifier);

        fl::InputLVar* var = new fl::InputLVar("Variable");
        var->addTerm(new fl::TriangularTerm("NEAR_1", 0, 2));
        var->addTerm(new fl::TriangularTerm("NEAR_2", 1, 3));
        var->addTerm(new fl::TriangularTerm("NEAR_3", 2, 4));
        var->addTerm(new fl::TriangularTerm("NEAR_4", 3, 5));
        var->addTerm(new fl::TriangularTerm("NEAR_5", 4, 6));
        var->addTerm(new fl::TriangularTerm("NEAR_6", 5, 7));
        var->addTerm(new fl::TriangularTerm("NEAR_7", 6, 8));
        var->addTerm(new fl::TriangularTerm("NEAR_8", 7, 9));
        var->addTerm(new fl::TriangularTerm("NEAR_9", 8, 10));
        model().addInputLVar(var);

        fl::OutputLVar* health = new fl::OutputLVar("f");
        health->addTerm(new fl::FunctionTerm("function", "(sin x) / x", 0, 10));
        model().addOutputLVar(health);

        model().ruleBlock(0)->addRule(new fl::TakagiSugenoRule("if Variable is NEAR_1 then f=0.84", model()));
        model().ruleBlock(0)->addRule(new fl::TakagiSugenoRule("if Variable is NEAR_2 then f=0.45", model()));
        model().ruleBlock(0)->addRule(new fl::TakagiSugenoRule("if Variable is NEAR_3 then f=0.04", model()));
        model().ruleBlock(0)->addRule(new fl::TakagiSugenoRule("if Variable is NEAR_4 then f=-0.18", model()));
        model().ruleBlock(0)->addRule(new fl::TakagiSugenoRule("if Variable is NEAR_5 then f=-0.19", model()));
        model().ruleBlock(0)->addRule(new fl::TakagiSugenoRule("if Variable is NEAR_6 then f=-0.04", model()));
        model().ruleBlock(0)->addRule(new fl::TakagiSugenoRule("if Variable is NEAR_7 then f=0.09", model()));
        model().ruleBlock(0)->addRule(new fl::TakagiSugenoRule("if Variable is NEAR_8 then f=0.12", model()));
        model().ruleBlock(0)->addRule(new fl::TakagiSugenoRule("if Variable is NEAR_9 then f=0.04", model()));

        refreshModel();
    }

    void MainWindowController::showSetupWindow() {
        view().getUi().tab_container->setCurrentIndex(0);
    }

    void MainWindowController::showTestWindow() {
        view().getUi().tab_container->setCurrentIndex(1);
    }

}
