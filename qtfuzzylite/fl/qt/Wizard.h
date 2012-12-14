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

#include "ui/ui_Wizard.h"

namespace fl {
    namespace qt {

        class Wizard: public QDialog {
        Q_OBJECT

        protected slots:
            void onClickGenerate();
            void reject();
        protected:
            virtual void connect();
            virtual void disconnect();

        public:
            std::vector<fl::Term*> terms;
            Ui::Wizard* ui;

            Wizard(QWidget* parent = NULL, Qt::WindowFlags = 0);
            virtual ~Wizard();

            virtual void setup();

        };

    } /* namespace qt */
} /* namespace fl */
#endif /* FLQT_WIZARD_H_ */
