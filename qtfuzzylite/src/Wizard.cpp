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
 * Wizard.cpp
 *
 *  Created on: 14/12/2012
 *      Author: jcrada
 */

#include "fl/qt/Wizard.h"

#include <QMessageBox>

namespace fl {
    namespace qt {

        Wizard::Wizard(QWidget* parent, Qt::WindowFlags f)
        : QDialog(parent, f), ui(new Ui::Wizard) {
            setWindowFlags(Qt::Dialog
                    | Qt::WindowSystemMenuHint
                    | Qt::WindowCloseButtonHint
                    | Qt::WindowStaysOnTopHint);
        }

        Wizard::~Wizard() {
            disconnect();
            delete ui;
        }

        void Wizard::setup(const std::string& prefix) {
            this->prefix = prefix;
            ui->setupUi(this);
            QRect scr = parentWidget()->geometry();
            move(scr.center() - rect().center());
            
            autoGenerate(ui->sbx_copies->value());
            
            connect();
        }

        void Wizard::connect() {
            QObject::connect(ui->sbx_copies, SIGNAL(valueChanged(int)),
                    this, SLOT(autoGenerate(int)));
        }

        void Wizard::disconnect() {
            QObject::connect(ui->sbx_copies, SIGNAL(valueChanged(int)),
                    this, SLOT(autoGenerate(int)));
        }

        void Wizard::autoGenerate(int copies) {
            if (not ui->chx_auto_generate->isChecked()) return;
            
            ui->ptx_terms->clear();
            char name = 'A';
            std::string separator = prefix.empty() ? "" : ".";
            for (int i = 0; i < copies; ++i) {
                ui->ptx_terms->appendPlainText(
                        QString::fromStdString(prefix + separator) + name++);
            }
        }
    }
}
