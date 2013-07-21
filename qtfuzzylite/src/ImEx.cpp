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

    Juan Rada-Vilela, 21 July 2013
    jcrada@fuzzylite.com
**/

/* 
 * File:   ImEx.cpp
 * Author: jcrada
 *
 * Created on 6 July 2013, 9:09 PM
 */

#include "fl/qt/ImEx.h"

#include <QPushButton>
#include <QDialogButtonBox>

namespace fl {
    namespace qt {

        ImEx::ImEx(QWidget* parent, Qt::WindowFlags f) :
        QDialog(parent, f), ui(new Ui::ImEx) {
            setWindowFlags(Qt::WindowSystemMenuHint
                    | Qt::WindowCloseButtonHint
                    | Qt::WindowMaximizeButtonHint);
            setModal(true);
        }

        ImEx::~ImEx() {
            delete ui;
        }

        void ImEx::setup() {
            ui->setupUi(this);
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
            QObject::connect(ui->pte_code, SIGNAL(textChanged()),
                    this, SLOT(onTextChanged()));
        }

        void ImEx::onTextChanged() {
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(not
                    ui->pte_code->toPlainText().trimmed().isEmpty());
        }


    }
}