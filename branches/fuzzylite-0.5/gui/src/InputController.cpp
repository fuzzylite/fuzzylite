#include "InputController.h"
#include "ui_LinguisticVariable.h"
#include "GuiLinguisticVariable.h"
#include "LinguisticVariableController.h"

#include <InputLVar.h>
#include <QtGui/QListWidgetItem>
namespace fuzzy_lite_gui {

    InputController::InputController(FuzzyEngine& model, GuiMainWindow& view)
    : _model(&model), _view(&view) {
        connect();
    }

    InputController::~InputController() {
        disconnect();
    }

    void InputController::connect() {
        QObject::connect(getView().getUi().btn_add_input, SIGNAL(clicked()), this, SLOT(onClickAddInput()));
        QObject::connect(getView().getUi().btn_remove_input, SIGNAL(clicked()), this, SLOT(onClickRemoveInput()));
        QObject::connect(getView().getUi().btn_edit_input, SIGNAL(clicked()), this, SLOT(onClickEditInput()));
        for (int i = 0 ; i < getModel().numberOfInputLVars() ; ++i){
            QListWidgetItem* item = new QListWidgetItem();
            item->setText(QString::fromStdString(getModel().getInputLVar(i).getName()));
            item->setToolTip(QString::fromStdString(getModel().getInputLVar(i).toString()));
            getView().getUi().lvw_inputs->addItem(item);
        }
    }

    void InputController::disconnect() {
        QObject::disconnect(getView().getUi().btn_add_input, SIGNAL(clicked()), this, SLOT(onClickAddInput()));
        QObject::disconnect(getView().getUi().btn_remove_input, SIGNAL(clicked()), this, SLOT(onClickRemoveInput()));
        QObject::disconnect(getView().getUi().btn_edit_input, SIGNAL(clicked()), this, SLOT(onClickEditInput()));

    }

    FuzzyEngine& InputController::getModel() const {
        return *this->_model;
    }

    GuiMainWindow& InputController::getView() const {
        return *this->_view;
    }

    void InputController::onClickAddInput() {
        InputLVar* model = new InputLVar();
        GuiLinguisticVariable view;
        view.setup();
        LinguisticVariableController c(*model, view);
        if (view.exec() == QDialog::Accepted) {
            c.saveModel();
            getModel().addInputLVar(*model);
            QListWidgetItem* item = new QListWidgetItem();
            item->setText(QString::fromStdString(model->getName()));
            item->setToolTip(QString::fromStdString(model->toString()));
            getView().getUi().lvw_inputs->addItem(item);
        } else {
            delete model;
        }

    }

    void InputController::onClickRemoveInput() {
        QListWidgetItem* selected = getView().getUi().lvw_inputs->selectedItems().at(0);
        std::string item = selected->text().toStdString();
        InputLVar* input = getModel().removeInputLVar(item);
        std::vector<FuzzyRule*> rules = getModel().removeAllRules();
        for (int i = 0; i < rules.size(); ++i) {
            delete rules[i];
        }
        delete input;

        getView().getUi().lvw_inputs->removeItemWidget(selected);
        delete selected;
    }

    void InputController::onClickEditInput() {
        QListWidgetItem* selected = getView().getUi().lvw_inputs->selectedItems().at(0);
        std::string item = selected->text().toStdString();
        InputLVar* input = getModel().getInputLVar(item);
        GuiLinguisticVariable view;
        view.setup();
        LinguisticVariableController c(*input, view);
        if (view.exec() == QDialog::Accepted) {
            c.saveModel();
            selected->setText(QString::fromStdString(input->getName()));
            selected->setToolTip(QString::fromStdString(input->toString()));
        }
    }


}
