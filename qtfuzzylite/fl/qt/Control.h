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
            void crispValueChanged();

        protected slots:
            void onChangeSliderValue(int position);
            void onEditInputValue();

            void onPressSlider();
            void onReleaseSlider();

        public slots:
            void drawVariable();
            void drawOutputVariable();

        protected:
            void setup();
            void connect();
            void disconnect();
            void showEvent(QShowEvent*);
            void resizeEvent(QResizeEvent*);

        public:
            fl::Variable* variable;
            Ui::Control* ui;
            Canvas* canvas;
            
            Control(QWidget* parent = NULL, Qt::WindowFlags f = 0);
            ~Control();

            void setup(fl::Variable* variable);
            
            void setReadOnly(bool readOnly);
            bool isReadOnly() const;
            


        };

    }
}
#endif /* CONTROL_H_ */
