/*
 * Wizard.h
 *
 *  Created on: 14/12/2012
 *      Author: jcrada
 */

#ifndef FLQT_WIZARD_H_
#define FLQT_WIZARD_H_

#include <QtGui/QDialog>
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
