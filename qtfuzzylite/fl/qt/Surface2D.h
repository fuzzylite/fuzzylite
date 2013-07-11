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

#include <fl/Headers.h>

namespace fl {
    namespace qt {

        class Surface2D : public QDialog {
            Q_OBJECT

        protected slots:
            void onChangeInputA(int);
            void onChangeInputB(int);
            void onChangeOutput(int);
            
            void onChangeSpinBox(double);

            void onClickInputs();

            void onClickOptions();
            void onOptionTriggered();


            void onClickMinimumColor();
            void onClickMaximumColor();

            void onClickGenerate();
            
            void onEngineChanged();

        protected:
            bool _swap;
            std::vector<std::vector<scalar> > _matrix;
            void generateLockingRange();
            void generateRangeFree();
            
            void updateWindowTitle();

        public:
            Ui::Surface2D* ui;
            Surface2D(QWidget* parent = 0, Qt::WindowFlags f = 0);
            ~Surface2D();

            void setup();

            class MouseTrackingFilter : public QObject {
            protected:
                Surface2D* _owner;
            public:

                MouseTrackingFilter(Surface2D* owner) : _owner(owner) {

                }

                bool eventFilter(QObject* object, QEvent* event) {
                    if (object == _owner->ui->canvas and
                            event->type() == QEvent::MouseMove) {
                        
                        return true;
                    }
                    return false;
                }
            };

        };
    }

}

#endif	/* FLQT_SURFACE2D_H */

