#include "GuiLinguisticVariable.h"

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
                getUi().lvw_terms->selectedItems().size() > 0);
        getUi().btn_remove_term->setEnabled(
                getUi().lvw_terms->selectedItems().size() > 0);
    }

    void GuiLinguisticVariable::onDoubleClickDiscreteItem(QListWidgetItem*) {
        getUi().btn_edit_term->click();
    }


}
