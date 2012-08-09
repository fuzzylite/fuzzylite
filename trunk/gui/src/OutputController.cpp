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
#include "gui/OutputController.h"
#include "gui/ui_LinguisticVariable.h"
#include "gui/GuiLinguisticVariable.h"
#include "gui/LinguisticVariableController.h"

#include <QtGui/QListWidgetItem>
namespace fl_gui {

    OutputController::OutputController(fl::FuzzyEngine& model, GuiMainWindow& view) :
    _model(&model), _view(&view) {
        connect();
    }

    OutputController::~OutputController() {
        disconnect();
    }

    void OutputController::connect() {
        QObject::connect(view().getUi().btn_add_output, SIGNAL(clicked()), this,
                SLOT(onClickAddOutput()));
        QObject::connect(view().getUi().btn_remove_output, SIGNAL(clicked()), this,
                SLOT(onClickRemoveOutput()));
        QObject::connect(view().getUi().btn_edit_output, SIGNAL(clicked()), this,
                SLOT(onClickEditOutput()));
        refreshModel();
    }

    void OutputController::disconnect() {
        QObject::disconnect(view().getUi().btn_add_output, SIGNAL(clicked()), this,
                SLOT(onClickAddOutput()));
        QObject::disconnect(view().getUi().btn_remove_output, SIGNAL(clicked()), this,
                SLOT(onClickRemoveOutput()));
        QObject::disconnect(view().getUi().btn_edit_output, SIGNAL(clicked()), this,
                SLOT(onClickEditOutput()));
    }

    void OutputController::refreshModel() {
        view().getUi().lvw_outputs->clear();
        for (int i = 0; i < model().numberOfOutputLVars(); ++i) {
            QListWidgetItem* item = new QListWidgetItem();
            item->setText(QString::fromStdString(model().outputLVar(i)->name()));
            item->setToolTip(QString::fromStdString(model().outputLVar(i)->toString()));
            view().getUi().lvw_outputs->addItem(item);
        }
    }

    fl::FuzzyEngine& OutputController::model() const {
        return *this->_model;
    }

    GuiMainWindow& OutputController::view() const {
        return *this->_view;
    }

    void OutputController::onClickAddOutput() {
        fl::OutputLVar* lvar = new fl::OutputLVar();
        GuiLinguisticVariable lvar_view;
        lvar_view.setup();
        LinguisticVariableController c(*lvar, lvar_view);
        if (lvar_view.exec() == QDialog::Accepted) {
            c.saveModel();
            model().addOutputLVar(lvar);
            QListWidgetItem* item = new QListWidgetItem();
            item->setText(QString::fromStdString(lvar->name()));
            item->setToolTip(QString::fromStdString(lvar->toString()));
            view().getUi().lvw_outputs->addItem(item);
        } else {
            delete lvar;
        }
    }

    void OutputController::onClickRemoveOutput() {
        QListWidgetItem* selected = view().getUi().lvw_outputs->selectedItems().at(0);
        std::string item = selected->text().toStdString();
        fl::OutputLVar* output = model().removeOutputLVar(item);
        model().ruleBlock(0)->reset();
        delete output;

        view().getUi().lvw_outputs->removeItemWidget(selected);
        delete selected;
    }

    void OutputController::onClickEditOutput() {
        QListWidgetItem* selected = view().getUi().lvw_outputs->selectedItems().at(0);
        std::string item = selected->text().toStdString();
        fl::OutputLVar* lvar = model().outputLVar(item);
        GuiLinguisticVariable lvar_view;
        lvar_view.setup();
        LinguisticVariableController c(*lvar, lvar_view);
        if (lvar_view.exec() == QDialog::Accepted) {
            c.saveModel();
            selected->setText(QString::fromStdString(lvar->name()));
            selected->setToolTip(QString::fromStdString(lvar->toString()));
        }
    }

}
