/**
    This file is part of qtfuzzylite.

    qtfuzzylite is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    qtfuzzylite is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with qtfuzzylite.  If not, see <http://www.gnu.org/licenses/>.

    Juan Rada-Vilela, 01 February 2013
    jcrada@fuzzylite.com
**/

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
        Viewer(parent, f) { }

        Control::~Control() {
            //Exception on reset?
            //            if (dynamic_cast<InputVariable*> (variable)) {
            //                QObject::disconnect(this, SIGNAL(valueChanged(double)),
            //                        this, SLOT(updateInput(double)));
            //            }
        }

        void Control::setup(const fl::Variable* model) {
            Viewer::setup(model);
            this->variable = const_cast<fl::Variable*> (model);

            if (dynamic_cast<OutputVariable*> (variable)) {
                ui->sld_x->setEnabled(false);
                ui->sbx_x->setVisible(false);
                ui->led_x->setVisible(true);
                ui->lbl_fuzzy_out->setVisible(true);
                ui->lbl_fuzzy->setVisible(false);
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
            ui->led_x->setText(QString::number(x, 'f', fl::fuzzylite::decimals()));
            ui->sbx_x->setValue(x);
            refresh();
            if (not outputVariable->output()->isEmpty())
                draw(outputVariable->output());

            QString fuzzify = QString::fromStdString(outputVariable->fuzzify(x));
            ui->lbl_fuzzy_out->setText("&#956;=" + fuzzify);
        }

    }
}
