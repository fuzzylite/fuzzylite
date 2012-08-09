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
#include "gui/LinguisticVariableController.h"
#include "gui/GuiLVarWizard.h"

#include "gui/GuiLinguisticTerm.h"
#include "gui/LinguisticTermController.h"
namespace fl_gui {

    LinguisticVariableController::LinguisticVariableController(
            fl::LinguisticVariable& model, GuiLinguisticVariable& view) :
    _model(&model), _view(&view) {
        connect();
    }

    LinguisticVariableController::~LinguisticVariableController() {
        disconnect();
    }

    void LinguisticVariableController::connect() {
        QObject::connect(view().getUi().btn_add_term, SIGNAL(clicked()), this,
                SLOT(onClickAddTerm()));
        QObject::connect(view().getUi().btn_edit_term, SIGNAL(clicked()), this,
                SLOT(onClickEditTerm()));
        QObject::connect(view().getUi().btn_remove_term, SIGNAL(clicked()), this,
                SLOT(onClickRemoveTerm()));
        QObject::connect(view().getUi().btn_wizard, SIGNAL(clicked()), this,
                SLOT(onClickWizard()));
        QObject::connect(view().getUi().lvw_terms, SIGNAL(itemSelectionChanged()), this,
                SLOT(onChangeTerm()));
//        QObject::connect(view().getUi().lvw_terms,
//                SIGNAL(itemDoubleClicked(QListWidgetItem*)), this,
//                SLOT(onDoubleClickTerm(QListWidgetItem*)));
        QObject::connect(view().getUi().led_name, SIGNAL(textChanged(const QString&)), this,
                SLOT(onChangeName(const QString&)));
        refreshModel();
    }

    void LinguisticVariableController::disconnect() {
        QObject::disconnect(view().getUi().btn_add_term, SIGNAL(clicked()), this,
                SLOT(onClickAddTerm()));
        QObject::disconnect(view().getUi().btn_edit_term, SIGNAL(clicked()), this,
                SLOT(onClickEditTerm()));
        QObject::disconnect(view().getUi().btn_remove_term, SIGNAL(clicked()), this,
                SLOT(onClickRemoveTerm()));
        QObject::disconnect(view().getUi().btn_wizard, SIGNAL(clicked()), this,
                SLOT(onClickWizard()));
        QObject::disconnect(view().getUi().lvw_terms, SIGNAL(itemSelectionChanged()),
                this, SLOT(onChangeTerm()));
//        QObject::disconnect(view().getUi().lvw_terms,
//                SIGNAL(itemDoubleClicked(QListWidgetItem*)), this,
//                SLOT(onDoubleClickTerm(QListWidgetItem*)));
        QObject::disconnect(view().getUi().led_name, SIGNAL(textChanged(const QString&)), this,
                SLOT(onChangeName(const QString&)));
    }

    void LinguisticVariableController::refreshModel() {
        view().getUi().led_name->setText(QString::fromStdString(model().name()));
        view().getUi().lvw_terms->clear();
        for (int i = 0; i < model().numberOfTerms(); ++i) {
            QListWidgetItem* item = new QListWidgetItem;
            item->setText(QString::fromStdString(model().term(i)->name()));
            item->setToolTip(QString::fromStdString(model().term(i)->toString()));
            view().getUi().lvw_terms->addItem(item);
        }
        draw();
    }

    fl::LinguisticVariable& LinguisticVariableController::model() const {
        return *this->_model;
    }

    GuiLinguisticVariable& LinguisticVariableController::view() const {
        return *this->_view;
    }

    void LinguisticVariableController::draw() {
        view().getUi().gvw_canvas->clear();

        view().getUi().gvw_canvas->drawVariable(model());
        view().getUi().led_lvar_min->setText(QString::number(model().minimum(), 'f', 3));
        view().getUi().led_lvar_max->setText(QString::number(model().maximum(), 'f', 3));
    }

    void LinguisticVariableController::onClickAddTerm() {
        GuiLinguisticTerm lterm_view;
        lterm_view.setup();
        lterm_view.getUi().led_name->setText(QString::fromStdString(model().name()) + "."
                + QString::number(model().numberOfTerms() + 1));
        if (lterm_view.exec() == QDialog::Accepted) {
            fl::LinguisticTerm* term = lterm_view.selectedTerm();
            model().addTerm(term);
        }
        refreshModel();
    }

    void LinguisticVariableController::onClickEditTerm() {

        QListWidgetItem* selected = view().getUi().lvw_terms->selectedItems().at(0);
        std::string name = selected->text().toStdString();
        fl::LinguisticTerm* lterm = model().term(name);
        GuiLinguisticTerm lterm_view;
        lterm_view.setup();
        lterm_view.showTerm(*lterm);
        lterm_view.updateUi();
        if (lterm_view.exec() == QDialog::Accepted) {
            delete model().removeTerm(name);
            lterm = lterm_view.selectedTerm();
            model().addTerm(lterm);
        }
        refreshModel();
    }

    void LinguisticVariableController::onClickRemoveTerm() {
        for (int i = view().getUi().lvw_terms->count() - 1; i >=0 ; --i) {
            QListWidgetItem* item = view().getUi().lvw_terms->item(i);
            if (item->isSelected()) {
                delete model().removeTerm(i);
            }
        }
        
        refreshModel();
    }

    void LinguisticVariableController::saveModel() {
        //TODO: Implement cancellation of changes of DiscreteTerms...
    }

    void LinguisticVariableController::onClickWizard() {
        GuiLVarWizard wizard(model(), &view());
        wizard.setup();
        if (wizard.exec() == QDialog::Accepted) {

        }
        refreshModel();
    }

    void LinguisticVariableController::onChangeTerm() {
        draw();
        for (int i = 0; i < view().getUi().lvw_terms->count(); ++i) {
            if (view().getUi().lvw_terms->item(i)->isSelected()) {
                QColor color = QColor(Qt::green).darker(125);
                color.setAlpha(230);
                view().getUi().gvw_canvas->drawTerm(*model().term(i), color);
            }
        }
    }

    void LinguisticVariableController::onChangeName(const QString& name) {
        model().setName(name.toStdString());
    }

    void LinguisticVariableController::onDoubleClickTerm(QListWidgetItem* item) {
    	(void)item;
//        item->setSelected(false);
    }


}
