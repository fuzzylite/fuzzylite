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
 * Control.h
 *
 *  Created on: 18/12/2012
 *      Author: jcrada
 */

#ifndef CONTROL_H_
#define CONTROL_H_

#include <QtGui/QWidget>
#include "fl/qt/Viewer.h"

#include <fl/Headers.h>

namespace fl {
    namespace qt {

        class Control : public Viewer {
            Q_OBJECT

        protected slots:
            void updateInput(double value);
            void updateOutput();


        public:
            fl::Variable* variable;

            Control(QWidget* parent = NULL, Qt::WindowFlags f = 0);
            virtual ~Control();

          void setup(const fl::Variable* variable);

        };

    }
}
#endif /* CONTROL_H_ */
