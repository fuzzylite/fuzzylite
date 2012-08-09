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
#include "gui/InputController.h"
#include "gui/ui_LinguisticVariable.h"
#include "gui/GuiLinguisticVariable.h"
#include "gui/LinguisticVariableController.h"

#include <QtGui/QListWidgetItem>

namespace fl_gui {

    InputController::InputController(fl::FuzzyEngine& model, GuiMainWindow& view) :
    _model(&model), _view(&view) {
        connect();
    }

    InputController::~InputController() {
        disconnect();
    }

    void InputController::connect() {
        QObject::connect(getView().getUi().btn_add_input, SIGNAL(clicked()), this,
                SLOT(onClickAddInput()));
        QObject::connect(getView().getUi().btn_remove_input, SIGNAL(clicked()), this,
                SLOT(onClickRemoveInput()));
        QObject::connect(getView().getUi().btn_edit_input, SIGNAL(clicked()), this,
                SLOT(onClickEditInput()));
        refreshModel();
    }

    void InputController::disconnect() {
        QObject::disconnect(getView().getUi().btn_add_input, SIGNAL(clicked()), this,
                SLOT(onClickAddInput()));
        QObject::disconnect(getView().getUi().btn_remove_input, SIGNAL(clicked()), this,
                SLOT(onClickRemoveInput()));
        QObject::disconnect(getView().getUi().btn_edit_input, SIGNAL(clicked()), this,
                SLOT(onClickEditInput()));

    }

    void InputController::refreshModel() {
        getView().getUi().lvw_inputs->clear();
        for (int i = 0; i < getModel().numberOfInputLVars(); ++i) {
            QListWidgetItem* item = new QListWidgetItem();
            item->setText(QString::fromStdString(getModel().inputLVar(i)->name()));
            item->setToolTip(QString::fromStdString(getModel().inputLVar(i)->toString()));
            getView().getUi().lvw_inputs->addItem(item);
        }
    }

    fl::FuzzyEngine& InputController::getModel() const {
        return *this->_model;
    }

    GuiMainWindow& InputController::getView() const {
        return *this->_view;
    }

    void InputController::onClickAddInput() {
        fl::InputLVar* model = new fl::InputLVar();
        GuiLinguisticVariable view;
        view.setup();
        view.getUi().led_name->setText("LinguisticVariable" + QString::number(
                getModel().numberOfInputLVars()));
        LinguisticVariableController c(*model, view);
        if (view.exec() == QDialog::Accepted) {
            c.saveModel();
            getModel().addInputLVar(model);
            QListWidgetItem* item = new QListWidgetItem();
            item->setText(QString::fromStdString(model->name()));
            item->setToolTip(QString::fromStdString(model->toString()));
            getView().getUi().lvw_inputs->addItem(item);
        } else {
            delete model;
        }

    }

    void InputController::onClickRemoveInput() {
        QListWidgetItem* selected = getView().getUi().lvw_inputs->selectedItems().at(0);
        std::string item = selected->text().toStdString();
        fl::InputLVar* input = getModel().removeInputLVar(item);
        getModel().ruleBlock(0)->reset();
        delete input;

        getView().getUi().lvw_inputs->removeItemWidget(selected);
        delete selected;
    }

    void InputController::onClickEditInput() {
        QListWidgetItem* selected = getView().getUi().lvw_inputs->selectedItems().at(0);
        std::string item = selected->text().toStdString();
        fl::InputLVar* input = getModel().inputLVar(item);
        GuiLinguisticVariable view;
        view.setup();
        LinguisticVariableController c(*input, view);
        if (view.exec() == QDialog::Accepted) {
            c.saveModel();
            selected->setText(QString::fromStdString(input->name()));
            selected->setToolTip(QString::fromStdString(input->toString()));
        }
    }

}
