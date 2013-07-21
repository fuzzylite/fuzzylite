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
 * File:   ImEx.h
 * Author: jcrada
 *
 * Created on 6 July 2013, 9:06 PM
 */

#ifndef FL_QT_IMEX_H
#define	FL_QT_IMEX_H

#include <QDialog>
#include "ui_ImEx.h"
#include <fl/Headers.h>

namespace fl {
    namespace qt {

        class ImEx : public QDialog {
            Q_OBJECT
        protected slots:
            void onTextChanged();
        public:
            Ui::ImEx* ui;

            ImEx(QWidget* parent = NULL, Qt::WindowFlags f = 0);
            ~ImEx();

            void setup();

        };
    }
}

#endif	/* FL_QT_IMEX_H */

