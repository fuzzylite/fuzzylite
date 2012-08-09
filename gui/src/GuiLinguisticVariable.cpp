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
#include "gui/GuiLinguisticVariable.h"

#include <QtGui/qerrormessage.h>
#include <QtGui/qmessagebox.h>

namespace fl_gui {

    GuiLinguisticVariable::GuiLinguisticVariable(QWidget* parent, Qt::WindowFlags f)
    : QDialog(parent, f), _ui(new Ui::LinguisticVariable) {
    }

    GuiLinguisticVariable::~GuiLinguisticVariable() {
        disconnect();
        delete _ui;
    }

    Ui::LinguisticVariable& GuiLinguisticVariable::getUi() const {
        return *this->_ui;
    }

    void GuiLinguisticVariable::setup() {
        getUi().setupUi(this);
        connect();
    }

    void GuiLinguisticVariable::connect() {
        QObject::connect(getUi().lvw_terms, SIGNAL(itemSelectionChanged()),
                this, SLOT(onChangeTermSelection()));
    }

    void GuiLinguisticVariable::disconnect() {
        QObject::disconnect(getUi().lvw_terms, SIGNAL(itemSelectionChanged()),
                this, SLOT(onChangeTermSelection()));
    }

    void GuiLinguisticVariable::accept() {
        getUi().led_name->setText(getUi().led_name->text().trimmed());
        if (getUi().led_name->text().size() <= 0) {
            QMessageBox::critical(this, "Error",
                    "The name cannot be empty", QMessageBox::Ok, QMessageBox::Ok);
            return;
        }
        done(QDialog::Accepted);
    }

    void GuiLinguisticVariable::onChangeTermSelection() {
        getUi().btn_edit_term->setEnabled(
                getUi().lvw_terms->selectedItems().size() == 1);
        getUi().btn_remove_term->setEnabled(
                getUi().lvw_terms->selectedItems().size() > 0);
    }

    void GuiLinguisticVariable::onDoubleClickDiscreteItem(QListWidgetItem*) {
        getUi().btn_edit_term->click();
    }


}
