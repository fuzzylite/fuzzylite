/* 
 * File:   About.h
 * Author: jcrada
 *
 * Created on 25 January 2013, 6:00 PM
 */

#ifndef FL_ABOUT_H
#define	FL_ABOUT_H

#include <QtGui/QDialog>

#include "ui/ui_About.h"

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
            
            std::vector<std::string> _generosityTerm;
            std::vector<int> _generosityValue;


        public:
            Ui::About* ui;
            About(QWidget* parent = NULL, Qt::WindowFlags f = 0);
            ~About();

            void setup();
        };
    }
}
#endif	/* FL_ABOUT_H */

