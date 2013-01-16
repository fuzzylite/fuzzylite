/*
 * Control.cpp
 *
 *  Created on: 18/12/2012
 *      Author: jcrada
 */

#include "fl/qt/Control.h"

#include <QtGui/QMessageBox>

#include "fl/qt/Model.h"
namespace fl {
    namespace qt {

        Control::Control(QWidget* parent, Qt::WindowFlags f) :
        Viewer(parent, f) {
        }

        Control::~Control() {
//            if (dynamic_cast<OutputVariable*> (variable)) {
//                //do nothing
//            } else if (dynamic_cast<InputVariable*> (variable)) {
//                QObject::disconnect(this, SIGNAL(valueChanged(double)),
//                        this, SLOT(updateInput(double)));
//            }
        }

        void Control::setup(const fl::Variable* model) {
            Viewer::setup(model);
            this->variable = const_cast<fl::Variable*>(model);

            if (dynamic_cast<OutputVariable*> (variable)) {
                ui->sld_x->setEnabled(false);
                ui->sbx_x->setVisible(false);
                ui->led_x->setVisible(true);
                ui->lbl_fuzzy_out->setVisible(true);
                ui->lbl_fuzzy->setVisible(false);
//                QObject::connect(this, SIGNAL(valueChanged(double)),
//                        this, SLOT(updateOutput()), Qt::QueuedConnection);
            } else if (dynamic_cast<InputVariable*> (variable)) {
                QObject::connect(this, SIGNAL(valueChanged(double)),
                        this, SLOT(updateInput(double)));
            }
        }

        void Control::updateInput(double value) {
            fl::InputVariable* inputVariable = dynamic_cast<fl::InputVariable*> (variable);
            if (inputVariable) {
                inputVariable->setInput(value);
            }
        }

        void Control::updateOutput() {
            fl::OutputVariable* outputVariable = dynamic_cast<fl::OutputVariable*> (variable);
            if (not outputVariable) {
                throw fl::Exception("[cast error] trying to cast OutputVariable", FL_AT);
            }

            scalar x = outputVariable->defuzzify();
            std::ostringstream oss;
            oss << std::setprecision(5) <<  x;
            ui->led_x->setText(QString::fromStdString(oss.str()));
            ui->sbx_x->setValue(x);
            refresh();
            draw(outputVariable->output());
            
            QString fuzzify;
            Accumulated* output = outputVariable->output();
            
            for (int i = 0; i < output->numberOfTerms(); ++i) {
                fuzzify += QString::number(output->getTerm(i)->membership(x), 'f', 2)
                        + "/" + QString::fromStdString(output->getTerm(i)->getName());

                if (i < output->numberOfTerms() - 1) fuzzify += " + ";
            }
            ui->lbl_fuzzy_out->setText("&#956;=" + fuzzify);
        }

    }
}
