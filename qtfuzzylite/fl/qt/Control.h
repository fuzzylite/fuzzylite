/*
 * Control.h
 *
 *  Created on: 18/12/2012
 *      Author: jcrada
 */

#ifndef CONTROL_H_
#define CONTROL_H_

#include <QtGui/QWidget>
#include "ui/ui_Control.h"

#include <fl/Headers.h>

namespace fl {
    namespace qt {

        class Control: public QWidget {
        Q_OBJECT

        signals:
            void inputValueChanged();

        public slots:
             void updateOutputValue();

        protected slots:
             void onChangeSliderValue(int position);
             void onEditInputValue();

        protected:
             void connect();
             void disconnect();
             void resizeEvent(QResizeEvent* e);
             void focusInEvent(QFocusEvent* e);

        public:
            Ui::Control* ui;
            Control(QWidget* parent = NULL, Qt::WindowFlags f = 0);
            ~Control();

            void setup();
            void setup(InputVariable* input);
            void setup(OutputVariable* input);

        };

    } /* namespace qt */
} /* namespace fl */
#endif /* CONTROL_H_ */
