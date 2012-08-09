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
#include <QtGui/qmessagebox.h>

#include "gui/GuiLVarWizard.h"

namespace fl_gui {

    GuiLVarWizard::GuiLVarWizard(fl::LinguisticVariable& model, QWidget* parent,
            Qt::WindowFlags f) :
    QDialog(parent, f), _model(&model), _ui(new Ui::LinguisticVariableWizard) {

    }

    GuiLVarWizard::~GuiLVarWizard() {
        disconnect();
        delete _ui;
    }

    fl::LinguisticVariable& GuiLVarWizard::getModel() const {
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
        QObject::connect(getUi().dsx_min, SIGNAL(valueChanged(double)), this,
                SLOT(onChangeMinimum(double)));
        QObject::connect(getUi().dsx_max, SIGNAL(valueChanged(double)), this,
                SLOT(onChangeMaximum(double)));
        QObject::connect(getUi().btn_create_labels, SIGNAL(clicked()), this,
                SLOT(onClickCreateLabels()));
    }

    void GuiLVarWizard::disconnect() {
        QObject::disconnect(getUi().dsx_min, SIGNAL(valueChanged(double)), this,
                SLOT(onChangeMinimum(double)));
        QObject::disconnect(getUi().dsx_max, SIGNAL(valueChanged(double)), this,
                SLOT(onChangeMaximum(double)));
        QObject::disconnect(getUi().btn_create_labels, SIGNAL(clicked()), this,
                SLOT(onClickCreateLabels()));
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

    void GuiLVarWizard::onClickCreateLabels() {
        getUi().ptx_term_labels->clear();
        for (int i = 1; i <= getUi().spx_terms->value(); ++i) {
            getUi().ptx_term_labels->appendPlainText(QString::fromStdString(
                    getModel().name()) + "." + QString::number(i));
        }
    }

    void GuiLVarWizard::accept() {
        fl::flScalar min = getUi().dsx_min->value();
        fl::flScalar max = getUi().dsx_max->value();
        if (fl::FuzzyOperation::IsEq(max - min, 0)) {
            QMessageBox::critical(this, "Error",
                    "Minimum and maximum values must be different", QMessageBox::Ok,
                    QMessageBox::Ok);
            return;
        }

        int terms = getUi().spx_terms->value();

        QStringList labels = getUi().ptx_term_labels->toPlainText().split("\n",
                QString::SkipEmptyParts);
        if (labels.count() != terms) {
            QMessageBox::critical(this, "Error",
                    "The amount of terms does not match the amount of labels",
                    QMessageBox::Ok, QMessageBox::Ok);
            return;
        }
        fl::LinguisticTerm::eMembershipFunction mf = fl::LinguisticTerm::MF_TRIANGULAR;
        switch (getUi().cbx_lterm->currentIndex()){
            case 0: mf = fl::LinguisticTerm::MF_TRIANGULAR;
            break;
            case 1: mf = fl::LinguisticTerm::MF_SHOULDER;
            break;
            case 2: mf = fl::LinguisticTerm::MF_TRAPEZOIDAL;
            break;
            default: fl::FuzzyException::Assert(FL_AT, false);
        }
        std::vector<std::string> std_labels;
        for (int i = 0 ; i < labels.size() ; ++i){
            std_labels.push_back(labels.at(i).toStdString());
        }
        getModel().createTerms(terms,mf,min, max,std_labels);
        done(QDialog::Accepted);
    }
}
