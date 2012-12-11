/*
 * InputVariable.h
 *
 *  Created on: 11/12/2012
 *      Author: jcrada
 */

#ifndef FLQT_INPUTVARIABLE_H_
#define FLQT_INPUTVARIABLE_H_

#include "ui/ui_Variable.h"

namespace fl {
    namespace qt {

        class InputVariable {
        public:
            Ui::Variable* ui;
            InputVariable();
            virtual ~InputVariable();

            virtual void refresh();
        };

    } /* namespace qt */
} /* namespace fl */
#endif /* FLQT_INPUTVARIABLE_H_ */
