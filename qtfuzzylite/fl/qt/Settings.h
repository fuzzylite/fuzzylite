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
 * Settings.h
 *
 *  Created on: 11/12/2012
 *      Author: jcrada
 */

#ifndef FLQT_SETTINGS_H_
#define FLQT_SETTINGS_H_

#include <QDialog>
#include <QWidget>

#include "ui_Settings.h"

#include <fl/Headers.h>

#include <vector>


namespace fl {
    namespace qt {

        class Settings : public QDialog {
            Q_OBJECT

        protected slots:
            virtual void onChangeTNorm(int);
            virtual void onChangeSNorm(int);
            virtual void onChangeActivation(int);
            virtual void onChangeAccumulation(int);
            virtual void onChangeDefuzzifier(int);
            virtual void onChangeDivisions(int);
            
            virtual void onChangeHedgeSelection(int);

        protected:

            virtual void connect();
            virtual void disconnect();
            
            virtual int indexOfTnorm(const std::string& tnorm);
            virtual int indexOfSnorm(const std::string& snorm);
            virtual int indexOfDefuzzifier(const std::string& defuzzifier);
            
            virtual void showEvent(QShowEvent*);
            
        public:
            Ui::Settings* ui;

            Settings(QWidget* parent = NULL, Qt::WindowFlags f = 0);
            virtual ~Settings();

            virtual void setup();

            virtual void applyDefaults();
            virtual void loadFromModel();

            static void main();
        };
    }
}
#endif /* FLQT_SETTINGS_H_ */
