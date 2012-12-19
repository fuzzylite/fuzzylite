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

        class Control : public QWidget {
            Q_OBJECT

        signals:
            void inputValueChanged();

        protected slots:
            void onChangeSliderValue(int position);
            void onEditInputValue();
            void onChangeToolBoxPage(int);
            void onClickShowMoreInformation(int);
            void refreshModel();
            void onOutputValueChanged();


        protected:
            void setup();
            void connect();
            void disconnect();
            void showEvent(QShowEvent* e);
            void resizeEvent(QResizeEvent* e);
            void focusInEvent(QFocusEvent* e);
            
        public:
            fl::InputVariable* inputVariable;
            fl::OutputVariable* outputVariable;
            Ui::Control* ui;
            Control(QWidget* parent = NULL, Qt::WindowFlags f = 0);
            ~Control();

            void setup(fl::Variable* variable);


        };

    } /* namespace qt */
} /* namespace fl */
#endif /* CONTROL_H_ */
