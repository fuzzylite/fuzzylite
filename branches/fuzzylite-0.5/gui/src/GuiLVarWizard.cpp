#include <QtGui/qmessagebox.h>

#include "GuiLVarWizard.h"

namespace fuzzy_lite_gui {

    GuiLVarWizard::GuiLVarWizard(LinguisticVariable& model, QWidget* parent, Qt::WindowFlags f)
    : QDialog(parent, f), _model(&model), _ui(new Ui::LinguisticVariableWizard) {

    }

    GuiLVarWizard::~GuiLVarWizard() {
        disconnect();
        delete _ui;
    }

    LinguisticVariable& GuiLVarWizard::getModel() const {
        return *this->_model;
    }

    Ui::LinguisticVariableWizard& GuiLVarWizard::getUi() const {
        return *this->_ui;
    }

    void GuiLVarWizard::setup() {
        getUi().setupUi(this);
        connect();
    }

    void GuiLVarWizard::connect() {
        QObject::connect(getUi().dsx_min, SIGNAL(valueChanged(double)),
                this, SLOT(onChangeMinimum(double)));
        QObject::connect(getUi().dsx_max, SIGNAL(valueChanged(double)),
                this, SLOT(onChangeMaximum(double)));
        QObject::connect(getUi().btn_create_labels, SIGNAL(clicked()),
                this, SLOT(onClickCreateLabels()));
    }

    void GuiLVarWizard::disconnect() {
        QObject::disconnect(getUi().dsx_min, SIGNAL(valueChanged(double)),
                this, SLOT(onChangeMinimum(double)));
        QObject::disconnect(getUi().dsx_max, SIGNAL(valueChanged(double)),
                this, SLOT(onChangeMaximum(double)));
        QObject::disconnect(getUi().btn_create_labels, SIGNAL(clicked()),
                this, SLOT(onClickCreateLabels()));
    }

    void GuiLVarWizard::onChangeMinimum(double min) {
        if (getUi().dsx_max->value() < min) {
            getUi().dsx_max->setValue(min);
        }
    }

    void GuiLVarWizard::onChangeMaximum(double max) {
        if (getUi().dsx_min->value() > max) {
            getUi().dsx_min->setValue(max);
        }
    }

    void GuiLVarWizard::onClickCreateLabels(){
        getUi().ptx_term_labels->clear();
        for (int i = 1 ; i <= getUi().spx_terms->value(); ++i){
            getUi().ptx_term_labels->appendPlainText(QString::number(i));
        }
    }

    void GuiLVarWizard::accept() {
        QStringList qt_labels = getUi().ptx_term_labels->toPlainText().split("\n", QString::SkipEmptyParts);
        if (qt_labels.size() != getUi().spx_terms->value()) {
            QMessageBox::critical(this, "Error", "The number of labels does not match the number of terms",
                    QMessageBox::Ok, QMessageBox::Ok);
            return;
        }
        bool from_infinity = getUi().chx_from_infinity->isChecked();
        bool to_infinity = getUi().chx_to_infinity->isChecked();
        LinguisticVariable::MEMBERSHIP_FUNCTION mf = getUi().rbt_triangular->isChecked() ?
                LinguisticVariable::MF_TRIANGULAR : LinguisticVariable::MF_TRAPEZOIDAL;
        flScalar min = (flScalar) getUi().dsx_min->value();
        flScalar max = (flScalar) getUi().dsx_max->value();
        int terms = getUi().spx_terms->value();
        std::vector<std::string> labels;
        for (int i = 0; i < qt_labels.size(); ++i) {
            labels.push_back(qt_labels.at(i).toStdString());
        }
        getModel().autoCreateTerms(min, max, terms, labels, mf, from_infinity, to_infinity);
        done(QDialog::Accepted);
    }
}
