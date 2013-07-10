/* 
 * File:   Surface2D.h
 * Author: jcrada
 *
 * Created on 10 July 2013, 10:09 PM
 */

#ifndef FLQT_SURFACE2D_H
#define	FLQT_SURFACE2D_H

#include <QDialog>
#include "ui_Surface2D.h"

namespace fl {
    namespace qt {

        class Surface2D : public QDialog {
            Q_OBJECT

        protected slots:
            void onChangeInputA(int);
            void onChangeInputB(int);
            void onChangeOutput(int);
            
            void onClickInputs();
            
            void onClickSwapAxes();
            
            void onClickMinimumColor();
            void onClickMaximumColor();
            
            void onClickGenerate();
            
        public :
            Ui::Surface2D* ui;
            Surface2D(QWidget* parent= 0, Qt::WindowFlags f = 0);
            ~Surface2D();
            
            void setup();
            
        };
    }

}

#endif	/* FLQT_SURFACE2D_H */

