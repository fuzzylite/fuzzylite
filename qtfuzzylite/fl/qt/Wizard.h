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
 * Wizard.h
 *
 *  Created on: 14/12/2012
 *      Author: jcrada
 */

#ifndef FLQT_WIZARD_H_
#define FLQT_WIZARD_H_

#include <QDialog>
#include <fl/Headers.h>

#include "ui_Wizard.h"

#include <vector>
#include <string>

namespace fl {
    namespace qt {

        class Wizard : public QDialog {
            Q_OBJECT

        protected slots:
            void autoGenerate(int);
        protected:
            void connect();
            void disconnect();

        public:
            std::string prefix;
            Ui::Wizard* ui;

            Wizard(QWidget* parent = NULL, Qt::WindowFlags = 0);
            ~Wizard();

            void setup(const std::string& prefix);

        };

    }
}
#endif /* FLQT_WIZARD_H_ */
