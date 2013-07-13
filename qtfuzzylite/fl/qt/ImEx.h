/* 
 * File:   ImEx.h
 * Author: jcrada
 *
 * Created on 6 July 2013, 9:06 PM
 */

#ifndef FL_QT_IMEX_H
#define	FL_QT_IMEX_H

#include <QDialog>
#include "ui_ImEx.h"
#include <fl/Headers.h>

namespace fl {
    namespace qt {

        class ImEx : public QDialog {
            Q_OBJECT
        protected slots:
            void onTextChanged();
        public:
            Ui::ImEx* ui;

            ImEx(QWidget* parent = NULL, Qt::WindowFlags f = 0);
            ~ImEx();

            void setup();

        };
    }
}

#endif	/* FL_QT_IMEX_H */

