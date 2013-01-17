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
