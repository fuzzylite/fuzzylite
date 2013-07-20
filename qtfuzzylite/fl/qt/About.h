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
 * File:   About.h
 * Author: jcrada
 *
 * Created on 25 January 2013, 6:00 PM
 */

#ifndef FL_ABOUT_H
#define	FL_ABOUT_H

#include <QDialog>

#include "ui_About.h"

namespace fl {
    namespace qt {

        class About : public QDialog {
            Q_OBJECT

        protected slots:
            void onChangeSliderGenerosity(int);
            void onChangeComboGenerosity(int);


        protected:
            void connect();
            void disconnect();

            void changeDonation();

            std::vector<std::string> _donationTerm;
            std::vector<std::string> _donationValue;


        public:
            Ui::About* ui;
            About(QWidget* parent = NULL, Qt::WindowFlags f = 0);
            ~About();

            void setup();
            void accept();
        };
    }
}
#endif	/* FL_ABOUT_H */

