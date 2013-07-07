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
 * Variable.h
 *
 *  Created on: 11/12/2012
 *      Author: jcrada
 */

#ifndef FLQT_VARIABLE_H_
#define FLQT_VARIABLE_H_

#include <QDialog>

#include "ui_Variable.h"

#include <fl/Headers.h>

namespace fl {
    namespace qt {
        class Viewer;
        class Variable: public QDialog {
        Q_OBJECT

        protected slots:
            void onClickAddTerm();
            void onClickRemoveTerm();
            void onClickEditTerm();

            void onClickMoveUp();
            void onClickMoveDown();
            
            void onChangeMinRange(double);
            void onChangeMaxRange(double);
            
            void onClickTerm(QListWidgetItem* item);
            void onDoubleClickTerm(QListWidgetItem* item);

            void onSelectTerm();
            
            void onSelectDefuzzifier(int);
            
            void redraw();
            void showSelectedTerms();

            void accept();
            void reject();

        protected:
            void connect();

        public:
            Ui::Variable* ui;
            Viewer* viewer;
            fl::Variable* variable;

            enum VariableType {
                INPUT_VARIABLE, OUTPUT_VARIABLE
            };

            Variable(QWidget* parent = NULL, Qt::WindowFlags f = 0);
            virtual ~Variable();

            virtual void showEvent(QShowEvent* event);

            virtual void setup(VariableType type);

            virtual void edit(const InputVariable* variable);
            virtual void edit(const OutputVariable* variable);

        public slots:
            virtual void reloadModel();

        };

    } 
} 
#endif /* FLQT_VARIABLE_H_ */
