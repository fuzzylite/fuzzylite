#include "MainWindowController.h"

namespace fuzzy_lite_gui {

    MainWindowController::MainWindowController(FuzzyEngine& model, GuiMainWindow& view)
    : _model(&model), _view(&view),
    _input_controller(new InputController(model, view)),
    _output_controller(new OutputController(model, view)),
    _rule_controller(new RuleController(model, view)),
    _test_controller(new TestController(model, view)) {
        connect();
    }

    MainWindowController::~MainWindowController() {
        delete _test_controller;
        delete _rule_controller;
        delete _output_controller;
        delete _input_controller;
        disconnect();
    }

    void MainWindowController::connect() {

    }

    void MainWindowController::disconnect() {

    }
}
